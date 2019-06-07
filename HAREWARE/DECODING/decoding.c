#include "decoding.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ��-STM32������
//������Ϊ���������еĽ������
//���¿Ƽ�����@isuoao
//�޸�����:2019/5/1
//�汾��V1.0							  
//////////////////////////////////////////////////////////////////////////////////	 

enum
{
	DCOD_WAIT,
	DCOD_SYNCING,
	DCOD_FULLRUN,
}DCOD_Status;     //��¼�����źŴ���״̬

u8 DCOD_BUF[DCOD_BUF_LEN] = {'\0'}; 
//�������ݻ�����(�ѽ���)

u32 DCOD_BUF_STA;
//bit31:����
//bit30:������ɱ�־λ
//bit29:������һλ
//bit28://��¼����ʱ����ڰ�֡ʱ�ı�־λ
//bit27-24:bitλ�ü�¼
//bit23-0:byteλ�ü�¼/�����ֽ���

u32 timeCapture = 0; //������Ҫ,��¼���β�������ϴβ����ʱ��
u8 ITRPT_Flag = FALSE;

//==============================================================
//���������������źŵ�GPIO��ʼ��
//��������
//���أ���
//==============================================================
void DCOD_GPIOInit()
{
	GPIO_InitTypeDef GPIO_InitStruc;
	RCC_APB2PeriphClockCmd(DCOD_RCC,ENABLE);
	GPIO_InitStruc.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruc.GPIO_Pin = DCOD_PIN;
	GPIO_InitStruc.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DCOD_GPIO,&GPIO_InitStruc);
	GPIO_ResetBits(DCOD_GPIO,DCOD_PIN);
}

//==============================================================
//������������������ʱ�ӵĳ�ʼ��
//������arr,psc->��װ��ֵ,Ԥ��Ƶ
//���أ���
//==============================================================
void DCOD_TIMInit(u32 arr, u32 psc)
{
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(DCOD_TIM_RCC,ENABLE);
	TIM_InitStructure.TIM_Prescaler = psc;
	TIM_InitStructure.TIM_Period  = arr;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(DCOD_TIM,&TIM_InitStructure);
	
	TIM_ICInitStructure.TIM_Channel = DCOD_CC_Channel; 
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; 
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; 
	TIM_ICInitStructure.TIM_ICFilter = 0x00;
	TIM_ICInit(DCOD_TIM, &TIM_ICInitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DCOD_TIM_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig(DCOD_TIM,TIM_IT_Update|DCOD_IT_CC,ENABLE);
	TIM_Cmd(DCOD_TIM,ENABLE); 
}
//==============================================================
//������������������ʼ��(�������)
//������arr,psc->��װ��ֵ,Ԥ��Ƶ
//���أ���
//==============================================================
void DCOD_Init(void)
{
	DCOD_GPIOInit();
	DCOD_TIM_InitUS; //��ʼ��ʱ�Ӳ�ָ�������жϵĴ���ʱ��
}

void DCOD_End(void)
{
	TIM_Cmd(DCOD_TIM,DISABLE);
	TIM_ITConfig(DCOD_TIM,TIM_IT_Update|DCOD_IT_CC,DISABLE);
}

//==============================================================
//�������������ݻ�������ʼ��
//��������
//���أ���
//==============================================================
void DCOD_CharInit(void)
{
	DCOD_BUF_STA = 0;
}
//==============================================================
//����������bit���ճ���
//������bit->0/1
//���أ�TURE/FALSE
//==============================================================
u8 DCOD_Receive(int bit)
{
	#define bitNum     ((DCOD_BUF_STA>>24)&0xf)
	#define bitNumInit (DCOD_BUF_STA &= ~(0xf<<24))
	#define bitNumINC  (DCOD_BUF_STA += 1<<24)
	#define charNum    (DCOD_BUF_STA&(0Xffffff))
	#define charNumINC (DCOD_BUF_STA++)
	if(1 == bit)
	{
		DCOD_BUF[charNum] |= (1<<bitNum);
	}
	else if(0 == bit)
	{
		DCOD_BUF[charNum] &= ~(1<<bitNum);
	}
	else
	{
		return FALSE;
	}
	bitNumINC;
	if(bitNum > 7)
	{
		bitNumInit;
		charNumINC;
	}
	return TRUE;
}
//==============================================================
//����������DCOD_�жϷ������
//��������
//���أ���
//==============================================================
u8 DCOD_Error = 0;
void TIM5_IRQHandler(void)
{
	#define bit (DCOD_BUF_STA&(1<<29))>>29
	#define GpioLevel GPIO_ReadInputDataBit(DCOD_GPIO,DCOD_PIN)
	static int  tim5_cctemp = 0;
	u8  receive_temp = FALSE;
	if(TIM_GetITStatus(TIM5,TIM_IT_Update) != RESET)
	{
		timeCapture++;
		if(DCOD_FULLRUN == DCOD_Status && DCOD_T_IFEND)
		{//�����涨ʱ�䲻����������Ϊ��������
			DCOD_Status = DCOD_WAIT;
			DCOD_BUF[DCOD_BUF_STA&0xffffff] = '\0';
			DCOD_BUF_STA |= (1<<30);
			printf("n%d\r\n",tim5_cctemp);
			tim5_cctemp = 0;
		}
		else if(DCOD_SYNCING == DCOD_Status && DCOD_T_IFEND)
		{
			DCOD_Status = DCOD_WAIT;
			DCOD_BUF[DCOD_BUF_STA&0xffffff] = '\0';
			DCOD_Error = 1;
//			DCOD_BUF_STA |= (1<<31);
			printf("e%d\r\n",tim5_cctemp);
			tim5_cctemp = 0;
		}
	}
	if(TIM_GetITStatus(TIM5,DCOD_IT_CC) != RESET)
	{ //���������ж�
		tim5_cctemp++;
		ITRPT_Flag = GpioLevel;
		TIM_ITConfig(DCOD_TIM,DCOD_IT_CC,DISABLE);
		delay_us(50);
		if(ITRPT_Flag == GpioLevel)
		{
//			printf("%d\r\n",timeCapture);
			if(!LED1_State)LED1_On;
			else LED1_Off;
			if(DCOD_FULLRUN == DCOD_Status)
			{//����
				if(DCOD_T_IF2T)
				{//ÿ��һ��֡λ���񴥷�ʱ/����bit���ϴ�bit���෴��
					DCOD_BUF_STA ^= (1<<29);
					receive_temp = DCOD_Receive(bit);
				}
				else if(DCOD_T_IFT)
				{//ÿ��һ����֡λ���񴥷�ʱ���������/���´δ����ж�Ҳ�ǰ�֡λʱ/����bit�����ϴ�bit
					if(DCOD_BUF_STA&(1<<28))
					{
						receive_temp = DCOD_Receive(bit);
						DCOD_BUF_STA &= ~(1<<28);
					}
					else
					{
						DCOD_BUF_STA |= (1<<28);
					}
				}
			}
			else if(DCOD_WAIT == DCOD_Status)
			{//�ж��Ƿ��ڷǴ���״̬/�����ִ���������
				if(GpioLevel && DCOD_T_IFEND)
				{//�ж��Ƿ�ߵ�ƽ
					DCOD_Status = DCOD_SYNCING; //����ͬ��״̬
					DCOD_CharInit(); //��BUF���г�ʼ��
				}
			}
			else if(DCOD_SYNCING == DCOD_Status)
			{//ͬ��
				if( !GpioLevel && DCOD_T_IFSTART)
				{//��ʼλ�ߵ�ƽͬ�����/�ȴ��͵�ƽ��ͬ��
				}
				else if(GpioLevel && DCOD_T_IF4T)
				{//��֡λͬ�����/�������
					DCOD_BUF_STA |= (1<<29); //��һλ������1
					receive_temp = DCOD_Receive(bit);
				}
				else if(GpioLevel && DCOD_T_IFSTART)
				{//����֡λ/����һ���жϴ���Ϊ��֡λʱ��֮������ɰ�֡λͬ��
				}
				else if(!GpioLevel && DCOD_T_IFT)
				{//��֡λͬ�����/�������
					DCOD_BUF_STA &= ~(1<<29); //��һλ������0
					receive_temp = DCOD_Receive(bit);
				}
				else
				{ //���ƥ����������ȴ�
					DCOD_Status = DCOD_WAIT;
				}
				if(receive_temp) DCOD_Status = DCOD_FULLRUN; //��һλ���ݴ�����ɺ����ȫ��ͬ��״̬
			}
			timeCapture = 0;//ÿ�δ��������жϺ����㲶��ʱ��
			//�Բ��񴥷����ؽ��и���/��֤ÿ�����ض����Ա�����
			if(TIM5->CCER & (1<<DCOD_ICPolarity_Bit))
				TIM5->CCER &= ~(1<<DCOD_ICPolarity_Bit);
			else
				TIM5->CCER |= (1<<DCOD_ICPolarity_Bit);
		}
		TIM_ITConfig(DCOD_TIM,DCOD_IT_CC,ENABLE);
	}
	TIM_ClearITPendingBit(TIM5, DCOD_IT_CC|TIM_IT_Update); //���´��ж�
}
	
