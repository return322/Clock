#include "stm32f10x.h"                  // Device header

extern int8_t Time[3];
extern int8_t *Pin;
extern int8_t Flag;


//初始化编码盘
void Ecoder_Init(void)
{
	//配置GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//配置AFIO
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3 | GPIO_PinSource4);
	
	//初始化EXTI
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line3 | EXTI_Line4;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE ;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	//初始化NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);	
}

//中断函数
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) == 1)
	{
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0 && Flag == 1)
		{
			(*Pin)++;
			if(Time[2] > 59)
			{
				Time[2] = 0;
				Time[1]++;
			}
			if(Time[1] > 59)
			{
				Time[1] = 0;
				Time[0]++;
			}
			if(Time[0] >23)
				Time[0] = 0;
		}
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}

void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) == 1)
	{
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0 && Flag == 1)
		{
			(*Pin)--;
			if(Time[2] < 0)
			{
				Time[2] = 59;
				Time[1]--;
			}
			if(Time[1] < 0)
			{
				Time[1] = 59;
				Time[0]--;
			}
			if(Time[0] < 0)
				Time[0] = 23;                                                           
		}
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

