#include "t14-adc.h"
#include "t14-dma.h"
//uint16_t valueVoltsMem[4] = {2048, 2048, 2048, 2048};
//uint8_t initADC = 0;

#define TIM_ADC TIM1

void ADC_init()
{

	ADC_DeInit (ADC1);


	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;

	//--Enable DMA1 clock--
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB , ENABLE);

	/* PCLK2 is the APB2 clock */
	/* ADCCLK = PCLK2/4 = 32/4 = 8MHz - ADCCLK should be less than 14MHz*/
	RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div4);

	/* Enable ADC1 clock so that we can talk to it */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);


	/* Configure GPIO pins to have photo sensors connected to ADC channels */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  // ����������� �����
	GPIO_Init(GPIOB, &GPIO_InitStructure);








	/* DMA Channel1 Configuration ----------------------------------------------*/
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) &DMA_BUFFER[0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = DMA_BUFFER_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn ;
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//Enable transfer complete interrupt for DMA1 channel 1
	DMA_ClearITPendingBit(DMA1_IT_TC1);
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE); //�������� ���������� �� �������� ������
	//DMA_ITConfig(DMA1_Channel1, DMA_IT_HT, ENABLE); //�������� ���������� �� ��������� ������

	//DMA_Cmd(DMA1_Channel1, ENABLE);
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);


	ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
	//ADC_ContinuousModeCmd(ADC1, ENABLE);
	ADC_DMACmd (ADC1, ENABLE);


	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_TRGO;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;

	ADC_Init(ADC1, &ADC_InitStructure);

	//��������� �������������� ��� �� �������� �������
	//ADC_ExternalTrigConvCmd(ADC1, DISABLE);


	/* ADCx Regular Channel Configuration */
	ADC_ChannelConfig(ADC1, ADC_Channel_8, ADC_SampleTime_239_5Cycles);




	/* Enable ADC1 */
	//ADC_DMACmd(ADC1, ENABLE); //Enable ADC1 DMA
	ADC_Cmd(ADC1, ENABLE);
	/* Wait until the ADC1 is ready */

	//** while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET){}
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY));
	while (! ADC_GetFlagStatus (ADC1, ADC_FLAG_ADEN));



//	   TIM_DeInit(TIM_ADC);
//	   TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
//
//	   /* Time Base configuration */
//	   TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
//	   TIM_TimeBaseStructure.TIM_Period = 10000;
//	   TIM_TimeBaseStructure.TIM_Prescaler = 320;
//	   TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//	   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	   TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
//	   TIM_TimeBaseInit(TIM_ADC, &TIM_TimeBaseStructure);
//	   TIM_SelectOutputTrigger(TIM_ADC, TIM_TRGOSource_Update);
//
//	   /* TIM1 counter enable */
//	   TIM_Cmd(TIM_ADC, ENABLE);



	//** ADC_SoftwareStartConv(ADC1);
	ADC_StartOfConversion(ADC1);


}

//float get_adc_value()
//{
//    float valueVolt = 0;
//
////**    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_4Cycles);
//
//    ADC_StartOfConversion(ADC1); /*ADC1->CR2 |= ADC_CR2_SWSTART;*/
//
//    if (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == SET)
//    {
//
//      if (initADC == 0)
//      {
//        initADC = 1;
//        for (int k = 0; k <= 3 ; k++)
//        {
//          valueVoltsMem[k] = ADC_GetConversionValue(ADC1);/*ADC1->DR;*/
//        }
//      }
//      else
//      {
//        // convert valueADC to valueVolt -> valueADC * (MAX VOLT / 2^12)
//        // and also :
//        // ADC_SoftwareStartConvCmdsoftwareS,
//        // ADC_GetCalibrationStatus
//        // with
//        // return (uint16_t) ADCx->DR;
//        ////valueVoltsMem[0] = ADC_GetConversionValue(ADC1);/*ADC1->DR;*/
//
//        for (int k = 0; k <= 39 ; k++)
//        {
//          valueVoltsMem[0] = valueVoltsMem[0] + ADC_GetConversionValue(ADC1);
//
//          for (int j = 0; j <= 50; )
//            j++;
//        }
//
//        valueVoltsMem[0] = valueVoltsMem[0]/40;
//
//
//      }
//
//      // ����������
//      for (int k = 0; k <= 3 ; k++)
//      {
//        valueVolt += valueVoltsMem[k];
//      }
//      valueVolt = valueVolt/4;
//
//      for (int k = 3; k > 0; k--)
//      {
//        valueVoltsMem[k] = valueVoltsMem[k - 1];
//      }
//
//
//      // convert  // Volt = 3.3
//      // ADC = 12 bits, so 2^12 = 4096
//      valueVolt = valueVolt * (ADC_VOLTS / ADC_MAX_BITS);
//
//      return valueVolt;
//    }
//    else
//    {
//      return 0;
//    }
//}
