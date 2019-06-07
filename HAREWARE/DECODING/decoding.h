#ifndef __DCOD_H
#define __DCOD_H

#include "Global.h"

#define DCOD_MS     (100)
#define DCOD_USx100 (100)
#define DCOD_USx10  (10)
#define DCOD_US (1)

//����GPIO�漰
#define DCOD_RCC RCC_APB2Periph_GPIOA
#define DCOD_GPIO GPIOA
#define DCOD_PIN GPIO_Pin_1            //***

//����ʱ���漰
#define DCOD_TIM TIM5
#define DCOD_TIM_RCC RCC_APB1Periph_TIM5
#define DCOD_TIM_IRQn TIM5_IRQn
#define DCOD_CC_Channel TIM_Channel_2   //***
#define DCOD_IT_CC TIM_IT_CC2           //***
#define DCOD_ICPolarity_Bit 5           //***

//�����жϼ������
#define DCOD_TIM_InitMS     (DCOD_TIMInit(1999,35))
#define DCOD_TIM_InitUSx100 (DCOD_TIMInit(199,35))
#define DCOD_TIM_InitUSx10  (DCOD_TIMInit(19,35))
#define DCOD_TIM_InitUS     (DCOD_TIMInit(9,35))

//����ʱ��涨
#define DCOD_T_CAP    (timeCapture*DCOD_US)  //ʹDCOD_T_T�ĵ�λ��timeCapture�ĵ�λͳһ
#define DCOD_T_T      (100)            //��/����/ͬ��״̬��/ʹ��
#define DCOD_T_2T     (DCOD_T_T*2)      //��/����/ͬ��״̬��/ʹ��
#define DCOD_T_START  (DCOD_T_T*3)      //��/ͬ��״̬��/��ʱ���������ͬ��
#define DCOD_T_END    (DCOD_T_T*5)      //��/����״̬��/���������ʱ��δ���������ж���Ϊֹͣ����
//#define DCOD_MKUP     (int)(DCOD_T_T/1.3+1)
#define DCOD_MKUP     (int)(DCOD_T_T/2+1)

//����ʱ���Ƿ��ڹ涨ʱ����
#define DCOD_T_IFT     ((DCOD_T_CAP > DCOD_T_T-DCOD_MKUP) && (DCOD_T_CAP < DCOD_T_T+DCOD_MKUP))
#define DCOD_T_IF2T    ((DCOD_T_CAP > DCOD_T_2T-DCOD_MKUP) && (DCOD_T_CAP < DCOD_T_2T+DCOD_MKUP))
#define DCOD_T_IFSTART ((DCOD_T_CAP > DCOD_T_START-DCOD_MKUP) && (DCOD_T_CAP < DCOD_T_START+DCOD_MKUP))
#define DCOD_T_IF4T    ((DCOD_T_CAP > DCOD_T_START+DCOD_T_T-DCOD_MKUP) && (DCOD_T_CAP < DCOD_T_START+DCOD_T_T+DCOD_MKUP))
#define DCOD_T_IFEND   (DCOD_T_CAP > DCOD_T_END)

extern u8 DCOD_Error;
extern u8 DCOD_BUF[];  //�������ݻ���
#define DCOD_BUF_LEN (100) //�궨�建������С
extern u32 DCOD_BUF_STA;
//bit31:����
//bit30:������ɱ�־λ
//bit29:������һλ
//bit28://��¼����ʱ����ڰ�֡ʱ�ı�־λ
//bit27-24:bitλ�ü�¼
//bit23-0:byteλ�ü�¼/�����ֽ���


void DCOD_Init(void);  //��ʼ������
void DCOD_End(void); //����

#endif
