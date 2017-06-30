#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include<inttypes.h>
#include<stdio.h>

#define S_1 Sensor_1
#define S_2 Sensor_2
#define S_3 Sensor_3
#define S_4 Sensor_4
#define S_5 Sensor_5

#define CS_3 ColorSensor_3
#define CS_4 ColorSensor_4
#define CS_5 ColorSensor_5

#define US_3 UltraSensor_3
#define US_4 UltraSensor_4
#define US_5 UltraSensor_5

/* color Sensor Related Macros  */

#define COLORPORT3_PIN_S3  PE7         // S3 PIN OF COLOR SENSOR AT PORT3  timer 0 pd7  INT7
#define COLORPORT4_PIN_S3  PE4         // S3 PIN OF COLOR SENSOR AT PORT4  timer 5 pl2  INT4
#define COLORPORT5_PIN_S3  PE5         // S3 PIN OF COLOR SENSOR AT PORT5  timer 3 pe6  INT5 

#define RED_COLOR 'r'	
#define BLUE_COLOR 'b'
#define GREEN_COLOR 'g'
#define WHITE_COLOR 'w'
#define BLACK_COLOR 'x'
#define YELLOW_COLOR 'y'

/* Water Level Sensor Related Macros  */
#define W_EMPTY 1
#define W_FULL 2  

/* IR Sensor Related Macros Also  S2 PIN COLOR SENSOR AND TRIGGER FOR ULTRASONIC	*/

#define SENSOR_PORT_1 4        //ADC4 PF4     
#define SENSOR_PORT_2 5        //ADC5 PF5    
#define SENSOR_PORT_3 2        //ADC2 PF2    
#define SENSOR_PORT_4 0        //ADC4 PF0    
#define SENSOR_PORT_5 3        //ADC3 PF3       

#define Humidity 1
#define Temperature 0

/* Global Variable Declaration for Sensors */

extern unsigned int Sensor_1, Sensor_2, Sensor_3, Sensor_4, Sensor_5;	// IR/ Touch Sensor Values
extern unsigned char ColorSensor_3, ColorSensor_4, ColorSensor_5;				// For Color Sensor Values at Port 3, 4, 5
extern unsigned int Temp_1, Temp_2, Temp_3, Temp_4, Temp_5;			// Temperature Sensor Values
extern unsigned int Humid_1, Humid_2, Humid_3, Humid_4, Humid_5;	// Humidity Sensor Values

extern volatile unsigned int DHT11_PIN;

extern volatile unsigned int UltraSensor_3, g_working_3, g_timer_3, g_rise_3;		  // For Ultrasonic Sensor at Port 3
extern volatile unsigned int UltraSensor_4, g_working_4, g_timer_4, g_rise_4;		 // For Ultrasonic Sensor at Port 4
extern volatile unsigned int UltraSensor_5, g_working_5, g_timer_5, g_rise_5;		// For Ultrasonic Sensor at Port 5

/* Method Declaration for Sensors */

void fn_Initialize_SensorPort(void);	 // Method to Initialize Sensor Ports
void fn_Enable_Interrupt_Sensor(void);	// Method to Interrupt Enable 

void fn_Enable_ADC(void);						// To Enable ADC
void fn_Disable_ADC(void);						// To Disable ADC
void fn_Initialize_TimerRegister(void);  		// Initialization Timer Register


void fn_Read_Ultrasonic_Sensor(uint8_t ui_PortNum);			// Function to get Ultrasonic Sensor range
unsigned int uifn_Read_Analog_sensor(uint8_t ui_PortNum);	// Function to Read IR / Touch Sensor Value
unsigned char cfn_Read_Color_Sensor(uint8_t ui_PortNum);	// Function to Read Color at Color Sensor 

unsigned int uifn_Read_Remote(uint8_t ui_PortNum);			// Method to get values from Robotronics Remote 

// Temperature and Humidity Sensor Code
int fn_Read_Humidity_Temperature(uint8_t DHT_PIN, uint8_t option);

uint8_t fn_Get_data(uint8_t DHT11_PIN, uint8_t select);			/* receive data */

#define DHT11_ERROR 255

/* --------Function FOr Water Level Indicator-------- */
unsigned int uifn_Read_waterlevel_sensor(uint8_t PortNum);
unsigned int freq_calc(unsigned int sensor_no);

/*------End of Code--------*/