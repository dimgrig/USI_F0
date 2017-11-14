#include "stm32f0xx.h"
#include "userdef.h"

typedef enum
{
	ERROR_ = 0x30, /*!<  */
	IDLE = 0x31, /*!<  */
	TOUCH   = 0x32, /*!< */
	MEASURE  = 0x33, /*!<  */
	REMOVAL  = 0x34,  /*!<  */
	RESULTS  = 0x35,  /*!<  */
	RESET_  = 0x36  /*!<  */
}State_TypeDef;

typedef enum
{
	MAIN = 0x00, /*!<  */
	SETTINGS   = 0x01, /*!< */
	MATERIAL  = 0x02, /*!<  */
	CALIBRATION  = 0x03  /*!<  */
}Screen_TypeDef;


uint16_t SPI_SendRecvByte (unsigned char byte_s);
void delay_ms( unsigned short ms);

void SPI_SendData(SPI_TypeDef* SPIx, uint16_t Data);
uint16_t SPI_ReceiveData(SPI_TypeDef* SPIx);
