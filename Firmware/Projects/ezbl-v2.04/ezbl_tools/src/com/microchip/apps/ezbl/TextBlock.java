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
public class TextBlock implements Serializable, Cloneable
{
    public String data = "";
    public int foundOuterStartIndex = 0;           // Inclusive of all matched block data (i.e. immediately left of outer block start regex)
    public int foundOuterEndIndex = 0;             // Inclusive of all matched block data (i.e. immediately right of outer block end regex)
    public int foundInnerStartIndex = 0;           // Exclusive of matched inner block data (i.e. immediately right of inner block start regex)
    public int foundInnerEndIndex = 0;             // Exclusive of matched inner block data (i.e. immediately left of inner block end regex)
    public int lastInnerBlockStartMatchLength = 0; // Number of matched characters by the inner block start regex (0 for single, non-block searches)
    public int lastInnerBlockEndMatchLength = 0;   // Number of matched characters by the inner block end regex (0 for single, non-block searches)
    public String lastOuterBlockStartRegex = null;
    public String lastOuterBlockEndRegex = null;
    public String lastInnerBlockStartRegex = null;
    public String lastInnerBlockEndRegex = null;
    public Pattern lastOuterBlockStartPattern = null;
    public Pattern lastOuterBlockEndPattern = null;

    /**
     * Initializes a TextBlock class with an empty string (i.e.: "").
     */
    public TextBlock()
    {
    }

    /**
     * Initializes a TextBlock class with the supplied text.
     *
     * @param blockText String to initialize the TextBlock with. If null, an
     *                  empty string (i.e. "") will be initialized into the
     *                  class instead.
     */
    public TextBlock(String blockText)
    {
        if(blockText != null)
        {
            data = blockText;
        }
    }

    /**
     * @return the data
     */
    public String getData()
    {
        return data;
    }

    /**
     * @param data the data to set
     */
    public void setData(String data)
    {
        this.data = data;
    }

    /**
     * @return the foundOuterStartIndex
     */
    public int getFoundOuterStartIndex()
    {
        return foundOuterStartIndex;
    }

    /**
     * @param foundOuterStartIndex the foundOuterStartIndex to set
     */
    public void setFoundOuterStartIndex(int foundOuterStartIndex)
    {
        this.foundOuterStartIndex = foundOuterStartIndex;
    }

    /**
     * @return the foundOuterEndIndex
     */
    public int getFoundOuterEndIndex()
    {
        return foundOuterEndIndex;
    }

    /**
     * @param foundOuterEndIndex the foundOuterEndIndex to set
     */
    public void setFoundOuterEndIndex(int foundOuterEndIndex)
    {
        this.foundOuterEndIndex = foundOuterEndIndex;
    }

    /**
     * @return the foundInnerStartIndex
     */
    public int getFoundInnerStartIndex()
    {
        return foundInnerStartIndex;
    }

    /**
     * @param foundInnerStartIndex the foundInnerStartIndex to set
     */
    public void setFoundInnerStartIndex(int foundInnerStartIndex)
    {
        this.foundInnerStartIndex = foundInnerStartIndex;
    }

    /**
     * @return the foundInnerEndIndex
     */
    public int getFoundInnerEndIndex()
    {
        return foundInnerEndIndex;
    }

    /**
     * @param foundInnerEndIndex the foundInnerEndIndex to set
     */
    public void setFoundInnerEndIndex(int foundInnerEndIndex)
    {
        this.foundInnerEndIndex = foundInnerEndIndex;
    }

    /**
     * @return the lastInnerBlockStartMatchLength
     */
    public int getLastInnerBlockStartMatchLength()
    {
        return lastInnerBlockStartMatchLength;
    }

    /**
     * @param lastInnerBlockStartMatchLength the lastInnerBlockStartMatchLength
     *                                       to set
     */
    public void setLastInnerBlockStartMatchLength(int lastInnerBlockStartMatchLength)
    {
        this.lastInnerBlockStartMatchLength = lastInnerBlockStartMatchLength;
    }

    /**
     * @return the lastInnerBlockEndMatchLength
     */
    public int getLastInnerBlockEndMatchLength()
    {
        return lastInnerBlockEndMatchLength;
    }

    /**
     * @param lastInnerBlockEndMatchLength the lastInnerBlockEndMatchLength to
     *                                     set
     */
    public void setLastInnerBlockEndMatchLength(int lastInnerBlockEndMatchLength)
    {
        this.lastInnerBlockEndMatchLength = lastInnerBlockEndMatchLength;
    }

    /**
     * @return the lastOuterBlockStartRegex
     */
    public String getLastOuterBlockStartRegex()
    {
        return lastOuterBlockStartRegex;
    }

    /**
     * @param lastOuterBlockStartRegex the lastOuterBlockStartRegex to set
     */
    public void setLastOuterBlockStartRegex(String lastOuterBlockStartRegex)
    {
        this.lastOuterBlockStartRegex = lastOuterBlockStartRegex;
    }

    /**
     * @return the lastOuterBlockEndRegex
     */
    public String getLastOuterBlockEndRegex()
    {
        return lastOuterBlockEndRegex;
    }

    /**
     * @param lastOuterBlockEndRegex the lastOuterBlockEndRegex to set
     */
    public void setLastOuterBlockEndRegex(String lastOuterBlockEndRegex)
    {
        this.lastOuterBlockEndRegex = lastOuterBlockEndRegex;
    }

    /**
     * @return the lastInnerBlockStartRegex
     */
    public String getLastInnerBlockStartRegex()
    {
        return lastInnerBlockStartRegex;
    }

    /**
     * @param lastInnerBlockStartRegex the lastInnerBlockStartRegex to set
     */
    public void setLastInnerBlockStartRegex(String lastInnerBlockStartRegex)
    {
        this.lastInnerBlockStartRegex = lastInnerBlockStartRegex;
    }

    /**
     * @return the lastInnerBlockEndRegex
     */
    public String getLastInnerBlockEndRegex()
    {
        return lastInnerBlockEndRegex;
    }

    /**
     * @param lastInnerBlockEndRegex the lastInnerBlockEndRegex to set
     */
    public void setLastInnerBlockEndRegex(String lastInnerBlockEndRegex)
    {
        this.lastInnerBlockEndRegex = lastInnerBlockEndRegex;
    }

    /**
     * @return the lastOuterBlockStartPattern
     */
    public Pattern getLastOuterBlockStartPattern()
    {
        return lastOuterBlockStartPattern;
    }

    /**
     * @param lastOuterBlockStartPattern the lastOuterBlockStartPattern to set
     */
    public void setLastOuterBlockStartPattern(Pattern lastOuterBlockStartPattern)
    {
        this.lastOuterBlockStartPattern = lastOuterBlockStartPattern;
    }

    /**
     * @return the lastOuterBlockEndPattern
     */
    public Pattern getLastOuterBlockEndPattern()
    {
        return lastOuterBlockEndPattern;
    }

    /**
     * @param lastOuterBlockEndPattern the lastOuterBlockEndPattern to set
     */
    public void setLastOuterBlockEndPattern(Pattern lastOuterBlockEndPattern)
    {
        this.lastOuterBlockEndPattern = lastOuterBlockEndPattern;
    }


    /**
     * Searches the TextBlock for the given regular expression. The search is
     * made, starting at the last found outer end index (or start of the text
     * block if the search is being performed for the first time since class
     * initialization/ResetFind() call). If found, both the inner and outer
     * blocks will point to the matched regular expression and both inner and
     * outer delimiter lengths will be set to 0 (i.e. excludeDelimiter,
     * rightOfDelimiter, and leftOfDelimiter options becomes don't-cares).
     *
     * @param regex Regular expression to search for in the TextBlock
     *
     * @return true if the regular expression is found, false otherwise. If not
     *         found, the search position and state are not changed.
     */
    boolean Find(String regex)
    {
        Matcher m;
        int searchStartOffset = foundOuterEndIndex;
        Pattern p = Pattern.compile("(" + regex + ")");

        m = p.matcher(data.substring(searchStartOffset));
        if(!m.find())
        {
            return false;
        }
        foundOuterStartIndex = searchStartOffset + m.start();
        foundOuterEndIndex = searchStartOffset + m.end();
        foundInnerStartIndex = foundOuterStartIndex;
        foundInnerEndIndex = foundOuterEndIndex;
        lastInnerBlockStartMatchLength = 0;
        lastInnerBlockEndMatchLength = 0;
        return true;
    }

    /**
     * Searches the TextBlock for an inner block. The search is
     * made, starting at the last found outer end index (or start of the text
     * block if the search is being performed for the first time since class
     * initialization/ResetFind() call).
     *
     * @param regex Regular expression to search for in the TextBlock
     *
     * @return true if the regular expression is found, false otherwise.
     */
    boolean Find(String regexInnerBlockStart, String regexInnerBlockEnd)
    {
        return Find(foundOuterEndIndex, null, regexInnerBlockStart, regexInnerBlockEnd, null);
    }

    boolean Find(String regexOuterBlockStart, String regexInnerBlockStart, String regexInnerBlockEnd)
    {
        return Find(foundOuterEndIndex, regexOuterBlockStart, regexInnerBlockStart, regexInnerBlockEnd, null);
    }

    boolean Find(String regexOuterBlockStart, String regexInnerBlockStart, String regexInnerBlockEnd, String regexOuterBlockEnd)
    {
        return Find(foundOuterEndIndex, regexOuterBlockStart, regexInnerBlockStart, regexInnerBlockEnd, regexOuterBlockEnd);
    }

    /**
     * Searches the TextBlock for up to four simultaneous, sequential regular
     * expressions. Two of the regular expressions specify an outer block, while
     * the other two specify an inner block. The outer block ending regular
     * expression ignores all text found that matches the inner block criteria
     * to allow nested inner searches of like-termination. For example, given a
     * TextBlock string containing:
     * UNIMPORTANT-TOKENS
     * OUTER-BLOCK-TOKENS
     * {
     * INNER-BLOCK-1-TOKENS
     * {
     * FIND-TOKENS
     * }
     * INNER-BLOCK-2-TOKENS
     * {
     * MORE-FIND-TOKENS
     * }
     * INNER-BLOCK-3-TOKENS
     * {
     * YET-MORE-FIND-TOKENS
     * }
     * }
     * MORE-UNIMPORTANT-TOKENS
     *
     * If you want to locate the MORE-FIND-TOKENS containing block, which are
     * known to be inside braces (just like the outer block braces), you would
     * specify a search criteria of:
     * outerBlockStartRegEx = OUTER-BLOCK-TOKENS\\s*\\{
     * innerBlockStartRegEx = INNER-BLOCK-2-TOKENS\\s*\\{
     * innerBlockEndRegEx = \\}
     * outerBlockEndRegEx = \\}
     * In this case, the outerBlockEndRegEx would locate the final closing brace
     * (just before MORE-UNIMPORTANT-TOKENS), not the closing brace belonging to
     * the INNER-BLOCK-1-TOKENS, INNER-BLOCK-2-TOKENS or INNER-BLOCK-3-TOKENS
     * blocks.
     *
     * @param startOffset          Character offset from the start of the string
     *                             to begin searching.
     *
     * @param regexOuterBlockStart Regular expression to define the beginning of
     *                             the outer block.
     *
     * @param regexInnerBlockStart Regular expression to define the beginning of
     *                             the inner block.
     *
     * @param regexInnerBlockEnd   Regular expression to define the end of
     *                             the inner block.
     *
     * @param regexOuterBlockEnd   Regular expression to define the end of
     *                             the outer block.
     *
     * @return true if all four regular expressions are found, false otherwise.
     */
    boolean Find(int startOffset, String regexOuterBlockStart, String regexInnerBlockStart, String regexInnerBlockEnd, String regexOuterBlockEnd)
    {
        Matcher m;

        lastInnerBlockStartRegex = regexInnerBlockStart;
        lastInnerBlockEndRegex = regexInnerBlockEnd;

        if(foundOuterEndIndex < 0)
        {
            return false;
        }

        if(regexOuterBlockStart == null)
        {
            lastOuterBlockStartPattern = null;
        }
        else if(!regexOuterBlockStart.equals(lastOuterBlockStartRegex))
        {
            lastOuterBlockStartRegex = regexOuterBlockStart;
            lastOuterBlockStartPattern = Pattern.compile(regexOuterBlockStart);
        }
        if(regexOuterBlockEnd == null)
        {
            lastOuterBlockEndPattern = null;
        }
        else if(!regexOuterBlockEnd.equals(lastOuterBlockEndRegex))
        {
            lastOuterBlockEndRegex = regexOuterBlockEnd;
            lastOuterBlockEndPattern = Pattern.compile(regexOuterBlockEnd);
        }

        foundOuterStartIndex = startOffset;
        foundInnerStartIndex = foundOuterStartIndex;
        if(lastOuterBlockStartPattern != null) // If an outer block start regex is specified, first find it
        {
            m = lastOuterBlockStartPattern.matcher(data.substring(foundOuterStartIndex));
            if(!m.find())
            {
                return false;
            }

            foundInnerStartIndex = foundOuterStartIndex + m.end();
            foundOuterStartIndex += m.start();
        }

        int foundDelimiters = 0;
        boolean openComplete = false, closeComplete = false;
        int initialStartOffset = foundInnerStartIndex;

        Pattern p = Pattern.compile("(" + regexInnerBlockStart + ")|(" + regexInnerBlockEnd + ")");
        m = p.matcher(data.substring(initialStartOffset));
        while(m.find())
        {
            if(m.group(1) != null)
            {
                foundDelimiters++;
                if(openComplete == false)
                {
                    openComplete = true;
                    foundInnerStartIndex = initialStartOffset + m.end();
                    lastInnerBlockStartMatchLength = m.group().length();
                    if(lastOuterBlockStartPattern == null)
                    {
                        foundOuterStartIndex = foundInnerStartIndex - lastInnerBlockStartMatchLength;
                    }
                }
            }
            else
            {
                // Ignore matches on close delimiter until we find the first open delimiter
                if(openComplete == false)
                {
                    continue;
                }

                foundDelimiters--;
                lastInnerBlockEndMatchLength = m.group().length();
            }

            if(foundDelimiters == 0)
            {
                closeComplete = true;
                foundInnerEndIndex = initialStartOffset + m.start();
                foundOuterEndIndex = initialStartOffset + m.end();
                break;
            }
        }
        if(!closeComplete)
        {
            foundOuterEndIndex = -1;
            return false;
        }

        if(regexOuterBlockEnd != null) // If an outer block end is specified, finally locate it.
        {
            m = lastOuterBlockEndPattern.matcher(data.substring(foundOuterEndIndex));
            if(!m.find())
            {
                foundOuterEndIndex = -1;
                return false;
            }
            foundOuterEndIndex += m.end();
        }

        return true;
    }

    /**
     * Repeats the last Find() operation with the start index set just after the
     * last regex matched outer block end characters.
     */
    boolean FindNext()
    {
        return Find(foundOuterEndIndex, lastOuterBlockStartRegex, lastInnerBlockStartRegex, lastInnerBlockEndRegex, lastOuterBlockEndRegex);
    }


    /**
     * Resets the TextBlock search state so the next Find() call will start
     * searching from the beginning of the entire TextBlock.
     */
    void ResetFind()
    {
        foundOuterStartIndex = 0;
        foundInnerStartIndex = 0;
        foundInnerEndIndex = 0;
        foundOuterEndIndex = 0;
    }

    /**
     * Replaces the last matched outer text block, INCLUDING the outer matched
     * regex portions, with the specified string.
     */
    void ReplaceOuter(String newText)
    {
        data = data.substring(0, foundOuterStartIndex) + newText + data.substring(foundOuterEndIndex);
        foundOuterEndIndex = foundOuterStartIndex + newText.length();
        foundInnerStartIndex = foundOuterStartIndex;
        foundInnerEndIndex = foundOuterEndIndex;
    }

    /**
     * Replaces the inner text block, NOT INCLUDING the inner matched regex
     * portions, with the specified string.
     */
    void ReplaceInner(String newText)
    {
        int sizeDiff = newText.length() - (foundInnerEndIndex - foundInnerStartIndex);

        data = data.substring(0, foundInnerStartIndex) + newText + data.substring(foundInnerEndIndex);
        foundInnerEndIndex += sizeDiff;
        foundOuterEndIndex += sizeDiff;
    }

    /**
     * Replaces the last matched outer text block, INCLUDING the outer matched
     * regex portions, with an empty string.
     */
    void DeleteOuter()
    {
        data = data.substring(0, foundOuterStartIndex) + data.substring(foundOuterEndIndex);
        foundOuterEndIndex = foundOuterStartIndex;
        foundInnerStartIndex = foundOuterStartIndex;
    }

    /**
     * Replaces the inner text block, NOT INCLUDING the inner matched regex
     * portions, with an empty string.
     */
    void DeleteInner()
    {
        data = data.substring(0, foundInnerStartIndex) + data.substring(foundInnerEndIndex);
        foundOuterEndIndex -= foundInnerEndIndex - foundInnerStartIndex;
        foundInnerEndIndex = foundInnerStartIndex + 1;
    }

    /**
     * Sets the entire data contained within and searched by the TextBlock.
     * Changing the complete block text resets all searches to the beginning
     * (same as ResetFind() does).
     */
    void ReplaceFullBlock(String newBlockData)
    {
        data = newBlockData;
        ResetFind();
    }

    /**
     * Retrieves the entire data contained within and searched by the TextBlock.
     * This text does not depend on the search state.
     */
    String GetFullBlock()
    {
        return data;
    }

    /**
     * Retrieves the last matched outer text block, including the regex matched
     * characters. This is not the entire data of the TextBlock.
     */
    String GetOuter()
    {
        return data.substring(foundOuterStartIndex, foundOuterEndIndex);
    }

    /**
     * Retrieves the last matched inner text block, optionally excluding the
     * regex matched characters from the inner block search.
     */
    String GetInner(boolean excludeDelimiters)
    {
        if(excludeDelimiters)
        {
            return data.substring(foundInnerStartIndex, foundInnerEndIndex);
        }
        return data.substring(foundInnerStartIndex - lastInnerBlockStartMatchLength, foundInnerEndIndex + lastInnerBlockEndMatchLength);
    }


    /**
     * Retrieves the last matched outer block text located between the outer
     * block start and inner block start. Optionally, the regex matched
     * characters from the outer block start search is excluded.
     */
    String GetOuterLeft(boolean excludeDelimiter)
    {
        if(excludeDelimiter)
        {
            return data.substring(foundOuterStartIndex, foundInnerStartIndex - lastInnerBlockStartMatchLength);
        }
        return data.substring(foundOuterStartIndex, foundInnerStartIndex);
    }

    /**
     * Retrieves the last matched outer block text located between the inner
     * block end and the outer block end. Optionally, the regex matched
     * characters from the outer block end search is excluded.
     */
    String GetOuterRight(boolean excludeDelimiter)
    {
        if(excludeDelimiter)
        {
            return data.substring(foundInnerEndIndex + lastInnerBlockStartMatchLength, foundOuterEndIndex);
        }
        return data.substring(foundInnerEndIndex, foundOuterEndIndex);
    }

    /**
     * Retrieves the character index of the last matched outer block matching
     * regex characters.
     */
    int StartOuter()
    {
        return foundOuterStartIndex;
    }

    /**
     * Retrieves the character index of the last matched inner block. Either the
     * index of the start of the inner regex or the index of the last matched
     * inner regex character can be returned.
     */
    int StartInner(boolean rightOfDelimiter)
    {
        if(rightOfDelimiter)
        {
            return foundInnerStartIndex;
        }
        return foundInnerStartIndex - lastInnerBlockStartMatchLength;
    }

    /**
     * Retrieves the character index of the end of the last matched outer block.
     * This index is to the immediate right of the regex matched characters for
     * the outer block end.
     */
    int EndOuter()
    {
        return foundOuterEndIndex;
    }

    /**
     * Retrieves the character index at the end of the last matched inner block.
     * Either the index of the first inner regex end character or the index of
     * the first unmatched character after the inner regex end match can be
     * returned.
     */
    int EndInner(boolean leftOfDelimiter)
    {
        if(leftOfDelimiter)
        {
            return foundInnerEndIndex;
        }
        return foundInnerEndIndex + lastInnerBlockEndMatchLength;
    }
}

