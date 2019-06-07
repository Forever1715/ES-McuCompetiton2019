#ifndef _ECOD_H
#define _ECOD_H
#include "Global.h"

//�����궨��
#define ECOD_GPIO_RCC RCC_APB2Periph_GPIOB
#define ECOD_GPIO GPIOB
#define ECOD_GPIO_PIN  GPIO_Pin_5
#define ECOD_Line_High GPIO_SetBits(ECOD_GPIO,ECOD_GPIO_PIN);
#define ECOD_Line_Low GPIO_ResetBits(ECOD_GPIO,ECOD_GPIO_PIN);

//�ı������ֵ�ɸ��Ĵ����ٶ�
#define ECOD_T (100)
#define ECOD_2T (ECOD_T*2)
#define ECOD_Start_T (ECOD_T*3)
#define ECOD_End_T (ECOD_T*3)


//�ɶ�����ú���
void ECOD_Init(void);  //�������
void ECOD_End(void);
u8 ECOD_Send_Str(u8 *str); //strָ��������

//void ECOD_Send_Char(u8 chr);

#endif
