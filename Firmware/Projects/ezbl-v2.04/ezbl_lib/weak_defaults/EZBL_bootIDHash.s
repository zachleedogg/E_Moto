;/*******************************************************************************
;  Easy Bootloader for PIC24 and dsPIC33 Core Source File
;
;  Summary:
;    Section for storing Bootloader and Application meta data used to identify 
;    and match the two at run time.
;
;  Description:
;    Section for storing Bootloader and Application meta data used to identify 
;    and match the two at run time.
;
;*******************************************************************************/
;
;/*******************************************************************************
;  Copyright (C) 2017 Microchip Technology Inc.
;
;  MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
;  derivatives created by any person or entity by or on your behalf, exclusively
;  with Microchip's products.  Microchip and its licensors retain all ownership
;  and intellectual property rights in the accompanying software and in all
;  derivatives here to.
;
;  This software and any accompanying information is for suggestion only.  It
;  does not modify Microchip's standard warranty for its products.  You agree
;  that you are solely responsible for testing the software and determining its
;  suitability.  Microchip has no obligation to modify, test, certify, or
;  support the software.
;
;  THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
;  EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
;  WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
;  PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS,
;  COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
;
;  IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
;  (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
;  INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
;  EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
;  ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
;  MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
;  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
;  CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
;  FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
;
;  MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
;  TERMS.
;*******************************************************************************/

    
; Allocation for:
; const EZBL_BOOTID_HASH EZBL_bootIDHash;
    .pushsection    .const, psv
    .global _EZBL_bootIDHash
    .global _EZBL_bootIDHash0
    .global _EZBL_bootIDHash1
    .global _EZBL_bootIDHash2
    .global _EZBL_bootIDHash3
    .weak   _BOOTID_HASH0
    .weak   _BOOTID_HASH1
    .weak   _BOOTID_HASH2
    .weak   _BOOTID_HASH3
_EZBL_bootIDHash:
_EZBL_bootIDHash0:
    .long   _BOOTID_HASH0       ; /* Right-most bytes when printed of SHA-256 hash for BOOTID strings in makefile */
_EZBL_bootIDHash1:
    .long   _BOOTID_HASH1
_EZBL_bootIDHash2:
    .long   _BOOTID_HASH2
_EZBL_bootIDHash3:
    .long   _BOOTID_HASH3
    .popsection
