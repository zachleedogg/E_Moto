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
import java.util.regex.*;


/**
 *
 * @author C12128
 */
public class MemoryRegion implements Serializable, Cloneable, Comparable<MemoryRegion>
{

    public String name;
    public int attribR;   // Read-only                -1 = not set, 0 = absent, 1 = set
    public int attribW;   // Read-Write               -1 = not set, 0 = absent, 1 = set
    public int attribX;   // Executable               -1 = not set, 0 = absent, 1 = set
    public int attribA;   // Allocatable              -1 = not set, 0 = absent, 1 = set
    public int attribI;   // Initialized (also 'L')   -1 = not set, 0 = absent, 1 = set
    public long origin;
    public long length;
    public String comment = null;

    public MemoryRegion Clone()
    {
        MemoryRegion mem2 = new MemoryRegion();

        mem2.name = this.name;
        mem2.attribR = this.attribR;
        mem2.attribW = this.attribW;
        mem2.attribX = this.attribX;
        mem2.attribA = this.attribA;
        mem2.attribI = this.attribI;
        mem2.origin = this.origin;
        mem2.length = this.length;
        mem2.comment = this.comment;

        return mem2;
    }

    public MemoryRegion()
    {
    }

    public MemoryRegion(String gldLine)
    {
        String parseTemp;
        Long multiplier;

        //   data  (a!xr)   : ORIGIN = 0x1000,        LENGTH = 0x1000
        gldLine = gldLine.replaceAll("[\\s]", "");  // Remove all whitespace
        //data(a!xr):ORIGIN=0x1000,LENGTH=0x1000
        Pattern p = Pattern.compile("([\\w]+\\b)([(].*?[)])??[:].*?[=]([0-9xXa-fA-F]+[MKmk]??)[,].*?[=]([0-9xXa-fA-F]+[MKmk]??)", Pattern.DOTALL);
        Matcher m = p.matcher(gldLine);
        if(m.find() == false)
        {
            return;
        }
        name = m.group(1);
        parseTemp = m.group(2); // Get attributes and decode them
        if(parseTemp != null)
        {
            parseTemp = parseTemp.toUpperCase();
            if(parseTemp.matches(".*R.*[!].*"))    // Not Read-only
            {
                attribR = -1;
            }
            else if(parseTemp.matches(".*R.*"))    // Read-only
            {
                attribR = 1;
            }
            if(parseTemp.matches(".*W.*[!].*"))    // Not Writeable
            {
                attribW = -1;
            }
            else if(parseTemp.matches(".*W.*"))    // Writable
            {
                attribW = 1;
            }
            if(parseTemp.matches(".*X.*[!].*"))    // Not Executable
            {
                attribX = -1;
            }
            else if(parseTemp.matches(".*X.*"))    // Executable
            {
                attribX = 1;
            }
            if(parseTemp.matches(".*A.*[!].*"))    // Not Allocatable (loadable)
            {
                attribA = -1;
            }
            else if(parseTemp.matches(".*A.*"))    // Allocatable (loadable)
            {
                attribA = 1;
            }
            if(parseTemp.matches(".*(I|L).*[!].*"))    // Not Initialized
            {
                attribI = -1;
            }
            else if(parseTemp.matches(".*(I|L).*"))    // Initialized
            {
                attribI = 1;
            }
        }

        // Decode ORIGIN and LENGTH fields
        for(int i = 3; i <= 4; i++)
        {
            parseTemp = m.group(i).toUpperCase();
            if(parseTemp.contains("M"))
            {
                multiplier = 1024L * 1024L;
                parseTemp = parseTemp.substring(0, parseTemp.length() - 1);
            }
            else if(parseTemp.contains("K"))
            {
                multiplier = 1024L;
                parseTemp = parseTemp.substring(0, parseTemp.length() - 1);
            }
            else
            {
                multiplier = 1L;
            }

            if(i == 3)
            {
                origin = Long.decode(parseTemp) * multiplier;
            }
            else
            {
                length = Long.decode(parseTemp) * multiplier;
            }
        }
    }

    @Override
    public int compareTo(MemoryRegion y)
    {
        return this.origin == y.origin ? 0 : (this.origin > y.origin ? 1 : -1);
    }

    @Override
    public String toString()
    {
        String nameAttributes;
        String attributes;
        String s;

        nameAttributes = "    " + name + " ";
        attributes = "";
        if(attribA == -1)
        {
            attributes += "a";
        }
        if(attribI == -1)
        {
            attributes += "i";
        }
        if(attribX == -1)
        {
            attributes += "x";
        }
        if(attribR == -1)
        {
            attributes += "r";
        }
        if(attribW == -1)
        {
            attributes += "w";
        }
        if(!attributes.isEmpty())
        {
            attributes += "!";
        }
        if(attribA == 1)
        {
            attributes += "a";
        }
        if(attribI == 1)
        {
            attributes += "i";
        }
        if(attribX == 1)
        {
            attributes += "x";
        }
        if(attribR == 1)
        {
            attributes += "r";
        }
        if(attribW == 1)
        {
            attributes += "w";
        }
        if(!attributes.isEmpty())
        {
            nameAttributes += "(" + attributes + ") ";
        }
        if(comment == null)
        {
            return String.format("%1$-33s: ORIGIN = 0x%2$06X, LENGTH = 0x%3$06X", nameAttributes, origin, length);
        }
        else
        {
            return String.format("%1$-33s: ORIGIN = 0x%2$06X, LENGTH = 0x%3$06X    /* %4$s */", nameAttributes, origin, length, comment);
        }
    }

    /**
     * @return the name
     */
    public String getName()
    {
        return name;
    }

    /**
     * @param name the name to set
     */
    public void setName(String name)
    {
        this.name = name;
    }

    /**
     * @return the attribR
     */
    public int getAttribR()
    {
        return attribR;
    }

    /**
     * @param attribR the attribR to set
     */
    public void setAttribR(int attribR)
    {
        this.attribR = attribR;
    }

    /**
     * @return the attribW
     */
    public int getAttribW()
    {
        return attribW;
    }

    /**
     * @param attribW the attribW to set
     */
    public void setAttribW(int attribW)
    {
        this.attribW = attribW;
    }

    /**
     * @return the attribX
     */
    public int getAttribX()
    {
        return attribX;
    }

    /**
     * @param attribX the attribX to set
     */
    public void setAttribX(int attribX)
    {
        this.attribX = attribX;
    }

    /**
     * @return the attribA
     */
    public int getAttribA()
    {
        return attribA;
    }

    /**
     * @param attribA the attribA to set
     */
    public void setAttribA(int attribA)
    {
        this.attribA = attribA;
    }

    /**
     * @return the attribI
     */
    public int getAttribI()
    {
        return attribI;
    }

    /**
     * @param attribI the attribI to set
     */
    public void setAttribI(int attribI)
    {
        this.attribI = attribI;
    }

    /**
     * @return the origin
     */
    public long getOrigin()
    {
        return origin;
    }

    /**
     * @param origin the origin to set
     */
    public void setOrigin(long origin)
    {
        this.origin = origin;
    }

    /**
     * @return the length
     */
    public long getLength()
    {
        return length;
    }

    /**
     * @param length the length to set
     */
    public void setLength(long length)
    {
        this.length = length;
    }

    /**
     * @return the comment
     */
    public String getComment()
    {
        return comment;
    }

    /**
     * @param comment the comment to set
     */
    public void setComment(String comment)
    {
        this.comment = comment;
    }
}

