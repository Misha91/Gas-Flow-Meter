#include "stdlib.h"

volatile uint8_t timerPrescaler = 0;
volatile uint8_t sec = 0;
volatile uint8_t idle = 0;


volatile uint16_t count = 0;

volatile char fanStatus = 0;
volatile char intChanged = 0;
volatile char fanOn = 0;


void uinttochar(char* a, uint16_t n); // convert int to string for LCD
void everySecondRoutine(void);

void sleepRoutine(void);
void wakeUpRoutine(void);


// transfering uint16_t into a string


void uinttochar(char* a, uint16_t n)
{

	if (n == 0)
	{
		*a = '0';
		*(a+1) = '\0';
		return;
	}
	
	char aux[20];
	aux[19] = '\0';
	char* auxp = aux + 19;
	
	int c = 1;
	while (n != 0)
	{
		int mod = n % 10;
		*(--auxp) = mod | 0x30;
		n /=  10;
		c++;
	}
	
	memcpy(a, auxp, c);

}


void everySecondRoutine(void){
		
		if (count <= 20) count = 0;
		
		if (count == 0 && idle <= 70) idle++;
		else if (count != 0) idle = 0;
		
		char buf1[10];
		
		
		uint8_t result = count / 9 + 55;		// 0.1075 * x + 53.05
		
		//float result = 1;
	//	dtostrf(result, 4, 2, buf1);
		uinttochar(buf1, result);						
		LCD_write_english_string(27, 0, "Flow:");
		LCD_write_english_string(35, 2, "     ");
		if (result <= 57) {
			LCD_write_english_string(20, 3, "Too low!");
			LCD_write_english_string(20, 4, "                    ");
		}
		else if (result < 100) {
			LCD_write_english_string(15, 3, "                           ");
			LCD_write_english_string(35, 3, buf1);
			LCD_write_english_string(25, 4, "L/min");
		}
		
		else {
			LCD_write_english_string(15, 3, "                           ");
			LCD_write_english_string(31, 3, buf1);
			LCD_write_english_string(25, 4, "L/min");
		}
					
		count = 0;
	
}

void sleepRoutine(){
	
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sleep_bod_disable();
	
	PORTA &= ~(1 << PINA1); // LED OFF
	PORTA &= ~(1 << PINA2); // Fan disable
	PORTA &= !(1 << PINA7); // Back light disable
	
	LCD_RST_clr; // LCD off
	LCD_DC_clr;
	SDIN_clr;
	SCLK_clr;
	
	TIMSK1 &= ~(1<<OCIE1A); // Timer off
	
	sei();
	sleep_cpu();
}

void wakeUpRoutine(){
	
	sleep_disable();
	sec = 0; // reseting ON timer
	PORTA |= 1 << PINA1; // LED ON
	PORTA |= 1 << PINA7; // Back Light ON
	
	LCD_init(); // LCD ON
	TIMSK1 |=  (1<<OCIE1A); // Timer ON
	sec = 0;
	count = 0;
	idle = 0;
		
}