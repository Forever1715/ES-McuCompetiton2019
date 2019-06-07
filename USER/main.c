#include "Global.h"

/************************************************
 //���ĵ�Ϊmian
 SuoAo �ڰ˽쵥Ƭ���������꼶���ź���
 ��Ŀ˵�������⹲���������̣��ֱ�Ϊ����˹��̺ͽ��ն˹���
 ����˵����������Ϊ���ն˹���
 �����ˣ���ǿ������ƽ
 If Chinses can't corrent display,
 please modify the Encoding Method to ANSI or GB2312(Simplified) 
 ***********************************************/

int main()
{
	u8 key; 
	u8 send = 0x00;
	uart_init(115200);
	delay_init();
	KEY_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
	LCD_Init();
	while(1)
	{
		LCD_Print(0,0,"Selcet Mode",TYPE16X16,TYPE8X16);
		LCD_Print(0,16,"KEY2:Both",TYPE16X16,TYPE8X16);
		LCD_Print(0,32,"KEY1:Sender",TYPE16X16,TYPE8X16);
		LCD_Print(0,48,"KEY0:Receiver",TYPE16X16,TYPE8X16);
		while(1)
		{
			key = KEY_Scan(0);
			if(key)
			{
				switch(key)
				{
					case KEY0_PRES:
					{
						//�˴�д��KEY0������ʱ��Ҫ��������
						LCD_CLS();
						DCOD_Init();
						LCD_Print(0,0,"Receiver",TYPE16X16,TYPE8X16);
						break;
					}
					case KEY1_PRES:
					{
						//�˴�д��KEY1������ʱ��Ҫ��������
						LCD_CLS();
						ECOD_Init();
						LCD_Print(0,0,"Sender",TYPE16X16,TYPE8X16);
						break;
					}
					case KEY2_PRES:
					{
						//�˴�д��KEY2������ʱ��Ҫ��������
						LCD_CLS();
						DCOD_Init();	
						ECOD_Init();
						LCD_Print(0,0,"Receiver&Sender",TYPE16X16,TYPE8X16);
						break;
					}
				}//switch(key)
				if(key != WK_UP && key != 0)break;
				key = 0;
			}//if(key)
		}//while(1)
		LCD_Print(0,56,"WK_UP To Exit!",TYPE16X16,TYPE6X8);
		while(1)
		{
			//ECOD_Send_Str(&send);
			if(KEY_Scan(0) == WKUP_PRES) break;
			if(USART_RX_STA & (1<<15))
			{
				ECOD_Send_Str(USART_RX_BUF);
				USART_RX_STA =0;
			}
			if(DCOD_BUF_STA & (1<<30))
			{
				printf("���յ��ַ�����%s\r\n",DCOD_BUF);
				LCD_CLS_ny(16,16);
				LCD_Print(0,16,DCOD_BUF,TYPE16X16,TYPE8X16);
				DCOD_BUF_STA = 0;
			}

		}
		DCOD_End(); //�ر�
		ECOD_End(); //�ر�
	}
}
