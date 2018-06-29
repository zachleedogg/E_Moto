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
import java.util.regex.*;


/**
 *
 * @author C12128
 */
public class GLDMemories implements Serializable, Cloneable
{

    public List<MemoryRegion> all = null;
    public List<MemoryRegion> nonvolatile = null;
    public List<MemoryRegion> data = null;
    public List<MemoryRegion> execution = null;
    public List<MemoryRegion> vectors = null;
    public List<MemoryRegion> eeprom = null;
    public List<MemoryRegion> other = null;
    public List<MemoryRegion> test = null;

    public GLDMemories(String gldFileData)
    {
        Pattern p;
        Matcher m;
        String memoryStrings[];

        gldFileData = gldFileData.replaceAll("((?s)/[*].*?[*]/)|(//[^\n]*?\n)", "");  // Remove all comments

        // Find MEMORY declarations
        p = Pattern.compile("\n[\\s]*MEMORY.*?\\{[\\s]*([\\s\\S]*?)\\}", Pattern.DOTALL);
        m = p.matcher(gldFileData);
        if(!m.find())
        {
            System.err.println("Couldn't find MEMORY region in linker script.");
            return;
        }

        all = new ArrayList<MemoryRegion>();
        data = new ArrayList<MemoryRegion>();
        nonvolatile = new ArrayList<MemoryRegion>();
        execution = new ArrayList<MemoryRegion>();
        vectors = new ArrayList<MemoryRegion>();
        eeprom = new ArrayList<MemoryRegion>();
        other = new ArrayList<MemoryRegion>();
        test = new ArrayList<MemoryRegion>();

        // Split each memory region line into a string and add to the memories list
        memoryStrings = m.group(1).trim().split("\n");
        for(String memoryString : memoryStrings)
        {
            MemoryRegion mem = new MemoryRegion(memoryString);
            if(mem.name != null)
            {
                all.add(mem);
            }
        }

        // Split all memories into smaller grouped lists
        for(MemoryRegion mem : all)
        {
            if(mem.attribA < 0) // Data memories are !Allocatable
            {   // Ex: RAM (and peripherals, if they aren't EQUated directly)
                data.add(mem);
            }
            else if(mem.attribX > 0)
            {   // Ex: Main Flash, Aux Flash, boot flash, other executable and allocatable/loadable region
                nonvolatile.add(mem);
                execution.add(mem);
            }
            else if((mem.origin < 0x800) || mem.name.equals("reset") || mem.name.equals("ivt") || mem.name.equals("aivt") || mem.name.equals("_reserved"))
            {   // Ex: reset, ivt, aivt and _reserved (if existent)
                nonvolatile.add(mem);
                vectors.add(mem);
            }
            else if(mem.name.equalsIgnoreCase("eeprom"))
            {   // Ex: eeprom
                nonvolatile.add(mem);
                eeprom.add(mem);
            }
            else if((mem.origin & 0x800000) != 0)
            {   // Ex: Test memories (often config words) outside ordinary Flash have bit 23 of the address set
                nonvolatile.add(mem);
                test.add(mem);
            }
            else
            {   // Ex: Configuration words in Flash
                nonvolatile.add(mem);
                other.add(mem);
            }
        }
    }

    @Override
    public String toString()
    {
        String s = "\n"
                   + "MEMORY\n"
                   + "{\n";
        for(MemoryRegion mem : all)
        {
            s += mem.toString() + "\n";
        }
        s += "}\n";

        return s;
    }

    /**
     * @return the all
     */
    public List<MemoryRegion> getAll()
    {
        return all;
    }

    /**
     * @param all the all to set
     */
    public void setAll(List<MemoryRegion> all)
    {
        this.all = all;
    }

    /**
     * @return the nonvolatile
     */
    public List<MemoryRegion> getNonvolatile()
    {
        return nonvolatile;
    }

    /**
     * @param nonvolatile the nonvolatile to set
     */
    public void setNonvolatile(List<MemoryRegion> nonvolatile)
    {
        this.nonvolatile = nonvolatile;
    }

    /**
     * @return the data
     */
    public List<MemoryRegion> getData()
    {
        return data;
    }

    /**
     * @param data the data to set
     */
    public void setData(List<MemoryRegion> data)
    {
        this.data = data;
    }

    /**
     * @return the execution
     */
    public List<MemoryRegion> getExecution()
    {
        return execution;
    }

    /**
     * @param execution the execution to set
     */
    public void setExecution(List<MemoryRegion> execution)
    {
        this.execution = execution;
    }

    /**
     * @return the vectors
     */
    public List<MemoryRegion> getVectors()
    {
        return vectors;
    }

    /**
     * @param vectors the vectors to set
     */
    public void setVectors(List<MemoryRegion> vectors)
    {
        this.vectors = vectors;
    }

    /**
     * @return the eeprom
     */
    public List<MemoryRegion> getEeprom()
    {
        return eeprom;
    }

    /**
     * @param eeprom the eeprom to set
     */
    public void setEeprom(List<MemoryRegion> eeprom)
    {
        this.eeprom = eeprom;
    }

    /**
     * @return the other
     */
    public List<MemoryRegion> getOther()
    {
        return other;
    }

    /**
     * @param other the other to set
     */
    public void setOther(List<MemoryRegion> other)
    {
        this.other = other;
    }

    /**
     * @return the test
     */
    public List<MemoryRegion> getTest()
    {
        return test;
    }

    /**
     * @param test the test to set
     */
    public void setTest(List<MemoryRegion> test)
    {
        this.test = test;
    }
}

