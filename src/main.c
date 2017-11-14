// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include "diag/Trace.h"

//#include "Timer.h"
//#include "BlinkLed.h"

#include "t14-spi.h"
#include "t14-math.h"

#include "t14-timer.h"

#include "t14-adc.h"
#include "t14-dma.h"
#include "t14-ssi.h"
#include "t14-flash.h"
#include "GUI.h"


#include "t14-usb.h"
USB_CORE_HANDLE  USB_Device_dev ;

#include "FT_Platform.h"
#include "SampleApp.h"

//#define TP_TEST_DRAW

#define SPS 5000UL
#define Trc 0.001f
#define K (SPS*Trc)

static uint32_t Dacc = 0;
static uint32_t Dout = 0;

uint16_t init_finished = 0;

Screen_TypeDef SCREEN = MAIN;
ft_uint16_t dloffset;

State_TypeDef STATE = IDLE;
uint16_t TouchDelay = 0;

double F = 0; //текущие
double A = 0;
double E, HB, ST, SB;

double F1 = 0; //измерение
double A0 = 0; //касание
double A1 = 0; //измерение
double H = 0; //снятие

// ----- Timing definitions -------------------------------------------------

// Keep the LED on for 2/3 of a second.
#define BLINK_ON_TICKS  (TIMER_FREQUENCY_HZ * 3 / 4)
#define BLINK_OFF_TICKS (TIMER_FREQUENCY_HZ - BLINK_ON_TICKS)

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wunused-parameter"
//#pragma GCC diagnostic ignored "-Wmissing-declarations"
//#pragma GCC diagnostic ignored "-Wreturn-type"

void main(int argc, char* argv[])
{

	__disable_irq();

//	RCC_DeInit(); //reset all rcc settings
//	RCC_HSEConfig(RCC_HSE_ON); //hse on
//	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE); //HSE like system clk source
//	//RCC_HSICmd(ENABLE);  //HSI 8 MHz on
//	RCC_PREDIV1Config(RCC_PREDIV1_Div2);
//	RCC_PLLConfig(RCC_PLLSource_PREDIV1 ,RCC_PLLMul_12); //pll work with (hsi/2)*12=32MHz
//	//RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_12);  //pll work with (hsi/2)*12=48MHz
//	RCC_PLLCmd(ENABLE); // pll enable
//	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

	/* Setup STM32 system (clock, PLL and Flash configuration) */
	SystemInit();

	uint8_t clk = RCC_GetSYSCLKSource();

	TIMERS_init();


	clk = 1;

	SPI1_init();
	GPIO_SPI_init();
	SPI_LCD_CS_HIGH();

	DMA_init();
	ADC_init();

	InitSSI();


	/* rcc init end */
	/* usb init start*/
	//Set_System();

//	RCC_Initializatiion();
	USBD_Init(&USB_Device_dev, &USR_desc, &USBD_CDC_cb, &USR_cb);

//**	RCC_Initializatiion();
//**	SYSCFG_USBPuCmd( ENABLE );
//
//**	USB_Interrupts_Config();
//**	USB_Init();

	Ft_Gpu_HalInit_t halinit;
	Ft_Gpu_Hal_Context_t host,*phost;

	halinit.TotalChannelNum = 1;

	Ft_Gpu_Hal_Init(&halinit);
	host.hal_config.channel_no = 0;
	host.hal_config.spi_clockrate_khz = 2000; //in KHz

	Ft_Gpu_Hal_Open(&host);
	phost = &host;

	Ft_Gpu_Hal_Sleep(100);

//	SAMAPP_BootupConfig(phost);

//	FLASH_Status FLASHStatus;
//	ft_uint32_t storedMaterial = FLASH_Read_DataWord(0);
//	if (storedMaterial >= 201 & storedMaterial <=203) {
//	  ;
//	} else {
//	  FLASHStatus = FLASH_Write_DataWord(0, 201);
//	}

	//*	dloffset = API_Screen_BasicScreen(phost, SCREEN);

	Ft_Gpu_Hal_Sleep(50);

	int flag = 1;
	uint8_t i;
	init_finished = 1;
	__enable_irq();



	// Infinite loop
	while (1)
	{
//	  int tag = Ft_Gpu_Hal_Rd8(phost,REG_TOUCH_TAG);

//	  if (tag != 0){
//
//		  SAMAPP_API_Screen_Content(phost, SCREEN, STATE, tag, dloffset, F, A, A0, H, F1, A1, E, HB, ST, SB, init_finished);
//		  Ft_Gpu_Hal_Sleep(5);
//
//		  switch (tag){
//			case 1:
//				switch (SCREEN){
//					case MAIN:
//						SCREEN = SETTINGS;
//						dloffset = API_Screen_BasicScreen(phost, SCREEN);
//					break;
//					case SETTINGS:
//						SCREEN = MAIN;
//						dloffset = API_Screen_BasicScreen(phost, SCREEN);
//					break;
//					case MATERIAL:
//						SCREEN = SETTINGS;
//						dloffset = API_Screen_BasicScreen(phost, SCREEN);
//					break;
//					case CALIBRATION:
//						SCREEN = SETTINGS;
//						dloffset = API_Screen_BasicScreen(phost, SCREEN);
//					break;
//				}
//			break;
//			case 2:
//				switch (STATE)
//			  {
//				case IDLE:
//				  STATE = TOUCH;
//				break;
//				case TOUCH:
//				  A0 = A;
//				  STATE = MEASURE;
//				break;
//				case MEASURE:
//				  F1 = F;//измерение
//				  A1 = A;
//				  STATE = REMOVAL;
//				break;
//				case REMOVAL:
//				  H = A;
//
//				  T14math(&E, &ST, &SB, &HB, F1, A0, A1, H);
//				  STATE = RESULTS;
//				break;
//				case RESULTS:
//					//clear
//				  STATE = TOUCH;
//				break;
//			  }
//
//				SAMAPP_API_Screen_Content(phost, SCREEN, STATE, tag, dloffset, F, A, A0, H, F1, A1, E, HB, ST, SB, init_finished);
//				Ft_Gpu_Hal_Sleep(5);
//				flag = 1;
//			break;
//			case 3:
//				STATE = IDLE;
//				SAMAPP_API_Screen_Content(phost, SCREEN, STATE, tag, dloffset, F, A, A0, H, F1, A1, E, HB, ST, SB, init_finished);
//				flag = 0;
//			break;
//			case 4:
//				switch (SCREEN){
//					case MAIN:
//					break;
//					case SETTINGS:
//						SCREEN = MATERIAL;
//						dloffset = API_Screen_BasicScreen(phost, SCREEN);
//					break;
//					case MATERIAL:
//					break;
//					case CALIBRATION:
//					break;
//				}
//			break;
//			case 5:
//				switch (SCREEN){
//					case MAIN:
//					break;
//					case SETTINGS:
//						SCREEN = CALIBRATION;
//						dloffset = API_Screen_BasicScreen(phost, SCREEN);
//					break;
//					case MATERIAL:
//					break;
//					case CALIBRATION:
//					break;
//				}
//			break;
//			case 201:
//			case 202:
//			case 203:
//				//** FLASHStatus = FLASH_Write_DataWord(0, tag);
//				//ft_uint32_t storedValue = FLASH_Read_DataWord(0);
//			break;
//		  }
//
//		  while (tag != 0){
//			  tag = Ft_Gpu_Hal_Rd8(phost,REG_TOUCH_TAG);
//		  }
//
//	  } else {
//		  SAMAPP_API_Screen_Content(phost, SCREEN, STATE, tag, dloffset, F, A, A0, H, F1, A1, E, HB, ST, SB, init_finished);
//		  tag = Ft_Gpu_Hal_Rd8(phost,REG_TOUCH_TAG);
//		  Ft_Gpu_Hal_Sleep(5);
//	  }

	if (USB_Device_dev.dev.device_status == USB_CONFIGURED) {
	  USB_Send_DataPair(&USB_Device_dev, STATE, F, A);
	  //USB_Send_State(&USB_Device_dev, STATE);
	}

	  if (flag != 0)
	  	  init_finished++;

	  //SAMAPP_API_Screen_Content(phost, dloffset, init_finished);

	}
  // Infinite loop, never return.
}


//void EXTI0_1_IRQHandler()
//{
//    EXTI_ClearFlag(EXTI_Line0);
//}


void TIM2_IRQHandler (void)
{

  // Clear update interrupt bit
  TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);

////////  float adc_value_f = get_adc_value();
////////  F = F1K * adc_value_f + F1B;

    int mean = 0;
  	for (int i = 0 ; i < DMA_BUFFER_SIZE; i++){
  		mean += DMA_BUFFER[i];
  	}
  	F = mean / DMA_BUFFER_SIZE;

  	Dacc = Dacc + F - Dout;
  	Dout = Dacc/(uint16_t)K;
  	F = Dout;
//	int sum = 0;
//	int cnt = 0;
//	for (int i =0 ; i < DMA_BUFFER_SIZE; i++){
//		if ( abs( DMA_BUFFER[i] - mean ) < 50) {
//			DMA_BUFFER_[cnt] = DMA_BUFFER[i];
//			sum += DMA_BUFFER[i];
//			cnt++;
//		}
//	}
//	sum = sum / cnt;
	//F = F1K * sum * (ADC_VOLTS / ADC_MAX_BITS) + F1B;
	//F = sum * (ADC_VOLTS / ADC_MAX_BITS);
//	F = sum;

//**  	F = F1K * F + F1B;
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	//DMA_Cmd(DMA1_Channel1, ENABLE);

	//костыль
//	if (F < 0.099)
//	{
//		F = 0;
//	}



  A = A1K*ReadSSI()+A1B;

  //DMA_ClearITPendingBit( DMA1_IT_TC1);
  //DMA_ITConfig(DMA1_Channel1, DMA1_IT_TC1, ENABLE);
}


void DMA1_Channel1_IRQHandler(void)
{

   DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, DISABLE);
   //DMA_Cmd(DMA1_Channel1, DISABLE);


}

// ----------------------------------------------------------------------------
