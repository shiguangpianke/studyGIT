#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "beep.h"
#include "led.h"
#include "usart.h"
#include "pulse.h"

int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	
	delay_init();
	uart_init(9600);	 			//9600	 
	LED_Init();
	while(1)
	{
		LED0=0;
		LED1=1;
		delay_ms(500);	 //��ʱ300ms
		LED0=1;
		LED1=0;
		delay_ms(500);	//��ʱ300ms
		Pulse_output(1000,3200);     // ����Ƶ��1KHz��ÿ����1s�����3200�����壬���������ת1�ܡ�
	}
}




