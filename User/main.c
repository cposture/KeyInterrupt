#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

//两个二极管分别连接PB0和PC4，两个按键分别连接PC13，PA0(接通，PA0产生上升沿)，PC13(接通，PC13产生下降沿)对应的外部中断为EXTI0和EXTI13
//设置相应的时钟
void RCC_conf()
{
	//配置系统时钟
	SystemInit();
	//打开PB和PC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);//加 RCC_APB2Periph_AFIO，使PC13变为普通IO口
}
//设置中断分组和各个通道优先级
void NVIC_conf()
{
	NVIC_InitTypeDef NVIC_Struct;
	//设置中断通道优先级，1位抢占优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_Struct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Struct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Struct);
	
	NVIC_Struct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Struct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Struct);
}
//GPIO口初始化
void GPIO_conf()
{
	GPIO_InitTypeDef GPIO_Struct;
	GPIO_Struct.GPIO_Pin = GPIO_Pin_0;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Struct);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	
	GPIO_Struct.GPIO_Pin = GPIO_Pin_4;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_Struct);
	GPIO_SetBits(GPIOC,GPIO_Pin_4);
	
	GPIO_Struct.GPIO_Pin = GPIO_Pin_0;       
  GPIO_Struct.GPIO_Mode = GPIO_Mode_Out_PP;	 // 上拉输入
	GPIO_Init(GPIOA,&GPIO_Struct);
}
//把相应的IO口设置为中断线路
void EXTI_conf()
{
	EXTI_InitTypeDef EXTI_Struct;
	
	EXTI_ClearITPendingBit(EXTI_Line0);
	EXTI_ClearITPendingBit(EXTI_Line13);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);
	
	EXTI_Struct.EXTI_Line = EXTI_Line0;
	EXTI_Struct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_Struct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Struct.EXTI_LineCmd = ENABLE;
	
	EXTI_Init(&EXTI_Struct);
	
	EXTI_Struct.EXTI_Line = EXTI_Line13;
	EXTI_Struct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_Struct);
}

int main()
{
	int i;
	RCC_conf();
	NVIC_conf();
	GPIO_conf();
	EXTI_conf();
	
	while(1)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_0);
		for(i=0;i<0xFFFFF;i++)
			;
		GPIO_ResetBits(GPIOA,GPIO_Pin_0);
		for(i=0;i<0xFFFFF;i++)
			;
	}
}
