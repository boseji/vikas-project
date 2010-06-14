/*****************************************************************************
 *   uart.h:  Header file for NXP LPC1xxx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.08.21  ver 1.00    Preliminary version, first Release
 *   2010.06.03  ver 1.01    Modifications by Boseji.
******************************************************************************/
#ifndef __UART_H 
#define __UART_H

#ifndef F_AHB
#error "AHB bus frequency F_AHB definition missing"
#endif
// @@ Modified to have static frequency

#define RS485_ENABLED		0
#define TX_INTERRUPT		0		/* 0 if TX uses polling, 1 interrupt driven. */
#define MODEM_TEST			0

#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80

#define BUFSIZE		0x40

/* RS485 mode definition. */
#define RS485_NMMEN		(0x1<<0)
#define RS485_RXDIS		(0x1<<1)
#define RS485_AADEN		(0x1<<2)
#define RS485_SEL		(0x1<<3)
#define RS485_DCTRL		(0x1<<4)
#define RS485_OINV		(0x1<<5)

extern void ModemInit( void );
extern void UARTInit(uint8_t ahbdivision,uint32_t baudrate);
extern void UARTSendByte(uint8_t data);
extern void UART_IRQHandler(void);
extern void UARTSend(uint8_t *BufferPtr, uint32_t Length);
extern uint8_t UARTRead(void);

#endif /* end __UART_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
