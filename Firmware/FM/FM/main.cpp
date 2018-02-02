/*
 * FM.cpp
 *
 * Created: 04.09.2017 10:52:23
 * Author : Mikhail Ivanov mikhail.ivanov@rocketmail.com
 */ 
#define F_CPU 12000000
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
	
	DDRA |= (1 << PINA7); // OUTPUTs: LED, Fan, LCD back light pins
	PORTB |= (1 << PINB2); // High reading button
	
	
	// setting 1s timer
	TCCR1B|=( (1<<WGM12)|(1<<CS11)|(1<<CS10) ); // WGM Mode, Prescaler is set to 64
	OCR1A = 1875; // 187500 = 1 sec, 1875 = 0,01 sec
	TIMSK1 |=  (1<<OCIE1A);
	
	MCUCR|= (1 << ISC01) | (1 << ISC00); // Button trigers on raise
	
	wdt_reset(); // Watch dog reset
	wdt_enable(WDTO_1S); // WD enable with 1s prescaler
	WDTCSR |= (1 << WDIE); // WD interrupt enable
		
	sei();
	
	// Button interruption
	
	PCMSK0 |= (1 << PCINT0);
	GIMSK |= (1 << INT0) | (1 << PCIE0);
	GIFR |= 1<<PCIF0;
	
	PORTA |= 1 << PINA7; // Back Light ON
	
	sleepRoutine(); // sleep after start up
	
	
    while (1) 
    {
		
    }
	
}

ISR(TIM1_COMPA_vect)
{
	//CPU Jumps here every 1 sec exactly!
	cli();
	
	timerPrescaler++;
	
	if (timerPrescaler >= 99){ 
		
		sec++;
		if (sec == 245) sec = 5;
		timerPrescaler = 0;
		if (idle >= 60)	sleepRoutine();		// sleep routine			
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
	
	if (MCUCR & (1 << SE)) wakeUpRoutine(); // if OFF, turns OFF
	else if ((!(MCUCR & (1 << SE))) && sec >= 3) sleepRoutine(); // if ON, turns OFF
	
	sei();
}

