#include "encoding.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ��-STM32������
//������Ϊ���������еı������
//���¿Ƽ�����@isuoao
//�޸�����:2019/5/1
//�汾��V1.0							  
//////////////////////////////////////////////////////////////////////////////////	 

u8 ECOD_Switch = TRUE;

//==============================================================
//�����������������ų�ʼ��
//��������
//���أ���
//==============================================================
void ECOD_GPIOInit()
{
	GPIO_InitTypeDef GPIO_InitStruc;
	RCC_APB2PeriphClockCmd(ECOD_GPIO_RCC,ENABLE);
	GPIO_InitStruc.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruc.GPIO_Pin = ECOD_GPIO_PIN;
	GPIO_InitStruc.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ECOD_GPIO,&GPIO_InitStruc);
	ECOD_Line_Low;
}
//==============================================================
//���������������ʼ��(�������)
//��������
//���أ���
//==============================================================
void ECOD_Init()
{
	delay_init();
	ECOD_GPIOInit();
	ECOD_Switch = TRUE;
}
//==============================================================
//������������������ֹ����
//��������
//���أ���
//==============================================================
void ECOD_End()
{
	ECOD_Switch = FALSE;
}
//==============================================================
//����������λ���뷢��
//������λbit
//���أ�TRUE
//==============================================================
u8 ECOD_Send(u8 bit)
{
	if(bit)
	{
		ECOD_Line_Low;
		delay_us(ECOD_T);
		ECOD_Line_High;
		delay_us(ECOD_T);
	}
	else
	{
		ECOD_Line_High;
		delay_us(ECOD_T);
		ECOD_Line_Low;
		delay_us(ECOD_T);
	}
	return TRUE;
}
//==============================================================
//�����������ֽڷ���
//������u8Ҫ���͵��ֽ�
//���أ���
//==============================================================
void ECOD_Send_Char(u8 chr)
{
	int i;
	for(i=0; i<8; i++)
	{
		if(chr&(1<<i)) ECOD_Send(1);
		else ECOD_Send(0);
	}
}
//==============================================================
//�����������ַ�������(���ɷ���'\0')
//������ָ���ַ�����ָ��
//���أ�TRUE/FALSE
//==============================================================
u8 ECOD_Send_Str(u8 *str)
{
	if(ECOD_Switch)
	{
		u8 *p_str = str;
		ECOD_Line_High;
		delay_us(ECOD_Start_T); //��ʼλ
		ECOD_Line_Low;
		delay_us(ECOD_Start_T); //����ͬ��
		while(*p_str != '\0')
		{
			ECOD_Send_Char(*p_str); //���ֽڷ���
			p_str++;
		}
		ECOD_Line_Low;//������������ź�
  }
	else
	{
		return FALSE;
	}
	return TRUE;
}
