/*
* Sensor.c
*
*  */ 

#include"sensor.h"
#include"robog.h"

// Defining global sensor variables

unsigned int Sensor_1 = 0, Sensor_2 = 0, Sensor_3 = 0, Sensor_4 = 0, Sensor_5 = 0;	// IR/ Touch Sensor Values
unsigned char ColorSensor_3 = 0, ColorSensor_4 = 0, ColorSensor_5 = 0;				// For Color Sensor Values at Port 3, 4, 5
unsigned int Temp_1 = 0, Temp_2 = 0, Temp_3 = 0, Temp_4 = 0, Temp_5 = 0;			// Temperature Sensor Values
unsigned int Humid_1 = 0, Humid_2 = 0, Humid_3 = 0, Humid_4 = 0, Humid_5 = 0;	// Humidity Sensor Values

volatile unsigned int UltraSensor_3 = 0, g_working_3 = 0, g_timer_3 = 0, g_rise_3 = 0; // variables for ulrasonic sensor at port 3
volatile unsigned int UltraSensor_4 = 0, g_working_4 = 0, g_timer_4 = 0, g_rise_4 = 0; // variables for ulrasonic sensor at port 4
volatile unsigned int UltraSensor_5 = 0, g_working_5 = 0, g_timer_5 = 0, g_rise_5 = 0; // variables for ulrasonic sensor at port 5

//volatile unsigned int DHT11_PIN = 4;

//This function initializes the ADC specific control registers for Sensor usage
void fn_Enable_ADC()
{
	ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // ADC sampling frequency is 125 Khz ( 16 MhZ/128) 
	ADMUX |= (1<<REFS0);                                   // Reference voltage for ADC is AVcc( 5v)
	return;
}

//This function Disables the ADC specific control registers
void fn_Disable_ADC()
{
	ADCSRA &=~ (1<<ADEN);		// Turning off ADC
}

// This method initializes Sensor Port i.e., PORTA
void fn_Initialize_SensorPort()
{
	DDRA |= (1<<PA3)|(1<<PA4); 		// control pin for power in sensor attachment
	PORTA |= ((1<<PA3)|(1<<PA4));
}

// This method initializes Timer Register
void fn_Initialize_TimerRegister()
{
	TCCR0A = 0; 
	TCCR0B |= (1<<CS01);   // Timer0 enabled with (Fcpu/8) clock
	//TCCR5A |= (1<<WGM50);  // setting Timer 5 for 8 bit(256 counts)
	TCCR5A = 0;  // setting Timer 5 for 16 bit(65536 counts)
	TCCR5B |= (1<<CS51);   // Timer5 enabled with (Fcpu/8) clock
	TCCR3A |= (1<<WGM30);  // setting Timer 3 for 8 bit(256 counts)
	TCCR3B |= (1<<CS31);  // Timer1 enabled with (Fcpu/8) clock
	return;
}

// This Method Enables Interrupts for timers and ultrasonic sensors
void fn_Enable_Interrupt_Sensor()
{
	cli();
	DDRE &=~ ((1<<PE5)|(1<<PE4)|(1<<PE7));
	EICRB |= (1<<ISC50)|(1<<ISC40)|(1<<ISC70);    // Setting interrupts for logical level change
	EIMSK |= (1<<INT4)|(1<<INT5)|(1<<INT7);       // Enabling interrupts INT4,INT5,INT7
	sei();	
	return;
}	

// This Method Read Sensor Values for IR Sensor and Touch Sensor
unsigned int uifn_Read_Analog_sensor(uint8_t PortNum)
{
	fn_Enable_ADC();
	if(PortNum == SENSOR_PORT_1 || PortNum == SENSOR_PORT_2)
	{
		DDRF &=~ (1<<PortNum);	
		PortNum = (PortNum)&(0x0F);  

		ADMUX |= PortNum;        // Selecting the ADC channel
		ADCSRA |= (1<<ADSC)|(1<<ADEN);  // Start ADC conversion and enabling ADC
		while(!(ADCSRA&(1<<ADIF)));   // Waiting for ADC conversion to complete
		ADCSRA |= (1<<ADIF);   // Clearing ADIF after conversion  is complete
		ADMUX = (1<<REFS0);    // Clearing ADMUX ADC channel
		return(ADCW);
	}
	
	else if(PortNum == SENSOR_PORT_3)
	{
		EIMSK &=~ (1<<INT7);		 // Disabling INT7
		DDRF &=~ (1<<PortNum);
		TCCR0B &=~ ((1<<CS02)|(1<<CS01)|(1<<CS00));	// Disabling Timer0
		PortNum = (PortNum)&(0x0F);
		ADMUX |= PortNum;            // Selecting the ADC channel
		ADCSRA |= (1<<ADSC)|(1<<ADEN); // Start ADC conversion and enabling ADC
		while(!(ADCSRA&(1<<ADIF)));  // Waiting for ADC conversion to complete
		ADCSRA |=(1<<ADIF);    // Clearing ADIF after conversion  is complete
		ADMUX = (1<<REFS0);      // Clearing ADMUX ADC channel
		return(ADCW);
	}

	else if(PortNum == SENSOR_PORT_4)
	{ 
		EIMSK &=~ (1<<INT4);		 // Disabling INT4
		DDRF &=~ (1<<PortNum);	
		TCCR5B &=~ ((1<<CS52)|(1<<CS51)|(1<<CS50));   //Disabling Timer5
		PortNum = (PortNum)&(0x0F); 
		ADMUX |= PortNum;             // Selecting the ADC channel
		ADCSRA |= (1<<ADSC)|(1<<ADEN); // Start ADC conversion and enabling ADC
		while(!(ADCSRA&(1<<ADIF)));  // Waiting for ADC conversion to complete
		ADCSRA |=(1<<ADIF);    // Clearing ADIF after conversion  is complete
		ADMUX = (1<<REFS0);    // Clearing ADMUX ADC channel
		return(ADCW);
	}

	else if(PortNum == SENSOR_PORT_5)
	{
		EIMSK &=~ (1<<INT5);		 // Disabling INT5
		DDRF &=~ (1<<PortNum);
		TCCR3B &=~ ((1<<CS32)|(1<<CS31)|(1<<CS30)); //Disabling Timer3	
		PortNum = (PortNum)&(0x0F);
		ADMUX |= PortNum;         // Selecting the ADC channel
		ADCSRA|=(1<<ADSC)|(1<<ADEN);  // Start ADC conversion and enabling ADC
		while(!(ADCSRA&(1<<ADIF)));  // Waiting for ADC conversion to complete
		ADCSRA |=(1<<ADIF);  // Clearing ADIF after conversion  is complete
		ADMUX = (1<<REFS0);    // Clearing ADMUX ADC channel
		return(ADCW);
	}
	return -1;
}

// This Method Read Sensor Values for Ultrasonic Sensor
void fn_Read_Ultrasonic_Sensor(uint8_t PortNum )
{
     fn_Disable_ADC();    // Disabling ADC
 
 
	if(PortNum == SENSOR_PORT_5)
	{
		if(g_working_5 == 0)
		{
			_delay_ms(50);
			TCCR3A = 0;
			TCCR3B = 0;
			TCCR3A |= (1<<WGM30);    // setting Timer 3 for 8 bit(256 counts) 
			TCCR3B |= (1<<CS31);      //  timer running at 2 Mhz(Fcpu/8) clock
			EIMSK |= (1<<INT5);      // Enabling interrupt for echo 
			DDRF |= (1<<PortNum);   // TRIGGER PIN
			PORTF &=~ (1 << PortNum);
			_delay_us(1);
			PORTF |= (1 << PortNum); //Send 10us second pulse // enable trigger
			_delay_us(10);
			PORTF &=~ (1 << PortNum);
			g_working_5 = 1;
			TCNT3=0;            // Setting Timer 3 to 0
			TIMSK3 |= (1<<TOIE3);  // Enabling Timer 3 overflow interrupt
		}    
	}	

	else if(PortNum == SENSOR_PORT_4)
	{
		if(g_working_4 == 0)
		{
			_delay_ms(50);
			TCCR5A = 0;
			TCCR5B = 0;
			//TCCR5A |= (1<<WGM50);  // setting Timer 5 for 8 bit(256 counts)
			TCCR5B |= (1<<CS51);   //  timer running at 2 Mhz(Fcpu/8) clock
			EIMSK |= (1<<INT4);    //  Enabling interrupt for echo 
			DDRF |= (1<<PortNum);   // TRIGGER PIN
			PORTF &=~ (1 << PortNum);
			_delay_us(1);
			PORTF |= (1 << PortNum); //Send 10us second pulse // enable trigger
			_delay_us(10);
			PORTF &=~ (1 << PortNum);
			g_working_4 = 1;
			TCNT5 = 0;
			TIMSK5 |= (1<<TOIE5);
		}
	}

	else if(PortNum == SENSOR_PORT_3)  
	{
	 //distance_5 = 0;
		if(g_working_3==0)
		{
			_delay_ms(50);
			TCCR0A = 0;
			TCCR0B = 0;            //  setting Timer 0 for 8 bit(256 counts)
			TCCR0B |= (1<<CS01);  //  timer running at 2 Mhz(Fcpu/8) clock 
			EIMSK |= (1<<INT7);    //  Enabling interrupt for echo 
			DDRF |= (1<<PortNum);   // TRIGGER PIN
			PORTF &=~ (1 << PortNum);
			_delay_us(1); 
			PORTF |= (1 << PortNum); //Send 10us second pulse // enable trigger
			_delay_us(10);
			PORTF &=~ (1 << PortNum);
			g_working_3 = 1;
			TCNT0 = 0;
			TIMSK0 |= (1<<TOIE0);	
		}
	}

}

// This Method Read Sensor Values for Color Sensor
unsigned char cfn_Read_Color_Sensor(uint8_t PortNum)
{
    unsigned int red=0,green=0,white = 0,blue=0,r_freq = 0 ,g_freq = 0, b_freq = 0 , w_freq=0,red_scaled = 0,blue_scaled = 0,large,ratio_b,ratio_r,ratio_g,ratio_w;
    fn_Disable_ADC();
    unsigned int red_count=0,blue_count=0,green_count=0,yellow_count=0,white_count=0,black_count=0,count=0;
	unsigned int hbyt=0,lbyt=0,small=0,r=0,b=0,g=0,wb=0;
	unsigned int red_blue=0,green_blue=0,red_green=0,blue_red=0,blue_green=0,green_red=0;
	if(PortNum == SENSOR_PORT_3)
	{
		while(count<1)
		{
		     DDRF |= (1<<PortNum);// S2
		     DDRE |= (1<<COLORPORT3_PIN_S3);	// S3
		     EIMSK &=~ (1<<INT7);
		     DDRD &=~ (1<<PD7);
		     //TCCR0A = 0x00;
		     //TCCR0B = 0x00;
		     //TCCR0B |= (1<<CS00)|(1<<CS01)|(1<<CS02);
			 //TCNT0 = 0;
		     PORTF |= (1<<PortNum); // S2=1
		     PORTE &=~ (1<<COLORPORT3_PIN_S3);  // S3=0 for white
		     //_delay_ms(2);
		     white =  freq_calc(3);
		     
			 if(white == 9999)
			 return('x');
			 
			 w_freq = 8000/white;
		     //TCNT0 = 0;
		     PORTF &=~ (1<<PortNum);   // S2=0
		     PORTE &=~ (1<<COLORPORT3_PIN_S3);   // S3=0 for red
		     //_delay_ms(20); 
		     red =  freq_calc(3);
		     if(red == 9999)
			 return('x');
			 r_freq = 8000/red;
		     r_freq = r_freq*100;
			 //TCNT0 = 0;
		     PORTF &=~ (1<<PortNum);   // S2=0
		     PORTE |= (1<<COLORPORT3_PIN_S3); // S3=1 blue
		     //_delay_ms(20);
		     blue = freq_calc(3);
		     if(blue == 9999)
			 return('x');
			 b_freq = 8000/blue;
			 b_freq=b_freq*100;
			 PORTF |= (1<<PortNum);   // S2=0
		     PORTE |= (1<<COLORPORT3_PIN_S3); // S3=1 green
		     
			 green = freq_calc(3);
		     if(green == 9999)
			 return('x');
			 g_freq = 8000/green;
			 g_freq=g_freq*100;
		     
		 
		     ratio_w = 1;
		     ratio_r = 1;
		     ratio_b = 1;
		     ratio_g = 1;
		
		    if((r_freq < b_freq) && (r_freq < g_freq ))
			small = r_freq;
		    else if((b_freq < g_freq) && (b_freq < r_freq ))
			small = b_freq;
		    else 
			small = g_freq;
         
		     r=r_freq/small;
		     b=b_freq/small;
		     g=g_freq/small;
		     red_blue=r_freq/b_freq;
		     red_green=r_freq/g_freq;
		     blue_red=b_freq/r_freq;
		     blue_green=b_freq/g_freq;
		     green_red=g_freq/r_freq;
            
			if( w_freq >15)   // original 1300 ,1800   // initial 15
	        {
	            if((r_freq > b_freq)&&(r_freq >g_freq)&&(red_green>=2)&&(red_blue>=2))
			    {
			        if((b==1))
			        {
				         red_count++;
			             fn_SendCharacter_USART0('r');
			        }
			    }
			    else if((b_freq > r_freq)&&(b_freq >g_freq)&&(blue_green>=2)&&(blue_red>=2))
                {
			    //return('b');
				    if((r==1))
				    {
				        if(b_freq < 500)
					    {
					         fn_SendCharacter_USART0('z');
						     black_count++;
					    }
					 
					    else
					    {
					         fn_SendCharacter_USART0('b');
				             blue_count++;
					    }
				    }	 
			    }
		      
                else if((g_freq >r_freq)&&(r==1))
			    {
			        if((small>800)&&(b_freq>g_freq))
				    { 
				         fn_SendCharacter_USART0('w');
					     white_count++;
				    }
				    else if (g_freq<=700)
				    {
				         fn_SendCharacter_USART0('X');
				         black_count++;
				    }
                    else
				    {
				         fn_SendCharacter_USART0('g');
				         green_count++;
				    }
	            }
			   
			    else if(((r_freq > b_freq)||(g_freq > b_freq))&&(b==1))
			    {	
			         yellow_count++;
			         fn_SendCharacter_USART0('y');
			    }	
			    else if((small>800)&&(r==1))
			    {
                     white_count++; 
                     fn_SendCharacter_USART0('W');  
			    }
			    else
	            {
			         fn_SendCharacter_USART0('U');
			    }		
	        }    
		
		    else
		    {
			     black_count++;
                 fn_SendCharacter_USART0('x');
		    }
	     count++;
	    }
        
		if((red_count>blue_count)&&(red_count>green_count)&&(red_count>yellow_count)&&(red_count>black_count)&&(red_count>white_count))
			return('r');
          
		else if((blue_count>red_count)&&(blue_count>green_count)&&(blue_count>yellow_count)&&(blue_count>black_count)&&(blue_count>white_count))
			return('b');
		else if((green_count>blue_count)&&(green_count>red_count)&&(green_count>yellow_count)&&(green_count>black_count)&&(green_count>white_count))
			return('g');
		else if((yellow_count>blue_count)&&(yellow_count>green_count)&&(yellow_count>red_count)&&(yellow_count>black_count)&&(yellow_count>white_count))
			return('y');
		else if((white_count>blue_count)&&(white_count>green_count)&&(white_count>yellow_count)&&(white_count>black_count)&&(white_count>red_count))
			return('w');
		else
			return('x');
	   
	}

	
	else if(PortNum == SENSOR_PORT_4)
	{
		while(count<1)
		{
		     DDRF |= (1<<PortNum);// S2  
		     DDRE |= (1<<COLORPORT4_PIN_S3);	// S3
		     EIMSK &=~ (1<<INT4);
		     DDRL &=~ (1<<PL2);
		     PORTF |= (1<<PortNum); // S2=1
		     PORTE &=~ (1<<COLORPORT4_PIN_S3);  // S3=0 for white
		     white = freq_calc(4);
		     if(white == 9999)
			 return('x');
			 w_freq = 8000/white;
             
			 PORTF &=~ (1<<PortNum); // S2=0
		     PORTE &=~ (1<<COLORPORT4_PIN_S3);  // S3=0 for red
		     red=freq_calc(4);
		     if(red == 9999)
			 return('x');
			 r_freq = 8000/red;
		     r_freq = r_freq*100;
			 
			 PORTF &=~ (1<<PortNum);   // S2=0
		     PORTE |= (1<<COLORPORT4_PIN_S3); // S3=1 blue
		     blue = freq_calc(4);
		     if(blue == 9999)
			 return('x');
			 b_freq = 8000/blue;
		     b_freq = b_freq*100;
		
			 PORTF |= (1<<PortNum);   // S2=0
		     PORTE |= (1<<COLORPORT4_PIN_S3); // S3=1 green
		     green = freq_calc(4);
		     if(green == 9999)
			 return('x');
			 g_freq = 8000/green;
			 g_freq = g_freq*100;
		
		     if((r_freq < b_freq) && (r_freq < g_freq ))
			 small = r_freq;
		     else if((b_freq < g_freq) && (b_freq < r_freq ))
			 small = b_freq;
		     else 
			 small = g_freq;

	         r=r_freq/small;
		     b=b_freq/small;
		     g=g_freq/small;
		     red_blue=r_freq/b_freq;
		     red_green=r_freq/g_freq;
		     blue_red=b_freq/r_freq;
		     blue_green=b_freq/g_freq;
		     green_red=g_freq/r_freq;
           
            if( w_freq >15)   // original 1300 ,1800   // initial 15
	        {
	            if((r_freq > b_freq)&&(r_freq >g_freq)&&(red_green>=2)&&(red_blue>=2))
			    {
			        if((b==1))
			        {
				         red_count++;
			             fn_SendCharacter_USART0('r');
			        }
			    }
			    else if((b_freq > r_freq)&&(b_freq >g_freq)&&(blue_green>=2)&&(blue_red>=2))
                {
			         //return('b');
				    if((r==1))
				    {
				        if(b_freq < 500)
					    {
					         fn_SendCharacter_USART0('z');
						     black_count++;
					    }
					 
					    else
					    {
					         fn_SendCharacter_USART0('b');
				             blue_count++;
					    }
				    }	 
			    }
		        
				else if((g_freq >r_freq)&&(r==1))
			    {
			        if((small>800)&&(b_freq>g_freq))
				    {
				         fn_SendCharacter_USART0('w');
					     white_count++;
				    }
				    else if (g_freq<=700)
				    {
				         fn_SendCharacter_USART0('X');
				         black_count++;
				    }
                    else
				    {
				         fn_SendCharacter_USART0('g');
				         green_count++;
				    }
	            }
			    else if(((r_freq > b_freq)||(g_freq > b_freq))&&(b==1))
			    {	
				
				     yellow_count++;
			         fn_SendCharacter_USART0('y');
			    }	
			    else if((small>800)&&(r==1))
			    {
                     white_count++; 
                     fn_SendCharacter_USART0('W');  
			    }
			    else
	            {
			         fn_SendCharacter_USART0('U');
			    }		
	        }      
		
		    else
		    {
			     black_count++;
                 fn_SendCharacter_USART0('x');
		    }
	         count++;
	    }
        if((red_count>blue_count)&&(red_count>green_count)&&(red_count>yellow_count)&&(red_count>black_count)&&(red_count>white_count))
			return('r');
          
		else if((blue_count>red_count)&&(blue_count>green_count)&&(blue_count>yellow_count)&&(blue_count>black_count)&&(blue_count>white_count))
			return('b');
		else if((green_count>blue_count)&&(green_count>red_count)&&(green_count>yellow_count)&&(green_count>black_count)&&(green_count>white_count))
			return('g');
		else if((yellow_count>blue_count)&&(yellow_count>green_count)&&(yellow_count>red_count)&&(yellow_count>black_count)&&(yellow_count>white_count))
			return('y');
		else if((white_count>blue_count)&&(white_count>green_count)&&(white_count>yellow_count)&&(white_count>black_count)&&(white_count>red_count))
			return('w');
		else
			return('x');
	   
	}

	
	else if(PortNum == SENSOR_PORT_5)
	{
	    while(count<1)
		{
		     DDRF|=(1<<PortNum);// S2
	         DDRE|=(1<<COLORPORT5_PIN_S3);	// S3
	         EIMSK&=~(1<<INT5);
	         DDRE&=~(1<<PE6); 
	         PORTF|=(1<<PortNum); // S2=1
	         PORTE&=~(1<<COLORPORT5_PIN_S3);  // S3=0 for white
	         white=freq_calc(5);
	         if(white == 9999)
			 return('x');
			 w_freq = 8000/white;
	         
			 PORTF&=~(1<<PortNum);   // S2=0
	         PORTE&=~(1<<COLORPORT5_PIN_S3);   // S3=0 for red
	         red=freq_calc(5);
	         if(red == 9999)
			 return('x');
			 r_freq = 8000/red;
	         r_freq = r_freq*100;
			 
	         PORTF&=~(1<<PortNum);   // S2=0
	         PORTE|=(1<<COLORPORT5_PIN_S3); // S3=1 blue
	         blue=freq_calc(5);
	         if(blue == 9999)
			 return('x');
			 b_freq = 8000/blue;
			 b_freq = b_freq*100;
	         TCNT3=0;
	    
             PORTF|=(1<<PortNum);   // S2=0
	         PORTE|=(1<<COLORPORT5_PIN_S3); // S3=1 green
	     	 green=freq_calc(5);
	         if(green == 9999)
			 return('x');
			 g_freq = 8000/green;
			 g_freq = g_freq*100;
	     
             if((r_freq < b_freq) && (r_freq < g_freq ))
			 small = r_freq;
		     else if((b_freq < g_freq) && (b_freq < r_freq ))
			 small = b_freq;
		     else 
			 small = g_freq;

	         r=r_freq/small;
		     b=b_freq/small;
		     g=g_freq/small;
		     red_blue=r_freq/b_freq;
	 	     red_green=r_freq/g_freq;
		     blue_red=b_freq/r_freq;
		     blue_green=b_freq/g_freq;
		     green_red=g_freq/r_freq;
        
		    if( w_freq >15)   // original 1300 ,1800   // initial 15
	        {
	            if((r_freq > b_freq)&&(r_freq >g_freq)&&(red_green>=2)&&(red_blue>=2))
			    {
			        if((b==1))
			        {
				         red_count++;
			             fn_SendCharacter_USART0('r');
			        }
			    }
			    else if((b_freq > r_freq)&&(b_freq >g_freq)&&(blue_green>=2)&&(blue_red>=2))
                {
			         //return('b');
				    if((r==1))
				    {
				        if(b_freq < 500)
					    {
					         fn_SendCharacter_USART0('z');
						     black_count++;
					    }
					    else
					    {
					         fn_SendCharacter_USART0('b');
				             blue_count++;
					    }
				    }	 
			    }
		        
				else if((g_freq >r_freq)&&(r==1))
			    {
			        if((small>800)&&(b_freq>g_freq))
				    {
				         fn_SendCharacter_USART0('w');
					     white_count++;
				    }
				    else if (g_freq<=700)
				    {
				         fn_SendCharacter_USART0('X');
				         black_count++;
				    }
                    else
				    {
				         fn_SendCharacter_USART0('g');
				         green_count++;
				    }
	            }
			
			    else if(((r_freq > b_freq)||(g_freq > b_freq))&&(b==1))
			    {	
				     yellow_count++;
			         fn_SendCharacter_USART0('y');
			    }	
			    
				else if((small>800)&&(r==1))
			    {
                     white_count++; 
                     fn_SendCharacter_USART0('W');  
			    }
			
			    else
	            {
			         fn_SendCharacter_USART0('U');
			    }		
	        }    
		
		    else
		    {
			     black_count++;
                 fn_SendCharacter_USART0('x');
		    }
	         count++;
	    }
        if((red_count>blue_count)&&(red_count>green_count)&&(red_count>yellow_count)&&(red_count>black_count)&&(red_count>white_count))
			return('r');
          
		else if((blue_count>red_count)&&(blue_count>green_count)&&(blue_count>yellow_count)&&(blue_count>black_count)&&(blue_count>white_count))
			return('b');
		else if((green_count>blue_count)&&(green_count>red_count)&&(green_count>yellow_count)&&(green_count>black_count)&&(green_count>white_count))
			return('g');
		else if((yellow_count>blue_count)&&(yellow_count>green_count)&&(yellow_count>red_count)&&(yellow_count>black_count)&&(yellow_count>white_count))
			return('y');
		else if((white_count>blue_count)&&(white_count>green_count)&&(white_count>yellow_count)&&(white_count>black_count)&&(white_count>red_count))
			return('w');
		else
			return('x');
	   
	}

return ' ';
}


// This method is to initialize variables to operate Robotronics Remote
unsigned int uifn_Read_Remote(uint8_t PortNum)
{
	fn_Enable_ADC();
	PortNum = (PortNum)&(0x0F);
	ADMUX |= PortNum;
	ADCSRA |= (1<<ADSC);
	while(!(ADCSRA&(1<<ADIF)));
	ADCSRA &=~ (1<<ADIF);
	ADMUX = (1<<REFS0);
	return(ADCW);
}


/* --------------- Temperature and Humidity Sensor Functions -------------- */

int fn_Read_Humidity_Temperature(uint8_t port, uint8_t option)
{
	fn_Disable_ADC();
	//DHT11_PIN = DHT_PIN;
	PORTB|=(1<<PB0);
	CS_0;
	_delay_ms(1000);
	
	if(option == Temperature)
	{
		uint8_t temperature = fn_Get_data(port, Temperature);
		if(temperature == DHT11_ERROR)
			return 0;
		else
			return temperature;
	}
	else if(option == Humidity)
	{
		uint8_t humid = fn_Get_data(port, Humidity);
		if(humid == DHT11_ERROR)
			return 0;
		else
			return humid;
	}
	
}

uint8_t fn_Get_data(uint8_t DHT11_PIN, uint8_t select)			/* receive data */
{
	uint8_t bits[5];
	uint8_t i, j = 0;

	//reset port
	DDRF |= (1 << DHT11_PIN); //output
	PORTF |= (1 << DHT11_PIN); //high
	_delay_ms(100);

	//send request
	PORTF &= ~(1 << DHT11_PIN); //low
	_delay_ms(18);
	PORTF |= (1 << DHT11_PIN); //high
	_delay_us(1);
	DDRF &= ~(1 << DHT11_PIN); //input
	_delay_us(39);

	//check start condition 1
	if ((PINF & (1 << DHT11_PIN))) {
		return DHT11_ERROR;
	}
	_delay_us(80);
	//check start condition 2
	if (!(PINF & (1 << DHT11_PIN))) {
		return DHT11_ERROR;
	}
	_delay_us(80);

	//read the data
	for (j = 0; j < 5; j++) { //read 5 byte
		uint8_t result = 0;
		for (i = 0; i < 8; i++) { //read every bit
			while (!(PINF & (1 << DHT11_PIN)))
				; //wait for an high input
			_delay_us(30);
			if (PINF & (1 << DHT11_PIN)) //if input is high after 30 us, get result
				result |= (1 << (7 - i));
			while (PINF & (1 << DHT11_PIN))
				; //wait until input get low
		}
		bits[j] = result;
	}

	//reset port
	DDRF |= (1 << DHT11_PIN); //output
	PORTF |= (1 << DHT11_PIN); //low
	_delay_ms(100);

	//check checksum
	if (bits[0] + bits[1] + bits[2] + bits[3] == bits[4]) {
		if (select == 0) { //return temperature
			return (bits[2]);
		} else if (select == 1) { //return humidity
			return (bits[0]);
		}
	}

	return DHT11_ERROR;
}

/* --------Function FOr Water Level Indicator-------- */
unsigned int uifn_Read_waterlevel_sensor(uint8_t PortNum)
{
	if(PortNum == SENSOR_PORT_1 || PortNum == SENSOR_PORT_2)
	{
		fn_Disable_ADC();
		DDRF &=~ (1<<PortNum);	
		
		if((PINF)&(1<<PortNum))
			return W_EMPTY;
		else
			return W_FULL;
	}
	
	else if(PortNum == SENSOR_PORT_3)
	{
		fn_Disable_ADC();
		DDRF &=~ (1<<PortNum);	
		
		if((PINF)&(1<<PortNum))
			return W_EMPTY;
		else
			return W_FULL;
	}

	else if(PortNum == SENSOR_PORT_4)
	{ 
		
		fn_Disable_ADC();
		DDRF &=~ (1<<PortNum);	
		
		if((PINF)&(1<<PortNum))
			return W_EMPTY;
		else
			return W_FULL;
	}

	else if(PortNum == SENSOR_PORT_5)
	{
		fn_Disable_ADC();
		DDRF &=~ (1<<PortNum);	
		 
		if((PINF)&(1<<PortNum))
			return W_EMPTY;
		else
			return W_FULL;
	}
	return -1;
}


/* ---------------------- All the Interrupts are mentioned here ----------------*/

// start of ISR(int 7)
ISR(INT7_vect)
{
	if(g_rise_3==0)	
	{
		if(g_working_3==1)	
		{
			 TCNT0=0;     // Start timer after echo pin goes high
			 g_rise_3=1;  // variable for identifying state of echo pin(low to high)  
		}  	  
	}

	else
	{
		 UltraSensor_3 = (g_timer_3*256 + TCNT0)/116;	// Distance calculated after echo pin goes low
		 g_rise_3=0;     // variable for identifying state of echo pin(high to low)
		 g_timer_3=0;     // setting timer overflow variable to zero after distance calculation
		 g_working_3=0;  // variable for checking distance measurement is calculated
	}

}


// start of ISR(int 5)
ISR(INT5_vect)
{
	if(g_rise_5==0)	
	{
		if(g_working_5==1)	
		{
			 TCNT3=0;   // Start timer after echo pin goes high
			 g_rise_5=1;  // variable for identifying state of echo pin(low to high)
		}  	  
	}
  
	else
	{
		 UltraSensor_5 = (g_timer_5*256 + TCNT3)/116;  // Distance calculated after echo pin goes low	
		 g_rise_5=0;   // variable for identifying state of echo pin(high to low)
		 g_timer_5=0;  // setting timer overflow variable to zero after distance calculation
		 g_working_5=0;  // variable for checking distance measurement is calculated
	}
	
}


//start of ISR(int 4)
ISR(INT4_vect)
{
	if(g_rise_4==0)	
	{
	    if(g_working_4==1)	
	    {
	     TCNT5=0;   // Start timer after echo pin goes high
	     g_rise_4=1;  // variable for identifying state of echo pin(low to high)
	    }  	  
	}

	else
	{
	     //UltraSensor_4 = (g_timer_4*256 + TCNT5)/116;	// Distance calculated after echo pin goes low
	     UltraSensor_4 = (g_timer_4*65536 + TCNT5)/116;	// Distance calculated after echo pin goes low
	     g_rise_4=0;   // variable for identifying state of echo pin(high to low)
	     g_timer_4=0;  // setting timer overflow variable to zero after distance calculation
	     g_working_4=0;  // variable for checking distance measurement is calculated
	}

}



// start of ISR(timer3 overflow)
ISR(TIMER3_OVF_vect)
{
    if(g_rise_5==1)
     g_timer_5++;	// Timer overflow variable incremented after 256 counts(value of TCNT5 greater than 255) 
}


// start of ISR(timer0 overflow)
ISR(TIMER0_OVF_vect)
{
 
    if(g_rise_3==1)
     g_timer_3++;	 // Timer overflow variable incremented after 256 counts(value of TCNT3 greater than 255)
}


// start of ISR(timer5 overflow)
ISR(TIMER5_OVF_vect)
{
    if(g_rise_4==1)
     g_timer_4++;	 // Timer overflow variable incremented after 256 counts(value of TCNT4 greater than 255)
}

unsigned int freq_calc(unsigned int sensor_no)
{
     unsigned int freq_count=0;
    if(sensor_no==3)
	{
	     TCCR3A=0;
		 TCCR3B=0;
	     TCCR3B|=(1<<CS31);
	     TCNT3=0;
		if(!((PIND)&(1<<PD7)))        // IF output is already low
        {
	        while((PIND&(1<<PD7))==0)   // wait for output to become high
 			{
			    if(TCNT3 > 5000)  
				return(9999);
			}
        }
	     TCCR3A=0;
		 TCCR3B=0;
	     TCCR3B|=(1<<CS31);
	     TCNT3=0;
	     while((PIND)&(1<<PD7))
	     {
		    if(TCNT3 > 5000)
			return(9999);
		 }
         freq_count=TCNT3;
	     TCNT3=0;
	     TCCR3B=0;
         return(freq_count);
    }
	else if(sensor_no==4)
	{
	     TCCR3A=0;
		 TCCR3B=0;
	     TCCR3B|=(1<<CS31);
	     TCNT3=0;
		if(!((PINL)&(1<<PL2)))        // IF output is already low
        {
	        while((PINL&(1<<PL2))==0)// wait for output to become high
			{
			    if(TCNT3 > 5000)
			    return(9999);
			}
			
        }
	     TCCR3A=0;
		 TCCR3B=0;
	     TCCR3B|=(1<<CS31);
	     TCNT3=0;
	     
		 while((PINL)&(1<<PL2))
	    {
		    if(TCNT3 > 5000)
			return(9999);
		}
         freq_count=TCNT3;
	     TCNT3=0;
	     TCCR3B=0;
         return(freq_count);
    }
	
    else
	{
	    if(!((PINE)&(1<<PE6)))        // IF output is already low
        {
	        while((PINE&(1<<PE6))==0)      // wait for output to become high
            {
			    if(TCNT3 > 5000)
			    return(9999);
			}
		}
	     TCCR3A=0;
		 TCCR3B=0;
	     TCCR3B|=(1<<CS31);
	     TCNT3=0;
	     
		 while((PINE)&(1<<PE6))
	    {
		    if(TCNT3 > 5000)
			return(9999);
			
		}
         freq_count=TCNT3;
	     TCNT3=0;
	     TCCR3B=0;
         return(freq_count);
    }

}

