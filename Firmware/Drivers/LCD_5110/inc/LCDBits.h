/* 
 * File:   LCDBits.h
 * Author: garrettd
 *
 * Created on February 23, 2016, 10:06 PM
 */

#ifndef LCDBITS_H
#define	LCDBITS_H

#include <xc.h> // include processor files - each processor file is guarded.
#include <stdint.h>

/*All these have spaces in them as well*/
/*Capital letters*/
#define A_ASCII 0x7e,0x11,0x11,0x11,0x7e,0x00
#define B_ASCII 0x7f,0x49,0x49,0x49,0x36,0x00
#define C_ASCII 0x3e,0x41,0x41,0x41,0x22,0x00
#define D_ASCII 0x7f,0x41,0x41,0x22,0x1c,0x00
#define E_ASCII 0x7f,0x49,0x49,0x49,0x41,0x00
#define F_ASCII 0x7f,0x09,0x09,0x09,0x01,0x00
#define G_ASCII 0x3e,0x41,0x49,0x49,0x7a,0x00
#define H_ASCII 0x7f,0x08,0x08,0x08,0x7f,0x00
#define I_ASCII 0x00,0x41,0x7f,0x41,0x00,0x00
#define J_ASCII 0x20,0x40,0x41,0x3f,0x01,0x00
#define K_ASCII 0x7f,0x08,0x14,0x22,0x41,0x00
#define L_ASCII 0x7f,0x40,0x40,0x40,0x40,0x00
#define M_ASCII 0x7f,0x02,0x0c,0x02,0x7f,0x00
#define N_ASCII 0x7f,0x04,0x08,0x10,0x7f,0x00
#define O_ASCII 0x3e,0x41,0x41,0x41,0x3e,0x00
#define P_ASCII 0x7f,0x09,0x09,0x09,0x06,0x00
#define Q_ASCII 0x3e,0x41,0x51,0x21,0x5e,0x00
#define R_ASCII 0x7f,0x09,0x19,0x29,0x46,0x00
#define S_ASCII 0x46,0x49,0x49,0x49,0x31,0x00
#define T_ASCII 0x01,0x01,0x7f,0x01,0x01,0x00
#define U_ASCII 0x3f,0x40,0x40,0x40,0x3f,0x00
#define V_ASCII 0x1f,0x20,0x40,0x20,0x1f,0x00
#define W_ASCII 0x3f,0x40,0x38,0x40,0x3f,0x00
#define X_ASCII 0x63,0x14,0x08,0x14,0x63,0x00
#define Y_ASCII 0x07,0x08,0x70,0x08,0x07,0x00
#define Z_ASCII 0x61,0x51,0x49,0x45,0x43,0x00
/*Small letters*/
#define aa_ASCII 0x20,0x54,0x54,0x54,0x78,0x00
#define bb_ASCII 0x7f,0x48,0x44,0x44,0x38,0x00
#define cc_ASCII 0x38,0x44,0x44,0x44,0x20,0x00
#define dd_ASCII 0x38,0x44,0x44,0x48,0x7f,0x00
#define ee_ASCII 0x38,0x54,0x54,0x54,0x18,0x00
#define ff_ASCII 0x08,0x7e,0x09,0x01,0x02,0x00
#define gg_ASCII 0x0c,0x52,0x52,0x52,0x3e,0x00
#define hh_ASCII 0x7f,0x08,0x04,0x04,0x78,0x00
#define ii_ASCII 0x00,0x44,0x7d,0x40,0x00,0x00
#define jj_ASCII 0x20,0x40,0x44,0x3d,0x00,0x00
#define kk_ASCII 0x7f,0x10,0x28,0x44,0x00,0x00
#define ll_ASCII 0x00,0x41,0x7f,0x40,0x00,0x00
#define mm_ASCII 0x7c,0x04,0x18,0x04,0x78,0x00
#define nn_ASCII 0x7c,0x08,0x04,0x04,0x78,0x00
#define oo_ASCII 0x38,0x44,0x44,0x44,0x38,0x00
#define pp_ASCII 0x7c,0x14,0x14,0x14,0x08,0x00
#define qq_ASCII 0x08,0x14,0x14,0x18,0x7c,0x00
#define rr_ASCII 0x7c,0x08,0x04,0x04,0x08,0x00
#define ss_ASCII 0x48,0x54,0x54,0x54,0x20,0x00
#define tt_ASCII 0x04,0x3f,0x44,0x40,0x20,0x00
#define uu_ASCII 0x3c,0x40,0x40,0x20,0x7c,0x00
#define vv_ASCII 0x1c,0x20,0x40,0x20,0x1c,0x00
#define ww_ASCII 0x3c,0x40,0x30,0x40,0x3c,0x00
#define xx_ASCII 0x44,0x28,0x10,0x28,0x44,0x00
#define yy_ASCII 0x0c,0x50,0x50,0x50,0x3c,0x00
#define zz_ASCII 0x44,0x64,0x54,0x4c,0x44,0x00

#define space_ASCII 0x00,0x00,0x00,0x00,0x00,0x00
#define comma_ASCII 0x00,0x50,0x30,0x00,0x00,0x00
#define excla_ASCII 0x00,0x00,0x5f,0x00,0x00,0x00
#define colon_ASCII 0x00,0x36,0x36,0x00,0x00,0x00

/* All These contain a space in it as well in the 6th hex value */
#define SIXOFNOTHING 0x00,0x00,0x00,0x00,0x00,0x00
#define ZERO_ASCII 0x3e,0x51,0x49,0x45,0x3e,0x00
#define ONE_ASCII 0x00,0x42,0x7f,0x40,0x00,0x00
#define TWO_ASCII 0x42,0x61,0x51,0x49,0x46,0x00
#define THREE_ASCII 0x21,0x41,0x45,0x4b,0x31,0x00
#define FOUR_ASCII 0x18,0x14,0x12,0x7f,0x10,0x00
#define FIVE_ASCII 0x27,0x45,0x45,0x45,0x39,0x00
#define SIX_ASCII 0x3c,0x4a,0x49,0x49,0x30,0x00
#define SEVEN_ASCII 0x01,0x71,0x09,0x05,0x03,0x00
#define EIGHT_ASCII 0x36,0x49,0x49,0x49,0x36,0x00
#define NINE_ASCII 0x06,0x49,0x49,0x29,0x1e,0x00


//These are the text values that are pixels for 5 pixel by 8 pixel
static const uint8_t ASCII[96][5] = {
   {0x00, 0x00, 0x00, 0x00, 0x00} // 20
  ,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
  ,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
  ,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
  ,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
  ,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
  ,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
  ,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
  ,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
  ,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
  ,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
  ,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
  ,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
  ,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
  ,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
  ,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
  ,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
  ,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
  ,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
  ,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
  ,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
  ,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
  ,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
  ,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
  ,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
  ,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
  ,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
  ,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
  ,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
  ,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
  ,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
  ,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
  ,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
  ,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
  ,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
  ,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
  ,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
  ,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
  ,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
  ,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
  ,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
  ,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
  ,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
  ,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
  ,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
  ,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
  ,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
  ,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
  ,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
  ,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
  ,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
  ,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
  ,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
  ,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
  ,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
  ,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
  ,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
  ,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
  ,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
  ,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
  ,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c blackslash
  ,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
  ,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
  ,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
  ,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
  ,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
  ,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
  ,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
  ,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
  ,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
  ,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
  ,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
  ,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
  ,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
  ,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
  ,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
  ,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
  ,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
  ,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
  ,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
  ,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
  ,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
  ,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
  ,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
  ,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
  ,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
  ,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
  ,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
  ,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
  ,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
  ,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
  ,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
  ,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
  ,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
  ,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ~
  ,{0x78, 0x46, 0x41, 0x46, 0x78} // 7f DEL
};

static const uint8_t ASCII_SPACE[96][6] = {
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00} // 20
  ,{0x00, 0x00, 0x5f, 0x00, 0x00, 0x00} // 21 !
  ,{0x00, 0x07, 0x00, 0x07, 0x00, 0x00} // 22 "
  ,{0x14, 0x7f, 0x14, 0x7f, 0x14, 0x00} // 23 #
  ,{0x24, 0x2a, 0x7f, 0x2a, 0x12, 0x00} // 24 $
  ,{0x23, 0x13, 0x08, 0x64, 0x62, 0x00} // 25 %
  ,{0x36, 0x49, 0x55, 0x22, 0x50, 0x00} // 26 &
  ,{0x00, 0x05, 0x03, 0x00, 0x00, 0x00} // 27 '
  ,{0x00, 0x1c, 0x22, 0x41, 0x00, 0x00} // 28 (
  ,{0x00, 0x41, 0x22, 0x1c, 0x00, 0x00} // 29 )
  ,{0x14, 0x08, 0x3e, 0x08, 0x14, 0x00} // 2a *
  ,{0x08, 0x08, 0x3e, 0x08, 0x08, 0x00} // 2b +
  ,{0x00, 0x50, 0x30, 0x00, 0x00, 0x00} // 2c ,
  ,{0x08, 0x08, 0x08, 0x08, 0x08, 0x00} // 2d -
  ,{0x00, 0x60, 0x60, 0x00, 0x00, 0x00} // 2e .
  ,{0x20, 0x10, 0x08, 0x04, 0x02, 0x00} // 2f /
  ,{0x3e, 0x51, 0x49, 0x45, 0x3e, 0x00} // 30 0
  ,{0x00, 0x42, 0x7f, 0x40, 0x00, 0x00} // 31 1
  ,{0x42, 0x61, 0x51, 0x49, 0x46, 0x00} // 32 2
  ,{0x21, 0x41, 0x45, 0x4b, 0x31, 0x00} // 33 3
  ,{0x18, 0x14, 0x12, 0x7f, 0x10, 0x00} // 34 4
  ,{0x27, 0x45, 0x45, 0x45, 0x39, 0x00} // 35 5
  ,{0x3c, 0x4a, 0x49, 0x49, 0x30, 0x00} // 36 6
  ,{0x01, 0x71, 0x09, 0x05, 0x03, 0x00} // 37 7
  ,{0x36, 0x49, 0x49, 0x49, 0x36, 0x00} // 38 8
  ,{0x06, 0x49, 0x49, 0x29, 0x1e, 0x00} // 39 9
  ,{0x00, 0x36, 0x36, 0x00, 0x00, 0x00} // 3a :
  ,{0x00, 0x56, 0x36, 0x00, 0x00, 0x00} // 3b ;
  ,{0x08, 0x14, 0x22, 0x41, 0x00, 0x00} // 3c <
  ,{0x14, 0x14, 0x14, 0x14, 0x14, 0x00} // 3d =
  ,{0x00, 0x41, 0x22, 0x14, 0x08, 0x00} // 3e >
  ,{0x02, 0x01, 0x51, 0x09, 0x06, 0x00} // 3f ?
  ,{0x32, 0x49, 0x79, 0x41, 0x3e, 0x00} // 40 @
  ,{0x7e, 0x11, 0x11, 0x11, 0x7e, 0x00} // 41 A
  ,{0x7f, 0x49, 0x49, 0x49, 0x36, 0x00} // 42 B
  ,{0x3e, 0x41, 0x41, 0x41, 0x22, 0x00} // 43 C
  ,{0x7f, 0x41, 0x41, 0x22, 0x1c, 0x00} // 44 D
  ,{0x7f, 0x49, 0x49, 0x49, 0x41, 0x00} // 45 E
  ,{0x7f, 0x09, 0x09, 0x09, 0x01, 0x00} // 46 F
  ,{0x3e, 0x41, 0x49, 0x49, 0x7a, 0x00} // 47 G
  ,{0x7f, 0x08, 0x08, 0x08, 0x7f, 0x00} // 48 H
  ,{0x00, 0x41, 0x7f, 0x41, 0x00, 0x00} // 49 I
  ,{0x20, 0x40, 0x41, 0x3f, 0x01, 0x00} // 4a J
  ,{0x7f, 0x08, 0x14, 0x22, 0x41, 0x00} // 4b K
  ,{0x7f, 0x40, 0x40, 0x40, 0x40, 0x00} // 4c L
  ,{0x7f, 0x02, 0x0c, 0x02, 0x7f, 0x00} // 4d M
  ,{0x7f, 0x04, 0x08, 0x10, 0x7f, 0x00} // 4e N
  ,{0x3e, 0x41, 0x41, 0x41, 0x3e, 0x00} // 4f O
  ,{0x7f, 0x09, 0x09, 0x09, 0x06, 0x00} // 50 P
  ,{0x3e, 0x41, 0x51, 0x21, 0x5e, 0x00} // 51 Q
  ,{0x7f, 0x09, 0x19, 0x29, 0x46, 0x00} // 52 R
  ,{0x46, 0x49, 0x49, 0x49, 0x31, 0x00} // 53 S
  ,{0x01, 0x01, 0x7f, 0x01, 0x01, 0x00} // 54 T
  ,{0x3f, 0x40, 0x40, 0x40, 0x3f, 0x00} // 55 U
  ,{0x1f, 0x20, 0x40, 0x20, 0x1f, 0x00} // 56 V
  ,{0x3f, 0x40, 0x38, 0x40, 0x3f, 0x00} // 57 W
  ,{0x63, 0x14, 0x08, 0x14, 0x63, 0x00} // 58 X
  ,{0x07, 0x08, 0x70, 0x08, 0x07, 0x00} // 59 Y
  ,{0x61, 0x51, 0x49, 0x45, 0x43, 0x00} // 5a Z
  ,{0x00, 0x7f, 0x41, 0x41, 0x00, 0x00} // 5b [
  ,{0x02, 0x04, 0x08, 0x10, 0x20, 0x00} // 5c blackslash
  ,{0x00, 0x41, 0x41, 0x7f, 0x00, 0x00} // 5d ]
  ,{0x04, 0x02, 0x01, 0x02, 0x04, 0x00} // 5e ^
  ,{0x40, 0x40, 0x40, 0x40, 0x40, 0x00} // 5f _
  ,{0x00, 0x01, 0x02, 0x04, 0x00, 0x00} // 60 `
  ,{0x20, 0x54, 0x54, 0x54, 0x78, 0x00} // 61 a
  ,{0x7f, 0x48, 0x44, 0x44, 0x38, 0x00} // 62 b
  ,{0x38, 0x44, 0x44, 0x44, 0x20, 0x00} // 63 c
  ,{0x38, 0x44, 0x44, 0x48, 0x7f, 0x00} // 64 d
  ,{0x38, 0x54, 0x54, 0x54, 0x18, 0x00} // 65 e
  ,{0x08, 0x7e, 0x09, 0x01, 0x02, 0x00} // 66 f
  ,{0x0c, 0x52, 0x52, 0x52, 0x3e, 0x00} // 67 g
  ,{0x7f, 0x08, 0x04, 0x04, 0x78, 0x00} // 68 h
  ,{0x00, 0x44, 0x7d, 0x40, 0x00, 0x00} // 69 i
  ,{0x20, 0x40, 0x44, 0x3d, 0x00, 0x00} // 6a j
  ,{0x7f, 0x10, 0x28, 0x44, 0x00, 0x00} // 6b k
  ,{0x00, 0x41, 0x7f, 0x40, 0x00, 0x00} // 6c l
  ,{0x7c, 0x04, 0x18, 0x04, 0x78, 0x00} // 6d m
  ,{0x7c, 0x08, 0x04, 0x04, 0x78, 0x00} // 6e n
  ,{0x38, 0x44, 0x44, 0x44, 0x38, 0x00} // 6f o
  ,{0x7c, 0x14, 0x14, 0x14, 0x08, 0x00} // 70 p
  ,{0x08, 0x14, 0x14, 0x18, 0x7c, 0x00} // 71 q
  ,{0x7c, 0x08, 0x04, 0x04, 0x08, 0x00} // 72 r
  ,{0x48, 0x54, 0x54, 0x54, 0x20, 0x00} // 73 s
  ,{0x04, 0x3f, 0x44, 0x40, 0x20, 0x00} // 74 t
  ,{0x3c, 0x40, 0x40, 0x20, 0x7c, 0x00} // 75 u
  ,{0x1c, 0x20, 0x40, 0x20, 0x1c, 0x00} // 76 v
  ,{0x3c, 0x40, 0x30, 0x40, 0x3c, 0x00} // 77 w
  ,{0x44, 0x28, 0x10, 0x28, 0x44, 0x00} // 78 x
  ,{0x0c, 0x50, 0x50, 0x50, 0x3c, 0x00} // 79 y
  ,{0x44, 0x64, 0x54, 0x4c, 0x44, 0x00} // 7a z
  ,{0x00, 0x08, 0x36, 0x41, 0x00, 0x00} // 7b {
  ,{0x00, 0x00, 0x7f, 0x00, 0x00, 0x00} // 7c |
  ,{0x00, 0x41, 0x36, 0x08, 0x00, 0x00} // 7d }
  ,{0x10, 0x08, 0x08, 0x10, 0x08, 0x00} // 7e ~
  ,{0x78, 0x46, 0x41, 0x46, 0x78, 0x00} // 7f DEL
};

//This is the original bar that is written on the bottom of the screen. May need
//to change this to just the bottom two rows being written out
static const uint8_t Meter[] = {

    0xF0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,//Row 5
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,//Row 5
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,//Row 5
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xF0,//Row 5
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//Row 5
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//Row 5

    0xFF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,//Row 6
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,//Row 6
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,//Row 6
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFF,//Row 6
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//Row 6
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//Row 6
};

//This is the array for the Bolt Logo that is the nail looking thing with a
//lightning bolt coming out of the bottom.
static const uint8_t BoltZapLogo[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0xE0, 0xF0, 0xF0, 0xF8, 0xF8,
    0xF8, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xF0,
    0xF0, 0xF0, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC,
    0xF8, 0xF8, 0xF8, 0xF0, 0xF0, 0xE0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//<<

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x81, 0xC1, 0xC1, 0xE1, 0xF1, 0xF1, 0xF9, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD,
    0xFD, 0xFD, 0xFD, 0x7D, 0x7D, 0x3D, 0x1D, 0x0D, 0x05, 0x05, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//<<

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x30, 0x38, 0x38, 0x3C, 0x3E,
    0x3E, 0x3F, 0x3F, 0x3F, 0xBF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x3F, 0x3F, 0x1F, 0x0F, 0x0F, 0x07, 0x07, 0x03,
    0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//<<

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x60, 0x60, 0x70, 0x70, 0x78, 0x7C,
    0x7C, 0x7E, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0x7E, 0x7E, 0x3E, 0x3E, 0x1E, 0x0E, 0x0E, 0x06,
    0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//<<

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0x78, 0x7C, 0x3E, 0x1F, 0x1F, 0x0F, 0x0F,
    0x07, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//<<

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x18, 0x08,
    0x0C, 0x06, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//<<
    
};

/*Blank arrays for clearing*/
//static uint8_t blankArray[] = {0x00};
/*This is a 504 (84*6) blank array for clearing the screen*/
static const uint8_t clearArray[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00};

#endif	/* LCDBITS_H */
