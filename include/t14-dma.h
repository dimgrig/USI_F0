/*
 * t14-dma.h
 *
 *  Created on: 28 ���. 2017 �.
 *      Author: Dima
 */

#ifndef T14_DMA_H_
#define T14_DMA_H_


#define DMA_BUFFER_SIZE 64

#include "stm32f0xx.h"

void DMA_init(void);


uint16_t DMA_BUFFER[DMA_BUFFER_SIZE];
//uint16_t DMA_BUFFER_[DMA_BUFFER_SIZE];

#endif /* T14_DMA_H_ */
