/*
===============================================================================
 Name        : main.c
 Author      : Boseji
 Version     :
 Copyright   : Boseji (C) Copyright 2010 All rights reserved.
 Description : Independent Clock Configuration for Internal
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif
#include "clock.h"
#define BV(X) (1<<X)

/**
 * \brief Configure CLKOUT for reference clock check.
 * \param clksrc clock source: irc_osc(0), sys_osc(1), wdt_osc(2),main_clk(3).
 * \return None
 * \note This function was taken from the Original Implementation in LPClib
 *       Added the specific I/O initialization to help in Debug and future Use
 */
void CLKOUT_Setup ( uint32_t clksrc )
{
  /*Enable the PIO0_1 in CLOCKOUT Function*/
  LPC_IOCON->PIO0_1 |=0x01;
  /* debug PLL after configuration. */
  LPC_SYSCON->CLKOUTCLKSEL = clksrc;	/* Select Main clock */
  LPC_SYSCON->CLKOUTUEN = 0x01;		/* Update clock */
  LPC_SYSCON->CLKOUTUEN = 0x00;		/* Toggle update register once */
  LPC_SYSCON->CLKOUTUEN = 0x01;
  while ( !(LPC_SYSCON->CLKOUTUEN & 0x01) );		/* Wait until updated */
  LPC_SYSCON->CLKOUTDIV = 1;			/* Divided by 1 */
  return;
}

int main(void) {
	long unsigned int i = 0 ;
	//SystemInit(); //System Clock at 48MHz Can be used By default no need to enable this
	//Initialized External XTAL clock
	{
	  SYSCLK_ExternalOscillator_ON();
	  SYSCLK_PLL_Source(SYSCLK_PLL_SRC_EXTOSC);
	  SYSCLK_PLL_Config(3,0);
	  SYSCLK_PLL_ON();
	  SYSCLK_MainClock_Source(SYSCLK_MainClock_SRC_PLLINOSC);

	/**
	 * Check which Peripherals need AHB clock to be ON
	 * Bit 0 - SYS AHBtoAPB bridge,AHB Matrix, Cortex-M0 FCLK & HCLK, SysCon, PMU
	 * Bit 1 - ROM
	 * Bit 2 - RAM
	 * Bit 3 - Flash Register Interface
	 * Bit 4 - Flash Array
	 * Bit 5 - I2C
	 * Bit 6 - GPIO
	 * Bit 7 - CT16B0
	 * Bit 8 - CT16B1
	 * Bit 9 - CT32B0
	 * Bit 10 - CT32B1
	 * Bit 11 - SSP0 - SPI 0
	 * Bit 12 - UART
	 * Bit 13 - ADC
	 * Bit 14 - Reserved
	 * Bit 15 - WDT
	 * Bit 16 - IOCON - I/O Configuration Block
	 * Bit 17 - Reserved
	 * Bit 18 - SSP1 0 SPI1
	 */
	//LPC_SYSCON->SYSAHBCLKCTRL = AHBCLKCTRL_Val;
	}
	/* Enable AHB clock to the GPIO domain. */
	  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6); //Enable the I/O Clock
	  LPC_GPIO0->DIR |= BV(7);
	  LPC_GPIO0->DATA |= BV(7);

	  CLKOUT_Setup(3);
	// Enter an infinite loop, just incrementing a counter

	while(1) {
			i++ ;
			if(i==1000000)
			{
			i=0;
			LPC_GPIO0->DATA ^= BV(7);
			}
	}
		return 0 ;
}
