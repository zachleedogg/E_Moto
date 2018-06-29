#include "stdafx.h"

/**
 * Enumerates all available COM and MCP2221A I2C ports available on the 
 * system.
 *
 * @param *comStrings Pointer to a caller allocated TCHAR buffer to write 
 *                    the enumerated port strings to. The list is written 
 *                    as an array of null terminated strings with the list 
 *                    being terminated by a null string.
 * 
 * @param bufTCHARCount Size of the comStrings buffer, in TCHARs, to 
 *                      maximally write. If the list of devices requires more 
 *                      space than provided, the last complete printable 
 *						string is placed in the buffer and the list is null 
 *						terminated early. The return value will be negative
 *						who's magnitude indicates how big the comStrings 
 *						buffer must have been to retrieve all strings 
 *						without early termination.
 *
 * @return On success: Number characters written to the comStrings output 
 *                     buffer, excluding the final list null terminator (but 
 *                     including each list item null terminator)
 *         On failure: Number of TCHARs of buffer space required to receive 
 *                     all port strings, expressed as a negative value. The 
 *					   comStrings buffer contains only a partial list of 
 *                     available ports, but each entry is still null 
 *                     terminated with the list being also null terminated 
 *					   (assuming bufTCHARCount is >= 1; 0 is returned if 
 *				       bufTCHARCount is 0 to emulate a passing result with no 
 *                     entries to reduce risk of an invalid pointer dereference 
 *                     occuring since we can't write a null terminator to a 0 
 *                     character buffer).
 *
 */
int EnumCOMPorts(TCHAR *comStrings, int bufTCHARCount)
{
	TCHAR	strBuf[32768] = L"";
	int		strLen;
	int		strCopied;
	int		strNotCopied;
	int		ret;

	if((comStrings == NULL) || (bufTCHARCount <= 0))
		return 0;

	comStrings[0] = L'\0';
	ret = -1;
	strCopied = 0;
	strNotCopied = 0;

	DWORD dwSize = QueryDosDevice(NULL, strBuf, sizeof(strBuf)/sizeof(strBuf[0]));
	for(DWORD j = 0; j < dwSize; j++)
	{
		strLen = _tcsnlen(&strBuf[j], sizeof(strBuf)/sizeof(strBuf[0]) - j);
		if(_tcsnicmp(&strBuf[j], L"COM", 3) == 0)
		{
			ret -= 4 + 1 + strLen;
			if(bufTCHARCount >= strLen + 2)
			//if(bufTCHARCount >= 4 + strLen + 2)
			{
				//_tcscpy_s(comStrings, bufTCHARCount, L"\\\\.\\");		// Add \\.\ so COM10+ can be accessed
				//comStrings += 4;
                //bufTCHARCount -= 4;
                //strCopied += 4;
				_tcscpy_s(comStrings, bufTCHARCount, &strBuf[j]);
				comStrings += strLen + 1;			// +1 for string's null terminator
                bufTCHARCount -= strLen + 1;
				comStrings[0] = L'\0';				// Empty string list terminator
                strCopied += strLen + 1;
			}
			else
			{
				strNotCopied++;
			}
		}
        else if(_tcsnicmp(&strBuf[j], L"\\\\.\\COM", 7) == 0)
        {
            j += 4;         // Strip leading \\.\ in return string
            strLen -= 4;
			
            ret -= 1 + strLen;
			if(bufTCHARCount >= strLen + 2)
			{
				_tcscpy_s(comStrings, bufTCHARCount, &strBuf[j]);
				comStrings += strLen + 1;			// +1 for string's null terminator
                bufTCHARCount -= strLen + 1;
				comStrings[0] = L'\0';				// Empty string list terminator
				strCopied += strLen + 1;
			}
			else
			{
				strNotCopied++;
			}
        }
		j += strLen;
	}

	if(strNotCopied == 0)
		return strCopied;

	return ret;
}