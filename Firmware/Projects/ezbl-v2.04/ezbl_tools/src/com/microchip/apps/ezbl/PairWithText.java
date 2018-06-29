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
public class PairWithText implements Serializable, Cloneable, Comparable<PairWithText>
{
    public long first;
    public long second;
    public String text;

    public PairWithText(long first, long second, String text)
    {
        this.first = first;
        this.second = second;
        this.text = text;
    }

    public PairWithText(long first, String text)
    {
        this.first = first;
        this.text = text;
    }

    public PairWithText(long first, long second)
    {
        this.first = first;
        this.second = second;
    }

    public PairWithText(String text)
    {
        this.text = text;
    }

    public PairWithText()
    {
    }

    public long getFirst()
    {
        return first;
    }

    public void setFirst(long first)
    {
        this.first = first;
    }

    public long getSecond()
    {
        return second;
    }

    public void setSecond(long second)
    {
        this.second = second;
    }

    public String getText()
    {
        return text;
    }

    public void setText(String text)
    {
        this.text = text;
    }

    /**
     * Compares the 'first' element only as a signed numerical value.
     *
     * @param y PairWithText to compare against
     *
     * @return 0 if both .first elements are equal, -1 if y.first is less than
     *         this' first, or +1 otherwise.
     */
    @Override
    public int compareTo(PairWithText y) // Needed for calling Collections.sort()
    {
        return this.first < y.first ? -1 : this.first == y.first ? 0 : 1;
    }

}

