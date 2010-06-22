/*
 * N3310.c
 *
 *  Created on: Jun 21, 2010
 *      Author: Bose
 * "Original Driver Written by Muhammad J. A. Galadima 2004 / 02 / 05 for AVR ATmega8515"
 *      For Nokia 3310 LCD
 */
#include "LPC11xx.h"
#include "N3310.h"
#include "ssp.h"

uint8_t LCDfb[N3310_COLS][N3310_ROWS];
uint8_t LCDfbX,LCDfbY;

//To Write a Byte to the LCD Buffer
void N3310_writeb(uint8_t data)
{
	N3310_ENABLE();
	SSPByte(data);
	N3310_DISABLE();
}
//To Clear the LCD screen
void N3310_clr()
{
	uint32_t i;
	N3310_DATA_MODE();
	for(i=0; i<504; i++) {
		N3310_writeb(0x00);
	}
}
//To set XY co ordinates and update the FB coordinates
void N3310_setXY(uint8_t x, uint8_t y)
{
	if((x<N3310_COLS)&&(y<(N3310_ROWS*8))){
	LCDfbX = (x&0x7F);
	LCDfbY = y;
	N3310_CMD_MODE();
	N3310_writeb(LCDfbX|0x80);	// 0b1xxx xxxx
	N3310_writeb((y/8)|0x40);	// 0b0100 0xxx
	}
}
// NATIVE function to put a Byte onto the screen
void N3310_putpx(uint8_t x, uint8_t y,uint8_t px)
{
	N3310_CMD_MODE();
	N3310_writeb(x|0x80);	// 0b1xxx xxxx
	N3310_writeb(y|0x40);	// 0b0100 0xxx
	N3310_DATA_MODE();
	N3310_writeb(px);
}
//Function for Initialization of the LCD
void N3310_init()
{
	SSPInit();			/* initialize SSP port, share pins with SPI0
								on port0(p0.15-18). */
	LPC_GPIO0->DIR|=(1<<2);	//P0_2-D/C Pin
	LPC_GPIO0->DATA|=(1<<2);
	LPC_GPIO2->DIR|=(1<<0)+(1<<1);//P2_0-#CS P2_1-RES
	LPC_GPIO2->DATA|=(1<<0)+(1<<1);

	//LCD RESET
	N3310_RESET_EN();
	N3310_RESET_DI();
	N3310_clr();

	//LCD Initialization
	N3310_CMD_MODE(); //Set Command Mode
	N3310_writeb(0x21);	// extended instruction set

	N3310_writeb(0x90);	// set VOP
	N3310_writeb(0x06);	// set temperature co-eff
	N3310_writeb(0x13);	// bias (1:48)

	N3310_writeb(0x20);	// normal instruction set: PD= 0, V= 0 (Horizontal Addressing)
//	N3310_writeb(0x22);	// normal instruction set: PD= 0, V= 1 (Vertical Addressing)
	N3310_writeb(0x0C);	// normal mode: d=1, e=0

	N3310_setXY(0,0);
}
//To Put a Dot in FRAME Buffer
void N3310_fbDot(uint8_t x, uint8_t yLine, uint8_t color) {
	uint8_t yBit;			// 00LL LBBB

	// f x is more than screen x-res, wrap
	while(x>=N3310_COLS)
		x		-= N3310_COLS;
	// if yLine is more than screen y-res, wrap
	while(yLine>=(N3310_ROWS*8))
		yLine	-= (N3310_ROWS*8);


	// separate line number (0-5) from atual pixel in line (0-7)
	//	this is done by divide
	yBit	= yLine & 7;	// 0b0000 0bbb
	yLine	= yLine >> 3;	// yline / 8 ; 0b00l ll00 => 0b0000 0lll

	// if color is diff from frameBuffer, toggle; if color is toggle (2) expression is always true
	if( color ^ (LCDfb[x][yLine] & (1<<yBit))>>yBit ) {
		LCDfb[x][yLine] ^= (1<<yBit);
	}
}
//Clear the Frame Buffer
void N3310_fbClr(void) {
	uint8_t x, yLine;

	for(yLine=0; yLine<N3310_ROWS; yLine++) {
		for(x=0; x<N3310_COLS; x++) {
			LCDfb[x][yLine] = 0x00;
		}
	}
}
//Display the complete Frame Buffer
void N3310_fbDisplay(void) {
	uint8_t x;
	uint8_t yLine;

	N3310_setXY(0,0);

	N3310_DATA_MODE();
	for(yLine=0; yLine<N3310_ROWS; yLine++) {
		for(x=0; x<N3310_COLS; x++) {
			N3310_writeb(LCDfb[x][yLine]);
		}
	}
}
//Write a Single character in theFrame Buffer
void N3310_fbCh(const uint8_t* fontSet, uint8_t ch) {
	uint8_t i, j;
	uint8_t k = fontSet[0];
	uint8_t x = fontSet[1];
	uint8_t y = fontSet[2];

	N3310_DATA_MODE();
	if(ch < k) {
		for(i=0; i<x; i++) {
			for(j=0; j<y; j++) {
				k=0;
				if(fontSet[(x*ch+3)+i] & (1<<j)) {
					k=1;
				}

				N3310_fbDot(LCDfbX,LCDfbY+j,k);
			}
			LCDfbX++;
		}
	}
	else {
		for(j=0; j<y; j++) {
			N3310_fbDot(LCDfbX,LCDfbY+j,0);		// show empty if out of range
		}
		LCDfbX++;
	}
}
//Write a Stream of Characters
void N3310_fbWrite(const uint8_t* fontSet, uint8_t* buf) {
	uint8_t i=0, offset, ch;

	// check if string is in flash/sram and read appropriately
//	if(is_pstr) {
//		ch = pgm_read_byte(buf+i);
//	}
//	else {
		ch = buf[i];
	//}

	do {
		if(ch > 32) {
			offset = 33;

			if( (ch>96) && (ch<123) ) {
				offset = 65;
			}
			else if(ch>122) {
				offset = 60;
			}

			N3310_fbCh(fontSet, ch-offset);
		}
		else {
			N3310_fbCh(fontSet, 255);	// empty (not recognized)
		}
		LCDfbX++;	// char spacing

		i++;

		// check if string is in flash/sram and read appropriately
//		if(is_pstr) {
//			ch = pgm_read_byte(buf+i);
//		}
//		else {
			ch = buf[i];
//		}

	} while(ch != '\0');
}
