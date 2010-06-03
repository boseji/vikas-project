/**
 * clock.h
 *
 *  Created on: Jun 2, 2010
 *      Author: Boseji
 */

#ifndef CLOCK_LPC100_H_
#define CLOCK_LPC100_H_
///////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////		
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
		/* MSEL = 00000[1] - 11111[32] for 12MHz=>48MHz MSEL=4 -> 00011*/\
		/* PSEL = 00[1] 01[2] 10[4] 11[8] for 12MHz=>48MHz PSEL = 1 ->00*/\
		LPC_SYSCON->SYSPLLCTRL = (PSEL<<5)|MSEL;\
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
///////////////////////////////////////////////////////////////////////////////
/**
 * \brief WDT Oscillator Divider Values Setings wdt_osc_clk = Fclkana/(2 � (1 + DIVSEL))
 * \param clk: Analog Clock Frequency 0001-0.5MHz to 1111-3.4Mhz 
 *        Values in Order- 0.5,0.8,1.1,1.4,1.6(default),1.8,2.0,2.2,2.6,2.7,2.9,3.1,3.2,3.4
 * \param div: Digital Divider Value 00000[(2x(1+DIVSEL))=2] , 00001[=4] to 11111[=64]
 */
#define SYSCLK_WDTOSC_Config(clk,div) \
	{/*WDT Oscillator Divider Values Setings wdt_osc_clk = Fclkana/(2 � (1 + DIVSEL))*/\
	 /*clk: Analog Clock Frequency 0001-0.5MHz to 1111-3.4Mhz */\
	 /*Values in Order(MHz)- 0.5,0.8,1.1,1.4,1.6(default),1.8,2.0,2.2,2.6,2.7,2.9,3.1,3.2,3.4*/\
	 /*div: Digital Divider Value 00000[(2x(1+DIVSEL))=2] , 00001[=4] to 11111[=64]*/\
	 LPC_SYSCON->WDTOSCCTRL = (clk<<5)|div;\
	}
/**
 * \brief WDT Oscillator Power ON
 */	
#define SYSCLK_WDTOSC_ON()	LPC_SYSCON->PDRUNCFG     &= ~(1 << 6)
/**
 * \brief WDT Oscillator Power OFF
 */	
#define SYSCLK_WDTOSC_OFF()	LPC_SYSCON->PDRUNCFG     |= (1 << 6)
///////////////////////////////////////////////////////////////////////////////
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
 * \note Before changing the clock source make sure its already stable
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
///////////////////////////////////////////////////////////////////////////////
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
/**
 * \brief Set Clock Division Factor for Watchdog Clock
 * \param div: 0 - Disable Clock 1 - Divide By 1 ... 255 - Divide By 255
 */
#define SYSCLK_WDTDIV(div) \
		{/*Clock Division Factors 0 - Disable Clock 1 - Divide By 1 .. 255 - Divide By 255 */\
		LPC_SYSCON->WDTCLKDIV = div; /* SYS WDT clock Divider */\
		}
/**
 * \brief Set Clock Division Factor for ClockOut pin Clock
 * \param div: 0 - Disable Clock 1 - Divide By 1 ... 255 - Divide By 255
 */
#define SYSCLK_CLKOUTDIV(div) \
		{/*Clock Division Factors 0 - Disable Clock 1 - Divide By 1 .. 255 - Divide By 255 */\
		LPC_SYSCON->CLKOUTDIV = div; /* SYS WDT clock Divider */\
		}
///////////////////////////////////////////////////////////////////////////////		
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
#define SYSCLK_Active_SYS 			(1<<0)
#define SYSCLK_Active_ROM 			(1<<1)
#define SYSCLK_Active_RAM 			(1<<2)
#define SYSCLK_Active_FLASHREG 		(1<<3)
#define SYSCLK_Active_FLASHARRAY 	(1<<4)
#define SYSCLK_Active_I2C 			(1<<5)
#define SYSCLK_Active_GPIO 			(1<<6)
#define SYSCLK_Active_CT16B0 		(1<<7)
#define SYSCLK_Active_CT16B1 		(1<<8)
#define SYSCLK_Active_CT32B0 		(1<<9)
#define SYSCLK_Active_CT32B1 		(1<<10)
#define SYSCLK_Active_SPI0 			(1<<11)
#define SYSCLK_Active_UART 			(1<<12)
#define SYSCLK_Active_ADC 			(1<<13)
#define SYSCLK_Active_WDT 			(1<<15)
#define SYSCLK_Active_IOCON 		(1<<16)
#define SYSCLK_Active_SPI1 			(1<<18)
/**
 * \brief Set Clock active for the Modules
 * \param clk the bit pattern for Activation
 */
#define SYSCLK_Enable(clk) LPC_SYSCON->SYSAHBCLKCTRL |= (clk)
/**
 * \brief Deactivate Clock for the Modules
 * \param clk the bit pattern for Deactivation
 */
#define SYSCLK_Disable(clk) LPC_SYSCON->SYSAHBCLKCTRL &= ~(clk)
///////////////////////////////////////////////////////////////////////////////
/**
 * Watchdog Clock Source from Internal RC Oscillator (IRC Clk)
 */
#define WDT_SRC_IRCOSC 0
/**
 * Watchdog Clock Source from Main Clock (Main Clk)
 */
#define WDT_SRC_MCLK   1
/**
 * Watchdog Clock Source from WDT Oscillator (WDT Clk)
 */
#define WDT_SRC_WDTOSC 2
/**
 * \brief Set the Clock Source for Watchdog
 * \param X: Source for the Clock
 * 00-IRC Oscillator[WDT_SRC_IRCOSC]
 * 01-Main Clock[WDT_SRC_MCLK]
 * 10-WDT Oscillator[WDT_SRC_WDTOSCC]
 * \note Before changing the clock source make sure its already stable
 */
#define WDT_Clock_Source(X) \
	{\
	  /* Select Watchdog Clock Source 00-IRC Oscillator 01-Main Clock 10-WDT Oscillator*/\
	  LPC_SYSCON->WDTCLKSEL    = X;\
	  /*Setting Up procedure for Watchdog Clock source*/\
	  {\
		LPC_SYSCON->WDTCLKUEN    = 0x01;               /* Update Watchdog Clock Source */\
		LPC_SYSCON->WDTCLKUEN    = 0x00;               /* Toggle Update Register   */\
		LPC_SYSCON->WDTCLKUEN    = 0x01; \
		while (!(LPC_SYSCON->WDTCLKUEN & 0x01));       /* Wait Until Updated       */\
	  }\
	}
/**
 * \brief Watchdog Reset
 */
#define WDT_Reset()	{LPC_WDT->FEED=0xAA;LPC_WDT->FEED=0x55;LPC_WDT->MOD&=~(1<<2);}
/**
 * \brief Configure Watchdog Reset
 * \param enable: enable the Watchdog clock
 * \param reset: enable Watchdog reset for the Micro
 * \param time: total time for watchdog reset 00 to FF WDT_timeout=TWDCLK � Time � 4
 */	
#define WDT_Config(enable,reset,time) \
	{\
	  LPC_WDT->TC=time;/*Timeout Value 00 to FF WDT_timeout=TWDCLKxTCx4*/\
	  LPC_SYSCON->SYSAHBCLKCTRL|=(1<<15);\
	  LPC_WDT->MOD =(reset<<1)|enable;/*Configure the Watchdog bit0-Enable bit1-Reset*/\
	  if(enable)WDT_Reset();/*Reset Watchdog if Enabled*/\
	}
/**
 * Watchdog current Value Register
 */	
#define WDT_Val LPC_WDT->TV	
///////////////////////////////////////////////////////////////////////////////
/**
 * Clock Out Source as Internal RC Oscillator
 */
#define CLKOUT_SRC_IRCOSC 0
/**
 * Clock Out Source as External Oscillator (Sys Osc)
 */
#define CLKOUT_SRC_EXTOSC 1
/**
 * Clock Out Source as WDT Oscillator
 */
#define CLKOUT_SRC_WDTOSC 2
/**
 * Clock Out Source as Main Clock
 */
#define CLKOUT_SRC_MCLK   3
/**
 * \brief Set the Clock source for Clock Out pin
 * \param src: source 0-IRC Oscillator 1-External Oscillator 2-WDT Oscillator 3-Main Clock
 */
#define CLKOUT_Source(src) \
	{/* Set Clock out source 0-IRC Oscillator 1-External Oscillator 2-WDT Oscillator 3-Main Clock*/\
	 LPC_SYSCON->CLKOUTCLKSEL = src;	/* Select clock source*/\
	 /* Setup Clock out Clock Source */\
	 {\
	    LPC_SYSCON->CLKOUTUEN = 0x01;		/* Update clock */\
	    LPC_SYSCON->CLKOUTUEN = 0x00;		/* Toggle update register once */\
	    LPC_SYSCON->CLKOUTUEN = 0x01;\
	    while ( !(LPC_SYSCON->CLKOUTUEN & 0x01) );		/* Wait until updated */\
	 }\
	}
/**
 * Enable Clock Out
 */
#define CLKOUT_ON() \
{/*Enable the PIO0_1 in CLOCKOUT Function*/LPC_IOCON->PIO0_1 |=0x01;}
/**
 * Disable Clock Out
 */
#define CLKOUT_OFF() \
{/*Disable the PIO0_1 in CLOCKOUT Function*/LPC_IOCON->PIO0_1 &=(~0x01);}

/**
 * \brief Configure CLKOUT.
 * \param clksrc clock source: irc_osc(0), ext_osc(1), wdt_osc(2),main_clk(3).
 */
#define CLKOUT_Config(clksrc) {CLKOUT_Source(clksrc);CLKOUT_ON();}
///////////////////////////////////////////////////////////////////////////////
/**
 * Set the Main Clock Source with Direct Feed from Internal RC Oscillator
 */
#define SYSCLK_Setup_12MHzIRCDirect() \
	{/*Set the Main Clock Source with Direct Feed from Internal RC Oscillator*/\
	SYSCLK_MainClock_Source(SYSCLK_MainClock_SRC_IRCOSC);\
	SYSCLK_ExternalOscillator_OFF();\
	SYSCLK_PLL_OFF();\
	}
/**
 * Set the Main Clock Source with Direct Feed from External Oscillator
 */
#define SYSCLK_Setup_ExternalOscillatorDirect() \
	{/*Set the Main Clock Source with Direct Feed from External Oscillator*/\
	SYSCLK_ExternalOscillator_ON();\
	SYSCLK_PLL_Source(SYSCLK_PLL_SRC_EXTOSC);\
	SYSCLK_MainClock_Source(SYSCLK_MainClock_SRC_PLLINOSC);\
	}
/**
 * Set the Main Clock Source with PLL from Internal RC Oscillator at 24MHz
 */
#define SYSCLK_Setup_24MHzIRCPLL() \
	{\
	SYSCLK_ExternalOscillator_ON();\
	SYSCLK_PLL_Source(SYSCLK_PLL_SRC_IRCOSC);\
	SYSCLK_PLL_Config(1,0);\
	SYSCLK_PLL_ON();\
	SYSCLK_MainClock_Source(SYSCLK_MainClock_SRC_PLLGEN);\
	}
/**
 * Set the Main Clock Source with PLL from Internal RC Oscillator at 36MHz
 */
#define SYSCLK_Setup_36MHzIRCPLL() \
	{\
	SYSCLK_ExternalOscillator_ON();\
	SYSCLK_PLL_Source(SYSCLK_PLL_SRC_IRCOSC);\
	SYSCLK_PLL_Config(2,0);\
	SYSCLK_PLL_ON();\
	SYSCLK_MainClock_Source(SYSCLK_MainClock_SRC_PLLGEN);\
	}
/**
 * Set the Main Clock Source with PLL from Internal RC Oscillator at 48MHz
 */
#define SYSCLK_Setup_48MHzIRCPLL() \
	{\
	SYSCLK_ExternalOscillator_ON();\
	SYSCLK_PLL_Source(SYSCLK_PLL_SRC_IRCOSC);\
	SYSCLK_PLL_Config(3,0);\
	SYSCLK_PLL_ON();\
	SYSCLK_MainClock_Source(SYSCLK_MainClock_SRC_PLLGEN);\
	}

/**
 * Set the Main Clock Source with PLL from Exernal Oscillator at 24MHz
 */
#define SYSCLK_Setup_24MHzExternalOscillatorPLL() \
	{\
	SYSCLK_ExternalOscillator_ON();\
	SYSCLK_PLL_Source(SYSCLK_PLL_SRC_EXTOSC);\
	SYSCLK_PLL_Config(1,0);\
	SYSCLK_PLL_ON();\
	SYSCLK_MainClock_Source(SYSCLK_MainClock_SRC_PLLGEN);\
	}
/**
 * Set the Main Clock Source with PLL from Exernal Oscillator at 36MHz
 */
#define SYSCLK_Setup_36MHzExternalOscillatorPLL() \
	{\
	SYSCLK_ExternalOscillator_ON();\
	SYSCLK_PLL_Source(SYSCLK_PLL_SRC_EXTOSC);\
	SYSCLK_PLL_Config(2,0);\
	SYSCLK_PLL_ON();\
	SYSCLK_MainClock_Source(SYSCLK_MainClock_SRC_PLLGEN);\
	}
/**
 * Set the Main Clock Source with PLL from Exernal Oscillator at 48MHz
 */
#define SYSCLK_Setup_48MHzExternalOscillatorPLL() \
	{\
	SYSCLK_ExternalOscillator_ON();\
	SYSCLK_PLL_Source(SYSCLK_PLL_SRC_EXTOSC);\
	SYSCLK_PLL_Config(3,0);\
	SYSCLK_PLL_ON();\
	SYSCLK_MainClock_Source(SYSCLK_MainClock_SRC_PLLGEN);\
	}
///////////////////////////////////////////////////////////////////////////////

#endif /* CLOCK_H_ */          
