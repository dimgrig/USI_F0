#include <stdio.h>
#include "stm32f0xx.h"

#define ADC_VOLTS 3.3
#define ADC_MAX_BITS 4095
#define F1K 0.0857f
#define F1B -4.457f

//#define ADC_ExternalTrigConv_None   ((uint32_t)0x000E0000)

void ADC_init();
void ADC_init2();
//float get_adc_value();

