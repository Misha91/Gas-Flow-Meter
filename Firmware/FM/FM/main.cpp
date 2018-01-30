/*
 * FM.cpp
 *
 * Created: 04.09.2017 10:52:23
 * Author : A5TL9ZZ
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include "lcd.h"
#include "Services.h"

int main(void)
{
	LCD_init();
	
	DDRA |= (1 << PINA1) | (1 << PINA2) | (1 << PINA7); // OTPUTs: LED, Fan, LCD back light pins
	PORTB |= (1 << PINB2); // High reading button
	
	// setting 1s timer
	TCCR1B|=( (1<<WGM12)|(1<<CS12) );
	OCR1A = 625; // 62500 = 1 sec, 625 = 0,01 sec
	TIMSK1 |=  (1<<OCIE1A);
	
	MCUCR|= (1 << ISC01) | (1 << ISC00);
	
	wdt_reset(); // Watch dog reset
	wdt_enable(WDTO_1S); // WD enable with 1s prescaler
	WDTCSR |= (1 << WDIE); // WD interrupt enable
	
	sei();
	PCMSK0 |= (1 << PCINT0);
	GIMSK |= (1 << INT0) | (1 << PCIE0);
	GIFR |= 1<<PCIF0;
	
	
	
    while (1) 
    {
		//PORTA |= 1 << PINA2;	
    }
}

ISR(TIM1_COMPA_vect)
{
	//CPU Jumps here every 1 sec exactly!
	cli();
	timerPrescaler++;
	
	/*if (!(timerPrescaler % 3) && fanOn == 0){
		fanOn = 1;
		PORTA |= 1 << PINA2;
	}
	
	else if ((timerPrescaler % 3) && (fanOn == 1)){
		fanOn = 0;
		PORTA &= ~(1 << PINA2);
	}*/
	
	
	if ((timerPrescaler >= 10) && (timerPrescaler < 25) && (count <= 3) && (fanStatus == 0)){
		
		PORTA |= 1 << PINA2;
		PCMSK0 &= ~(1 << PCINT0);
		intChanged = 1;
		fanStatus = 1;
						
	}
	
	if ((timerPrescaler >= 25) && (fanStatus == 1)){
	
		fanStatus = 0;
		PORTA &= ~(1 << PINA2);
				
	}
	
		
	if (timerPrescaler >= 99){
		
		if (intChanged == 1){
			
			PCMSK0 |= (1 << PCINT0);
			intChanged = 0;
		}
		
		sec++;
		timerPrescaler = 0;
		
			
		if (idle >= 60){
			// sleep routine			
			sleepRoutine();				
		}
					
		everySecondRoutine();
	}
	
	sei();
}

ISR(PCINT0_vect)
{
	//counts interrupts
	count++;
	
}

ISR (WDT_vect) {
	
	WDTCSR |= (1 << WDIE); // WD interrupt enable - reseting WD
}


ISR (EXT_INT0_vect){
	
	cli();
	
	if (MCUCR & (1 << SE)) wakeUpRoutine();
	else if (!(MCUCR & (1 << SE))) sleepRoutine();
	
	sei();
}


