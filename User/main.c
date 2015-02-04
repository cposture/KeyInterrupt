#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

//���������ֱܷ�����PB0��PC4�����������ֱ�����PC13��PA0(��ͨ��PA0����������)��PC13(��ͨ��PC13�����½���)��Ӧ���ⲿ�ж�ΪEXTI0��EXTI13
//������Ӧ��ʱ��
void RCC_conf()
{
	//����ϵͳʱ��
	SystemInit();
	//��PB��PCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);//�� RCC_APB2Periph_AFIO��ʹPC13��Ϊ��ͨIO��
}
//�����жϷ���͸���ͨ�����ȼ�
void NVIC_conf()
{
	NVIC_InitTypeDef NVIC_Struct;
	//�����ж�ͨ�����ȼ���1λ��ռ���ȼ�
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
//GPIO�ڳ�ʼ��
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
  GPIO_Struct.GPIO_Mode = GPIO_Mode_Out_PP;	 // ��������
	GPIO_Init(GPIOA,&GPIO_Struct);
}
//����Ӧ��IO������Ϊ�ж���·
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
