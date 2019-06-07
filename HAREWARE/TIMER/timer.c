#include "timer.h"
//==============================================================
//����������Timer2 Channel3��ʼ����PWMģʽ
//������//������TOPֵ,Ԥ��Ƶ (ARR,PSC)
//���أ���
//==============================================================
void TIM2_PWM_Init(u16 period, u16 prescaler)  //������TOPֵ,Ԥ��Ƶ (ARR,PSC)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitTypeStruct;
	TIM_OCInitTypeDef TIM_OCInitTypeStruct;
	
	//������ӳ��
	GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2,ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʹ�ܶ�ʱ��5ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
		
	//ʹ�ܶ˿�PB10
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //GPIOB PB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//��ʼ��TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//TIM2ʱ��ʹ��
	TIM_TimeBaseInitTypeStruct.TIM_Period = period;
	TIM_TimeBaseInitTypeStruct.TIM_Prescaler = prescaler;
	TIM_TimeBaseInitTypeStruct.TIM_CounterMode = TIM_CounterMode_Up; //���м���
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitTypeStruct); //TIM2��ʼ��
	
	//��ʼ��TIM2 Channel3 PWM2
	TIM_OCInitTypeStruct.TIM_OCMode = TIM_OCMode_PWM2; //CNT>CCR��Ч
	TIM_OCInitTypeStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitTypeStruct.TIM_OCPolarity = TIM_OCPolarity_Low;  //������Ե͵�ƽ
	TIM_OC3Init(TIM2,&TIM_OCInitTypeStruct);
	
	//ʹ��TIM2 Channel3Ԥװ�ؼĴ���
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_Cmd(TIM2,ENABLE);
}


//==============================================================
//����������Timer3��ʼ������ͨģʽ
//������//������TOPֵ,Ԥ��Ƶ (ARR,PSC)
//���أ���
//==============================================================
void TIM3_Normal_Init(u16 period, u16 prescaler)  //������TOPֵ,Ԥ��Ƶ (ARR,PSC)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitTypeStruct;
	NVIC_InitTypeDef NVIC_InitTypeStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//TIM3ʱ��ʹ��
	TIM_TimeBaseInitTypeStruct.TIM_Period = period;
	TIM_TimeBaseInitTypeStruct.TIM_Prescaler = prescaler;
	TIM_TimeBaseInitTypeStruct.TIM_CounterMode = TIM_CounterMode_Up; //���м���
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitTypeStruct); //TIM3��ʼ��
	
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //�����ж�(���)ʹ�� TimeOut = (perid+1)(prescaler+1)/Tclk 
	NVIC_InitTypeStruct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitTypeStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitTypeStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitTypeStruct.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitTypeStruct); //���ȼ�����
	
	TIM_Cmd(TIM3,ENABLE);
}

//==============================================================
//����������Timer3 Channel2��ʼ����PWMģʽ
//������//������TOPֵ,Ԥ��Ƶ (ARR,PSC)
//���أ���
//ע�⣺ʹ��TIM_SetCompare2(TIM3,6000);�ı�ռ�ձ�
//==============================================================
void TIM3_PWM_Init(u16 period, u16 prescaler)  //������TOPֵ,Ԥ��Ƶ (ARR,PSC)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitTypeStruct;
	TIM_OCInitTypeDef TIM_OCInitTypeStruct;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʹ�ܶ�ʱ��3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	//������ӳ��
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
	
	//ʹ�ܶ˿�PB5
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //GPIOB PB5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//��ʼ��TIM3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//TIM3ʱ��ʹ��
	TIM_TimeBaseInitTypeStruct.TIM_Period = period;
	TIM_TimeBaseInitTypeStruct.TIM_Prescaler = prescaler;
	TIM_TimeBaseInitTypeStruct.TIM_CounterMode = TIM_CounterMode_Up; //���м���
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitTypeStruct); //TIM3��ʼ��
	
	//��ʼ��TIM3 Channel2 PWM2
	TIM_OCInitTypeStruct.TIM_OCMode = TIM_OCMode_PWM2; //CNT>CCR��Ч
	TIM_OCInitTypeStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitTypeStruct.TIM_OCPolarity = TIM_OCPolarity_High;  //������Ըߵ�ƽ
	TIM_OC2Init(TIM3,&TIM_OCInitTypeStruct);
	
	//ʹ��TIM3 Channel2Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_Cmd(TIM3,ENABLE);
}



//==============================================================
//����������Timer4 Channel1-2��ʼ����PWMģʽ
//������//������TOPֵ,Ԥ��Ƶ (ARR,PSC)
//���أ���
//==============================================================
void TIM5_PWM_Init(u16 period, u16 prescaler)  //������TOPֵ,Ԥ��Ƶ (ARR,PSC)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitTypeStruct;
	TIM_OCInitTypeDef TIM_OCInitTypeStruct;
	
	//ʹ��RCCʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //ʹ�ܶ�ʱ��5ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//������ӳ��
	//GPIO_PinRemapConfig(GPIO_Remap_TIM5,ENABLE);
	
	//ʹ�ܶ˿�PA2 PA3
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //GPIOA PA2 PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//��ʼ��TIM5
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);//TIM5ʱ��ʹ��
	TIM_TimeBaseInitTypeStruct.TIM_Period = period;
	TIM_TimeBaseInitTypeStruct.TIM_Prescaler = prescaler;
	TIM_TimeBaseInitTypeStruct.TIM_CounterMode = TIM_CounterMode_Up; //���м���
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitTypeStruct); //TIM5��ʼ��
	
	//��ʼ��TIM5 Channel3/Channel4 PWM2
	TIM_OCInitTypeStruct.TIM_OCMode = TIM_OCMode_PWM2; //CNT>CCR��Ч
	TIM_OCInitTypeStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitTypeStruct.TIM_OCPolarity = TIM_OCPolarity_Low;  //������Ե͵�ƽ
	TIM_OC3Init(TIM5,&TIM_OCInitTypeStruct);
	TIM_OC4Init(TIM5,&TIM_OCInitTypeStruct);
	
	//ʹ��TIM5 Channel3/Channel4 Ԥװ�ؼĴ���
	TIM_OC3PreloadConfig(TIM5,TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM5,TIM_OCPreload_Enable);
	TIM_Cmd(TIM5,ENABLE);
}

//==============================================================
//����������ͨ�ö�ʱ��7�жϳ�ʼ��
//         Tout=((period+1)*(prescaler+1))/Ft us
//������//������TOPֵ,Ԥ��Ƶ (ARR,PSC)
//���أ���
//==============================================================
void TIM7_Int_Init(u16 period,u16 prescaler)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//TIM7ʱ��ʹ��    
	
	//��ʱ��TIM7��ʼ��
	TIM_TimeBaseStructure.TIM_Period = period; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =prescaler; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM7�ж�,��������ж�
	
	TIM_Cmd(TIM7,ENABLE);//������ʱ��7
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}

extern vu16 USART3_RX_STA;


//==============================================================
//����������Timer7�жϷ�����
//��������
//���أ���
//==============================================================
void TIM7_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	 			   
		USART3_RX_STA|=1<<15;	//��ǽ������
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //���TIM7�����жϱ�־    
		TIM_Cmd(TIM7, DISABLE);  //�ر�TIM7 
	}
}

//==============================================================
//����������Timer3�жϷ�����
//��������
//���أ���
//==============================================================
void TIM3_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update))
	{
		/*Code*/
		if(LED0_State)
			LED0_Off;
		else
			LED0_On;
		/*End*/
		
		
		TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	}
}
