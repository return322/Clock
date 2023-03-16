#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Butten.h"
#include "Ecoder.h"
#include "Timer.h"

int8_t Time[3];
int8_t *Pin = Time+1;

int main(void)
{
	//配置RCC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//初始化各个配件
	OLED_Init();
	Butten_Init();
	Ecoder_Init();
	Timer_Init();
	
	OLED_ShowString(1, 1, "Time:  :  :  ");
	while(1)
	{
		OLED_ShowNum(1, 6, Time[0], 2);
		OLED_ShowNum(1, 9, Time[1], 2);
		OLED_ShowNum(1, 12, Time[2], 2);
		Butten_Select();
	}
}
