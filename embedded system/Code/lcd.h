/******************************************************************************
*
* Module: lCD
*
* File Name: LCD.h
*
* Description: Header file for the LCD driver
*
* Author: Ziad Hazem
*
*******************************************************************************/

#ifndef LCD_H_
#define LCD_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/* LCD HW Pins */
#define RS PA7
#define RW PA6
#define E PA5
#define LCD_DATA_PORT PORTB
#define LCD_CTRL_PORT PORTA
#define LCD_DATA_PORT_DIR DDRB
#define LCD_CTRL_PORT_DIR DDRA

/* LCD Commands */
#define CLEAR_COMMAND 0x01
#define FOUR_BITS_DATA_MODE 0x02
#define TWO_LINE_LCD_Four_BIT_MODE 0x28
#define TWO_LINE_LCD_Eight_BIT_MODE 0x38
#define CURSOR_OFF 0x0C
#define CURSOR_ON 0x0E
#define SET_CURSOR_LOCATION 0x80 

/*******************************************************************************
 *						Functions Prototypes (Private)						   *
 *******************************************************************************/
void LCD_start(void);
void LCD_clear(void);
void LCD_sendCommand(uint8 command);
void LCD_displayCharacter(uint8 data);
void LCD_init(void);
void LCD_displayString(uint8 *string);
void LCD_goToRowColumn(uint8 row, uint8 col);
void LCD_displayDecimal(uint8 number);
void LCD_displayStatus(void);
#endif /* LCD_H_ */
