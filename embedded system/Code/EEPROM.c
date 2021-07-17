/******************************************************************************
*
* Module: EEPROM
*
* File Name: EEPROM.h
*
* Description: Source file for the AVR EEPROM driver
*
* Author: Ziad Hazem
*
*******************************************************************************/

#include "EEPROM.H"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description : Function to initialize the EEPROM driver
 * 	1. Enable EEPROM interrupt.
 */
void EEPROM_init(void)
{
	EECR |= 1<<EERIE; /* Enable EEPROM interrupt */
}

/*
 * Description : Function to read a byte.
 */
uint8 EEPROM_readByte(uint16 address)
{
	/* Wait for previous operation to end */
	while(EECR & (1<<EEWE));

	/* Set memory address */
	EEAR = address;

	/* Set read bit */
	EECR = (1<<EERE);

	/* Return read data */
	return EEDR;
}

/*
 * Description : Function to write a Byte.
 */
void EEPROM_writeByte(uint16 address, uint8 data)
{
	/* Wait for previous operation to end */
	while (EECR & (1<<EEWE));

	/* Set memory address */
	EEAR = address;

	/* Set data to be written */
	EEDR = data;

	/* Set write bits */
	EECR |= (1<<EEMWE) | (1<<EEWE);
}
