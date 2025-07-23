#ifndef __RF_433_KEY_H__
#define __RF_433_KEY_H__

#include "includes.h"

// 433信号检测引脚
// #define RF_433_KEY_SCAN_PIN IO_PORTB_06 // PB6 ,IC 1脚
#define RF_433_KEY_SCAN_PIN IO_PORTB_07 // PB7 ,IC 16脚

extern volatile u32 rf_433_data;
extern volatile u8 flag_is_recved_rf_433_data;

void rf_433_key_config(void);

#endif
