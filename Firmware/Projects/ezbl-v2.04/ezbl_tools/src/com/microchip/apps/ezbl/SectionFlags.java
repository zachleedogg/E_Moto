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


/**
 *
 * @author C12128
 */
public class SectionFlags implements Serializable, Cloneable
{
    public String wholeString = "";
    public boolean CONTENTS = false;
    public boolean ALLOC = false;
    public boolean LOAD = false;
    public boolean CODE = false;
    public boolean DEBUGGING = false;
    public boolean DATA = false;
    public boolean NEVER_LOAD = false;
    public boolean PERSIST = false;
    public boolean PSV = false;
    public boolean PAGE = false;
    public boolean READONLY = false;
    public boolean EDS = false;
    public boolean RELOC = false;
    public boolean NEAR = false;
    public boolean REVERSE = false;
    public boolean SECURE = false;
    public boolean XMEMORY = false;
    public boolean YMEMORY = false;
    public boolean MEMORY = false; // __external__ memory
    public boolean PACKEDFLASH = false;
    public boolean ABSOLUTE = false;
    public boolean PRESERVED = false;
    public boolean UPDATE = false;
    public String unknown = "";

    public SectionFlags()
    {
    }

    public SectionFlags Clone()
    {
        SectionFlags ret = new SectionFlags();

        ret.ABSOLUTE = this.ABSOLUTE;
        ret.ALLOC = this.ALLOC;
        ret.CODE = this.CODE;
        ret.CONTENTS = this.CONTENTS;
        ret.DATA = this.DATA;
        ret.DEBUGGING = this.DEBUGGING;
        ret.EDS = this.EDS;
        ret.LOAD = this.LOAD;
        ret.MEMORY = this.MEMORY;
        ret.NEAR = this.NEAR;
        ret.NEVER_LOAD = this.NEVER_LOAD;
        ret.PACKEDFLASH = this.PACKEDFLASH;
        ret.PAGE = this.PAGE;
        ret.PERSIST = this.PERSIST;
        ret.PSV = this.PSV;
        ret.READONLY = this.READONLY;
        ret.RELOC = this.RELOC;
        ret.REVERSE = this.REVERSE;
        ret.SECURE = this.SECURE;
        ret.XMEMORY = this.XMEMORY;
        ret.YMEMORY = this.YMEMORY;
        ret.PRESERVED = this.PRESERVED;
        ret.UPDATE = this.UPDATE;
        ret.unknown = this.unknown;
        ret.wholeString = this.wholeString;

        return ret;
    }

    /**
     * @return the wholeString
     */
    public String getWholeString()
    {
        return wholeString;
    }

    /**
     * @param wholeString the wholeString to set
     */
    public void setWholeString(String wholeString)
    {
        this.wholeString = wholeString;
    }

    /**
     * @return the CONTENTS
     */
    public boolean isCONTENTS()
    {
        return CONTENTS;
    }

    /**
     * @param CONTENTS the CONTENTS to set
     */
    public void setCONTENTS(boolean CONTENTS)
    {
        this.CONTENTS = CONTENTS;
    }

    /**
     * @return the ALLOC
     */
    public boolean isALLOC()
    {
        return ALLOC;
    }

    /**
     * @param ALLOC the ALLOC to set
     */
    public void setALLOC(boolean ALLOC)
    {
        this.ALLOC = ALLOC;
    }

    /**
     * @return the LOAD
     */
    public boolean isLOAD()
    {
        return LOAD;
    }

    /**
     * @param LOAD the LOAD to set
     */
    public void setLOAD(boolean LOAD)
    {
        this.LOAD = LOAD;
    }

    /**
     * @return the CODE
     */
    public boolean isCODE()
    {
        return CODE;
    }

    /**
     * @param CODE the CODE to set
     */
    public void setCODE(boolean CODE)
    {
        this.CODE = CODE;
    }

    /**
     * @return the DEBUGGING
     */
    public boolean isDEBUGGING()
    {
        return DEBUGGING;
    }

    /**
     * @param DEBUGGING the DEBUGGING to set
     */
    public void setDEBUGGING(boolean DEBUGGING)
    {
        this.DEBUGGING = DEBUGGING;
    }

    /**
     * @return the DATA
     */
    public boolean isDATA()
    {
        return DATA;
    }

    /**
     * @param DATA the DATA to set
     */
    public void setDATA(boolean DATA)
    {
        this.DATA = DATA;
    }

    /**
     * @return the NEVER_LOAD
     */
    public boolean isNEVER_LOAD()
    {
        return NEVER_LOAD;
    }

    /**
     * @param NEVER_LOAD the NEVER_LOAD to set
     */
    public void setNEVER_LOAD(boolean NEVER_LOAD)
    {
        this.NEVER_LOAD = NEVER_LOAD;
    }

    /**
     * @return the PERSIST
     */
    public boolean isPERSIST()
    {
        return PERSIST;
    }

    /**
     * @param PERSIST the PERSIST to set
     */
    public void setPERSIST(boolean PERSIST)
    {
        this.PERSIST = PERSIST;
    }

    /**
     * @return the PSV
     */
    public boolean isPSV()
    {
        return PSV;
    }

    /**
     * @param PSV the PSV to set
     */
    public void setPSV(boolean PSV)
    {
        this.PSV = PSV;
    }

    /**
     * @return the PAGE
     */
    public boolean isPAGE()
    {
        return PAGE;
    }

    /**
     * @param PAGE the PAGE to set
     */
    public void setPAGE(boolean PAGE)
    {
        this.PAGE = PAGE;
    }

    /**
     * @return the READONLY
     */
    public boolean isREADONLY()
    {
        return READONLY;
    }

    /**
     * @param READONLY the READONLY to set
     */
    public void setREADONLY(boolean READONLY)
    {
        this.READONLY = READONLY;
    }

    /**
     * @return the EDS
     */
    public boolean isEDS()
    {
        return EDS;
    }

    /**
     * @param EDS the EDS to set
     */
    public void setEDS(boolean EDS)
    {
        this.EDS = EDS;
    }

    /**
     * @return the RELOC
     */
    public boolean isRELOC()
    {
        return RELOC;
    }

    /**
     * @param RELOC the RELOC to set
     */
    public void setRELOC(boolean RELOC)
    {
        this.RELOC = RELOC;
    }

    /**
     * @return the NEAR
     */
    public boolean isNEAR()
    {
        return NEAR;
    }

    /**
     * @param NEAR the NEAR to set
     */
    public void setNEAR(boolean NEAR)
    {
        this.NEAR = NEAR;
    }

    /**
     * @return the REVERSE
     */
    public boolean isREVERSE()
    {
        return REVERSE;
    }

    /**
     * @param REVERSE the REVERSE to set
     */
    public void setREVERSE(boolean REVERSE)
    {
        this.REVERSE = REVERSE;
    }

    /**
     * @return the SECURE
     */
    public boolean isSECURE()
    {
        return SECURE;
    }

    /**
     * @param SECURE the SECURE to set
     */
    public void setSECURE(boolean SECURE)
    {
        this.SECURE = SECURE;
    }

    /**
     * @return the XMEMORY
     */
    public boolean isXMEMORY()
    {
        return XMEMORY;
    }

    /**
     * @param XMEMORY the XMEMORY to set
     */
    public void setXMEMORY(boolean XMEMORY)
    {
        this.XMEMORY = XMEMORY;
    }

    /**
     * @return the YMEMORY
     */
    public boolean isYMEMORY()
    {
        return YMEMORY;
    }

    /**
     * @param YMEMORY the YMEMORY to set
     */
    public void setYMEMORY(boolean YMEMORY)
    {
        this.YMEMORY = YMEMORY;
    }

    /**
     * @return the MEMORY
     */
    public boolean isMEMORY()
    {
        return MEMORY;
    }

    /**
     * @param MEMORY the MEMORY to set
     */
    public void setMEMORY(boolean MEMORY)
    {
        this.MEMORY = MEMORY;
    }

    /**
     * @return the PACKEDFLASH
     */
    public boolean isPACKEDFLASH()
    {
        return PACKEDFLASH;
    }

    /**
     * @param PACKEDFLASH the PACKEDFLASH to set
     */
    public void setPACKEDFLASH(boolean PACKEDFLASH)
    {
        this.PACKEDFLASH = PACKEDFLASH;
    }

    /**
     * @return the ABSOLUTE
     */
    public boolean isABSOLUTE()
    {
        return ABSOLUTE;
    }

    /**
     * @param ABSOLUTE the ABSOLUTE to set
     */
    public void setABSOLUTE(boolean ABSOLUTE)
    {
        this.ABSOLUTE = ABSOLUTE;
    }

    /**
     * @return the unknown
     */
    public String getUnknown()
    {
        return unknown;
    }

    /**
     * @param unknown the unknown to set
     */
    public void setUnknown(String unknown)
    {
        this.unknown = unknown;
    }
}

