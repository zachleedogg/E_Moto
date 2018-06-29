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
import java.util.*;


public class AddressRange implements Serializable, Comparable<AddressRange>
{

    public long startAddress;   // Inclusive
    public long endAddress;     // Exclusive (points to 'one' higher than the last element within the address range; 'one' may be 0x2 or some other value if the underlying architecture is intrinsically aligned to some minimum address increment)

    public AddressRange()
    {
        startAddress = 0;
        endAddress = 0;
    }

    public AddressRange(long startAddress_inclusive, long endAddress_exclusive)
    {
        startAddress = startAddress_inclusive;
        endAddress = endAddress_exclusive;
    }

    public AddressRange(long startAddress_inclusive, int length)
    {
        startAddress = startAddress_inclusive;
        endAddress = startAddress + length;
    }

    public AddressRange(String startAndEndAddresses)    // Format should be "[xxx=]0x00000000,0x12345678", where xxx are any number of don't care characters, 0x00000000 is the range start address, inclusive, and 0x12345678 is the range end address, exclusive.
    {
        String addresses[];

        // Trim outter quotes if string is passed in as: "-ignore=0x123,0xABC"
        startAndEndAddresses = Multifunction.TrimQuotes(startAndEndAddresses);

        // Trim everything up to and including an equal sign, if there is one
        if(startAndEndAddresses.contains("="))
        {
            startAndEndAddresses = startAndEndAddresses.replaceFirst(".*[=]", "");
        }

        // Trim quotes again in case if it was passed in as: -ignore="0x123,0xABC"
        startAndEndAddresses = Multifunction.TrimQuotes(startAndEndAddresses);

        // Trim brackets
        startAndEndAddresses = startAndEndAddresses.replace("[", "");
        startAndEndAddresses = startAndEndAddresses.replace("(", "");
        startAndEndAddresses = startAndEndAddresses.replace("{", "");
        startAndEndAddresses = startAndEndAddresses.replace("]", "");
        startAndEndAddresses = startAndEndAddresses.replace(")", "");
        startAndEndAddresses = startAndEndAddresses.replace("}", "");

        // Remove leading and trailing whitespace
        startAndEndAddresses = startAndEndAddresses.trim();

        // Split addresses on comma or whitespace
        addresses = startAndEndAddresses.split("(\\s*,\\s*)|,|(\\s+)", 2);
        startAddress = Long.decode(addresses[0].trim());
        endAddress = Long.decode(addresses[1].trim());
    }

    @Override
    public int compareTo(AddressRange y)
    {
        return this.startAddress == y.startAddress ? 0 : (this.startAddress > y.startAddress ? 1 : -1);
    }

    public AddressRange clone()
    {
        return new AddressRange(this.startAddress, this.endAddress);
    }

    public long getStartAddress()
    {
        return startAddress;
    }

    public void setStartAddress(long startAddress)
    {
        this.startAddress = startAddress;
    }

    public long getEndAddress()
    {
        return endAddress;
    }

    public void setEndAddress(long endAddress)
    {
        this.endAddress = endAddress;
    }

    /**
     * Sorts the list of address ranges and then reduces the list to the Union
     * of all elements. Address ranges that overlap another address range (or
     * perfectly abut each other) are combined into a single address range.
     * Address Ranges of zero length are removed.
     */
    static void SortAndReduce(List<AddressRange> list)
    {
        Collections.sort(list);
        for(int i = 0; i < list.size(); i++)
        {
            AddressRange ar = list.get(i);
            if(ar.startAddress == ar.endAddress)
            {
                list.remove(i--);
                continue;
            }
            for(int j = i + 1; j < list.size(); j++)
            {
                AddressRange ar2 = list.get(j);
                if((ar2.startAddress >= ar.startAddress) && (ar2.startAddress <= ar.endAddress))    // Directly abut or overlap each other
                {
                    if(ar2.endAddress > ar.endAddress)
                    {
                        ar.endAddress = ar2.endAddress;
                    }
                    list.remove(j);
                    j--;
                    continue;
                }
            }
        }
    }

    /**
     * SubtractRange
     *
     * Removes an address range from a list of address ranges, should the given
     * range exist in the list. If the subtraction range lies inside an
     * overlapping range, the non overlapping regions are split into two ranges
     * with the subtraction range removed.
     *
     * @param rangeList     List of AddressRange elements to be subtracted from.
     *                      This list is modified.
     * @param subtractRange An AddressRange element that should be compared for
     *                      overlap against the AddressRanges in the rangeList,
     *                      and in the event there is overlap the rangeList will
     *                      be shrunken or deleted so that no overlap exists.
     */
    static void SubtractRange(List<AddressRange> rangeList, AddressRange subtractRange)
    {
        for(int i = 0; i < rangeList.size(); i++)
        {
            AddressRange ar = rangeList.get(i);
            if((subtractRange.endAddress > ar.startAddress) && (subtractRange.startAddress < ar.endAddress))    // Overlap exists
            {
                if((subtractRange.startAddress >= ar.startAddress) && (subtractRange.endAddress < ar.endAddress))
                {   // Lies in the middle -> must split reference range into two ranges
                    rangeList.add(i + 1, new AddressRange(subtractRange.endAddress, ar.endAddress));
                    ar.endAddress = subtractRange.startAddress;
                }
                else if((subtractRange.startAddress >= ar.startAddress) && (subtractRange.endAddress >= ar.endAddress))
                {   // Overlaps with right half -> trim the right edge off the reference range
                    ar.endAddress = subtractRange.startAddress;
                }
                else if((subtractRange.startAddress <= ar.startAddress) && (subtractRange.endAddress > ar.startAddress))
                {   // Overlaps with left half -> trim the left edge off the reference range
                    ar.startAddress = subtractRange.endAddress;
                }
                rangeList.set(i, ar);
            }
        }
    }


    /**
     * SubtractRanges
     *
     * Removes all overlapping address ranges from a set of reference ranges.
     *
     * @param referenceRangeList List of AddressRange elements to be
     *                           subtracted from.
     * @param subtractRangeList  List of AddressRange elements that should be
     *                           compared for overlap against the
     *                           referenceRangeList, and in the event there
     *                           is overlap the reference range will be
     *                           shrunken or deleted so that no overlap
     *                           exists. This does not actually modify the
     *                           referenceRangeList elements, but rather
     *                           makes a copy of it before starting any
     *                           subtraction.
     *
     * @return List of AddressRange elements from the reference list, but now
     *         trimmed and modified by the subtracted ranges. i.e.:
     *         return List = referenceRangeList - subtractRangeList;
     */
    static List<AddressRange> SubtractRanges(List<AddressRange> referenceRangeList, List<AddressRange> subtractRangeList)
    {
        List<AddressRange> ret = new ArrayList<AddressRange>();

        // Make a copy of all of the reference range list elements so we can
        // modify them without disturbing the original
        for(int i = 0; i < referenceRangeList.size(); i++)
        {
            ret.add(new AddressRange(referenceRangeList.get(i).startAddress, referenceRangeList.get(i).endAddress));
        }

        Collections.sort(ret);
        for(int i = 0; i < ret.size(); i++)
        {
            AddressRange ar = ret.get(i);
            for(int j = 0; j < subtractRangeList.size(); j++)
            {
                AddressRange ar2 = subtractRangeList.get(j);
                if((ar2.endAddress > ar.startAddress) && (ar2.startAddress < ar.endAddress))    // Overlap exists
                {
                    if((ar2.startAddress >= ar.startAddress) && (ar2.endAddress < ar.endAddress))
                    {   // Lies in the middle -> must split reference range into two ranges
                        ret.add(i + 1, new AddressRange(ar2.endAddress, ar.endAddress));
                        ar.endAddress = ar2.startAddress;
                    }
                    else if((ar2.startAddress >= ar.startAddress) && (ar2.endAddress >= ar.endAddress))
                    {   // Overlaps with right half -> trim the right edge off the reference range
                        ar.endAddress = ar2.startAddress;
                    }
                    else if((ar2.startAddress <= ar.startAddress) && (ar2.endAddress > ar.startAddress))
                    {   // Overlaps with left half -> trim the left edge off the reference range
                        ar.startAddress = ar2.endAddress;
                    }
                    ret.set(i, ar);
                }
            }
        }

        // All subtracted off, now simplify and return
        SortAndReduce(ret);
        return ret;
    }

    /**
     * Subtracts a list of AddressRanges from the addresses occupied by a given
     * list of Sections. Returns the union of address ranges that were common to
     * both lists.
     *
     * @param addressRangeList List of AddressRange elements to be
     *                         subtracted from.
     * @param sectionList      List of Section elements that should be
     *                         compared for overlap against the
     *                         addressRangeList, and in the event there
     *                         is overlap the reference range will be
     *                         shrunken or deleted so that no overlap
     *                         exists. This does not actually modify the
     *                         addressRangeList elements, but rather
     *                         makes a copy of it before starting any
     *                         subtraction.
     *
     * @return List of AddressRange elements from the reference list, but now
     *         trimmed and modified by the subtracted ranges. i.e.:
     *         return List = addressRangeList - sectionList;
     */
    static List<AddressRange> SectionAddressUnion(List<AddressRange> addressRangeList, List<Section> sectionList)
    {
        List<AddressRange> sectionRanges = new ArrayList<AddressRange>();

        for(Section sec : sectionList)
        {
            sectionRanges.add(new AddressRange(sec.loadMemoryAddress, sec.size));
        }

        return SubtractRanges(addressRangeList, sectionRanges);
    }
}

