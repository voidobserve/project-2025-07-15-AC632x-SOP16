#ifndef __RF_433_KEY_H__
#define __RF_433_KEY_H__

#include "includes.h"

// 433信号检测引脚
// #define RF_433_KEY_SCAN_PIN IO_PORTB_06 // PB6 ,IC 1脚 (被外部硬件拉低)
// #define RF_433_KEY_SCAN_PIN IO_PORTB_07 // PB7 ,IC 16脚 （无法控制，可能有其他程序占用）
#define RF_433_KEY_SCAN_PIN IO_PORTB_05 // PB5 IC 2脚

// #define RF_433_KEY_SCAN_PIN IO_PORTB_01 //  PB1 用开发板来测试

/*
    按键的扫描周期，单位：ms
    注意不能超过变量的最大值
*/

#define RF_433_KEY_SCAN_CIRCLE_TIMES (10) 

#define RF_433_KEY_SCAN_FILTER_TIMES (0)                // 按键消抖次数
#define RF_433_KEY_SCAN_LONG_PRESS_TIME_THRESHOLD ((u16)750) // 长按时间阈值，单位：ms
#define RF_433_KEY_SCAN_HOLD_PRESS_TIME_THRESHOLD ((u16)150) // 长按持续（不松手）的时间阈值，单位：ms，每隔 xx ms认为有一次长按持续事件

// #define RF_433_KEY_SCAN_LONG_PRESS_TIME_THRESHOLD ((u16)750 * 2) // 长按时间阈值，单位：ms
// #define RF_433_KEY_SCAN_HOLD_PRESS_TIME_THRESHOLD ((u16)150 * 2) // 长按持续（不松手）的时间阈值，单位：ms，每隔 xx ms认为有一次长按持续事件

// #define RF_433_KEY_SCAN_LONG_PRESS_TIME_THRESHOLD ((u16)750 * 3) // 长按时间阈值，单位：ms
// #define RF_433_KEY_SCAN_HOLD_PRESS_TIME_THRESHOLD ((u16)150 * 3) // 长按持续（不松手）的时间阈值，单位：ms，每隔 xx ms认为有一次长按持续事件
#define RF_433_KEY_SCAN_MUILTY_CLICK_TIME_THRESHOLD (500) // 等待多击的时间间隔，单位：ms

/*
    定义遥控器按键键值
*/
#define RF_433_KEY_ID_1 (0x14 & 0xFF) // R1C1，第一行第一列
#define RF_KEY_ID_2 (0x18 & 0xFF)     // R1C2
#define RF_KEY_ID_3 (0x10 & 0xFF)     // R2C1
#define RF_KEY_ID_4 (0x15 & 0xFF)
#define RF_KEY_ID_5 (0x05 & 0xFF)
#define RF_KEY_ID_6 (0x08 & 0xFF)
#define RF_KEY_ID_7 (0x06 & 0xFF)
#define RF_KEY_ID_8 (0x09 & 0xFF)

typedef struct
{
    struct key_driver_para rf_433_key_para;
    u8 rf_433_key_driver_event;   // 存放得到的按键事件，在key_driver_scan()中更新
    u8 rf_433_key_latest_key_val; // 存放最新扫描到的按键键值
    // u8 rf_433_key_last_key_val; // 存放上一次得到的按键键值
} rf_433_key_struct_t;

extern rf_433_key_struct_t rf_433_key_structure;
// extern struct key_driver_para rf_433_key_para; // rf 433 按键扫描参数 结构体

extern volatile u32 rf_433_data;               // 存放收到的rf433数据
extern volatile u8 flag_is_recved_rf_433_data; // 标志位，是否收到一次数据

void rf_433_key_config(void);

// void rf_433_key_event_handle(void);// 按键事件处理函数
void rf_433_key_event_handle(void *p); // 按键事件处理函数

// void rf_433_key_test(void * p);

#endif
