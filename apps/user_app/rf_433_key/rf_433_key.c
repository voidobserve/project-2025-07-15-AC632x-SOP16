#include "rf_433_key.h"

volatile u32 rf_433_data = 0;
volatile u8 flag_is_recved_rf_433_data = 0;

// 定义按键事件
enum
{
    RF_433_KEY_EVENT_NONE = 0,
    RF_433_KEY_EVENT_ID1_CLICK,
    RF_433_KEY_EVENT_ID1_DOUBLE_CLICK,
    RF_433_KEY_EVENT_ID1_LONG_PRESS,
    RF_433_KEY_EVENT_ID1_HOLD_PRESS,
    RF_433_KEY_EVENT_ID1_LOOSE,

};

#define RF_433_KEY_VALID_EVENT_NUMS (5) // 单个按键，有效的按键事件数量
// 将遥控器按键的键值和按键事件绑定
const u8 rf_433_key_event_table[][RF_433_KEY_VALID_EVENT_NUMS + 1] = {
    {RF_433_KEY_ID_1, RF_433_KEY_EVENT_ID1_CLICK, RF_433_KEY_EVENT_ID1_DOUBLE_CLICK, RF_433_KEY_EVENT_ID1_LONG_PRESS, RF_433_KEY_EVENT_ID1_HOLD_PRESS, RF_433_KEY_EVENT_ID1_LOOSE},
};

static u8 rf_433_key_get_value(void);
rf_433_key_struct_t rf_433_key_structure = {
    .rf_433_key_para.scan_time = RF_433_KEY_SCAN_CIRCLE_TIMES, // 扫描间隔时间
    .rf_433_key_para.last_key = NO_KEY,
    .rf_433_key_para.filter_value = 0,
    .rf_433_key_para.filter_cnt = 0,
    .rf_433_key_para.filter_time = 0,
    .rf_433_key_para.long_time = RF_433_KEY_SCAN_LONG_PRESS_TIME_THRESHOLD / RF_433_KEY_SCAN_CIRCLE_TIMES,
    .rf_433_key_para.hold_time = (RF_433_KEY_SCAN_LONG_PRESS_TIME_THRESHOLD + RF_433_KEY_SCAN_HOLD_PRESS_TIME_THRESHOLD) / RF_433_KEY_SCAN_CIRCLE_TIMES,
    .rf_433_key_para.press_cnt = 0,
    .rf_433_key_para.click_cnt = 0,
    .rf_433_key_para.click_delay_cnt = 0,
    .rf_433_key_para.click_delay_time = RF_433_KEY_SCAN_MUILTY_CLICK_TIME_THRESHOLD / RF_433_KEY_SCAN_CIRCLE_TIMES,
    .rf_433_key_para.notify_value = 0,
    .rf_433_key_para.key_type = KEY_DRIVER_TYPE_RF_433_KEY,
    .rf_433_key_para.get_value = rf_433_key_get_value,

    .rf_433_key_driver_event = 0,
    .rf_433_key_latest_key_val = NO_KEY,
};

// struct key_driver_para rf_433_key_para = {
//     .scan_time = RF_433_KEY_SCAN_CIRCLE_TIMES, // 扫描间隔时间
//     .last_key = NO_KEY,
//     .filter_value = 0,
//     .filter_cnt = 0,
//     .filter_time = 0,
//     .long_time = RF_433_KEY_SCAN_LONG_PRESS_TIME_THRESHOLD / RF_433_KEY_SCAN_CIRCLE_TIMES,
//     .hold_time = RF_433_KEY_SCAN_HOLD_PRESS_TIME_THRESHOLD / RF_433_KEY_SCAN_CIRCLE_TIMES,
//     .press_cnt = 0,
//     .click_cnt = 0,
//     .click_delay_cnt = 0,
//     .click_delay_time = 0,
//     .notify_value = 0,
//     .key_type = KEY_DRIVER_TYPE_RF_433_KEY,
//     .get_value = rf_433_key_get_value,
// };

static u8 rf_433_key_get_value(void)
{
    static u8 time_out_cnt = 0;
    static u32 last_rf_433_data = 0;

    u8 ret = NO_KEY;

    printf("get key val 0x %lx \n", rf_433_data); 
    if (flag_is_recved_rf_433_data)
    {
        flag_is_recved_rf_433_data = 0;

        last_rf_433_data = rf_433_data;
        time_out_cnt = 200; //  RF_433_KEY_SCAN_CIRCLE_TIMES

        // return (u8)(rf_433_data & 0xFF);
        ret = (u8)(rf_433_data & 0xFF);
    }
    else if (time_out_cnt > 0)
    {
        ret = (u8)(last_rf_433_data & 0xFF);
        time_out_cnt--;
    }

    return ret;
}

void rf_433_key_config(void)
{
    gpio_set_pull_up(RF_433_KEY_SCAN_PIN, 1);   // 上拉
    gpio_set_pull_down(RF_433_KEY_SCAN_PIN, 0); // 不下拉

    gpio_set_die(RF_433_KEY_SCAN_PIN, 1);       // 普通输入模式
    gpio_set_direction(RF_433_KEY_SCAN_PIN, 1); // 输入模式
}

/**
 * @brief 根据 key_driver_scan()得到的按键键值和按键事件，转换成自定义的按键事件
 *
 * @param key_value key_driver_scan()得到的按键键值
 * @param key_event key_driver_scan()得到的按键事件
 *
 * @return u8 自定义的按键事件
 */
static u8 rf_433_key_get_keyevent(const u8 key_val, const u8 key_event)
{
    u8 i;
    u8 rf_433_key_event = RF_433_KEY_EVENT_NONE;
    u8 rf_433_key_event_table_index = 0;

    // printf("key event %u\n", (u16)key_event);

    /* 将 key_driver_scan() 得到的按键事件映射到自定义的按键事件列表中，用于下面的查表 */
    switch (key_event)
    {
    case KEY_EVENT_CLICK:
        /* 短按，在数组 rf_433_key_event_table 的[x][1]位置*/
        rf_433_key_event_table_index = 1;
        break;
    case KEY_EVENT_DOUBLE_CLICK:
        /* 双击，在数组 rf_433_key_event_table 的[x][2]位置*/
        rf_433_key_event_table_index = 2;
        break;
    case KEY_EVENT_LONG:
        /* 长按，在数组 rf_433_key_event_table 的[x][3]位置 */
        rf_433_key_event_table_index = 3;
        break;
    case KEY_EVENT_HOLD:
        /* 长按持续（不松手），在数组 rf_433_key_event_table 的[x][4]位置 */
        rf_433_key_event_table_index = 4;
        break;

    case KEY_EVENT_UP:
        /* 长按后松手，在数组 rf_433_key_event_table 的[x][5]位置 */
        rf_433_key_event_table_index = 5;
        break;

    default:
        // 其他按键事件，认为是没有事件
        rf_433_key_event = RF_433_KEY_EVENT_NONE;
        return rf_433_key_event;
        break;
    }

    for (i = 0; i < ARRAY_SIZE(rf_433_key_event_table); i++)
    {
        if (key_val == rf_433_key_event_table[i][0])
        {
            rf_433_key_event = rf_433_key_event_table[i][rf_433_key_event_table_index];
            break;
        }
    }

    return rf_433_key_event;
}

// 按键事件处理函数
void rf_433_key_event_handle(void *p)
{
    u8 rf_433_key_event = RF_433_KEY_EVENT_NONE;

    // if (NO_KEY == rf_433_key_structure.rf_433_key_latest_key_val)
    // {
    //     return;
    // }

    // printf("get key event\n");

    rf_433_key_event = rf_433_key_get_keyevent(rf_433_key_structure.rf_433_key_latest_key_val, rf_433_key_structure.rf_433_key_driver_event);
    rf_433_key_structure.rf_433_key_latest_key_val = NO_KEY;
    rf_433_key_structure.rf_433_key_driver_event = RF_433_KEY_EVENT_NONE;

#if 1
    switch (rf_433_key_event)
    {
    case RF_433_KEY_EVENT_ID1_CLICK:
        printf("ID1 click \n");
        break;

    case RF_433_KEY_EVENT_ID1_DOUBLE_CLICK:
        printf("ID1 double click \n");
        break;

    case RF_433_KEY_EVENT_ID1_LONG_PRESS:
        printf("ID1 long press \n");
        break;

    case RF_433_KEY_EVENT_ID1_HOLD_PRESS:
        printf("ID1 hold press \n");
        break;

    case RF_433_KEY_EVENT_ID1_LOOSE:
        printf("ID1 loose \n");
        break;
    }
#endif
}

#if 0
void rf_433_key_test(void * p)
{
    if (flag_is_recved_rf_433_data)
    {
        flag_is_recved_rf_433_data = 0;
        // return (u8)(rf_433_data & 0xFF);

        printf("rf 433 data 0x %lx\n", rf_433_data);
    }
}
#endif
