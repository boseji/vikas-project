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
#ifndef BV
#define BV(X) (1<<X)
#endif
// TODO: insert other include files here

// TODO: insert other definitions and declarations here

int main(void) {
	
	// TODO: insert code here
	/* Enable AHB clock to the GPIO domain. */
	  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);
	  LPC_GPIO0->DIR |= BV(7);
	  LPC_GPIO0->DATA |= BV(7);
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
