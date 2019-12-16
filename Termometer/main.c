/*
 * Termometer.c
 *
 * Created: 09-12-2019 14:44:51
 * Author : robe1819
 */ 

#define F_CPU 16E6
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "stdio_setup.h"
#include <avr/interrupt.h>
#include "lcd.h"

int main(void)
{
	char buffer[7];
	unsigned int ADC_data;
	float temp;
	float tempK;
	int tempC = 0;

	lcd_init(LCD_DISP_ON_CURSOR_BLINK);
	
	ADMUX |= (1<<REFS0);
	DIDR0 |= (1<<ADC0D);
	ADCSRA = (1<<ADEN) | (1<<ADSC) | (1<<ADPS2) | (1<<ADPS1) | (ADPS0);
	
	while(1)
	{	
		lcd_clrscr();
		
		if(!(ADCSRA & (1 << ADSC)))
		{
			ADC_data = ADC;
			temp = log(10000.0 * (1024.0 / ADC_data - 1));
			tempK = 1/(0.001129148 + (0.000234125 + (0.0000000876741 * temp * temp)) * temp);
			tempC = tempK - 273.15;
		}
		
		sprintf(buffer, "Temp: %d C", tempC);
		lcd_puts(buffer);
					
		ADCSRA |= (1<<ADSC);
		
		_delay_ms(800);
	}
}
