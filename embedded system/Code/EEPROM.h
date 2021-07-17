/******************************************************************************
*
* Module: EEPROM
*
* File Name: EEPROM.h
*
* Description: Header file for the AVR EEPROM driver
*
* Author: Ziad Hazem
*
*******************************************************************************/
#ifndef EEPROM_H_
#define EEPROM_H_

/*******************************************************************************
 *                         Macros Definition                                   *
 *******************************************************************************/
#include "micro_config.h"
#include "std_types.h"

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description : Function to initialize the EEPROM driver
 * 	1. Enable EEPROM interrupt.
 */
void EEPROM_init(void);

/*
 * Description : Function to read a byte.
 */
uint8 EEPROM_readByte(uint16 address);

/*
 * Description : Function to Write a byte.
 */
void EEPROM_writeByte(uint16 address, uint8 data);

#endif /* EEPROM_H_ */
