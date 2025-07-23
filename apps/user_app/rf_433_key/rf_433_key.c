#include "rf_433_key.h"

volatile u32 rf_433_data = 0;
volatile u8 flag_is_recved_rf_433_data = 0;

void rf_433_key_config(void)
{
    gpio_set_pull_up(RF_433_KEY_SCAN_PIN, 1);   // 上拉
    gpio_set_pull_down(RF_433_KEY_SCAN_PIN, 0); // 不下拉

    gpio_set_die(RF_433_KEY_SCAN_PIN, 1);       // 普通输入模式
    gpio_set_direction(RF_433_KEY_SCAN_PIN, 1); // 输入模式
}
