#ifndef led_srtip_sys_h
#define led_srtip_sys_h

// 控灯系统属性
#include "cpu.h"
/******************************************************************系统cfg******************************************************************/
/*定义数据总线大端/小端，二选一*/
#define SYS_LITTLE_END 0
#define SYS_BIG_END 1
// 定义协议大小端
#define PROTOCOL_END SYS_BIG_END
#define SYS_BUS_END SYS_LITTLE_END

#if (PROTOCOL_END == SYS_BUS_END)
#define __SWP16(n) n
#define __SWP32(n) n
#endif
#if (PROTOCOL_END != SYS_BUS_END)
#define __SWP16(n) ((((u16)(n) & 0xff00) >> 8) | (((u16)(n) & 0x00FF) << 8))
#define __SWP32(n) (((u32)(n) & 0xff000000 >> 24) | \
                    ((u32)(n) & 0x00ff0000 >> 8) |  \
                    ((u32)(n) & 0x0000ff00 << 8) |  \
                    ((u32)(n) & 0x000000ff << 24))

#endif
/******************************************************************灯具配置 cfg******************************************************************/

#define TYPE_Fiber_optic_lights (1) // 光纤灯
#define TYPE_Magic_lights (2)       // 幻彩灯
#define LED_STRIP_TYPE TYPE_Fiber_optic_lights

/* 定义灯珠颜色，五选1*/
// #define LED_STRIP_R
// #define LED_STRIP_RG
// #define LED_STRIP_RGB
#define LED_STRIP_RGBW 1
// #define LED_STRIP_RGBCW

/* 定义灯珠通道,取值1~5*/
#define LED_STRIP_CH 3

/******************************************************************common*****************************************************************/

typedef struct _HSV_COLOUR_DAT
{
    u32 h_val;
    u32 s_val;
    u32 v_val;
} HSV_COLOUR_DAT;

typedef struct
{
    u16 h_val; // h的数值，色相 0~360
    u16 s_val; // s的数值，饱和度 0~360
    u16 v_val; // v的数值，亮度0~1000
} hsv_t;

typedef struct
{
#ifdef LED_STRIP_R
    u8 r;
#endif
#ifdef LED_STRIP_RG
    u8 r;
    u8 g;
#endif
#ifdef LED_STRIP_RGB
    u8 r;
    u8 g;
    u8 b;
#endif
#ifdef LED_STRIP_RGBW
    u8 r;
    u8 g;
    u8 b;
    u8 w;
#endif
#ifdef LED_STRIP_RGBCW
    u8 r;
    u8 g;
    u8 b;
    u8 c;
    u8 w;
#endif
} color_t;

typedef enum
{
    DEVICE_OFF, // 关机
    DEVICE_ON,  // 开机
} ON_OFF_FLAG;

typedef enum
{
    IS_AUTO,
    IS_PAUSE,
} _AUTO_T;

typedef enum
{
    PHONE_MIC,    // 手机麦克风
    EXTERIOR_MIC, // 外部麦克风
} MIC_TYPE_T;

typedef enum
{
    IR_TIMER_NO = 0, // 无定时
    IR_TIMER_30MIN = 30 * 60 * 1000,
    IR_TIMER_60MIN = 60 * 60 * 1000,
    IR_TIMER_90MIN = 90 * 60 * 1000,
    IR_TIMER_120MIN = 120 * 60 * 1000,
} AUTO_TIME_T;

void soft_turn_on_the_light(void);
void soft_turn_off_the_light(void);
void full_color_init(void);

void fan_gpio_init(void);

void open_fan(void);
void close_fan(void);
void count_down_run(void);
void time_clock_handler(void);

// extern ALARM_CLOCK alarm_clock[3];
// extern TIME_CLOCK time_clock; 

#endif
