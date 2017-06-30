#define F_CPU 16000000
#include<avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>


/********Method Declaration Serial Interface ******/

// Initialization methods for SPI

void fn_Initialize_Spi_Tft(void); 			// Initialize SPI for TFT
void fn_Initialize_Spi_Touch(void);			// Initialize SPI to enable Touch
void fn_Initialize_Spi_Fast_SD(void);		// Initialize SPI fast for SD card 
void fn_Initialize_Spi_Slow_SD(void);		// Initialize SPI slow for SD card
void fn_Initialize_Spi_nokia_lcd(void);

// Reading and Writing through SPI

unsigned char ucfn_Spi_Read_SD(void);					// Read through SPI for SD card
unsigned char ucfn_Spi_Write(unsigned char c_Data);		// Method to Write thorugh SPI 
unsigned char ucfn_Spi_Read_Touch(void);				// Read through SPI for TOuch Screen