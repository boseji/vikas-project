/*
===============================================================================
 Name        : main.c
 Author      : 
 Version     :
 Copyright   : (C) Copyright
 Description : main definition
===============================================================================
*/
#define __USE_CMSIS
#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

#define BV(X) (1<<X)

/*****************************************************************************
** Function name:		CLKOUT_Setup
**
** Descriptions:		Configure CLKOUT for reference clock check.
** parameters:			clock source: irc_osc(0), sys_osc(1), wdt_osc(2),
**						main_clk(3).
**
** Returned value:		None
** Note: This function was taken from the Original Implementation in LPClib
** Added the specific I/O initialization to help in Debug and future Use
*****************************************************************************/
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
	SystemInit(); //System Clock at 48MHz Can be used By default

	/* Enable AHB clock to the GPIO domain. */
	  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6); //Enable the I/O Clock
	  LPC_GPIO0->DIR |= BV(7);
	  LPC_GPIO0->DATA |= BV(7);

	  CLKOUT_Setup(3);
	// Enter an infinite loop, just incrementing a counter
	  long unsigned int i = 0 ;
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
