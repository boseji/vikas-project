/*
 * clock.h
 *
 *  Created on: Jun 2, 2010
 *      Author: Bose
 */

#ifndef CLOCK_LPC100_H_
#define CLOCK_LPC100_H_

/**
 * \brief Configures the External Oscillator to be Turned ON
 */
#define SYSCLK_ExternalOscillator_ON() \
		{ \
		   unsigned int i; \
		  LPC_SYSCON->PDRUNCFG     &= ~(1 << 5);/* Power-up System Osc (Crystal)     */ \
		  LPC_SYSCON->SYSOSCCTRL    = 0;		/* Oscillator Not Bypassed Freq Range 1-20MHz */ \
		  for (i = 0; i < 200; i++) __NOP();	/* Wait for Stabilization */ \
		}
/**
 * \brief Configures the External Oscillator to be Turned Off
 * \note If this turn OFF is being performed where in External Oscillator is
 * the Main Clock or PLL clock input, then first change the clock source and
 * only then perform this operation
 */
#define SYSCLK_ExternalOscillator_OFF() \
		{ \
		   unsigned int i; \
		  LPC_SYSCON->PDRUNCFG     |= (1 << 5);/* Power-up System Osc (Crystal) 1-Power Down 0-Power Up*/ \
		  LPC_SYSCON->SYSOSCCTRL    = 1;		/* Oscillator Bypassed Freq Range 1-20MHz */ \
		  for (i = 0; i < 200; i++) __NOP();	/* Wait for Stabilization */ \
		}
/**
 * PLL Clock Source Option for External Oscillator (Sys Clock)
 */
#define SYSCLK_PLL_SRC_EXTOSC 1
/**
 * PLL Clock Source Option for Internal RC Oscillator (IRC Oscillator)
 */
#define SYSCLK_PLL_SRC_IRCOSC 0
/**
 * \brief Select the Input Source for the PLL
 * \param X:
 * 1-System Oscillator[SYSCLK_PLL_SRC_EXTOSC]
 * 0-IRC Oscillator[SYSCLK_PLL_SRC_IRCOSC]
 */
#define SYSCLK_PLL_Source(X) \
		{/* Select PLL Input 1-System Osc 0-IRC Osc    */\
		LPC_SYSCON->SYSPLLCLKSEL  = X; \
		/*Setting Procedure for PLL input Source*/\
			{\
			  LPC_SYSCON->SYSPLLCLKUEN  = 0x01;               /* Update Clock Source      */\
			  LPC_SYSCON->SYSPLLCLKUEN  = 0x00;               /* Toggle Update Register   */\
			  LPC_SYSCON->SYSPLLCLKUEN  = 0x01;	\
			  while (!(LPC_SYSCON->SYSPLLCLKUEN & 0x01));     /* Wait Until Updated       */\
			}\
		}
/**
 * \brief Set the PLL Division Factors Fpll-out = Fin * MSEL / PSEL
 * \param MSEL: Multiplier value = 0000[1] - 1111[32] for 12MHz=>48MHz MSEL=4 -> 00011
 * \param PSEL: Post-Divider value = 00[1] 01[2] 10[4] 11[8] for 12MHz=>48MHz PSEL = 1 ->00
 * \note  PLL input Frequency Range 10-25MHz (Fin) so first set the Input Frequency and
 * then use this us configure the PLL
 */
#define SYSCLK_PLL_Config(MSEL,PSEL) \
		{\
		/*Setting the PLL Division Factors Fpll-out = Fin * MSEL / PSEL*/\
		/* PLL input Freq Range 10-25MHz (Fin)*/\
		/* MSEL = 0000[1] - 1111[32] for 12MHz=>48MHz MSEL=4 -> 00011*/\
		/* PSEL = 00[1] 01[2] 10[4] 11[8] for 12MHz=>48MHz PSEL = 1 ->00*/\
		LPC_SYSCON->SYSPLLCTRL = (PSEL<<4)|MSEL;\
		}
/**
 * \brief Turn ON PLL and wait for Stability
 * \note Before calling this function the PLL must be properly initialized
 */
#define SYSCLK_PLL_ON() \
		{\
		LPC_SYSCON->PDRUNCFG     &= ~(1 << 7);          /* Power-up SYSPLL          */\
		while (!(LPC_SYSCON->SYSPLLSTAT & 0x01));	    /* Wait Until PLL Locked    */\
		}
/**
 * \brief Turn OFF the PLL
 * \note Before calling this function make sure that you have switched
 * the main clock to some other clock source
 */
#define SYSCLK_PLL_OFF() LPC_SYSCON->PDRUNCFG |= (1 << 7)
/**
 * Main Clock Source Option for Internal RC Oscillator (IRC Clock)
 */
#define SYSCLK_MainClock_SRC_IRCOSC 0
/**
 * Main Clock Source Option for Input to PLL Oscillator (PLL Fin Clock)
 */
#define SYSCLK_MainClock_SRC_PLLINOSC 1
/**
 * Main Clock Source Option for Watchdog Oscillator (WDT Clock)
 */
#define SYSCLK_MainClock_SRC_WDTOSC 2
/**
 * Main Clock Source Option for PLL Clock Output (Sys PLL Clock out)
 */
#define SYSCLK_MainClock_SRC_PLLGEN 3
/**
 * \brief Select the clock source for the Main Clock
 * \param X: Source for the Clock
 * 00-IRC Oscillator[SYSCLK_MainClock_SRC_IRCOSC]
 * 01-Input clock to system PLL[SYSCLK_MainClock_SRC_PLLINOSC]
 * 10-WDT oscillator[SYSCLK_MainClock_SRC_WDTOSC]
 * 11-System PLL clock out[SYSCLK_MainClock_SRC_PLLGEN]
 *  \note Before changing the clock source make sure its already stable
 */
#define SYSCLK_MainClock_Source(X) \
		{\
		  /* Select Main Clock Source 00-IRC Oscillator 01-Input clock to system PLL */\
		  /* 10-WDT oscillator 11-System PLL clock out */\
	      LPC_SYSCON->MAINCLKSEL    = X;\
	      /*Setting Up procedure for Main Clock*/\
		  {\
		    LPC_SYSCON->MAINCLKUEN    = 0x01;               /* Update MCLK Clock Source */\
		    LPC_SYSCON->MAINCLKUEN    = 0x00;               /* Toggle Update Register   */\
		    LPC_SYSCON->MAINCLKUEN    = 0x01; \
		    while (!(LPC_SYSCON->MAINCLKUEN & 0x01));       /* Wait Until Updated       */\
		  }\
		}
/**
 * \brief Set Clock Division Factor for AHB Clock
 * \param div: 0 - Disable Clock 1 - Divide By 1 ... 255 - Divide By 255
 */
#define SYSCLK_AHBDIV(div) \
		{/*Clock Division Factors 0 - Disable Clock 1 - Divide By 1 .. 255 - Divide By 255 */\
		LPC_SYSCON->SYSAHBCLKDIV = div; /* SYS AHB clock Divider */\
		}
/**
 * \brief Set Clock Division Factor for SPI0 Clock
 * \param div: 0 - Disable Clock 1 - Divide By 1 ... 255 - Divide By 255
 */
#define SYSCLK_SPI0DIV(div) \
		{/*Clock Division Factors 0 - Disable Clock 1 - Divide By 1 .. 255 - Divide By 255 */\
		LPC_SYSCON->SSP0CLKDIV = div; /* SYS SPI0 clock Divider */\
		}
/**
 * \brief Set Clock Division Factor for SPI1 Clock
 * \param div: 0 - Disable Clock 1 - Divide By 1 ... 255 - Divide By 255
 */
#define SYSCLK_SPI1DIV(div) \
		{/*Clock Division Factors 0 - Disable Clock 1 - Divide By 1 .. 255 - Divide By 255 */\
		LPC_SYSCON->SSP1CLKDIV = div; /* SYS SPI1 clock Divider */\
		}
/**
 * \brief Set Clock Division Factor for UART Clock
 * \param div: 0 - Disable Clock 1 - Divide By 1 ... 255 - Divide By 255
 */
#define SYSCLK_UARTDIV(div) \
		{/*Clock Division Factors 0 - Disable Clock 1 - Divide By 1 .. 255 - Divide By 255 */\
		LPC_SYSCON->UARTCLKDIV = div; /* SYS UART clock Divider */\
		}

#endif /* CLOCK_H_ */
