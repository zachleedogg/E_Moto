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
import java.util.*;


/**
 *
 * @author C12128
 */
/**
 * Class for decoding Symbol flags from an ELF dump. For flag decoding values,
 * see the <a
 * href=http://sourceware.org/binutils/docs/binutils/objdump.html>GNU objdump
 * documentation</a>
 *
 * @param flagString 7 character flags string from the object dump
 *
 * @return Class containing decoded symbols. If the input flagString is null or
 *         of incorrect length, all flags are unset.
 */
public class SymbolFlags implements Serializable, Cloneable
{
    public boolean local = false;                   // l
    public boolean global = false;                  // g
    public boolean uniqueGlobal = false;            // u
    public boolean neitherGlobalNorLocal = false;   // (space)
    public boolean globalAndLocal = false;          // !

    public boolean weak = false;                    // w
    // normal                                       // (space)

    public boolean constructor = false;             // C
    // normal                                       // (space)

    public boolean warning = false;                 // W
    // normal                                       // (space)

    public boolean indirectReference = false;       // I
    public boolean relocatableEvaluated = false;    // i
    // normal                                       // (space)

    public boolean debugging = false;               // d
    public boolean dynamic = false;                 // D
    // normal                                       // (space)

    public boolean function = false;                // F
    public boolean file = false;                    // f
    public boolean object = false;                  // O
    // normal                                       // (space)

    public SymbolFlags()
    {
    }

    public SymbolFlags(String flagString)
    {
        if(flagString == null)
        {
            return;
        }

        if(flagString.length() != 7)
        {
            return;
        }

        char c;
        c = flagString.charAt(0);
        this.local = (c == 'l');
        this.global = (c == 'g');
        this.uniqueGlobal = (c == 'u');
        this.neitherGlobalNorLocal = (c == ' ');
        this.globalAndLocal = (c == '!');

        c = flagString.charAt(1);
        this.weak = (c == 'w');

        c = flagString.charAt(2);
        this.constructor = (c == 'C');

        c = flagString.charAt(3);
        this.warning = (c == 'W');

        c = flagString.charAt(4);
        this.indirectReference = (c == 'I');
        this.relocatableEvaluated = (c == 'i');

        c = flagString.charAt(5);
        this.debugging = (c == 'd');
        this.dynamic = (c == 'D');

        c = flagString.charAt(6);
        this.function = (c == 'F');
        this.file = (c == 'f');
        this.object = (c == 'O');
    }

    public SymbolFlags Clone()
    {
        SymbolFlags ret = new SymbolFlags();

        ret.constructor = this.constructor;
        ret.debugging = this.debugging;
        ret.dynamic = this.dynamic;
        ret.file = this.file;
        ret.function = this.function;
        ret.global = this.global;
        ret.globalAndLocal = this.globalAndLocal;
        ret.indirectReference = this.indirectReference;
        ret.local = this.local;
        ret.neitherGlobalNorLocal = this.neitherGlobalNorLocal;
        ret.object = this.object;
        ret.relocatableEvaluated = this.relocatableEvaluated;
        ret.uniqueGlobal = this.uniqueGlobal;
        ret.warning = this.warning;
        ret.weak = this.weak;

        return ret;
    }

    public boolean equals(SymbolFlags symFlags)
    {
        return (this.constructor == symFlags.constructor)
               && (this.debugging == symFlags.debugging)
               && (this.dynamic == symFlags.dynamic)
               && (this.file == symFlags.file)
               && (this.function == symFlags.function)
               && (this.global == symFlags.global)
               && (this.globalAndLocal == symFlags.globalAndLocal)
               && (this.indirectReference == symFlags.indirectReference)
               && (this.local == symFlags.local)
               && (this.neitherGlobalNorLocal == symFlags.neitherGlobalNorLocal)
               && (this.object == symFlags.object)
               && (this.relocatableEvaluated == symFlags.relocatableEvaluated)
               && (this.uniqueGlobal == symFlags.uniqueGlobal)
               && (this.warning == symFlags.warning)
               && (this.weak == symFlags.weak);
    }

    @Override
    public String toString()
    {
        char flags[] = new char[7];
        for(int i = 0; i < flags.length; i++)
        {
            flags[i] = ' ';
        }

        if(this.local)
        {
            flags[0] = 'l';
        }
        if(this.global)
        {
            flags[0] = 'g';
        }
        if(this.uniqueGlobal)
        {
            flags[0] = 'u';
        }
        if(this.neitherGlobalNorLocal)
        {
            flags[0] = ' ';
        }
        if(this.globalAndLocal)
        {
            flags[0] = '!';
        }

        if(this.weak)
        {
            flags[1] = 'w';
        }

        if(this.constructor)
        {
            flags[2] = 'C';
        }

        if(this.warning)
        {
            flags[3] = 'W';
        }

        if(this.indirectReference)
        {
            flags[4] = 'I';
        }
        if(this.relocatableEvaluated)
        {
            flags[4] = 'i';
        }

        if(this.debugging)
        {
            flags[5] = 'd';
        }
        if(this.dynamic)
        {
            flags[5] = 'D';
        }

        if(this.function)
        {
            flags[6] = 'F';
        }
        if(this.file)
        {
            flags[6] = 'f';
        }
        if(this.object)
        {
            flags[6] = 'O';
        }

        return Arrays.toString(flags);
    }

    public boolean isLocal()
    {
        return local;
    }

    public void setLocal(boolean local)
    {
        this.local = local;
    }

    public boolean isGlobal()
    {
        return global;
    }

    public void setGlobal(boolean global)
    {
        this.global = global;
    }

    public boolean isUniqueGlobal()
    {
        return uniqueGlobal;
    }

    public void setUniqueGlobal(boolean uniqueGlobal)
    {
        this.uniqueGlobal = uniqueGlobal;
    }

    public boolean isNeitherGlobalNorLocal()
    {
        return neitherGlobalNorLocal;
    }

    public void setNeitherGlobalNorLocal(boolean neitherGlobalNorLocal)
    {
        this.neitherGlobalNorLocal = neitherGlobalNorLocal;
    }

    public boolean isGlobalAndLocal()
    {
        return globalAndLocal;
    }

    public void setGlobalAndLocal(boolean globalAndLocal)
    {
        this.globalAndLocal = globalAndLocal;
    }

    public boolean isWeak()
    {
        return weak;
    }

    public void setWeak(boolean weak)
    {
        this.weak = weak;
    }

    public boolean isConstructor()
    {
        return constructor;
    }

    public void setConstructor(boolean constructor)
    {
        this.constructor = constructor;
    }

    public boolean isWarning()
    {
        return warning;
    }

    public void setWarning(boolean warning)
    {
        this.warning = warning;
    }

    public boolean isIndirectReference()
    {
        return indirectReference;
    }

    public void setIndirectReference(boolean indirectReference)
    {
        this.indirectReference = indirectReference;
    }

    public boolean isRelocatableEvaluated()
    {
        return relocatableEvaluated;
    }

    public void setRelocatableEvaluated(boolean relocatableEvaluated)
    {
        this.relocatableEvaluated = relocatableEvaluated;
    }

    public boolean isDebugging()
    {
        return debugging;
    }

    public void setDebugging(boolean debugging)
    {
        this.debugging = debugging;
    }

    public boolean isDynamic()
    {
        return dynamic;
    }

    public void setDynamic(boolean dynamic)
    {
        this.dynamic = dynamic;
    }

    public boolean isFunction()
    {
        return function;
    }

    public void setFunction(boolean function)
    {
        this.function = function;
    }

    public boolean isFile()
    {
        return file;
    }

    public void setFile(boolean file)
    {
        this.file = file;
    }

    public boolean isObject()
    {
        return object;
    }

    public void setObject(boolean object)
    {
        this.object = object;
    }
}

