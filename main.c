/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   用3.5.0版本库建的工程模板
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "usart.h"

uint64_t millis=0;
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
	
void delay(uint32_t delayTime)
{
	SysTick_Config(SystemCoreClock/1000);
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	millis=delayTime;
	while(millis>0);
	SysTick->CTRL&=SysTick_CTRL_ENABLE_Msk;	
}
void GPIO_Init_Led()
{
	GPIO_InitTypeDef led;
	GPIO_InitTypeDef input;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	led.GPIO_Mode=GPIO_Mode_Out_PP;
	led.GPIO_Pin=GPIO_Pin_2;
	led.GPIO_Speed=GPIO_Speed_50MHz;
	
	input.GPIO_Mode=GPIO_Mode_IPU;
	input.GPIO_Pin=GPIO_Pin_0;
	input.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&input);	
	GPIO_Init(GPIOA,&led);
}
#define OPEN_INT	_set_PRIMASK(1)
#define CLOSE_INT	_set_PRIMASK(0)

void EXITInit()
{
	NVIC_InitTypeDef nvic;
	EXTI_InitTypeDef exit;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA,ENABLE);
	
	nvic.NVIC_IRQChannel=EXTI0_IRQn;
	nvic.NVIC_IRQChannelCmd=ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority=1;
	nvic.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&nvic);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	exit.EXTI_Line=EXTI_Line0;
	exit.EXTI_LineCmd=ENABLE;
	exit.EXTI_Mode=EXTI_Mode_Interrupt;
	exit.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&exit);
}

int main(void)
{
	uint8_t a;
	uint16_t count=0;
	
	ElemType sh;
	uint8_t i;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	GPIO_Init_Led();
	EXITInit();
	Serial_begin();
	GPIO_WriteBit(GPIOA,GPIO_Pin_2,1);
	printf("Hello world\n");

			while(Serial_available())
		  {
				printf("%d ;\n",Serial_read());
			}
		
		printf("  done!\n");
	

	while(1)
	{
		while(Serial_available())
		  {
				printf("%c",Serial_read());
			}
	}
	  
}

/*********************************************END OF FILE**********************/

