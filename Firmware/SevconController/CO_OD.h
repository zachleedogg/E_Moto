// clang-format off
/*******************************************************************************

   File - CO_OD.c/CO_OD.h
   CANopen Object Dictionary.

   This file was automatically generated with libedssharp Object
   Dictionary Editor v0.8-0-gb60f4eb   DON'T EDIT THIS FILE MANUALLY !!!!
*******************************************************************************/


#ifndef CO_OD_H_
#define CO_OD_H_

/*******************************************************************************
   CANopen DATA TYPES
*******************************************************************************/
   typedef bool_t       BOOLEAN;
   typedef uint8_t      UNSIGNED8;
   typedef uint16_t     UNSIGNED16;
   typedef uint32_t     UNSIGNED32;
   typedef uint64_t     UNSIGNED64;
   typedef int8_t       INTEGER8;
   typedef int16_t      INTEGER16;
   typedef int32_t      INTEGER32;
   typedef int64_t      INTEGER64;
   typedef float32_t    REAL32;
   typedef float64_t    REAL64;
   typedef char_t       VISIBLE_STRING;
   typedef oChar_t      OCTET_STRING;

   #ifdef DOMAIN
   #undef DOMAIN
   #endif

   typedef domain_t     DOMAIN;

#ifndef timeOfDay_t
    typedef union {
        unsigned long long ullValue;
        struct {
            unsigned long ms:28;
            unsigned reserved:4;
            unsigned days:16;
            unsigned reserved2:16;
        };
    }timeOfDay_t;
#endif

    typedef timeOfDay_t TIME_OF_DAY;
    typedef timeOfDay_t TIME_DIFFERENCE;


/*******************************************************************************
   FILE INFO:
      FileName:     
      FileVersion:  1
      CreationTime: 
      CreationDate: 
      CreatedBy:    
******************************************************************************/


/*******************************************************************************
   DEVICE INFO:
      VendorName:     
      VendorNumber    0
      ProductName:    New Product
      ProductNumber:  0
******************************************************************************/


/*******************************************************************************
   FEATURES
*******************************************************************************/
  #define CO_NO_SYNC                     0   //Associated objects: 1005-1007
  #define CO_NO_EMERGENCY                0   //Associated objects: 1014, 1015
  #define CO_NO_TS                       0   //Associated objects: 1012, 1013
  #define CO_NO_SDO_SERVER               0   //Associated objects: 1200-127F
  #define CO_NO_SDO_CLIENT               0   //Associated objects: 1280-12FF
  #define CO_NO_LSS_SERVER               0   //LSS Slave
  #define CO_NO_LSS_CLIENT               0   //LSS Master
  #define CO_NO_RPDO                     0   //Associated objects: 14xx, 16xx
  #define CO_NO_TPDO                     0   //Associated objects: 18xx, 1Axx
  #define CO_NO_NMT_MASTER               0


/*******************************************************************************
   OBJECT DICTIONARY
*******************************************************************************/
   #define CO_OD_NoOfElements             4


/*******************************************************************************
   TYPE DEFINITIONS FOR RECORDS
*******************************************************************************/

/*******************************************************************************
   TYPE DEFINITIONS FOR OBJECT DICTIONARY INDEXES

   some of those are redundant with CO_SDO.h CO_ObjDicId_t <Common CiA301 object 
   dictionary entries>
*******************************************************************************/
/*1f80 */
        #define OD_1f80_NMTStartup                                  0x1f80

/*1f81 */
        #define OD_1f81_slaveAssignment                             0x1f81

        #define OD_1f81_0_slaveAssignment_maxSubIndex               0
        #define OD_1f81_1_slaveAssignment_                          1
        #define OD_1f81_2_slaveAssignment_                          2
        #define OD_1f81_3_slaveAssignment_                          3
        #define OD_1f81_4_slaveAssignment_                          4
        #define OD_1f81_5_slaveAssignment_                          5
        #define OD_1f81_6_slaveAssignment_                          6
        #define OD_1f81_7_slaveAssignment_                          7
        #define OD_1f81_8_slaveAssignment_                          8
        #define OD_1f81_9_slaveAssignment_                          9
        #define OD_1f81_10_slaveAssignment_                         10
        #define OD_1f81_11_slaveAssignment_                         11
        #define OD_1f81_12_slaveAssignment_                         12
        #define OD_1f81_13_slaveAssignment_                         13
        #define OD_1f81_14_slaveAssignment_                         14
        #define OD_1f81_15_slaveAssignment_                         15
        #define OD_1f81_16_slaveAssignment_                         16
        #define OD_1f81_17_slaveAssignment_                         17
        #define OD_1f81_18_slaveAssignment_                         18
        #define OD_1f81_19_slaveAssignment_                         19
        #define OD_1f81_20_slaveAssignment_                         20
        #define OD_1f81_21_slaveAssignment_                         21
        #define OD_1f81_22_slaveAssignment_                         22
        #define OD_1f81_23_slaveAssignment_                         23
        #define OD_1f81_24_slaveAssignment_                         24
        #define OD_1f81_25_slaveAssignment_                         25
        #define OD_1f81_26_slaveAssignment_                         26
        #define OD_1f81_27_slaveAssignment_                         27
        #define OD_1f81_28_slaveAssignment_                         28
        #define OD_1f81_29_slaveAssignment_                         29
        #define OD_1f81_30_slaveAssignment_                         30
        #define OD_1f81_31_slaveAssignment_                         31
        #define OD_1f81_32_slaveAssignment_                         32
        #define OD_1f81_33_slaveAssignment_                         33
        #define OD_1f81_34_slaveAssignment_                         34
        #define OD_1f81_35_slaveAssignment_                         35
        #define OD_1f81_36_slaveAssignment_                         36
        #define OD_1f81_37_slaveAssignment_                         37
        #define OD_1f81_38_slaveAssignment_                         38
        #define OD_1f81_39_slaveAssignment_                         39
        #define OD_1f81_40_slaveAssignment_                         40
        #define OD_1f81_41_slaveAssignment_                         41
        #define OD_1f81_42_slaveAssignment_                         42
        #define OD_1f81_43_slaveAssignment_                         43
        #define OD_1f81_44_slaveAssignment_                         44
        #define OD_1f81_45_slaveAssignment_                         45
        #define OD_1f81_46_slaveAssignment_                         46
        #define OD_1f81_47_slaveAssignment_                         47
        #define OD_1f81_48_slaveAssignment_                         48
        #define OD_1f81_49_slaveAssignment_                         49
        #define OD_1f81_50_slaveAssignment_                         50
        #define OD_1f81_51_slaveAssignment_                         51
        #define OD_1f81_52_slaveAssignment_                         52
        #define OD_1f81_53_slaveAssignment_                         53
        #define OD_1f81_54_slaveAssignment_                         54
        #define OD_1f81_55_slaveAssignment_                         55
        #define OD_1f81_56_slaveAssignment_                         56
        #define OD_1f81_57_slaveAssignment_                         57
        #define OD_1f81_58_slaveAssignment_                         58
        #define OD_1f81_59_slaveAssignment_                         59
        #define OD_1f81_60_slaveAssignment_                         60
        #define OD_1f81_61_slaveAssignment_                         61
        #define OD_1f81_62_slaveAssignment_                         62
        #define OD_1f81_63_slaveAssignment_                         63
        #define OD_1f81_64_slaveAssignment_                         64
        #define OD_1f81_65_slaveAssignment_                         65
        #define OD_1f81_66_slaveAssignment_                         66
        #define OD_1f81_67_slaveAssignment_                         67
        #define OD_1f81_68_slaveAssignment_                         68
        #define OD_1f81_69_slaveAssignment_                         69
        #define OD_1f81_70_slaveAssignment_                         70
        #define OD_1f81_71_slaveAssignment_                         71
        #define OD_1f81_72_slaveAssignment_                         72
        #define OD_1f81_73_slaveAssignment_                         73
        #define OD_1f81_74_slaveAssignment_                         74
        #define OD_1f81_75_slaveAssignment_                         75
        #define OD_1f81_76_slaveAssignment_                         76
        #define OD_1f81_77_slaveAssignment_                         77
        #define OD_1f81_78_slaveAssignment_                         78
        #define OD_1f81_79_slaveAssignment_                         79
        #define OD_1f81_80_slaveAssignment_                         80
        #define OD_1f81_81_slaveAssignment_                         81
        #define OD_1f81_82_slaveAssignment_                         82
        #define OD_1f81_83_slaveAssignment_                         83
        #define OD_1f81_84_slaveAssignment_                         84
        #define OD_1f81_85_slaveAssignment_                         85
        #define OD_1f81_86_slaveAssignment_                         86
        #define OD_1f81_87_slaveAssignment_                         87
        #define OD_1f81_88_slaveAssignment_                         88
        #define OD_1f81_89_slaveAssignment_                         89
        #define OD_1f81_90_slaveAssignment_                         90
        #define OD_1f81_91_slaveAssignment_                         91
        #define OD_1f81_92_slaveAssignment_                         92
        #define OD_1f81_93_slaveAssignment_                         93
        #define OD_1f81_94_slaveAssignment_                         94
        #define OD_1f81_95_slaveAssignment_                         95
        #define OD_1f81_96_slaveAssignment_                         96
        #define OD_1f81_97_slaveAssignment_                         97
        #define OD_1f81_98_slaveAssignment_                         98
        #define OD_1f81_99_slaveAssignment_                         99
        #define OD_1f81_100_slaveAssignment_                        100
        #define OD_1f81_101_slaveAssignment_                        101
        #define OD_1f81_102_slaveAssignment_                        102
        #define OD_1f81_103_slaveAssignment_                        103
        #define OD_1f81_104_slaveAssignment_                        104
        #define OD_1f81_105_slaveAssignment_                        105
        #define OD_1f81_106_slaveAssignment_                        106
        #define OD_1f81_107_slaveAssignment_                        107
        #define OD_1f81_108_slaveAssignment_                        108
        #define OD_1f81_109_slaveAssignment_                        109
        #define OD_1f81_110_slaveAssignment_                        110
        #define OD_1f81_111_slaveAssignment_                        111
        #define OD_1f81_112_slaveAssignment_                        112
        #define OD_1f81_113_slaveAssignment_                        113
        #define OD_1f81_114_slaveAssignment_                        114
        #define OD_1f81_115_slaveAssignment_                        115
        #define OD_1f81_116_slaveAssignment_                        116
        #define OD_1f81_117_slaveAssignment_                        117
        #define OD_1f81_118_slaveAssignment_                        118
        #define OD_1f81_119_slaveAssignment_                        119
        #define OD_1f81_120_slaveAssignment_                        120
        #define OD_1f81_121_slaveAssignment_                        121
        #define OD_1f81_122_slaveAssignment_                        122
        #define OD_1f81_123_slaveAssignment_                        123
        #define OD_1f81_124_slaveAssignment_                        124
        #define OD_1f81_125_slaveAssignment_                        125
        #define OD_1f81_126_slaveAssignment_                        126
        #define OD_1f81_127_slaveAssignment_                        127

/*1f82 */
        #define OD_1f82_requestNMT                                  0x1f82

        #define OD_1f82_0_requestNMT_maxSubIndex                    0
        #define OD_1f82_1_requestNMT_                               1
        #define OD_1f82_2_requestNMT_                               2
        #define OD_1f82_3_requestNMT_                               3
        #define OD_1f82_4_requestNMT_                               4
        #define OD_1f82_5_requestNMT_                               5
        #define OD_1f82_6_requestNMT_                               6
        #define OD_1f82_7_requestNMT_                               7
        #define OD_1f82_8_requestNMT_                               8
        #define OD_1f82_9_requestNMT_                               9
        #define OD_1f82_10_requestNMT_                              10
        #define OD_1f82_11_requestNMT_                              11
        #define OD_1f82_12_requestNMT_                              12
        #define OD_1f82_13_requestNMT_                              13
        #define OD_1f82_14_requestNMT_                              14
        #define OD_1f82_15_requestNMT_                              15
        #define OD_1f82_16_requestNMT_                              16
        #define OD_1f82_17_requestNMT_                              17
        #define OD_1f82_18_requestNMT_                              18
        #define OD_1f82_19_requestNMT_                              19
        #define OD_1f82_20_requestNMT_                              20
        #define OD_1f82_21_requestNMT_                              21
        #define OD_1f82_22_requestNMT_                              22
        #define OD_1f82_23_requestNMT_                              23
        #define OD_1f82_24_requestNMT_                              24
        #define OD_1f82_25_requestNMT_                              25
        #define OD_1f82_26_requestNMT_                              26
        #define OD_1f82_27_requestNMT_                              27
        #define OD_1f82_28_requestNMT_                              28
        #define OD_1f82_29_requestNMT_                              29
        #define OD_1f82_30_requestNMT_                              30
        #define OD_1f82_31_requestNMT_                              31
        #define OD_1f82_32_requestNMT_                              32
        #define OD_1f82_33_requestNMT_                              33
        #define OD_1f82_34_requestNMT_                              34
        #define OD_1f82_35_requestNMT_                              35
        #define OD_1f82_36_requestNMT_                              36
        #define OD_1f82_37_requestNMT_                              37
        #define OD_1f82_38_requestNMT_                              38
        #define OD_1f82_39_requestNMT_                              39
        #define OD_1f82_40_requestNMT_                              40
        #define OD_1f82_41_requestNMT_                              41
        #define OD_1f82_42_requestNMT_                              42
        #define OD_1f82_43_requestNMT_                              43
        #define OD_1f82_44_requestNMT_                              44
        #define OD_1f82_45_requestNMT_                              45
        #define OD_1f82_46_requestNMT_                              46
        #define OD_1f82_47_requestNMT_                              47
        #define OD_1f82_48_requestNMT_                              48
        #define OD_1f82_49_requestNMT_                              49
        #define OD_1f82_50_requestNMT_                              50
        #define OD_1f82_51_requestNMT_                              51
        #define OD_1f82_52_requestNMT_                              52
        #define OD_1f82_53_requestNMT_                              53
        #define OD_1f82_54_requestNMT_                              54
        #define OD_1f82_55_requestNMT_                              55
        #define OD_1f82_56_requestNMT_                              56
        #define OD_1f82_57_requestNMT_                              57
        #define OD_1f82_58_requestNMT_                              58
        #define OD_1f82_59_requestNMT_                              59
        #define OD_1f82_60_requestNMT_                              60
        #define OD_1f82_61_requestNMT_                              61
        #define OD_1f82_62_requestNMT_                              62
        #define OD_1f82_63_requestNMT_                              63
        #define OD_1f82_64_requestNMT_                              64
        #define OD_1f82_65_requestNMT_                              65
        #define OD_1f82_66_requestNMT_                              66
        #define OD_1f82_67_requestNMT_                              67
        #define OD_1f82_68_requestNMT_                              68
        #define OD_1f82_69_requestNMT_                              69
        #define OD_1f82_70_requestNMT_                              70
        #define OD_1f82_71_requestNMT_                              71
        #define OD_1f82_72_requestNMT_                              72
        #define OD_1f82_73_requestNMT_                              73
        #define OD_1f82_74_requestNMT_                              74
        #define OD_1f82_75_requestNMT_                              75
        #define OD_1f82_76_requestNMT_                              76
        #define OD_1f82_77_requestNMT_                              77
        #define OD_1f82_78_requestNMT_                              78
        #define OD_1f82_79_requestNMT_                              79
        #define OD_1f82_80_requestNMT_                              80
        #define OD_1f82_81_requestNMT_                              81
        #define OD_1f82_82_requestNMT_                              82
        #define OD_1f82_83_requestNMT_                              83
        #define OD_1f82_84_requestNMT_                              84
        #define OD_1f82_85_requestNMT_                              85
        #define OD_1f82_86_requestNMT_                              86
        #define OD_1f82_87_requestNMT_                              87
        #define OD_1f82_88_requestNMT_                              88
        #define OD_1f82_89_requestNMT_                              89
        #define OD_1f82_90_requestNMT_                              90
        #define OD_1f82_91_requestNMT_                              91
        #define OD_1f82_92_requestNMT_                              92
        #define OD_1f82_93_requestNMT_                              93
        #define OD_1f82_94_requestNMT_                              94
        #define OD_1f82_95_requestNMT_                              95
        #define OD_1f82_96_requestNMT_                              96
        #define OD_1f82_97_requestNMT_                              97
        #define OD_1f82_98_requestNMT_                              98
        #define OD_1f82_99_requestNMT_                              99
        #define OD_1f82_100_requestNMT_                             100
        #define OD_1f82_101_requestNMT_                             101
        #define OD_1f82_102_requestNMT_                             102
        #define OD_1f82_103_requestNMT_                             103
        #define OD_1f82_104_requestNMT_                             104
        #define OD_1f82_105_requestNMT_                             105
        #define OD_1f82_106_requestNMT_                             106
        #define OD_1f82_107_requestNMT_                             107
        #define OD_1f82_108_requestNMT_                             108
        #define OD_1f82_109_requestNMT_                             109
        #define OD_1f82_110_requestNMT_                             110
        #define OD_1f82_111_requestNMT_                             111
        #define OD_1f82_112_requestNMT_                             112
        #define OD_1f82_113_requestNMT_                             113
        #define OD_1f82_114_requestNMT_                             114
        #define OD_1f82_115_requestNMT_                             115
        #define OD_1f82_116_requestNMT_                             116
        #define OD_1f82_117_requestNMT_                             117
        #define OD_1f82_118_requestNMT_                             118
        #define OD_1f82_119_requestNMT_                             119
        #define OD_1f82_120_requestNMT_                             120
        #define OD_1f82_121_requestNMT_                             121
        #define OD_1f82_122_requestNMT_                             122
        #define OD_1f82_123_requestNMT_                             123
        #define OD_1f82_124_requestNMT_                             124
        #define OD_1f82_125_requestNMT_                             125
        #define OD_1f82_126_requestNMT_                             126
        #define OD_1f82_127_requestNMT_                             127

/*1f89 */
        #define OD_1f89_bootTime                                    0x1f89

/*******************************************************************************
   STRUCTURES FOR VARIABLES IN DIFFERENT MEMORY LOCATIONS
*******************************************************************************/
#define  CO_OD_FIRST_LAST_WORD     0x55 //Any value from 0x01 to 0xFE. If changed, EEPROM will be reinitialized.

/***** Structure for ROM variables ********************************************/
struct sCO_OD_ROM{
               UNSIGNED32     FirstWord;


               UNSIGNED32     LastWord;
};

/***** Structure for RAM variables ********************************************/
struct sCO_OD_RAM{
               UNSIGNED32     FirstWord;

/*1f80      */ UNSIGNED32      NMTStartup;
/*1f81      */ UNSIGNED32      slaveAssignment[127];
/*1f82      */ UNSIGNED8       requestNMT[127];
/*1f89      */ UNSIGNED32      bootTime;

               UNSIGNED32     LastWord;
};

/***** Structure for EEPROM variables ********************************************/
struct sCO_OD_EEPROM{
               UNSIGNED32     FirstWord;


               UNSIGNED32     LastWord;
};

/***** Declaration of Object Dictionary variables *****************************/
extern struct sCO_OD_ROM CO_OD_ROM;

extern struct sCO_OD_RAM CO_OD_RAM;

extern struct sCO_OD_EEPROM CO_OD_EEPROM;

/*******************************************************************************
   ALIASES FOR OBJECT DICTIONARY VARIABLES
*******************************************************************************/
/*1f80, Data Type: UNSIGNED32 */
        #define OD_NMTStartup                                       CO_OD_RAM.NMTStartup

/*1f81, Data Type: UNSIGNED32, Array[127] */
        #define OD_slaveAssignment                                  CO_OD_RAM.slaveAssignment
        #define ODL_slaveAssignment_arrayLength                     127
        #define ODA_slaveAssignment_                                0

/*1f82, Data Type: UNSIGNED8, Array[127] */
        #define OD_requestNMT                                       CO_OD_RAM.requestNMT
        #define ODL_requestNMT_arrayLength                          127
        #define ODA_requestNMT_                                     0

/*1f89, Data Type: UNSIGNED32 */
        #define OD_bootTime                                         CO_OD_RAM.bootTime

#endif
// clang-format on
