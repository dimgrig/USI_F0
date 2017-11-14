#include "GUI.h"


uint16_t SPI_SendRecvByte (unsigned char byte_s)
{
	/* wait for current SSP activity complete */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) ==  SET);
	//while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	SPI_SendData(SPI1, byte_s); //SPI_I2S_SendData(SPI1, byte_s);

//	uint16_t i = 0;
//	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){
////		i++;
////		if (i > 1000) {
////			break;
////		}
//		;
//	}
//        for(int i = 0; i < 1000; )
//        {
//          i++;
//          if (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != RESET)
//              break;
//        }
	return (uint16_t)SPI_ReceiveData(SPI1);
}


void delay_ms( unsigned short ms)
{
	unsigned short i,j;
	for( i = 0; i < ms; i++ )
	{
		for( j = 0; j < 1711; j++ ); // 1141 l1
	}
}

/**
  * @brief  Transmits a Data through the SPIx peripheral.
  * @param  SPIx: where x can be 1 or 2 in SPI mode
  * @param  Data : Data to be transmitted.
  * @retval None
  */
void SPI_SendData(SPI_TypeDef* SPIx, uint16_t Data)
{
  /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));

  /* Write in the DR register the data to be sent */
  SPIx->DR = Data;
}

/**
  * @brief  Returns the most recent received data by the SPIx peripheral.
  * @param  SPIx: where x can be 1 or 2 in SPI mode
  * @retval The value of the received data.
  */
uint16_t SPI_ReceiveData(SPI_TypeDef* SPIx)
{
  /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));

  /* Return the data in the DR register */
  return (uint16_t)SPIx->DR;
}
