#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

extern int8_t Time[3];
extern int8_t *Pin;
int8_t Set;
int8_t Flag;


void Butten_Init(void)
{
	//初始化GPIOB
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//初始化GPIOA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}


//读取复位按钮
uint8_t Butten_ReadReSet(void)
{
	uint8_t mark = 0;
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) ==0);
		Delay_ms(20);
		mark = 1;
	}
	return mark;
}

//读取左按钮
uint8_t Butten_ReadLeft(void)
{
	uint8_t mark = 0;
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) ==0);
		Delay_ms(20);
		mark = 1;
	}
	return mark;
}


//读取右按钮
uint8_t Butten_ReadRight(void)
{
	uint8_t mark = 0;
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) ==0);
		Delay_ms(20);
		mark = 1;
	}
	return mark;
}

void Butten_Select(void)
{
		if(Butten_ReadReSet() == 1)
	{
		TIM_Cmd(TIM2, DISABLE);
		Flag = 1;
		Set = 12;
		Pin = Time+2;
		while(Butten_ReadReSet() == 0)
		{
			OLED_ShowNum(1, 6, Time[0], 2);
			OLED_ShowNum(1, 9, Time[1], 2);
			OLED_ShowNum(1, 12, Time[2], 2);
			if(Butten_ReadLeft() == 1)
			{
				OLED_ShowChar(2, Set, ' ');
				Set -= 3;
				Pin -= 1;
				if(Set < 6)
					Set = 12;
				if(Pin < Time)
					Pin = Time+2;
			}
			if(Butten_ReadRight() == 1)
			{
				OLED_ShowChar(2, Set, ' ');
				Set += 3;
				Pin += 1;
				if(Set > 12)
					Set = 6;
				if(Pin > Time+2)
					Pin = Time;
			}
			OLED_ShowChar(2, Set, '^');
		}
		OLED_ShowChar(2, Set, ' ');
		Flag = 0;
		TIM_Cmd(TIM2, ENABLE);
	}
}
