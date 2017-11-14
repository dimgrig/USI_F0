/*
 * t14-usb.h
 *
 *  Created on: 30 окт. 2017 г.
 *      Author: Dima
 */

#ifndef T14_USB_H_
#define T14_USB_H_

#include <stdio.h>
#include "stm32f0xx.h"
#include "usbd_cdc_core.h"
#include "usbd_usr.h"

void RCC_Initializatiion(void);
void USB_LP_IRQHandler (void);
void USB_Send_Packet(USB_CORE_HANDLE *pdev, uint8_t *packet, uint16_t length);
//void USB_Send_State(State_TypeDef STATE);
uint8_t hextoascii(uint8_t hex);

#endif /* T14_USB_H_ */
