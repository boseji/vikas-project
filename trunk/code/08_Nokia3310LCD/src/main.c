/*
===============================================================================
 Name        : main.c
 Author      : Boseji
 Version     :
 Copyright   : Boseji (C) Copyright 2010 All rights reserved.
 Description : NOKIA 3310 LCD on SPI
===============================================================================
*/
#include "LPC11xx.h"
#include "clock.h"
#include "uart.h"
#include "eeprom_i2c.h"
#include "N3310.h"
#include "font_ttuf1.h"
#define BV(X) (1<<X)
#define __KEY_DEBUG



uint8_t data[]={"Aum Sri Ganeshay Namh\n\r"};
uint32_t key;
void KeyScan();
int main(void) {
	long unsigned int i = 0 ;

	SYSCLK_Setup_24MHzExternalOscillatorPLL();
	/* Enable AHB clock to the GPIO domain. */
	SYSCLK_Enable(SYSCLK_Active_GPIO); //Enable the I/O Clock
	LPC_GPIO0->DIR |= BV(7);
	LPC_GPIO0->DATA |= BV(7);

	//Key Config
	// R0-P3.0 R1-P3.1 R2-P3.2 R3-P3.3
	LPC_GPIO3->DIR |= BV(0)+BV(1)+BV(2)+BV(3);
	LPC_GPIO3->DATA |= (BV(0)+BV(1)+BV(2)+BV(3));
	// C0-P2.6 C1-P2.7 C2-P2.8 C3-P2.9
	LPC_GPIO2->DIR &= ~(BV(6)+BV(7)+BV(8)+BV(9));
	LPC_GPIO2->DATA &= ~(BV(6)+BV(7)+BV(8)+BV(9));

    SYSCLK_CLKOUTDIV(4);
	CLKOUT_Config(CLKOUT_SRC_MCLK);

	UARTInit(1,9600);
	eeprom_init();
	N3310_init();
	N3310_putpx(0,0,0xaa);
	N3310_fbDot(0,10,1);
	N3310_fbDot(0,11,1);
	N3310_fbDot(0,12,1);

	N3310_fbWrite(fontSet1,data);
	N3310_fbDisplay();
	while(1)
	{
		i++ ;//WDT_Reset();
		if((i==100000))
		{
			i=0;
			LPC_GPIO0->DATA ^= BV(7);
			UARTSend(data,sizeof(data)-1);

		}
		if((i%10000)==0)
		{
		 KeyScan();
//		 eeprom_write(0x110,0x15);
//    	 EEPROM_DELAY();
//    	 eeprom_read(0x110,4);
		}
	}
	return 0 ;
}
void KeyScan()
{
	uint32_t col;
	key=0;
			//Row 0 - P3.0
			LPC_GPIO3->DATA &= ~BV(0);
			col = (LPC_GPIO2->DATA);
			col = ~((col>>6)&0x0f);
			LPC_GPIO3->DATA |= BV(0);
	#ifdef __KEY_DEBUG
			if(col&1)UARTSendByte('*');
			if(col&2)UARTSendByte('0');
			if(col&4)UARTSendByte('#');
			if(col&8)UARTSendByte('D');
	#endif
			key |= col;
			//Row 1 - P3.1
			LPC_GPIO3->DATA &= ~BV(1);
			col = (LPC_GPIO2->DATA);
			col = ~((col>>6)&0x0f);
			LPC_GPIO3->DATA |= BV(1);
	#ifdef __KEY_DEBUG
			if(col&1)UARTSendByte('7');
			if(col&2)UARTSendByte('8');
			if(col&4)UARTSendByte('9');
			if(col&8)UARTSendByte('C');
	#endif
			key |= col<<4;
			//Row 2 - P3.2
			LPC_GPIO3->DATA &= ~BV(2);
			col = (LPC_GPIO2->DATA);
			col = ~((col>>6)&0x0f);
			LPC_GPIO3->DATA |= BV(2);
	#ifdef __KEY_DEBUG
			if(col&1)UARTSendByte('4');
			if(col&2)UARTSendByte('5');
			if(col&4)UARTSendByte('6');
			if(col&8)UARTSendByte('B');
	#endif
			key |= col<<8;
			//Row 3 - P3.3
			LPC_GPIO3->DATA &= ~BV(3);
			col = (LPC_GPIO2->DATA);
			col = ~((col>>6)&0x0f);
			LPC_GPIO3->DATA |= BV(3);
	#ifdef __KEY_DEBUG
			if(col&1)UARTSendByte('1');
			if(col&2)UARTSendByte('2');
			if(col&4)UARTSendByte('3');
			if(col&8)UARTSendByte('A');
	#endif
			key |= col<<12;
}

