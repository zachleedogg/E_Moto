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
import static com.microchip.apps.ezbl.Multifunction.TrimQuotes;
import java.io.*;
import static java.net.InetAddress.getByAddress;
import java.net.*;
import java.util.*;


/**
 *
 * @author C12128
 */
public class PCTraceLogger
{
    private int[] PCs = new int[16384];
    private byte[] bl2Bytes;
    ELFDumpParser elf = new ELFDumpParser();
    List<String> outLines;// = new ArrayList<String>();
    String outputPath;
    Bl2b code;
    Map<Integer, Integer> instr;
    String syncString = "HELO";
    long idleTimeout = 1000 * 60;
    int[] instanceCounts = new int[256];
    int[] staticCounts = new int[256];
    int staticCount = 0;
    int netBranchesTaken = 0;
    int retfies = 0;
    int returns = 0;
    int total2Cycles = 0;
    int skips = 0;
    int skipsNotTaken = 0;
    int obviousBranches = 0; // call, goto, rcall, bra, return, retlw, retfie, goto.l
    int repeats = 0;
    int calls = 0;
    int gotoBras = 0;
    int conditionalBraches = 0;
    int conditionalBrachesTaken = 0;
    int conditionalBrachesNotTaken = 0;
    int conditionalBranchesTakenForward = 0;
    int conditionalBranchesTakenBackward = 0;
    int conditionalBranchesNotTakenForward = 0;
    int conditionalBranchesNotTakenBackward = 0;
    DatagramSocket udp = null;
    byte[] pktData = new byte[1536];
    DatagramPacket pkt = new DatagramPacket(pktData, pktData.length);
    int pktLen;
    private File file;
    private FileWriter fileOut;
    private int fileSequence = 0;
    int traceBytesProcessed = 0;
    int netInstructions = 0;
    int logLinesThisFile;
    long fileStartTime = 0;
    float instrPerSecond;

    public PCTraceLogger PCTraceLogger()
    {
        return this;
    }

    private static final Map<Integer, String> OpcodeToMnemonic;

    static
    {
        Map<Integer, String> mnemonic = new HashMap<Integer, String>(512);
        mnemonic.put(0x000000, "nop");
        mnemonic.put(0x010000, "bra/call/goto/rcall Wn");      // single instruction word versions, w-register direct destination address
        mnemonic.put(0x020000, "call        #lit23");                 // two instr word version
        mnemonic.put(0x030000, "-");
        mnemonic.put(0x040000, "goto        #lit23");                 // two instr word version
        mnemonic.put(0x050000, "retlw");
        mnemonic.put(0x060000, "retfie/return");
//        mnemonic.put(0x064000, "retfie");
//        mnemonic.put(0x060000, "return");
        mnemonic.put(0x070000, "rcall (PC rel)");                        // PC relative call
        mnemonic.put(0x080000, "do");
        mnemonic.put(0x090000, "repeat");
        mnemonic.put(0x0A0000, "-");
        mnemonic.put(0x0B0000, "-");
        mnemonic.put(0x0C0000, "bra OA");
        mnemonic.put(0x0D0000, "bra OB");
        mnemonic.put(0x0E0000, "bra SA");
        mnemonic.put(0x0F0000, "bra SB");
        mnemonic.put(0x100000, "subr.{b}      w1/w0, #lit5, Wd");   // Bit 15 selects w1/w0
        mnemonic.put(0x110000, "subr.{b}      w3/w2, #lit5, Wd");
        mnemonic.put(0x120000, "subr.{b}      w5/w4, #lit5, Wd");
        mnemonic.put(0x130000, "subr.{b}      w7/w6, #lit5, Wd");
        mnemonic.put(0x140000, "subr.{b}      w9/w8, #lit5, Wd");
        mnemonic.put(0x150000, "subr.{b}    w11/w10, #lit5, Wd");
        mnemonic.put(0x160000, "subr.{b}    w13/w12, #lit5, Wd");
        mnemonic.put(0x170000, "subr.{b}    w15/w14, #lit5, Wd");
        mnemonic.put(0x180000, "subbr.{b}     w1/w0, #lit5, Wd");
        mnemonic.put(0x190000, "subbr.{b}     w3/w2, #lit5, Wd");
        mnemonic.put(0x1A0000, "subbr.{b}     w5/w4, #lit5, Wd");
        mnemonic.put(0x1B0000, "subbr.{b}     w7/w6, #lit5, Wd");
        mnemonic.put(0x1C0000, "subbr.{b}     w9/w8, #lit5, Wd");
        mnemonic.put(0x1D0000, "subbr.{b}   w11/w10, #lit5, Wd");
        mnemonic.put(0x1E0000, "subbr.{b}   w13/w12, #lit5, Wd");
        mnemonic.put(0x1F0000, "subbr.{b}   w15/w14, #lit5, Wd");
        mnemonic.put(0x200000, "mov         #lit16, Wnd      (0000..0FFF)");
        mnemonic.put(0x210000, "mov         #lit16, Wnd      (1000..1FFF)");
        mnemonic.put(0x220000, "mov         #lit16, Wnd      (2000..2FFF)");
        mnemonic.put(0x230000, "mov         #lit16, Wnd      (3000..3FFF)");
        mnemonic.put(0x240000, "mov         #lit16, Wnd      (4000..4FFF)");
        mnemonic.put(0x250000, "mov         #lit16, Wnd      (5000..5FFF)");
        mnemonic.put(0x260000, "mov         #lit16, Wnd      (6000..6FFF)");
        mnemonic.put(0x270000, "mov         #lit16, Wnd      (7000..7FFF)");
        mnemonic.put(0x280000, "mov         #lit16, Wnd      (8000..8FFF)");
        mnemonic.put(0x290000, "mov         #lit16, Wnd      (9000..9FFF)");
        mnemonic.put(0x2A0000, "mov         #lit16, Wnd      (A000..AFFF)");
        mnemonic.put(0x2B0000, "mov         #lit16, Wnd      (B000..BFFF)");
        mnemonic.put(0x2C0000, "mov         #lit16, Wnd      (C000..CFFF)");
        mnemonic.put(0x2D0000, "mov         #lit16, Wnd      (D000..DFFF)");
        mnemonic.put(0x2E0000, "mov         #lit16, Wnd      (E000..EFFF)");
        mnemonic.put(0x2F0000, "mov         #lit16, Wnd      (F000..FFFF)");
        mnemonic.put(0x300000, "bra OV");
        mnemonic.put(0x310000, "bra C");
        mnemonic.put(0x320000, "bra Z");
        mnemonic.put(0x330000, "bra N");
        mnemonic.put(0x340000, "bra LE");
        mnemonic.put(0x350000, "bra LT");
        mnemonic.put(0x360000, "bra LEU");
        mnemonic.put(0x370000, "bra (PC rel)");
        mnemonic.put(0x380000, "bra NOV");
        mnemonic.put(0x390000, "bra NC");
        mnemonic.put(0x3A0000, "bra NZ");
        mnemonic.put(0x3B0000, "bra NN");
        mnemonic.put(0x3C0000, "bra GT");
        mnemonic.put(0x3D0000, "bra GE");
        mnemonic.put(0x3E0000, "bra GTU");
        mnemonic.put(0x3F0000, "-");
        mnemonic.put(0x400000, "add.{b}       w1/w0, #lit5, Wd");
        mnemonic.put(0x410000, "add.{b}       w3/w2, #lit5, Wd");
        mnemonic.put(0x420000, "add.{b}       w5/w4, #lit5, Wd");
        mnemonic.put(0x430000, "add.{b}       w7/w6, #lit5, Wd");
        mnemonic.put(0x440000, "add.{b}       w9/w8, #lit5, Wd");
        mnemonic.put(0x450000, "add.{b}     w11/w10, #lit5, Wd");
        mnemonic.put(0x460000, "add.{b}     w13/w12, #lit5, Wd");
        mnemonic.put(0x470000, "add.{b}     w15/w14, #lit5, Wd");
        mnemonic.put(0x480000, "addc.{b}      w1/w0, #lit5, Wd");
        mnemonic.put(0x490000, "addc.{b}      w3/w2, #lit5, Wd");
        mnemonic.put(0x4A0000, "addc.{b}      w5/w4, #lit5, Wd");
        mnemonic.put(0x4B0000, "addc.{b}      w7/w6, #lit5, Wd");
        mnemonic.put(0x4C0000, "addc.{b}      w9/w8, #lit5, Wd");
        mnemonic.put(0x4D0000, "addc.{b}    w11/w10, #lit5, Wd");
        mnemonic.put(0x4E0000, "addc.{b}    w13/w12, #lit5, Wd");
        mnemonic.put(0x4F0000, "addc.{b}    w15/w14, #lit5, Wd");
        mnemonic.put(0x500000, "sub.{b}       w1/w0, #lit5, Wd");
        mnemonic.put(0x510000, "sub.{b}       w3/w2, #lit5, Wd");
        mnemonic.put(0x520000, "sub.{b}       w5/w4, #lit5, Wd");
        mnemonic.put(0x530000, "sub.{b}       w7/w6, #lit5, Wd");
        mnemonic.put(0x540000, "sub.{b}       w9/w8, #lit5, Wd");
        mnemonic.put(0x550000, "sub.{b}     w11/w10, #lit5, Wd");
        mnemonic.put(0x560000, "sub.{b}     w13/w12, #lit5, Wd");
        mnemonic.put(0x570000, "sub.{b}     w15/w14, #lit5, Wd");
        mnemonic.put(0x580000, "subb.{b}      w1/w0, #lit5, Wd");
        mnemonic.put(0x590000, "subb.{b}      w3/w2, #lit5, Wd");
        mnemonic.put(0x5A0000, "subb.{b}      w5/w4, #lit5, Wd");
        mnemonic.put(0x5B0000, "subb.{b}      w7/w6, #lit5, Wd");
        mnemonic.put(0x5C0000, "subb.{b}      w9/w8, #lit5, Wd");
        mnemonic.put(0x5D0000, "subb.{b}    w11/w10, #lit5, Wd");
        mnemonic.put(0x5E0000, "subb.{b}    w13/w12, #lit5, Wd");
        mnemonic.put(0x5F0000, "subb.{b}    w15/w14, #lit5, Wd");
        mnemonic.put(0x600000, "and.{b}       w1/w0, #lit5, Wd");
        mnemonic.put(0x610000, "and.{b}       w3/w2, #lit5, Wd");
        mnemonic.put(0x620000, "and.{b}       w5/w4, #lit5, Wd");
        mnemonic.put(0x630000, "and.{b}       w7/w6, #lit5, Wd");
        mnemonic.put(0x640000, "and.{b}       w9/w8, #lit5, Wd");
        mnemonic.put(0x650000, "and.{b}     w11/w10, #lit5, Wd");
        mnemonic.put(0x660000, "and.{b}     w13/w12, #lit5, Wd");
        mnemonic.put(0x670000, "and.{b}     w15/w14, #lit5, Wd");
        mnemonic.put(0x680000, "xor.{b}       w1/w0, #lit5, Wd");
        mnemonic.put(0x690000, "xor.{b}       w3/w2, #lit5, Wd");
        mnemonic.put(0x6A0000, "xor.{b}       w5/w4, #lit5, Wd");
        mnemonic.put(0x6B0000, "xor.{b}       w7/w6, #lit5, Wd");
        mnemonic.put(0x6C0000, "xor.{b}       w9/w8, #lit5, Wd");
        mnemonic.put(0x6D0000, "xor.{b}     w11/w10, #lit5, Wd");
        mnemonic.put(0x6E0000, "xor.{b}     w13/w12, #lit5, Wd");
        mnemonic.put(0x6F0000, "xor.{b}     w15/w14, #lit5, Wd");
        mnemonic.put(0x700000, "ior.{b}       w1/w0, #lit5, Wd");
        mnemonic.put(0x710000, "ior.{b}       w3/w2, #lit5, Wd");
        mnemonic.put(0x720000, "ior.{b}       w5/w4, #lit5, Wd");
        mnemonic.put(0x730000, "ior.{b}       w7/w6, #lit5, Wd");
        mnemonic.put(0x740000, "ior.{b}       w9/w8, #lit5, Wd");
        mnemonic.put(0x750000, "ior.{b}     w11/w10, #lit5, Wd");
        mnemonic.put(0x760000, "ior.{b}     w13/w12, #lit5, Wd");
        mnemonic.put(0x770000, "ior.{b}     w15/w14, #lit5, Wd");
        mnemonic.put(0x780000, "mov{.b}     Ws, Wd");
        mnemonic.put(0x790000, "mov{.b}     Ws, Wd");
        mnemonic.put(0x7A0000, "mov{.b}     Ws, Wd");
        mnemonic.put(0x7B0000, "mov{.b}     Ws, Wd");
        mnemonic.put(0x7C0000, "mov{.b}     Ws, Wd");
        mnemonic.put(0x7D0000, "mov{.b}     Ws, Wd");
        mnemonic.put(0x7E0000, "mov{.b}     Ws, Wd");
        mnemonic.put(0x7F0000, "mov{.b}     Ws, Wd");
        mnemonic.put(0x800000, "mov         f, Wnd           (0000..1FFF)");
        mnemonic.put(0x810000, "mov         f, Wnd           (2000..3FFF)");
        mnemonic.put(0x820000, "mov         f, Wnd           (4000..5FFF)");
        mnemonic.put(0x830000, "mov         f, Wnd           (6000..7FFF)");
        mnemonic.put(0x840000, "mov         f, Wnd           (8000..9FFF)");
        mnemonic.put(0x850000, "mov         f, Wnd           (A000..BFFF)");
        mnemonic.put(0x860000, "mov         f, Wnd           (C000..DFFF)");
        mnemonic.put(0x870000, "mov         f, Wnd           (E000..FFFF)");
        mnemonic.put(0x880000, "mov         Wns, f           (0000..1FFF)");
        mnemonic.put(0x890000, "mov         Wns, f           (2000..3FFF)");
        mnemonic.put(0x8A0000, "mov         Wns, f           (4000..5FFF)");
        mnemonic.put(0x8B0000, "mov         Wns, f           (6000..7FFF)");
        mnemonic.put(0x8C0000, "mov         Wns, f           (8000..9FFF)");
        mnemonic.put(0x8D0000, "mov         Wns, f           (A000..BFFF)");
        mnemonic.put(0x8E0000, "mov         Wns, f           (C000..DFFF)");
        mnemonic.put(0x8F0000, "mov         Wns, f           (E000..FFFF)");
        mnemonic.put(0x900000, "mov{.b}     [Ws+Slit10], Wnd (   0.. 127)");
        mnemonic.put(0x910000, "mov{.b}     [Ws+Slit10], Wnd ( 128.. 255)");
        mnemonic.put(0x920000, "mov{.b}     [Ws+Slit10], Wnd ( 256.. 383)");
        mnemonic.put(0x930000, "mov{.b}     [Ws+Slit10], Wnd ( 384.. 511)");
        mnemonic.put(0x940000, "mov{.b}     [Ws+Slit10], Wnd (-512..-385)");
        mnemonic.put(0x950000, "mov{.b}     [Ws+Slit10], Wnd (-384..-257)");
        mnemonic.put(0x960000, "mov{.b}     [Ws+Slit10], Wnd (-256..-129)");
        mnemonic.put(0x970000, "mov{.b}     [Ws+Slit10], Wnd (-128..  -1)");
        mnemonic.put(0x980000, "mov{.b}     Wns, [Wd+Slit10] (   0.. 127)");
        mnemonic.put(0x990000, "mov{.b}     Wns, [Wd+Slit10] ( 128.. 255)");
        mnemonic.put(0x9A0000, "mov{.b}     Wns, [Wd+Slit10] ( 256.. 383)");
        mnemonic.put(0x9B0000, "mov{.b}     Wns, [Wd+Slit10] ( 384.. 511)");
        mnemonic.put(0x9C0000, "mov{.b}     Wns, [Wd+Slit10] (-512..-385)");
        mnemonic.put(0x9D0000, "mov{.b}     Wns, [Wd+Slit10] (-384..-257)");
        mnemonic.put(0x9E0000, "mov{.b}     Wns, [Wd+Slit10] (-256..-129)");
        mnemonic.put(0x9F0000, "mov{.b}     Wns, [Wd+Slit10] (-128..  -1)");
        mnemonic.put(0xA00000, "bset{.b}    Ws, #bit4");
        mnemonic.put(0xA10000, "bclr{.b}    Ws, #bit4");
        mnemonic.put(0xA20000, "btg{.b}     Ws, #bit4");
        mnemonic.put(0xA30000, "btst.[cz]   Ws, #bit4");
        mnemonic.put(0xA40000, "btsts.[cz]  Ws, #bit4");
        mnemonic.put(0xA50000, "btst.[cz]   Ws, Wb");
        mnemonic.put(0xA60000, "btss        Ws, #lit4");
        mnemonic.put(0xA70000, "btsc        Ws, #lit4");
        mnemonic.put(0xA80000, "bset{.b}    f,  #bit4");
        mnemonic.put(0xA90000, "bclr{.b}    f,  #bit4");
        mnemonic.put(0xAA0000, "btg{.b}     f,  #bit4");
        mnemonic.put(0xAB0000, "btst{.b}    f,  #bit4");
        mnemonic.put(0xAC0000, "btsts{.b}   f,  #bit4");
        mnemonic.put(0xAD0000, "bsw.[cz]    Ws, Wb");
        mnemonic.put(0xAE0000, "btss        f,  #lit4");
        mnemonic.put(0xAF0000, "btsc        f,  #lit4");
        mnemonic.put(0xB00000, "add/addc");
        mnemonic.put(0xB10000, "sub/subb");
        mnemonic.put(0xB20000, "and/xor");
        mnemonic.put(0xB30000, "ior/mov");
        mnemonic.put(0xB40000, "add.{b} f/addc.{b} f");
        mnemonic.put(0xB50000, "sub.{b} f/subb.{b} f");
        mnemonic.put(0xB60000, "and.{b} f/xor.{b} f");
        mnemonic.put(0xB70000, "ior.{b} f/mov.{b} WREG, f");
        mnemonic.put(0xB80000, "mul.us/mul.uu");
        mnemonic.put(0xB90000, "mul.ss/mul.su");
        mnemonic.put(0xBA0000, "tblrdh/tblrdl");
        mnemonic.put(0xBB0000, "tblwth/tblwtl");
        mnemonic.put(0xBC0000, "mul.{b}     f");
        mnemonic.put(0xBD0000, "sub/subb");
        mnemonic.put(0xBE0000, "mov.d       Ws, Wnd/mov.d Wns, Wd");
        mnemonic.put(0xBF0000, "mov.{b}     f");
        mnemonic.put(0xC00000, "mac/mpy/mpy.n/msc");
        mnemonic.put(0xC10000, "mac/mpy/mpy.n/msc");
        mnemonic.put(0xC20000, "mac/mpy/mpy.n/msc");
        mnemonic.put(0xC30000, "clrac");
        mnemonic.put(0xC40000, "mac/mpy/mpy.n/msc");
        mnemonic.put(0xC50000, "mac/mpy/mpy.n/msc");
        mnemonic.put(0xC60000, "mac/mpy/mpy.n/msc");
        mnemonic.put(0xC70000, "movsac");
        mnemonic.put(0xC80000, "sftac");
        mnemonic.put(0xC90000, "add         Ws, #Slit4, Acc");
        mnemonic.put(0xCA0000, "lac");
        mnemonic.put(0xCB0000, "add/neg/sub");
        mnemonic.put(0xCC0000, "sac");
        mnemonic.put(0xCD0000, "sac.r");
        mnemonic.put(0xCE0000, "-");
        mnemonic.put(0xCF0000, "ff1l/ff1r");
        mnemonic.put(0xD00000, "sl.{b}      Ws, Wd");
        mnemonic.put(0xD10000, "asr/lsr");
        mnemonic.put(0xD20000, "rlc/rlnc");
        mnemonic.put(0xD30000, "rrc/rrnc");
        mnemonic.put(0xD40000, "sl.{b}      f");
        mnemonic.put(0xD50000, "asr/lsr");
        mnemonic.put(0xD60000, "rlc/rlnc");
        mnemonic.put(0xD70000, "rrc/rrnc");
        mnemonic.put(0xD80000, "div.s/div.u");
        mnemonic.put(0xD90000, "divf");
        mnemonic.put(0xDA0000, "-");
        mnemonic.put(0xDB0000, "-");
        mnemonic.put(0xDC0000, "-");
        mnemonic.put(0xDD0000, "sl          Wb, #lit4, Wnd");
        mnemonic.put(0xDE0000, "asr/lsr");
        mnemonic.put(0xDF0000, "fbcl");
        mnemonic.put(0xE00000, "cp0         Ws");
        mnemonic.put(0xE10000, "cp/cpb      Wb, #lit/Ws");
        mnemonic.put(0xE20000, "cp0         f");
        mnemonic.put(0xE30000, "cp/cpb      f");
        mnemonic.put(0xE40000, "-");
        mnemonic.put(0xE50000, "-");
        mnemonic.put(0xE60000, "cpbgt/cpblt/cpsgt/cpslt");
        mnemonic.put(0xE70000, "cpbeq/cpbne/cpseq/cpsne");
        mnemonic.put(0xE80000, "inc/inc2    Ws, Wd");
        mnemonic.put(0xE90000, "dec/dec2    Ws, Wd");
        mnemonic.put(0xEA0000, "com/neg     Ws, Wd");
        mnemonic.put(0xEB0000, "clr/setm    Wd");
        mnemonic.put(0xEC0000, "inc/inc2    f");
        mnemonic.put(0xED0000, "dec/dec2    f");
        mnemonic.put(0xEE0000, "com/neg     f");
        mnemonic.put(0xEF0000, "clr/setm    f");
        mnemonic.put(0xF00000, "ed/edac/mac/mpy");
        mnemonic.put(0xF10000, "ed/edac/mac/mpy");
        mnemonic.put(0xF20000, "ed/edac/mac/mpy");
        mnemonic.put(0xF30000, "ed/edac/mac/mpy");
        mnemonic.put(0xF40000, "-");
        mnemonic.put(0xF50000, "-");
        mnemonic.put(0xF60000, "-");
        mnemonic.put(0xF70000, "-");
        mnemonic.put(0xF80000, "push        f");
        mnemonic.put(0xF90000, "pop         f");
        mnemonic.put(0xFA0000, "lnk/ulnk");
        mnemonic.put(0xFB0000, "se/ze");
        mnemonic.put(0xFC0000, "disi");
        mnemonic.put(0xFD0000, "daw/exch/swap");
        mnemonic.put(0xFE0000, "clrwdt/movpag/pwrsav/pop.s/push.s/reset");
        mnemonic.put(0xFF0000, "nopr");
        OpcodeToMnemonic = Collections.unmodifiableMap(mnemonic);
    }

    static String GetMnemonic(Integer Opcode)
    {
        int mask = 0x00FF0000;
        if(Opcode == null)
        {
            return "no_code";
        }
        String ret = null;

        for(int i = 0; i < 1; i++)
        {
            ret = OpcodeToMnemonic.get(Opcode & mask);
            if(ret != null)
            {
                break;
            }
            mask <<= 1;
            mask &= 0x00FFFFFF;
        }
        if(ret == null)
        {
            return "unimpl";
        }
        return ret;
    }

    /**
     * Builds a report containing a static count of the number of instructions
     * in a Bl2b image and how many times the given instruction class occurred.
     *
     * @param bl2file Input Bl2b image data to analyze.
     *
     * @return Report displaying instruction counts found and other statistics.
     *         If bl2file is null, null is returned.
     */
    static String StaticAnalysis(Bl2b bl2file)
    {
        List<String> outLines = new ArrayList<String>();
        int line;
        List<PairWithText> instanceCountList = new ArrayList<PairWithText>();
        int[] instrCounts = new int[256];
        int movdIndirectDirectCount;
        int instrCountTotal;            // Raw size of flash used/3
        int effectiveInstrCountTotal;   // Size of flash used/3, with nop, nopr, and some space for ivt removed
        Map<Integer, Integer> flashMap;     // Address, Opcode
        Map<Integer, Integer> opCountMap;   // Opcode, Instance count

        if(bl2file == null)
        {
            return null;
        }

        flashMap = bl2file.GetData();
        opCountMap = new HashMap<Integer, Integer>(4096);

        instrCountTotal = 0;
        effectiveInstrCountTotal = 0;
        movdIndirectDirectCount = 0;
        for(int opcode : flashMap.values())
        {
            int opclass = (opcode & 0xFF0000) >> 16;
            Integer opCount = opCountMap.get(opcode);
            if(opCount == null)
            {
                opCount = 0;
            }
            opCountMap.put(opcode, opCount + 1);
            instrCounts[opclass]++;
            instrCountTotal++;
            effectiveInstrCountTotal++;
            if(opclass == 0xBE) // mov.d
            {
                if(((opcode & 0x70) != 0) && ((opcode & 0xF) != 0xF))   // 0x70 > 0 means [Ws/--Ws/++Ws/Ws--/Ws++] indirect, !0xF means Ws != w15 stack pointer
                {
                    movdIndirectDirectCount++;
                }
            }
            else if((opclass == 0x00) || (opclass == 0xFF)) // NOP and NOPR
            {
                effectiveInstrCountTotal--;
            }
        }

        line = 0;
        outLines.add(line++, String.format("\n"
                                           + "\nStatic instruction counters for .elf/.hex file contents (%1$d total instruction words, %2$d instruction words not NOP/NOPR) %3$,d bytes/%4$,d bytes:",
                                           instrCountTotal, effectiveInstrCountTotal, instrCountTotal * 3, effectiveInstrCountTotal * 3));
        for(int i = 0; i < 256; i++)
        {
            instanceCountList.add(new PairWithText((long)instrCounts[i], (long)i, GetMnemonic(i << 16)));
        }
        Collections.sort(instanceCountList);
        for(int i = 255; i >= 0; i--)
        {
            PairWithText p = instanceCountList.get(i);
            outLines.add(line++, String.format("\n  0x%1$02Xxxxx  %2$-48s: %3$,8d (%4$5.2f%% of total flash, %5$5.2f%% of effective code) %6$,d bytes", p.second, p.text, p.first, 100.0 * p.first / instrCountTotal, 100.0 * p.first / effectiveInstrCountTotal, p.first * 3));
            if(p.second == 0x00)    // Add NOTE for Nops, which can be quite common
            {
                outLines.add(line++, "  NOTE: includes IVT, PSV constants, Config words, and second instruction word in goto #lit23/call #lit23 instructions");
            }
            if(p.second == 0xBE)
            {
                outLines.add(line++, String.format("\n%1$-12smov.d [indirect, not w15], Wnd                  : %2$,8d (%3$5.2f%% of total flash, %4$5.2f%% of effective code) %5$,d bytes", "", movdIndirectDirectCount, 100.0 * movdIndirectDirectCount / instrCountTotal, 100.0 * movdIndirectDirectCount / effectiveInstrCountTotal, movdIndirectDirectCount * 3));
            }
        }


        // Print unique opcode report and effective compressed data size
        int uniqueOpcodeCount = opCountMap.size();
        instanceCountList = new ArrayList<PairWithText>(uniqueOpcodeCount);
        double minBitsPerDictionaryWord = Math.log10(uniqueOpcodeCount) / Math.log10(2);
        int compressedSize = uniqueOpcodeCount * 3 + (instrCountTotal * ((int)(minBitsPerDictionaryWord + 0.99999)) + 7) / 8;
        outLines.add(line++, String.format("\n\n\n"
                                           + "\nCompression analysis and unique opcode counts"
                                           + "\n---------------------------------------------"
                                           + "\nUnique Opcodes: %1$,d"
                                           + "\nDictionary data size: %2$,d bytes"
                                           + "\nDictionary min bits/opcode: %3$6.3f"
                                           + "\nTotal contiguous compressed size: %4$,d bytes (%5$,d bytes uncompressed, %6$5.2f%%)",
                                           uniqueOpcodeCount,
                                           uniqueOpcodeCount * 3,
                                           minBitsPerDictionaryWord,
                                           compressedSize,
                                           instrCountTotal * 3,
                                           ((double)compressedSize) / (instrCountTotal * 3)
             ));
        for(int opcode : opCountMap.keySet())
        {
            instanceCountList.add(new PairWithText((long)opCountMap.get(opcode), (long)opcode, GetMnemonic(opcode & 0xFF0000)));
        }
        Collections.sort(instanceCountList);
        for(int i = uniqueOpcodeCount - 1; i >= 0; i--)
        {
            PairWithText p = instanceCountList.get(i);
            outLines.add(line++, String.format("\n  %1$06X %2$-48s: %3$,8d (%4$5.2f%% of total flash, %5$5.2f%% of effective code) %6$,d bytes",
                                               p.second, p.text, p.first, 100.0 * p.first / instrCountTotal, 100.0 * p.first / effectiveInstrCountTotal, p.first * 3));
        }

        return CatStringList(outLines);
    }


    void StartTrace(String inputElfPath, String inputBL2Path, String outputFilePath)
    {
        String mnemonic;
        int PC;
        boolean branched;
        boolean skipped;
        boolean repeatStarted = false;
        boolean repeated = false;
        int lastPC = 0;
        int opcode;
        int ophigh;
        String label;
        boolean lastWasSkip = false;
        boolean lastWasMultiCycle = false;
        boolean lastWas2InstrWord = false;
        boolean lastWasRetfie = false;
        boolean lastWasReturn = false;
        boolean lastWasRepeat = false;
        boolean lastWasObviousBranch = false;
        boolean lastWasCall = false;
        boolean lastWasGotoBra = false;
        boolean lastWasConditionalBra = false;
        boolean lastConditionalBraTaken = false;
        int lastBranchSource = 0;
        int lastBranchTarget = 0;

        outputPath = Multifunction.FixSlashes(TrimQuotes(outputFilePath));

        // Create a UDP socket listening on port 42460 to receive the binary trace data from the PIC
        try
        {
            byte[] wildAddr = new byte[4];
            udp = new DatagramSocket(42460, getByAddress(wildAddr));
            udp.setReceiveBufferSize(256 * 1024);
            udp.setSoTimeout(100);
            udp.setBroadcast(true);
        }
        catch(SocketException ex)
        {
            System.err.printf("Error opening and configuring UDP RX socket: %s\n", ex.getMessage());
            System.exit(-1);
        }
        catch(UnknownHostException ex)
        {
            System.err.printf("Error opening wildcard address: %s\n", ex.getMessage());
            System.exit(-1);
        }


        // Load all reference data for generating the human readable trace log
        if(elf.startObjDump("C:/Program Files (x86)/Microchip/xc16/vLatestPublic/bin/xc16-objdump", "--syms", "--section-headers", inputElfPath) < 0)
        {
            System.err.printf("Can't get symbols from '%1$s'", inputElfPath);
        }
        if(elf.startObjDump("C:/Program Files (x86)/Microchip/xc16/vLatestPublic/bin/xc16-objdump", "--disassemble", inputElfPath) < 0)
        {
            System.err.printf("Can't get disassembly contents from '%1$s'", inputElfPath);
        }
        elf.parseAllObjOutput(0);
        elf.parseAllObjOutput(0);

        for(Section sec : elf.romSectionMapByName.values())
        {
            if(sec.symbols == null)
            {
                sec.LoadSymbols(elf.symbols);
            }
        }
        bl2Bytes = Multifunction.ReadFileBinary(inputBL2Path);
        code = new Bl2b(bl2Bytes, null);
        instr = code.GetData();
        for(int instruction : instr.values())
        {
            staticCounts[instruction >> 16]++;
        }
        staticCount = instr.size();


        // Check for files that already exist of the same base name and delete them
        file = new File(String.format("%1$s", outputPath));

        File folder = file.getParentFile();
        File[] existingFiles = folder.listFiles(new FilenameFilter()
        {
            @Override
            public boolean accept(File dir, String name)
            {
                return name.matches(String.format("%1$s\\.[0-9]{4}\\.txt", file.getName()));
            }
        });
        if(existingFiles != null)
        {
            for(File f : existingFiles)
            {
                f.delete();
            }
        }

        // Create the first output file
        file = new File(String.format("%1$s.%2$04d.txt", outputPath, fileSequence++));
        logLinesThisFile = 0;
        try
        {
            fileOut = new FileWriter(file);
        }
        catch(IOException ex)
        {
            System.err.printf("Could not create %s: %s\n", file.getName(), ex.getMessage());
            System.exit(-2);
        }
        outLines = new ArrayList<String>(8192);


        long startTime = System.currentTimeMillis();
        long timeout = idleTimeout;
        while(true)
        {
            if(logLinesThisFile >= 2048 * 1024) // Save log every 2 million instructions and start a new file
            {
                FlushLogData(true);
            }

            try
            {
                udp.receive(pkt);
                if(fileStartTime == 0)
                {
                    fileStartTime = System.nanoTime();
                }
            }
            catch(IOException ex)
            {
                if(System.currentTimeMillis() - startTime > timeout)
                {
                    System.out.printf("No trace data received for %1$d seconds. Terminating.\n", timeout / 1000);
                    fileStartTime += timeout * (long)1000000;
                    FlushLogData(false);
                    return;
                }
                continue;
            }
            startTime = System.currentTimeMillis();
            timeout = 2000;

            pktLen = pkt.getLength();
            if(pktLen == 4)    // New HELO sync packet
            {
                if(logLinesThisFile != 0)
                {
                    FlushLogData(true);
                }
                continue;
            }


            // First pass, count how many lines of trace data are in this packet
            int instrInPkt = 0;
            for(int i = 0; i < pktLen; i++)
            {
                if((pktData[i] & (byte)0x80) == (byte)0)
                {   // Linear PC increment count
                    instrInPkt += (int)pktData[i];
                }
                else
                {
                    instrInPkt++;
                    i += 2;
                }
            }
            netInstructions += instrInPkt;
            logLinesThisFile += instrInPkt;

            // Populate PCs array with all the PC locations represented in this trace packet
            int PCindex = 0;
            PC = lastPC;
            for(int i = 0; i < pktLen; i++)
            {
                byte b = pktData[i];
                if((b & (byte)0x80) == (byte)0)
                {   // Linear PC increment count
                    while(b-- != 0)
                    {
                        PC += 0x2;
                        PCs[PCindex++] = PC;
                    }
                }
                else
                {
                    PC = (((int)(b & (byte)0x7F)) << 16) | ((((int)pktData[i + 1]) & 0xFF) << 8) | (((int)pktData[i + 2]) & 0xFF);
                    PCs[PCindex++] = PC;
                    i += 2;
                }
            }


            for(PCindex = 0; PCindex < instrInPkt; PCindex++)
            {
                PC = PCs[PCindex];
                branched = (lastPC + 0x2 != PC);

                skipped = false;
                repeated = false;
                if(lastWasSkip)
                {
                    if(lastPC + 0x4 == PC)
                    {
                        skips++;
                        skipped = true;
                        branched = false;
                    }
                    else
                    {
                        skipsNotTaken++;
                    }
                }
                else if(lastWasRepeat || repeatStarted)
                {
                    if(lastPC == PC)
                    {
                        branched = false;
                        repeated = true;
                        repeats++;
                    }
                    else
                    {
                        repeatStarted = lastWasRepeat;
                    }
                }
                else if(lastWasConditionalBra)
                {
                    if((PC == lastBranchTarget))
                    {
                        branched = true;
                        lastConditionalBraTaken = true;
                        conditionalBrachesTaken++;
                        if(lastBranchTarget >= lastBranchSource)
                        {
                            conditionalBranchesTakenForward++;
                        }
                        else
                        {
                            conditionalBranchesTakenBackward++;
                        }
                    }
                    else
                    {
                        lastConditionalBraTaken = false;
                        conditionalBrachesNotTaken++;
                        if(lastBranchTarget >= lastBranchSource)
                        {
                            conditionalBranchesNotTakenForward++;
                        }
                        else
                        {
                            conditionalBranchesNotTakenBackward++;
                        }
                    }
                }

                if(branched)
                {
                    netBranchesTaken++;
                }

                if(!instr.containsKey(PC))  // Print special unknown line if the opcode isn't known to us (ex: new code created by bootloader is executing; not in bootloader's .elf)
                {
                    String lineBasic = String.format("\n%1$06X  ??????  (unknown)", PC);
                    outLines.add(lineBasic);
                    if(branched || skipped || repeated)
                    {
                        int lenSoFar = lineBasic.length();
                        int padLen = 16 + 38 - lenSoFar;
                        if(padLen < 2)
                        {
                            padLen = 2;
                        }
                        outLines.add(String.format("%1$" + String.valueOf(padLen) + "s%2$s",
                                                   "",
                                                   branched ? "(branched)" : skipped ? "(skipped1)" : repeated ? "(repeated)" : ""));
                    }
                    lastPC = PC;
                    lastWasSkip = false;
                    lastWasRepeat = false;
                    lastWasConditionalBra = false;
                    continue;
                }

                opcode = instr.get(PC);
                ophigh = opcode >> 16;
                if(lastWas2InstrWord && (ophigh == 0x00))   // nop
                {
                    continue;
                }
                label = "";
                Section sec = elf.romSectionMapByAddr.get(PC);
                if((sec != null) && sec.symbolsByAddr.containsKey(PC))
                {
                    label = sec.symbolsByAddr.get(PC).name;
                }

                instanceCounts[opcode >> 16]++;
                mnemonic = elf.disassemblyMap.get(PC);
                if(mnemonic == null)
                {
                    mnemonic = GetMnemonic(opcode);
                }

                //"\nPC      Opcode  Instruction                            Notes     \t\tLabel"
                String lineBasic = String.format("\n%1$06X  %2$06X  %3$s", PC, opcode, mnemonic);
                outLines.add(lineBasic);
                if(branched || skipped || repeated || !label.isEmpty())
                {
                    int lenSoFar = lineBasic.length();
                    int padLen = 16 + 38 - lenSoFar;
                    if(padLen < 2)
                    {
                        padLen = 2;
                    }
                    if(branched || skipped || repeated)
                    {
                        outLines.add(String.format("%1$" + String.valueOf(padLen) + "s%2$s",
                                                   "",
                                                   branched ? "(branched)" : skipped ? "(skipped1)" : repeated ? "(repeated)" : ""));
                        padLen = 0;
                    }
                    else
                    {
                        padLen += 10;
                    }
                    if(!label.isEmpty())
                    {
                        outLines.add(String.format("%1$" + (padLen != 0 ? String.valueOf(padLen) : "") + "s\t\t%2$s",
                                                   "",
                                                   label));
                    }
                }


                lastWasSkip = (ophigh == 0xA6) || // btss
                        (ophigh == 0xA7) || // btsc
                        (ophigh == 0xAE) || // btss f
                        (ophigh == 0xAF) || // btsc f
                        ((opcode & 0xFF83E0) == 0xE60000) || ((opcode & 0xFF83E0) == 0xE68000) ||// cpsgt/cpslt
                        ((opcode & 0xFF83E0) == 0xE78000) || ((opcode & 0xFF83E0) == 0xE78000);  // cpseq/cpsne
                lastWasMultiCycle = (ophigh == 0x01) || // bra/call/goto/rcall
                        (ophigh == 0x02) || // call
                        (ophigh == 0x04) || // goto
                        (ophigh == 0x05) || // retlw
                        (ophigh == 0x06) || // retfie/return
                        (ophigh == 0x07) || // rcall
                        (ophigh == 0xBA) || // tblrdh/tblrdl
                        (ophigh == 0xBB) || // tblwth/tblwtl
                        (ophigh == 0xBE);   // mov.d
                lastWas2InstrWord = (ophigh == 0x02) || // call
                        (ophigh == 0x04) || // goto
                        (ophigh == 0xBE);   // mov.d
                lastWasRetfie = (ophigh == 0x06) && ((opcode & 0xC000) == 0x4000);      // retfie (in retfie/return)
                lastWasReturn = (ophigh == 0x05) || // retlw
                        ((ophigh == 0x06) && ((opcode & 0xC000) != 0x4000));    // return (in retfie/return)
                lastWasObviousBranch = (ophigh == 0x01) || // bra/call/goto/rcall
                        (ophigh == 0x02) || // call
                        (ophigh == 0x04) || // goto
                        (ophigh == 0x05) || // retlw
                        (ophigh == 0x06) || // retfie/return
                        (ophigh == 0x07) || // rcall
                        (ophigh == 0x37);   // bra
                lastWasCall = ((opcode & 0xFFFFF0) == 0x012000) || // rcall Wn (pic24f)
                        ((opcode & 0xFFFFF0) == 0x010200) || // rcall Wn (dspic33e)
                        ((opcode & 0xFF87F0) == 0x018000) || // call.l Wn+1:Wn
                        ((opcode & 0xFFFFF0) == 0x010000) || // call Wn (pic24f and dspic33e)
                        (ophigh == 0x02) || // call #lit23
                        (ophigh == 0x07);   // rcall (pc relative)
                lastWasGotoBra = ((ophigh == 0x01) && // bra/call/goto/rcall
                                  !(((opcode & 0xFFFFF0) == 0x012000) || // rcall Wn (pic24f)
                                    ((opcode & 0xFFFFF0) == 0x010200) || // rcall Wn (dspic33e)
                                    ((opcode & 0xFF87F0) == 0x018000) || // call.l Wn+1:Wn
                                    ((opcode & 0xFFFFF0) == 0x010000))) || // call Wn (pic24f and dspic33e)
                        (ophigh == 0x04) || // goto
                        (ophigh == 0x37);   // bra

                lastWasConditionalBra = (ophigh == 0x0C) || // bra OA
                        (ophigh == 0x0D) || // bra OB
                        (ophigh == 0x0E) || // bra SA
                        (ophigh == 0x0F) || // bra SB
                        (ophigh == 0x30) || // bra OV
                        (ophigh == 0x31) || // bra C
                        (ophigh == 0x32) || // bra Z
                        (ophigh == 0x33) || // bra N
                        (ophigh == 0x34) || // bra LE
                        (ophigh == 0x35) || // bra LT
                        (ophigh == 0x36) || // bra LEU
                        (ophigh == 0x38) || // bra NOV
                        (ophigh == 0x39) || // bra NC
                        (ophigh == 0x3A) || // bra NZ
                        (ophigh == 0x3B) || // bra NN
                        (ophigh == 0x3C) || // bra GT
                        (ophigh == 0x3D) || // bra GE
                        (ophigh == 0x3E) || // bra GTU
                        (((ophigh == 0xE6) || // cpbgt/cpblt/!cpsgt/!cpslt
                          (ophigh == 0xE7)) && !lastWasSkip);   // cpbeq/cpbne/!cpseq/!cpsne
                lastWasRepeat = (ophigh == 0x09);       // repeat
                if(lastWasReturn)
                {
                    lastBranchSource = PC;
                    returns++;
                }
                if(lastWasRetfie)
                {
                    lastBranchSource = PC;
                    retfies++;
                }
                if(lastWas2InstrWord)
                {
                    total2Cycles++;
                }
                if(lastWasObviousBranch)
                {
                    lastBranchSource = PC;
                    obviousBranches++;
                }
                if(lastWasCall)
                {
                    lastBranchSource = PC;
                    calls++;
                }
                if(lastWasGotoBra)
                {
                    lastBranchSource = PC;
                    gotoBras++;
                }
                if(lastWasConditionalBra)
                {
                    lastBranchSource = PC;
                    conditionalBraches++;

                    int offset;
                    if((ophigh == 0xE6) || (ophigh == 0xE7))   // cpbgt/cpblt, cpbeq/cpbne
                    {
                        offset = (opcode & 0x0003F0) >> 4;
                        if((opcode & 0x3F) > 0x1F)
                        {
                            offset |= 0xFFFFFFC0;
                        }
                    }
                    else    // Standard bra C/NC/Z/NZ/OV/etc.
                    {
                        offset = opcode & 0xFFFF;
                        if((opcode & 0xFFFF) > 0x7FFF)
                        {
                            offset |= 0xFFFF0000;
                        }
                    }
                    lastBranchTarget = (PC + 0x2) + 2 * offset;
                }
                lastPC = PC;
            }


            traceBytesProcessed += pktLen;
        }
    }

    boolean FlushLogData(boolean startAnother)
    {
        String outLog = null;
        int line = 0;
        List<PairWithText> instanceCountList = new ArrayList<PairWithText>(256);


        if(outLines != null)
        {
            instrPerSecond = (float)(logLinesThisFile / ((System.nanoTime() - fileStartTime) / 1.0e9));
            netInstructions -= total2Cycles;
            outLines.add(line++, String.format("Total instructions logged: %1$,10d (%2$,7d instructions/second)", netInstructions, (int)instrPerSecond));
            outLines.add(line++, String.format("\nTwo instr word instrs:     %1$,10d", total2Cycles));
            outLines.add(line++, String.format("\nTotal times PC branched:   %1$,10d (%2$5.2f%% of total instrs)", netBranchesTaken, 100.0 * netBranchesTaken / netInstructions));
            outLines.add(line++, String.format("\n  Obvious branches:        %1$,10d (%2$5.2f%% of total PC branches); call, goto, rcall, bra, retfie, return, retlw", obviousBranches, 100.0 * obviousBranches / netBranchesTaken));
            outLines.add(line++, String.format("\n    call:                  %1$,10d (%2$5.2f%% of obvious branches); includes rcall", calls, 100.0 * calls / obviousBranches));
            outLines.add(line++, String.format("\n    return:                %1$,10d (%2$5.2f%% of obvious branches); includes retlw", returns, 100.0 * returns / obviousBranches));
            outLines.add(line++, String.format("\n    retfie:                %1$,10d (%2$5.2f%% of obvious branches)", retfies, 100.0 * retfies / obviousBranches));
            outLines.add(line++, String.format("\n    branch always:         %1$,10d (%2$5.2f%% of obvious branches); includes goto, goto.l, bra", gotoBras, 100.0 * gotoBras / obviousBranches));
            outLines.add(line++, String.format("\n  Cond branches taken:     %1$,10d (%2$5.2f%% of total PC branches; %3$5.2f%% of conditional branches total)", conditionalBrachesTaken, 100.0 * conditionalBrachesTaken / netBranchesTaken, 100.0 * conditionalBrachesTaken / (conditionalBrachesTaken + conditionalBrachesNotTaken)));
            outLines.add(line++, String.format("\n    Taken forwards:        %1$,10d (%2$5.2f%% of conditionals taken; %3$5.2f%% of conditionals pointing forwards)", conditionalBranchesTakenForward, 100.0 * conditionalBranchesTakenForward / conditionalBrachesTaken, 100.0 * conditionalBranchesTakenForward / (conditionalBranchesTakenForward + conditionalBranchesNotTakenForward)));
            outLines.add(line++, String.format("\n    Taken backwards:       %1$,10d (%2$5.2f%% of conditionals taken; %3$5.2f%% of conditionals pointing backwards)", conditionalBranchesTakenBackward, 100.0 * conditionalBranchesTakenBackward / conditionalBrachesTaken, 100.0 * conditionalBranchesTakenBackward / (conditionalBranchesTakenBackward + conditionalBranchesNotTakenBackward)));
            outLines.add(line++, String.format("\n  Surprise branches:       %1$,10d (%2$5.2f%% of total PC branches); presumably interrupts", netBranchesTaken - obviousBranches - conditionalBrachesTaken, 100.0 * (netBranchesTaken - obviousBranches - conditionalBrachesTaken) / netBranchesTaken));
            outLines.add(line++, String.format("\nBranches not taken:        %1$,10d (%2$5.2f%% of conditional branches total)", conditionalBrachesNotTaken, 100.0 * conditionalBrachesNotTaken / (conditionalBrachesTaken + conditionalBrachesNotTaken)));
            outLines.add(line++, String.format("\n    Not taken forwards:    %1$,10d (%2$5.2f%% of conditionals not taken; %3$5.2f%% of conditionals pointing forwards)", conditionalBranchesNotTakenForward, 100.0 * conditionalBranchesNotTakenForward / conditionalBrachesNotTaken, 100.0 * conditionalBranchesNotTakenForward / (conditionalBranchesTakenForward + conditionalBranchesNotTakenForward)));
            outLines.add(line++, String.format("\n    Not taken backwards:   %1$,10d (%2$5.2f%% of conditionals not taken; %3$5.2f%% of conditionals pointing backwards)", conditionalBranchesNotTakenBackward, 100.0 * conditionalBranchesNotTakenBackward / conditionalBrachesNotTaken, 100.0 * conditionalBranchesNotTakenBackward / (conditionalBranchesTakenForward + conditionalBranchesNotTakenBackward)));
            outLines.add(line++, String.format("\nSkipped instructions:      %1$,10d (%2$5.2f%% of total instrs; %3$5.2f%% of skip instrs)", skips, 100.0 * skips / netInstructions, 100.0 * skips / (skips + skipsNotTaken)));
            outLines.add(line++, String.format("\nNot skipped instructions:  %1$,10d (%2$5.2f%% of total instrs; %3$5.2f%% of skip instrs)", skipsNotTaken, 100.0 * skipsNotTaken / netInstructions, 100.0 * skipsNotTaken / (skips + skipsNotTaken)));
            outLines.add(line++, String.format("\nRepeated instructions:     %1$,10d (%2$5.2f%% of total instrs)", repeats, 100.0 * repeats / netInstructions));

            outLines.add(line++, "\n"
                                 + "\nExecution counters:");
            for(int i = 0; i < 256; i++)
            {
                instanceCountList.add(new PairWithText((long)instanceCounts[i], (long)i, GetMnemonic(i << 16)));
            }
            Collections.sort(instanceCountList);
            for(int i = 255; i >= 0; i--)
            {
                PairWithText p = instanceCountList.get(i);
                outLines.add(line++, String.format("\n  0x%1$02Xxxxx  %2$-32s: %3$,8d (%4$5.2f%% of total instrs)", p.second, p.text, p.first, 100.0 * p.first / netInstructions));
            }
            instanceCountList.clear();

            outLines.add(line++, String.format("\n"
                                               + "\nStatic instruction counters for .elf/.hex file contents (%1$d total instruction words):",
                                               staticCount));
            for(int i = 0; i < 256; i++)
            {
                instanceCountList.add(new PairWithText((long)staticCounts[i], (long)i, GetMnemonic(i << 16)));
            }
            Collections.sort(instanceCountList);
            for(int i = 255; i >= 0; i--)
            {
                PairWithText p = instanceCountList.get(i);
                outLines.add(line++, String.format("\n  0x%1$02Xxxxx  %2$-32s: %3$,8d (%4$5.2f%% of total instrs)", p.second, p.text, p.first, 100.0 * p.first / staticCount));
                if(p.second == 0x00)    // Add NOTE for Nops, which can be quite common
                {
                    outLines.add(line++, "  NOTE: includes IVT, PSV constants, Config words, and second instruction word in goto #lit23/call #lit23 instructions");
                }
            }


            outLines.add(line++, "\n"
                                 + "\nPC      Opcode  Instruction                           Notes    \t\tLabel");
            outLog = Multifunction.CatStringList(outLines);
            outLines = new ArrayList<String>(8192);
            instanceCountList = null;
        }

        try
        {
            if(outLog != null)
            {
                fileOut.write(outLog);
            }
            fileOut.close();
        }
        catch(IOException ex)
        {
            System.err.printf("Could not write to %s: %s\n", file.getName(), ex.getMessage());
            return false;
        }


        // Check if the next file already exists and if so, delete it
        if(startAnother)
        {
            file = new File(String.format("%1$s.%2$04d.txt", outputPath, fileSequence++));
            if(file.exists())
            {
                file.delete();
            }
            try
            {
                fileOut = new FileWriter(file);
            }
            catch(IOException ex)
            {
                System.err.printf("Could not create %s: %s\n", file.getName(), ex.getMessage());
                return false;
            }
            logLinesThisFile = 0;
            fileStartTime = 0;
        }

        return true;
    }
}

