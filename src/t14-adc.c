#include "t14-adc.h"

uint16_t valueVoltsMem[4] = {2048, 2048, 2048, 2048};
uint8_t initADC = 0;
         
void ADC_init()
{
//	/* TIM15 Periph clock enable */
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15 , ENABLE);
//	// TIMER 15:
//	TIM_DeInit(TIM15);
//	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
//	/* Time base configuration */
//	TIM_TimeBaseStructure.TIM_Period = SystemCoreClock/1000 - 1;
//	TIM_TimeBaseStructure.TIM_Prescaler = 0;
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM15, &TIM_TimeBaseStructure);
//
//	/* TIM15 TRGO selection */
//	TIM_SelectOutputTrigger(TIM15, TIM_TRGOSource_Update);
//
//	/* TIM1 enable counter */
//	TIM_Cmd(TIM15, ENABLE);

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
        
  RCC_HSICmd(ENABLE);
  /*!< Wait till HSI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
  {}
  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);

//**  ADC_CommonInitTypeDef ADC_CommonInitStructure;
//**  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div1;
//**  ADC_CommonInit(&ADC_CommonInitStructure);

//Инициализация ADC
  ADC_InitTypeDef ADC_InitStruct;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //Включаем тактирование ADC
  ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;  //Разрешение ADC - 12 бит
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  //Выравнивание результата вправо
  ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStruct.ADC_ScanDirection = ADC_ScanDirection_Upward;
  
//**  ADC_InitStruct.ADC_ScanConvMode = ENABLE; //DISABLE;
  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;  
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_TRGO;
//**  ADC_InitStruct.ADC_NbrOfConversion = 1;


  ADC_Init(ADC1, &ADC_InitStruct);  //Конифигурируем модуль ADC заданной структурой

  
  ADC_ChannelConfig(ADC1, ADC_Channel_8, ADC_SampleTime_239_5Cycles);
  //** ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
  ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_OneShot);

  /* Enable ADC1 */
  ADC_DMACmd(ADC1, ENABLE); //Enable ADC1 DMA
  ADC_Cmd(ADC1, ENABLE);
  /* Wait until the ADC1 is ready */

  //** while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET){}
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN));

  //** ADC_SoftwareStartConv(ADC1);
  ADC_StartOfConversion(ADC1);
}


float get_adc_value()
{   
    float valueVolt = 0;
      
//**    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_4Cycles);

    ADC_SoftwareStartConv(ADC1); /*ADC1->CR2 |= ADC_CR2_SWSTART;*/
    
    if (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == SET)
    {

      if (initADC == 0)
      {
        initADC = 1;
        for (int k = 0; k <= 3 ; k++)
        {
          valueVoltsMem[k] = ADC_GetConversionValue(ADC1);/*ADC1->DR;*/ 
        }
      }
      else
      {
        // convert valueADC to valueVolt -> valueADC * (MAX VOLT / 2^12)
        // and also :
        // ADC_SoftwareStartConvCmdsoftwareS,
        // ADC_GetCalibrationStatus
        // with
        // return (uint16_t) ADCx->DR;
        ////valueVoltsMem[0] = ADC_GetConversionValue(ADC1);/*ADC1->DR;*/      
        
        for (int k = 0; k <= 39 ; k++)
        {
          valueVoltsMem[0] = valueVoltsMem[0] + ADC_GetConversionValue(ADC1);
          
          for (int j = 0; j <= 50; )
            j++;
        }
        
        valueVoltsMem[0] = valueVoltsMem[0]/40;
      
 
      }
      
      // фильтрация       
      for (int k = 0; k <= 3 ; k++)
      {
        valueVolt += valueVoltsMem[k];
      }
      valueVolt = valueVolt/4;

      for (int k = 3; k > 0; k--)
      {
        valueVoltsMem[k] = valueVoltsMem[k - 1];
      }


      // convert  // Volt = 3.3
      // ADC = 12 bits, so 2^12 = 4096
      valueVolt = valueVolt * (ADC_VOLTS / ADC_MAX_BITS);
      
      return valueVolt;
    }
    else
    {
      return 0;
    }
}
