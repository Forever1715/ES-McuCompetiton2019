#include "decoding.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用-STM32开发板
//本程序为比赛过程中的解码程序
//索奥科技中心@isuoao
//修改日期:2019/5/1
//版本：V1.0							  
//////////////////////////////////////////////////////////////////////////////////	 

enum
{
	DCOD_WAIT,
	DCOD_SYNCING,
	DCOD_FULLRUN,
}DCOD_Status;     //记录整个信号传输状态

u8 DCOD_BUF[DCOD_BUF_LEN] = {'\0'}; 
//接收数据缓冲区(已解码)

u32 DCOD_BUF_STA;
//bit31:保留
//bit30:解码完成标志位
//bit29:解码上一位
//bit28://记录捕获时间等于半帧时的标志位
//bit27-24:bit位置记录
//bit23-0:byte位置记录/解码字节数

u32 timeCapture = 0; //解码需要,记录本次捕获距离上次捕获的时间
u8 ITRPT_Flag = FALSE;

//==============================================================
//功能描述：接收信号的GPIO初始化
//参数：无
//返回：无
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
//功能描述：解码所需时钟的初始化
//参数：arr,psc->重装载值,预分频
//返回：无
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
//功能描述：解码程序初始化(必须调用)
//参数：arr,psc->重装载值,预分频
//返回：无
//==============================================================
void DCOD_Init(void)
{
	DCOD_GPIOInit();
	DCOD_TIM_InitUS; //初始化时钟并指定更新中断的触发时间
}

void DCOD_End(void)
{
	TIM_Cmd(DCOD_TIM,DISABLE);
	TIM_ITConfig(DCOD_TIM,TIM_IT_Update|DCOD_IT_CC,DISABLE);
}

//==============================================================
//功能描述：数据缓冲区初始化
//参数：无
//返回：无
//==============================================================
void DCOD_CharInit(void)
{
	DCOD_BUF_STA = 0;
}
//==============================================================
//功能描述：bit接收程序
//参数：bit->0/1
//返回：TURE/FALSE
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
//功能描述：DCOD_中断服务程序
//参数：无
//返回：无
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
		{//超过规定时间不传输数据视为结束传输
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
	{ //触发捕获中断
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
			{//传输
				if(DCOD_T_IF2T)
				{//每隔一个帧位捕获触发时/本次bit是上次bit的相反数
					DCOD_BUF_STA ^= (1<<29);
					receive_temp = DCOD_Receive(bit);
				}
				else if(DCOD_T_IFT)
				{//每隔一个半帧位捕获触发时有两种情况/当下次触发中断也是半帧位时/本次bit等于上次bit
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
			{//判断是否处于非传输状态/如果是执行下面程序
				if(GpioLevel && DCOD_T_IFEND)
				{//判断是否高电平
					DCOD_Status = DCOD_SYNCING; //进入同步状态
					DCOD_CharInit(); //对BUF进行初始化
				}
			}
			else if(DCOD_SYNCING == DCOD_Status)
			{//同步
				if( !GpioLevel && DCOD_T_IFSTART)
				{//起始位高电平同步完毕/等待低电平的同步
				}
				else if(GpioLevel && DCOD_T_IF4T)
				{//半帧位同步完成/进入解码
					DCOD_BUF_STA |= (1<<29); //第一位数据是1
					receive_temp = DCOD_Receive(bit);
				}
				else if(GpioLevel && DCOD_T_IFSTART)
				{//处于帧位/若下一个中断触发为半帧位时间之后，则完成半帧位同步
				}
				else if(!GpioLevel && DCOD_T_IFT)
				{//半帧位同步完成/进入解码
					DCOD_BUF_STA &= ~(1<<29); //第一位数据是0
					receive_temp = DCOD_Receive(bit);
				}
				else
				{ //如果匹配错误则进入等待
					DCOD_Status = DCOD_WAIT;
				}
				if(receive_temp) DCOD_Status = DCOD_FULLRUN; //第一位数据传输完成后进入全速同步状态
			}
			timeCapture = 0;//每次触发捕获中断后都清零捕获时间
			//对捕获触发边沿进行更改/保证每个边沿都可以被捕获
			if(TIM5->CCER & (1<<DCOD_ICPolarity_Bit))
				TIM5->CCER &= ~(1<<DCOD_ICPolarity_Bit);
			else
				TIM5->CCER |= (1<<DCOD_ICPolarity_Bit);
		}
		TIM_ITConfig(DCOD_TIM,DCOD_IT_CC,ENABLE);
	}
	TIM_ClearITPendingBit(TIM5, DCOD_IT_CC|TIM_IT_Update); //打开下次中断
}
	
