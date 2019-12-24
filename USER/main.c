#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "beep.h"
#include "led.h"
#include "usart.h"
#include "pulse.h"

int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	
	delay_init();
	uart_init(9600);	 			//9600	 
	LED_Init();
	while(1)
	{
		LED0=0;
		LED1=1;
		delay_ms(500);	 //延时300ms
		LED0=1;
		LED1=0;
		delay_ms(500);	//延时300ms
		Pulse_output(1000,3200);     // 脉冲频率1KHz，每经过1s会输出3200个脉冲，步进电机会转1周。
	}
}




