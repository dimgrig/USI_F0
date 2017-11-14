/*
 * t14-flash.h
 *
 *  Created on: 25 ���. 2017 �.
 *      Author: Dima
 */

#ifndef T14_FLASH_H_
#define T14_FLASH_H_

#include <stdint.h>
#include "stm32f0xx.h"

FLASH_Status FLASH_Write_DataWord(uint32_t address, uint32_t data);

uint32_t FLASH_Read_DataWord(uint32_t address);



#endif /* T14_FLASH_H_ */
