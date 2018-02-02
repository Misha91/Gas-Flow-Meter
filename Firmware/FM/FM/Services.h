#include "stdlib.h"
#define helloLenght 3

volatile uint8_t timerPrescaler = 0;
volatile uint8_t sec = 0;
volatile uint8_t idle = 0;

volatile uint16_t count = 0;

volatile char fanStatus = 0;
volatile char intChanged = 0;
//volatile char fanOn = 0;

void helloScreen(void);

void uinttochar(char* a, uint16_t n); // transferring uint16_t into a string
void everySecondRoutine(void);

void sleepRoutine(void);
void wakeUpRoutine(void);

/*void initADC(void); // Int temp sensor
void adcRoutine(void);
volatile uint8_t adc_lobyte; // to hold the low byte of the ADC register (ADCL)
volatile int raw_temp;*/


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
	if (sec <= 3) {
		LCD_clear();
		helloScreen();
	}
	
	else {			
		char buf1[10];
			
		if (count <= 35) count = 0;
		
		if (count == 0 && idle <= 70) idle++;
		else if (count != 0) idle = 0;
		
		/*if (!(sec % 2)) {
			
			adcRoutine();
			itoa(raw_temp, buf2, 10);
		
		}*/
				
		
		uint16_t result = count* 22 / 200 + 52;		// RPM to LPM conversion
		
		uinttochar(buf1, result);
		
				
							
		LCD_write_english_string(27, 0, "Flow:");
		LCD_write_english_string(35, 2, "     ");
		LCD_write_english_string(0, 4, "                 ");
		
		/*LCD_write_english_string(9, 5, "Temp:");
		LCD_write_english_string(44, 5, buf2);
		
		if (raw_temp < 0){	
			LCD_write_english_string(65, 5, "C");
		}
		
		else if (raw_temp >= 0){
			LCD_write_english_string(60, 5, "C");
		}*/
		
		
		if (result <= 55) {
			LCD_write_english_string(20, 3, "                    ");
			LCD_write_english_string(20, 2, "                    ");
			LCD_write_english_string(20, 3, "Too low!");
	
		}
		
		else if (result < 100) {
			LCD_write_english_string(15, 3, "          ");
			LCD_write_english_string(20, 4, "          ");
			LCD_write_english_string(35, 3, buf1);
			LCD_write_english_string(25, 4, "L/min");
		//	LCD_write_english_string(20, 4, buf2);
		}
		
		else {
			LCD_write_english_string(15, 3, "           ");
			LCD_write_english_string(20, 4, "           ");
			LCD_write_english_string(31, 3, buf1);
			LCD_write_english_string(25, 4, "L/min");
		//	LCD_write_english_string(20, 4, buf2);
			}
									
		count = 0;
	}
}

void sleepRoutine(){
	
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sleep_bod_disable();
	
	PORTA &= !(1 << PINA7); // Back light disable
	
	LCD_RST_clr; // LCD off
	LCD_DC_clr;
	SDIN_clr;
	SCLK_clr;
	
	TIMSK1 &= ~(1<<OCIE1A); // Timer off

	_delay_ms(1000); // Preventing from occasional restart
	
	sei();
	sleep_cpu();
}

void wakeUpRoutine(){
	
	sleep_disable();
		
	PORTA |= 1 << PINA7; // Back Light ON
	LCD_init(); // LCD ON
	//initADC();
	
	TIMSK1 |=  (1<<OCIE1A); // Timer ON
	sec = 0; // reseting ON timer
	count = 0;
	idle = 0;
	
}

void helloScreen(void){
	LCD_write_english_string(32, 0, "(c) ");
	LCD_write_english_string(0, 1, "Mikhail Ivanov");
	LCD_write_english_string(0, 2, "         ");
	LCD_write_english_string(0, 3, "mivanov@3m.com");
	LCD_write_english_string(28, 5, "v1.3");
	if (sec == 3) LCD_clear();
		
}

/*
void initADC(void){
	ADMUX =
	(0 << ADLAR) |     // do not left shift result (for 10-bit values)
	(1 << REFS1) |     // Sets ref. voltage to internal 1.1V, bit 1
	(0 << REFS0) |     // Sets ref. voltage to internal 1.1V, bit 0
	(1 << MUX5)  |     // use ADC1 for input (PB2), MUX bit 5
	(0 << MUX4)  |     // use ADC1 for input (PB2), MUX bit 4
	(0 << MUX3)  |     // use ADC1 for input (PB2), MUX bit 3
	(0 << MUX2)  |     // use ADC1 for input (PB2), MUX bit 2
	(1 << MUX1)  |     // use ADC1 for input (PB2), MUX bit 1
	(0 << MUX0);       // use ADC1 for input (PB2), MUX bit 0

	ADCSRA =
	(1 << ADEN)  |     // Enable ADC
	(1 << ADPS2) |     // set prescaler to 128, bit 2
	(1 << ADPS1) |     // set prescaler to 128, bit 1
	(1 << ADPS0);      // set prescaler to 128, bit 0
}

void adcRoutine(void){
	set_sleep_mode(SLEEP_MODE_ADC);
	sleep_enable();	
	sei();
	sleep_cpu();
	
	ADCSRA |= (1 << ADSC);         // start ADC measurement
	while (ADCSRA & (1 << ADSC) ); // wait till conversion complete

	// for 10-bit resolution:
	adc_lobyte = ADCL; // get the sample value from ADCL
	raw_temp = ((ADCH<<8 | adc_lobyte) * 89 / 100 - 234);   // add lobyte and hibyte, convert them to the temperature
	sleep_disable();
	cli();
}
*/