#include"wifi.h"
#include <string.h>
#include "motor.h"


int change = FALSE;
unsigned int gi_PasswordChanged = FALSE;
unsigned int server = 0;

volatile WIFIMode g_WifiMode = Comm;

//unsigned char *Command = "";
//unsigned char *AT_SendToserver = "AT+CIPSEND=";
//unsigned char *AT_SendToClient = "AT+CIPSEND=0,";

volatile unsigned char *join_network_command = "thisisteststring";
volatile unsigned char *command_password = "AB";

volatile unsigned char g_Received_Data_IPD[50];
volatile unsigned char g_Connection_String[50];
volatile unsigned char g_uc_Receive_Buffer[50];
volatile unsigned char g_uc_Test_Char = 'x';
volatile unsigned int  g_ui_Count_Flag = 0;
volatile unsigned int  g_ui_Start_Position = 0;
volatile unsigned int  g_ui_Receive_Flag = 0;
volatile unsigned int  g_ui_Communication_Start_Flag = 0;
volatile unsigned int  g_ui_Changed = 0;
volatile unsigned char g_uc_Received_Char = 'x';
volatile unsigned int  g_Recvd_Start = 0;
volatile unsigned int  g_ui_recv_length = 0;

volatile char first_Packet = 'x';

volatile unsigned int  g_Connection_Count = 0;

// This Method defines to Turn On Wifi
void fn_Turn_Wifi_On(void)
{
	DDRA|=(1<<PA5);
	PORTA|=(1<<PA5);
	
	fn_Clear_ReceivedDataBuffer();
}

// This Method defines to Turn OFF Wifi
void fn_Turn_Wifi_Off(void)
{
	DDRA &= ~(1<<PA5);
	PORTA &= ~(1<<PA5);
}	

// This Method configures Server on SR BRain

unsigned int uifn_Initialize_Server(void)
{
	unsigned int reply;
	cli();

	fn_Send_Command("AT+RST");
	_delay_ms(500);

fn_Clear_ConnectionString();

	sei();
	g_ui_Communication_Start_Flag = 1;
	fn_Clear_Buffer();
	fn_Send_Command("ATE0");
	_delay_ms(100);

	reply = uifn_Check_Reply();
	
	if(reply == 1)
	{
	 fn_SendCharacter_USART0('0'); 
	 g_ui_Count_Flag = 0;
	 fn_Clear_Buffer();
	}

	fn_Send_Command("AT+CWMODE=2");
	_delay_ms(50);
	reply = uifn_Check_Reply();
	
	if(reply == 1)
	{
		fn_SendCharacter_USART0('1'); 
		g_ui_Count_Flag = 0;
		fn_Clear_Buffer();
	}
	

	fn_Send_Command("AT+CIPMUX=1");
	_delay_ms(10);
	reply = uifn_Check_Reply();
	
	if(reply == 1)
	{
		fn_SendCharacter_USART0('2'); 
		g_ui_Count_Flag = 0;
		fn_Clear_Buffer();
	}	

	fn_Send_Command("AT+CIPSERVER=1,900");
	_delay_ms(10);
	
	reply = uifn_Check_Reply();
	
	if(reply == 1)
	{
		fn_SendCharacter_USART0('3'); 
		g_ui_Communication_Start_Flag = 2;
		g_ui_Count_Flag = 0;
		fn_Clear_Buffer();		
		_delay_ms(10);
		print_text(" Init Server  ",1, 1);
		server = 1;
		return(1);
	}
	return -1;
}

//
void fn_Change_Wifi_Password()
{
	if(gi_PasswordChanged == FALSE)
	{
		fn_Initialize_WirelessConnection_USART3();	 
		
		fn_Turn_Wifi_On();
		sei();

		change = TRUE;
		int reply = uifn_Initialize_Server();
		
		int counter =0;
		fn_Send_Command(command_password);
		_delay_ms(1000);
		
		int rply = uifn_Check_Reply();
		
		if(rply == 1)
		{
			fn_SendCharacter_USART0('5');
			g_ui_Count_Flag = 0;
			fn_Clear_Buffer();
		}
		else
		{
			fn_SendCharacter_USART0('6');
		}
		
	}
	
}



// This Method configures Client on SR Brain 

unsigned int uifn_Initialize_Client(void)
{
	unsigned int reply;
	cli();
	fn_Send_Command("AT+RST");
	_delay_ms(500);

	fn_Clear_Buffer();
	sei();
	fn_Send_Command("ATE0");
	g_ui_Communication_Start_Flag = 1;
	_delay_ms(100);
	reply = uifn_Check_Reply();
	
	if(reply == 1)
	{
		fn_SendCharacter_USART0('0'); 
		g_ui_Count_Flag = 0;
		fn_Clear_Buffer();
	}

	fn_Send_Command("AT+CWMODE=1");
	_delay_ms(100);
	reply = uifn_Check_Reply();
	
	if(reply == 1)
	{
		fn_SendCharacter_USART0('1'); 
		g_ui_Count_Flag = 0;
		fn_Clear_Buffer();
	}
	
	fn_Send_Command("AT+CIPMUX=0");
	_delay_ms(100);
	reply = uifn_Check_Reply();

	if(reply == 1)
	{
		fn_SendCharacter_USART0('2'); 
		g_ui_Count_Flag = 0;
		fn_Clear_Buffer();
	}

	print_text(" Init Client  ",1, 1);
	server = 2;
 
 return(1);	

}



// This Method Starts Wifi Connection on SR BRain

unsigned int uifn_Start_Wifi_Connection()
{
	int reply = 0;
	fn_Clear_Buffer();

	fn_Send_Command("AT+CIPSTART=\"TCP\",\"192.168.4.1\",900");

	_delay_ms(5000);

	reply = uifn_Check_Reply();
	
	if(reply == 1)
	{
		fn_SendCharacter_USART0('%'); 
		print_text(" Wifi Started ",1, 1);
		return 1;
	}
	
	else
	{
		print_text("Not Connected ",1, 1);
		for(int t=0;t<50;t++)
		{
			fn_SendCharacter_USART0(g_uc_Receive_Buffer[t]);
			_delay_ms(50);
		}
		return 0;
	}

}

// This Method ask to join network before sending some data packet on SR BRain

unsigned int uifn_Join_Network()
{
	int reply=0;
	
	
	fn_Send_Command(join_network_command);
	
	//fn_Send_Command("AT+CWJAP=\"UK11ESP\",\"gotohell\"");		
		
	_delay_ms(5000);
	_delay_ms(5000);
	_delay_ms(2000);
	_delay_ms(5000);
	reply = uifn_Check_Reply();

	if(reply==1)
	{
		 fn_SendCharacter_USART0('^');
		 print_text("Network Joined",1, 1);
		 for(int q=0;q<50;q++)
			{
				_delay_ms(100);
				fn_SendCharacter_USART0(g_uc_Receive_Buffer[q]); 
			}
		 return(1);
	}
	else
	{
		fn_SendCharacter_USART0('b');
		print_text("Not Connected ",1, 1);
		for(int q=0;q<50;q++)
			fn_SendCharacter_USART0(g_uc_Receive_Buffer[q]);
		 
		return(0);
	}
}


// This Method Clears the Received Buffer

void fn_Clear_Buffer(void)
{
	unsigned int num;

	for(num = 0 ; num < 50 ; num++)
	{
		 g_uc_Receive_Buffer[num] = 'x';
	}
	g_ui_Count_Flag = 0;
}

void fn_Clear_ConnectionString(void)
{
	unsigned int num;

	for(num = 0 ; num < 50 ; num++)
	{
		g_Connection_String[num] = 'x';
	}
}

void fn_Clear_ReceivedDataBuffer(void)
{
	unsigned int count;

	for(count = 0 ; count < 50 ; count++)
	{
		 g_Received_Data_IPD[count] = ' ';
	}
	g_ui_Count_Flag = 0;
}

// This Method initialize wireless connection for USART 0
void fn_Initialize_WirelessConnection_USART0(void)
{
	 UCSR0A |=(1<<U2X0);
	 UCSR0B |= (1 << RXEN0) | (1<<TXEN0)|(1<<RXCIE0);   // Turn on the transmission and reception circuitry
	 UCSR0C |= (1<<USBS0) | (1 << UCSZ00) | (1 << UCSZ01); // Use 8-bit character sizes
	 UBRR0H = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value..
	 //UBRR0H=0x00;
	 UBRR0L = BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR register
	 return;								
}

// This Method initialize wireless connection for USART 3

void fn_Initialize_WirelessConnection_USART3(void) 
{
	 UCSR3A |=(1<<U2X3);
	 UCSR3B |= (1 << RXEN3) | (1<<TXEN3)|(1<<RXCIE3);   // Turn on the transmission and reception circuitry
	 UCSR3C |= (1<<USBS3) | (1 << UCSZ30) | (1 << UCSZ31); // Use 8-bit character sizes
	 UBRR3H = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value..
	 //UBRR0H=0x00;
	 UBRR3L =BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR register
	 return;								
} /* end of initWirelessConnection function */ 

   
   
   
// This Method sends character on wireless connection 0

void fn_SendCharacter_USART0(unsigned char c_SendByte) 
{
	while ( !( UCSR0A & (1<<UDRE0))) ;
	UDR0 = c_SendByte;
} // end of sendCharacter(unsigned char sendByte)


   
   
// This Method sends character on wireless connection 3

void fn_SendCharacter_USART3(unsigned char c_SendByte) 
{
	while ( !( UCSR3A & (1<<UDRE3))) ;
	UDR3 = c_SendByte;
} // end of sendCharacter(unsigned char sendByte)

// This Method sends command to serial

void fn_Send_Command(unsigned char comm[])
{
	 int i;
	 for(i = 0 ; comm[i] != ' ' ; i++)
	{
		 fn_SendCharacter_USART3(comm[i]);
		 _delay_ms(4);
	}
	 fn_SendCharacter_USART3('\r');
	 fn_SendCharacter_USART3('\n');
}




// This Method is used to check received data through wifi

unsigned int uifn_Check_Reply(void)
{
	unsigned int done=0,num=0;
	for(num = 0 ; g_uc_Receive_Buffer[num] != 'x' ; num++)
    {
		   if((g_uc_Receive_Buffer[num] == 'O') && (g_uc_Receive_Buffer[num+1] == 'K'))
		   {
				 done = 1;    
		   } 
	} 

	 if(done == 1)
		return(1);
	 else
		return(0);
}







// This Method Check the Status

unsigned int uifn_Status(void)
{
	 unsigned int num=0, pos=0;
	 fn_Clear_Buffer();
	 fn_Send_Command("AT+CIPSTATUS");
	 _delay_ms(500);
	 
	for(num = 0 ; g_uc_Receive_Buffer[num] != 'x'; num++)
	{
		if((g_uc_Receive_Buffer[num]=='U')&&(g_uc_Receive_Buffer[num+1]=='S'))
			pos=num;
	}

	if(g_uc_Receive_Buffer[pos+3]=='2')
		 return(1);  // connection can be started

	else
	{
		 fn_SendCharacter_USART0(g_uc_Receive_Buffer[pos+3]);
		 return(0);  // join AP firtst
	}
}



// This Method 

unsigned int uifn_Send_Wifi(unsigned int ui_size,unsigned char *c_arr_txdata)
{
	unsigned int num_ch=0, reply=0, temp, check=0, pos=0, counter = 0;
	fn_Clear_Buffer();
	unsigned char Command1[] = "AT+CIPSEND=" ;
	unsigned char Command2[] = "AT+CIPSEND=0," ;
		
	unsigned char arr_size[4];
	
	itoa((ui_size+1), arr_size, 10);
	
	if(server == 1)
	{
		for(counter=0;Command2[counter]!='\0';counter++)
		{
			 fn_SendCharacter_USART3(Command2[counter]);
			 _delay_ms(4);
		}
		for(counter=0;arr_size[counter]!='\0';counter++)
		{
			 fn_SendCharacter_USART3(arr_size[counter]);
			 _delay_ms(4);
		}
		fn_SendCharacter_USART3('\r');
		fn_SendCharacter_USART3('\n');
		_delay_ms(200);
	}
	else if(server == 2)
	{
		for(counter=0;Command1[counter]!='\0';counter++)
		{
			 fn_SendCharacter_USART3(Command1[counter]);
			 _delay_ms(4);
		}
		for(counter=0;arr_size[counter]!='\0';counter++)
		{
			 fn_SendCharacter_USART3(arr_size[counter]);
			 _delay_ms(4);
		}
		fn_SendCharacter_USART3('\r');
		fn_SendCharacter_USART3('\n');
		_delay_ms(200);
	}
	
	reply = uifn_Check_Reply();
	
	
	if(reply==1)
	{
		char dst[ui_size] ;
		strcpy(dst, c_arr_txdata);
		
		g_ui_Count_Flag = 0;
		
		fn_SendCharacter_USART0('~');
		_delay_ms(40);
		
		print_text(" Packet Sent  ",1, 1);
		for(int count = 0; count < ui_size; count++)
		{
			fn_SendCharacter_USART0(dst[count]);
			_delay_ms(10);
		}
		fn_SendCharacter_USART3('^');
		_delay_ms(10);
		for(int count = 0; count < ui_size; count++)
		{
			fn_SendCharacter_USART3(dst[count]);
			//fn_SendCharacter_USART3('A');
			_delay_ms(10);
		}
		fn_SendCharacter_USART3('\r');
		fn_SendCharacter_USART3('\n');
	}
	
	else
	{
		print_text("Packt Not Sent",1, 1);
		fn_SendCharacter_USART0('P');
		 _delay_ms(40);
		 
		for(int y=0;y<50;y++){
			fn_SendCharacter_USART0(g_uc_Receive_Buffer[y]);_delay_ms(100);
		}
		return(2);// error either connection not started so check cipstatus
	}
	 _delay_ms(40);


		for(int y=0;y<50;y++){
			fn_SendCharacter_USART0(g_uc_Receive_Buffer[y]);_delay_ms(50);
		}
		
	for(temp=0;g_uc_Receive_Buffer[temp]!='x';temp++)
	{
		if((g_uc_Receive_Buffer[temp]=='S')&&(g_uc_Receive_Buffer[temp+1]=='E'))
		{
			 check=1;
			 pos=temp;
		}
	}

	if(check==1)
	{
		if(g_uc_Receive_Buffer[pos+5]=='O')
		{
			//print_text("  Data Sent   ",1, 1);
			return(1); // sending ok
		}
		else
		return(0); // sending failed
	} 

	 return(0); // sending failed
}


int fn_Get_Received_Data(void)
{
	if(g_Recvd_Start == 1)
	{
		fn_Clear_ReceivedDataBuffer();
		
		int size = 0;
		int pos = 0;
		for(int t = 0 ; t < 12 ; t++)
		{
			if(g_uc_Receive_Buffer[t] == ':')
				pos = t;
		}
		
		char num1[2];     num1[0] = g_uc_Receive_Buffer[pos-1];
		num1[1] = '\0';
		char num2[2];     num2[0] = g_uc_Receive_Buffer[pos-2];
		num2[1] = '\0';
		char num3[1];     num3[0] = g_uc_Receive_Buffer[pos-3];
		
		if(num2[0] == ',')
		{
			size = atoi(num1);			
		}
		else if(num3[0] == ',')
		{
			char num[3];
			num[0] =  num1[0];
			num[1] =  num2[0];
			num[2] = '\0';
			size = atoi(num);
		}
		//fn_Print_Char_On_Screen(5,3, num1[0], 0x0000, 0xFFFF);
		//fn_Print_Int_On_Screen(11,11, size, 0x0000, 0xFFFF);
		
		g_ui_recv_length = size;
		//char *data = malloc(size * sizeof(char));
		fn_SendCharacter_USART0('r');_delay_ms(50);
		int count = 0;
		
		
		first_Packet = g_uc_Receive_Buffer[pos+1];
		fn_SendCharacter_USART0('a');_delay_ms(50);
		if(first_Packet == '^')
		{
			g_WifiMode = Comm;
			fn_SendCharacter_USART0('i');_delay_ms(50);
		}
		else
		{
			g_WifiMode = RemoteApp;
			fn_SendCharacter_USART0('o');_delay_ms(50);
			return 0;
		}
		
		for(int t = pos+2 ; t < pos+size+1 ; t++)
		{
			g_Received_Data_IPD[count] = g_uc_Receive_Buffer[t];
			//fn_SendCharacter_USART0(g_uc_Receive_Buffer[t]);
			count = count + 1;
			_delay_ms(50);
		}
		g_Received_Data_IPD[count] = '\0';
		fn_SendCharacter_USART0('e');_delay_ms(50);fn_SendCharacter_USART0('\n');_delay_ms(50);
		g_Recvd_Start = 0;
		g_ui_Count_Flag = 0;
		fn_Clear_Buffer();
		
		/*for(int x = 0; x < g_ui_recv_length ; x++)
		{
			fn_Print_Char_On_Screen(11,x+3, data[x], 0x0000, 0xFFFF);
		}*/
		
		_delay_ms(100);
		// return data;
		return 1;
	}
	_delay_ms(100);
	return -1;
}


void fn_Open_Wifi(void)
{
	fn_Initialize_WirelessConnection_USART0();
	fn_Initialize_WirelessConnection_USART3();	 
	fn_Turn_Wifi_On();
	sei();
	_delay_ms(20);	
}

void fn_Close_Wifi(void)
{
	fn_Clear_Buffer();
	fn_Turn_Wifi_Off();
	_delay_ms(20);
	
}//-------------------------Execute WIfi Remote app ------------------------

unsigned int uifn_Execute_RobotronicsRemoteApp(void)
{
	PORTJ &=~ (1<<PJ5);
	PORTG |= (1<<PG2);
	fn_Initialize_PWMForMotors();
	fn_Initialize_DCMotors();
	
	//fn_SendCharacter_USART0('R');fn_SendCharacter_USART0('E');fn_SendCharacter_USART0('P');fn_SendCharacter_USART0('\n');
	
	
switch(g_uc_Receive_Buffer[9])
{
	case('A'):
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT1,CW);				
				break;	//M1-CW
	
	case('B'):
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT1,ACW);				
				break;	//M1-CCW
	
	case('C'):
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT2,CW);				
				break;
	
	case('D'):
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT2,ACW);				
				break;
	case('E'):
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT1,CW);
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT2,CW);
				break;
	case('F'):
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT1,CW);
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT2,ACW);
				break;
	case('G'):
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT1,ACW);
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT2,CW);
				break;
	case('H'):
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT1,ACW);
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT2,ACW);
				break;

	case('X'):
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT1,STOP);
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT2,STOP);
				break;

	case('M'):
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT3,CW);
				break;
	
	case('N'):
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT3,ACW);
				break;
	case('O'):
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT4,CW);
				break;
	case('P'):
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT4,ACW);
				break;

	case('Q'):
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT3,CW);
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT4,CW);
				break;
	case('R'):
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT3,CW);
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT4,ACW);
				break;
	case('S'):
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT3,ACW);
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT4,CW);
				break;
	case('T'):
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT3,ACW);
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT4,ACW);
				break;

	case('Y'):
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT3,STOP);
				fn_MoveDCMotors_ConstantSpeed(MOTORPORT4,STOP);
				break;
				
	case('Z'):
				g_Recvd_Start=0;
				break;			

	default :
		return 0;
	}
}

// Interrupt for Wireleass connection 3 or USART 3

ISR(USART3_RX_vect)
{

	g_uc_Test_Char = UDR3;
 
	if(g_ui_Communication_Start_Flag == 1)
	{
		if((g_uc_Test_Char == 0x0A)||(g_uc_Test_Char == 0x0D))
		{
			// To be Defined...
		}
		else
		{
			g_uc_Receive_Buffer[g_ui_Count_Flag]=g_uc_Test_Char;
			g_ui_Count_Flag = g_ui_Count_Flag + 1;
		}
	}

	else if(g_ui_Communication_Start_Flag == 2)
   {
		
		
		if((g_uc_Test_Char == 0x0A)||(g_uc_Test_Char == 0x0D))
		{
	   
		}
		else if(g_uc_Test_Char == '+')
		{
			 g_ui_Start_Position = 0;
			 g_ui_Count_Flag = 1;
			 g_Recvd_Start = 1;
		}	
		else
		{
	   
			 g_uc_Receive_Buffer[g_ui_Count_Flag]  =  g_uc_Test_Char;
			 g_ui_Count_Flag = g_ui_Count_Flag + 1;
		}
		
		if(g_WifiMode == RemoteApp)
		{
			//fn_SendCharacter_USART0(g_uc_Test_Char);
			if(g_uc_Test_Char == ',' && g_Connection_Count == 0)
			{
				g_Connection_String[g_Connection_Count]  =  g_uc_Test_Char;
				g_Connection_Count = g_Connection_Count + 1;
			}
			if(g_Connection_Count > 0 )
			{
				if(g_Connection_Count > 1)
				{
					g_Connection_String[g_Connection_Count]  =  g_uc_Test_Char;
					g_Connection_Count = g_Connection_Count + 1;
					fn_SendCharacter_USART0('C');
					//fn_SendCharacter_USART0(g_uc_Test_Char);
				}
				if(g_uc_Test_Char == 'C' || g_uc_Test_Char == 'c')
				{
					g_Connection_String[g_Connection_Count]  =  g_uc_Test_Char;
					g_Connection_Count = g_Connection_Count + 1;
					fn_SendCharacter_USART0('A');
				}
				if(g_Connection_Count > 2)
				{
					if(g_Connection_String[2] == 'L' || g_Connection_String[2] == 'l')
					{
					}
					else
					{
						g_Connection_Count = 1;
						fn_SendCharacter_USART0('B');
					}
				}
			}
			if(g_Connection_Count >= 6)
			{
				if((g_Connection_String[1] == 'C' || g_Connection_String[1] == 'c') 
				&& (g_Connection_String[2] == 'L' || g_Connection_String[2] == 'l') 
				&& (g_Connection_String[3] == 'O' || g_Connection_String[3] == 'o') 
				&& (g_Connection_String[4] == 'S' || g_Connection_String[4] == 's')
				&& (g_Connection_String[5] == 'E' || g_Connection_String[5] == 'e')
				&& (g_Connection_String[6] == 'D' || g_Connection_String[6] == 'd'))
				{
					g_Recvd_Start = 0;
					g_Connection_Count = 0;
				}
			}		
		}		
	}
}


