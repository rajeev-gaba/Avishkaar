#define F_CPU 16000000UL
#include<avr/io.h>
#include<util/delay.h>
#include<inttypes.h>
#include<stdio.h>
#include <avr/interrupt.h>

#define BAUD_PRESCALE 16

#define pkt g_Received_Data_IPD

// *********    Global Variables Defined ***************
extern volatile unsigned char g_Received_Data_IPD[50];
extern volatile unsigned char g_uc_Receive_Buffer[50];
extern volatile unsigned char g_Connection_String[50];
extern volatile unsigned char g_uc_Test_Char;
extern volatile unsigned char g_uc_Received_Char;
extern volatile unsigned int  g_ui_Start_Position;
extern volatile unsigned int  g_ui_Receive_Flag;
extern volatile unsigned int  g_ui_Count_Flag;
extern volatile unsigned int  g_ui_Communication_Start_Flag;
extern volatile unsigned int  g_ui_Changed;
extern volatile unsigned int  g_Recvd_Start;

extern volatile unsigned int  g_Connection_Count;

extern volatile unsigned int  g_ui_recv_length;
extern volatile unsigned char *join_network_command;
extern volatile unsigned char *command_password ;
// Defining Enums and Types for SR Brain

#define TRUE 0
#define FALSE 1

// Global Variable to Change WIFI Passowrd 1 time
extern unsigned int gi_PasswordChanged;

// State/ Mode of SR Brain
typedef enum 
{
	RemoteApp,
	Remote,
	Comm,
	None
}WIFIMode;

extern volatile WIFIMode g_WifiMode;

// ******* Method Declaration For WIFI Libray *****

void fn_Turn_Wifi_On(void);			 	 // Method to Turn on Wifi
void fn_Turn_Wifi_Off(void);			// Method to Turn off Wifi

unsigned int uifn_Initialize_Server(void);			// Configures Server on SR BRain
unsigned int uifn_Initialize_Client(void);		   // Configures Client on SR BRain

unsigned int uifn_Status(void);					 // Method returns the Status
void fn_Clear_Buffer(void);						// Clears Received Buffer
void fn_Clear_ReceivedDataBuffer(void);

void fn_Initialize_WirelessConnection_USART0(void);		 // Initialize wireless connection for USART 0
void fn_Initialize_WirelessConnection_USART3(void);		//  Initialize wireless connection for USART 3

void fn_SendCharacter_USART0(unsigned char c_SendByte);		 // Initialize wireless connection for USART 0
void fn_SendCharacter_USART3(unsigned char c_SendByte);		//  Initialize wireless connection for USART 3

unsigned int uifn_Execute_RobotronicsRemoteApp(void);		// Method used to operate from Wifi Remote App

unsigned int uifn_Check_Reply(void);					// Method to Check Reply 
void fn_Send_Command(unsigned char comm[]);				// Sends the Command

unsigned int uifn_Start_Wifi_Connection();// To Start Wifi Connection on SR BRain with provided server port
unsigned int uifn_Join_Network();				// Join a network by provind its SSID and Password
unsigned int uifn_Send_Wifi();		// Send data from Brain with the help of wifi

void fn_Change_Wifi_Password(); // To Change Password and ESP (Wifi) Name

int fn_Get_Received_Data(void);

void fn_Close_Wifi(void);
void fn_Open_Wifi(void);
//END/