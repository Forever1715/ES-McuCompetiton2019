#ifndef _ECOD_H
#define _ECOD_H
#include "Global.h"

//基本宏定义
#define ECOD_GPIO_RCC RCC_APB2Periph_GPIOB
#define ECOD_GPIO GPIOB
#define ECOD_GPIO_PIN  GPIO_Pin_5
#define ECOD_Line_High GPIO_SetBits(ECOD_GPIO,ECOD_GPIO_PIN);
#define ECOD_Line_Low GPIO_ResetBits(ECOD_GPIO,ECOD_GPIO_PIN);

//改变下面的值可更改传输速度
#define ECOD_T (100)
#define ECOD_2T (ECOD_T*2)
#define ECOD_Start_T (ECOD_T*3)
#define ECOD_End_T (ECOD_T*3)


//可对外调用函数
void ECOD_Init(void);  //必须调用
void ECOD_End(void);
u8 ECOD_Send_Str(u8 *str); //str指向发送数据

//void ECOD_Send_Char(u8 chr);

#endif
