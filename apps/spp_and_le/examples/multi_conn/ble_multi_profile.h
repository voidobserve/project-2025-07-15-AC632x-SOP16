// binary representation
// attribute size in bytes (16), flags(16), handle (16), uuid (16/128), value(...)

#ifndef _BLE_MUTIL_PROFILE_H
#define _BLE_MUTIL_PROFILE_H

#include <stdint.h>
#include "app_config.h"

//----------------------------------------------------------------------------------------------------------
//
// gatt profile include file, generated by jieli gatt_inc_generator.exe
//

/*
生成profile工具的链接下载
https://gitee.com/Jieli-Tech/fw-AC63_BT_SDK/tree/master/sdk_tools/BLE%20Profile%E5%B7%A5%E5%85%B7
profile cfg 文件参考工具包里面目录trans_data_cfg
 */
static const uint8_t multi_profile_data[] = {

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  中道iLamp APP通信协议  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    //////////////////////////////////////////////////////
    //
    // 0x0001 PRIMARY_SERVICE  1800
    //
    //////////////////////////////////////////////////////
    0x0a, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x28, 0x00, 0x18,

     /* CHARACTERISTIC,  2a00, READ | WRITE | DYNAMIC, */
    // 0x0002 CHARACTERISTIC 2a00 READ | WRITE | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x02, 0x00, 0x03, 0x28, 0x0a, 0x03, 0x00, 0x00, 0x2a,
    // 0x0003 VALUE 2a00 READ | WRITE | DYNAMIC
    0x08, 0x00, 0x0a, 0x01, 0x03, 0x00, 0x00, 0x2a,

    //////////////////////////////////////////////////////
    //
    // 0x0004 PRIMARY_SERVICE  fff0
    //
    //////////////////////////////////////////////////////
    0x0a, 0x00, 0x02, 0x00, 0x04, 0x00, 0x00, 0x28, 0xf0, 0xff,

     /* CHARACTERISTIC,  fff1, READ | WRITE | NOTIFY| DYNAMIC, */
    // 0x0005 CHARACTERISTIC fff1 READ | WRITE | NOTIFY| DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x05, 0x00, 0x03, 0x28, 0x1a, 0x06, 0x00, 0xf1, 0xff,
    // 0x0006 VALUE fff1 READ | WRITE | NOTIFY| DYNAMIC
    0x08, 0x00, 0x1a, 0x01, 0x06, 0x00, 0xf1, 0xff,
    // 0x0007 CLIENT_CHARACTERISTIC_CONFIGURATION
    0x0a, 0x00, 0x0a, 0x01, 0x07, 0x00, 0x02, 0x29, 0x00, 0x00,




    #if 0
    //////////////////////////////////////////////////////
    //
    // 0x0001 PRIMARY_SERVICE  1800
    //
    //////////////////////////////////////////////////////
    0x0a, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x28, 0x00, 0x18,

    /* CHARACTERISTIC,  2a00, READ | WRITE | DYNAMIC, */
    // 0x0002 CHARACTERISTIC 2a00 READ | WRITE | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x02, 0x00, 0x03, 0x28, 0x0a, 0x03, 0x00, 0x00, 0x2a,
    // 0x0003 VALUE 2a00 READ | WRITE | DYNAMIC
    0x08, 0x00, 0x0a, 0x01, 0x03, 0x00, 0x00, 0x2a,

    //////////////////////////////////////////////////////
    //
    // 0x0004 PRIMARY_SERVICE  ae30
    //
    //////////////////////////////////////////////////////
    0x0a, 0x00, 0x02, 0x00, 0x04, 0x00, 0x00, 0x28, 0x30, 0xae,

    /* CHARACTERISTIC,  ae01, WRITE_WITHOUT_RESPONSE | DYNAMIC, */
    // 0x0005 CHARACTERISTIC ae01 WRITE_WITHOUT_RESPONSE | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x05, 0x00, 0x03, 0x28, 0x04, 0x06, 0x00, 0x01, 0xae,
    // 0x0006 VALUE ae01 WRITE_WITHOUT_RESPONSE | DYNAMIC
    0x08, 0x00, 0x04, 0x01, 0x06, 0x00, 0x01, 0xae,

    /* CHARACTERISTIC,  ae02, NOTIFY, */
    // 0x0007 CHARACTERISTIC ae02 NOTIFY
    0x0d, 0x00, 0x02, 0x00, 0x07, 0x00, 0x03, 0x28, 0x10, 0x08, 0x00, 0x02, 0xae,
    // 0x0008 VALUE ae02 NOTIFY
    0x08, 0x00, 0x10, 0x00, 0x08, 0x00, 0x02, 0xae,
    // 0x0009 CLIENT_CHARACTERISTIC_CONFIGURATION
    0x0a, 0x00, 0x0a, 0x01, 0x09, 0x00, 0x02, 0x29, 0x00, 0x00,

    /* CHARACTERISTIC,  ae03, WRITE_WITHOUT_RESPONSE | DYNAMIC, */
    // 0x000a CHARACTERISTIC ae03 WRITE_WITHOUT_RESPONSE | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x0a, 0x00, 0x03, 0x28, 0x04, 0x0b, 0x00, 0x03, 0xae,
    // 0x000b VALUE ae03 WRITE_WITHOUT_RESPONSE | DYNAMIC
    0x08, 0x00, 0x04, 0x01, 0x0b, 0x00, 0x03, 0xae,

    /* CHARACTERISTIC,  ae04, NOTIFY, */
    // 0x000c CHARACTERISTIC ae04 NOTIFY
    0x0d, 0x00, 0x02, 0x00, 0x0c, 0x00, 0x03, 0x28, 0x10, 0x0d, 0x00, 0x04, 0xae,
    // 0x000d VALUE ae04 NOTIFY
    0x08, 0x00, 0x10, 0x00, 0x0d, 0x00, 0x04, 0xae,
    // 0x000e CLIENT_CHARACTERISTIC_CONFIGURATION
    0x0a, 0x00, 0x0a, 0x01, 0x0e, 0x00, 0x02, 0x29, 0x00, 0x00,

    /* CHARACTERISTIC,  ae05, INDICATE, */
    // 0x000f CHARACTERISTIC ae05 INDICATE
    0x0d, 0x00, 0x02, 0x00, 0x0f, 0x00, 0x03, 0x28, 0x20, 0x10, 0x00, 0x05, 0xae,
    // 0x0010 VALUE ae05 INDICATE
    0x08, 0x00, 0x20, 0x00, 0x10, 0x00, 0x05, 0xae,
    // 0x0011 CLIENT_CHARACTERISTIC_CONFIGURATION
    0x0a, 0x00, 0x0a, 0x01, 0x11, 0x00, 0x02, 0x29, 0x00, 0x00,

    /* CHARACTERISTIC,  ae10, READ | WRITE | DYNAMIC, */
    // 0x0012 CHARACTERISTIC ae10 READ | WRITE | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x12, 0x00, 0x03, 0x28, 0x0a, 0x13, 0x00, 0x10, 0xae,
    // 0x0013 VALUE ae10 READ | WRITE | DYNAMIC
    0x08, 0x00, 0x0a, 0x01, 0x13, 0x00, 0x10, 0xae,

#endif 
#if 0
    //for uuid128,sample
    //	PRIMARY_SERVICE, 0000F530-1212-EFDE-1523-785FEABCD123
    //	CHARACTERISTIC,  0000F531-1212-EFDE-1523-785FEABCD123, NOTIFY,
    //	CHARACTERISTIC,  0000F532-1212-EFDE-1523-785FEABCD123, WRITE_WITHOUT_RESPONSE | DYNAMIC,

    //////////////////////////////////////////////////////
    //
    // 0x0014 PRIMARY_SERVICE  0000F530-1212-EFDE-1523-785FEABCD123
    //
    //////////////////////////////////////////////////////
    0x18, 0x00, 0x02, 0x00, 0x14, 0x00, 0x00, 0x28, 0x23, 0xd1, 0xbc, 0xea, 0x5f, 0x78, 0x23, 0x15, 0xde, 0xef, 0x12, 0x12, 0x30, 0xf5, 0x00, 0x00,

    /* CHARACTERISTIC,  0000F531-1212-EFDE-1523-785FEABCD123, NOTIFY, */
    // 0x0015 CHARACTERISTIC 0000F531-1212-EFDE-1523-785FEABCD123 NOTIFY
    0x1b, 0x00, 0x02, 0x00, 0x15, 0x00, 0x03, 0x28, 0x10, 0x16, 0x00, 0x23, 0xd1, 0xbc, 0xea, 0x5f, 0x78, 0x23, 0x15, 0xde, 0xef, 0x12, 0x12, 0x31, 0xf5, 0x00, 0x00,
    // 0x0016 VALUE 0000F531-1212-EFDE-1523-785FEABCD123 NOTIFY
    0x16, 0x00, 0x10, 0x02, 0x16, 0x00, 0x23, 0xd1, 0xbc, 0xea, 0x5f, 0x78, 0x23, 0x15, 0xde, 0xef, 0x12, 0x12, 0x31, 0xf5, 0x00, 0x00,
    // 0x0017 CLIENT_CHARACTERISTIC_CONFIGURATION
    0x0a, 0x00, 0x0a, 0x01, 0x17, 0x00, 0x02, 0x29, 0x00, 0x00,

    /* CHARACTERISTIC,  0000F532-1212-EFDE-1523-785FEABCD123, WRITE_WITHOUT_RESPONSE | DYNAMIC, */
    // 0x0018 CHARACTERISTIC 0000F532-1212-EFDE-1523-785FEABCD123 WRITE_WITHOUT_RESPONSE | DYNAMIC
    0x1b, 0x00, 0x02, 0x00, 0x18, 0x00, 0x03, 0x28, 0x04, 0x19, 0x00, 0x23, 0xd1, 0xbc, 0xea, 0x5f, 0x78, 0x23, 0x15, 0xde, 0xef, 0x12, 0x12, 0x32, 0xf5, 0x00, 0x00,
    // 0x0019 VALUE 0000F532-1212-EFDE-1523-785FEABCD123 WRITE_WITHOUT_RESPONSE | DYNAMIC
    0x16, 0x00, 0x04, 0x03, 0x19, 0x00, 0x23, 0xd1, 0xbc, 0xea, 0x5f, 0x78, 0x23, 0x15, 0xde, 0xef, 0x12, 0x12, 0x32, 0xf5, 0x00, 0x00,
#endif

    //////////////////////////////////////////////////////
    //
    // 0x0014 PRIMARY_SERVICE  1812
    //
    //////////////////////////////////////////////////////
    // 0x0a, 0x00, 0x02, 0x00, 0x34, 0x00, 0x00, 0x28, 0x12, 0x18,//ios app can't disconn

#if 0// authentication
    //
    10, 0x00,   0x00, 0x00,    0x1A, 0x00,     0x00, 0x28,     0x0A, 0x18,                                     //primary service declaration
    13, 0x00,   0x02, 0x00,    0x1B, 0x00,     0x03, 0x28,     0x02, 0x1C, 0x00, 0x50, 0x2A,                   //characteristic declaration
    15, 0x00,   0x02, 0x04,    0x1C, 0x00,     0x50, 0x2A,     0x02, 0x8A, 0x24, 0x66, 0x82, 0x34, 0x36,       //PnP ID
#endif


#if 0
    //////////////////////////////////////////////////////
    //
    // 0x0040 PRIMARY_SERVICE  ae3a
    //
    //////////////////////////////////////////////////////
    0x0a, 0x00, 0x02, 0x00, 0x40, 0x00, 0x00, 0x28, 0x3a, 0xae,

    /* CHARACTERISTIC,  ae3b, WRITE_WITHOUT_RESPONSE | DYNAMIC, */
    // 0x0041 CHARACTERISTIC ae3b WRITE_WITHOUT_RESPONSE | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x41, 0x00, 0x03, 0x28, 0x04, 0x42, 0x00, 0x3b, 0xae,
    // 0x0042 VALUE ae3b WRITE_WITHOUT_RESPONSE | DYNAMIC
    0x08, 0x00, 0x04, 0x01, 0x42, 0x00, 0x3b, 0xae,

    /* CHARACTERISTIC,  ae3c, NOTIFY, */
    // 0x0043 CHARACTERISTIC ae3c NOTIFY
    0x0d, 0x00, 0x02, 0x00, 0x43, 0x00, 0x03, 0x28, 0x10, 0x44, 0x00, 0x3c, 0xae,
    // 0x0044 VALUE ae3c NOTIFY
    0x08, 0x00, 0x10, 0x00, 0x44, 0x00, 0x3c, 0xae,
    // 0x0045 CLIENT_CHARACTERISTIC_CONFIGURATION
    0x0a, 0x00, 0x0a, 0x01, 0x45, 0x00, 0x02, 0x29, 0x00, 0x00,

    //////////////////////////////////////////////////////
    //
    // 0x0046 PRIMARY_SERVICE  1801
    //
    //////////////////////////////////////////////////////
    0x0a, 0x00, 0x02, 0x00, 0x46, 0x00, 0x00, 0x28, 0x01, 0x18,

    /* CHARACTERISTIC,  2a05, INDICATE, */
    // 0x0047 CHARACTERISTIC 2a05 INDICATE
    0x0d, 0x00, 0x02, 0x00, 0x47, 0x00, 0x03, 0x28, 0x20, 0x48, 0x00, 0x05, 0x2a,
    // 0x0048 VALUE 2a05 INDICATE
    0x08, 0x00, 0x20, 0x00, 0x48, 0x00, 0x05, 0x2a,
    // 0x0049 CLIENT_CHARACTERISTIC_CONFIGURATION
    0x0a, 0x00, 0x0a, 0x01, 0x49, 0x00, 0x02, 0x29, 0x00, 0x00,

#endif 


#if RCSP_BTMATE_EN
    //////////////////////////////////////////////////////
    //
    // 0x0004 PRIMARY_SERVICE  ae00
    //
    //////////////////////////////////////////////////////
    0x0a, 0x00, 0x02, 0x00, 0x80, 0x00, 0x00, 0x28, 0x00, 0xae,

    /* CHARACTERISTIC,  ae01, WRITE_WITHOUT_RESPONSE | DYNAMIC, */
    // 0x0040 CHARACTERISTIC ae01 WRITE_WITHOUT_RESPONSE | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x81, 0x00, 0x03, 0x28, 0x04, 0x82, 0x00, 0x01, 0xae,
    // 0x0041 VALUE ae01 WRITE_WITHOUT_RESPONSE | DYNAMIC
    0x08, 0x00, 0x04, 0x01, 0x82, 0x00, 0x01, 0xae,

    /* CHARACTERISTIC,  ae02, NOTIFY, */
    // 0x0042 CHARACTERISTIC ae02 NOTIFY
    0x0d, 0x00, 0x02, 0x00, 0x83, 0x00, 0x03, 0x28, 0x10, 0x84, 0x00, 0x02, 0xae,
    // 0x0043 VALUE ae02 NOTIFY
    0x08, 0x00, 0x10, 0x00, 0x84, 0x00, 0x02, 0xae,
    // 0x0044 CLIENT_CHARACTERISTIC_CONFIGURATION
    0x0a, 0x00, 0x0a, 0x01, 0x85, 0x00, 0x02, 0x29, 0x00, 0x00,
#endif
    // END
    0x00, 0x00,
};
//
// characteristics <--> handles
//
#if RCSP_BTMATE_EN
#define ATT_CHARACTERISTIC_ae01_02_VALUE_HANDLE 0x0082
#define ATT_CHARACTERISTIC_ae02_02_VALUE_HANDLE 0x0084
#define ATT_CHARACTERISTIC_ae02_02_CLIENT_CONFIGURATION_HANDLE 0x0085
#endif

#define ATT_CHARACTERISTIC_2a00_01_VALUE_HANDLE 0x0003
// #define ATT_CHARACTERISTIC_ae01_01_VALUE_HANDLE 0x0006
#define ATT_CHARACTERISTIC_ae01_01_VALUE_HANDLE 0x000A  //随便写的
#define ATT_CHARACTERISTIC_ae02_01_VALUE_HANDLE 0x0008
#define ATT_CHARACTERISTIC_ae02_01_CLIENT_CONFIGURATION_HANDLE 0x0009
#define ATT_CHARACTERISTIC_ae03_01_VALUE_HANDLE 0x000b
#define ATT_CHARACTERISTIC_ae04_01_VALUE_HANDLE 0x000d
#define ATT_CHARACTERISTIC_ae04_01_CLIENT_CONFIGURATION_HANDLE 0x000e
#define ATT_CHARACTERISTIC_ae05_01_VALUE_HANDLE 0x0010
#define ATT_CHARACTERISTIC_ae05_01_CLIENT_CONFIGURATION_HANDLE 0x0011
#define ATT_CHARACTERISTIC_ae10_01_VALUE_HANDLE 0x0013

#define ATT_CHARACTERISTIC_ae3b_01_VALUE_HANDLE 0x0042
#define ATT_CHARACTERISTIC_ae3c_01_VALUE_HANDLE 0x0044
#define ATT_CHARACTERISTIC_ae3c_01_CLIENT_CONFIGURATION_HANDLE 0x0045
#define ATT_CHARACTERISTIC_2a05_01_VALUE_HANDLE 0x0048
#define ATT_CHARACTERISTIC_2a05_01_CLIENT_CONFIGURATION_HANDLE 0x0049


//从机蓝牙服务----
#define ATT_CHARACTERISTIC_fff1_01_VALUE_HANDLE 0x0006
#define ATT_CHARACTERISTIC_fff1_01_CLIENT_CONFIGURATION_HANDLE 0x0007





#endif

