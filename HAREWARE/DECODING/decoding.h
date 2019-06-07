#ifndef __DCOD_H
#define __DCOD_H

#include "Global.h"

#define DCOD_MS     (100)
#define DCOD_USx100 (100)
#define DCOD_USx10  (10)
#define DCOD_US (1)

//解码GPIO涉及
#define DCOD_RCC RCC_APB2Periph_GPIOA
#define DCOD_GPIO GPIOA
#define DCOD_PIN GPIO_Pin_1            //***

//解码时钟涉及
#define DCOD_TIM TIM5
#define DCOD_TIM_RCC RCC_APB1Periph_TIM5
#define DCOD_TIM_IRQn TIM5_IRQn
#define DCOD_CC_Channel TIM_Channel_2   //***
#define DCOD_IT_CC TIM_IT_CC2           //***
#define DCOD_ICPolarity_Bit 5           //***

//更新中断间隔设置
#define DCOD_TIM_InitMS     (DCOD_TIMInit(1999,35))
#define DCOD_TIM_InitUSx100 (DCOD_TIMInit(199,35))
#define DCOD_TIM_InitUSx10  (DCOD_TIMInit(19,35))
#define DCOD_TIM_InitUS     (DCOD_TIMInit(9,35))

//解码时间规定
#define DCOD_T_CAP    (timeCapture*DCOD_US)  //使DCOD_T_T的单位与timeCapture的单位统一
#define DCOD_T_T      (100)            //在/传输/同步状态下/使用
#define DCOD_T_2T     (DCOD_T_T*2)      //在/传输/同步状态下/使用
#define DCOD_T_START  (DCOD_T_T*3)      //在/同步状态下/该时间用来完成同步
#define DCOD_T_END    (DCOD_T_T*5)      //在/传输状态下/如果超过此时间未触发捕获中断视为停止传输
//#define DCOD_MKUP     (int)(DCOD_T_T/1.3+1)
#define DCOD_MKUP     (int)(DCOD_T_T/2+1)

//捕获时间是否处于规定时间内
#define DCOD_T_IFT     ((DCOD_T_CAP > DCOD_T_T-DCOD_MKUP) && (DCOD_T_CAP < DCOD_T_T+DCOD_MKUP))
#define DCOD_T_IF2T    ((DCOD_T_CAP > DCOD_T_2T-DCOD_MKUP) && (DCOD_T_CAP < DCOD_T_2T+DCOD_MKUP))
#define DCOD_T_IFSTART ((DCOD_T_CAP > DCOD_T_START-DCOD_MKUP) && (DCOD_T_CAP < DCOD_T_START+DCOD_MKUP))
#define DCOD_T_IF4T    ((DCOD_T_CAP > DCOD_T_START+DCOD_T_T-DCOD_MKUP) && (DCOD_T_CAP < DCOD_T_START+DCOD_T_T+DCOD_MKUP))
#define DCOD_T_IFEND   (DCOD_T_CAP > DCOD_T_END)

extern u8 DCOD_Error;
extern u8 DCOD_BUF[];  //接收数据缓存
#define DCOD_BUF_LEN (100) //宏定义缓冲区大小
extern u32 DCOD_BUF_STA;
//bit31:保留
//bit30:解码完成标志位
//bit29:解码上一位
//bit28://记录捕获时间等于半帧时的标志位
//bit27-24:bit位置记录
//bit23-0:byte位置记录/解码字节数


void DCOD_Init(void);  //初始化程序
void DCOD_End(void); //结束

#endif
