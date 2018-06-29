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

import java.io.*;
import java.text.Collator;
import java.util.*;


/**
 *
 * @author C12128
 */
public class Symbol implements Serializable, Cloneable, Comparable<Symbol>
{

    public long address = -1;
    public SymbolFlags flags = null;
    public String section = null;
    public long alignment_size = -1;
    public String name = null;
    public String probableFile = null;

    public Symbol()
    {
    }

    public Symbol(String obj_dump_line)
    {
        if((obj_dump_line == null) || (obj_dump_line.length() < 18))
        {
            return;
        }

        //00000000 l    df *ABS*	00000000 C:\Work\mla\TCPIP\Demo App\MPLAB.X\..\..\..\Microchip\TCPIP Stack\UDP.c
        try
        {
            address = Long.parseLong(obj_dump_line.substring(0, 8), 16);
            flags = new SymbolFlags(obj_dump_line.substring(9, 16));
            int alignSpaceOffset = obj_dump_line.indexOf(' ', 17);
            int alignOffset = obj_dump_line.indexOf('\t', 17);
            if(alignOffset == -1 || ((alignSpaceOffset != 1) && (alignSpaceOffset < alignOffset)))  // Should always be a tab, but just in case...
            {
                alignOffset = alignSpaceOffset;
            }
            section = obj_dump_line.substring(17, alignOffset).trim();
            while((obj_dump_line.charAt(alignOffset) == ' ') || (obj_dump_line.charAt(alignOffset) == '\t'))
            {
                alignOffset++;
            }
            alignment_size = Long.parseLong(obj_dump_line.substring(alignOffset, alignOffset + 8), 16);
            name = obj_dump_line.substring(alignOffset + 9);
        }
        catch(Exception ex)
        {
            System.out.printf("ezbl_tools.jar: unable to convert symbol line:"
                    + "\n                \"%1$s\"\n", obj_dump_line);
        }

    }

    public Symbol Clone()
    {
        Symbol ret = new Symbol();

        ret.address = this.address;
        ret.flags = this.flags.Clone();
        ret.section = this.section;
        ret.alignment_size = this.alignment_size;
        ret.name = this.name;
        ret.probableFile = this.probableFile;

        return ret;
    }

    public boolean equals(Symbol symbol, boolean testSection)
    {
        boolean match = (this.address == symbol.address) && this.flags.equals(symbol.flags) && (this.alignment_size == symbol.alignment_size) && this.name.equals(symbol.name);
        if(!match)
        {
            return false;
        }
        if(!testSection)
        {
            return true;
        }
        return this.section.equals(symbol.section);
    }

    @Override
    public int compareTo(Symbol y)
    {
        return this.address == y.address ? 0 : (this.address > y.address ? 1 : -1);
    }

    /**
     * Checks if the current symbol is reasonable to expose to the Application
     * project as a weak equ.
     *
     * @return true if all internal conditions are met, false otherwise.
     *         Presently, symbols must meet all of these criteria: - not be
     *         local - must be a function, object, or contain "EZBL" in its name
     *         - name can't start with two or more underscore characters
     */
    public boolean isExportSuitable()
    {
        // Do not export local symbols
        if(this.flags.local)
        {
            return false;
        }

        // Do not export symbols that start with double or tripple
        // underscores (ex: __reset(), __T1Interrupt(), __c30_signature, __c30_info, etc.)
        if(this.name.startsWith("__"))
        {
            return false;
        }

        // Do not export special symbols
        if(this.flags.file || this.flags.relocatableEvaluated || this.flags.warning || this.flags.dynamic || this.flags.indirectReference)
        {
            return false;
        }

        // Do no export undefined (*UND* section) symbols
        if(this.section.equalsIgnoreCase("*UND*"))
        {
            return false;
        }

        // Do not export symbols which can't be encoded as a normal symbol in a .s file (ex: EZBL_ICD_RAM_RESERVE%GAPS, or other ones generated by the compiler/linker, not code)
        if(!this.name.matches("[A-Za-z_][A-Za-z0-9_]*"))
        {
            return false;
        }

        // Do not export absolute symbols unless they start with EZBL
        if(this.section.equalsIgnoreCase("*ABS*") && !this.name.contains("EZBL"))
        {
            return false;
        }

        return true;
    }

    // Retrieves the first symbol that matches the specified name. Returns null if no such symbol found in the provided symbol list.
    static Symbol GetSymbolByName(List<Symbol> symbols, String symbolName)
    {
        Symbol weakVersion = null;
        Symbol s;
        int symCount = symbols.size();

        for(int i = 0; i < symCount; i++)
        {
            s = symbols.get(i);
            if(s.name.equals(symbolName))
            {
                if(s.flags.weak)
                {
                    weakVersion = s;
                }
                else if(!s.section.equals("*UND*"))    // Don't evalute symbol value if it is in the *UND* section, meaning undefined and set elsewhere
                {
                    return s;
                }
            }
        }

        return weakVersion;
    }

    @Override
    public String toString()
    {
        return String.format("%1$08X %2$s %3$s\t%4$08X %5$s\n", this.address, this.flags, this.section, this.alignment_size, this.name);
    }
}


/**
 * Compares symbols by alphabetical name, not addr
 */
class SymbolNameComparator implements Comparator<Symbol>
{
    @Override
    public int compare(Symbol x, Symbol y)
    {
        return Collator.getInstance().compare(x.name, y.name);
    }
}
