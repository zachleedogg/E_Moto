/*
 *******************************************************************************
 * Copyright (c) 2015 Microchip Technology Inc. All rights reserved.
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

import java.io.*;
import java.lang.reflect.*;
import java.net.*;
import java.util.*;
import java.util.logging.Level;
import java.util.logging.Logger;


/**
 *
 * @author Howard Schlunder - C12128
 */
public class Main
{

    /**
     * Loads a java .jar file at run-time, effectively adding it to the class
     * path for accessing external .jar library functions/types/classes.
     *
     * @param filename Path to the .jar to load.
     *
     * @return true if the jar was successfully loaded without triggering any
     *         exceptions. false if an error occurred loading the .jar.
     */
    public static boolean LoadRuntimeJar(String filename)
    {
        if(!Multifunction.FileExists(filename))
            return false;

        try
        {
            Method method = URLClassLoader.class.getDeclaredMethod("addURL", new Class[]
                                                           {
                                                               URL.class
            });
            method.setAccessible(true);
            method.invoke(ClassLoader.getSystemClassLoader(), new Object[]
                  {
                      new File(filename).getCanonicalFile().toURI().toURL()
            });
        }
        catch(IllegalAccessException | IllegalArgumentException | NoSuchMethodException | SecurityException | InvocationTargetException | MalformedURLException ex)
        {
            Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex);
            return false;
        }
        catch(IOException ex)
        {
            Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex);
            return false;
        }

        return true;
    }

    public static void main(String[] args)
    {
        for(int inIndex = 0; inIndex < args.length; inIndex++)
        {
            // Get argument token, normalize to lowercase and if it starts with '--', make it only have one '-'.
            if(args[inIndex].startsWith("--"))
            {
                args[inIndex] = args[inIndex].substring(1);
            }
            String token = args[inIndex].toLowerCase();

            String newArgs[] = Arrays.copyOfRange(args, 1, args.length);

            if(token.startsWith("-blobber"))
            {
                Blobber.main(newArgs);
                return;
            }
            else if(token.startsWith("-gldbuilder"))
            {
                GLDBuilder.main(newArgs);
                return;
            }
            else if(token.startsWith("-communicator"))
            {
                Communicator.main(newArgs);
                return;
            }
            else if(token.startsWith("-make_editor"))
            {
                MakeEditor.main(newArgs);
                return;
            }
            else if(token.startsWith("-dump_parser"))
            {
                DumpParser.main(newArgs);
                return;
            }
            else if(token.startsWith("-objdump_filter"))
            {
                ObjDumpFilter.main(newArgs);
                return;
            }
            else if(token.startsWith("-analyze"))
            {
                Bl2b artifact = Bl2b.GetBl2bFromFile(newArgs[0]);
                String report = PCTraceLogger.StaticAnalysis(artifact);
                if(report == null)
                {
                    System.err.print("Unable to read \"" + newArgs[0] + "\"\n");
                    System.exit(-50);
                }
                System.out.print(report);
                return;
            }
            else if(token.startsWith("-peripheralheadergenerator"))
            {
                String partNumber = "dsPIC33EP512MU810";
                String outputFilename = Multifunction.FixSlashes("internal_files_do_not_distribute/pic_abstraction.h");
                for(String newArg : newArgs)
                {
                    if(newArg.startsWith("-mcpu="))
                    {
                        partNumber = newArg.substring(6);
                    }
                    else if(newArg.startsWith("-output="))
                    {
                        outputFilename = newArg.substring(8);
                    }
                }
                //System.exit(EDCReader.Generate(partNumber, outputFilename));
                //EDCReader.GenLEDPatternBoing("boing.bin");
                EDCReader.GenerateDifferentialSFRList("C:\\Work\\PIC Files\\Latest16Bit", outputFilename, "U1MODE, U2MODE, U3MODE, U4MODE, U5MODE, U6MODE, U7MODE, U8MODE, I2C1CON, I2C2CON, I2C3CON, I2C4CON, I2C5CON, I2C1CONL, I2C2CONL, I2C3CONL, I2C4CONL", "U1RXIE, U2RXIE, U3RXIE, U4RXIE, U5RXIE, U6RXIE, U7RXIE, U1TXIE, U2TXIE, U3TXIE, U4TXIE, U5TXIE, U6TXIE, U7TXIE, U1EIE, U1ERIE, U1ERRIE, SI2C1IE, SI2C2IE, SI2C3IE, SI2C4IE, MI2C1IE, MI2C2IE, MI2C3IE, MI2C4IE");
                //EDCReader.GenerateDifferentialSFRList("C:\\Work\\PIC Files\\Latest16Bit", outputFilename, "I2C1CON, I2C2CON, I2C3CON, I2C4CON, I2C1CONL, I2C2CONL, I2C3CONL, I2C4CONL, I2C1BRG, I2C2BRG, I2C3BRG, I2C4BRG, I2C1BRGL, I2C2BRGL, I2C3BRGL, I2C4BRGL", null);
                //PCTraceLogger trace = new PCTraceLogger();
                //trace.StartTrace("C:\\Work\\ezbl\\ex_boot_uart\\dist\\default\\production\\ex_boot_uart.production.elf",
                //                 "C:\\Work\\ezbl\\ex_boot_uart\\dist\\default\\production\\ex_boot_uart.production.bl2",
                //                 "C:\\Work\\ezbl\\ex_boot_uart\\dist\\default\\production\\trace");
                return;
            }

            // No valid engine argument specified; print help
            System.out.print("\r\n"
                             + Multifunction.FormatHelpText(79, 0 * 3, "Usage:")
                             + Multifunction.FormatHelpText(79, 1 * 3, "java -jar ezbl_tools.jar --engine [engine argument(s)]")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 1 * 3, "Inputs:")
                             + Multifunction.FormatHelpText(79, 2 * 3, "Specify '--blobber' to invoke the .hex blobber tool.")
                             + Multifunction.FormatHelpText(79, 2 * 3, "Specify '--gldbuilder' to invoke the gld linker script builder tool.")
                             + Multifunction.FormatHelpText(79, 2 * 3, "Specify '--communicator' to invoke the communicator tool.")
                             + Multifunction.FormatHelpText(79, 2 * 3, "Specify '--make_editor' to invoke the MPLAB X Makefile auto-editor tool.")
                             + Multifunction.FormatHelpText(79, 2 * 3, "Specify '--dump_parser' to invoke the xc16-objdump parser and data extractor tool.")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 1 * 3, "Options:")
                             + Multifunction.FormatHelpText(79, 2 * 3, "Options are arguments directly passed to the associated engine. Execute an engine with no extra arguments to get its command line help.")
                             + "\r\n");
        }
    }
}
