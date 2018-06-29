/*
 *******************************************************************************
 * Copyright (c) 2017 Microchip Technology Inc. All rights reserved.
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
import java.util.regex.*;


public class ELFReader
{
    class ELFHeader
    {
        char e_ident[] = new char[16];  // unsigned char e_ident[EI_NIDENT];     // const EI_NIDENT = 16
        int e_type;                     // Elf32_Half - 16-bit unsigned medium integer
        int e_machine;                  // Elf32_Half
        int e_version;                  // Elf32_Word - 32-bit unsigned large integer
        int e_entry;                    // Elf32_Word - // Entry point address
        int e_phoff;                    // Elf32_Addr - 32-bit unsigned program address - // Program Header Table file offset in bytes
        int e_shoff;                    // Elf32_Off  - 32-bit unsigned file offset - // Section Header Table file offset in bytes
        int e_flags;                    // Elf32_Off - // Processor-specific flags associated with the file of type EF_machine_flag
        int e_ehsize;                   // Elf32_Word - // ELF header's size in bytes
        int e_phentsize;                // Elf32_Half - // Size in bytes of one entry in the file's Program Header Table (all entries are the same size)
        int e_phnum;                    // Elf32_Half - // Number of entries in the Program Header Table
        int e_shentsize;                // Elf32_Half - // Size in bytes of one entry in the file's Section Header Table (all entries are the same size)
        int e_shnum;                    // Elf32_Half - // Number of entries in the Section Header Table
        int e_shstrndx;                 // Elf32_Half - // Section Header Table entry index associated with the section name string table
    }   // Elf32_Ehdr


    private final class e_type
    {
        int ET_NONE = 0;
        int ET_REL = 1;
        int ET_EXEC = 2;
        int ET_DYN = 3;
        int ET_CORE = 4;
        int ET_LOPROC = 0xFF00;
        int ET_HIPROC = 0xFFFF;
    }


    public static final class e_machine
    {
        static int EM_NONE = 0;
        //static int EM_M32 = 1;
        //static int EM_SPARC = 2;
        static int EM_386 = 3;
        //static int EM_68K = 4;
        //static int EM_88K = 5;
        //static int EM_860 = 7;
        static int EM_MIPS = 8;     // PIC32 and other MIPS devices
        static int EM_PIC30 = 0x76; // PIC24/dsPIC30/dsPIC33
        static int EM_XC8 = 0xCC;   // PIC16 (not sure if this includes PIC18 as well) made with XC8
    }


    class SectionHeader
    {
        int sh_name;
        int sh_type;
        int sh_flags;
        int sh_addr;
        int sh_offset;
        int sh_size;
        int sh_link;
        int sh_info;
        int sh_addralign;
        int sh_entsize;
    }


    private static final class sh_type
    {
        static int SHT_NULL = 0;
        static int SHT_PROGBITS = 1;
        static int SHT_SYMTAB = 2;
        static int SHT_STRTAB = 3;
        static int SHT_RELA = 4;
        static int SHT_HASH = 5;
        static int SHT_DYNAMIC = 6;
        static int SHT_NOTE = 7;
        static int SHT_NOBITS = 8;
        static int SHT_REL = 9;
        static int SHT_SHLIB = 10;
        static int SHT_DYNSYM = 11;
        static int SHT_LOPROC = 0x70000000;
        static int SHT_HIPROC = 0x7FFFFFFF;
        static int SHT_LOUSER = 0x80000000;
        static int SHT_HIUSER = 0xFFFFFFFF;
    }


    public static int Machine(String elfFilePath)
    {
        elfFilePath = elfFilePath.trim().replaceAll("[\\\\/]+", Matcher.quoteReplacement(File.separator));

        try
        {
            FileInputStream in;
            File f;
            byte[] data;

            f = new File(elfFilePath);
            if(!f.exists())
            {
                return 0;
            }

            in = new FileInputStream(f);
            data = new byte[20];
            in.read(data);
            in.close();

            return (((int)data[18]) & 0xFF) | ((((int)data[19]) & 0xFF) << 8);
        }

        catch(IOException ex)
        {
            return 0;
        }
    }
}

