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


#include "lcd.h"

/********************************************************************************
 *							Functions Definitions								*
 ********************************************************************************/
void LCD_sendCommand(uint8 command)
{
	LCD_CTRL_PORT &= ~(1<<RS);
	LCD_CTRL_PORT &= ~(1<<RW);
	_delay_ms(1);
	LCD_CTRL_PORT |= (1<<E);
	_delay_ms(1);
	LCD_DATA_PORT = command;
	_delay_ms(1);
	LCD_CTRL_PORT &= ~(1<<E);
	_delay_ms(1);
}

void LCD_displayCharacter(uint8 data)
{
	LCD_CTRL_PORT |= (1<<RS);
	LCD_CTRL_PORT &= ~(1<<RW);
	_delay_ms(1);
	LCD_CTRL_PORT|= (1<<E);
	_delay_ms(1);
	LCD_DATA_PORT= data;
	_delay_ms(1);
	LCD_CTRL_PORT &= ~(1<<E);
	_delay_ms(1);
}

void LCD_clear(void)
{
		LCD_sendCommand (0x01);		/* clear display */
		LCD_sendCommand (0x80);		/* cursor at home position */
}

void LCD_init(void)
{
	LCD_CTRL_PORT |= (1<<RS)|(1<<RW)|(1<<E);
	LCD_DATA_PORT = 0xFF;
	LCD_sendCommand(0x38);
	LCD_sendCommand(0x0C);
	LCD_sendCommand(0x01); // Clear screen
}

void LCD_displayString(uint8 *string)
{
	uint8 i = 0;
	while(string[i] != '\0')
	{
		LCD_displayCharacter(string[i]);
		i++;
	}
}

void LCD_goToRowColumn(uint8 row,uint8 col)
{
	uint8 Address;
	
	/* first of all calculate the required address */
	switch(row)
	{
		case 0:
				Address=col;
				break;
		case 1:
				Address=col+0x40;
				break;
		case 2:
				Address=col+0x10;
				break;
		case 3:
				Address=col+0x50;
				break;
	}					
	/* to write to a specific address in the LCD 
	 * we need to apply the corresponding command 0b10000000+Address */
	LCD_sendCommand(Address | SET_CURSOR_LOCATION); 
}

void LCD_start(void)
{
	LCD_DATA_PORT_DIR = 0xFF;
	LCD_CTRL_PORT_DIR |= (1<<RS);
	LCD_CTRL_PORT_DIR |= (1<<RW);
	LCD_CTRL_PORT_DIR |= (1<<E);
	LCD_init();
	LCD_sendCommand(0x0F);
	LCD_clear();
	LCD_displayString("PPL in room: ");
	LCD_displayStatus();
}

void LCD_displayDecimal(uint8 number)
{
	uint8 num2;
	if(number < 10)
	{
		LCD_displayCharacter(number + 48);
	}
	if(number > 10)
	{
		num2 = number / 10;
		number = number % 10;
		LCD_displayCharacter(num2 + 48);
		LCD_displayCharacter(number + 48);
	}
}
