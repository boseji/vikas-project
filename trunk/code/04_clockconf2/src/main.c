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
	SYSCLK_Setup_24MHzExternalOscillatorPLL();

	/* Enable AHB clock to the GPIO domain. */
	  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6); //Enable the I/O Clock
	  LPC_GPIO0->DIR |= BV(7);
	  LPC_GPIO0->DATA |= BV(7);

	  if(LPC_SYSCON->SYSRESSTAT&4)
		  while(1)
			  __NOP();

	  SYSCLK_CLKOUTDIV(4);
	  CLKOUT_Config(CLKOUT_SRC_MCLK);
	  WDT_Clock_Source(WDT_SRC_IRCOSC);
	  SYSCLK_WDTDIV(50);
	  WDT_Config(1,0,0xFF);
	  WDT_Reset();
	// Enter an infinite loop, just incrementing a counter
__NOP();
	while(1) {
			i++ ;//WDT_Reset();
			if((i==1000000))
			{
			i=0;WDT_Reset();
			LPC_GPIO0->DATA ^= BV(7);
			}
	}
		return 0 ;
}
