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

import static com.microchip.apps.ezbl.Multifunction.CatStringList;
import static com.microchip.apps.ezbl.Multifunction.FindFiles;
import static com.microchip.apps.ezbl.Multifunction.FindFilesRegEx;
import com.microchip.crownking.edc.*;
import java.io.*;
import java.text.*;
import java.util.*;
import java.util.logging.*;
import java.util.regex.*;
import javax.xml.parsers.*;
import org.w3c.dom.*;
import org.xml.sax.*;


/**
 *
 * @author C12128
 */
public class Multifunction
{
    /**
     * Checks if a file exists on the local file system.
     *
     * @param filePath Path to the local file. Quotes and whitespace are removed
     *                 and slashes/backslashes are converted to the system OS
     *                 dependent folder separator character before checking to
     *                 see if this file exists or not.
     *
     * @return true if the file exists, false otherwise.
     */
    public static boolean FileExists(String filePath)
    {
        filePath = Multifunction.FixSlashes(TrimQuotes(filePath));

        File f;
        f = new File(filePath);
        return f.exists();
    }

    /**
     * Deletes a file from the local file system. If the file does not exists or
     * cannot be removed, false is returned rather than trigger an exception.
     *
     * @param filePath Path to the local file to delete. Quotes and whitespace
     *                 are removed and slashes/backslashes are converted to the
     *                 system OS dependent folder separator character before
     *                 attempting to delete the file.
     *
     * @return true if the file was successfully deleted, false if the file does
     *         not exist, security permissions prevented the erase, the file was
     *         locked, or other I/O error occurs.
     */
    public static boolean DeleteFile(String filePath)
    {
        filePath = Multifunction.FixSlashes(TrimQuotes(filePath));

        File f;
        f = new File(filePath);
        if(!f.exists())
        {
            return false;
        }
        try
        {
            f.delete();
        }
        catch(Exception ex)
        {
            return false;
        }
        return true;
    }

    /**
     * Reads a file and returns a UTF-8 encoded string of it's contents. Maximum
     * supported file length is 2^31-1 bytes (2GB).
     *
     * @param fileName        Filename to open and read the contents of
     * @param convertCRLFtoLF Specifies if, all carriage return (\r) characters
     *                        should be removed before returning the string.
     *                        This normalizes text with \r\n line separators
     *                        into consistent \n only line separators. If false,
     *                        no line separator conversion is done.
     *
     * @return UTF-8 encoded string of the file contents. If the file does not
     *         exist, cannot be read (ex: no permission or file exclusively
     *         locked), or is greater than the 2^31-1 byte limit, then null is
     *         returned.
     */
    public static String ReadFile(String fileName, boolean convertCRLFtoLF)
    {
        byte data[] = ReadFileBinary(fileName);

        if(data == null)
        {
            return null;
        }
        try
        {
            if(convertCRLFtoLF)
            {
                return new String(data, "UTF-8").replaceAll("\r", "");
            }
            else
            {
                return new String(data, "UTF-8");
            }
        }
        catch(UnsupportedEncodingException ex)
        {
            return null;
        }
    }

    /**
     * Reads a file and returns a byte array of it's contents. Maximum supported
     * file length is 2^31-1 bytes (2GB).
     *
     * @param fileName Filename to open and read the contents of
     *
     * @return byte array of the file contents. If the file does not exist,
     *         cannot be read (ex: no permission or file exclusively locked), or
     *         is greater than the 2^31-1 byte limit, then null is returned.
     */
    public static byte[] ReadFileBinary(String fileName)
    {
        fileName = Multifunction.FixSlashes(TrimQuotes(fileName));

        try
        {
            FileInputStream in;
            File f;
            byte[] data;

            f = new File(fileName);
            if(!f.exists())
            {
                return null;
            }

            // File greater than 2^31 - 1 bytes (2GB)?
            if(f.length() > 0x7FFFFFFFL)
            {
                return null;
            }

            in = new FileInputStream(f);
            data = new byte[(int)(f.length() & 0x7FFFFFFF)];
            in.read(data);
            in.close();

            return data;

        }
        catch(IOException ex)
        {
            return null;
        }
    }

    // Compares newContents[] with the traceData stored in fileName, and if unequal, replaces the file with the newContents.
    // Returns 1 if output file didn't exist or exists but was different and therefore replaced with the newContents. The write succeeded.
    // Returns 0 if the output file exists and the traceData in it already exactly matches newContents. The file was not touched (so as not to change the modification date).
    // Returns -1 if the output file exists but couldn't be read, the path was invalid, or the file couldn't be written to (ex: file or file system is attributed read-only, out of space, etc).
    public static int UpdateFileIfDataDifferent(String fileName, byte[] newContents)
    {
        FileOutputStream out;
        File f;

        fileName = Multifunction.FixSlashes(TrimQuotes(fileName));

        // Check if the file already exists
        f = new File(fileName);
        if(f.exists())
        {
            // Check if the length is equal
            if((int)f.length() == newContents.length)
            {
                // Check if every byte of traceData is eqaul
                if(Arrays.equals(ReadFileBinary(fileName), newContents))
                {
                    // Data the same, do not write to the file again and change it's modification date
                    return 0;
                }
            }
        }

        // Write the changed traceData to the file
        try
        {
            out = new FileOutputStream(f);
            out.write(newContents);
            out.close();
            return 1;
        }
        catch(IOException ex)
        {
            return -1;
        }
    }

    /**
     * Writes a binary array of traceData to a given file or stdout. If the file
     * already exists, append is false, and/or isn't stdout, it is deleted
     * before writing starts.
     *
     * @param fileName File path to write to
     * @param data     Array of binary traceData to write
     * @param append   true to append to an existing file, if it exists. false
     *                 deletes the existing file and creates a new one This
     *                 parameter is ignored when writing to stdout since this
     *                 can always only be an append operation
     *
     * @return 1 if writing was successful. -1 if an I/O exception occurred (ex:
     *         file exists and is read-only, security access denied, file
     *         locked, drive failure, illegal file name)
     */
    public static int WriteFile(String fileName, byte[] data, boolean append)
    {
        FileOutputStream out;
        File f;

        if(fileName == null)
        {
            fileName = "";
        }
        fileName = Multifunction.FixSlashes(TrimQuotes(fileName));

        try
        {
            if(fileName.isEmpty())
            {
                String printableData = new String(data);    // Uses the platform default character set for conversion
                System.out.print(printableData);
                return 1;
            }
            else
            {
                // Check if the file already exists
                f = new File(fileName);
                if(f.exists() && !append)
                {
                    f.delete();
                }
                else if(append)
                {
                    f.createNewFile();  // Append will fail if the file doesn't exist yet. createNewFile() will create a new file when there is no such file, or just return false if the file already exists
                }

                // Write the changed traceData to the file
                out = new FileOutputStream(f, append);
                out.write(data);
                out.close();
            }
            return 1;
        }
        catch(IOException ex)
        {
            return -1;
        }
        catch(NullPointerException ex)
        {
            return -2;
        }
    }

    /**
     * Writes the specified string to a given file, but only if the file doesn't
     * already exactly contain the same contents (so as to preserve the file
     * last modified timestamp if nothing actually changes in the file).
     *
     * @param fileName        Filename to open and write the string to. If the
     *                        file doesn't already exist, it will be created.
     * @param newContents     UTF-8 formatting String contents to write to the
     *                        file.
     * @param convertLFtoCRLF If specified, causes all linefeeds (\n) in the
     *                        input string to be converted to a carriage return
     *                        + line feed (\r\n).
     *
     * @return 1 if output file didn't exist or exists but was different and
     *         therefore replaced with the newContents. The write succeeded.
     *
     * @return 0 if the output file exists and the data in it already exactly
     *         matches newContents. The file was not touched (so as not to
     *         change the modification date).
     *
     * -1 if the output file exists but couldn't be read, the path was invalid,
     * or the file couldn't be written to (ex: file or file system is attributed
     * read-only, out of space, no permission, etc).
     */
    public static int UpdateFileIfDataDifferent(String fileName, String newContents, boolean convertLFtoCRLF)
    {
        if(convertLFtoCRLF)
        {
            newContents = newContents.replaceAll("\n", Matcher.quoteReplacement("\r\n"));
        }
        return UpdateFileIfDataDifferent(fileName, newContents.getBytes());
    }

    public static boolean SaveObjToFile(Object o, String outputFilename)
    {
        // Write everything to a binary file for later use in another stage
        ObjectOutputStream objectOut;
        FileOutputStream fileOut;
        try
        {
            fileOut = new FileOutputStream(outputFilename, false);
            objectOut = new ObjectOutputStream(fileOut);
            objectOut.writeObject(o);
            objectOut.close();
            fileOut.close();
        }
        catch(Exception ex)
        {
            System.err.printf("Error: unable to save object to \"%1$s\""
                              + "\n       %2$s", outputFilename, ex.getMessage());
            return false;
        }
        return true;
    }

    public static Object ReadObjFromFile(String objFilename)
    {
        // Read everything from file
        ObjectInputStream objectIn;
        FileInputStream in;
        try
        {
            in = new FileInputStream(objFilename);
            objectIn = new ObjectInputStream(in);
            Object ret = objectIn.readObject();
            objectIn.close();
            in.close();
            return ret;
        }
        catch(InvalidClassException ex)
        {
            System.err.printf("Error: unable to read object from \"%1$s\""
                              + "\n       %2$s", objFilename, ex.getMessage());
        }
        catch(Exception ex)
        {
            System.err.printf("Error: unable to read object from \"%1$s\""
                              + "\n       %2$s", objFilename, ex.getMessage());
        }
        return null;
    }

    /**
     * Strips off pairs of leading and trailing quotation marks, if present.
     * Only double-quote characters are removed, and only when the quotation
     * marks are matched. I.e. the first and last character of the string are
     * both quotation marks. If multiple layers of matched quotes are present,
     * all of them are removed.
     *
     * Whitespace before and after quote removal are trimmed off from both ends
     * of the string.
     *
     * @param str String to remove quotes from
     *
     * @return Input string without any matched leading or trailing quotation
     *         marks present anymore.
     */
    public static String TrimQuotes(String str)
    {
        if(str == null)
        {
            return null;
        }

        str = str.trim();
        while(str.startsWith("\"") && str.endsWith("\""))
        {
            str = str.substring(1, str.length() - 1).trim();
        }
        return str;
    }

    /**
     * Replaces all slashes and backslashes in the given string with the OS
     * dependent folder path separator character (File.separator).
     *
     * @param path File or directory path to convert.
     *
     * @return path input string with all slashes/backslashes converted to
     *         slashes/backslashes to match File.separator.
     */
    public static String FixSlashes(String path)
    {
        if(path == null)
        {
            return null;
        }

        return path.replaceAll("[\\\\/]+", Matcher.quoteReplacement(File.separator));
    }


    /**
     * Returns a lit of Files[] matching the given filename within the specified
     * directory. Will optionally search inside sub folders recursively for the
     * same filename.
     *
     * On Windows platforms the search is case insensitive. On other platforms,
     * the searchFilename case is case sensitive.
     *
     * @return List of Files located within the directory (or recursed sub
     *         directories) matching the given name.
     *
     * If no matching files are found, an empty list is returned.
     *
     * If a file I/O or other exception prevents searching in the folder or sub
     * folder (such as due to security restrictions), searching within the given
     * directory or subdirectory branch is terminated and an empty list is
     * returned for that branch.
     */
    public static class SimpleFileFilter implements FileFilter
    {
        String filterString = "";
        public SimpleFileFilter()
        {

        }
        public SimpleFileFilter(String filterString)
        {
            this.filterString = filterString;
        }

        @Override
        public boolean accept(File pathname)
        {
            if(pathname.isDirectory())
                return true;
            return pathname.getName().equalsIgnoreCase(filterString);
        }
    }
    public static List<File> FindFiles(String directory, final String searchFilename, boolean recursive)
    {
        List<File> ret = new ArrayList<File>();
        File[] fileList = null;
        try
        {
            fileList = new File(directory).listFiles(new SimpleFileFilter(searchFilename));
        }
        catch(Exception ex)
        {   // Possible security exception, abort searching in this folder and return empty set
            return ret;
        }

        if(fileList != null)
        {
            for(File f : fileList)
            {
                if(f.isFile())
                {
                    ret.add(f);
                }
                else if(f.isDirectory() && recursive)
                {
                    List<File> foundSubFiles;
                    foundSubFiles = FindFiles(f.getPath(), searchFilename, recursive);
                    if(foundSubFiles != null)
                    {
                        for(File subFile : foundSubFiles)
                        {
                            ret.add(subFile);
                        }
                    }
                }
            }
        }
        return ret;
    }

    /**
     * Returns a lit of Files[] matching the given regular expression within the
     * specified directory. Will optionally search inside sub folders
     * recursively for the same regular expression.
     *
     * @return List of Files located within the directory (or recursed sub
     *         directories) matching the given regular expression.
     *
     * If no files are found matching the regular expression, an empty list is
     * returned.
     *
     * If a file I/O or other exception prevents searching in the folder or sub
     * folder (such as due to security restrictions), searching within the given
     * directory or subdirectory branch is terminated and an empty list is
     * returned for that branch.
     */
    public static List<File> FindFilesRegEx(String directory, String regexFile, boolean recursive)
    {
        Pattern p = Pattern.compile(regexFile);
        return FindFilesRegEx(directory, p, recursive);
    }

    public static List<File> FindFilesRegEx(String directory, Pattern fileSearchPattern, boolean recursive)
    {
        List<File> ret = new ArrayList<File>();
        File[] fileList = null;

        try
        {
            fileList = new File(directory).listFiles();
        }
        catch(Exception ex)
        {   // Possible security exception, abort searching in this folder and return empty set
            return ret;
        }

        if(fileList != null)
        {
            for(File f : fileList)
            {
                if(fileSearchPattern.matcher(f.getName()).matches() && f.isFile())
                {
                    ret.add(f);
                }
                else if(f.isDirectory() && recursive)
                {
                    List<File> foundSubFiles;
                    foundSubFiles = FindFilesRegEx(f.getPath(), fileSearchPattern, recursive);
                    if(foundSubFiles != null)
                    {
                        for(File subFile : foundSubFiles)
                        {
                            ret.add(subFile);
                        }
                    }
                }
            }
        }
        return ret;
    }

    private final static char[] hexChars = "0123456789ABCDEF".toCharArray();

    /**
     * Returns all bytes printed in hexadecimal as 2 characters with a space
     * separating each byte and new line characters added after a specified
     * number of bytes are converted.
     *
     * The last character does not have a space after it, but there will always
     * be one new line character appended to the end of the string, even if the
     * input data is 0 bytes
     *
     * ex: "00DEAF 03C1EE\n"
     *
     * @param dataBytes          Array of bytes to convert to hex ASCII
     *                           printable text.
     *
     * @param bytesPerGroup      Number of bytes to convert before printing a '
     *                           ' space character. The final group does not
     *                           receive a space trailing character, nor do the
     *                           last group on each line.
     *
     * @param groupsLittleEndian true if the bytes should be printed in little
     *                           endian format. Ex: for dataBytes[] = {0x00,
     *                           0x11, 0x22, 0x33, 0x44, 0x55}, bytesPerGroup =
     *                           3, and groupsLittleEndian = true, the output
     *                           would be: "221100 554433\n"
     *
     * @param groupsPerLine      Number of bytes to convert before inserting
     *                           '\n' new line characters. Multiple new lines
     *                           are added if the dataBytes extends past
     *                           bytesPerLine multiple times.
     *
     * Specify 0 or less if no new line characters should be generated.
     *
     * @return ASCII hex representation of the byte array stored in a String
     *         with '\n' new line characters inserted after every bytesPerLine
     *         and at the end of the string, unless groupsPerLine <= 0.
     *
     * If a full number of bytesPerLine were not converted on the same line, a
     * new line is still added unless groupsPerLine <= 0.
     *
     * If dataBytes is null or zero length, the string "\n" is returned for
     * groupsPerLine > 0 and "" is returned for groupsPerLine < 0.
     */
    public static String bytesToHex(byte[] dataBytes, int bytesPerGroup, boolean groupsLittleEndian, int groupsPerLine)
    {
        int outIndex = 0;
        StringBuilder sb;

        if((dataBytes == null) || (dataBytes.length == 0))
        {
            return groupsPerLine >= 0 ? "\n" : "";
        }

        sb = new StringBuilder(dataBytes.length * 3);

        for(int inIndex = 0; inIndex < dataBytes.length;)
        {
            if(groupsLittleEndian)
            {
                inIndex += bytesPerGroup - 1;
                for(int i = 0; i < bytesPerGroup; i++, inIndex--)
                {
                    if(inIndex >= dataBytes.length)
                    {
                        sb.insert(outIndex++, ' ');
                        sb.insert(outIndex++, ' ');
                    }
                    else
                    {
                        sb.insert(outIndex++, hexChars[((int)(dataBytes[inIndex] >> 4)) & 0x0F]);
                        sb.insert(outIndex++, hexChars[((int)(dataBytes[inIndex])) & 0x0F]);
                    }
                }
                inIndex += bytesPerGroup + 1;
            }
            else
            {
                for(int i = 0; (i < bytesPerGroup) && (inIndex < dataBytes.length); i++, inIndex++)
                {
                    sb.insert(outIndex++, hexChars[((int)(dataBytes[inIndex] >> 4)) & 0x0F]);
                    sb.insert(outIndex++, hexChars[((int)(dataBytes[inIndex])) & 0x0F]);
                }
            }
            if((groupsPerLine > 0) && ((outIndex + 1) % (groupsPerLine * (bytesPerGroup * 2 + 1)) == 0))
            {
                sb.insert(outIndex++, '\n');
            }
            else if(inIndex < dataBytes.length)
            {
                sb.insert(outIndex++, ' ');
            }
        }
        if((groupsPerLine > 0) && (sb.charAt(outIndex - 1) != '\n'))
        {
            sb.insert(outIndex, '\n');
        }

        return sb.toString();
    }

    /**
     * Returns all bytes printed in hexadecimal as 2 characters with a space
     * separating each byte and new line characters added after a specified
     * number of bytes are converted.
     *
     * The last character does not have a space after it, but there will always
     * be one new line character appended to the end of the string, even if the
     * input data is 0 bytes
     *
     * ex: "00 DE AF 03 C1 EE\n"
     *
     * @param dataBytes    Array of bytes to convert to hex ASCII printable
     *                     text.
     *
     * @param bytesPerLine Number of bytes to convert before inserting '\n' new
     *                     line characters. Multiple new lines are added if the
     *                     dataBytes extends past bytesPerLine multiple times.
     *
     * @return ASCII hex representation of the byte array stored in a String
     *         with '\n' new line characters inserted after every bytesPerLine
     *         and at the end of the string (even if a full number of
     *         bytesPerLine were not converted on the same line).
     *
     * If dataBytes is null or zero length, the string "\n" is returned.
     */
    public static String bytesToHex(byte[] dataBytes, int bytesPerLine)
    {
        return bytesToHex(dataBytes, 1, false, bytesPerLine);
    }

    /**
     * Returns all bytes printed in hexadecimal as 2 characters with a space
     * separating each byte. The last character does not have a space after it.
     *
     * ex: "00 DE AF 03 C1 EE"
     *
     * @param dataBytes Array of bytes to convert
     *
     * @return ASCII hex representation of the byte array stored in a String. If
     *         dataBytes is null or zero length, a zero length string is
     *         returned ("").
     */
    public static String bytesToHex(byte[] dataBytes)
    {
        return bytesToHex(dataBytes, 1, false, -1);
    }

    static String FormatHelpText(int lineWidth, int indent, String helpText)
    {
        return FormatHelpText(helpText, lineWidth, indent, 0, true);
    }

    static String FormatHelpText(String helpText, int lineWidth, int indent, int hangingIndent, boolean trimConsecutiveWhitespace)
    {
        String textOut = "";
        int readIndex = 0, endReadIndex;
        boolean firstLine = true;

        // Replace consecutive spaces, newlines, tabs or other whitespace with a single space when requested
        if(trimConsecutiveWhitespace)
        {
            helpText = helpText.replaceAll("\\s+", " ").trim();
        }

        readIndex = 0;
        while(readIndex < helpText.length())
        {
            for(int i = 0; i < indent; i++)
            {
                textOut += " ";
            }
            if(!firstLine)
            {
                for(int i = 0; i < hangingIndent; i++)
                {
                    textOut += " ";
                }
            }

            // Choose the last character in the line as the initial line-break
            // position
            endReadIndex = readIndex + lineWidth - indent;
            if(helpText.length() < endReadIndex)
            {
                endReadIndex = helpText.length();
            }

            // Check if there are any explicit new line characters preceeding the chosen line-break
            int newLinePosition = helpText.substring(readIndex, endReadIndex).indexOf('\n');
            if(newLinePosition >= 0)
            {
                endReadIndex = readIndex + newLinePosition + 1;
            }
            else
            {
                // Find the first space character we can do a line break on
                while(readIndex < endReadIndex)
                {
                    if(endReadIndex == helpText.length())
                    {
                        break;
                    }
                    if(helpText.charAt(endReadIndex) == ' ')
                    {
                        break;
                    }

                    endReadIndex--;
                }
            }

            // If the whole line cannot be broken since there are no spaces,
            // then just use the complete line and break it anyway. We must
            // recheck the maximum in case if this is the last line.
            if(endReadIndex == readIndex)
            {
                endReadIndex = readIndex + lineWidth - indent;
                if(endReadIndex > helpText.length())
                {
                    endReadIndex = helpText.length();
                }
            }

            textOut += helpText.substring(readIndex, endReadIndex);
            if(newLinePosition < 0)
            {
                textOut += "\r\n";
            }
            readIndex = endReadIndex;

            // If we didn't come upon an explicit new line character, trim any
            // trailing spaces after this newly generated line so they
            // don't cause unexpected indentation for the next line.
            if(newLinePosition < 0)
            {
                while(readIndex < helpText.length())
                {
                    if(helpText.charAt(readIndex) != ' ')
                    {
                        break;
                    }
                    readIndex++;
                    if(readIndex == helpText.length())
                    {
                        return textOut;
                    }
                }
            }

        }

        return textOut;
    }

    /**
     * Fast static method for converting a List<String> type to a single
     * concatenated output string. Output is generated much faster than iterated
     * String concatenation for large lists.
     */
    public static String CatStringList(List<String> stringList)
    {
        return CatStringList(stringList, null);
    }

    /**
     * Fast static method for converting a List<String> type to a single
     * concatenated output string. Output is generated much faster than iterated
     * String concatenation for large lists.
     */
    public static String CatStringList(Collection<Integer> decimalList, String delimiter)
    {
        List<String> strList = new ArrayList<String>();
        for(Integer i : decimalList)
        {
            strList.add(i.toString());
        }
        return CatStringList(strList, delimiter);
    }

    /**
     * Fast static method for converting a List<String> type to a single
     * concatenated output string. Output is generated much faster than iterated
     * String concatenation for large lists.
     */
    public static String CatStringList(Collection<String> stringList)
    {
        //List<String> strList = new ArrayList<String>(stringList);
        return CatStringList(new ArrayList<String>(stringList), null);
    }

    /**
     * Fast static method for converting a List<String> type to a single
     * concatenated output string with a specified string added between each
     * list item. Output is generated much faster than iterated String
     * concatenation for large lists.
     *
     * The delimiter is not added before the first element or after the last
     * element.
     */
    public static String CatStringList(List<String> stringList, String delimiter)
    {
        int outSize = 0;

        if(stringList == null)
        {
            return null;
        }

        for(String s : stringList)
        {
            outSize += s.length();
        }

        // Handle concatination with delimiters
        if((delimiter != null) && stringList.size() > 1)
        {
            outSize += (stringList.size() - 1) * delimiter.length();
            StringBuilder builder = new StringBuilder(outSize);
            int i = 0;
            for(int j = 0; j < stringList.size() - 1; j++)  // All but last element requires a delimiter afterwards
            {
                String s = stringList.get(j);
                builder.insert(i, s);
                i += s.length();
                builder.insert(i, delimiter);
                i += delimiter.length();
            }
            builder.insert(i, stringList.get(stringList.size() - 1));   // Last element
            return builder.toString();
        }
        else    // Handle concatination faster without delimiters considered
        {
            StringBuilder builder = new StringBuilder(outSize);
            int i = 0;
            for(String s : stringList)
            {
                builder.insert(i, s);
                i += s.length();
            }
            return builder.toString();
        }
    }


    static class StringList implements Serializable
    {
        String topString;
        List<String> list = new ArrayList<String>();

        public void sortPartNumbers()
        {
            Collections.sort(list, new PICNameComparator());
        }

        public void sortSFRNames()
        {
            Collections.sort(list, new SFRNameComparator());
        }

        public void sort()
        {
            Collections.sort(list);
        }

        public void reverse()
        {
            Collections.reverse(list);
        }

        public String CatToString()
        {
            return Multifunction.CatStringList(list);
        }

        public String CatToString(String delimiter)
        {
            return Multifunction.CatStringList(list, delimiter);
        }

        public void Substitute1IfAllPresent(List<String> findList, String singleReplacement)
        {
            Multifunction.Substitute1IfAllPresent(this.list, findList, singleReplacement);
        }

    }

    public static void Substitute1IfAllPresent(List<String> findInList, List<String> findList, String singleReplacement)
    {
        if((findList == null) || findList.isEmpty() || (findInList == null) || findInList.isEmpty())
        {
            return;
        }
        if(findInList.containsAll(findList))
        {
            findInList.removeAll(findList);
            findInList.add(singleReplacement);
        }
    }
}


class DSPIC33EPPLL
{
    int inputHz;
    int outputHz;
    int PLLPRE;
    int PLLPOST;
    int PLLFBD;
    double errorPercent;
    int errorHz;

    public DSPIC33EPPLL()
    {
        this.inputHz = 7370000;
        this.PLLFBD = 0;
        this.PLLPOST = 0;
        this.PLLPRE = 0;
        this.outputHz = this.inputHz / (this.PLLPRE + 2) * (this.PLLFBD + 2) / (this.PLLPOST + 2);
        this.errorHz = 7370000 - this.errorHz;
        this.errorPercent = this.errorHz * 100.0 / 7370000;
    }

    /**
     * Iteratively sweeps over all PLL options to find the closest match to the
     * desired frequency. Before return, the closest match found is programmed
     * into the PLL configuration register fields.
     *
     * @param refInputClock   PLL module input frequency, in Hz.
     * @param targetOutputFCY Target output frequency in Instructions Per Second
     *                        (Hz).
     *
     * @return Optimally matching frequency we came up with.
     */
    void FindOptimalPLLSettings(String deviceName, int refInputClock, int targetOutputFCY, int minOutputFCY, int maxOutputFCY)
    {
        int bestErrorAbs;
        int PLLPRE, PLLFBD, PLLPOST;
        int FOSC;
        int FCYOut;
        int FVCOIN;
        int FVCOOut;
        int currentError;
        int lastErrorAbs;
        int currentErrorAbs;
        int configValid = 0;

        this.inputHz = refInputClock;
        this.PLLFBD = 0;
        this.PLLPOST = 0;
        this.PLLPRE = 0;
        this.outputHz = refInputClock / (this.PLLPRE + 2) * (this.PLLFBD + 2) / (this.PLLPOST + 2);
        this.errorHz = targetOutputFCY - this.errorHz;
        this.errorPercent = this.errorHz * 100.0 / targetOutputFCY;

        bestErrorAbs = 0x7FFFFFFF;
        for(PLLPRE = 0; PLLPRE < (1 << 5); PLLPRE++) // Goes down in frequency with each step
        {
            FVCOIN = refInputClock / (PLLPRE + 2);
            if(FVCOIN < 800000)
            {
                break;
            }
            if(FVCOIN > 8000000)
            {
                continue;
            }
            for(PLLFBD = 120000000 / FVCOIN - 2; PLLFBD < (1 << 9); PLLFBD++) // Goes up in frequency with each step
            {
                FVCOOut = FVCOIN * (PLLFBD + 2);
                if((FVCOOut < 120000000))
                {
                    continue;
                }
                if(FVCOOut > 340000000)
                {
                    break;
                }

                lastErrorAbs = 0x7FFFFFFF;
                for(PLLPOST = 0; PLLPOST < (1 << 3); PLLPOST++)  // Goes down in frequency
                {
                    FOSC = FVCOOut / (2 * (PLLPOST + 1));
                    FCYOut = FOSC / 2;
                    if(FCYOut < minOutputFCY)
                    {
                        break;
                    }
                    if(FCYOut > maxOutputFCY)
                    {
                        continue;
                    }
                    currentError = FCYOut - targetOutputFCY;
                    currentErrorAbs = currentError;
                    if(currentError < 0)
                    {
                        currentErrorAbs = -currentError;
                    }
                    if(lastErrorAbs < currentErrorAbs)  // Abort early if we are diverging
                    {
                        break;
                    }
                    lastErrorAbs = currentErrorAbs;

                    if(currentErrorAbs < bestErrorAbs)
                    {
                        bestErrorAbs = currentErrorAbs;
                        this.errorHz = currentError;
                        this.PLLPRE = PLLPRE;
                        this.PLLPOST = PLLPOST;
                        this.PLLFBD = PLLFBD;
                        this.outputHz = FCYOut;
                        this.errorPercent = this.errorHz * 100.0 / targetOutputFCY;
                        configValid = 1;
                    }
                    if(currentError == 0)
                    {
                        return;
                        //return configValid != 0 ? this : null;
                    }
                }
            }
        }
        //return configValid != 0 ? this : null;
    }
}
