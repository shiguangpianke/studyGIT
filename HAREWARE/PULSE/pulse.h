#ifndef __PUSEL_H
#define __PUSEL_H
#include "sys.h"
 
void TIM1_config(u32 Cycle);
void TIM2_config(u32 PulseNum);
void Pulse_output(u32 Cycle,u32 PulseNum);
 
 
#endif


