/**
 * Copyright (C) 2017 Microchip Technology Inc.
 *
 * MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
 * derivatives created by any person or entity by or on your behalf, exclusively
 * with Microchip's products.  Microchip and its licensors retain all ownership
 * and intellectual property rights in the accompanying software and in all
 * derivatives here to.
 *
 * This software and any accompanying information is for suggestion only.  It
 * does not modify Microchip's standard warranty for its products.  You agree
 * that you are solely responsible for testing the software and determining its
 * suitability.  Microchip has no obligation to modify, test, certify, or
 * support the software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
 * PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS,
 * COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
 *
 * IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
 * (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
 * INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 * EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
 * ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
 * MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
 * TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
 * CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
 * FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
 * TERMS.
 */
package com.microchip.apps.ezbl;

import static com.microchip.apps.ezbl.Multifunction.CatStringList;
import static com.microchip.apps.ezbl.Multifunction.FindFilesRegEx;
import static com.microchip.apps.ezbl.Multifunction.StringList;
import com.microchip.crownking.edc.PICFactory;
import com.microchip.crownking.edc.PackageLoggingImpl;
import java.io.*;
import java.text.Collator;
import java.util.*;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;


/**
 * EZBL code to facilitate reading MPLAB X Extended Device Characteristics (edc)
 * data base to extract target PIC/dsPIC device specific information.
 *
 * Note: requires crownking.edc.jar and crownking.jar (MPLAB X 4.00-) or
 * crownking.common.jar (MPLAB X 4.01+)
 *
 * @author C12128
 */
public class EDCReader
{
    @SuppressWarnings("unchecked")
    public static void GroupPICPartNumbers(List<String> partNumberList, DeviceProductLines productClasses, TreeMap<Integer, StringList> productFamilies)
    {
        for(java.lang.reflect.Field f : productClasses.getClass().getDeclaredFields())
        {
            List<String> classList = null;
            try
            {
                classList = (List<String>)(f.get(productClasses));
            }
            catch(IllegalArgumentException | IllegalAccessException ex)
            {
                Logger.getLogger(EDCReader.class.getName()).log(Level.SEVERE, null, ex);
            }

            if(classList != null)
            {
                Multifunction.Substitute1IfAllPresent(partNumberList, classList, f.getName() + "*");
            }
        }
        for(StringList partFamily : productFamilies.values())
        {
            Multifunction.Substitute1IfAllPresent(partNumberList, partFamily.list, partFamily.topString);
        }

        Collections.sort(partNumberList, new PICNameComparator());
    }


    static class BitField implements Serializable
    {

        String name;
        String desc;
        EZBL_SFR parentSFR;
        List<String> aliases = new ArrayList<String>();
        int position;
        int length;
        int implementedBits;
        int readOnlyBits;
        int writeOnlyBits;
        int reset0Bits;
        int reset1Bits;
        int unknownBits;
    }


    public static class EZBL_SFR implements Comparable<EZBL_SFR>, Serializable
    {

        String name;
        List<String> aliases = new ArrayList<String>();
        String desc;
        Long addr;
        Long endAddr;
        String srcModule;
        int moduleAddrWidth;
        int width;
        int bitsDefined;
        boolean isHidden;
        boolean isDCR;
        List<BitField> bitfields = new ArrayList<BitField>();
        Map<String, BitField> bitfieldByName = new HashMap<String, BitField>();    // BitField name, BitField
        Node xNode;

        @Override
        public int compareTo(EZBL_SFR s)
        {
            return Objects.equals(this.addr, s.addr) ? 0 : (this.addr > s.addr ? 1 : -1);
        }
    }


    static class DeviceProductLines implements Serializable
    {

        List<String> dsPIC30F = new ArrayList<String>();
        List<String> dsPIC33F = new ArrayList<String>();
        List<String> dsPIC33E = new ArrayList<String>();
        List<String> dsPIC33C = new ArrayList<String>();
        List<String> PIC24FK = new ArrayList<String>();
        List<String> PIC24FJ = new ArrayList<String>();
        List<String> PIC24H = new ArrayList<String>();
        List<String> PIC24E = new ArrayList<String>();
        List<String> generic_16bit = new ArrayList<String>();       // PIC24/dsPIC devices with PSVPAG
        List<String> generic_16bit_da = new ArrayList<String>();    // PIC24 devices with DSRPAG/DSWPAG
        List<String> generic_16bit_ep = new ArrayList<String>();    // PIC24E/dsPIC33E/dsPIC33D devices
        List<String> generic_16dsp_ch = new ArrayList<String>();    // dsPIC33C devices
        List<String> generic_unknown = new ArrayList<String>();     // Shouldn't have any of these, needs implementing if we do
    }


    static class FieldsOnGenericDevice implements Serializable
    {

        List<BitFieldParams> generic_16bit = new ArrayList<BitFieldParams>();       // PIC24/dsPIC devices with PSVPAG
        List<BitFieldParams> generic_16bit_da = new ArrayList<BitFieldParams>();    // PIC24 devices with DSRPAG/DSWPAG
        List<BitFieldParams> generic_16bit_ep = new ArrayList<BitFieldParams>();    // PIC24E/dsPIC33E/dsPIC33D devices
        List<BitFieldParams> generic_16dsp_ch = new ArrayList<BitFieldParams>();    // dsPIC33C devices

        public void sortByFieldName()
        {
            Collections.sort(generic_16bit, new BitFieldParamsNameComparator());
            Collections.sort(generic_16bit_da, new BitFieldParamsNameComparator());
            Collections.sort(generic_16bit_ep, new BitFieldParamsNameComparator());
            Collections.sort(generic_16dsp_ch, new BitFieldParamsNameComparator());
        }

        public void sortByAddress()
        {
            Collections.sort(generic_16bit, new BitFieldParamsAddrComparator());
            Collections.sort(generic_16bit_da, new BitFieldParamsAddrComparator());
            Collections.sort(generic_16bit_ep, new BitFieldParamsAddrComparator());
            Collections.sort(generic_16dsp_ch, new BitFieldParamsAddrComparator());
        }

    }


    static class SFRList implements Serializable
    {

        List<EZBL_SFR> list = new ArrayList<EZBL_SFR>();
    }


    static class BitFieldList implements Serializable
    {

        List<BitField> list = new ArrayList<BitField>();
    }


    static class DevRegisters implements Serializable
    {
        TreeMap<Integer, EZBL_SFR> RegByAddress = new TreeMap<Integer, EZBL_SFR>();
        TreeMap<String, EZBL_SFR> RegByName = new TreeMap<String, EZBL_SFR>();                // SFR name, SFR class
        TreeMap<String, SFRList> RegsWithBitfield = new TreeMap<String, SFRList>(); // Bitfield name, List of SFRs containing the bitfield
        List<BitField> AllBitFieldsInAllRegs = new ArrayList<BitField>();

        public DevRegisters addAll(DevRegisters moreRegs)
        {
            this.AllBitFieldsInAllRegs.addAll(moreRegs.AllBitFieldsInAllRegs);
            this.RegByAddress.putAll(moreRegs.RegByAddress);
            this.RegByName.putAll(moreRegs.RegByName);
            this.RegsWithBitfield.putAll(moreRegs.RegsWithBitfield);

            return this;
        }
    }


    static class AddrNamePair implements Comparable<AddrNamePair>
    {
        long addr;
        String name;

        @Override
        public int compareTo(AddrNamePair o)
        {
            if(addr == o.addr)
            {
                return name.compareTo(o.name);
            }

            if(addr < o.addr)
            {
                return -1;
            }
            return 1;
        }

    }


    public static class BitFieldParamsNameComparator implements Comparator<BitFieldParams>
    {
        @Override
        public int compare(BitFieldParams x, BitFieldParams y)
        {
            SFRNameComparator c = new SFRNameComparator();
            int ret = c.compare(x.name, y.name) * 2;
            if(x.addr < y.addr)
            {
                ret /= 2;
            }

            return ret;
        }
    }


    public static class BitFieldParamsAddrComparator implements Comparator<BitFieldParams>
    {
        @Override
        public int compare(BitFieldParams x, BitFieldParams y)
        {
            SFRNameComparator c = new SFRNameComparator();
            if(x.addr < y.addr)
            {
                return -1;
            }
            if(x.addr == y.addr)
            {
                return 0;
            }
            return 1;
        }
    }


    static class BitFieldParams implements Comparable<BitFieldParams>, Serializable
    {
        boolean isSFR = false;
        boolean isDCR = false;      // Will also be tagged as isSFR = true for Config words
        boolean isBitfield = false;
        boolean isDevice = false;
        boolean isGenericDevice = false;
        boolean isEverything = false;
        String name;                // Name of the Bitfield, SFR, DCR, device, or generic device
        String parentName;          // The parent SFR name when isBitfield == true, or the parent module name when isSFR == true
        long addr;
        int position;
        int length;
        int mask;
        List<String> devsWithMatchingAddr = new ArrayList<String>();
        List<String> devsWithMatchingParentAndPosition = new ArrayList<String>();
        List<String> devsWithMatchingAddrAndParentAndPosition = new ArrayList<String>();
        List<String> devsWithMatchingPosition = new ArrayList<String>();
        List<String> containedByGenericDevs = new ArrayList<String>();

        public void GroupMatchingDevNames(DeviceProductLines productClasses, TreeMap<Integer, StringList> productFamilies)
        {
            GroupPICPartNumbers(devsWithMatchingAddr, productClasses, productFamilies);
            GroupPICPartNumbers(devsWithMatchingParentAndPosition, productClasses, productFamilies);
            GroupPICPartNumbers(devsWithMatchingAddrAndParentAndPosition, productClasses, productFamilies);
            GroupPICPartNumbers(devsWithMatchingPosition, productClasses, productFamilies);
            GroupPICPartNumbers(containedByGenericDevs, productClasses, productFamilies);
        }

        @Override
        public int compareTo(BitFieldParams s)
        {
            if(this.equals(s))
            {
                return 0;
            }
            if(this.isSFR && s.isSFR)
            {
                if(addr < s.addr)
                {
                    return -1;
                }
                if(addr > s.addr)
                {
                    return 1;
                }
                return name.compareTo(s.name);
            }
            else if(this.isBitfield && s.isBitfield)
            {
                if(name.equals(s.name) && parentName.equals(s.parentName) && (position == s.position) && (length == s.length))
                {
                    return 0;
                }
                return 1;
            }
            else if(this.isBitfield)
            {
                return 1;
            }
            else if(this.isSFR)
            {
                return -1;
            }

            if(addr < s.addr)
            {
                return -1;
            }
            if(addr > s.addr)
            {
                return 1;
            }
            if(position < s.position)
            {
                return -1;
            }
            if(position > s.position)
            {
                return 1;
            }
            return 1;
        }

    }

    static public void GenLEDPatternBoing(String outputFilename)
    {
        byte[] data = new byte[4096];
        int statesPerLED = 4;
        double velocity = 0;                // Current speed and heading
        double position = 8 * statesPerLED; // LED 8, with 2 bits of brightness
        double acceleration = -9.81 * 4;    // LEDs/second^2
        double fps = 250;                   // Framerate (62.5 Hz effective with pseudo dimming)
        double deltaTime = fps / data.length;
        double reboundEfficiency = 0.98;

        for(int i = 0; i < data.length; i += 4)
        {
            velocity += acceleration * deltaTime;
            position += velocity * deltaTime;
            if(position <= 0)
            {
                position = 0;
                velocity = -velocity * reboundEfficiency;
            }
            int mainPos = ((int)(position / statesPerLED));
            data[i] = (byte)(1 << ((int)(position / statesPerLED)));
            data[i + 2] = ((int)((position - 1) / statesPerLED)) == mainPos ? data[i] : 0;
            data[i + 1] = ((int)((position - 2) / statesPerLED)) == mainPos ? data[i] : 0;
            data[i + 3] = ((int)((position - 3) / statesPerLED)) == mainPos ? data[i] : 0;
        }

        Multifunction.UpdateFileIfDataDifferent(outputFilename, data);
    }

    static public Node FindAncestorByNodeType(Node startingNode, String ancestorNodeType) // ancestorNodeName is a string like "edc:SFRDef", not a cname attribute.
    {
        Node ancestor;

        ancestor = startingNode.getParentNode();
        while(true)
        {
            if(ancestor.getNodeName().equals(ancestorNodeType))
            {
                return ancestor;
            }

            ancestor = ancestor.getParentNode();
            if(ancestor == null)
            {
                return null;
            }
        }
    }

    public EDCReader()
    {
    }

    /**
     * Recursively searches a NodeList for nodes and node children for a
     * specific name, returning all found nodes in a List<Node> structure.
     *
     * @param startingNodeList NodeList to enumerate all children and match
     *                         against.
     *
     * @param nodeNames        Case sensitive node name(s) to search for. When
     *                         searching for multiple node names, specify each
     *                         name separated by the OR/pipe character '|' (do
     *                         not add whitespace before or after)
     *
     * @return List<Node> containing all Node elements matching the specified
     *         name.
     *
     * If startingNodeList is null, null is returned.
     *
     * If no matches are found, the returned List<Node> has 0 elements in it.
     */
    static public List<Node> FindChildrenByNodeNames(NodeList startingNodeList, String nodeNames) // nodeName is a string like "edc:SFRFieldDef" or "edc:SFRFieldDef|edc:AdjustPoint", not a cname attribute.
    {
        if(startingNodeList == null)
            return null;

        List<Node> matchingChildren = new ArrayList<Node>();
        for(int i = 0; i < startingNodeList.getLength(); i++)
        {
            Node item = startingNodeList.item(i);
            matchingChildren.addAll(FindChildrenByNodeNames(item, nodeNames));
        }
        return matchingChildren;
    }

    /**
     * Recursively searches a Node for children nodes of a specific name or
     * names, returning all found nodes in a List<Node> structure.
     *
     * @param startingNode Node to start enumerating all children and match
     *                     against.
     *
     * @param nodeNames    Case sensitive node name(s) to search for. When
     *                     searching for multiple node names, specify each name
     *                     separated by the OR/pipe character '|' (do not add
     *                     whitespace before or after)
     *
     * @return List<Node> containing all Node elements matching the specified
     *         name.
     *
     * If startingNode is null, null is returned.
     *
     * If no matches are found, the returned List<Node> has 0 elements in it.
     */
    static public List<Node> FindChildrenByNodeNames(Node startingNode, String nodeNames) // childNodeType is a string like "edc:SFRFieldDef" or "edc:SFRFieldDef|edc:AdjustPoint", not a cname attribute.
    {
        if(startingNode == null)
            return null;

        NodeList children;
        Node testNode;
        List<Node> matchingChildren = new ArrayList<Node>();
        String searchTypes[] = nodeNames.split("[\\|]");

        if(!startingNode.hasChildNodes())
        {
            return matchingChildren;
        }

        children = startingNode.getChildNodes();
        int numSearchStrings = searchTypes.length;
        int numChildren = children.getLength();
        for(int i = 0; i < numChildren; i++)
        {
            testNode = children.item(i);
            for(int j = 0; j < numSearchStrings; j++)
            {
                if(testNode.getNodeName().equals(searchTypes[j]))
                {
                    matchingChildren.add(testNode);
                }
            }
            matchingChildren.addAll(FindChildrenByNodeNames(testNode, nodeNames));
        }

        return matchingChildren;
    }

    /**
     * Returns RAM SFRs or DCRs (Device Configuration Registers, or "fuses")
     * based on input NodeList input ("edc:SFRDef" and "edc:DCRDef")
     *
     * @param SFRDefNodes A NodeList containing any or all XML nodes to index
     *                    and return in mapped form. This input can be
     *
     * @return DevRegisters class containing all SFRs and Config registers in
     *         easily accessed map objects.
     *
     * If SFRDefNodes is null, null is returned.
     */
    static public DevRegisters IndexSFRDCRNodes(NodeList SFRDefNodes)
    {
        boolean searchingForRegularSFRs;
        Node item;
        Node attribs;
        String nodeName;
        Node temp;
        Node temp2;

        if(SFRDefNodes == null)
            return null;

        DevRegisters regs = new DevRegisters();

        List<Node> allSFRDCRNodes = FindChildrenByNodeNames(SFRDefNodes, "edc:SFRDef|edc:DCRDef");

        for(int i = 0; i < allSFRDCRNodes.size(); i++)
        {
            item = allSFRDCRNodes.get(i);
            searchingForRegularSFRs = item.getNodeName().equals("edc:SFRDef");

            if(!item.hasAttributes())
            {
                continue;
            }
            attribs = item.getAttributes().getNamedItem("edc:cname");
            if(attribs == null)
            {
                continue;
            }
            nodeName = attribs.getNodeValue();
            if(nodeName == null)
            {
                continue;
            }

            EZBL_SFR sfr = new EZBL_SFR();
            sfr.name = nodeName;
            sfr.xNode = item;
            sfr.addr = -1L;
            sfr.isDCR = !searchingForRegularSFRs;
            temp = item.getAttributes().getNamedItem("edc:_addr");
            if(temp != null)
            {
                sfr.addr = Long.decode(temp.getNodeValue());
            }
            sfr.isHidden = false;
            temp = item.getAttributes().getNamedItem("edc:ishidden");
            if(temp != null)
            {
                sfr.isHidden = Boolean.parseBoolean(temp.getNodeValue());
            }
            sfr.desc = "";
            temp = item.getAttributes().getNamedItem("edc:desc");
            if(temp != null)
            {
                sfr.desc = temp.getNodeValue();
            }
            sfr.srcModule = "";
            temp = item.getAttributes().getNamedItem("edc:_modsrc");
            if(temp != null)
            {
                sfr.srcModule = temp.getNodeValue();
            }
            sfr.width = 16;
            temp = item.getAttributes().getNamedItem("edc:nzwidth");
            if(temp != null)
            {
                sfr.width = Integer.decode(temp.getNodeValue());
            }
            sfr.endAddr = sfr.addr + (sfr.width / 8);

            sfr.moduleAddrWidth = sfr.width / 8;
            temp = item.getAttributes().getNamedItem("edc:_begin");
            temp2 = item.getAttributes().getNamedItem("edc:_end");
            if((temp != null) && (temp2 != null))
            {
                sfr.moduleAddrWidth = Integer.decode(temp2.getNodeValue()) - Integer.decode(temp.getNodeValue());
            }

            List<Node> aliases = FindChildrenByNodeNames(item, "edc:AliasList");
            for(int j = 0; j < aliases.size(); j++)
            {
                if(!aliases.get(j).hasAttributes())
                {
                    continue;
                }
                sfr.aliases.add(aliases.get(j).getAttributes().getNamedItem("edc:cname").getNodeValue());
            }

            List<Node> bitfields;
            if(searchingForRegularSFRs)
            {
                bitfields = FindChildrenByNodeNames(item, "edc:SFRMode|edc:SFRFieldDef|edc:AdjustPoint");
            }
            else
            {
                bitfields = FindChildrenByNodeNames(item, "edc:DCRMode|edc:DCRFieldDef|edc:AdjustPoint");
            }
            int bitPos = -1;
            for(int j = 0; j < bitfields.size(); j++)
            {
                Node bfNode = bitfields.get(j);
                if(!bfNode.hasAttributes())
                {
                    continue;
                }
                if(bfNode.getNodeName().equals("edc:SFRMode") || bfNode.getNodeName().equals("edc:DCRMode"))
                {
                    bitPos = 0;
                    continue;
                }
                if(bfNode.getNodeName().equals("edc:AdjustPoint"))
                {
                    bitPos += Integer.decode(GetAttr(bfNode, "edc:offset", "1"));
                    continue;
                }
                String localCName = GetAttr(bfNode, "edc:cname");
                if(localCName != null)
                {
                    BitField bf = new BitField();
                    bf.name = localCName;
                    bf.parentSFR = sfr;
                    bf.position = bitPos;
                    bf.length = Integer.decode(GetAttr(bfNode, "edc:nzwidth", "16"));
                    bf.desc = GetAttr(bfNode, "edc:desc", "");
                    sfr.bitfieldByName.put(bf.name, bf);
                    regs.AllBitFieldsInAllRegs.add(bf);
                    SFRList sfrsWithThisBitfield = regs.RegsWithBitfield.get(bf.name);
                    if(sfrsWithThisBitfield == null)
                        sfrsWithThisBitfield = new SFRList();
                    sfrsWithThisBitfield.list.add(sfr);
                    regs.RegsWithBitfield.put(bf.name, sfrsWithThisBitfield);
                    bitPos += bf.length;
                }
            }
            regs.RegByName.put(sfr.name, sfr);
            regs.RegByAddress.put(sfr.addr.intValue(), sfr);
        }

        return regs;
    }

    static public LinkedHashMap<String, DecodedNodeList> IndexNodesByTag(NodeList XMLNodes, boolean recursive) // Node tag name, DocodedNode of the original Node
    {
        LinkedHashMap<String, DecodedNodeList> nodeListMap = new LinkedHashMap<String, DecodedNodeList>();
        Node item;
        String tagName;

        for(int i = 0; i < XMLNodes.getLength(); i++)
        {
            item = XMLNodes.item(i);
            tagName = item.getNodeName();
            if(!nodeListMap.containsKey(tagName))
                nodeListMap.put(tagName, new DecodedNodeList());
            nodeListMap.get(tagName).list.add(new DecodedNode(item));
            if(recursive)
            {
                if(item.hasChildNodes())
                {
                    nodeListMap.putAll(IndexNodesByTag(item.getChildNodes(), recursive));
                }
            }
        }

        return nodeListMap;
    }

    static public Map<String, EZBL_SFR> IndexSFRNodes(NodeList SFRDefNodes)
    {
        return IndexSFRDCRNodes(SFRDefNodes).RegByName;
    }

    static public Map<String, Node> IndexNodesByCName(NodeList XMLNodes)
    {
        HashMap<String, Node> nodeMap = new HashMap<String, Node>();
        Node item;
        String nodeName;

        for(int i = 0; i < XMLNodes.getLength(); i++)
        {
            item = XMLNodes.item(i);
            nodeName = GetAttr(item, "edc:cname");
            if(nodeName != null)
            {
                nodeMap.put(nodeName, item);
            }
        }

        return nodeMap;
    }

    static public Node GetAttrNode(Node XMLNode, String attrName)
    {
        return GetAttrNode(XMLNode, attrName, null);
    }

    static public Node GetAttrNode(Node XMLNode, String attrName, Node defaultValue)
    {
        if(XMLNode == null)
            return defaultValue;

        if(attrName == null)
            attrName = "edc:cname";

        NamedNodeMap nm = XMLNode.getAttributes();
        if(nm == null)
            return defaultValue;

        Node attrNode = nm.getNamedItem(attrName);
        if(attrNode == null)
            return defaultValue;

        return attrNode;
    }

    static public String GetAttr(Node XMLNode, String attrName)
    {
        return GetAttr(XMLNode, attrName, null);
    }

    static public String GetAttr(Node XMLNode, String attrName, String defaultValue)
    {
        if(XMLNode == null)
            return defaultValue;

        if(attrName == null)
            attrName = "edc:cname";

        NamedNodeMap nm = XMLNode.getAttributes();
        if(nm == null)
            return defaultValue;

        Node item = nm.getNamedItem(attrName);
        if(item == null)
            return defaultValue;

        String nodeVal = item.getNodeValue();
        if(nodeVal == null)
        {
            return defaultValue;
        }

        return nodeVal;
    }


    static class SFRAlternates implements Comparable<SFRAlternates>
    {
        Long lowestAddr;
        String name;
        boolean isDCR = false;
        TreeMap<Long, String> addrDevListPairs = new TreeMap<Long, String>();   // SFR address, CSV list of device part number strings that implement this SFR address

        @Override
        public int compareTo(SFRAlternates o)
        {
            if(isDCR && !o.isDCR)
            {
                return 1;
            }
            if(o.isDCR && !isDCR)
            {
                return -1;
            }
            if(this.lowestAddr < o.lowestAddr)
            {
                return -1;
            }
            if((this.lowestAddr.longValue() == o.lowestAddr.longValue()) && this.name.equals(o.name))
            {
                if(this.addrDevListPairs.size() < o.addrDevListPairs.size())
                {
                    return -1;
                }
                if(this.addrDevListPairs.size() == o.addrDevListPairs.size())
                {
                    return 0;
                }
            }
            return 1;
        }

    }

    static public void GenerateDifferentialSFRList(String inputDir, String outputFilename)
    {
        GenerateDifferentialSFRList(inputDir, outputFilename, null, null);
    }

    static public void GenerateDifferentialSFRList(String inputDir, String outputFilename, String findSFRs, String findBitfields)
    {
        Map<String, SFRAlternates> allSFRAddrMap = new HashMap<String, SFRAlternates>();    // SFR Name, List of addresses that the SFR appears at
        String[] searchListBits = null;
        String[] searchListSFRs = null;
        List<File> picFiles;

        picFiles = FindFilesRegEx(inputDir, ".*[\\.][Pp][Ii][Cc]", true);
        Map<String, Integer> devIDMap = new HashMap<String, Integer>(picFiles.size());  // "PartNumber", DevID
        TreeMap<Integer, String> partNumIDMap = new TreeMap<Integer, String>();         // DevID, "PartNumber"
        TreeMap<Integer, StringList> partsByDSNum = new TreeMap<Integer, StringList>();
        List<BitFieldParams> BitFieldDifferences = new ArrayList<BitFieldParams>();
        DeviceProductLines allProducts = new DeviceProductLines();
        String partNumber;
        String genericPartNumber;

        if(findBitfields != null)
        {
            searchListBits = findBitfields.split(",[\\s]*");
        }
        if(findSFRs != null)
        {
            searchListSFRs = findSFRs.split(",[\\s]*");
        }

        DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
        DocumentBuilder db = null;
        try
        {
            db = dbf.newDocumentBuilder();
        }
        catch(ParserConfigurationException ex)
        {
            Logger.getLogger(EDCReader.class.getName()).log(Level.SEVERE, null, ex);
        }
        if(db != null)
        {
            for(File pic : picFiles)
            {
                partNumber = pic.getName().replaceFirst("[.][Pp][Ii][Cc]$", "");
                if(partNumber.toUpperCase().startsWith("DS"))
                {
                    partNumber = "ds" + partNumber.substring(2);
                }
                System.out.println("Processing " + partNumber);

                if(partNumber.startsWith("dsPIC30"))
                {
                    allProducts.dsPIC30F.add(partNumber);
                }
                else if(partNumber.startsWith("dsPIC33F"))
                {
                    allProducts.dsPIC33F.add(partNumber);
                }
                else if(partNumber.startsWith("dsPIC33E") || partNumber.startsWith("dsPIC33D"))
                {
                    allProducts.dsPIC33E.add(partNumber);
                }
                else if(partNumber.startsWith("dsPIC33C"))
                {
                    if(partNumber.contains("RA"))
                    {
                        continue;
                    }
                    allProducts.dsPIC33C.add(partNumber);
                }
                else if(partNumber.startsWith("PIC24E"))
                {
                    allProducts.PIC24E.add(partNumber);
                }
                else if(partNumber.startsWith("PIC24H"))
                {
                    allProducts.PIC24H.add(partNumber);
                }
                else if(partNumber.startsWith("PIC24F"))
                {
                    if(partNumber.contains("K"))
                    {
                        allProducts.PIC24FK.add(partNumber);
                    }
                    else
                    {
                        allProducts.PIC24FJ.add(partNumber);
                    }
                }

                try
                {
                    Document doc = db.parse(pic);
                    NodeList mainPICNode = doc.getElementsByTagName("edc:PIC");
                    if(mainPICNode.getLength() != 1)
                    {
                        System.err.printf("    Found %1$d edc:PIC nodes!\n", mainPICNode.getLength());
                    }
                    String dsidStr = GetAttr(mainPICNode.item(0), "edc:dsid");
                    if(dsidStr.equals(""))
                    {
                        dsidStr = "222222";
                    }
                    else if(dsidStr.equals("none"))
                    {
                        dsidStr = "30";
                    }
                    try
                    {
                        int ds = Integer.decode(dsidStr);
                        StringList partsInDS = partsByDSNum.get(ds);
                        if(partsInDS == null)
                        {
                            partsInDS = new StringList();
                        }
                        partsInDS.list.add(partNumber);
                        partsByDSNum.put(ds, partsInDS);
                    }
                    catch(NumberFormatException ex)
                    {
                        System.err.printf("    Can't convert '%1$s' to data sheet number\n", dsidStr);
                    }

                    NodeList devIDNodes = doc.getElementsByTagName("edc:DeviceIDSector");
                    if(devIDNodes.getLength() != 1)
                    {
                        System.err.printf("\n    Found %1$d edc:DeviceIDSector nodes!\n", devIDNodes.getLength());
                    }
                    String devVal = GetAttr(devIDNodes.item(0), "edc:value");
                    String devMask = GetAttr(devIDNodes.item(0), "edc:mask");
                    if(Long.decode(devMask) != 0x00000000FFFF0000L)
                    {
                        System.err.printf("\n    DEVID mask is not 0xFFFF0000: %1$s\n", devMask);
                    }
                    if(devVal == null)
                    {
                        System.err.printf("\n    edc:value for DeviceIDSector not found\n");
                    }
                    // Add device to devIDMap
                    Integer devID = (int)(Long.decode(devVal) >> 16);
                    devIDMap.put(partNumber, devID);

                    // Add device to PartNumIDMap, overwriting existing value if devID already used and this new part number has a shorter name
                    if(partNumIDMap.get(devID) != null)
                    {
                        if(partNumber.length() <= partNumIDMap.get(devID).length())
                        {
                            partNumIDMap.put(devID, partNumber);
                        }
                    }
                    else
                    {
                        partNumIDMap.put(devID, partNumber);
                    }

                    NodeList sfrNodes = doc.getElementsByTagName("edc:SFRDef");
                    DevRegisters regs = IndexSFRDCRNodes(sfrNodes);
                    NodeList dcrNodes = doc.getElementsByTagName("edc:DCRDef");
                    regs.addAll(IndexSFRDCRNodes(dcrNodes));

//                    String cachedName = String.format(inputDir + File.separatorChar + "cached" + File.separatorChar + partNumber + ".bin");
//                    Multifunction.SaveObjToFile(regs, cachedName);
//                    DevRegisters regsRead = (DevRegisters)Multifunction.ReadObjFromFile(cachedName);
//                    if(regsRead == null)
//                    {
//                        System.err.printf("Error: could not read \"%1$s\"\n", cachedName);
//                    }
//                    else
//                    {
//                        regs = regsRead;
//                    }
                    if(regs.RegByName.containsKey("PSVPAG"))
                    {
                        genericPartNumber = "generic-16bit";
                        allProducts.generic_16bit.add(partNumber);
                    }
                    else if(partNumber.startsWith("PIC24F"))
                    {
                        genericPartNumber = "generic-16bit-da";
                        allProducts.generic_16bit_da.add(partNumber);
                    }
                    else if(partNumber.startsWith("dsPIC33C"))
                    {
                        genericPartNumber = "generic-16dsp-ch";
                        allProducts.generic_16dsp_ch.add(partNumber);
                    }
                    else if(partNumber.startsWith("dsPIC33E") || partNumber.startsWith("PIC24E") || partNumber.startsWith("dsPIC33D"))
                    {
                        genericPartNumber = "generic-16bit-ep";
                        allProducts.generic_16bit_ep.add(partNumber);
                    }
                    else
                    {
                        genericPartNumber = "generic-unknown";
                        allProducts.generic_unknown.add(partNumber);
                    }

                    if(searchListSFRs != null)
                    {
                        for(String search : searchListSFRs)
                        {
                            EZBL_SFR foundSFR = regs.RegByName.get(search);
                            if(foundSFR == null)
                            {
                                continue;
                            }
                            BitFieldParams bfp = new BitFieldParams();
                            bfp.isSFR = true;
                            bfp.isDCR = foundSFR.isDCR;
                            bfp.name = search;
                            bfp.length = foundSFR.width;
                            bfp.mask = foundSFR.bitsDefined;
                            bfp.position = 0;
                            bfp.parentName = foundSFR.srcModule;
                            bfp.containedByGenericDevs.add(genericPartNumber);
                            bfp.addr = foundSFR.addr;
                            bfp.devsWithMatchingAddr.add(partNumber);
                            bfp.devsWithMatchingParentAndPosition.add(partNumber);
                            bfp.devsWithMatchingAddrAndParentAndPosition.add(partNumber);
                            bfp.devsWithMatchingPosition.add(partNumber);

                            boolean foundMatch = false;
                            for(BitFieldParams cmp : BitFieldDifferences)
                            {
                                if(bfp.compareTo(cmp) == 0)
                                {
                                    foundMatch = true;
                                    cmp.devsWithMatchingAddr.add(partNumber);
                                    cmp.devsWithMatchingParentAndPosition.add(partNumber);
                                    cmp.devsWithMatchingAddrAndParentAndPosition.add(partNumber);
                                    cmp.devsWithMatchingPosition.add(partNumber);
                                    if(!cmp.containedByGenericDevs.contains(genericPartNumber))
                                    {
                                        cmp.containedByGenericDevs.add(genericPartNumber);
                                    }
                                    break;
                                }
                            }
                            if(!foundMatch)
                            {
                                BitFieldDifferences.add(bfp);
                            }
                        }
                    }

                    if(searchListBits != null)
                    {
                        for(String search : searchListBits)
                        {
                            SFRList regsWithBitField = regs.RegsWithBitfield.get(search);
                            if(regsWithBitField == null)
                            {
                                continue;
                            }
                            for(EZBL_SFR foundBitSFRs : regsWithBitField.list)
                            {
                                BitField bf = foundBitSFRs.bitfieldByName.get(search);
                                BitFieldParams bfp = new BitFieldParams();
                                bfp.isBitfield = true;
                                bfp.name = bf.name;
                                bfp.length = bf.length;
                                bfp.mask = bf.implementedBits;
                                bfp.position = bf.position;
                                bfp.parentName = bf.parentSFR.name;
                                bfp.containedByGenericDevs.add(genericPartNumber);
                                bfp.addr = bf.parentSFR.addr;
                                bfp.devsWithMatchingAddr.add(partNumber);
                                bfp.devsWithMatchingParentAndPosition.add(partNumber);
                                bfp.devsWithMatchingAddrAndParentAndPosition.add(partNumber);
                                bfp.devsWithMatchingPosition.add(partNumber);

                                boolean foundMatch = false;
                                for(BitFieldParams cmp : BitFieldDifferences)
                                {
                                    if(bfp.compareTo(cmp) == 0)
                                    {
                                        foundMatch = true;
                                        cmp.devsWithMatchingAddr.add(partNumber);
                                        cmp.devsWithMatchingParentAndPosition.add(partNumber);
                                        cmp.devsWithMatchingAddrAndParentAndPosition.add(partNumber);
                                        cmp.devsWithMatchingPosition.add(partNumber);
                                        if(!cmp.containedByGenericDevs.contains(genericPartNumber))
                                        {
                                            cmp.containedByGenericDevs.add(genericPartNumber);
                                        }
                                        break;
                                    }
                                }
                                if(!foundMatch)
                                {
                                    BitFieldDifferences.add(bfp);
                                }
                            }
                        }
                    }

                    for(EZBL_SFR s : regs.RegByAddress.values())    // List all addresses appearing in EDC for the given SFR name
                    {
                        SFRAlternates altSFR = allSFRAddrMap.get(s.name);
                        if(altSFR == null)
                        {
                            altSFR = new SFRAlternates();
                            altSFR.name = s.name;
                            altSFR.isDCR = s.isDCR;
                            altSFR.lowestAddr = s.addr;
                            altSFR.addrDevListPairs.put(s.addr, partNumber);
                            allSFRAddrMap.put(s.name, altSFR);
                        }
                        else
                        {
                            String applicableParts = altSFR.addrDevListPairs.get(s.addr);
                            applicableParts += ", " + partNumber;
                            if(altSFR.lowestAddr > s.addr)
                            {
                                altSFR.lowestAddr = s.addr;
                            }
                            altSFR.addrDevListPairs.put(s.addr, applicableParts);
                            allSFRAddrMap.put(s.name, altSFR);
                        }
                    }
                }
                catch(SAXException | IOException ex)
                {
                    System.err.print("  Could not open: \"" + pic.getPath() + "\"\n"
                            + "                  " + ex.getMessage());
                }
            }

            // All .pic files processed. Find superset part name in each family and populate the top level for it.
            for(int dsNum : partsByDSNum.keySet())
            {
                StringList dsPartList = partsByDSNum.get(dsNum);
                dsPartList.sortPartNumbers();
                dsPartList.topString = dsPartList.list.get(0) + " family";
                partsByDSNum.put(dsNum, dsPartList);
            }

            Collections.sort(BitFieldDifferences);
        }

        String[] partList = new String[devIDMap.size()];
        devIDMap.keySet().toArray(partList);
        Arrays.sort(partList, new PICNameComparator());

        Map<String, StringList> fieldsOnGeneric = new TreeMap<String, StringList>();
        List<String> outLines = new ArrayList<String>(partList.length * 5 + allSFRAddrMap.size());

        outLines.add(String.format("Searched %1$d part numbers\n\n", devIDMap.size()));

        for(BitFieldParams bfp : BitFieldDifferences)
        {
            bfp.GroupMatchingDevNames(allProducts, partsByDSNum);
            String foundItem;

            if(bfp.isSFR)   // SFR search
            {
                String formatStr = bfp.isDCR
                        ? "0x%1$06X %2$-10s<%3$s:%4$s>    // %5$s\n"
                        : "  0x%1$04X %2$-10s<%3$s:%4$s>    // %5$s\n";
                foundItem = String.format(formatStr, bfp.addr, bfp.name, bfp.position + bfp.length - 1, bfp.position, CatStringList(bfp.devsWithMatchingAddr, ", "));
            }
            else if(bfp.isBitfield) // Bitfield search
            {
                foundItem = String.format("%1$s<%2$s>, #%3$d", bfp.parentName, bfp.name, bfp.position);
                String extras = "";
                String onParts = CatStringList(bfp.devsWithMatchingParentAndPosition, ", ");

                if(bfp.parentName.matches("^IEC.*|^IFS.*|^IPC.*"))
                {
                    int regNum = Integer.decode(bfp.parentName.substring(3));
                    extras = String.format("IRQ %1$3d", regNum * 16 + bfp.position);
                }
                else if(bfp.parentName.matches("^PMD.*"))
                {
                    int regNum = Integer.decode(bfp.parentName.substring(3));
                    extras = String.format("PMD bit offset %1$3d", (regNum - 1) * 16 + bfp.position);
                }

                foundItem = String.format("%1$-21s // %2$s On: %3$s\n", foundItem, extras, onParts);
            }
            else
            {
                continue;
            }

            for(String genericDev : bfp.containedByGenericDevs)
            {
                StringList thingList = fieldsOnGeneric.get(genericDev);
                if(thingList == null)
                {
                    thingList = new StringList();
                }
                thingList.list.add(foundItem);
                fieldsOnGeneric.put(genericDev, thingList);
            }

            outLines.add(foundItem);
        }

        outLines.add("\n\n");
        FieldsOnGenericDevice genericDevs = new FieldsOnGenericDevice();
        for(BitFieldParams dif : BitFieldDifferences)
        {
            if(dif.containedByGenericDevs.contains("generic-16bit"))
            {
                genericDevs.generic_16bit.add(dif);
            }
            if(dif.containedByGenericDevs.contains("generic-16bit-da"))
            {
                genericDevs.generic_16bit_da.add(dif);
            }
            if(dif.containedByGenericDevs.contains("generic-16bit-ep"))
            {
                genericDevs.generic_16bit_ep.add(dif);
            }
            if(dif.containedByGenericDevs.contains("generic-16dsp-ch"))
            {
                genericDevs.generic_16dsp_ch.add(dif);
            }
        }
        genericDevs.sortByFieldName();
        outLines.add("\n\ngeneric-16bit has:");
        for(BitFieldParams bfp : genericDevs.generic_16bit)
        {
            if(bfp.isBitfield)
            {
                outLines.add(String.format("\n    %1$-8s = %2$s<%3$d>\t// %4$s", bfp.name, bfp.parentName, bfp.position, CatStringList(bfp.devsWithMatchingParentAndPosition, ", ")));
            }
            else    // SFR or DCR
            {
                outLines.add(String.format("\n    %1$-8s 0x%2$04X // %3$s", bfp.name, bfp.addr, CatStringList(bfp.devsWithMatchingAddr, ", ")));
            }
        }
        outLines.add("\n\ngeneric-16bit-da has:");
        for(BitFieldParams bfp : genericDevs.generic_16bit_da)
        {
            if(bfp.isBitfield)
            {
                outLines.add(String.format("\n    %1$-8s = %2$s<%3$d>\t// %4$s", bfp.name, bfp.parentName, bfp.position, CatStringList(bfp.devsWithMatchingParentAndPosition, ", ")));
            }
            else    // SFR or DCR
            {
                outLines.add(String.format("\n    %1$-8s 0x%2$04X // %3$s", bfp.name, bfp.addr, CatStringList(bfp.devsWithMatchingAddr, ", ")));
            }
        }
        outLines.add("\n\ngeneric-16bit-ep has:");
        for(BitFieldParams bfp : genericDevs.generic_16bit_ep)
        {
            if(bfp.isBitfield)
            {
                outLines.add(String.format("\n    %1$-8s = %2$s<%3$d>\t// %4$s", bfp.name, bfp.parentName, bfp.position, CatStringList(bfp.devsWithMatchingParentAndPosition, ", ")));
            }
            else    // SFR or DCR
            {
                outLines.add(String.format("\n    %1$-8s 0x%2$04X // %3$s", bfp.name, bfp.addr, CatStringList(bfp.devsWithMatchingAddr, ", ")));
            }
        }
        outLines.add("\ngeneric-16dsp-ch has:");
        for(BitFieldParams bfp : genericDevs.generic_16dsp_ch)
        {
            if(bfp.isBitfield)
            {
                outLines.add(String.format("\n    %1$-8s = %2$s<%3$d>\t// %4$s", bfp.name, bfp.parentName, bfp.position, CatStringList(bfp.devsWithMatchingParentAndPosition, ", ")));
            }
            else    // SFR or DCR
            {
                outLines.add(String.format("\n    %1$-8s 0x%2$04X // %3$s", bfp.name, bfp.addr, CatStringList(bfp.devsWithMatchingAddr, ", ")));
            }
        }

//        for(String genericDev : fieldsOnGeneric.keySet())
//        {
//            StringList fieldReport = fieldsOnGeneric.get(genericDev);
//            fieldReport.sortSFRNames();
//            outLines.add("\n" + genericDev + " has:\n    " + CatStringList(fieldReport.list, "    "));
//        }
        outLines.add("\n\n#if defined(DEVID) || defined(SUPPRESS_DEVID_DEF)");
        for(String dev : partList)
        {
            outLines.add("\n#elif defined(__" + dev + "__)");
            outLines.add(String.format("\n#define DEVID 0x%1$04X", devIDMap.get(dev)));
        }
        outLines.add("\n#else");
        outLines.add("\n#define DEVID 0x0000");
        outLines.add("\n#endif");

        outLines.add("\n\n#define LOOKUP_DEVID(dev_name) (                                               \\");
        for(String dev : partList)
        {
            outLines.add(String.format("\n            %1$-60s) == 0 ? 0x%2$04X : \\", "__builtin_strcmp(dev_name, \"" + dev + "\"", devIDMap.get(dev)));
        }
        outLines.add(String.format("\n                                                                                 0x0000)    /* Device not found - may need to regenerate lookup table */"));

        outLines.add("\n\n#define LOOKUP_DEVNAME(devid) (                        \\");
        for(Integer devid : partNumIDMap.keySet())
        {
            outLines.add(String.format("\n            (devid) == 0x%1$04X ? %2$-22s : \\", devid, "\"" + partNumIDMap.get(devid) + "\""));
        }
        outLines.add(String.format("\n                                \"Unknown\")    /* Device not found - may need to regenerate lookup table */"));

        List<SFRAlternates> alts = new ArrayList<SFRAlternates>();
        alts.addAll(allSFRAddrMap.values());
        Collections.sort(alts);
        String formatStr;
        for(SFRAlternates a : alts)
        {
            outLines.add(String.format("\n%1$-16s ", a.name));
            for(long addr : a.addrDevListPairs.keySet())
            {
                formatStr = a.isDCR
                        ? "    0x%1$06X (%2$3d)"
                        : "    0x%1$04X (%2$3d)";
                outLines.add(String.format(formatStr, addr, a.addrDevListPairs.get(addr).split(", ").length));
            }
        }

        String fileOutContents = Multifunction.CatStringList(outLines);
        outLines.clear();

        Multifunction.UpdateFileIfDataDifferent(outputFilename, fileOutContents, true);
    }

    static public int GeneratePICAbstractionHeader(String partNumber, String outputFilename)
    {
        String fileOutContents;
        List<String> hStrings = new ArrayList<String>();
        List<String> asmStrings = new ArrayList<String>();
        List<PairWithText> RPOutEncodings = new ArrayList<PairWithText>();
        List<PairWithText> pinNumberFunctionMappings = new ArrayList<PairWithText>();
        int ret = 0;

//        try
//        {
//            PackageLoggingImpl.getInstance().installFileHandler();
//        }
//        catch(IOException ex)
//        {
//            System.err.printf("Could not switch to crownking file logging\n.");
//        }
//        PICFactory pf = PICFactory.getInstance();
        try
        {
            //com.microchip.crownking.edc.PIC pic = pf.get(partNumber); // use p pf.release(p) p = null
            //Document doc = pic.getDoc();

            //com.microchip.crownking.edc.edcDocumentLocator.familyDirs
            com.microchip.crownking.edc.edcDocumentLocator docLocator = new com.microchip.crownking.edc.edcDocumentLocator(partNumber);
            InputStream picXMLStream = docLocator.searchJarForPIC();
            if(picXMLStream == null)
            {

            }
            DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
            DocumentBuilder db = dbf.newDocumentBuilder();
            Document doc = db.parse(picXMLStream);

            NodeList sfrNodes = doc.getElementsByTagName("edc:SFRDef");
            NodeList dcrNodes = doc.getElementsByTagName("edc:DCRDef");
            Map<String, EZBL_SFR> SFRMap = IndexSFRNodes(sfrNodes);
            Map<String, EZBL_SFR> DCRMap = IndexSFRNodes(dcrNodes);
            List<EZBL_SFR> sortedSFRs = new ArrayList<EZBL_SFR>();
            sortedSFRs.addAll(SFRMap.values());
            Collections.sort(sortedSFRs);
            List<EZBL_SFR> sortedDCRs = new ArrayList<EZBL_SFR>();
            sortedSFRs.addAll(DCRMap.values());
            Collections.sort(sortedDCRs);

            for(EZBL_SFR sfr : sortedSFRs)
            {
                hStrings.add(String.format("\n#define ADDR_%1$-15s 0x%2$04X // to 0x%3$04X; Module: %4$s", sfr.name, sfr.addr, sfr.endAddr, sfr.srcModule.replace(".Module", "")));
                asmStrings.add(String.format("\n_%1$-15s = 0x%2$04X", sfr.name, sfr.addr));
                asmStrings.add(String.format("\n\t.global   _%1$s", sfr.name));
                for(BitField bf : sfr.bitfieldByName.values())
                {
                    String macroName = sfr.name + "_" + bf.name + "_POS";
                    hStrings.add(String.format("\n#define %1$-20s %2$-2d     // to %3$2d", macroName, bf.position, bf.position + bf.length));
                }
            }
            for(EZBL_SFR sfr : sortedDCRs)
            {
                hStrings.add(String.format("\n#define ADDR_%1$-15s 0x%2$06X   // to 0x%3$06X; Module: %4$s", sfr.name, sfr.addr, sfr.endAddr, sfr.srcModule.replace(".Module", "")));
                for(BitField bf : sfr.bitfieldByName.values())
                {
                    String macroName = sfr.name + "_" + bf.name + "_POS";
                    hStrings.add(String.format("\n#define %1$-20s %2$-2d     // to %3$2d; %4$s", macroName, bf.position, bf.position + bf.length, bf.desc));
                }
            }

            // Build a list of pins and what functions are multiplexed on each of them
            NodeList pinLists = doc.getElementsByTagName("edc:PinList");
            if(pinLists.getLength() != 0)
            {
                NodeList pinList = pinLists.item(0).getChildNodes();

                int pinNumber = 0;
                for(int i = 0; i < pinList.getLength(); i++)
                {
                    Node pin = pinList.item(i);
                    if(!pin.getNodeName().equals("edc:Pin"))
                    {
                        continue;
                    }
                    pinNumber++;
                    int functionNumber = 0;
                    NodeList virtualPins = pin.getChildNodes();
                    if(virtualPins.getLength() == 0)
                    {
                        continue;
                    }
                    for(int j = 0; j < virtualPins.getLength(); j++)
                    {
                        Node virtualPin = virtualPins.item(j);
                        String funcMneumonic;

                        if(!virtualPin.getNodeName().equals("edc:VirtualPin"))
                        {
                            continue;
                        }
                        functionNumber++;
                        funcMneumonic = virtualPin.getAttributes().getNamedItem("edc:name").getNodeValue();
                        pinNumberFunctionMappings.add(new PairWithText(pinNumber, functionNumber, funcMneumonic));
                    }
                }
            }

            // Build a list of RP output pin to peripheral signal encodings
            NodeList virtualPins = doc.getElementsByTagName("edc:VirtualPin");
            for(int i = 0; i < virtualPins.getLength(); i++)
            {
                Node n = virtualPins.item(i);
                if(n.hasAttributes())
                {
                    NamedNodeMap attr = n.getAttributes();
                    Node name = attr.getNamedItem("edc:name");
                    Node RPOutVal = attr.getNamedItem("edc:ppsval");
                    if((name != null) && (RPOutVal != null))
                    {
                        RPOutEncodings.add(new PairWithText(Long.decode(RPOutVal.getNodeValue()), name.getNodeValue()));
                    }
                }
            }

            //pf.release(pic);
            //pic = null;
        }
        catch(Exception ex)
        {
            System.err.println("Exception reading device database: " + ex.getMessage());
            ex.printStackTrace();
            ret = -2;
        }
        //pf = null;

//        System.setOut(originalStdout);
//        System.setErr(originalStderr);
//        nullOutput.close();
//        nullOutput = null;
        fileOutContents = "// THIS IS AN AUTO-GENERATED FILE!"
                + "\n// Created for: " + partNumber
                + "\n"
                + "\n#ifndef PIC_ABSTRACTION_H"
                + "\n#define PIC_ABSTRACTION_H"
                + "\n\n" + Multifunction.CatStringList(hStrings)
                + "\n"
                + "\n"
                + "\n // I/O pins and their named function mappings"
                + "\n //     Pin Function     Pin Number             All pin functions";
        for(int i = 0; i < pinNumberFunctionMappings.size(); i++)
        {
            PairWithText mapping = pinNumberFunctionMappings.get(i);
            String allFunctionComment = "";
            for(PairWithText mapping2 : pinNumberFunctionMappings)
            {
                if(mapping.first != mapping2.first)
                {
                    continue;
                }
                if(!mapping.equals(mapping2) && mapping.text.equals(mapping2.text))
                {
                    mapping.text = mapping.text + String.format("_AT_%1$d", mapping.first);
                }
                allFunctionComment += mapping2.text + "/";
            }
            allFunctionComment = allFunctionComment.substring(0, allFunctionComment.length() - 1);
            mapping.text = mapping.text.replace("+", "p");
            mapping.text = mapping.text.replace("-", "n");
            fileOutContents += String.format("\n#define %1$-16s %2$3d\t// Pin %2$3d: %3$s", mapping.text, mapping.first, allFunctionComment);
        }

        fileOutContents += "\n"
                + "\n"
                + "\n // Remappable Peripheral Output encodings";
        for(PairWithText mapping : RPOutEncodings)
        {
            fileOutContents += String.format("\n#define RPOUT_%1$-16s 0x%2$02Xu", mapping.text, mapping.first);
        }

        fileOutContents += "\n"
                + "\n#endif  // #ifndef PIC_ABSTRACTION_H"
                + "\n";

        Multifunction.UpdateFileIfDataDifferent(outputFilename, fileOutContents, true);

        return ret;
    }

}


/**
 * Compares PIC names by alphabetical name, compensating for internal number
 * fields (like flash size) that are not the same length
 */
class PICNameComparator implements Comparator<String>
{

    @Override
    public int compare(String x, String y)
    {
        if(x.equals(y))
        {
            return 0;
        }

        if(x.endsWith("*") && !y.endsWith("*"))
        {
            return -1;
        }
        if(y.endsWith("*") && !x.endsWith("*"))
        {
            return 1;
        }
        if(x.endsWith(" family") && !y.endsWith(" family"))
        {
            return -1;
        }
        if(y.endsWith(" family") && !x.endsWith(" family"))
        {
            return 1;
        }

        String[] devClass = new String[2];
        String[] memSize = new String[2];
        Integer[] memSizeInt = new Integer[2];
        String[] featureSet = new String[2];
        String[] featureSetNum = new String[2];
        String[] pinoutCode = new String[2];
        Integer[] pinoutCodeInt = new Integer[2];
        String[] suffix = new String[2];
        Pattern p = Pattern.compile("(?<devClass>(dsPIC30|dsPIC33|PIC24)[^0-9]+)(?<memSize>[0-9]*)(?<featureSet>[a-zA-Z]*)(?<featureSetNum>[0-9]{0,1})(?<pinoutCode>[0-9]*)(?<suffix>.*)", Pattern.DOTALL);
        Matcher m;

        for(int i = 0; i < 2; i++)
        {
            String s = i == 0 ? x : y;
            m = p.matcher(s);
            if(!m.find())
            {
                return x.compareTo(y);
            }
            devClass[i] = m.group("devClass");
            memSize[i] = m.group("memSize");
            try
            {
                memSizeInt[i] = Integer.decode(memSize[i]);
            }
            catch(NullPointerException | NumberFormatException ex)
            {
                memSizeInt[i] = -1;
            }
            featureSet[i] = m.group("featureSet");
            featureSetNum[i] = m.group("featureSetNum");
            pinoutCode[i] = m.group("pinoutCode");
            try
            {
                pinoutCodeInt[i] = Integer.parseInt(pinoutCode[i]);
            }
            catch(NullPointerException | NumberFormatException ex)
            {
                pinoutCodeInt[i] = -1;
            }
            suffix[i] = m.group("suffix");
        }
        if(((devClass[0] == null) || (devClass[1] == null)) || !devClass[0].equals(devClass[1]))
        {
            if(devClass[0].startsWith("dsPIC") && !devClass[1].startsWith("dsPIC"))
            {
                return -1;
            }
            if(devClass[1].startsWith("dsPIC") && !devClass[0].startsWith("dsPIC"))
            {
                return 1;
            }

            return x.compareTo(y);
        }

        if(!featureSet[1].equals(featureSet[0]))
        {
            return featureSet[1].compareTo(featureSet[0]);
        }
        if(!memSize[1].equals(memSize[0]))
        {
            return memSizeInt[1].compareTo(memSizeInt[0]);
        }
        if(!pinoutCode[1].equals(pinoutCode[0]))
        {
            return pinoutCodeInt[1].compareTo(pinoutCodeInt[0]);
        }
        if(!featureSetNum[1].equals(featureSetNum[0]))
        {
            return featureSetNum[1].compareTo(featureSetNum[0]);
        }
        if(!suffix[0].equals(suffix[1]))
        {
            return suffix[0].compareTo(suffix[1]);
        }
        return x.compareTo(y);
    }

}


/**
 * Compares SFR/DCR/Bitfield names by alphabetical name, compensating for
 * internal number fields
 */
class SFRNameComparator implements Comparator<String>
{

    @Override
    public int compare(String x, String y)
    {
        if((x == null) && (y != null))
        {
            return -1;
        }
        if((y == null) && (x != null))
        {
            return 1;
        }
        if(x == null)
        {
            return 0;
        }
        if(y == null)
        {
            return 0;
        }
        if(x.equals(y))
        {
            return 0;
        }
        if(x.isEmpty() && !y.isEmpty())
        {
            return -1;
        }
        if(y.isEmpty() && !x.isEmpty())
        {
            return 1;
        }

        String tokenx, tokeny;
        int numx, numy;
        Pattern p = Pattern.compile("([^0-9]+)([0-9]*)", Pattern.DOTALL);
        Matcher mx, my;

        mx = p.matcher(x);
        my = p.matcher(y);
        while(mx.find() && my.find())
        {
            tokenx = mx.group(1);
            tokeny = my.group(1);
            if(!tokenx.equals(tokeny))
            {
                return tokenx.compareTo(tokeny);
            }
            numx = Integer.parseInt(mx.group(2));
            numy = Integer.parseInt(my.group(2));
            if(numx < numy)
            {
                return -1;
            }
            if(numx > numy)
            {
                return 1;
            }
        }
        return x.compareTo(y);
    }

}


class DecodedNode
{
    Node node;
    String tagName;
    LinkedHashMap<String, String> attribs = new LinkedHashMap<String, String>();    // Attribute name, Attribute value as String

    public DecodedNode()
    {
        node = null;
        tagName = null;
    }

    public DecodedNode(Node node)
    {
        this.node = node;
        this.tagName = node.getNodeName();
        NamedNodeMap attribs = node.getAttributes();
        if(attribs != null)
        {
            for(int i = 0; i < attribs.getLength(); i++)
            {
                String val = attribs.item(i).getNodeValue();
                if(val == null)
                    val = "";
                this.attribs.put(attribs.item(i).getNodeName(), val);
            }
        }
    }

    public Integer getAttrAsInt(String attribName)
    {
        if(!this.attribs.containsKey(attribName))
            return null;
        return Integer.decode(this.attribs.get(attribName));
    }
    public int getAttrAsInt(String attribName, int defaultValue)
    {
        if(!this.attribs.containsKey(attribName))
            return defaultValue;
        return Integer.decode(this.attribs.get(attribName));
    }
    public Long getAttrAsLong(String attribName)
    {
        if(!this.attribs.containsKey(attribName))
            return null;
        return Long.decode(this.attribs.get(attribName));
    }
    public long getAttrAsLong(String attribName, long defaultValue)
    {
        if(!this.attribs.containsKey(attribName))
            return defaultValue;
        return Long.decode(this.attribs.get(attribName));
    }
    public String getAttrAsString(String attribName, String defaultValue)
    {
        if(!this.attribs.containsKey(attribName))
            return defaultValue;
        return this.attribs.get(attribName);
    }
    public String getAttrAsString(String attribName)    // Returns null if the named attribute doesn't exist
    {
        return this.attribs.get(attribName);
    }
}


class DecodedNodeList
{
    List<DecodedNode> list = new ArrayList<DecodedNode>();
    DecodedNode first = null;
    DecodedNode firstDefault = null;

    public DecodedNodeList()
    {
    }
    public DecodedNodeList(boolean setFirstDefault)
    {
        if(setFirstDefault)
            firstDefault = new DecodedNode();
    }
    public DecodedNodeList(DecodedNode firstDecodedNode)
    {
        list.add(first);
        firstDefault = firstDecodedNode;
        first = firstDecodedNode;
    }
}
