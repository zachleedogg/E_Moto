#include "stdafx.h"
#include "malloc.h"


// Data from the PIC to the PC software (i.e. Read from the COM, write to the pipe)
DWORD COMInToPipeThreadProc(LPVOID comHandle)
{
	HANDLE	hComm = *((HANDLE*)comHandle);
	HANDLE	hPipe;
	BYTE	data[65536];
	DWORD	dwSize, dwActual;
	DWORD	dwCOMEvents;
	COMSTAT	comstat;
	DWORD	dwOffset;
	double  activityTime;
	HANDLE  hI2CNotifyMutex = NULL;
	DWORD	dwError;
	int		pipeConnected;
    OVERLAPPED comEvent;	// Event object for use by WaitCommEvent
	OVERLAPPED pipeConnect;	// Event object for use by ConnectNamedPipe()
	

	// Set up overalapped event object for WaitCommEvent()
	comEvent.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	// auto-reset event, not signaled, not named
	comEvent.Internal = 0;
	comEvent.InternalHigh = 0;
	comEvent.Offset = 0;
	comEvent.OffsetHigh = 0;

	pipeConnect.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	// auto-reset event, not signaled, not named
	pipeConnect.Internal = 0;
	pipeConnect.InternalHigh = 0;
	pipeConnect.Offset = 0;
	pipeConnect.OffsetHigh = 0;

	if(!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL))	// Boost thread priority by 1 point
	{
		DWORD dwError = GetLastError();
		writeLogError(dwError, L"\n%10.6f: SetThreadPriority() in COMInToPipeThreadProc() returned: %d\n            ", GetTimeStamp(), dwError);
	} 

	if(i2cComNumber)
	{
		hI2CNotifyMutex = OpenMutex(SYNCHRONIZE, FALSE, L"Local\\ezbl_i2c_thread_to_thread_notify_mutex");
		if(hI2CNotifyMutex == NULL)
		{
			dwError = GetLastError();
			writeLogError(dwError, L"\n%10.6f: OpenMutex() in COMInToPipeThreadProc() returned: %d\n            ", GetTimeStamp(), dwError);
		}
	}



	while(1)
	{
		// Create a pipe
		//_snwprintf_s((TCHAR*)data, sizeof(data)/sizeof(TCHAR), _TRUNCATE, _T("\\\\.\\pipe\\ezbl_pipe_in_from_com_%d"), GetCurrentProcessId());
		_snwprintf_s((TCHAR*)data, sizeof(data)/sizeof(TCHAR), _TRUNCATE, _T("\\\\.\\pipe\\ezbl_pipe_in_from_com"));
		hPipe = CreateNamedPipe((TCHAR*)data, PIPE_ACCESS_OUTBOUND | FILE_FLAG_OVERLAPPED, PIPE_TYPE_BYTE | PIPE_ACCEPT_REMOTE_CLIENTS, 8, 16384, 0, 0, NULL);
		if(hPipe == INVALID_HANDLE_VALUE)
		{
			DWORD dwError = GetLastError();
			writeLogError(dwError, L"\n%10.6f: Could not create %s handle in COMInToPipeThreadProc(): %d\n            ", GetTimeStamp(), (TCHAR*)data, dwError);
			CloseHandle(comEvent.hEvent);
			CloseHandle(pipeConnect.hEvent);
			return dwError;
		}

		// Reset RX data byte offset marker for logging purposes
		dwOffset = 0;
		pipeConnected = 0;

		// Forward data from communications port to the pipe
		while(1)
		{
			// Check if someone has connected to the pipe, or if already connected, call again to test if the client has disconnected
			ConnectNamedPipe(hPipe, &pipeConnect);
			DWORD dwError = GetLastError();

			if((dwError == 0) || (dwError == ERROR_PIPE_CONNECTED))
			{
				if(!pipeConnected)
					writeLogf(L"\n%10.6f: \\\\.\\pipe\\ezbl_pipe_in_from_com connected\n", GetTimeStamp());
				pipeConnected = 1;
			}
			else if(dwError != ERROR_IO_PENDING)
			{
				pipeConnected = 0;
				writeLogError(dwError, L"\n%10.6f: ConnectNamedPipe() return in COMInToPipeThreadProc(): %d\n            ", GetTimeStamp(), dwError);
				if(dwError != 232u)	// 232 == "The pipe is being closed.", natural termination, so don't delay process termination
					Sleep(1);
				break;
			}

			
			if(i2cComNumber == 0)	// UART mode
			{
				// Wait for at least one byte to be ready to be read from COM
				if(ClearCommError(hComm, &dwCOMEvents, &comstat) == 0)
				{
					DWORD dwError = GetLastError();
					writeLogError(dwError, L"\n%10.6f: ClearCommError() return in COMInToPipeThreadProc(): %d\n            ", GetTimeStamp(), dwError);
					Sleep(1);
					break;
				}
				if(dwCOMEvents & (CE_BREAK | CE_FRAME | CE_OVERRUN | CE_RXOVER | CE_RXPARITY))
				{
					TCHAR errorStrings[513];
					DWORD errorStringLen = 0;
					errorStrings[0] = L'\0';

					if(dwCOMEvents & CE_BREAK)
					{
						errorStringLen += wsprintf(&errorStrings[errorStringLen], L"break condition%s", (dwCOMEvents & ~CE_BREAK) ? ", " : "");
						dwCOMEvents &= ~CE_BREAK;
					}
					if(dwCOMEvents & CE_FRAME)
					{
						errorStringLen += wsprintf(&errorStrings[errorStringLen], L"framing error%s", (dwCOMEvents & ~CE_FRAME) ? ", " : "");
						dwCOMEvents &= ~CE_FRAME;
					}
					if(dwCOMEvents & CE_OVERRUN)
					{
						errorStringLen += wsprintf(&errorStrings[errorStringLen], L"character buffer overrun%s", (dwCOMEvents & ~CE_OVERRUN) ? ", " : "");
						dwCOMEvents &= ~CE_OVERRUN;
					}
					if(dwCOMEvents & CE_RXOVER)
					{
						errorStringLen += wsprintf(&errorStrings[errorStringLen], L"input buffer overflow%s", (dwCOMEvents & ~CE_RXOVER) ? ", " : "");
						dwCOMEvents &= ~CE_RXOVER;
					}
					if(dwCOMEvents & CE_RXPARITY)
					{
						errorStringLen += wsprintf(&errorStrings[errorStringLen], L"parity error%s", (dwCOMEvents & ~CE_RXPARITY) ? ", " : "");
						dwCOMEvents &= ~CE_RXPARITY;
					}
					errorStrings[errorStringLen] = L'\0';

					writeLogf(L"\n%10.6f: RX hardware indicates: %s\n", GetTimeStamp(), errorStrings);
				}

				if(comstat.cbInQue == 0)	// If no data go to sleep for a bit
				{
					if(!WaitCommEvent(hComm, &dwCOMEvents, &comEvent))
					{
						DWORD dwError = GetLastError();
						if(dwError != ERROR_IO_PENDING)
						{	// ERROR_IO_PENDING is the expected result, so only print other values
							writeLogError(dwError, L"\n%10.6f: WaitCommEvent() return in COMInToPipeThreadProc(): %d\n            ", GetTimeStamp(), dwError);
							Sleep(1);
						}
					}
					// Wait for one of the EV_BREAK | EV_CTS | EV_ERR | EV_RXCHAR | EV_TXEMPTY events to wake this thread up, or termination of the other thread calling SetCommMask().
					GetOverlappedResult(hComm, &comEvent, &dwActual, TRUE);	// dwActual is meaningless/don't care data
					continue;
				}

				activityTime = GetTimeStamp();

				// Read the data from the COM
				dwSize = comstat.cbInQue;
				if(dwSize > sizeof(data))
					dwSize = sizeof(data);

				if(ReadFile(hComm, data, dwSize, &dwActual, &comEvent) == 0)
				{
					DWORD dwError = GetLastError();
					if(dwError != ERROR_IO_PENDING)
					{
						writeLogError(dwError, L"\n%10.6f: ReadFile() return in COMInToPipeThreadProc() when reading com port: %d\n            ", GetTimeStamp(), dwError);
						Sleep(1);
						continue;
					}
				}
				GetOverlappedResult(hComm, &comEvent, &dwActual, TRUE);
				if(dwActual != dwSize)
				{
					writeLogf(L"\n%10.6f: ReadFile() returned %d bytes from com port in COMInToPipeThreadProc() while expecting %d bytes\n", GetTimeStamp(), dwActual, dwSize);
				}
			}
			else	// I2C mode
			{
				dwCOMEvents = WaitForSingleObject(hI2CNotifyMutex, 50);
				if(dwCOMEvents != WAIT_OBJECT_0)
				{
					continue;
				}

				// Data arrived in the other thread, so copy it to a buffer in our thread and zero the COMInDataLength single variable atomically so the other thread can read some more data
				activityTime = GetTimeStamp();
                for(dwActual = 0; dwActual < COMInDataLength; dwActual++)
                {
                    data[dwActual] = COMInData[dwActual];
                }
				COMInDataLength = 0;
				if(ReleaseMutex(hI2CNotifyMutex) == 0u)
				{
					dwError = GetLastError();
					writeLogError(dwError, L"\n%10.6f: ReleaseMutex() in COMInToPipeThreadProc() returned: %d\n            ", GetTimeStamp(), dwError);
				}
			}

			if(dwActual == 0u)
			{
				continue;
			}


			// Diagnostic display/log what we just received
			if(rxBinLogFile != NULL)	// Write binary data if -rx_bin_log specified on command line
			{
				fwrite(data, 1, dwActual, rxBinLogFile);
			}
			if(displayTXRXData)			// Write human hex ASCII text log if -log specified on command line
			{
				WriteLogData(activityTime, L"RX", dwOffset, data, dwActual);
				dwOffset += dwActual;
			}

			// Write the data to the pipe, assuming someone is connected
			if(pipeConnected)
			{
				dwSize = dwActual;
				if(WriteFile(hPipe, data, dwSize, &dwActual, NULL) == 0)
				{
					DWORD dwError = GetLastError();
					writeLogError(dwError, L"\n%10.6f: WriteFile() return in COMInToPipeThreadProc() when writing to pipe: %d\n            ", GetTimeStamp(), dwError);
					break;
				}
				if(dwActual != dwSize)
				{
					writeLogf(L"\n%10.6f: WriteFile() wrote %d bytes to pipe in COMInToPipeThreadProc() while attempting %d bytes\n", GetTimeStamp(), dwActual, dwSize);
				}
			}
		}

		// Close the pipe
		DisconnectNamedPipe(hPipe);
		CloseHandle(hPipe);

		// Restart loop if -persistent specified; otherwise, terminate.
		if(!persistAfterPipeClose)
			break;
	}

	if(hI2CNotifyMutex != NULL)
		CloseHandle(hI2CNotifyMutex);

	CloseHandle(pipeConnect.hEvent);
	CloseHandle(comEvent.hEvent);
	return 0;
}