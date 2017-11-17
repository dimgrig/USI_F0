#include "t14-spi.h"


         
void SPI1_init()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	//GPIO_StructInit(&GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//SPI_I2S_DeInit(SPI1);
	SPI_InitTypeDef SPI_InitStructure;
	//SPI_StructInit(&SPI_InitStructure);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //////////////!!!!!!!!
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;///////////////!!!!!!!!!!!!! SPI_BaudRatePrescaler_4

    SPI_SSOutputCmd(SPI1, ENABLE);
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_CalculateCRC(SPI1, DISABLE);

	SPI_RxFIFOThresholdConfig (SPI1, SPI_RxFIFOThreshold_QF);
	//SPI_SSOutputCmd(SPI1, DISABLE); // Disable SPI1 NSS output for master mode


	SPI_Cmd(SPI1, ENABLE);

	SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);
}

uint16_t SPI_receive(SPI_TypeDef* SPIx)
{
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
	return (uint16_t)SPI_I2S_ReceiveData(SPIx);
}

          
void GPIO_SPI_init()
{
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  GPIO_InitTypeDef  CS_init;  // создаем структуру

  CS_init.GPIO_Mode = GPIO_Mode_OUT;  // направление - выход
  CS_init.GPIO_OType = GPIO_OType_PP;  // Двухтактный выход
  CS_init.GPIO_PuPd = GPIO_PuPd_UP;  // Без подтяжки
  CS_init.GPIO_Speed = GPIO_Speed_Level_3;  // Скорость низкая
  CS_init.GPIO_Pin = GPIO_Pin_6; // Светодиод на 15м выводе

  GPIO_Init(GPIOB, &CS_init);  // Функция, выполняющая настройку портов
  
  GPIO_InitTypeDef  PD_init;  // создаем структуру

  PD_init.GPIO_Mode = GPIO_Mode_OUT;  // направление - выход
  PD_init.GPIO_OType = GPIO_OType_PP;  // Двухтактный выход
  PD_init.GPIO_PuPd = GPIO_PuPd_UP;  // Без подтяжки
  PD_init.GPIO_Speed = GPIO_Speed_Level_3;  // Скорость низкая
  PD_init.GPIO_Pin = GPIO_Pin_8; // Светодиод на 15м выводе

  GPIO_Init(GPIOB, &PD_init);  // Функция, выполняющая настройку портов
 
  
//  GPIO_InitTypeDef TP_IRQ_init;
//  EXTI_InitTypeDef exti;
//  NVIC_InitTypeDef nvic;
  
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
//  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
//
//  TP_IRQ_init.GPIO_Mode = GPIO_Mode_IN;
//  TP_IRQ_init.GPIO_Pin = GPIO_Pin_7;
//  TP_IRQ_init.GPIO_Speed = GPIO_Speed_Level_3;
//  GPIO_Init(GPIOB, &TP_IRQ_init);
  
 
//  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);
//
//  exti.EXTI_Line = EXTI_Line0;
//  exti.EXTI_Mode = EXTI_Mode_Interrupt;
//  exti.EXTI_Trigger = EXTI_Trigger_Falling;
//  exti.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&exti);
//
//  nvic.NVIC_IRQChannel = EXTI0_1_IRQn; //** EXTI0_IRQn;
//  nvic.NVIC_IRQChannelPriority = 0;
//  //nvic.NVIC_IRQChannelPreemptionPriority = 0;
//  //nvic.NVIC_IRQChannelSubPriority = 0;
//  nvic.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&nvic);

}     

//void SPI_LCD_CS_HIGH()
//{
//  GPIO_SetBits(GPIOB, GPIO_Pin_8);
//}
//
//void SPI_LCD_CS_LOW()
//{
//  GPIO_ResetBits(GPIOB, GPIO_Pin_8);
//}

void SPI_LCD_CS_HIGH()
{
  GPIO_SetBits(GPIOB, GPIO_Pin_6);
}

void SPI_LCD_CS_LOW()
{
  GPIO_ResetBits(GPIOB, GPIO_Pin_6);
}

void SPI_PD_HIGH()
{
  GPIO_SetBits(GPIOB, GPIO_Pin_8);
}

void SPI_PD_LOW()
{
  GPIO_ResetBits(GPIOB, GPIO_Pin_8);
}

/* рабочий код SPI*/
/*  
    SPI_I2S_SendData(SPI1, 15);
    uint16_t data1 = SPI_receive(SPI2);
 
    while(!s_Tick);
    s_Tick = 0;  
    if (data1 == 15)
       LEDsSet(0x3);

*/
