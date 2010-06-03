/*
===============================================================================
 Name        : main.c
 Author      : Boseji
 Version     :
 Copyright   : Boseji (C) Copyright 2010 All rights reserved.
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif
#include "clock.h"
#define BV(X) (1<<X)
int main(void) {
	long unsigned int i = 0 ;
	uint32_t Fdiv;
	uint32_t regVal;

	SYSCLK_Setup_24MHzExternalOscillatorPLL();
	/* Enable AHB clock to the GPIO domain. */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6); //Enable the I/O Clock
	LPC_GPIO0->DIR |= BV(7);
	LPC_GPIO0->DATA |= BV(7);
	
    SYSCLK_CLKOUTDIV(4);
	CLKOUT_Config(CLKOUT_SRC_MCLK);

	//uart
	NVIC_DisableIRQ(UART_IRQn);

	LPC_IOCON->PIO1_6 &= ~0x07;    /*  UART I/O config */
	LPC_IOCON->PIO1_6 |= 0x01;     /* UART RXD */
	LPC_IOCON->PIO1_7 &= ~0x07;
	LPC_IOCON->PIO1_7 |= 0x01;     /* UART TXD */

	/* Enable UART clock */
	SYSCLK_Enable(SYSCLK_Active_UART);
	SYSCLK_UARTDIV(1);/* divided by 1 */

	LPC_UART->LCR = 0x83;             /* 8 bits, no Parity, 1 Stop bit */
	regVal = LPC_SYSCON->UARTCLKDIV;
	// CodeRed - variable name changed in CMSIS 1.3
	//  Fdiv = ((SystemAHBFrequency/regVal)/16)/baudrate ;	/*baud rate */
	//Fdiv = (((SystemCoreClock/LPC_SYSCON->SYSAHBCLKDIV)/regVal)/16)/baudrate ;	/*baud rate */
	Fdiv = (((24000000UL)/regVal)/16)/9600;//

	LPC_UART->DLM = Fdiv / 256;
    LPC_UART->DLL = Fdiv % 256;
    LPC_UART->LCR = 0x03;		/* DLAB = 0 */
    LPC_UART->FCR = 0x07;		/* Enable and reset TX and RX FIFO. */

    /* Read to clear the line status. */
    regVal = LPC_UART->LSR;
#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80

    /* Ensure a clean start, no data in either TX or RX FIFO. */
    while ( (LPC_UART->LSR & (LSR_THRE|LSR_TEMT)) != (LSR_THRE|LSR_TEMT) );
    while ( LPC_UART->LSR & LSR_RDR )
    {
    	regVal = LPC_UART->RBR;	/* Dump data from RX FIFO */
    }

	while(1)
	{
		i++ ;//WDT_Reset();
		if((i==1000000))
		{
			i=0;
			LPC_GPIO0->DATA ^= BV(7);

			while ( !(LPC_UART->LSR & LSR_THRE) );
				  LPC_UART->THR = 'S';
		}
	}
	return 0 ;
}
