#include "stdafx.h"

int DoI2CRead(HANDLE hComm, unsigned char *buf, int bufSize, int init);
int DoI2CWrite(HANDLE hComm, unsigned char *buf, int writeSize, int init);


// Sending data from the PC to the PIC (i.e. Reading from the pipe + writing to the COM handle)
DWORD PipeOutToCOMThreadProc(LPVOID comHandle)
{
	HANDLE	hComm = *((HANDLE*)comHandle);
	HANDLE	hPipe;
	BYTE	data[65536];
	DWORD	dwSize, dwActual;
	BYTE	*outToCOMPtr;
	DWORD	bytesToNextFrameHeader;
	DWORD	dwOffset;
	double  activityTime;
    OVERLAPPED comEvent;	// Event object for use by WaitCommEvent() or ReadFile()/WriteFile() on hComm UART
    int     holdOffReads = 0;
	
	// Set up overalapped event object for WaitCommEvent()
	comEvent.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);   // auto-reset event, not signaled, not named
	comEvent.Internal = 0;
	comEvent.InternalHigh = 0;
	comEvent.Offset = 0;
	comEvent.OffsetHigh = 0;


	if(!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL))	// Boost thread priority by 1 point
	{
		DWORD dwError = GetLastError();
		writeLogError(dwError, L"\n%10.6f: SetThreadPriority() in PipeOutToCOMThreadProc() returned: %d\n            ", GetTimeStamp(), dwError);
	} 

	if(i2cComNumber)
	{
		DoI2CRead(hComm, data, sizeof(data), 1);	// Initialize state variables in DoI2CRead - not an actual read
		DoI2CWrite(hComm, data, 0, 1);				// Initialize state variables in DoI2CWrite - not an actual write
	}


	while(1)
	{
		// Create a pipe
		//_snwprintf_s((TCHAR*)data, sizeof(data)/sizeof(TCHAR), _TRUNCATE, _T("\\\\.\\pipe\\ezbl_pipe_out_to_com_%d"), GetCurrentProcessId());
		_snwprintf_s((TCHAR*)data, sizeof(data)/sizeof(TCHAR), _TRUNCATE, _T("\\\\.\\pipe\\ezbl_pipe_out_to_com"));
		hPipe = CreateNamedPipe((TCHAR*)data, PIPE_ACCESS_INBOUND, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT | PIPE_ACCEPT_REMOTE_CLIENTS, 8, 0, 8192, 0, NULL);
		if(hPipe == INVALID_HANDLE_VALUE)
		{
			DWORD dwError = GetLastError();
			writeLogError(dwError, L"\n%10.6f: Could not create %s handle in PipeOutToCOMThreadProc(): %d\n            ", GetTimeStamp(), (TCHAR*)data, dwError);
			CloseHandle(comEvent.hEvent);
			return dwError;
		}

		// Wait for someone to connect to the pipe. ConnectNamedPipe() is blocking since not doing overlapped IO
		if(ConnectNamedPipe(hPipe, NULL) == 0)
		{
			DWORD dwError = GetLastError();
			writeLogError(dwError, L"\n%10.6f: ConnectNamedPipe() return in PipeOutToCOMThreadProc(): %d\n            ", GetTimeStamp(), dwError);
			DisconnectNamedPipe(hPipe);
			CloseHandle(hPipe);
			Sleep(1);
			continue;
		}
		writeLogf(L"\n%10.6f: \\\\.\\pipe\\ezbl_pipe_out_to_com connected\n", GetTimeStamp());


		bytesToNextFrameHeader = 0;	// For I2C RX stream polling
		dwOffset = 0;
		
		// Forward all incomming pipe data
		while(1)
		{
			// Check if there is data in the pipe ready for transmission to the remote node
			if(PeekNamedPipe(hPipe, NULL, 0, NULL, &dwSize, NULL) == 0)
			{
				DWORD dwError = GetLastError();
				writeLogError(dwError, L"\n%10.6f: PeekNamedPipe() return in PipeOutToCOMThreadProc(): %d\n            ", GetTimeStamp(), dwError);
				if(dwError != 109u)	// 109 == "The pipe has been ended.", natural termination so don't delay process termination
					Sleep(1);
				break;
			}
			if(dwSize == 0)	// No data waiting in the pipe, sleep for data to arrive
			{
				Sleep(1);
				// I2C requires us to address the target node before it can send us RX data, so let's poll the target
				// The MCP2221 APIs are accessed in this thread so we don't have to worry so much about multithreading exclusions
				if(i2cComNumber)    // I2C for >= 1
				{
                    if(holdOffReads <= 0)
                    {
                        holdOffReads = DoI2CRead(hComm, data, sizeof(data), 0);
                        if(holdOffReads < 0)
						    break;
                    }
                    else
                        holdOffReads--;
				}
				continue;
			}

			// Data available; limit transfer chunk to size of actual buffer
			if(dwSize > sizeof(data))
				dwSize = sizeof(data);
			
			// Read a chunk from the pipe
			if(ReadFile(hPipe, data, dwSize, &dwActual, NULL) == 0)
			{
				DWORD dwError = GetLastError();
				writeLogError(dwError, L"\n%10.6f: ReadFile() return in PipeOutToCOMThreadProc() when reading pipe: %d\n            ", GetTimeStamp(), dwError);
				Sleep(1);
				break;
			}
			if(dwActual != dwSize)
			{
				writeLogf(L"\n%10.6f: ReadFile() returned %d bytes in PipeOutToCOMThreadProc() while expecting %d bytes\n", GetTimeStamp(), dwActual, dwSize);
			}

			if(dwActual == 0u)
			{
				continue;
			}

			activityTime = GetTimeStamp();

			// Diagnostic display/log what we are about to transmit
			if(displayTXRXData)
			{
				WriteLogData(activityTime, L"TX", dwOffset, data, dwActual);
				dwOffset += dwActual;
			}

			// Write the chunk to the COM
			if(i2cComNumber == 0)	// UART COM port mode
			{
				outToCOMPtr = data;
				dwSize = dwActual;
				while(1)
				{
					if(WriteFile(hComm, outToCOMPtr, dwSize, &dwActual, &comEvent) == 0)
					{
						DWORD dwError = GetLastError();
						if(dwError != ERROR_IO_PENDING)
						{
							writeLogError(dwError, L"\n%10.6f: WriteFile() return in PipeOutToCOMThreadProc() when writing to com port: %d\n            ", GetTimeStamp(), dwError);
						}
					}

					if(!GetOverlappedResult(hComm, &comEvent, &dwActual, TRUE))
					{
						DWORD dwError = GetLastError();
						writeLogError(dwError, L"\n%10.6f: GetOverlappedResult() returned %d\n", GetTimeStamp(), dwError);
					}
					if(dwActual == dwSize)
					{
						break;
					}

					// Couldn't send everything
					writeLogf(L"\n%10.6f: WriteFile() of %d bytes to com port in PipeOutToCOMThreadProc() only wrote %d bytes\n", GetTimeStamp(), dwSize, dwActual);
					
					// See what the problem might be
					DWORD dwCOMErrors;
					COMSTAT comstat;
					if(ClearCommError(hComm, &dwCOMErrors, &comstat) == 0)
					{
						DWORD dwError = GetLastError();
						writeLogError(dwError, L"\n%10.6f: ClearCommError() return in PipeOutToCOMThreadProc(): %d, driver flags = 0x%08X\n            ", GetTimeStamp(), dwError, dwCOMErrors);
					}
					
					dwSize -= dwActual;
					outToCOMPtr += dwActual;
				}
			}
			else	// I2C communications mode - write to MCP2221 DDL
			{
                int ret = DoI2CWrite(hComm, data, dwActual, 0);
                if(ret < 0)
                    break;
                else if(ret > 0)
                    holdOffReads = 0;
			}
		}

		// Close the pipe (about to regenerate it)
		DisconnectNamedPipe(hPipe);
		CloseHandle(hPipe);

		// Start the comm loop over again if the -persist command line option was specified
		if(!persistAfterPipeClose)
			break;
	}
	
	CloseHandle(comEvent.hEvent);
	return 0;
}


int DoI2CRead(HANDLE hComm, unsigned char *buf, int bufSize, int init)
{
	static int i2cReadSize;
	static BOOLEAN weOwnMutex;
	static HANDLE hI2CNotifyMutex = NULL;
	DWORD dwError;
	int ret;
    unsigned char localBuf[9];
    int dataReady;
    int outIndex = 0;


	if(init)
	{
		COMInDataLength = 0;
		i2cReadSize = 0;

		hI2CNotifyMutex = OpenMutex(SYNCHRONIZE, FALSE, L"Local\\ezbl_i2c_thread_to_thread_notify_mutex");
		if(hI2CNotifyMutex == NULL)
		{
			dwError = GetLastError();
			writeLogError(dwError, L"\n%10.6f: OpenMutex() in PipeOutToCOMThreadProc() returned: %d\n            ", GetTimeStamp(), dwError);
		}
		ret = WaitForSingleObject(hI2CNotifyMutex, 75);
		weOwnMutex = TRUE;
		if(ret != WAIT_OBJECT_0)
		{
			weOwnMutex = FALSE;
			writeLogf(L"\n%10.6f: PipeOutToCOMThreadProc() could not obtain initial sync ownership for hI2CNotifyMutex: %d.\n", GetTimeStamp(), ret);
		}
		return 0;
	}


	if(!weOwnMutex)
	{
		ret = WaitForSingleObject(hI2CNotifyMutex, 0);
		if(ret != WAIT_OBJECT_0)
			return 0;
		weOwnMutex = TRUE;
	}

	if(COMInDataLength)
	{
		writeLogf(L"\n%10.6f: COMInDataLength == %d, not zero as it should be.\n", GetTimeStamp(), COMInDataLength);
		return 0;
	}

    i2cReadSize = sizeof(localBuf);
    if(i2cReadSize >= bufSize)
        i2cReadSize = bufSize + 1;  // +1 offset for initial length byte

    ret = Mcp2221_I2cRead(hComm, i2cReadSize, i2cSlaveAddress, 1, localBuf);
    if(ret)
    {
	    writeLogf(L"\n%10.6f: Mcp2221_I2cRead() return: %d (%s).\n", GetTimeStamp(), ret, GetMCP2221ErrorString(ret));
	    if(ret == E_ERR_ADDRESS_NACK)
	    {	// Address NACK means the target node did not respond. Slave address may be wrong or the target isn't running I2C RX code right now.
		    writeLogf(L"\n%10.6f: Mcp2221_I2cRead() return: %d (%s).\n", GetTimeStamp(), ret, GetMCP2221ErrorString(ret));
		    CloseHandle(hI2CNotifyMutex);
		    return -1;
	    }
	    ret = Mcp2221_I2cCancelCurrentTransfer(hComm);
	    if(ret)
	    {
		    writeLogf(L"\n%10.6f: Mcp2221_I2cCancelCurrentTransfer() return: %d (%s).\n", GetTimeStamp(), ret, GetMCP2221ErrorString(ret));
	    }
        return 0;
    }
    for(int i = 0; i < i2cReadSize; i++)
    {
        dataReady = (unsigned int)localBuf[i];
        if(dataReady > i2cReadSize - 1 - i)
        {
            dataReady = i2cReadSize - 1 - i;
        }
        while(dataReady--)
        {
            COMInData[outIndex++] = localBuf[++i];
        }
    }
    if(outIndex)
    {
	    COMInDataLength = outIndex;
        weOwnMutex = FALSE;
        if(ReleaseMutex(hI2CNotifyMutex) == 0u)
        {
	        weOwnMutex = TRUE;
	        dwError = GetLastError();
	        writeLogError(dwError, L"\n%10.6f: ReleaseMutex() in PipeOutToCOMThreadProc() returned: %d\n            ", GetTimeStamp(), dwError);
        }
    }
    return outIndex;
}



int DoI2CWrite(HANDLE hComm, unsigned char *buf, int writeSize, int init)
{
	int ret;
	int cancelRet;
	int i;

	if(init)				// Nothing to do
		return 0;

	for(i = 0; i < 5u; i++)	// Retry up to 5 times and give up if we constantly get timeouts (probably ~100ms)
	{
		ret = Mcp2221_I2cWrite(hComm, writeSize, i2cSlaveAddress, 1, buf);
		if(ret == 0)
			return writeSize;

		writeLogf(L"\n%10.6f: MCP2221 Mcp2221_I2cWrite() returned code %d (%s).\n", GetTimeStamp(), ret, GetMCP2221ErrorString(ret));
		if(ret == E_ERR_ADDRESS_NACK)
		{	// Address NACK means the target node did not respond. Slave address may be wrong or the target isn't running I2C RX code right now.
			return -1;
		}
		if(ret == E_ERR_TIMEOUT)
		{	// Timeout - remote node could be holding SCL low while busy erasing flash page or something long running. MCP2221 doesn't let as adjust what defines a timeout, so we need to be careful not to lose data.
			cancelRet = Mcp2221_I2cCancelCurrentTransfer(hComm);
			if(cancelRet)
			{
				writeLogf(L"\n%10.6f: Mcp2221_I2cCancelCurrentTransfer() return: %d (%s).\n", GetTimeStamp(), cancelRet, GetMCP2221ErrorString(cancelRet));
			}
		}
	}

	return -1;
}