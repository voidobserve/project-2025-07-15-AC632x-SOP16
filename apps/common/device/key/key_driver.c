#include "device/key_driver.h"
#include "system/event.h"
#include "system/init.h"
#include "iokey.h"
#include "adkey.h"
#include "slidekey.h"
#include "irkey.h"
#include "touch_key.h"
#include "system/timer.h"
#include "asm/power_interface.h"
#include "app_config.h"
#include "rdec_key.h"
#include "tent600_key.h"
#if TCFG_KEY_TONE_EN
#include "tone_player.h"
#endif

#include "rf_433_key.h"

#if (TCFG_IRSENSOR_ENABLE == 1)
#include "irSensor/ir_manage.h"
#endif

#define LOG_TAG_CONST KEY
#define LOG_TAG "[KEY]"
#define LOG_ERROR_ENABLE
#define LOG_DEBUG_ENABLE
#define LOG_INFO_ENABLE
// #define LOG_DUMP_ENABLE
// #define LOG_CLI_ENABLE
#include "debug.h"

#define KEY_EVENT_CLICK_ONLY_SUPPORT 0 // 是否支持某些按键只响应单击事件

#if TCFG_SPI_LCD_ENABLE
#ifndef ALL_KEY_EVENT_CLICK_ONLY
#define ALL_KEY_EVENT_CLICK_ONLY 0 // 是否全部按键只响应单击事件
#endif
#else
#ifndef ALL_KEY_EVENT_CLICK_ONLY
#define ALL_KEY_EVENT_CLICK_ONLY 0 // 是否全部按键只响应单击事件
#endif
#endif

static volatile u8 is_key_active = 0;
static volatile u8 key_poweron_flag = 0;

extern u32 timer_get_ms(void);

//=======================================================//
// 按键值重新映射函数:
// 用户可以实现该函数把一些按键值重新映射, 可用于组合键的键值重新映射
//=======================================================//
int __attribute__((weak)) key_event_remap(struct sys_event *e)
{
    return true;
}

//=======================================================//
// 设置按键开机标志位
//=======================================================//
void set_key_poweron_flag(u8 flag)
{
    key_poweron_flag = flag;
}

//=======================================================//
// 获取按键开机标志位
//=======================================================//
u8 get_key_poweron_flag(void)
{
    return key_poweron_flag;
}

//=======================================================//
// 清除按键开机标志位
//=======================================================//
void clear_key_poweron_flag(void)
{
    key_poweron_flag = 0;
}

#if TCFG_SOFTOFF_WAKEUP_KEY_DRIVER_ENABLE
static u8 key_scan_flag = 0, en_key_cnt = 0, en_key_cnt_flag = 1, nonsrc_wakeup_flag = 0, save_notify_flag = 0, key_wk_send_flag = 0;
struct sys_event e_tmp;
//=======================================================//
// 清除保存的key_notify
//=======================================================//
void clear_save_key_notify(void)
{
    save_notify_flag = 0;
    memset(&e_tmp, 0, sizeof(struct sys_event));
}
//=======================================================//
// 设置软关机按键唤醒补充发键标志位
//=======================================================//
void set_key_wakeup_send_flag(u8 flag)
{
    key_wk_send_flag = flag;
    if (save_notify_flag)
    {
        sys_event_notify(&e_tmp);
        clear_save_key_notify();
    }
}

//=======================================================//
// 获取软关机按键唤醒补充发键标志位
//=======================================================//
u8 get_key_wakeup_send_flag(void)
{
    return key_wk_send_flag;
}

//=======================================================//
// 保存未初始化按键消息前推送的按键notify
//=======================================================//
void save_wakeup_key_notify(struct sys_event *event)
{
    save_notify_flag = 1;

    e_tmp.type = event->type;
    e_tmp.arg = event->arg;
    e_tmp.u.key.init = event->u.key.init;
    e_tmp.u.key.type = event->u.key.type;
    e_tmp.u.key.event = event->u.key.event;
    e_tmp.u.key.value = event->u.key.value;
    e_tmp.u.key.tmr = event->u.key.tmr;
}
#endif

//=======================================================//
// 按键扫描函数: 扫描所有注册的按键驱动
//=======================================================//
static void key_driver_scan(void *_scan_para)
{
    struct key_driver_para *scan_para = (struct key_driver_para *)_scan_para;

    u8 key_event = 0;
    u8 cur_key_value = NO_KEY;
    u8 key_value = 0;
    struct sys_event e;
    static u8 poweron_cnt = 0;

    // 为了滤掉adkey与mic连在一起时电容充放电导致的开机按键误判,一般用于type-c耳机
    /* if (poweron_cnt <= 250) { */
    /*     poweron_cnt++; */
    /*     return; */
    /* } */

    cur_key_value = scan_para->get_value();

    /* if (cur_key_value != NO_KEY) { */
    /*     printf(">>>cur_key_value: %d\n", cur_key_value); */
    /* } */

#if TCFG_SOFTOFF_WAKEUP_KEY_DRIVER_ENABLE
    u8 wkup_src = get_wakeup_pnd();

    // 按键唤醒后若未按下第二击，推出单击事件
    // key_driver初始化后未扫描到按键
    if (key_wk_send_flag && key_scan_flag == 0)
    {
        key_scan_flag = 1;
        // 记住唤醒源——识别NO_KEY时的键值
        if (cur_key_value == NO_KEY)
        {
            if (wkup_src == TCFG_WAKEUP_PORT_POWER_SRC)
            { // 唤醒口port1
                key_value = TCFG_IOKEY_POWER_ONE_PORT_VALUE;
            }
            else if (wkup_src == TCFG_WAKEUP_PORT_PREV_SRC)
            { // 唤醒口port2
                key_value = TCFG_IOKEY_PREV_ONE_PORT_VALUE;
            }
            else if (wkup_src == TCFG_WAKEUP_PORT_NEXT_SRC)
            { // 唤醒口port3
                key_value = TCFG_IOKEY_NEXT_ONE_PORT_VALUE;
            }
            else
            {
                nonsrc_wakeup_flag = 1;
                printf("wakeup source err is 0x%x !!!", wkup_src);
                return;
            }
            key_event = KEY_EVENT_CLICK; // 单击
            goto _notify;
        }
        // 初始化后扫描到了按键，但是未满足key_driver的单击事件推出，补充推出消息
    }
    else if (en_key_cnt && en_key_cnt <= (scan_para->filter_time + 1) && key_scan_flag == 1 && en_key_cnt_flag == 0)
    {
        en_key_cnt = 0;
        key_scan_flag = 0;
    }

    if (cur_key_value == NO_KEY)
    {
        en_key_cnt_flag = 0;
    }
    else
    {
        key_scan_flag = 1;
        if (en_key_cnt_flag)
        {
            en_key_cnt++;
        }
    }
#endif

    if (cur_key_value != NO_KEY)
    {
        is_key_active = 35; // 35*10Ms
    }
    else if (is_key_active)
    {
        is_key_active--;
    }
    //===== 按键消抖处理
    if (cur_key_value != scan_para->filter_value && scan_para->filter_time)
    {                                            // 当前按键值与上一次按键值如果不相等, 重新消抖处理, 注意filter_time != 0;
        scan_para->filter_cnt = 0;               // 消抖次数清0, 重新开始消抖
        scan_para->filter_value = cur_key_value; // 记录上一次的按键值
        return;                                  // 第一次检测, 返回不做处理
    } // 当前按键值与上一次按键值相等, filter_cnt开始累加;
    if (scan_para->filter_cnt < scan_para->filter_time)
    {
        scan_para->filter_cnt++;
        return;
    }
    // 为了滤掉adkey与mic连在一起时电容充放电导致的按键误判,一般用于type-c耳机
    /* if ((cur_key_value != scan_para->last_key) && (scan_para->last_key != NO_KEY) && (cur_key_value != NO_KEY)) { */
    /*     return; */
    /* } */
    //===== 按键消抖结束, 开始判断按键类型(单击, 双击, 长按, 多击, HOLD, (长按/HOLD)抬起)
    if (cur_key_value != scan_para->last_key)
    {
        if (cur_key_value == NO_KEY)
        { // cur_key = NO_KEY; last_key = valid_key -> 按键被抬起

#if MOUSE_KEY_SCAN_MODE
            /* if (scan_para->press_cnt >= scan_para->long_time) {  //长按/HOLD状态之后被按键抬起; */
            key_event = KEY_EVENT_UP;
            key_value = scan_para->last_key;
            goto _notify; // 发送抬起消息
                          /* } */
#else
#if TCFG_SOFTOFF_WAKEUP_KEY_DRIVER_ENABLE
            if (scan_para->press_cnt >= (scan_para->long_time - TCFG_LONGKEY_SUPPLEMENT_TIME)) // 长按/HOLD状态之后被按键抬起, 补充长按时间;
#else
            if (scan_para->press_cnt >= scan_para->long_time) // 长按/HOLD状态之后被按键抬起;
#endif
            {
                key_event = KEY_EVENT_UP;
                key_value = scan_para->last_key;
                goto _notify; // 发送抬起消息
            }
#endif

            scan_para->click_delay_cnt = 1; // 按键等待下次连击延时开始
        }
        else
        {                             // cur_key = valid_key, last_key = NO_KEY -> 按键被按下
            scan_para->press_cnt = 1; // 用于判断long和hold事件的计数器重新开始计时;
            if (cur_key_value != scan_para->notify_value)
            { // 第一次单击/连击时按下的是不同按键, 单击次数重新开始计数
#if TCFG_SOFTOFF_WAKEUP_KEY_DRIVER_ENABLE
                if ((en_key_cnt > scan_para->filter_time && en_key_cnt < scan_para->long_time) || key_wk_send_flag || nonsrc_wakeup_flag)
                {
                    scan_para->click_cnt = 1;
                }
                else
                {
                    scan_para->click_cnt = 2;
                }
#else
                scan_para->click_cnt = 1;
#endif
                scan_para->notify_value = cur_key_value;
            }
            else
            {
                scan_para->click_cnt++; // 单击次数累加
            }
        }
        goto _scan_end; // 返回, 等待延时时间到
    }
    else
    { // cur_key = last_key -> 没有按键按下/按键长按(HOLD)
        if (cur_key_value == NO_KEY)
        { // last_key = NO_KEY; cur_key = NO_KEY -> 没有按键按下
            if (scan_para->click_cnt > 0)
            { // 有按键需要消息需要处理

#if ALL_KEY_EVENT_CLICK_ONLY                 // 彩屏方案支持单击
                key_event = KEY_EVENT_CLICK; // 单击
                key_value = scan_para->notify_value;
                goto _notify;

#endif

#if KEY_EVENT_CLICK_ONLY_SUPPORT // 是否支持某些按键只响应单击事件
                if (scan_para->notify_value & BIT(7))
                {                                // BIT(7)按键特殊处理标志, 只发送单击事件, 也可以用于其它扩展
                    key_event = KEY_EVENT_CLICK; // 单击
                    key_value = scan_para->notify_value;
                    goto _notify;
                }
#endif
                if (scan_para->click_delay_cnt > scan_para->click_delay_time)
                { // 按键被抬起后延时到
                    // TODO: 在此可以添加任意多击事件
                    if (scan_para->click_cnt >= 5)
                    {
                        key_event = KEY_EVENT_FIRTH_CLICK; // 五击
                    }
                    else if (scan_para->click_cnt >= 4)
                    {
                        key_event = KEY_EVENT_FOURTH_CLICK; // 4击
                    }
                    else if (scan_para->click_cnt >= 3)
                    {
                        key_event = KEY_EVENT_TRIPLE_CLICK; // 三击
                    }
                    else if (scan_para->click_cnt >= 2)
                    {
                        key_event = KEY_EVENT_DOUBLE_CLICK; // 双击
                    }
                    else
                    {
                        key_event = KEY_EVENT_CLICK; // 单击
                    }
                    key_value = scan_para->notify_value;
                    goto _notify;
                }
                else
                { // 按键抬起后等待下次延时时间未到
                    scan_para->click_delay_cnt++;
                    goto _scan_end; // 按键抬起后延时时间未到, 返回
                }
            }
            else
            {
                goto _scan_end; // 没有按键需要处理
            }
        }
        else
        { // last_key = valid_key; cur_key = valid_key, press_cnt累加用于判断long和hold
            scan_para->press_cnt++;

#if TCFG_SOFTOFF_WAKEUP_KEY_DRIVER_ENABLE
            if (scan_para->press_cnt == (scan_para->long_time - TCFG_LONGKEY_SUPPLEMENT_TIME))
#else
            if (scan_para->press_cnt == scan_para->long_time)
#endif
            {
                key_event = KEY_EVENT_LONG;
            }
            else if (scan_para->press_cnt == scan_para->hold_time)
            {
                key_event = KEY_EVENT_HOLD;
                scan_para->press_cnt = scan_para->long_time;
            }
            else
            {
                goto _scan_end; // press_cnt没到长按和HOLD次数, 返回
            }
            // press_cnt没到长按和HOLD次数, 发消息
            key_value = cur_key_value;
            goto _notify;
        }
    }

_notify:
#if TCFG_IRSENSOR_ENABLE
    if (get_irSensor_event() == IR_SENSOR_EVENT_FAR)
    { // 未佩戴的耳机不响应按键
        return;
    }
#endif
    if (scan_para->key_type == KEY_DRIVER_TYPE_RF_433_KEY)
    {
        rf_433_key_structure.rf_433_key_latest_key_val = key_value;
        // printf("rf 433 cur key value %u\n", (u16)cur_key_value);
        rf_433_key_structure.rf_433_key_driver_event = key_event;

        scan_para->click_cnt = 0; // 单击次数清0
        scan_para->notify_value = NO_KEY;

        // printf("key event %u\n", key_event);
    }
    else
    {
        // key_value &= ~BIT(7);  //BIT(7) 用作按键特殊处理的标志
        e.type = SYS_KEY_EVENT;
        e.u.key.init = 1;
        e.u.key.type = scan_para->key_type; // 区分按键类型
        e.u.key.event = key_event;
        e.u.key.value = key_value;
        e.u.key.tmr = timer_get_ms();

        scan_para->click_cnt = 0; // 单击次数清0
        scan_para->notify_value = NO_KEY;

        e.arg = (void *)DEVICE_EVENT_FROM_KEY;

        // printf("scan_para->press_cnt = %d",scan_para->press_cnt);
        // printf("key_value: 0x%x, event: %d, key_poweron_flag: %d\n", key_value, key_event, key_poweron_flag);
        if (key_poweron_flag)
        {
            if (key_event == KEY_EVENT_UP)
            {
                clear_key_poweron_flag();
                return;
            }
            return;
        }
        if (key_event_remap(&e))
        {
#if TCFG_SOFTOFF_WAKEUP_KEY_DRIVER_ENABLE
            if (key_wk_send_flag)
            {
                sys_event_notify(&e);
            }
            else
            {
                save_wakeup_key_notify(&e);
            }
            nonsrc_wakeup_flag = 0;
#else
            sys_event_notify(&e);
#endif

#if TCFG_KEY_TONE_EN
            audio_key_tone_play();
#endif
        }
    }

_scan_end:
    scan_para->last_key = cur_key_value;
    return;
}

#include "rf24g_app.h"

// wakeup callback
void key_active_set(u8 port)
{
    /*要用按键宏包住，不用按键功能，唤醒后就再也不进睡眠*/
#if (TCFG_IOKEY_ENABLE || TCFG_ADKEY_ENABLE || TCFG_TOUCH_KEY_ENABLE)
    is_key_active = 35; // 35*10Ms
#endif
}

//=======================================================//
// 按键初始化函数: 初始化所有注册的按键驱动
//=======================================================//
int key_driver_init(void)
{
    int err;

#if TCFG_RF24GKEY_ENABLE
    extern void RF24G_Key_Handle(void);
    extern void RF24G_Key_Long_Scan(void);
    extern struct key_driver_para rf24g_scan_para;
    extern struct RF24G_PARA is_rf24g_;
    sys_s_hi_timer_add((void *)&rf24g_scan_para, key_driver_scan, rf24g_scan_para.scan_time); // 注册按键扫描定时器
    sys_s_hi_timer_add(NULL, RF24G_Key_Handle, is_rf24g_._sacn_t);                            // 注册按键扫描定时器
    sys_s_hi_timer_add(NULL, RF24G_Key_Long_Scan, is_rf24g_._sacn_t);                         // 注册按键扫描定时器

#endif

    extern rf_433_key_struct_t rf_433_key_structure;
    sys_hi_timer_add((void *)&rf_433_key_structure.rf_433_key_para, key_driver_scan, rf_433_key_structure.rf_433_key_para.scan_time); // 注册按键扫描定时器
    sys_timer_add(NULL, rf_433_key_event_handle, 1);
    // sys_hi_timer_add(NULL, rf_433_key_event_handle, 1);

    // sys_hi_timer_add(NULL, rf_433_key_test, 1); // 测试会不会误解码或者丢包

#if TCFG_IOKEY_ENABLE
    extern const struct iokey_platform_data iokey_data;
    extern struct key_driver_para iokey_scan_para;
    err = iokey_init(&iokey_data);
#ifdef TCFG_IOKEY_TIME_REDEFINE
    extern struct key_driver_para iokey_scan_user_para;
    if (err == 0)
    {
        sys_s_hi_timer_add((void *)&iokey_scan_user_para, key_driver_scan, iokey_scan_user_para.scan_time); // 注册按键扫描定时器
    }
#else
    if (err == 0)
    {
        sys_s_hi_timer_add((void *)&iokey_scan_para, key_driver_scan, iokey_scan_para.scan_time); // 注册按键扫描定时器
    }
#endif
#endif

#if TCFG_ADKEY_ENABLE
#ifdef CONFIG_ICRECORDER_CASE_ENABLE
    extern multi_adkey_init(const struct adkey_platform_data *multi_adkey_data);
    extern const struct adkey_platform_data multi_adkey_data[];
    extern struct key_driver_para multi_adkey_scan_para;
    err = multi_adkey_init(multi_adkey_data);
    if (err == 0)
    {
        sys_s_hi_timer_add((void *)&multi_adkey_scan_para, key_driver_scan, multi_adkey_scan_para.scan_time); // 注册按键扫描定时器
    }
#else
    extern const struct adkey_platform_data adkey_data;
    extern struct key_driver_para adkey_scan_para;
    err = adkey_init(&adkey_data);
    if (err == 0)
    {
        sys_s_hi_timer_add((void *)&adkey_scan_para, key_driver_scan, adkey_scan_para.scan_time); // 注册按键扫描定时器
    }
#endif
#endif

#if TCFG_IRKEY_ENABLE
    extern const struct irkey_platform_data irkey_data;
    extern struct key_driver_para irkey_scan_para;
    err = irkey_init(&irkey_data);
    if (err == 0)
    {
        sys_s_hi_timer_add((void *)&irkey_scan_para, key_driver_scan, irkey_scan_para.scan_time); // 注册按键扫描定时器
    }
#endif

#if TCFG_TOUCH_KEY_ENABLE
    extern const struct touch_key_platform_data touch_key_data;
    extern struct key_driver_para touch_key_scan_para;
    err = touch_key_init(&touch_key_data);
    if (err == 0)
    {
        sys_s_hi_timer_add((void *)&touch_key_scan_para, key_driver_scan, touch_key_scan_para.scan_time); // 注册按键扫描定时器
    }
#endif

#if TCFG_ADKEY_RTCVDD_ENABLE
    extern const struct adkey_rtcvdd_platform_data adkey_rtcvdd_data;
    extern struct key_driver_para adkey_rtcvdd_scan_para;
    err = adkey_rtcvdd_init(&adkey_rtcvdd_data);
    if (err == 0)
    {
        sys_s_hi_timer_add((void *)&adkey_rtcvdd_scan_para, key_driver_scan, adkey_rtcvdd_scan_para.scan_time); // 注册按键扫描定时器
    }
#endif

#if TCFG_RDEC_KEY_ENABLE
    extern const struct rdec_platform_data rdec_key_data;
    extern struct key_driver_para rdec_key_scan_para;
    err = rdec_key_init(&rdec_key_data);
    if (err == 0)
    {
        sys_s_hi_timer_add((void *)&rdec_key_scan_para, key_driver_scan, rdec_key_scan_para.scan_time); // 注册按键扫描定时器
    }
#endif

#if TCFG_CTMU_TOUCH_KEY_ENABLE
#include "asm/ctmu.h"
    extern const struct ctmu_touch_key_platform_data ctmu_touch_key_data;
    extern struct key_driver_para ctmu_touch_key_scan_para;
    extern int ctmu_touch_key_init(const struct ctmu_touch_key_platform_data *ctmu_touch_key_data);
    err = ctmu_touch_key_init(&ctmu_touch_key_data);
    if (err == 0)
    {
        sys_s_hi_timer_add((void *)&ctmu_touch_key_scan_para, key_driver_scan, ctmu_touch_key_scan_para.scan_time); // 注册按键扫描定时器
    }
#endif /* #if TCFG_CTMU_TOUCH_KEY_ENABLE */

#if TCFG_SLIDE_KEY_ENABLE
    extern const struct slidekey_platform_data slidekey_data;
    extern int slidekey_init(const struct slidekey_platform_data *slidekey_data);
    err = slidekey_init(&slidekey_data);
    if (err == 0)
    {
    }
#endif // TCFG_SLIDE_KEY_ENABLE

#if TCFG_6083_ADKEY_ENABLE
    extern int adkey_init_6083();
    err = adkey_init_6083();
    if (err == 0)
    {
    }
#endif
#if TCFG_TENT600_KEY_ENABLE
    extern const struct tent600_key_platform_data key_data;
    extern struct key_driver_para tent600_key_scan_para;
    err = tent600_key_init(&key_data);
    if (err == 0)
    {
        sys_s_hi_timer_add((void *)&tent600_key_scan_para, key_driver_scan, tent600_key_scan_para.scan_time); // 注册按键扫描定时器
    }
#endif

    return 0;
}

static u8 key_idle_query(void)
{
    return !is_key_active;
}
#if ((!TCFG_LP_TOUCH_KEY_ENABLE) && (TCFG_IOKEY_ENABLE || TCFG_ADKEY_ENABLE))
REGISTER_LP_TARGET(key_lp_target) = {
    .name = "key",
    .is_idle = key_idle_query,
};
#endif /* #if !TCFG_LP_TOUCH_KEY_ENABLE */

#if (TCFG_SOFTOFF_WAKEUP_KEY_DRIVER_ENABLE && TCFG_IOKEY_ENABLE)
// 默认0，按键消息使能后置1,避免(软关机唤醒——发送按键消息)这段时间进入睡眠状态
REGISTER_LP_TARGET(softoff_wake_key) = {
    .name = "softoff_wake_key",
    .is_idle = get_key_wakeup_send_flag,
};
#endif
