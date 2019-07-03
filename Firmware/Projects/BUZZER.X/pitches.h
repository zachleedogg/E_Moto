/* 
 * File:   pitches.h
 * Author: kid group
 *
 * Created on November 27, 2018, 4:24 PM
 */

#ifndef PITCHES_H
#define	PITCHES_H

#define PITCH_SCALAR



#define	NOTE_B0	64714
#define	NOTE_C1	61087
#define	NOTE_CS1	57655
#define	NOTE_D1	54418
#define	NOTE_DS1	51363
#define	NOTE_E1	48477
#define	NOTE_F1	45760
#define	NOTE_FS1	43186
#define	NOTE_G1	40768
#define	NOTE_GS1	38480
#define	NOTE_A1	36322
#define	NOTE_AS1	34281
#define	NOTE_B1	32357
#define	NOTE_C2	30537
#define	NOTE_CS2	28821
#define	NOTE_D2	27209
#define	NOTE_DS2	25688
#define	NOTE_E2	24245
#define	NOTE_F2	22880
#define	NOTE_FS2	21593
#define	NOTE_G2	20384
#define	NOTE_GS2	19240
#define	NOTE_A2	18161
#define	NOTE_AS2	17147
#define	NOTE_B2	16185
#define	NOTE_C3	15275
#define	NOTE_CS3	14417
#define	NOTE_D3	13611
#define	NOTE_DS3	12844
#define	NOTE_E3	12116
#define	NOTE_F3	11440
#define	NOTE_FS3	10803
#define	NOTE_G3	10192
#define	NOTE_GS3	9620
#define	NOTE_A3	9074
#define	NOTE_AS3	8567
#define	NOTE_B3	8086
#define	NOTE_C4	7631
#define	NOTE_CS4	7202
#define	NOTE_D4	6799
#define	NOTE_DS4	6422
#define	NOTE_E4	6058
#define	NOTE_F4	5720
#define	NOTE_FS4	5395
#define	NOTE_G4	5096
#define	NOTE_GS4	4810
#define	NOTE_A4	4537
#define	NOTE_AS4	4290
#define	NOTE_B4	4043
#define	NOTE_C5	3822
#define	NOTE_CS5	3601
#define	NOTE_D5	3406
#define	NOTE_DS5	3211
#define	NOTE_E5	3029
#define	NOTE_F5	2860
#define	NOTE_FS5	2704
#define	NOTE_G5	2548
#define	NOTE_GS5	2405
#define	NOTE_A5	2275
#define	NOTE_AS5	2145
#define	NOTE_B5	2028
#define	NOTE_C6	1911
#define	NOTE_CS6	1807
#define	NOTE_D6	1703
#define	NOTE_DS6	1599
#define	NOTE_E6	1521
#define	NOTE_F6	1430
#define	NOTE_FS6	1352
#define	NOTE_G6	1274
#define	NOTE_GS6	1209
#define	NOTE_A6	1131
#define	NOTE_AS6	1066
#define	NOTE_B6	1014
#define	NOTE_C7	949
#define	NOTE_CS7	897
#define	NOTE_D7	845
#define	NOTE_DS7	806
#define	NOTE_E7	754
#define	NOTE_F7	715
#define	NOTE_FS7	676
#define	NOTE_G7	637
#define	NOTE_GS7	598
#define	NOTE_A7	572
#define	NOTE_AS7	533
#define	NOTE_B7	507
#define	NOTE_C8	481
#define	NOTE_CS8	455
#define	NOTE_D8	429
#define	NOTE_DS8	403

#define	END	-1


uint32_t scales[] = {
    403,
    429,
    455,
    481,
    507,
    533,
    572,
    598,
    637,
    676,
    715,
    754,
    806,
    845,
    897,
    949,
    1014,
    1066,
    1131,
    1209,
    1274,
    1352,
    1430,
    1521,
    1599,
    1703,
    1807,
    1911,
    2028,
    2145,
    2275,
    2405,
    2548,
    2704,
    2860,
    3029,
    3211,
    3406,
    3601,
    3822,
    4043,
    4290,
    4537,
    4810,
    5096,
    5395,
    5720,
    6058,
    6422,
    6799,
    7202,
    7631,
    8086,
    8567,
    9074,
    9620,
    10192,
    10803,
    11440,
    12116,
    12844,
    13611,
    14417,
    15275,
    16185,
    17147,
    18161,
    19240,
    20384,
    21593,
    22880,
    24245,
    25688,
    27209,
    28821,
    30537,
    32357,
    34281,
    36322,
    38480,
    40768,
    43186,
    45760,
    48477,
    51363,
    54418,
    57655,
    61087,
    64714,


};

int MarioMelody[] = {
    NOTE_E2, NOTE_E2, 0, NOTE_E2,
    0, NOTE_C2, NOTE_E2, 0,
    NOTE_G2, 0, 0, 0,
    NOTE_G1, 0, 0, 0,

    NOTE_C2, 0, 0, NOTE_G1,
    0, 0, NOTE_E1, 0,
    0, NOTE_A1, 0, NOTE_B1,
    0, NOTE_AS1, NOTE_A1, 0,

    NOTE_G1, NOTE_E2, NOTE_G2,
    NOTE_A2, 0, NOTE_F2, NOTE_G2,
    0, NOTE_E2, 0, NOTE_C2,
    NOTE_D2, NOTE_B1, 0, 0,

    NOTE_C2, 0, 0, NOTE_G1,
    0, 0, NOTE_E1, 0,
    0, NOTE_A1, 0, NOTE_B1,
    0, NOTE_AS1, NOTE_A1, 0,

    NOTE_G1, NOTE_E2, NOTE_G2,
    NOTE_A2, 0, NOTE_F2, NOTE_G2,
    0, NOTE_E2, 0, NOTE_C2,
    NOTE_D2, NOTE_B1, 0, 0
};
//Mario main them tempo
int MarioTempo[] = {
    12, 12, 12, 12,
    12, 12, 12, 12,
    12, 12, 12, 12,
    12, 12, 12, 12,

    12, 12, 12, 12,
    12, 12, 12, 12,
    12, 12, 12, 12,
    12, 12, 12, 12,

    9, 9, 9,
    12, 12, 12, 12,
    12, 12, 12, 12,
    12, 12, 12, 12,

    12, 12, 12, 12,
    12, 12, 12, 12,
    12, 12, 12, 12,
    12, 12, 12, 12,

    9, 9, 9,
    12, 12, 12, 12,
    12, 12, 12, 12,
    12, 12, 12, 12,
};

int MarioLenth = sizeof (MarioTempo) / sizeof (MarioTempo[0]);

// Parts 1 and 2 (Intro)

int RickMelody[] ={
    NOTE_CS5, NOTE_DS5, NOTE_DS5, NOTE_F5, NOTE_GS4, NOTE_FS5, NOTE_F5, NOTE_DS5, NOTE_CS5, NOTE_DS5, 0, NOTE_GS3, NOTE_GS3,
    
    
    NOTE_A3, NOTE_A3, NOTE_GS3, NOTE_GS3,
    NOTE_F5, NOTE_F5, NOTE_DS5, NOTE_A3, NOTE_A3, NOTE_GS3, NOTE_GS3, NOTE_DS5, NOTE_DS5, NOTE_CS5, NOTE_C5, NOTE_A3,
    NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5,
    NOTE_CS5, NOTE_DS5, NOTE_C5, NOTE_A3, NOTE_GS3, NOTE_GS3, NOTE_GS3, NOTE_DS5, NOTE_CS5,
    NOTE_A3, NOTE_A3, NOTE_GS3, NOTE_GS3,
    NOTE_F5, NOTE_F5, NOTE_DS5, NOTE_A3, NOTE_A3, NOTE_GS3, NOTE_GS3, NOTE_GS4, NOTE_C5, NOTE_CS5, NOTE_C5, NOTE_A3,
    NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5,
    NOTE_CS5, NOTE_DS5, NOTE_C5, NOTE_A3, NOTE_GS3, 0, NOTE_GS3, NOTE_DS5, NOTE_CS5, 0,
    
    0, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_DS4, 0, NOTE_C4, NOTE_A3, NOTE_GS2,
    0, NOTE_A3, NOTE_A3, NOTE_C4, NOTE_CS4, NOTE_GS2, NOTE_GS3, NOTE_GS3, NOTE_DS4,
    0, NOTE_A3, NOTE_A3, NOTE_C4, NOTE_CS4, NOTE_A3, NOTE_CS4, NOTE_DS4, 0, NOTE_C4, NOTE_A3, NOTE_A3, NOTE_GS2,
    0, NOTE_A3, NOTE_A3, NOTE_C4, NOTE_CS4, NOTE_GS2, NOTE_GS2, NOTE_DS4, NOTE_DS4, NOTE_DS4, NOTE_F4, NOTE_DS4,
    NOTE_CS4, NOTE_DS4, NOTE_F4, NOTE_CS4, NOTE_DS4, NOTE_DS4, NOTE_DS4, NOTE_F4, NOTE_DS4, NOTE_GS2,
    0, NOTE_A3, NOTE_C4, NOTE_CS4, NOTE_GS2, 0, NOTE_DS4, NOTE_F4, NOTE_DS4
};

int RickTempo[] ={
    6, 10, 6, 6, 1, 1, 1, 1, 6, 10, 4, 2, 10,

    1, 1, 1, 1,
    3, 3, 6, 1, 1, 1, 1, 3, 3, 3, 1, 2,
    1, 1, 1, 1,
    3, 3, 3, 1, 2, 2, 2, 4, 8,
    1, 1, 1, 1,
    3, 3, 6, 1, 1, 1, 1, 3, 3, 3, 1, 2,
    1, 1, 1, 1,
    3, 3, 3, 1, 2, 2, 2, 4, 8, 4,
    
    2, 1, 1, 1, 1, 2, 1, 1, 1, 5,
    1, 1, 1, 1, 3, 1, 2, 1, 5,
    1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 3,
    1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 4,
    5, 1, 1, 1, 1, 1, 1, 1, 2, 2,
    2, 1, 1, 1, 3, 1, 1, 1, 3
    
    
};


int RickLenth = sizeof (RickTempo) / sizeof (RickTempo[0]);
// Parts 4 or 6 (Chorus)

int song1_chorus_melody[] ={NOTE_A3, NOTE_A3, NOTE_GS3, NOTE_GS3,
    NOTE_F5, NOTE_F5, NOTE_DS5, NOTE_A3, NOTE_A3, NOTE_GS3, NOTE_GS3, NOTE_DS5, NOTE_DS5, NOTE_CS5, NOTE_C5, NOTE_A3,
    NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5,
    NOTE_CS5, NOTE_DS5, NOTE_C5, NOTE_A3, NOTE_GS3, NOTE_GS3, NOTE_GS3, NOTE_DS5, NOTE_CS5,
    NOTE_A3, NOTE_A3, NOTE_GS3, NOTE_GS3,
    NOTE_F5, NOTE_F5, NOTE_DS5, NOTE_A3, NOTE_A3, NOTE_GS3, NOTE_GS3, NOTE_GS4, NOTE_C5, NOTE_CS5, NOTE_C5, NOTE_A3,
    NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5,
    NOTE_CS5, NOTE_DS5, NOTE_C5, NOTE_A3, NOTE_GS3, 0, NOTE_GS3, NOTE_DS5, NOTE_CS5, 0};

int song1_chorus_rhythmn[] ={1, 1, 1, 1,
    3, 3, 6, 1, 1, 1, 1, 3, 3, 3, 1, 2,
    1, 1, 1, 1,
    3, 3, 3, 1, 2, 2, 2, 4, 8,
    1, 1, 1, 1,
    3, 3, 6, 1, 1, 1, 1, 3, 3, 3, 1, 2,
    1, 1, 1, 1,
    3, 3, 3, 1, 2, 2, 2, 4, 8, 4};

// Parts 3 or 5 (Verse 1)

int song1_verse1_melody[] ={0, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_DS4, 0, NOTE_C4, NOTE_A3, NOTE_GS2,
    0, NOTE_A3, NOTE_A3, NOTE_C4, NOTE_CS4, NOTE_GS2, NOTE_GS3, NOTE_GS3, NOTE_DS4,
    0, NOTE_A3, NOTE_A3, NOTE_C4, NOTE_CS4, NOTE_A3, NOTE_CS4, NOTE_DS4, 0, NOTE_C4, NOTE_A3, NOTE_A3, NOTE_GS2,
    0, NOTE_A3, NOTE_A3, NOTE_C4, NOTE_CS4, NOTE_GS2, NOTE_GS2, NOTE_DS4, NOTE_DS4, NOTE_DS4, NOTE_F4, NOTE_DS4,
    NOTE_CS4, NOTE_DS4, NOTE_F4, NOTE_CS4, NOTE_DS4, NOTE_DS4, NOTE_DS4, NOTE_F4, NOTE_DS4, NOTE_GS2,
    0, NOTE_A3, NOTE_C4, NOTE_CS4, NOTE_GS2, 0, NOTE_DS4, NOTE_F4, NOTE_DS4};

int song1_verse1_rhythmn[] ={2, 1, 1, 1, 1, 2, 1, 1, 1, 5,
    1, 1, 1, 1, 3, 1, 2, 1, 5,
    1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 3,
    1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 4,
    5, 1, 1, 1, 1, 1, 1, 1, 2, 2,
    2, 1, 1, 1, 3, 1, 1, 1, 3};


int JingleMelody[] = {NOTE_E2,NOTE_E2,NOTE_E2,NOTE_E2,NOTE_E2,NOTE_E2,NOTE_E2,NOTE_G2,NOTE_C2,NOTE_D2,NOTE_E2,
NOTE_F2,NOTE_F2,NOTE_F2,NOTE_F2,NOTE_F2,NOTE_E2,NOTE_E2,NOTE_E2,NOTE_E2,NOTE_D2,NOTE_D2,NOTE_E2,NOTE_D2,NOTE_G2};
int JingleTempo[] = {1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2};


int JingleLenth = sizeof (JingleTempo) / sizeof (JingleTempo[0]);

#endif	/* PITCHES_H */

