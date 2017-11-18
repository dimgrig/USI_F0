/*
 * t14-dma.h
 *
 *  Created on: 28 окт. 2017 г.
 *      Author: Dima
 */

#ifndef T14_DMA_H_
#define T14_DMA_H_

<<<<<<< HEAD
#define DMA_BUFFER_SIZE 64
=======
#define DMA_BUFFER_SIZE 4
>>>>>>> 57d0eec03c0b4030ff015c4c701a728f7fb226e8

#include "stm32f0xx.h"

void DMA_init(void);


uint16_t DMA_BUFFER[DMA_BUFFER_SIZE];
//uint16_t DMA_BUFFER_[DMA_BUFFER_SIZE];

#endif /* T14_DMA_H_ */
