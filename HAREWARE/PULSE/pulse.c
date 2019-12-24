#include "pulse.h"
 
 
/***********************TIM1��ʼ������*************************/
/****������****************************************************/
/******u32 Cycle�����趨����Ƶ�ʣ����㹫ʽ��Cycle=1Mhz/Ŀ��Ƶ�ʣ�*/
/****����ֵ��**************************************************/
/******��*****************************************************/
void TIM1_config(u32 Cycle)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_TIM1 , ENABLE); //ʱ��ʹ��
 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;                   //TIM1_CH4 PE14
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
 
    TIM_TimeBaseStructure.TIM_Period = Cycle-1;                 //ʹ��Cycle������Ƶ�ʣ�f=72/(71+1)/Cycle��  ��CycleΪ100ʱ����Ƶ��Ϊ10KHZ                           
    TIM_TimeBaseStructure.TIM_Prescaler =71;                    //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ                                                     
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //����ʱ�ӷָTDTS= Tck_tim            
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;            //�ظ�������һ��Ҫ=0���������߼���ʱ�����У�
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);                                       
 
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;          		//ѡ��ʱ��ģʽ��TIM�����ȵ���ģʽ1       
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_Pulse = Cycle/2-1;                    	//���ô�װ�벶��Ĵ���������ֵ��ռ�ձȣ�Ĭ��50%�����Ҳ���Ե��������Ҫ�Ļ�������Ϊһ���������뼴�ɣ�                                   
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      	//�������       
 
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);        				//ʹ��ͨ��4                                                 
 
    TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);	//����Ϊ����ģʽ
    TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);			//ѡ��ʱ��1�Ĵ�����ʽ��ʹ�ø����¼���Ϊ���������
    
 
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);               //ʹ��ͨ��4Ԥװ�ؼĴ���               
    TIM_ARRPreloadConfig(TIM1, ENABLE);                             //ʹ��TIM1��ARR�ϵ�Ԥװ�ؼĴ���       
}
/***********************TIM2��ʼ������*************************/
/****������****************************************************/
/******u32 PulseNum�����趨��������****************************/
/****����ֵ��*************************************************/
/******��*****************************************************/
void TIM2_config(u32 PulseNum)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure; 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		//ʹ�ܶ�ʱ��2��ʱ��
 
    TIM_TimeBaseStructure.TIM_Period = PulseNum-1;   			//������
    TIM_TimeBaseStructure.TIM_Prescaler =0;    
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);  
 
    TIM_SelectInputTrigger(TIM2, TIM_TS_ITR0);					//ѡ��ʱ��2�����봥��Դ���ڲ�������TIM1����
 
    TIM2->SMCR|=0x07;                                  			//���ô�ģʽ�Ĵ�����SMS[2:0]:111 �ⲿʱ��ģʽ1�� 
 
    TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);					//�����ж�ʧ��
 
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;        
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;     
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure);								//��ʱ��2�жϳ�ʼ��
}
/************************�����������**************************/
/****������****************************************************/
/******u32 Cycle�����趨����Ƶ�ʣ����㹫ʽ��Cycle=1Mhz/Ŀ��Ƶ�ʣ�*/
/*******u32 PulseNum�����趨����������������λ������************/
/****����ֵ��**************************************************/
/******��*****************************************************/
void Pulse_output(u32 Cycle,u32 PulseNum)
{
    TIM2_config(PulseNum);						//������������
    TIM_Cmd(TIM2, ENABLE);						//ʹ��TIM2���Ӷ�ʱ����
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);	//����жϱ�־λ
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);	//ʹ�ܸ����ж�
    TIM1_config(Cycle);							//ʹ�ܶ�ʱ��1������ʱ����
    
    TIM_Cmd(TIM1, ENABLE);						//ʹ�ܶ�ʱ��1
    TIM_CtrlPWMOutputs(TIM1, ENABLE);   		//�߼���ʱ��һ��Ҫ���ϣ������ʹ��
}
 
 
 
/********************��ʱ��2���жϷ�����**********************/
/****������****************************************************/
/******u32 PulseNum�����趨��������****************************/
/****����ֵ��*************************************************/
/******��*****************************************************/
/****����˵��:************************************************/
/*��TIM��CNT�Ĵ�����ֵ�����趨��Updateֵ�ᴥ�������жϣ���ʱ�趨���������������ϣ��ر�TIM1��TIM2*/
void TIM2_IRQHandler(void) 
{ 
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 	//TIM_IT_Update
    { 
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 	// ����жϱ�־λ 
        TIM_CtrlPWMOutputs(TIM1, DISABLE);  			//�����ʹ��
        TIM_Cmd(TIM1, DISABLE); 						//�رն�ʱ�� 
        TIM_Cmd(TIM2, DISABLE); 						//�رն�ʱ�� 
        TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE); 	//�ر�TIM2�����ж�
        
    } 
} 


