/*
 * N3310.h
 *
 *  Created on: Jun 21, 2010
 *      Author: Bose
 *      For Nokia 3310 LCD
 */

#ifndef N3310_H_
#define N3310_H_

#define N3310_DC 2
#define N3310_CS 0
#define N3310_RST 1
#define N3310_CMD_MODE()  (LPC_GPIO0->DATA &=~(1<<N3310_DC))
#define N3310_DATA_MODE() (LPC_GPIO0->DATA |=(1<<N3310_DC))
#define N3310_ENABLE()	(LPC_GPIO2->DATA &=~(1<<N3310_CS))
#define N3310_DISABLE()	(LPC_GPIO2->DATA |=(1<<N3310_CS))
#define N3310_RESET_EN()	(LPC_GPIO2->DATA &=~(1<<N3310_RST))
#define N3310_RESET_DI()	(LPC_GPIO2->DATA |=(1<<N3310_RST))
#define N3310_COLS 84
#define N3310_ROWS 6

extern void N3310_writeb(uint8_t data);
extern void N3310_clr();
extern void N3310_setXY(uint8_t x, uint8_t y);
extern void N3310_putpx(uint8_t x, uint8_t y,uint8_t px);
extern void N3310_init();

extern void N3310_fbDot(uint8_t x, uint8_t yLine, uint8_t color);
extern void N3310_fbClr(void);
extern void N3310_fbDisplay(void);
extern void N3310_fbWrite(const uint8_t* fontSet, uint8_t* buf);

#endif /* N3310_H_ */
