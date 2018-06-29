// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include "Windows.h"
#include "Share.h"


// reference additional headers your program requires here
#define MCP2221_LIB	// We want to statically link to mcp2221_dll_um.lib instead of run-time link with an actual .dll file
#include "mcp2221_dll_um.h"


// Prototypes for ezbl_comm.cpp
HANDLE InitializeCOMPort(TCHAR *comFile, TCHAR *errorMsg, DWORD errorMsgSize);
HANDLE InitializeI2CPort(int vid, int pid, int instanceNum, int SCLFrequency);
DWORD PipeOutToCOMThreadProc(LPVOID comHandle);
DWORD COMInToPipeThreadProc(LPVOID comHandle);
DWORD PrintError(TCHAR *outBuf, DWORD maxCount, TCHAR *preErrorStr, DWORD winErrorCode);
void PrintError(DWORD errorCode);
void writeLogError(DWORD winErrorCode, TCHAR *format, ...);
void writeLogf(TCHAR *format, ...);
void writeLog(TCHAR *message);
void writeLog(TCHAR *message, LONG messageLen, BOOL flushToMain);
void WriteLogData(double timestamp, TCHAR *type, unsigned int streamOffset, unsigned char *data, unsigned int dataCount);
void FlushLog(void);
double GetTimeStamp();
const TCHAR *GetMCP2221ErrorString(int errorCode);


// Global vars in ezbl_comm.cpp
extern FILE				*logFile;
extern FILE				*rxBinLogFile;
extern int				i2cComNumber;	// 0 means not using I2C interface; 1 means MCP2221 device 1 I2C output, 2 means MCP2221 device 2 I2C output, etc.
extern int				i2cSlaveAddress;
extern BOOL				persistAfterPipeClose;
extern BOOL				displayTXRXData;
extern volatile BYTE	COMInData[4096];	// Transfer buffer for passing RX data from main thread to the dedicated Pipe writing thread
extern volatile DWORD	COMInDataLength;
extern FILE				*logFile;
extern double			estimatedTXTimePerByte;



// Prototypes for PC_to_PIC_thread.cpp
DWORD PipeOutToCOMThreadProc(LPVOID comHandle);

// Prototypes for PIC_to_PC_thread.cpp
DWORD COMInToPipeThreadProc(LPVOID comHandle);

// Prototypes for enum_ports.cpp
int EnumCOMPorts(TCHAR *comStrings, int bufTCHARCount);
