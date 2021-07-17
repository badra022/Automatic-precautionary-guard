/********************************************************************************
 *																				*
 * File Name: main.c															*
 *																				*
 * Description: Application file												*
 *																				*
 * Author: Ziad Hazem															*
 *																				*
 ********************************************************************************/
#include "lcd.h"
#include "adc.h"

#define AC PD0
#define HEATER PD1
#define SERVO PD5
#define OUTER_IR PC0
#define INNER_IR PC1
#define SEATSENSOR PC2
#define BUZZER PC3
#define LED PC4
#define MAX_NUM 3

/********************************************************************************
 * 				Application Related Functions	(Declaration)					*
 ********************************************************************************/
void DDR_set(void);
void Warning_on(uint8);
void Warning_off(uint8);
void LCD_displayStatus(void);
void gateAllow(void);
/********************************************************************************
 * 							Variables Declaration								*
 ********************************************************************************/
uint8 flag = 0;
uint8 g_numOfPeople = 0;
uint32 g_temp;

void main (void)
{
	/********** Initialization Code **********/
	DDR_set();									 /* Set the required direction for GPIO pins */
	LCD_start();								 /* Initialize the LCD and set required directions for its pins */
	ADC_init();									 /* Initialize the ADC to work with interrupts */
	SREG |= (1<<7);								 /* Enable Global Interrupts */
	/**********  Application code   **********/
	while(1)
	{
		/********** Counting people in room section **********/

		/* Write current number of people in room on LCD */
		LCD_goToRowColumn(0,13);
		LCD_displayDecimal(g_numOfPeople);

		/* If the outer IR detects an obstacle first */
		if(BIT_IS_SET(PINC,OUTER_IR))
		{
			while(BIT_IS_CLEAR(PINC,INNER_IR)); 	/* Wait until he reaches the second IR */
			while(BIT_IS_SET(PINC,INNER_IR));		/* Wait until he passes the second IR */
			if(g_numOfPeople < MAX_NUM)
			{
				gateAllow();
				g_numOfPeople++; 						/* Someone entered the room */
			}
			else
			{
				LCD_goToRowColumn(1,0);
				LCD_displayString("Sorry, room FULL");
				flag = 20;
			}
			_delay_ms(50);							/* Delay to prevent slow passing errors */
		}

		/* If the inner IR detects an obstacle first */
		if(BIT_IS_SET(PINC,INNER_IR))
		{
			while(BIT_IS_CLEAR(PINC,OUTER_IR)); 	/* Wait until he reaches the second IR */
			while(BIT_IS_SET(PINC,OUTER_IR));	 	/* Wait until he passes the second IR */
			gateAllow();
			g_numOfPeople--;						/* Someone left the room */
			_delay_ms(50);							/* Delay to prevent slow passing errors */
		}

		/********** Heater & AC section **********/

		/* Calculate the temperature from the ADC value*/
		ADC_readChannel(0);
		g_temp = ((uint32)g_adcResult*150*5)/(1023*1.5);

		/* If temperature goes below 20 */
		if(g_temp < 20)
		{
			SET_BIT(PORTD,HEATER);				/* Turn ON Heater */
			CLEAR_BIT(PORTD,AC);				/* Turn OFF AC */
		}

		/* If temperature goes above 30 */
		else if(g_temp > 30)
		{
			SET_BIT(PORTD,AC); 					/* Turn ON AC */
			CLEAR_BIT(PORTD,HEATER);			/* TURN OFF Heater */
		}

		/* If temperature is between 20 & 30 */
		else
		{
			/* Turn OFF both */
			CLEAR_BIT(PORTD,AC);
			CLEAR_BIT(PORTD,HEATER);
		}


		/********** Warning section **********/

		/* Warning: Someone is setting on a prohibited seat */
		if(BIT_IS_SET(PINC,SEATSENSOR))
		{
			if(flag == 0)
				LCD_displayStatus();
			else
				flag--;
			Warning_on(BUZZER);							/* Turn ON BUZZER */
			Warning_on(LED); 							/* Turn ON LED */
		}
		else
		{
			if(flag == 0)
				LCD_displayStatus();
			else
				flag--;
			Warning_off(BUZZER);						/* Turn OFF BUZZER */
			Warning_off(LED);							/* Turn OFF LED */
		}

	}
}

void DDR_set(void)
{
	DDRC &= ~7;		 		/* Configure pins: PC0,PC1,PC2 as input pins (AND with 11111000) */
	SET_BIT(DDRD,SERVO);	/* Configure OC1A pin as output for SERVO*/
	SET_BIT(DDRD,AC);		/* Configure AC pin as output pin */
	SET_BIT(DDRD,HEATER);	/* Configure HEATER pin as output pin */
	SET_BIT(DDRC,BUZZER); 	/* Configure BUZZER pin as output pin */
	SET_BIT(DDRC,LED); 		/* Configure LED pin as output pin */
}

void LCD_displayStatus(void)
{
	/* If both number of people in room & distancing requirements are met */
	if(BIT_IS_CLEAR(PINC,SEATSENSOR))
	{
		LCD_goToRowColumn(1,0);
		LCD_displayString("Everything is OK");
	}
	/* If distance requirement violated*/
	else
		{
			LCD_goToRowColumn(1,0);
			LCD_displayString("DISTANCE PROBLEM");
		}
}
void Warning_on(uint8 type)
{
	if(type == LED)
		SET_BIT(PORTC,LED); 		/* Turn ON LED */
	if(type == BUZZER)
		SET_BIT(PORTC,BUZZER);		/* Turn ON BUZZER */
}

void Warning_off(uint8 type)
{
	if(type == LED)
		CLEAR_BIT(PORTC,LED);		/* Turn OFF LED */
	if(type == BUZZER)
		CLEAR_BIT(PORTC,BUZZER);	/* Turn OFF BUZZER */
}

void Timer1_Fast_PWM_Init(unsigned short duty_cycle)
{
	TCNT1 = 0;		/* Set timer1 initial count to zero */
	ICR1 = 2499;	/* Set TOP count for timer1 in ICR1 register */

	OCR1A = duty_cycle; /* Set the compare value */

	/* Configure timer control register TCCR1A
     * 1. Clear OC1A on compare match (non inverting mode) COM1A1=1 COM1A0=0
     * 2. Disconnect OC1B  COM1B0=0 COM1B1=0
     * 3. FOC1A=0 FOC1B=0 because these bits are only active in case non-pwm mode
     * 4. Fast Pwm Mode with the TOP in ICR1 WGM10=0 WGM11=1 (Mode Number 14)
	 */
	TCCR1A = (1<<WGM11) | (1<<COM1A1);

	/* Configure timer control register TCCR1B
	 * 1. Fast Pwm Mode with the TOP in ICR1 WGM12=1 WGM13=1 (Mode Number 14)
	 * 2. Prescaler = F_CPU/8
     */
	TCCR1B = (1<<WGM12) | (1<<WGM13) | (1<<CS11);
}

void gateAllow(void)
{
	Timer1_Fast_PWM_Init(124);	/* Set Servo shaft at 0° position by 1.5 ms pulse */
	_delay_ms(500);
	Timer1_Fast_PWM_Init(187);	/* Set Servo shaft at 90° position by 2 ms pulse */
	_delay_ms(500);
}
