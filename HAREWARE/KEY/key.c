#include "key.h"
//==============================================================
//����������������ʼ����ʹ��ǰ������г�ʼ��
//��������
//���أ���
//==============================================================
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	delay_init(); //ʹ�ð���ʱ��Ҫ��ʼ���ӳٺ���
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PE/PAʱ��
	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //��������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;     //PA0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ�50MHz
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //��������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;     //PE2/PE3/PE4
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ�50MHz
	GPIO_Init(GPIOE,&GPIO_InitStructure);
}


//==============================================================
//��������������Ҫʹ�ð���ʱ�����˳����Ƶ�main������ʹ��
//������Mood:/1-֧���������¼�� /0-��֧���������¼��
//���أ�u8���ͣ����ظ�����״̬
//==============================================================
u8 KEY_Scan(u8 Mood) //1-֧���������� /0-��֧������ɨ��
{
	static u8 key_up = 1; //��¼�ϴΰ���״̬
	if(Mood) key_up = 1; //֧����������
	if(key_up && (WK_UP == 1 || KEY0 == 0 || KEY1 == 0 || KEY2 == 0))
	{
		key_up = 0;
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1)  return WKUP_PRES;    //����WKUP������
		else if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) == 0) return KEY2_PRES; //����KEY2������
		else if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0) return KEY1_PRES; //����KEY1������
		else if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0) return KEY0_PRES; //����KEY0������
	}
	else if(WK_UP == 0 && KEY0 == 1 && KEY1 == 1 && KEY2 == 1) key_up = 1;
	return 0;
}


void KEY_Multi_Scan(u8 Mood, u8 *Key_Result) //1-֧���������� /0-��֧������ɨ��
{
	static u8 key_up = 1; //��¼�ϴΰ���״̬
	if(Mood) key_up = 1; //֧����������
	if(key_up)
	{
		if(WK_UP == 1 || KEY0 == 0 || KEY1 == 0 || KEY2 == 0)
		{
			Key_Result[0] = 1;
			key_up = 0;       //ֻ�ܽ���һ��
			delay_ms(10);
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1)  Key_Result[WKUP_PRES] = 1;    //����WKUP������
			else Key_Result[WKUP_PRES] = 0;
			if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) == 0) Key_Result[KEY2_PRES] = 1; //����KEY2������
			else Key_Result[KEY2_PRES] = 0;
			if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0) Key_Result[KEY1_PRES] = 1; //����KEY1������
			else Key_Result[KEY1_PRES] = 0;
			if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0) Key_Result[KEY0_PRES] = 1; //����KEY0������
			else Key_Result[KEY0_PRES] = 0;
		}

	}
	else
	{
		Key_Result[0] = 0;
		if(WK_UP == 0 && KEY0 == 1 && KEY1 == 1 && KEY2 == 1) 
		{
			key_up = 1; //���а����ͷ�ʱkey_up��λ���ȴ��´ΰ�������
		}
	}
}



//==============================================================
//��������������Ҫʹ�ð���ʱ�����˳����Ƶ�main������ʹ��
//��������Ҫ��main�ж���u8 key
//���أ���
//==============================================================
int KEY_main(void)
{
	u8 key;
	while(1)
	{
		key = KEY_Scan(0);
		if(key)
		{
			switch(key)
			{
				case WKUP_PRES:
				{
					//�˴�д��WKUP������ʱ��Ҫ��������
					break;
				}
			  case KEY0_PRES:
				{
					//�˴�д��KEY0������ʱ��Ҫ��������
					break;
				}
				case KEY1_PRES:
				{
					//�˴�д��KEY1������ʱ��Ҫ��������
					break;
				}
				case KEY2_PRES:
				{
					//�˴�д��KEY2������ʱ��Ҫ��������
					break;
				}
			}//switch(key)
		}//if(key)
	}//while(1)
}
