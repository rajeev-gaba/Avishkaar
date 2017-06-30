#include "lcd.h"
#include "spi.h"
#include "sensor.h"
#include "wifi.h"

volatile bool g_Stop_RoboG = false;
volatile bool g_SleepFlag = true;		 // Sleep Flag = 0 to make Brain Sleep, else 1	
volatile bool g_FirstSleep = true;		// is set to 0 for First Time Sleep in Brain, else 1*/
volatile unsigned int test_variable=0;
unsigned char ASCII[][5] =
{
 {0x00, 0x00, 0x00, 0x00, 0x00} // 20  
,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c ¥
,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j 
,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ?
,{0x78, 0x46, 0x41, 0x46, 0x78} // 7f ?
};

 void lcd_Cmnd(unsigned char DATA)
{
	unsigned char recve_data;
	RS_0;		    /* make DC pin to logic zero for command operation */
	CS_0;				/* enable SS pin to slave selection */	
	
    recve_data = ucfn_Spi_Write(DATA);				/* send data on data register */
	RS_1;		    /* make DC pin to logic high for data operation */
	CS_1;
	recve_data='x';
}
void lcd_Data(unsigned char *DATA)
{
	RS_1;								/* make DC pin to logic high for data operation */
	CS_0;									/* enable SS pin to slave selection */
	int lenan = strlen(DATA);						/* measure the length of data */
	for (int g=0; g<lenan; g++)
	{
		for (int index=0; index<5; index++)
		{
			ucfn_Spi_Write(ASCII[DATA[g] - 0x20][index]);	/* send the data on data register */			
		}
		ucfn_Spi_Write(0x00);
	}							
	CS_1;									
}
void lcd_Reset()					/* reset the Display at the beginning of initialization */
{
	RESET_0;
	_delay_ms(100);
	RESET_1;
}

void lcd_init()
{
	DDRE|=(1<<PE2);
	PORTE|=(1<<PE2);
	DDRB|=(1<<PB4)|(1<<PB6);
	DDRH|=(1<<PH0);
	fn_Initialize_Spi_nokia_lcd();
	lcd_Reset();					/* reset the display */
	lcd_Cmnd(0x21);				/* command set in addition mode */ // function mode set command
	lcd_Cmnd(0xC0);				/* set the voltage by sending C0 means VOP = 5V */
	lcd_Cmnd(0x07);				/* set the temp. coefficient to 3 */
	lcd_Cmnd(0x13);				/* set value of Voltage Bias System */
	lcd_Cmnd(0x20);				/* command set in basic mode */     // function mode set command
	lcd_Cmnd(0x0C);				/* display result in normal mode */ 
}

void lcd_setXY(char x, char y)		/* set the column and row */
{
	lcd_Cmnd(x);
	lcd_Cmnd(y);
}

void lcd_clear(int row)					/* clear the Display */
{
	uint8_t row_val;

	if(row == 11)
	{
		CS_0;
		RS_1;
		for (int k = 0; k < 504; k++)
		{
			ucfn_Spi_Write(0x00);		
		}
		
		RS_0;
		CS_1;
	}
	else if(row == 10)
	{
		CS_0;
		RS_1;
		for (int k = 0; k < 504; k++)
		{
			ucfn_Spi_Write(0x00);		
		}
		
		RS_0;
		CS_1;
		
		print_text("Wifi on",1, 7);
	}
	else
	{
		row_val = 0x40;
		row_val = (row_val)|(row-1);
		
		//lcd_Cmnd (0x80);   //column
		//lcd_Cmnd (0x40 | (row*8));   //row
		lcd_Cmnd(row_val);
		lcd_Cmnd(0x80);
		
		CS_0;
		RS_1;
		for (int k = 0; k < 84; k++)
		{
			ucfn_Spi_Write(0x00);		
		}
		RS_0;
		CS_1;
	}
}

void lcd_image(const unsigned char *image_data)		/* clear the Display */
{
	CS_0;
	RS_1;
	for (int k=0; k<=503; k++)
	{
		ucfn_Spi_Write(image_data[k]);
	}
	RS_0;
	CS_1;
}



void fn_init_wifi_lcd(void)
{
	fn_Reset_SRMode();
	fn_Initialize_WirelessConnection_USART0();
	fn_Initialize_WirelessConnection_USART3();
	fn_Turn_Wifi_On();
	_delay_ms(200);
	lcd_init();
	lcd_clear(10);
	uifn_Initialize_Server();
}

void print_text(unsigned char *DATA, uint8_t row, uint8_t column)
{
 
    uint8_t row_val,col_val;
	row = row -1;
	column = (column-1)*6;
	row_val= 0x40;
	col_val = 0x80;
	row_val= (row_val)|(row);
	col_val= (col_val)|(column);
    lcd_Cmnd(row_val);
    lcd_Cmnd(col_val);
	
	RS_1;								/* make DC pin to logic high for data operation */
	CS_0;									/* enable SS pin to slave selection */
	
	int lenan = strlen(DATA);						/* measure the length of data */
	for (int g=0; g<lenan; g++)
	{
		if(g < 14)
		{
			for (int index=0; index<5; index++)
			{
				ucfn_Spi_Write(ASCII[DATA[g] - 0x20][index]);	/* send the data on data register */			
			}
			ucfn_Spi_Write(0x00);
		}
	}							
	CS_1;
} 
void print_char(unsigned char DATA, uint8_t row, uint8_t column)
{
 
    uint8_t row_val,col_val;
	row = row -1;
	column = (column-1)*6;
	row_val= 0x40;
	col_val = 0x80;
	row_val= (row_val)|(row);
	col_val= (col_val)|(column);
    lcd_Cmnd(row_val);
    lcd_Cmnd(col_val);
	
	RS_1;								/* make DC pin to logic high for data operation */
	CS_0;									/* enable SS pin to slave selection */
	DATA = DATA - 0x20;
	for (int index=0; index<5; index++)
	{
		ucfn_Spi_Write(ASCII[DATA][index]);	/* send the data on data register */			
	}
		ucfn_Spi_Write(0x00);
								
	CS_1;
} 
void print_int(unsigned int ui_print_value, uint8_t row, uint8_t column)
{
 
	unsigned char DATA[6];
	itoa(ui_print_value, DATA, 10); // Conversion from int to string...

 
    uint8_t row_val,col_val;
	row = row -1;
	column = (column-1)*6;
	row_val= 0x40;
	col_val = 0x80;
	row_val= (row_val)|(row);
	col_val= (col_val)|(column);
    lcd_Cmnd(row_val);
    lcd_Cmnd(col_val);
	
	RS_1;								/* make DC pin to logic high for data operation */
	CS_0;									/* enable SS pin to slave selection */
	
	int lenan = strlen(DATA);						/* measure the length of data */
	for (int g=0; g<lenan; g++)
	{
		for (int index=0; index<5; index++)
		{
			ucfn_Spi_Write(ASCII[DATA[g] - 0x20][index]);	/* send the data on data register */			
		}
		ucfn_Spi_Write(0x00);
	}							
	CS_1;
} 

void fn_Enable_Interrupt_Sleep(void)
{
	 cli();					// Clears the global interrupt flag, prevents any form of interrupt occurring
	 sei();					// Set the interrupts
	 EICRA |= (1<<ISC31);
	 EIMSK |= (1<<INT3);
	 return;
}



ISR(INT3_vect,ISR_NOBLOCK)
{
	if(g_SleepFlag == true) // SR Brain is in Sleep Mode
	{
		 DDRG |= (1<<PG2);
		 PORTG |= (1<<PG2); 
		 g_SleepFlag = false;
		 g_FirstSleep = false;
	}

   else			// When SR Brain is not in Sleep Mode
   {
		 DDRG &=~ (1<<PG2);
		 PORTG &=~ (1<<PG2);
		 g_SleepFlag = true;
   }

}


void fn_Reset_SRMode(void)
{
	sleep_disable();  			// Clear the SE (sleep enable) bit
	_delay_ms(500);
	
	
// Resetting UART, TIMER, SPI and ADC on wake up after sleep.
	PRR0 &=~ ((1<<PRSPI)|(1<<PRUSART0)|(1<<PRADC));	
	PRR1 &=~ ((1<<PRUSART3)|(1<<PRTIM5));

	g_SleepFlag = false;
	_delay_ms(400);
}

/*void fn_Time(unsigned int counter)
{
	unsigned int val = 0,mode_var=0,new_counter,div;
	mode_var=uifn_Read_Remote(6);
	if(counter>10)
	{
	     div=counter/10;
	     new_counter=counter-div;
	}	 
	while((val < new_counter) && recvd_start == 0 && g_SleepFlag == false && mode_var>1000)
	{
		
		_delay_ms(1);
		mode_var=uifn_Read_Remote(6);
		val = val + 1;
	}	
	//return 1;
}*/


void fn_Shutdown(void)
{
	lcd_clear(11);
	POWER_GREEN_LED_OFF
	fn_Disable_ADC();
	
	g_Stop_RoboG = false;
	g_SleepFlag=true;
	//DDRB &= ~((1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3)|(1<PB6)|(1<<PB4));
	//PORTB &= ~((1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3)|(1<PB6)|(1<<PB4));
	DDRH &= ~((1<PH0)|(1<<PH2)|(1<<PH3)|(1<<PH4)|(1<<PH5));
	PORTH &= ~((1<PH0)|(1<<PH2)|(1<<PH3)|(1<<PH4)|(1<<PH5));
	DDRE &= ~(1<<PE2);
	PORTE &= ~(1<<PE2);
	DDRA &= ~((1<<PA3)|(1<<PA4)|(1<<PA5));
	PORTA &= ~((1<<PA3)|(1<<PA4)|(1<<PA5));
	_delay_ms(150);           
	DDRD &= ~((1<<PD4)|(1<<PD5)); 
	PORTD &= ~((1<<PD4)|(1<<PD5)); 
	DDRF &= ~((1<<PF2)|(1<<PF5)|(1<<PF6)|(1<<PF3)); 
	PORTG &= ~((1<<PG2)|(1<<PG0));
	DDRG &= ~((1<<PG2)|(1<<PG0));
	DDRJ &= ~(1<<PJ2)|(1<<PJ5);			// 9-5-17 Changed
	PORTJ &= ~(1<<PJ2)|(1<<PJ5);			// 9-5-17 Changed
	PORTC=0x00;
	DDRC=0x00;
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	_delay_ms(500);
	g_Recvd_Start=0;
	sleep_enable();
	fn_Enable_Interrupt_Sleep();
	sei();
	sleep_cpu();


	return;
}

