/*
 * eeprom_i2c.c
 *
 *  Created on: Jun 21, 2010
 *      Author: Bose
 *      For EEPROM 24LC16 I2C
 */
#include "type.h"
#include "eeprom_i2c.h"
#include "i2c.h"
void eeprom_init()
{
	if ( I2CInit( (uint32_t)I2CMASTER ) == FALSE )	/* initialize I2c */
		  {
			while ( 1 );				/* Fatal error */
		  }
}
void eeprom_write(uint16_t addr,uint8_t data){
		 /* Write SLA(W), address and one data byte */
		   I2CWriteLength = 3;
		   I2CReadLength = 0;
		   I2CMasterBuffer[0] = I2C_ADDR((addr>>8),0);
		   I2CMasterBuffer[1] = addr&0x0ff;		/* address */
		   I2CMasterBuffer[2] = data;		/* Data0 */
		   I2CEngine();
		 }
void eeprom_read(uint16_t addr,uint8_t len){
    	/* Write SLA(W), address, SLA(R), and read one byte back. */
		   I2CWriteLength = 2;
		   I2CReadLength = len;
		   I2CMasterBuffer[0] = I2C_ADDR((addr>>8),0);
		   I2CMasterBuffer[1] = (addr&0x0ff);		/* address */
		   I2CMasterBuffer[2] = I2C_ADDR(0,1);
		   I2CEngine();
	   }
