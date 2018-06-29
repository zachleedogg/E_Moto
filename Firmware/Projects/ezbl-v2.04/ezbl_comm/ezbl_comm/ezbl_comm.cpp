// ezbl_comm.cpp : Defines the main() entry point for the console application.

#include "stdafx.h"


static LARGE_INTEGER	freq;
static LARGE_INTEGER	timeStart;
static volatile char	*logFIFO;
static DWORD			logFIFOSize = 131072;	// 128K bytes (64K wide characters)
volatile DWORD			*headIndex;
volatile DWORD			*tailIndex;
FILE					*logFile = NULL;
FILE					*rxBinLogFile = NULL;

int						i2cComNumber = 0;	// 0 means not using I2C interface; 1 means MCP2221 device 1 I2C output, 2 means MCP2221 device 2 I2C output, etc.
static int				comBaudRate;
int						i2cSlaveAddress;
BOOL					persistAfterPipeClose = FALSE;
BOOL					displayTXRXData = FALSE;
volatile BYTE		    COMInData[4096];	// Transfer buffer for passing RX data from main thread to the dedicated Pipe writing thread
volatile DWORD			COMInDataLength;



int _tmain(int argc, TCHAR* argv[])
{
	HANDLE	hComm = INVALID_HANDLE_VALUE;
	TCHAR	*comFile = NULL;
    TCHAR   comFileBuf[_MAX_PATH+4];
	TCHAR	*artifactFile = NULL;
	TCHAR   *baudString = NULL;
	TCHAR   *slaveAddressString = NULL;
	TCHAR	*settings = NULL;
	TCHAR	strBuf[32768] = L"";
	DWORD	pid;
	int		ret;
	HANDLE	hOneEZBLCommMutex;
	HANDLE  hI2CNotifyMutex = NULL;
	DWORD	COMToPipeThreadID;
	DWORD	PipeToCOMThreadID;
	DWORD	dwError;
	DWORD	waitReturn;
	HANDLE	waitHandles[3]; // PIC_to_PC_thread, PC_to_PIC_thread handles + log data ready semaphore


	logFile = stdout;

	// Get the frequency of the high performance timeStamping counter and a starting offset value
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&timeStart);

	// Allocate shared thread memory in uncached memory for passing output log or error data back to this thread for a single threaded write to disk or stdout/stderr
	logFIFO = (volatile char *)VirtualAlloc(NULL, logFIFOSize+16, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE | PAGE_NOCACHE);	// Allocate uncached memory for passing log data between threads; +16 for a head and tail indexes stuffed at the top
	if(logFIFO == NULL)
	{
		logFIFOSize = 0;
		dwError = GetLastError();
		wprintf(L"\nCould not allocate memory for logging. Logging feature disabled.\n");

		headIndex = (volatile DWORD*)&tailIndex;	// Assign to dummy overlapping locations so *headIndex always == *tailIndex and won't be a null pointer
		tailIndex = (volatile DWORD*)&tailIndex;
		*headIndex = 0;
	}
	else
	{
		headIndex = ((volatile DWORD*)&logFIFO[logFIFOSize+0]);				// Set address for headIndex pointer
		tailIndex = ((volatile DWORD*)&logFIFO[logFIFOSize+sizeof(DWORD)]);	// Set address for tailIndex pointer
		*headIndex = 0;				// Head CHAR Index
		*tailIndex = 0;				// Tail CHAR Index
	}

	// Decode command line arguments
	for(int i = 1; i < argc; i++)
	{
		// Copy the argument to a temporary strBuf and convert to all lowercase
		_tcscpy_s(strBuf, sizeof(strBuf)/sizeof(strBuf[0]), argv[i]);
		_tcslwr_s(strBuf, sizeof(strBuf)/sizeof(strBuf[0]));

		// Decode option parameter
        if(memcmp(strBuf, L"-enum", sizeof(L"-enum") - sizeof(L"")) == 0)
		{
            int dwSize;
            int devCount = 0;
            if(memcmp(strBuf, L"-enum=all", sizeof(L"-enum=all")) == 0)
			    dwSize = (int)QueryDosDevice(NULL, strBuf, sizeof(strBuf)/sizeof(strBuf[0]));
            else
		        dwSize = EnumCOMPorts(strBuf, sizeof(strBuf)/sizeof(strBuf[0]));
            for(int j = 0; j < dwSize; j++)
            {
                _tprintf(L"%s%s", (devCount++ ? L"\n" : L""), &strBuf[j]);                  // Supress final newline when listing
		        j += _tcsnlen(&strBuf[j], sizeof(strBuf)/sizeof(strBuf[0]) - j);
            }

            return 0;
		}
		else if(memcmp(strBuf, L"-artifact=", sizeof(L"-artifact=")-sizeof(L"")) == 0)	// Not currently used; reserved for future standalone use without ezbl_tools.jar support
		{
			artifactFile = &argv[i][10];
		}
		else if(memcmp(strBuf, L"-com=", sizeof(L"-com=")-sizeof(L"")) == 0)
		{
			comFile = &argv[i][5];

			if(_tcscmp(&strBuf[5], L"i2c") == 0)
			{
				i2cComNumber = 1;
			}
            else if(_tcsnicmp(comFile, L"COM", 3) == 0)   // For "-com=COM..." prepend "\\.\" so we can access COM ports above 9
            {
                _tcscpy_s(&comFileBuf[0], sizeof(comFileBuf)/sizeof(comFileBuf[0]), L"\\\\.\\");
                _tcscpy_s(&comFileBuf[4], sizeof(comFileBuf)/sizeof(comFileBuf[0])-4, &argv[i][5]);
                comFile = comFileBuf;
            }
			if((i2cComNumber == 1) && (_tcslen(comFile) >= 4))	// Check for extra characters, and if present, decode as a device number in case if there are more than 1 MCP2221 devices attached to the system
			{
				i2cComNumber = _tcstol(&comFile[3], L'\0', 10);
			}
		}
		else if(memcmp(strBuf, L"-baud=", sizeof(L"-baud=")-sizeof(L"")) == 0)
		{
			baudString = &argv[i][6];
		}
		else if(memcmp(strBuf, L"-slave_address=", sizeof(L"-slave_address=")-sizeof(L"")) == 0)
		{
			slaveAddressString = &argv[i][15];
		}
		else if(memcmp(strBuf, L"-persist", sizeof(L"-persist")-sizeof(L"")) == 0)
		{
			persistAfterPipeClose = TRUE;
		}
		else if(memcmp(strBuf, L"-log", sizeof(L"-log")-sizeof(L"")) == 0)
		{
			displayTXRXData = TRUE;
			if(memcmp(strBuf, L"-log=", sizeof(L"-log=")-sizeof(L"")) == 0)
			{
				TCHAR *logFileName = &argv[i][5];
                _tfopen_s(&logFile, logFileName, L"w");
				if(logFile == NULL)
				{
					logFile = stdout;
					_fwprintf_p(logFile, L"Unable to open log file: %s\n", logFileName);
				}
			}
		}
		else if(memcmp(strBuf, L"-rx_bin_log", sizeof(L"-rx_bin_log")-sizeof(L"")) == 0)
		{
			if(memcmp(strBuf, L"-rx_bin_log=", sizeof(L"-rx_bin_log=")-sizeof(L"")) == 0)
			{
				TCHAR *logFileName = &argv[i][12];
				rxBinLogFile = _tfsopen(logFileName, L"w", _SH_DENYWR);
				if(rxBinLogFile == NULL)
				{
					_fwprintf_p(logFile, L"Unable to open binary RX log file: %s\n", logFileName);
				}
			}
		}
		else
		{
			_fwprintf_p(logFile, L"Unrecognized option: %s. Ignoring.\n", argv[i]);
			continue;
		}
	}
	DWORD dateLen = GetDateFormat(LOCALE_USER_DEFAULT, 0, NULL, NULL, strBuf, sizeof(strBuf)/sizeof(strBuf[0]));
	TCHAR *timeStrPtr = &strBuf[dateLen];
	GetTimeFormat(LOCALE_USER_DEFAULT, 0, NULL, NULL, timeStrPtr, sizeof(strBuf)/sizeof(strBuf[0]) - dateLen - 1);
	writeLogf(L"ezbl_comm.exe communications log started at %s %s\n    ", strBuf, timeStrPtr);
	for(int i = 0; i < argc - 1; i++)	// Write command line parameters
	{
		writeLogf(L"%s ", argv[i]);
	}
	writeLogf(L"%s\n\n", argv[argc-1]);


	hOneEZBLCommMutex = CreateMutex(NULL, FALSE, L"ezbl_comm.exe already running mutex");
	if(WaitForSingleObject(hOneEZBLCommMutex, 25) != WAIT_OBJECT_0)
	{
		// Wait for the other process to see the signal that it should close and if it does, we can then take over
		if(WaitForSingleObject(hOneEZBLCommMutex, 50) != WAIT_OBJECT_0)
		{
			CloseHandle(hOneEZBLCommMutex);
			writeLogf(L"\n%10.6f: Prior instance of ezbl_comm.exe is already running.\n", GetTimeStamp());
			FlushLog();
			if(logFIFO)
				VirtualFree((VOID*)logFIFO, 0, MEM_RELEASE);
			_fcloseall();
			return -3;  // Prior instance of ezbl_comm.exe already running. Be sure and close the prior process before proceeding.
		}
	}

	
	pid = GetCurrentProcessId();
	writeLogf( 
		L"Usage: ezbl_comm.exe -com=COMx [-baud=x] [-slave_address=x] [-persist] [-log]\n"
		L"\n"
		L"    Opens a communications port and bridges read and write data to two \n"
		L"    unidirectional named pipes. The pipe names are: \n"
		L"        \\\\.\\pipe\\ezbl_pipe_in_from_com\n"
		L"            and\n"
		L"        \\\\.\\pipe\\ezbl_pipe_out_to_com\n"
		//L"        \\\\.\\pipe\\ezbl_pipe_in_from_com_{PID}\n"
		//L"            and\n"
		//L"        \\\\.\\pipe\\ezbl_pipe_out_to_com_{PID}\n"
		//L"            where {PID} is the Process ID number for this instance. For \n"
		//L"            this invocation, the pipe names are:\n"
		//L"        \\\\.\\pipe\\ezbl_pipe_in_from_com_%d\n"
		//L"        \\\\.\\pipe\\ezbl_pipe_out_to_com_%d\n"
		L"\n"
		L"Parameters:\n"
		L"    -com=COMx\n"
		L"        Communications port to open. Can be for example, COM1, COM2, COM11, or\n"
		L"        I2C for MCP2221.\n"
		L"    -baud=x\n"
		L"        Baud rate, or physical bit rate of the communications medium. Example: \n"
		L"        115200, 460800, 400000. When unspecified, a default of 115200 baud is\n"
		L"        used for UARTs and 400000 baud for I2C.\n"
		L"    -slave_address=x\n"
		L"        Address of the I2C slave when using I2C communications (ignored for\n"
		L"        UART mode)\n"
		L"    -persist\n"
		L"        ezbl_comm.exe process will reopen the local pipes for a new session\n"
		L"        when the attached process closes its pipe handles.\n"
		L"    -log\n"
		L"        Displays timestamped TX and RX bytes of all data passing through.\n"
		L"    -rx_bin_log=rx_log.bin\n"
		L"        Writes all raw commnications RX bytes to the 'rx_log.bin' file without\n"
        L"        any formatting or time stamp information.\n"
		L"    -enum\n"
		L"        Displays all COM ports obtained by QueryDosDevice().\n"
		L"    -enum_all\n"
		L"        Displays all devices obtained by QueryDosDevice().\n"
		L"\n", pid, pid);




	// Decode any string arguments into numbers and verify required parameters are present. 
	// Also choose default values if argument not specified.
	if(comFile == NULL)
	{
		writeLogf(L"\nRequired target communications port not specified. Please run with -com= option specified."
				  L"\nThe following COM ports exist on this system:\n");

	    int dwSize = EnumCOMPorts(strBuf, sizeof(strBuf)/sizeof(strBuf[0]));
        for(int j = 0; j < dwSize; j++)
        {
	        DWORD strLen = _tcsnlen(&strBuf[j], sizeof(strBuf)/sizeof(strBuf[0]) - j);
            writeLogf(L"    %s\n", &strBuf[j]);
	        j += strLen;
        }
		FlushLog();
		if(logFIFO)
			VirtualFree((VOID*)logFIFO, 0, MEM_RELEASE);
		_fcloseall();
		CloseHandle(hOneEZBLCommMutex);
		return 2;   // Missing -com="x" parameter.
	}
	comBaudRate = i2cComNumber == 0 ? 115200 : 400000;	// If no -baud="x" option specified, default to 115200 baud (for UART) or 400kHz (for I2C). Otherwise, convert the given value to an integer. The value must be given as a decimal number.
	if(baudString != NULL)  
		comBaudRate = _tcstol(baudString, L'\0', 10);

	i2cSlaveAddress = 0x00;	// Default to I2C address 0x00 if -slave_address="x" not provided. The slave address must be specified as a decimal number.
	if(slaveAddressString != NULL)
	{
		int j = _tcslen(slaveAddressString);

		i2cSlaveAddress = -1;
		if(j >= 3)
		{
			if((slaveAddressString[0] == '0') && (slaveAddressString[1] == 'x'))
			{ // Decode hex
				i2cSlaveAddress = 0;
				int multiplier = 1;
				for(j--; j >= 2; j--)
				{
					int c = slaveAddressString[j] - '0';
					if(c > 9)
						c -= 'A' - '9' - 1;
					if(c > 0xF)
						c -= 'a' - 'A' - 1;
					i2cSlaveAddress += multiplier*c;
					multiplier *= 16;
				}
			}
		}
		if(i2cSlaveAddress == -1)
			i2cSlaveAddress = _tcstol(slaveAddressString, L'\0', 10);
	}


	// Open and configure the communications port
	if(i2cComNumber == 0)	// Ordinary UART communications mode
	{
		hComm = InitializeCOMPort(comFile, strBuf, sizeof(strBuf)/sizeof(strBuf[0]));
		if(hComm == INVALID_HANDLE_VALUE)
		{
			writeLogf(L"%s", strBuf);
			FlushLog();
			if(logFIFO)
				VirtualFree((VOID*)logFIFO, 0, MEM_RELEASE);
			_fcloseall();
			CloseHandle(hOneEZBLCommMutex);
			return 1;   // Could not open specified -com="x" COM port. Ensure the COM port exists and is not already in use by another application.
		}
	}
	else	// MCP2221 I2C communications mode
	{
        hComm = InitializeI2CPort(0x04D8, 0x00DD, i2cComNumber-1, comBaudRate);
        if(hComm == INVALID_HANDLE_VALUE)
        {
            FlushLog();
		    if(logFIFO)
			    VirtualFree((VOID*)logFIFO, 0, MEM_RELEASE);
		    _fcloseall();
		    CloseHandle(hOneEZBLCommMutex);
            return 1;
        }

        hI2CNotifyMutex = CreateMutex(NULL, FALSE, L"Local\\ezbl_i2c_thread_to_thread_notify_mutex");
		if(hI2CNotifyMutex == NULL)
		{
			dwError = GetLastError();
			writeLogError(dwError, L"\n%10.6f: CreateMutex() in main() returned: %d\n            ", GetTimeStamp(), dwError);
		}
	}


	if(!SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS))
	{
		dwError = GetLastError();
		writeLogf(L"\n%10.6f: SetPriorityClass() returned error code %d (%s).\n", GetTimeStamp(), dwError);
	}
	if(!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL))	// Drop thread priority since the TX/RX threads are much more important
	{
		dwError = GetLastError();
		writeLogError(dwError, L"\n%10.6f: SetThreadPriority() in main() returned: %d\n            ", GetTimeStamp(), dwError);
	} 



	// Launch TX and RX transfer threads, then wait for them both to terminate
	CreateThread(NULL, 524288, (LPTHREAD_START_ROUTINE)&COMInToPipeThreadProc, &hComm, 0, &COMToPipeThreadID);
	CreateThread(NULL, 524288, (LPTHREAD_START_ROUTINE)&PipeOutToCOMThreadProc, &hComm, 0, &PipeToCOMThreadID);
	waitHandles[0] = OpenThread(SYNCHRONIZE, FALSE, COMToPipeThreadID);
	waitHandles[1] = OpenThread(SYNCHRONIZE, FALSE, PipeToCOMThreadID);
	waitHandles[2] = CreateEvent(NULL, TRUE, TRUE, L"Local\\LogDataReadyEvent");

	writeLogf(L"Timestamp: Direction Size @ Offset in stream (direction specific)\n"
			  L"-----------------------------------------------------------------\n");

	FlushLog();
	waitReturn = 0;
	while(1)
	{
		waitReturn = WaitForMultipleObjects(3, waitHandles, FALSE, 500);
		if(waitReturn == WAIT_OBJECT_0 + 2)	// Log data ready event
		{
			FlushLog();
			if(*headIndex == *tailIndex)
				ResetEvent(waitHandles[2]);	// Log data not ready anymore
		}
		else if((waitReturn == WAIT_OBJECT_0 + 0) || (waitReturn == WAIT_OBJECT_0 + 1))	// Either thread terminated
		{	// One thread closed
			if(i2cComNumber == 0)		// Ordinary UART communications mode?
			{
				SetCommMask(hComm, 0);	// Set the event mask to 0x00000000 so WaitCommEvent() returns in hPipeThreadToCOMThread thread
			}
			WaitForMultipleObjects(2, waitHandles, TRUE, 10000);	// Wait until both threads are closed
			break;
		}
	}
	for(int i = 0; i < sizeof(waitHandles)/sizeof(waitHandles[0]); i++)
	{
		CloseHandle(waitHandles[i]);
		waitHandles[i] = NULL;
	}

	writeLogf(L"\n%10.6f: ezbl_comm.exe closing normally\n", GetTimeStamp());

	// Shutting down: disconnect from communications port
	if(hComm != INVALID_HANDLE_VALUE)
	{
		if(i2cComNumber == 0)	// Ordinary UART
		{
			CloseHandle(hComm);
		}
		else					// MCP2221 I2C
		{
			ret = Mcp2221_Close(hComm);
			if(ret)
			{
				writeLogf(L"\n%10.6f: Mcp2221_Close() returned error code %d (%s).\n", GetTimeStamp(), ret, GetMCP2221ErrorString(ret));
			}
		}
		hComm = INVALID_HANDLE_VALUE;
	}

	_flushall();
	FlushLog();
	_flushall();
	ReleaseMutex(hOneEZBLCommMutex);
	CloseHandle(hOneEZBLCommMutex);
	CloseHandle(hI2CNotifyMutex);
	if(logFIFO)
		VirtualFree((VOID*)logFIFO, 0, MEM_RELEASE);
	_fcloseall();
	return 0;   // ezbl_comm.exe closed normally
}


// Writes accumulated log data from synchronized FIFO RAM to the output file or stdout
void FlushLog(void)
{
	DWORD head, tail;
	unsigned int chunkSize;

	if(logFIFO == NULL)
	{
		_flushall();
		return;
	}

	head = *headIndex;
	tail = *tailIndex;
	if(head == tail)
		return;

	while(tail != head)
	{
		chunkSize = head - tail;	// In bytes
		if(head < tail)
			chunkSize = logFIFOSize - tail;

		if(logFile != NULL)
		{
			if(logFile != stdout)
				fwrite((const void*)&logFIFO[tail], 1, chunkSize, logFile);
			fwrite((const void*)&logFIFO[tail], 1, chunkSize, stdout);
		}
		tail += chunkSize;
		if(tail >= logFIFOSize)
			tail = 0;
	}

	*tailIndex = tail;
}


HANDLE InitializeCOMPort(TCHAR *comFile, TCHAR *errorMsg, DWORD errorMsgSize)
{
	HANDLE			hComm;
	DWORD			dwSize;
 	COMMCONFIG		comConfig;
	COMMTIMEOUTS	comTimeouts;
	DWORD			errorMsgGrowth;

	if((errorMsg != NULL) && errorMsgSize)	// Null terminate for successful return/no error message written
	{
		errorMsg[0] = L'\0';
	}

	hComm = CreateFile(comFile, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if(hComm == INVALID_HANDLE_VALUE)
	{
		PrintError(errorMsg, errorMsgSize, L"Could not open read/write handle to com port", GetLastError());
		return hComm;
	}

	dwSize = sizeof(comConfig);
	if(GetCommConfig(hComm, &comConfig, &dwSize) == 0)
	{
		errorMsgGrowth = PrintError(errorMsg, errorMsgSize, L"Could not read com port configuration", GetLastError());
		errorMsg      += errorMsgGrowth;
		errorMsgSize  -= errorMsgGrowth;
	}

	comConfig.dcb.DCBlength = sizeof(comConfig.dcb);
	comConfig.dcb.BaudRate = comBaudRate;
	comConfig.dcb.fBinary = TRUE;
	comConfig.dcb.fParity = FALSE;		// Do not check and error on parity
	comConfig.dcb.fOutxCtsFlow = FALSE;	// Do not monitor CTS flow control
	comConfig.dcb.fOutxDsrFlow = FALSE; // Do not monitor DSR flow control
	comConfig.dcb.fDtrControl = DTR_CONTROL_ENABLE;	// Turn on DTR line and leave it asserted
	comConfig.dcb.fDsrSensitivity = FALSE;	// Always receive, independent of DSR state
	comConfig.dcb.fTXContinueOnXoff = TRUE;	//TRUE: transmission continues after the input buffer has come within XoffLim bytes of being full and the driver has transmitted the XoffChar character to stop receiving bytes
	comConfig.dcb.fOutX = FALSE;		// Do not enable XON/XOFF flow control to avoid remote receive buffer overflow
	comConfig.dcb.fInX = FALSE;			// Do not enable XON/XOFF flow control to avoid local receive buffer overflow
	comConfig.dcb.fErrorChar = FALSE;	// No character replacement on parity errors
	comConfig.dcb.fNull = FALSE;		// No NULL character discarding occurs
	comConfig.dcb.fRtsControl = RTS_CONTROL_ENABLE;	// Turn on Request-To-Send and leave it asserted
	comConfig.dcb.fAbortOnError = TRUE;	// Abort read/write on error and wait for ClearCommError to be called
	comConfig.dcb.fDummy2 = 0;			// Reserved
	comConfig.dcb.wReserved = 0;		// Reserved - must be 0
	comConfig.dcb.XonLim = 3;			// The bytes in uses in the input buffer before flow control is deactivated to allow transmission by the sender
	comConfig.dcb.XoffLim = 3;			// The free bytes left in the input before before flow control is activated to inhibit the sender
	comConfig.dcb.ByteSize = 8;
	comConfig.dcb.Parity = NOPARITY;
	comConfig.dcb.StopBits = ONESTOPBIT;
	comConfig.dcb.XonChar = 0x11;
	comConfig.dcb.XoffChar = 0x13;
	comConfig.dcb.ErrorChar = 0x00;
	comConfig.dcb.EofChar = 0x00;
	comConfig.dcb.EvtChar = 0x00;
	comConfig.dcb.wReserved1 = 0;

	if(SetCommConfig(hComm, &comConfig, sizeof(comConfig)) == 0)
	{
		errorMsgGrowth = PrintError(errorMsg, errorMsgSize, L"Could not set com port configuration", GetLastError());
		errorMsg      += errorMsgGrowth;
		errorMsgSize  -= errorMsgGrowth;
	}

	if(GetCommTimeouts(hComm, &comTimeouts) == 0)
	{
		errorMsgGrowth = PrintError(errorMsg, errorMsgSize, L"Could not get com timeouts", GetLastError());
		errorMsg      += errorMsgGrowth;
		errorMsgSize  -= errorMsgGrowth;
	}
	comTimeouts.ReadIntervalTimeout = 0;			// Abort ReadFile operations if the data isn't available and the COM is idle for 1 milliseconds
	comTimeouts.ReadTotalTimeoutMultiplier = 0;		// No extra time given per byte of requested data
	comTimeouts.ReadTotalTimeoutConstant = 0;		// Wait at least 1ms for data to arrive (fixed offset)
	comTimeouts.WriteTotalTimeoutMultiplier = 0;	// Don't use total write timeout
	comTimeouts.WriteTotalTimeoutConstant = 0;		// Don't use total write timeout
	if(SetCommTimeouts(hComm, &comTimeouts) == 0)
	{
		errorMsgGrowth = PrintError(errorMsg, errorMsgSize, L"Could not set com timeouts", GetLastError());
		errorMsg      += errorMsgGrowth;
		errorMsgSize  -= errorMsgGrowth;
	}
	
	// Receive events from COM to wake us from sleeping states
	if(!SetCommMask(hComm, EV_BREAK | EV_CTS | EV_ERR | EV_RXCHAR | EV_TXEMPTY))
	{
		errorMsgGrowth = PrintError(errorMsg, errorMsgSize, L"Could not set com event mask", GetLastError());
		errorMsg      += errorMsgGrowth;
		errorMsgSize  -= errorMsgGrowth;
	}

	return hComm;
}


HANDLE InitializeI2CPort(int vid, int pid, int instanceNum, int SCLFrequency)
{
    HANDLE hComm;
    int ret;
    int retries = 10;
    
    hComm = Mcp2221_OpenByIndex(vid, pid, i2cComNumber-1);	// 0x04D8 is Microchip USB Vender ID (VID), 0x00DD is Microchip MCP2221 Product ID (PID)
	if(hComm == INVALID_HANDLE_VALUE)
	{
		ret = Mcp2221_GetLastError();
		writeLogf(L"\n%10.6f: Mcp2221_OpenByIndex() returned error code %d (%s).\n", GetTimeStamp(), ret, GetMCP2221ErrorString(ret));
		return INVALID_HANDLE_VALUE;
	}

	ret = Mcp2221_SetSpeed(hComm, SCLFrequency);	// Set the I2C frequency
	if(ret)
	{
		writeLogf(L"\n%10.6f: Mcp2221_SetSpeed() returned error code %d (%s).\n", GetTimeStamp(), ret, GetMCP2221ErrorString(ret));

		ret = Mcp2221_I2cCancelCurrentTransfer(hComm);
		if(ret)
		{
			writeLogf(L"\n%10.6f: Mcp2221_I2cCancelCurrentTransfer() returned error code %d (%s).\n", GetTimeStamp(), ret, GetMCP2221ErrorString(ret));
		}

        ret = Mcp2221_SetSpeed(hComm, SCLFrequency);	// Set the I2C frequency
		if(ret)
		{
			writeLogf(L"\n%10.6f: Mcp2221_SetSpeed() returned error code %d (%s).\n", GetTimeStamp(), ret, GetMCP2221ErrorString(ret));

			ret = Mcp2221_Reset(hComm);	// Issue a reset in case if the MCP2221 I2C communications ended previously at an inopportune time for the firmware
			if(ret)
			{
				writeLogf(L"\n%10.6f: Mcp2221_Reset() returned error code %d (%s).\n", GetTimeStamp(), ret, GetMCP2221ErrorString(ret));
			}
            CloseHandle(hComm);

			// Reopen the handle to the MCP2221 since Mcp2221_Reset() closes the handle as well.
            while(retries--)
            {
    			Sleep(100);	// Give a delay for Windows to reenumerate the MCP2221
			    hComm = Mcp2221_OpenByIndex(vid, pid, i2cComNumber-1);
			    if(hComm != INVALID_HANDLE_VALUE)
                    break;

                ret = Mcp2221_GetLastError();
			    writeLogf(L"\n%10.6f: Mcp2221_OpenByIndex() returned error code %d (%s).\n", GetTimeStamp(), ret, GetMCP2221ErrorString(ret));
            }
			ret = Mcp2221_SetSpeed(hComm, SCLFrequency);	// Set the I2C frequency
			if(ret)
			{
				writeLogf(L"\n%10.6f: Mcp2221_SetSpeed() returned error code %d (%s).\n", GetTimeStamp(), ret, GetMCP2221ErrorString(ret));
			}
		}
	}

    // Lengthen timeout to 60ms (default 3ms) and drop retries to 0 (default 5). This is needed since a flash page erase takes up to 48ms, so device may be stuck clock stretching for a read.
	ret = Mcp2221_SetAdvancedCommParams(hComm, 0, 60); // Timeout = 60ms, Max Retries = 1
	if(ret)
	{
		writeLogf(L"\n%10.6f: Mcp2221_SetAdvancedCommParams() returned error code %d (%s).\n", GetTimeStamp(), ret, GetMCP2221ErrorString(ret));
    }

    return hComm;
}



double GetTimeStamp()
{
	LARGE_INTEGER timeStamp;

	QueryPerformanceCounter(&timeStamp);
	timeStamp.QuadPart -= timeStart.QuadPart;
	return timeStamp.QuadPart/(double)freq.QuadPart;
}


// Prints a formatted Windows Error Code message to a string, provided a GetLastError() error code
DWORD PrintError(TCHAR *outBuf, DWORD maxCount, TCHAR *preErrorStr, DWORD winErrorCode)
{
	DWORD tcharsWritten = 0;	// Excludes terminating null TCHAR

	if((outBuf != NULL) && maxCount)
	{
		TCHAR systemMsg[16384];
		DWORD systemMsgSize;

		systemMsg[0] = L'\0';
		systemMsgSize = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, winErrorCode, 0, systemMsg, sizeof(systemMsg)/sizeof(systemMsg[0])-1, NULL);
		if(preErrorStr != NULL)
		{
			tcharsWritten = _swprintf_p(outBuf, maxCount, L"\n%10.6f: %s"
														  L"\n          Error % 4d: %s\n", GetTimeStamp(), preErrorStr, winErrorCode, systemMsg);
		}
		else
		{
			tcharsWritten = _swprintf_p(outBuf, maxCount, L"%s\n", systemMsg);
		}
	}
	return tcharsWritten;
}


void PrintError(DWORD errorCode)
{
	DWORD tcharsMade;
	TCHAR errorMsg[2048];
	tcharsMade = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode, 0, errorMsg, sizeof(errorMsg)/sizeof(TCHAR), NULL);
	if(tcharsMade)
	{
		writeLogf(L"            %s\n", errorMsg);
	}
}

// Prints formatted error message to the output log (or stdout) followed by the error message returned by the system for the corresponding winErrorCode parameter
void writeLogError(DWORD winErrorCode, TCHAR *format, ...)
{
	TCHAR outBuf[16384];
	va_list args;


	// Generate user formatted output log string
	va_start(args, format); // Initialize variable arguments list
	int fmtMsgCount = _vsnwprintf_s(outBuf, sizeof(outBuf)/sizeof(outBuf[0]), _TRUNCATE, format, args);
    va_end(args);			// Clean up variable arguments list (does nothing in GNU C)

	DWORD msgCount = PrintError(&outBuf[fmtMsgCount], sizeof(outBuf)/sizeof(outBuf[0])-fmtMsgCount-1, NULL, winErrorCode);
	writeLog(outBuf, -1, TRUE);
}


void writeLogf(TCHAR *format, ...)
{
	TCHAR	myBuf[32768+1];
	va_list args;

	// Generate output log string
	va_start(args, format); // Initialize variable arguments list
	int charsMade = _vsnwprintf_s(myBuf, sizeof(myBuf)/sizeof(myBuf[0]), _TRUNCATE, format, args);
    va_end(args);			// Clean up variable arguments list (does nothing in GNU C)

	if(charsMade <= 0)
	{
		return;
	}
	writeLog(myBuf, -1, TRUE);
}


void writeLog(TCHAR *message)
{
	writeLog(message, -1, TRUE);
}


// Set messageLen == -1 when not known
// Set dontFlush == TRUE when the main log printing thread should not be awoken to process the new data yet (generally more data is about to be written)
void writeLog(TCHAR *message, LONG messageLen, BOOL flushToMain)
{
	static HANDLE hLogDataReady = NULL;
	static DWORD lastLogFlushTime = 0;
	CHAR charBuf[8193];
	HANDLE hPrintMutex;
	LONG strLen;
	DWORD chunkSize;

	strLen = messageLen;
	if(messageLen < 0)
		strLen = wcslen(message);

	if((strLen > 0) || flushToMain)
	{
		hPrintMutex = CreateMutex(NULL, FALSE, L"Local\\ezbl_comm.exe log write synchronization mutex");
		if(hPrintMutex == (HANDLE)ERROR_INVALID_HANDLE)
		{
			printf("\nCould not create ezbl_comm.exe log write synchronization mutex\n");
			return;
		}

		if(WaitForSingleObject(hPrintMutex, 200) != WAIT_OBJECT_0)
		{
			printf("\nCould not obtain ownership of ezbl_comm.exe log write synchronization mutex\n");
			return;
		}
	}

	while(strLen > 0)
	{
		chunkSize = sizeof(charBuf) - 1;
		if(chunkSize > (DWORD)strLen)
			chunkSize = strLen;
		wcstombs_s((size_t*)&chunkSize, charBuf, sizeof(charBuf), message, chunkSize);			// Convert to single byte chars (may not actually be single byte if input data isn't 7-bit ASCII)
		if((chunkSize == (DWORD)((size_t)-1)) || (chunkSize == 0))	// Abort if non convertable character encountered
			break;
		message += --chunkSize; // Need to negate null terminator in count

		// Copy output data to logFIFO
		if(logFIFO == NULL)
		{
			fwrite((const void*)charBuf, 1, chunkSize, stdout);
		}
		else
		{
			DWORD cpyLen;
			DWORD secondCpyLen;
			DWORD head, tail, freeSpace;

			head = *headIndex;
			tail = *tailIndex;
			freeSpace = logFIFOSize;
			if(head < tail)
				freeSpace = tail - head - 1;
			else if(head > tail)
				freeSpace = logFIFOSize - head + tail - 1;

			cpyLen = chunkSize;
			if(cpyLen > freeSpace)
				cpyLen = freeSpace;
			if(cpyLen > logFIFOSize - head)
				cpyLen = logFIFOSize - head;
			memcpy((void*)&logFIFO[head], charBuf, cpyLen);
			head += cpyLen;
			if(head >= logFIFOSize)
				head = 0;
			freeSpace -= cpyLen;
			if((cpyLen != chunkSize) && freeSpace)
			{
				secondCpyLen = chunkSize - cpyLen;
				if(secondCpyLen > freeSpace)
					secondCpyLen = freeSpace;
				memcpy((void*)&logFIFO[head], &charBuf[cpyLen], secondCpyLen);
				head += secondCpyLen;
			}

			*headIndex = head;
		}
		strLen -= chunkSize;
	}

	if(flushToMain)
	{
		if(hLogDataReady == NULL)
			hLogDataReady = OpenEvent(EVENT_MODIFY_STATE, FALSE, L"Local\\LogDataReadyEvent");
		if(hLogDataReady != NULL)
		{
			SetEvent(hLogDataReady);
		}
	}

	ReleaseMutex(hPrintMutex);
}


/**
 * Debug display of ASCII and hex decoded human readable forms of data passing through TX and RX (or whatever) streams.
 *
 * @param *type Pointer to null terminated string indicating the directionality, ex: L"RX" or L"TX".
 */
void WriteLogData(double timestamp, TCHAR *type, unsigned int streamOffset, unsigned char *data, unsigned int dataCount)
{
	TCHAR logLine[8+16*3+2+16+1+1];
	TCHAR msg[32768 - (32768 % (sizeof(logLine)/sizeof(logLine[0]))) + 1];	
	TCHAR h;
	TCHAR l;
	unsigned int i;
	unsigned int linePos;
	int printSize;

	for(i = 0; i < sizeof(logLine)/sizeof(logLine[0]) - 3; i++)
		logLine[i] = L' ';
	logLine[sizeof(logLine)/sizeof(logLine[0]) - 2] = L'\n';
	logLine[sizeof(logLine)/sizeof(logLine[0]) - 1] = L'\0';

	printSize = 0;
	printSize += _snwprintf_s(&msg[printSize], sizeof(msg)/sizeof(msg[0]) - printSize, _TRUNCATE, L"%10.6f: %s %2d @ %d:\n", timestamp, type, dataCount, streamOffset);


	while(dataCount)
	{
		linePos = 8;
		for(i = 0; i < 16; i++)
		{
			if(dataCount)
			{
				// Hex char MSbits and LSbits
				h = ((*data & 0xF0)>>4) + L'0';
				l = ((*data & 0x0F))  + L'0';
				if(h > L'9')
					h -= L'0' + 10 - L'A';
				if(l > L'9')
					l -= L'0' + 10 - L'A';
				logLine[linePos++] = h;
				logLine[linePos++] = l;

				// Text char
				l = (TCHAR)*data++;
				if((l < L' ') || (l > L'~'))
					l = L'.';
				logLine[8 + 16*3 + 2 + i] = l;

				dataCount--;
			}
			else
			{
				logLine[linePos++] = L' ';			// Hex char MSbits
				logLine[linePos++] = L' ';			// Hex char LSbits
				logLine[8 + 16*3 + 2 + i] = L' ';	// Text char
			}
			linePos++;								// Intra byte space
		}

		printSize += _snwprintf_s(&msg[printSize], sizeof(msg)/sizeof(msg[0]) - printSize, _TRUNCATE, L"%s", logLine);
		if(printSize >= sizeof(msg)/sizeof(msg[0]) - 1)	// Need to flush data -> note: will cause potential TX/RX intermingling since the whole write won't be atomic anymore
		{
			writeLog(msg);
			printSize = 0;
		}
	}
	if(printSize)
		writeLog(msg);
}


const TCHAR *GetMCP2221ErrorString(int errorCode)
{
	switch(errorCode)
	{
	case E_NO_ERR:							// 0
		return _T("No Error");
	case E_ERR_UNKOWN_ERROR:				// -1
		return _T("E_ERR_UNKOWN_ERROR");
	case E_ERR_CMD_FAILED:					// -2
		return _T("E_ERR_CMD_FAILED");
	case E_ERR_INVALID_HANDLE:				// -3
		return _T("E_ERR_INVALID_HANDLE");
	case E_ERR_INVALID_PARAMETER:			// -4
		return _T("E_ERR_INVALID_PARAMETER");
	case E_ERR_INVALID_PASS:				// -5
		return _T("E_ERR_INVALID_PASS");
	case E_ERR_PASSWORD_LIMIT_REACHED:		// -6
		return _T("E_ERR_PASSWORD_LIMIT_REACHED");
	case E_ERR_FLASH_WRITE_PROTECTED:		// -7
		return _T("E_ERR_FLASH_WRITE_PROTECTED");
	case E_ERR_NULL:						// -10 // null pointer received
		return _T("E_ERR_NULL: null pointer received");
	case E_ERR_DESTINATION_TOO_SMALL:		// -11 // destination string too small
		return _T("E_ERR_DESTINATION_TOO_SMALL: destination string too small");
	case E_ERR_INPUT_TOO_LARGE:				// -12
		return _T("E_ERR_INPUT_TOO_LARGE");
	case E_ERR_FLASH_WRITE_FAILED:			// -13
		return _T("E_ERR_FLASH_WRITE_FAILED");
	case E_ERR_NO_SUCH_INDEX:				// -101	// we tried to connect to a device with a non existent index
		return _T("E_ERR_NO_SUCH_INDEX: we tried to connect to a device with a non existent index");
	case E_ERR_DEVICE_NOT_FOUND:			// -103	// no device matching the provided criteria was found
		return _T("E_ERR_DEVICE_NOT_FOUND: no device matching the provided criteria was found");
	case E_ERR_INTERNAL_BUFFER_TOO_SMALL:	// -104	// one of the internal buffers of the function was too small
		return _T("E_ERR_INTERNAL_BUFFER_TOO_SMALL: one of the internal buffers of the function was too small");
	case E_ERR_OPEN_DEVICE_ERROR:			// -105	// an error occurred when trying to get the device handle
		return _T("E_ERR_OPEN_DEVICE_ERROR: an error occurred when trying to get the device handle");
	case E_ERR_CONNECTION_ALREADY_OPENED:	// -106	// connection already opened
		return _T("E_ERR_CONNECTION_ALREADY_OPENED: connection already opened");
	case E_ERR_CLOSE_FAILED:				// -107
		return _T("E_ERR_CLOSE_FAILED");
	case E_ERR_INVALID_SPEED:				// -401
		return _T("E_ERR_INVALID_SPEED");
	case E_ERR_SPEED_NOT_SET:				// -402
		return _T("E_ERR_SPEED_NOT_SET");
	case E_ERR_INVALID_BYTE_NUMBER:			// -403
		return _T("E_ERR_INVALID_BYTE_NUMBER");
	case E_ERR_INVALID_ADDRESS:				// -404
		return _T("E_ERR_INVALID_ADDRESS");
	case E_ERR_I2C_BUSY:					// -405
		return _T("E_ERR_I2C_BUSY");
	case E_ERR_I2C_READ_ERROR:				// -406		//mcp2221 signaled an error during the i2c read operation
		return _T("E_ERR_I2C_READ_ERROR: mcp2221 signaled an error during the i2c read operation");
	case E_ERR_ADDRESS_NACK:				// -407
		return _T("E_ERR_ADDRESS_NACK");
	case E_ERR_TIMEOUT:						// -408
		return _T("E_ERR_TIMEOUT");
	case E_ERR_TOO_MANY_RX_BYTES:			// -409
		return _T("E_ERR_TOO_MANY_RX_BYTES");
	case E_ERR_COPY_RX_DATA_FAILED:			// -410	//could not copy the data received from the slave into the provided buffer;
		return _T("E_ERR_COPY_RX_DATA_FAILED: could not copy the data received from the slave into the provided buffer");
	case E_ERR_COPY_TX_DATA_FAILED:			// -412	// failed to copy the data into the HID buffer
		return _T("E_ERR_COPY_TX_DATA_FAILED: failed to copy the data into the HID buffer");
	case E_ERR_NO_EFFECT:					// -411			// The i2c engine (inside mcp2221) was already idle. The cancellation command had no effect.
		return _T("E_ERR_NO_EFFECT: The i2c engine (inside mcp2221) was already idle. The cancellation command had no effect.");
	case E_ERR_INVALID_PEC:					// -413
		return _T("E_ERR_INVALID_PEC");
	case E_ERR_BLOCK_SIZE_MISMATCH:			// -414	// The slave sent a different value for the block size(byte count) than we expected
		return _T("E_ERR_BLOCK_SIZE_MISMATCH: The slave sent a different value for the block size(byte count) than we expected");
	case E_ERR_RAW_TX_TOO_LARGE:			// -301
		return _T("E_ERR_RAW_TX_TOO_LARGE");
	case E_ERR_RAW_TX_COPYFAILED:			// -302
		return _T("E_ERR_RAW_TX_COPYFAILED");
	case E_ERR_RAW_RX_COPYFAILED:			// -303
		return _T("E_ERR_RAW_RX_COPYFAILED");
	}
	return _T("Not a recognized MCP2221 error code");
}


