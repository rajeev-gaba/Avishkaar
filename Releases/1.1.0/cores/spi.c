
#include"spi.h"

/*   Initialize SPI for TFT  */
void fn_Initialize_Spi_Tft()
{
	DDRB |= (1<<PB1)|(1<<PB2)|(1<<PB0);
	SPCR=0x00;
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<CPHA)|(1<<CPOL);		// SPI is running at 8 MHz
	SPSR = 1;
}

/*   Initialize SPI to use Touch on Screen  */
void fn_Initialize_Spi_Touch()
{
	DDRB |= (1<<PB2)|(1<<PB1)|(1<<PB0);		
	SPCR=0x00;
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<CPOL)|(1<<SPR1)|(1<<SPR0)|(1<<CPHA);		// SPI is running at 125 MHz	
}

/*   Initialize SPI fast for SD Card    */
void fn_Initialize_Spi_Slow_SD()
{
	DDRB |= (1<<PB2)|(1<<PB1)|(1<<PB0);
	SPCR=0x00;
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<CPOL)|(1<<SPR1)|(1<<SPR0)|(1<<CPHA);		// Initialization for SD Card
	SPSR = 1;
}

/*   Initialize SPI slow for SD Card    */
void fn_Initialize_Spi_Fast_SD()
{
	SPCR=0x00;
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<CPOL)|(1<<CPHA)|(1<<SPR0);			// SPI is running at 8 MHz
	SPSR = 1;
}

/*  Read SPI for SD card  */
unsigned char ucfn_Spi_Read_SD()
{
	SPDR = 0xFF;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

/*	Read SPI for Touch  */
unsigned char ucfn_Spi_Read_Touch()
{
	SPDR = 0x00;
	while(!(SPSR & (1<<SPIF)));

	return SPDR;
}

/*	Write through SPI */
unsigned char ucfn_Spi_Write(unsigned char c_Data)
{
	unsigned char received_Char;   
	SPDR = c_Data;

	while(!(SPSR & (1<<SPIF)));

	received_Char = SPDR;
	return (received_Char);
}

void fn_Initialize_Spi_nokia_lcd()
{
	DDRB |= (1<<PB1)|(1<<PB2)|(1<<PB0);
	SPCR=0x00;
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<CPHA)|(1<<CPOL);		// SPI is running at 4 MHz
}
