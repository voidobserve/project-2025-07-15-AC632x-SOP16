// 中道单线通讯
//控制步进电机
#include "system/includes.h"
#include "one_wire.h"
#include "led_strand_effect.h"

u16 send_base_ins = 0;
u8 motor_period[6] = {8, 13, 18, 21, 26, 35};  //转速  app指令，需要将8 13 18 21 26 转换成相应的16进制

/**
 * @brief  mcu通讯接口
 * 
 * 
 */
void mcu_com_init(void)
{

	gpio_set_die(IO_PORTA_00, 1);         
    gpio_set_pull_up(IO_PORTA_00,1);
	gpio_direction_output(IO_PORTA_00, 1);
    
}


/**
 * @brief 打包基本数据  数据包
 * 
 */
void pack_base(void)
{
    u8 p;
    send_base_ins = 0;
    send_base_ins |= fc_effect.base_ins.mode;

    for(p = 0; p < 6; p++)
    {
        if(motor_period[p] == fc_effect.base_ins.period)
        {
            break;
        }
    }
    if(p > 5) p=0;
    send_base_ins |= p<<3;

    if(fc_effect.base_ins.dir)    send_base_ins |= BIT(6);
   
}

#define INS_LEN     7  //指令长度 
#define W_0_5MS     4   //脉宽0.5ms
#define W_1MS       8
#define W_2MS       16
u8 send_cnt = 0; 
u8 step = 0;
u8 _125ms_cnt = 0;  //125us
u8 h_l = 0; //0:输出低电平，1：高电平
u8 send_en = 0;     //0:不发送， 1：发送   使能变量


/**
 * @brief 构造单线通讯协议，16bit  构造波形
 * 
 * @param dat 
 */
void __attribute__((weak)) make_one_wire(u16 dat )
{
    static u16 count_ = 0;
    if(send_en == 0) {
        count_ = 0;
        return;
    }
    switch(step)
    {
        case 0: //起始位
        /***********************************************************/
        //解决了app发送指令的，波形不正确的问题，但是问题愿意未清晰
            if (count_ <= 40)  //2.5ms
            {
                count_++;
                return;
            }
        /**********************************************************/    
            if(h_l == 0)
            {
                gpio_direction_output(IO_PORTA_00, 0);
               _125ms_cnt++;
          
                //++_125ms_cnt;
                if(_125ms_cnt > (W_1MS))  //从1开始
                {
                    gpio_direction_output(IO_PORTA_00, 1);
                    
                    h_l = 1;
                    _125ms_cnt = 0;
                    
                }
            }
            else
            {
                // gpio_direction_output(IO_PORTA_00, 1);
                _125ms_cnt++;
                
                if(_125ms_cnt == W_1MS)
                { 
                    gpio_direction_output(IO_PORTA_00, 0);
                    h_l = 0;
                    step = 1;
                    _125ms_cnt = 0;
                }
            }
            break; 

        case 1:
            if(h_l == 0)
            {
                // gpio_direction_output(IO_PORTA_00, 0);
                _125ms_cnt++;
            
              
                if(_125ms_cnt == W_0_5MS)
                {
                   
                    gpio_direction_output(IO_PORTA_00, 1);
                     h_l = 1;
                    _125ms_cnt = 0;
                }
            }
            else
            {
                if( (dat >> send_cnt) & 0x01)   //1
                {
                    // gpio_direction_output(IO_PORTA_00, 1);
                    _125ms_cnt++;
                    
                    if(_125ms_cnt == W_1MS)
                    {
                        gpio_direction_output(IO_PORTA_00, 0);

                        h_l = 0;
                        _125ms_cnt = 0;
                        // 完成1bit发送
                        send_cnt++;
                        if(send_cnt == INS_LEN)
                        {
                            send_cnt = 0;
                            step = 2;
                        }
                    }
                }
                else
                {
                    // gpio_direction_output(IO_PORTA_00, 1);
                    _125ms_cnt++;              
                    if(_125ms_cnt == W_0_5MS)
                    {
                        gpio_direction_output(IO_PORTA_00, 0);
                        h_l = 0;
                        _125ms_cnt = 0;
                        // 完成1bit发送
                        send_cnt++;
                        if(send_cnt == INS_LEN)
                        {
                            send_cnt = 0;
                            step = 2;
                        }
                    }
                }
            }
            
            break;

        case 2:
            if(h_l == 0)
            {
                gpio_direction_output(IO_PORTA_00, 0);
                _125ms_cnt++;            
                if(_125ms_cnt == W_2MS)
                {
                    gpio_direction_output(IO_PORTA_00, 1);
                   
                    _125ms_cnt = 0;
                    step = 0;
                    send_cnt = 0;
                    send_en = 0;    //等待下一次触发
                }
            }
            break;
    }
    //  gpio_direction_output(IO_PORTA_01, 0);
}

void enable_one_wire(void)   //数据发送使能
{
    pack_base();   //打包数据

    send_en = 1;
    
    
}




/**
 * @brief 125ms调用一次  放在定时器使用
 * 
 */
// AT_VOLATILE_RAM_CODE
void one_wire_send(void)
{
    
    // pack_base();
    make_one_wire(send_base_ins);
    
    
}




// -------------------------------------------------------------------API
/**
 * @brief 设置电机的模式
 * 
 * @param m       // 000:回正
                 // 001:区域1摇摆
                // 010:区域2摇摆
                // 011:区域1和区域2摇摆
                // 100:360°正转
               // 101:音乐律动
               //110：停止
 */

void one_wire_set_mode(u8 m)
{
    fc_effect.base_ins.mode = m;
    printf("base_ins.mode = %d", fc_effect.base_ins.mode);
}
/**
 * @brief 设置电机转速
 * 
 * @param p ：8s 13s 18s 21s 26s
 */
void one_wire_set_period(u8 p)
{
   if( fc_effect.base_ins.mode == 0x05) return;
    fc_effect.base_ins.period = p;
    printf("base_ins.period = %d", fc_effect.base_ins.period);
}
/**
 * @brief 设置电机正反转
 * 
 */
void one_wire_set_dir(void)
{
    printf("base_ins.dir = %d", fc_effect.base_ins.dir);
    fc_effect.base_ins.dir = !fc_effect.base_ins.dir;
  
}
/**
 * @brief Get the stepmotor mode object
 * 获取电机当前模式
 * 
 * @return u8 
 */
u8 get_stepmotor_mode(void)
{
    printf(" base_ins.mode = %d",  fc_effect.base_ins.mode);
    return fc_effect.base_ins.mode;
}



/*************************************音乐律动模式**************************************/

/**
 * @brief 效果：反转
 * 
 */
void stepmotor_direction(void)
{
    // one_wire_set_dir();
    fc_effect.base_ins.dir = 1;
}

/**
 * @brief 效果：调到最慢速度
 * 
 */
void stepmotor_music_minSpeed(void)
{

    fc_effect.base_ins.dir = 0;
    fc_effect.base_ins.period = 26;  
}

/**
 * @brief 效果：调到最快速度
 * 
 */
void stepmotor_music_maxSpeed(void)
{
    fc_effect.base_ins.dir = 0;
   fc_effect.base_ins.period = 8;

}

void set_stepmotor_music_mode(void)
{

  
    switch(fc_effect.base_ins.music_mode)
    {
        case 0:
   
            stepmotor_direction();
            enable_one_wire();  //使用发送数据
        break;
        case 1:

            stepmotor_music_minSpeed();
            enable_one_wire();  //使用发送数据
        break;
        case 2:
   
            stepmotor_music_maxSpeed();
            enable_one_wire();  //使用发送数据
        break;
    }
  
   
}



/**
 * @brief 设置是最慢
 * 
 */
void set_stepmotor_slow(void)
{
    if(fc_effect.base_ins.period != 26)
    {
        stepmotor_music_minSpeed();
        enable_one_wire();  //使用发送数据
    }

}

/**
 * @brief 设置是最快
 * 
 */
void set_stepmotor_fast(void)
{
    if(fc_effect.base_ins.period != 8)
    {
        stepmotor_music_maxSpeed();
        enable_one_wire();  //使用发送数据
    }

}




/**
 * @brief 声控步进电机
 * 
 */

u8 stepmotor_sound_cnt = 0;

void effect_stepmotor(void)
{

    if(fc_effect.base_ins.mode == 0x05)
    {

        if(get_sound_result())  
        {
            set_stepmotor_fast();
           printf("1111\n");
            stepmotor_sound_cnt = 0;
        }

        if(stepmotor_sound_cnt < 100)
        {
            stepmotor_sound_cnt++;
           
        }
        if(stepmotor_sound_cnt >= 100)
        {
             printf("2222\n");
            set_stepmotor_slow();
        }

    }

}


u8 counting_flag = 0;  //1：计时中， 0：计时完成
u16 stop_cnt = 0;
u8 set_time = 0; //1：不允许修改时间  0：允许修改时间
u8 temp = 0;
void clean_stepmorot_flag(void)
{
    counting_flag = 0;
    stop_cnt = 0;
    set_time = 1;
}
//10计时
void stepmotor(void)
{
  
    if(set_time == 1)
    {
       
        set_time = 0;
        temp = fc_effect.base_ins.period;
        if(fc_effect.on_off_flag == DEVICE_OFF)
         temp = 0;
       
    }
    if(counting_flag == 1)
    {
       
        if(stop_cnt == temp * 100)
        {
         
            one_wire_set_mode(6);
            enable_one_wire();
            clean_stepmorot_flag();
        }
        else{
            stop_cnt++;
        }
       


    }
   
}


void motor_Init(void)
{
    counting_flag = 0;        //无霍尔时，电机
    set_time = 1;

}


