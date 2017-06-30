#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#define DIN_1 		PORTB |= _BV(PB2)
#define DIN_0 		PORTB &= ~_BV(PB2)// data in	->	MOSI
#define CLK_1 		PORTB |= _BV(PB1)
#define CLK_0 		PORTB &= ~_BV(PB1) // clocking the data to the LCD ->	SCLK
#define CS_1 		PORTB |= _BV(PB4)
#define CS_0 		PORTB &= ~_BV(PB4) // chip enable, active low	->	SS
#define RS_1 		PORTB |= _BV(PB6)
#define RS_0 		PORTB &= ~_BV(PB6) // data (high)/character(low)
#define RESET_1 	PORTH |= _BV(PH0)
#define RESET_0 	PORTH &= ~_BV(PH0) // reset pin

#define 	POWER_LED_ON		DDRG |= (1<<PG2);    PORTG |= (1<<PG2)
#define 	POWER_LED_OFF		PORTG &=~ (1<<PG2);  DDRG &=~ (1<<PG2)

#define 	POWER_GREEN_LED_OFF		PORTJ &=~ (1<<PJ5);

#define SPI_Port	PORTB
#define SPI_DataDir	DDRB

void fn_init_wifi_lcd(void);

void lcd_Cmnd(unsigned char DATA);
void lcd_Data(unsigned char *DATA);
void lcd_Reset(void);
void lcd_init(void);
void lcd_setXY(char x, char y);
void lcd_clear(int row);
void lcd_image(const unsigned char *image_data);
//void fn_Time(unsigned int counter);

void print_text(unsigned char *DATA, uint8_t row, uint8_t column);
void print_int(unsigned int ui_print_value, uint8_t row, uint8_t column);
void print_char(unsigned char DATA, uint8_t row, uint8_t column);

void fn_Reset_SRMode(void);
void fn_Enable_Interrupt_Sleep(void);
void fn_Shutdown(void);

typedef enum { false, true } bool;
extern volatile bool g_SleepFlag;		 // Sleep Flag = Set to 0 to make Brain Sleep, else 1	
extern volatile bool g_FirstSleep;	
extern volatile bool g_Stop_RoboG;				// True when wifi is on
extern volatile bool g_Execute_App;
extern volatile unsigned int test_variable;