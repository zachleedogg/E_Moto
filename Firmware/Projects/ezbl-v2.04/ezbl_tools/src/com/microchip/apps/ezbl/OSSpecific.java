/*
 *******************************************************************************
 * Copyright (c) 2016 Microchip Technology Inc. All rights reserved.
 *
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital signal
 * controller that is integrated into your product or third party product
 * (pursuant to the sublicense terms in the accompanying license agreement).
 *
 * You should refer to the license agreement accompanying this Software for
 * additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
 * MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED
 * UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF
 * WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR
 * EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT,
 * PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD
 * PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR
 * COSTS.
 *******************************************************************************
 */
package com.microchip.apps.ezbl;

import com.sun.jna.*;
import com.sun.jna.win32.*;
import java.util.*;


/**
 *
 * @author C12128
 */
public class OSSpecific
{
    private interface Kernel32 extends StdCallLibrary
    {
        Kernel32 INSTANCE = (Kernel32)Native.loadLibrary("kernel32", Kernel32.class);
        Kernel32 SYNC_INSTANCE = (Kernel32)Native.synchronizedLibrary(INSTANCE);

        int GetCurrentProcessId();

        int CreateFileW(char[] lpFileName, int dwDesiredAccess, int dwShareMode, int lpSecurityAttributes, int dwCreationDisposition, int dwFlagsAndAttributes, int hTemplateFile);

        int GetCommConfig(int hCommDev, int lpCC, int lpdwSize);
        // BuildCommDCB
        // SetCommConfig
        // GetCommTimeouts
        // SetCommTimeouts
        // ClearCommError
        // CreateThread
        // ReadFile
        // WriteFile
        // CloseHandle
        // CreateNamedPipe
        // ConnectNamedPipe
        // PeekNamedPipe
        // DisconnectNamedPipe
        // Sleep
        // GetLastError

        int QueryDosDeviceW(char[] lpDeviceName, char[] lpTargetPath, int ucchMax);
    }


//  Unused/incomplete code that might be useful someday. Ideally, ezbl_comm.exe
//  could be compiled as a .dll instead and called directly from this
//  ezbl_tools.jar code rather than by invoking a seperate process. This is,
//  however, not presently implemented because ezbl_tools.jar could be executed
//  in either a 32-bit JRE or a 64-bit JRE, which means the target DLL and all
//  libraries it in turn references must all be compiled as a matching 64-bit or
//  32-bit DLL. It is impossible to mix 32-bit and 64-bit code in a single
//  process.
//
//    private interface EZBL_Comm extends StdCallLibrary
//    {
//        EZBL_Comm com = (EZBL_Comm)Native.loadLibrary("ezbl_comm", EZBL_Comm.class);
//
//        void CloseCOM();
//
//        int OpenCOM(char[] comPort, char[] comSettings);
//
//        int COMInToPipeThreadProc(int comHandle);
//
//        int PipeOutToCOMThreadProc(int comHandle);
//    }
//
//
//    public static int OpenCOM(String comPortFile, String comSettings)
//    {
//        return EZBL_Comm.com.OpenCOM(comPortFile.toCharArray(), comSettings.toCharArray());
//    }
//
//    public static void CloseCOM()
//    {
//        EZBL_Comm.com.CloseCOM();
//    }
//
//    public static int COMInToPipeThreadProc(int comHandle)
//    {
//        return EZBL_Comm.com.COMInToPipeThreadProc(comHandle);
//    }
//
//    public static int PipeOutToCOMThreadProc(int comHandle)
//    {
//        return EZBL_Comm.com.PipeOutToCOMThreadProc(comHandle);
//    }
    /**
     * Enumerates all "Dos Device Names" according to the QueryDosDevice()
     * Windows API. This is a Windows only-function.
     *
     * @return Returns a string array of all of the DOS Device Names. Generally,
     *         when opening local devices as a file, you should prefix their
     *         returned name with "\\.\". Ex: "COM15" may be returned, but since
     *         COM15 could ambiguously be treated as a valid filename instead,
     *         to open the actual COM15 communications port device, use
     *         "\\.\COM15"
     */
    public static List<String> EnumDosDevices()
    {
        return EnumDosDevices(null);
    }

    /**
     * Enumerates all "Dos Device Names" according to the QueryDosDevice()
     * Windows API or various previous mappings for a given device name when
     * lpDeviceName is specified.
     *
     * @param lpDeviceName lpDeviceName to pass to QueryDosDevice(). See Windows
     *                     API documentation for full description.
     *
     * @return Returns a string array of all of the DOS Device Names when
     *         lpDeviceName is null. Otherwise, the current followed by previous
     *         (if any) path mappings.
     */
    public static List<String> EnumDosDevices(String lpDeviceName)
    {
        try
        {
            Kernel32 lib = Kernel32.INSTANCE;
            char[] buffer = new char[32768];
            int ret;
            List<String> devices = new ArrayList<String>();

            do
            {
                // Query the Win32 API
                if(lpDeviceName == null)
                {
                    ret = lib.QueryDosDeviceW(null, buffer, buffer.length);
                }
                else
                {
                    while(lpDeviceName.endsWith("\\"))  // API docs say the lpDeviceName parameter cannot end with a backslash, so trim all of them off the end
                    {
                        lpDeviceName = lpDeviceName.substring(0, lpDeviceName.length() - 1);
                    }
                    ret = lib.QueryDosDeviceW((lpDeviceName + "\0").toCharArray(), buffer, buffer.length);
                }

                // 122 is ERROR_INSUFFICIENT_BUFFER, meaning we need to make buffer bigger to read all the data
                if((ret == 0) && (Native.getLastError() == 122))
                {
                    buffer = new char[buffer.length * 2];   // Double the size of our buffer and try again
                }
            } while((ret == 0) && (Native.getLastError() == 122));

            // All other error codes are unhandled
            if(ret == 0)
            {
                System.err.print("\nQueryDosDevice() error; GetLastError(): " + Native.getLastError() + "\n");
                return null;
            }

            // Split the array of null terminated unicode character strings into a Java String array
            int startIndex = 0;
            for(int i = 0; i < ret - 1; i++)    // String list returned is double null terminated, so to avoid returning an empty string, use -1 offset
            {
                if(buffer[i] == '\0')
                {
                    devices.add(String.valueOf(buffer, startIndex, i - startIndex));
                    startIndex = i + 1;
                }
            }

            return devices;
        }
        catch(RuntimeException e)
        {
            return null;
        }
    }
}

