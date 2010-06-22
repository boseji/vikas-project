/*
 * eeprom_i2c.h
 *
 *  Created on: Jun 21, 2010
 *      Author: Bose
 *      For EEPROM 24LC16 I2C
 */

#ifndef EEPROM_I2C_H_
#define EEPROM_I2C_H_
//EEPROM Address
#define I2C_ADDR(blk,wr) (0xA0|((blk&0x07)<<1)|(wr&1))
//Delay for EEPROM Write
#define EEPROM_DELAY() \
	{uint32_t i;for (i = 0; i < 0x200000; i++ );	/* Delay after write */}
extern void eeprom_init();
extern void eeprom_write(uint16_t addr,uint8_t data);
extern void eeprom_read(uint16_t addr,uint8_t len);
#endif /* EEPROM_I2C_H_ */
