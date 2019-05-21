/*
 * Easy_Management.c
 *
 * Created: 21.05.2019 08:31:28
 * Author : Lukas Frank
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

char high;
char low;

int main(void)
{	
	MCUCR = MCUCR | (1<<JTD);
	MCUCR = MCUCR | (1<<JTD);
	
	CLKPR = 0x80;
	CLKPR = 0x00;
	
	DDRD = DDRD | (1<<DDD2);
	DDRD = DDRD | (1<<DDD3);
	
	PORTD = PORTD | (1<<PORTD3);
	
	//ADC
	DDRF = DDRF &~ (1<<DDF0);	// ADC0
	//PORTF = PORTF | (1<<PORTF0);	//PUll up
	
	ADMUX = ADMUX &~(1<<REFS1);
	ADMUX = ADMUX | (1<<REFS0);		//Vref=AVcc
	
	ADMUX = ADMUX | (1<<ADLAR);		// linksbündig
	
	ADMUX = ADMUX &~ (1<<MUX0);
	ADMUX = ADMUX &~ (1<<MUX1);
	ADMUX = ADMUX &~ (1<<MUX2);
	ADMUX = ADMUX &~ (1<<MUX3);
	ADMUX = ADMUX &~ (1<<MUX4);
	ADCSRB = ADCSRB &~ (1<<MUX5);	//  MUX5 = 0 -> Single ended ADC0 (PF0)
	
	//ADCSRA = ADCSRA | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);	//f_ADC = 125kHz
	ADCSRA = ADCSRA | (1<<ADPS0);
	ADCSRA = ADCSRA | (1<<ADPS1);
	ADCSRA = ADCSRA | (1<<ADPS2);
	
		
	ADCSRA = ADCSRA | (1<<ADEN);	// ADC EIN
	ADCSRA = ADCSRA | (1<<ADIE);	// Interrupt enable
	
	sei();
	
	ADCSRA = ADCSRA | (1<<ADSC);	// Messung Startet
	
	while (1);
}

ISR(ADC_vect)
{	
	PORTD = PORTD ^ (1<<PORTD2);
	
	low = ADCL;
	high = ADCH;
	
	
	if(high <= 204)
	{
		PORTD = PORTD &~ (1<<PORTD3);
	}
	else
	{
		PORTD = PORTD | (1<<PORTD3);
	}
	
	ADCSRA = ADCSRA | (1<<ADSC);	// Wandlung starten
}

