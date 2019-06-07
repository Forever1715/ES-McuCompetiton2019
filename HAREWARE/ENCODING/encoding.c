#include "encoding.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用-STM32开发板
//本程序为比赛过程中的编码程序
//索奥科技中心@isuoao
//修改日期:2019/5/1
//版本：V1.0							  
//////////////////////////////////////////////////////////////////////////////////	 

u8 ECOD_Switch = TRUE;

//==============================================================
//功能描述：编码引脚初始化
//参数：无
//返回：无
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
//功能描述：编码初始化(必须调用)
//参数：无
//返回：无
//==============================================================
void ECOD_Init()
{
	delay_init();
	ECOD_GPIOInit();
	ECOD_Switch = TRUE;
}
//==============================================================
//功能描述：结束并禁止编码
//参数：无
//返回：无
//==============================================================
void ECOD_End()
{
	ECOD_Switch = FALSE;
}
//==============================================================
//功能描述：位编码发送
//参数：位bit
//返回：TRUE
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
//功能描述：字节发送
//参数：u8要发送的字节
//返回：无
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
//功能描述：字符串发送(不可发送'\0')
//参数：指向字符串的指针
//返回：TRUE/FALSE
//==============================================================
u8 ECOD_Send_Str(u8 *str)
{
	if(ECOD_Switch)
	{
		u8 *p_str = str;
		ECOD_Line_High;
		delay_us(ECOD_Start_T); //起始位
		ECOD_Line_Low;
		delay_us(ECOD_Start_T); //进行同步
		while(*p_str != '\0')
		{
			ECOD_Send_Char(*p_str); //逐字节发送
			p_str++;
		}
		ECOD_Line_Low;//发送完后拉低信号
  }
	else
	{
		return FALSE;
	}
	return TRUE;
}
