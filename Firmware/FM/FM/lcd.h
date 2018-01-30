#define LCD_RST_set  PORTA |=  (1<<3)    //external reset input
#define LCD_RST_clr  PORTA &=~ (1<<3)
 
#define LCD_DC_set   PORTA |=  (1<<4)    //data/commande
#define LCD_DC_clr   PORTA &=~ (1<<4)
 
#define SDIN_set     PORTA |=  (1<<5)    //serial data input
#define SDIN_clr     PORTA &=~ (1<<5)
 
#define SCLK_set     PORTA |=  (1<<6)    //serial clock input
#define SCLK_clr     PORTA &= ~(1<<6)

#include "english_font.h"

void LCD_write_byte(unsigned char dat, unsigned char command);
void LCD_init(void);
void LCD_clear(void);
void LCD_set_XY(unsigned char X, unsigned char Y);
void LCD_write_char(unsigned char c);
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s);
 
 
void LCD_write_byte(unsigned char dat, unsigned char command)
{
 unsigned char i;
 
 if (command == 1)
 LCD_DC_clr;
 else
 LCD_DC_set;
 
 for(i=0;i<8;i++)
 {
  if(dat&0x80)
  SDIN_set;
  else
  SDIN_clr;
  SCLK_clr;
  dat = dat << 1;
  SCLK_set;
 }
}
 
void LCD_init() 
{
 DDRA |= (1<<PIN3) | (1<<PIN4) | (1<<PIN5) | (1<<PIN6);
 LCD_RST_clr;
 _delay_us(1);
 LCD_RST_set;
 
 _delay_us(1);
 
 LCD_write_byte(0x21, 1); // set LCD mode
// LCD_write_byte(0x13, 1); // 1:48
 LCD_write_byte(0xa1, 1); // Contrast
 LCD_write_byte(0x13, 1); // set bias voltage
 LCD_write_byte(0x04, 1); // temperature correction

 LCD_write_byte(0x20, 1); // use bias command, vertical
 LCD_write_byte(0x0c, 1); // set LCD mode,display normally
  
 LCD_clear();             // clear the LCD
}
 
void LCD_clear() 
{
 unsigned int i;
 
 LCD_write_byte(0x0c, 1);
 LCD_write_byte(0x80, 1);
 
 for (i=0; i<504; i++)
 {
  LCD_write_byte(0, 0);
 }
}
 
void LCD_set_XY(unsigned char X, unsigned char Y)
{
 LCD_write_byte(0x40 | Y, 1); // column
 LCD_write_byte(0x80 | X, 1);    // row
}
 
void LCD_write_char(unsigned char c)
{
 unsigned char line;
 
 c -= 32;
 
 for (line=0; line<6; line++)
 LCD_write_byte(pgm_read_byte(&(font6x8[c][line])), 0);
 
}
 
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s)
{
 LCD_set_XY(X,Y);
 while (*s)
 {
  LCD_write_char(*s);
  s++;
 }
}

