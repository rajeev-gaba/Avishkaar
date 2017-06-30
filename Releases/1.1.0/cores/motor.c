#include "motor.h"
#include "sensor.h"

/* Initializing Motor Ports and Pins */

volatile unsigned int m2_count=0,m3_count=0,m4_count=0,start_1=0,start_2=0,start_3=0,m3_state=0,m4_state=0,m2_state=0;

void fn_Initialize_DCMotors()
{
	DDRC = 0xFF;					// Defining Motor Port as Output Port 
	DDRD |= (1<<PD4)|(1<<PD5); 	// 5v pin for l293d
	PORTD |= (1<<PD4)|(1<<PD5); 	
	DDRJ |= (1<<PJ2); 			//control pin for transistor in motor attachment
	PORTJ |= (1<<PJ2);
	
	//DDRF&=~(1<<PF6);
	//PORTF|=(1<<PF6);
	
	// Initializations for Rotation Motor Count
	
	DDRB|=(1<<PB6); // 5v pin l293d
	PORTB|=(1<<PB6);
	
	DDRH|=(1<<PH6);// 5V pin l293d
	PORTH|=(1<<PH6);
	
	DDRG|=(1<<PG0); // control signal for transistor in motor attachment.
	PORTG&=~(1<<PG0);	
}


/* Initializing PWM for Motors */

void fn_Initialize_PWMForMotors()
{
	DDRH |= (1<<PH3)|(1<<PH4)|(1<<PH5);
	DDRE |= (1<<PE1);
	TCCR4A |= (1<<COM4A1)|(1<<WGM40)|(1<<COM4B1)|(1<<COM4C1);
	TCCR4B |= (1<<CS40);
	
	TCCR2A=0x00;
	TCCR2B|=(1<<CS22)|(1<<CS21)|(1<<CS20);
	TCCR1A|=(1<<WGM10);
	TCCR1B|=(1<<CS12)|(1<<CS10)|(1<<WGM12);
}

/* To Move DC Motors for different c_Speed */

void fn_MoveDCMotors_VariableSpeed(unsigned char c_Motor_Num, unsigned char c_Dir,unsigned char c_Speed)
{
	switch(c_Motor_Num)
	{
		case MOTORPORT1:
					{
						/* Motor at Port 1 will move Anti Clockwise */
						if(c_Dir == ACW)
						{
							MOTORPORT &=~ ((1<<MOTOR1_PIN1)|(1<<MOTOR1_PIN2));
							MOTORPORT |= (1<<MOTOR1_PIN1);
						}
						/* Motor at Port 1 will move Clockwise */	
						else if(c_Dir==CW) 
						{
							MOTORPORT &=~ ((1<<MOTOR1_PIN1)|(1<<MOTOR1_PIN2));
							MOTORPORT |= (1<<MOTOR1_PIN2);
						}
						/* Motor at Port 1 will stop */
						else if(c_Dir==STOP)
						{
							MOTORPORT &=~ ((1<<MOTOR1_PIN1)|(1<<MOTOR1_PIN2));	 
						}
					}
					break;
					
		case MOTORPORT2:
					{
						/* Speed at Motor Port 2 is set to Slow */
						if(c_Speed == MOTORSPEED_SLOW)
						{
							 MOTORPORT2_PWMPIN = SLOWPWM_VALUE;
						}
						/* Speed at Motor Port 2 is set to Medium */
						else if(c_Speed == MOTORSPEED_MEDIUM)
						{
							 MOTORPORT2_PWMPIN = MEDPWM_VALUE;
						}						
						/* Speed at Motor Port 2 is set to Fast */
						else if(c_Speed == MOTORSPEED_FAST)
						{
							 MOTORPORT2_PWMPIN = FASTPWM_VALUE;
						}     
					
						/* Motor at Port 2 will move Anti Clockwise */
						if(c_Dir == ACW)
						{
							MOTORPORT &=~ ((1<<MOTOR2_PIN1)|(1<<MOTOR2_PIN2));
							MOTORPORT |= (1<<MOTOR2_PIN1);  
						}
						/* Motor at Port 2 will move Clockwise */	
						else if(c_Dir == CW) 
						{
							MOTORPORT &=~ ((1<<MOTOR2_PIN1)|(1<<MOTOR2_PIN2));
							MOTORPORT |= (1<<MOTOR2_PIN2);
						}
						/* Motor at Port 2 will Stop */
						else if(c_Dir == STOP)
						{
							MOTORPORT &=~ ((1<<MOTOR2_PIN1)|(1<<MOTOR2_PIN2));	 
						}
					}
					break;
		case MOTORPORT3:
					{
						
						/* Speed at Motor Port 3 is set to Slow */
						if(c_Speed == MOTORSPEED_SLOW)
						{
							 MOTORPORT3_PWMPIN = SLOWPWM_VALUE;
						}
						/* Speed at Motor Port 3 is set to Medium */
						else if(c_Speed == MOTORSPEED_MEDIUM)
						{
							 MOTORPORT3_PWMPIN = MEDPWM_VALUE;
						}
						/* Speed at Motor Port 3 is set to Fast */
						else if(c_Speed == MOTORSPEED_FAST)
						{
							 MOTORPORT3_PWMPIN = FASTPWM_VALUE;
						}
					  
						/* Motor at Port 3 will move Anti Clockwise */
						if(c_Dir == ACW)
						{
							MOTORPORT &=~ ((1<<MOTOR3_PIN1)|(1<<MOTOR3_PIN2));
							MOTORPORT |= (1<<MOTOR3_PIN1);
						}	
						/* Motor at Port 3 will move Clockwise */
						else if(c_Dir == CW) 
						{
							MOTORPORT &=~ ((1<<MOTOR3_PIN1)|(1<<MOTOR3_PIN2));
							MOTORPORT |= (1<<MOTOR3_PIN2);
						}
						/* Motor at Port 3 will Stop */
						else if(c_Dir == STOP)
						{
							MOTORPORT &=~ ((1<<MOTOR3_PIN1)|(1<<MOTOR3_PIN2));	 
						}
					}
					break;
		case MOTORPORT4:
					{
						/* Speed at Motor Port 4 is set to Slow */
						if(c_Speed == MOTORSPEED_SLOW)
						{
							 MOTORPORT4_PWMPIN = SLOWPWM_VALUE;
						}
						/* Speed at Motor Port 4 is set to Medium */
						else if(c_Speed == MOTORSPEED_MEDIUM)
						{
							 MOTORPORT4_PWMPIN = MEDPWM_VALUE;
						}
						/* Speed at Motor Port 4 is set to Fast */
						else if(c_Speed == MOTORSPEED_FAST)
						{
							 MOTORPORT4_PWMPIN = FASTPWM_VALUE;
						}
						
						
						/* Motor at Port 4 will move Clockwise */
						if(c_Dir == ACW)
						{
							MOTORPORT &=~ ((1<<MOTOR4_PIN1)|(1<<MOTOR4_PIN2));
							MOTORPORT |= (1<<MOTOR4_PIN1);
						}	
						/* Motor at Port 4 will move Anti-Clockwise */
						else if(c_Dir == CW) 
						{
							MOTORPORT &=~ ((1<<MOTOR4_PIN1)|(1<<MOTOR4_PIN2));
							MOTORPORT |= (1<<MOTOR4_PIN2);
						}
						/* Motor at Port 4 will move Stop */
						else if(c_Dir == STOP)
						{
							MOTORPORT &=~ ((1<<MOTOR4_PIN1)|(1<<MOTOR4_PIN2));	 
						}
					}
					break;
		default:
			return -1;
	}
}

/* To Move DC Motors for Constant or Fast Speed only */

void fn_MoveDCMotors_ConstantSpeed(unsigned char c_Motor_Num, unsigned char c_Dir)
{
	// Using Function MoveDCMotors_VariableSpeed()
	
	// Function calls prevents Code Re- writing..

	switch(c_Motor_Num)
	{
		case MOTORPORT1:
					{
						/* To Move Motor at Port 1  */
						fn_MoveDCMotors_VariableSpeed(c_Motor_Num, c_Dir, MOTORSPEED_FAST);
					}
					break;
					
		case MOTORPORT2:
					{	/* To Move Motor at Port 1 for Constant Fast Speed  */
						fn_MoveDCMotors_VariableSpeed(c_Motor_Num, c_Dir, MOTORSPEED_FAST);
					}
					break;
		case MOTORPORT3:
					{	/* To Move Motor at Port 1 for Constant Fast Speed  */
						fn_MoveDCMotors_VariableSpeed(c_Motor_Num, c_Dir, MOTORSPEED_FAST);
					}
					break;
		case MOTORPORT4:
					{	/* To Move Motor at Port 1 for Constant Fast Speed */
						fn_MoveDCMotors_VariableSpeed(c_Motor_Num, c_Dir, MOTORSPEED_FAST);
					}
					break;
		default:
			return -1;
	}
}

/* To Move Motors with Avishkaar RObotronics Remote */

void fn_Execute_RobotronicsRemote(void)
{
	fn_Enable_ADC();
	
	DDRF &=~ ((1<<PF2)|(1<<PF5)|(1<<PF6)|(1<<PF3)); // pins for joystick
	PORTF |= (1<<PF3)|(1<<PF2)|(1<<PF5)|(1<<PF6);	
	DDRG |= (1<<PG2);								  // conrtrol pin transistor motor attachment
	PORTG |= (1<<PG0);  //previous
	DDRJ |= (1<<PJ5);
	PORTJ |= (1<<PJ5);
	PORTG &=~ (1<<PG2);

	fn_Initialize_PWMForMotors();

	fn_Initialize_DCMotors();   
	
	//Remote Specific Initiatlizations
	 unsigned int remote_js_1=0;  // joystick2(right joystick) up/down
	 unsigned int remote_js_2=0;  // joystick2(right joystick) left/right
	 unsigned int remote_js_3=0;  // joystick1 updown
	 unsigned int remote_js_4=0;  // joystick1 leftright
	
	
	 remote_js_1 = uifn_Read_Remote(2);
	 remote_js_2 = uifn_Read_Remote(6);
	 remote_js_3 = uifn_Read_Remote(3);
	 remote_js_4 = uifn_Read_Remote(5);
	
//------------------Car drive-------MOTORPORT3>LEFT, MOTORPORT4>RIGHT--------------------------

    
	if (remote_js_1 > 200 && remote_js_1 < 350 && remote_js_2 > 200 && remote_js_2 < 350)  
	{   
		fn_MoveDCMotors_ConstantSpeed(MOTORPORT3,CW);
		fn_MoveDCMotors_ConstantSpeed(MOTORPORT4,STOP);
	}
	else if (remote_js_1 > 200 && remote_js_1 < 350 && remote_js_2 > 660 && remote_js_2 < 800) 
	{   
		fn_MoveDCMotors_ConstantSpeed(MOTORPORT3,STOP);
		fn_MoveDCMotors_ConstantSpeed(MOTORPORT4,CW);
	}
	else if (remote_js_1 > 660 && remote_js_1 < 800 && remote_js_2 > 200 && remote_js_2 < 350) 
	{
	  
		fn_MoveDCMotors_ConstantSpeed(MOTORPORT3,ACW);
		fn_MoveDCMotors_ConstantSpeed(MOTORPORT4,STOP);
	}
	else if (remote_js_1 > 660 && remote_js_1 < 800 && remote_js_2 > 660 && remote_js_2 < 800)
	{
	  
		fn_MoveDCMotors_ConstantSpeed(MOTORPORT3,STOP);
		fn_MoveDCMotors_ConstantSpeed(MOTORPORT4,ACW);
	}

	else if (remote_js_1 > 200 && remote_js_1 < 350 )
	{
	  
		fn_MoveDCMotors_ConstantSpeed(MOTORPORT3,CW);
		fn_MoveDCMotors_ConstantSpeed(MOTORPORT4,ACW);
	}
	else if (remote_js_1 > 660 && remote_js_1 < 800)
	{
	  
		fn_MoveDCMotors_ConstantSpeed(MOTORPORT3,ACW);
		fn_MoveDCMotors_ConstantSpeed(MOTORPORT4,CW);
	}
	else if (remote_js_2 > 200 && remote_js_2 < 350)
	{
	 
		fn_MoveDCMotors_ConstantSpeed(MOTORPORT3,ACW);
		fn_MoveDCMotors_ConstantSpeed(MOTORPORT4,ACW);  
	}
	else if (remote_js_2 > 660 && remote_js_2 < 800)
	{
	 
		fn_MoveDCMotors_ConstantSpeed(MOTORPORT3,CW);
		fn_MoveDCMotors_ConstantSpeed(MOTORPORT4,CW);
	}
	else
	{
	 
		fn_MoveDCMotors_ConstantSpeed(MOTORPORT3,STOP);
		fn_MoveDCMotors_ConstantSpeed(MOTORPORT4,STOP);
	}
	
	if (remote_js_3>200 && remote_js_3 < 350)
	{
	
		fn_MoveDCMotors_ConstantSpeed(MOTORPORT1,CW);
	}
	else if (remote_js_3 >660 && remote_js_3 < 800)
	{
	 
		fn_MoveDCMotors_ConstantSpeed(MOTORPORT1,ACW);
	}

	else 
	{
   
		fn_MoveDCMotors_ConstantSpeed(MOTORPORT1,STOP);
	}

	
	if (remote_js_4>200 && remote_js_4 < 350)
	{
		fn_MoveDCMotors_ConstantSpeed(MOTORPORT2,CW);
	}
	else if (remote_js_4 > 660 && remote_js_4 < 800)
	{
		fn_MoveDCMotors_ConstantSpeed(MOTORPORT2,ACW);
	}

	else 
	{
		fn_MoveDCMotors_ConstantSpeed(MOTORPORT2,STOP);
	}
	
	return;
}

/* To Move Motors with Avishkaar RObotronics Remote */

/* Enable Interrupt for Motors */

void fn_Motors_Interrupt_Enable()
{
	cli();
	DDRD &=~ ((1<<PD1)|(1<<PD0)|(1<<PD2));
	EICRA |= (1<<ISC00)|(1<<ISC10)|(1<<ISC20);
	EIMSK |= (1<<INT0)|(1<<INT1)|(1<<INT2);
	sei();
	
	TCNT2=0;
	TCNT1=0;
	if((PIND)&(1<<PD2))
	{
		m2_state=1;
	}
	else
	{
		m2_state=0;
	}
	if((PIND)&(1<<PD1))
	{
		m3_state=1;
	}
	else
	{
		m3_state=0;
	}
	if((PIND)&(1<<PD0))
	{
		m4_state=1;
	}
	else
	{
		m4_state=0;
	}
}

/* To Move Motors with Avishkaar RObotronics Remote */

void fn_Stop_FullFeatures(void)
{
	// To Stop All Motors
	
	fn_MoveDCMotors_ConstantSpeed(MOTORPORT1,STOP);
	fn_MoveDCMotors_ConstantSpeed(MOTORPORT2,STOP);
	fn_MoveDCMotors_ConstantSpeed(MOTORPORT3,STOP);
	fn_MoveDCMotors_ConstantSpeed(MOTORPORT4,STOP);
}
void fn_MoveMotor1_RotationDegree(unsigned char c_Motor_Num, unsigned char c_Dir, unsigned int Degree_Rotation)
{
	m2_count = 0;
	m3_count = 0;
	m4_count = 0;
	
	switch(c_Motor_Num)
	{
		case MOTORPORT2:
		{
			
			while(m2_count < Degree_Rotation)
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num, c_Dir, MOTORSPEED_FAST);
			}
			fn_MoveDCMotors_VariableSpeed(c_Motor_Num, STOP, MOTORSPEED_FAST);
			
		}
			break;
			
		case MOTORPORT3:
		{
			while(m3_count < Degree_Rotation)
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num, c_Dir, MOTORSPEED_FAST);
			}
			fn_MoveDCMotors_VariableSpeed(c_Motor_Num, STOP, MOTORSPEED_FAST);
		}
			break;
			
		case MOTORPORT4:
		{
			while(m4_count < Degree_Rotation)
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num, c_Dir, MOTORSPEED_FAST);
			}
			fn_MoveDCMotors_VariableSpeed(c_Motor_Num, STOP, MOTORSPEED_FAST);
		}
			break;
			
		default:
			return -1;
	}
}

void fn_MoveMotor1_RotationCount(unsigned char c_Motor_Num, unsigned char c_Dir, unsigned int rotations)
{
	int Rotation_Count = 0;	

	while(rotations > Rotation_Count)
	{
		m2_count = 0;
		m3_count = 0;
		m4_count = 0;
		switch(c_Motor_Num)
		{
			case MOTORPORT2:
			{
				
				while(m2_count < 32)
				{
					fn_MoveDCMotors_VariableSpeed(c_Motor_Num, c_Dir, MOTORSPEED_FAST);
				}
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num, STOP, MOTORSPEED_FAST);
				
			}
				break;
				
			case MOTORPORT3:
			{
				while(m3_count < 32)
				{
					fn_MoveDCMotors_VariableSpeed(c_Motor_Num, c_Dir, MOTORSPEED_FAST);
				}
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num, STOP, MOTORSPEED_FAST);
			}
				break;
				
			case MOTORPORT4:
			{
				while(m4_count < 32)
				{
					fn_MoveDCMotors_VariableSpeed(c_Motor_Num, c_Dir, MOTORSPEED_FAST);
				}
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num, STOP, MOTORSPEED_FAST);
			}
				break;
				
			default:
				return -1;
		}
		Rotation_Count = Rotation_Count + 1;
	}
}

void fn_Forward_Rotation(unsigned char c_Motor_Num1, unsigned char c_Motor_Num2,unsigned char c_Speed, unsigned int Degree_Rotation)
{
	//fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, CW, c_Speed);
	fn_MoveDCMotors_VariableSpeed(MOTORPORT2, CW, MOTORSPEED_FAST);
	fn_MoveDCMotors_VariableSpeed(MOTORPORT3, CW, MOTORSPEED_FAST);
	//fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, ACW, c_Speed);
	fn_MoveDCMotors_VariableSpeed(MOTORPORT4, CW, MOTORSPEED_FAST);
	//m2_count = 0;
	//m3_count = 0;
	//m4_count = 0;
	
	while (m4_count < 320 || m3_count < 320 || m2_count < 320) 
	{
		if(m4_count < m3_count && m4_count < m2_count) 
		{
			
			fn_MoveDCMotors_VariableSpeed(MOTORPORT3, STOP, MOTORSPEED_FAST);
			fn_MoveDCMotors_VariableSpeed(MOTORPORT2, STOP, MOTORSPEED_FAST); 
			
			fn_MoveDCMotors_VariableSpeed(MOTORPORT4, CW, MOTORSPEED_FAST);
		} 
		else if(m3_count < m4_count && m3_count < m2_count) 
		{
			
			fn_MoveDCMotors_VariableSpeed(MOTORPORT2, STOP, MOTORSPEED_FAST);
			fn_MoveDCMotors_VariableSpeed(MOTORPORT4, STOP, MOTORSPEED_FAST); 
		
			fn_MoveDCMotors_VariableSpeed(MOTORPORT3, CW, MOTORSPEED_FAST);
		}
		else if(m2_count < m3_count && m2_count < m4_count) 
		{
			
			fn_MoveDCMotors_VariableSpeed(MOTORPORT3, STOP, MOTORSPEED_FAST);
			fn_MoveDCMotors_VariableSpeed(MOTORPORT4, STOP, MOTORSPEED_FAST); 
		
			fn_MoveDCMotors_VariableSpeed(MOTORPORT2, CW, MOTORSPEED_FAST);
		}
		else
		{
			fn_MoveDCMotors_VariableSpeed(MOTORPORT2, CW, MOTORSPEED_FAST);
			fn_MoveDCMotors_VariableSpeed(MOTORPORT3, CW, MOTORSPEED_FAST); 
			fn_MoveDCMotors_VariableSpeed(MOTORPORT4, CW, MOTORSPEED_FAST); 
		}
	}
	/*if((c_Motor_Num1 == MOTORPORT2 && c_Motor_Num2 == MOTORPORT3) || (c_Motor_Num1 == MOTORPORT3 && c_Motor_Num2 == MOTORPORT2))
	{
		while (m2_count < Degree_Rotation || m3_count < Degree_Rotation) 
		{
			if(m2_count < m3_count) 
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, CW, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, STOP, c_Speed); 
			} 
			else if(m3_count < m2_count) 
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, STOP, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, ACW, c_Speed); 
			}
			else
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, CW, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, ACW, c_Speed); 
			}   
		}
	}
	else if((c_Motor_Num1 == MOTORPORT2 && c_Motor_Num2 == MOTORPORT4) || (c_Motor_Num1 == MOTORPORT4 && c_Motor_Num2 == MOTORPORT2))
	{
		while (m2_count < Degree_Rotation || m4_count < Degree_Rotation) 
		{
			if(m2_count < m4_count) 
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, CW, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, STOP, c_Speed); 
			} 
			else if(m4_count < m2_count) 
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, STOP, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, ACW, c_Speed); 
			}
			else
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, CW, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, ACW, c_Speed); 
			}   
		}
	}
	else if((c_Motor_Num1 == MOTORPORT4 && c_Motor_Num2 == MOTORPORT3) || (c_Motor_Num1 == MOTORPORT3 && c_Motor_Num2 == MOTORPORT4))
	{
		while (m4_count < Degree_Rotation || m3_count < Degree_Rotation) 
		{
			if(m4_count < m3_count) 
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, CW, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, STOP, c_Speed); 
			} 
			else if(m3_count < m4_count) 
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, STOP, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, ACW, c_Speed); 
			}
			else
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, CW, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, ACW, c_Speed); 
			}   
		}
	}*/
	fn_MoveDCMotors_VariableSpeed(MOTORPORT3, STOP, MOTORSPEED_FAST);
	fn_MoveDCMotors_VariableSpeed(MOTORPORT4, STOP, MOTORSPEED_FAST);
}


void fn_Backward_Rotation(unsigned char c_Motor_Num1, unsigned char c_Motor_Num2,unsigned char c_Speed, unsigned int Degree_Rotation)
{
	fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, ACW, c_Speed);
	fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, CW, c_Speed);
	
	if((c_Motor_Num1 == MOTORPORT2 && c_Motor_Num2 == MOTORPORT3) || (c_Motor_Num1 == MOTORPORT3 && c_Motor_Num2 == MOTORPORT2))
	{
		while (m2_count < Degree_Rotation || m3_count < Degree_Rotation) 
		{
			if(m2_count < m3_count) 
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, ACW, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, STOP, c_Speed); 
			} 
			else if(m3_count < m2_count) 
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, STOP, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, CW, c_Speed); 
			}
			else
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, ACW, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, CW, c_Speed); 
			}   
		}
	}
	if((c_Motor_Num1 == MOTORPORT2 && c_Motor_Num2 == MOTORPORT4) || (c_Motor_Num1 == MOTORPORT4 && c_Motor_Num2 == MOTORPORT2))
	{
		while (m2_count < Degree_Rotation || m4_count < Degree_Rotation) 
		{
			if(m2_count < m4_count) 
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, ACW, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, STOP, c_Speed); 
			} 
			else if(m4_count < m2_count) 
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, STOP, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, CW, c_Speed); 
			}
			else
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, ACW, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, CW, c_Speed); 
			}   
		}
	}
	if((c_Motor_Num1 == MOTORPORT4 && c_Motor_Num2 == MOTORPORT3) || (c_Motor_Num1 == MOTORPORT3 && c_Motor_Num2 == MOTORPORT4))
	{
		while (m4_count < Degree_Rotation || m3_count < Degree_Rotation) 
		{
			if(m4_count < m3_count) 
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, ACW, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, STOP, c_Speed); 
			} 
			else if(m3_count < m4_count) 
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, STOP, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, CW, c_Speed); 
			}
			else
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, ACW, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, CW, c_Speed); 
			}
		}
	}
}


void fn_Left_Rotation(unsigned char c_Motor_Num1, unsigned char c_Motor_Num2,unsigned char c_Speed, unsigned int Degree_Rotation)
{
	fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, CW, c_Speed);
	fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, CW, c_Speed);
	
	if((c_Motor_Num1 == MOTORPORT2 && c_Motor_Num2 == MOTORPORT3) || (c_Motor_Num1 == MOTORPORT3 && c_Motor_Num2 == MOTORPORT2))
	{
		while (m2_count < Degree_Rotation || m3_count < Degree_Rotation) 
		{
			if(m2_count < m3_count) 
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, CW, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, STOP, c_Speed); 
			} 
			else if(m3_count < m2_count) 
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, STOP, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, CW, c_Speed); 
			}
			else
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, CW, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, CW, c_Speed); 
			}   
		}
	}
	if((c_Motor_Num1 == MOTORPORT2 && c_Motor_Num2 == MOTORPORT4) || (c_Motor_Num1 == MOTORPORT4 && c_Motor_Num2 == MOTORPORT2))
	{
		while (m2_count < Degree_Rotation || m4_count < Degree_Rotation) 
		{
			if(m2_count < m4_count) 
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, CW, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, STOP, c_Speed); 
			} 
			else if(m4_count < m2_count) 
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, STOP, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, CW, c_Speed); 
			}
			else
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, CW, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, CW, c_Speed); 
			}   
		}
	}
	if((c_Motor_Num1 == MOTORPORT4 && c_Motor_Num2 == MOTORPORT3) || (c_Motor_Num1 == MOTORPORT3 && c_Motor_Num2 == MOTORPORT4))
	{
		while (m4_count < Degree_Rotation || m3_count < Degree_Rotation) 
		{
			if(m4_count < m3_count) 
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, CW, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, STOP, c_Speed); 
			} 
			else if(m3_count < m4_count) 
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, STOP, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, CW, c_Speed); 
			}
			else
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, CW, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, CW, c_Speed); 
			}   
		}
	}
}




void fn_Right_Rotation(unsigned char c_Motor_Num1, unsigned char c_Motor_Num2,unsigned char c_Speed, unsigned int Degree_Rotation)
{
	fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, ACW, c_Speed);
	fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, ACW, c_Speed);
	
	if((c_Motor_Num1 == MOTORPORT2 && c_Motor_Num2 == MOTORPORT3) || (c_Motor_Num1 == MOTORPORT3 && c_Motor_Num2 == MOTORPORT2))
	{
		while (m2_count < Degree_Rotation || m3_count < Degree_Rotation) 
		{
			if(m2_count < m3_count) 
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, ACW, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, STOP, c_Speed); 
			} 
			else if(m3_count < m2_count) 
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, STOP, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, ACW, c_Speed); 
			}
			else
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, ACW, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, ACW, c_Speed); 
			}   
		}
	}
	if((c_Motor_Num1 == MOTORPORT2 && c_Motor_Num2 == MOTORPORT4) || (c_Motor_Num1 == MOTORPORT4 && c_Motor_Num2 == MOTORPORT2))
	{
		while (m2_count < Degree_Rotation || m4_count < Degree_Rotation) 
		{
			if(m2_count < m4_count) 
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, ACW, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, STOP, c_Speed); 
			} 
			else if(m4_count < m2_count) 
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, STOP, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, ACW, c_Speed); 
			}
			else
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, ACW, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, ACW, c_Speed); 
			}   
		}
	}
	if((c_Motor_Num1 == MOTORPORT4 && c_Motor_Num2 == MOTORPORT3) || (c_Motor_Num1 == MOTORPORT3 && c_Motor_Num2 == MOTORPORT4))
	{
		while (m4_count < Degree_Rotation || m3_count < Degree_Rotation) 
		{
			if(m4_count < m3_count) 
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, ACW, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, STOP, c_Speed); 
			} 
			else if(m3_count < m4_count) 
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, STOP, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, ACW, c_Speed); 
			}
			else
			{
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, ACW, c_Speed);
				fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, ACW, c_Speed); 
			}   
		}
	}
}


void fn_2Motors_Rotation_ByDegree(unsigned char c_Motor_Num1,unsigned char Direction1, unsigned int Degree1, unsigned char c_Motor_Num2,unsigned char Direction2, unsigned int Degree2)
{
	m2_count = 0;
	m3_count = 0;
	m4_count = 0;
	
	fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, Direction1, MOTORSPEED_FAST);
	fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, Direction2, MOTORSPEED_FAST);
	int Degree_Rotation = 0, extraRotation = 0;
	
	if(Degree1 > Degree2)
	{
		Degree_Rotation = Degree2;
		extraRotation = Degree1 - Degree2;
	}
	else if(Degree2 > Degree1)
	{
		Degree_Rotation = Degree1;
		extraRotation = Degree2 - Degree1;
	}
	else if(Degree2 == Degree1)
	{
		Degree_Rotation = Degree1;
		extraRotation = 0;
	}
	
	
	if((c_Motor_Num1 == MOTORPORT2 && c_Motor_Num2 == MOTORPORT3))
	{
		while (m2_count < Degree_Rotation || m3_count < Degree_Rotation)
		{
			if(m3_count > m2_count) 
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT2, Direction1, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT3, STOP, MOTORSPEED_FAST); 
			} 
			else if(m2_count > m3_count) 
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT2, STOP, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT3, Direction2, MOTORSPEED_FAST); 
			}
			else
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT2, Direction1, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT3, Direction2, MOTORSPEED_FAST); 
			}   
		}	
	}
	else if((c_Motor_Num1 == MOTORPORT3 && c_Motor_Num2 == MOTORPORT2))
	{
		while (m2_count < Degree_Rotation || m3_count < Degree_Rotation)
		{
			if(m2_count > m3_count) 
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT3, Direction1, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT2, STOP, MOTORSPEED_FAST); 
			} 
			else if(m3_count > m2_count) 
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT3, STOP, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT2, Direction2, MOTORSPEED_FAST); 
			}
			else
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT3, Direction1, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT2, Direction2, MOTORSPEED_FAST); 
			}   
		}	
	}
	else if((c_Motor_Num1 == MOTORPORT2 && c_Motor_Num2 == MOTORPORT4))
	{
		while (m2_count < Degree_Rotation || m4_count < Degree_Rotation)
		{
			if(m4_count > m2_count) 
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT2, Direction1, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT4, STOP, MOTORSPEED_FAST); 
			} 
			else if(m2_count > m4_count) 
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT2, STOP, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT4, Direction2, MOTORSPEED_FAST); 
			}
			else
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT2, Direction1, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT4, Direction2, MOTORSPEED_FAST); 
			}   
		}
	}
	else if((c_Motor_Num1 == MOTORPORT4 && c_Motor_Num2 == MOTORPORT2))
	{
		while (m2_count < Degree_Rotation || m4_count < Degree_Rotation)
		{
			if(m2_count > m4_count) 
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT4, Direction1, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT2, STOP, MOTORSPEED_FAST); 
			} 
			else if(m4_count > m2_count) 
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT4, STOP, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT2, Direction2, MOTORSPEED_FAST); 
			}
			else
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT4, Direction1, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT2, Direction2, MOTORSPEED_FAST); 
			}   
		}
	}

	else if((c_Motor_Num1 == MOTORPORT3 && c_Motor_Num2 == MOTORPORT4))
	{
		while (m3_count < Degree_Rotation || m4_count < Degree_Rotation)
		{
			if(m4_count > m3_count) 
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT3, Direction1, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT4, STOP, MOTORSPEED_FAST); 
			} 
			else if(m3_count > m4_count) 
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT3, STOP, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT4, Direction2, MOTORSPEED_FAST); 
			}
			else
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT3, Direction1, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT4, Direction2, MOTORSPEED_FAST); 
			}   
		}
	}
	else if((c_Motor_Num1 == MOTORPORT4 && c_Motor_Num2 == MOTORPORT3))
	{
		while (m3_count < Degree_Rotation || m4_count < Degree_Rotation)
		{
			if(m3_count > m4_count) 
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT4, Direction1, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT3, STOP, MOTORSPEED_FAST); 
			} 
			else if(m4_count > m3_count) 
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT4, STOP, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT3, Direction2, MOTORSPEED_FAST); 
			}
			else
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT4, Direction1, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT3, Direction2, MOTORSPEED_FAST); 
			}   
		}
	}

	
	fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, STOP, MOTORSPEED_FAST); 
	fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, STOP, MOTORSPEED_FAST); 
	if(Degree1 > Degree2)
	{
		fn_MoveMotor1_RotationDegree(c_Motor_Num1,Direction1, extraRotation);
		fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, STOP, MOTORSPEED_FAST);
	}
	else if(Degree2 > Degree1)
	{
		fn_MoveMotor1_RotationDegree(c_Motor_Num2,Direction2, extraRotation);
		fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, STOP, MOTORSPEED_FAST);
	}
}

void fn_2Motors_Rotation_ByCount(unsigned char c_Motor_Num1,unsigned char Direction1, unsigned int Count1, unsigned char c_Motor_Num2,unsigned char Direction2, unsigned int Count2)
{
	m2_count = 0;
	m3_count = 0;
	m4_count = 0;
	
	fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, Direction1, MOTORSPEED_FAST);
	fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, Direction2, MOTORSPEED_FAST);
	unsigned int Rotation = 32, extraRotation = 0;
	
	if(Count1 > Count2)
	{
		Rotation = 32*Count2;
		extraRotation = 32*(Count1-Count2);
	}
	else if(Count2 > Count1)
	{
		Rotation = 32*Count1;
		extraRotation = 32*(Count2-Count1);
	}
	else if(Count2 == Count1)
	{
		Rotation = 32*Count1;
		extraRotation = 0;
	}
	
	if((c_Motor_Num1 == MOTORPORT2 && c_Motor_Num2 == MOTORPORT3))
	{
		while (m2_count < Rotation || m3_count < Rotation) 
		{
			if(m2_count < m3_count)
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT2, Direction1, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT3, STOP, MOTORSPEED_FAST); 
			} 
			else if(m3_count < m2_count) 
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT2, STOP, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT3, Direction2, MOTORSPEED_FAST); 
			}
			else
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT2, Direction1, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT3, Direction2, MOTORSPEED_FAST); 
			}   
		}	
	}
	else if((c_Motor_Num1 == MOTORPORT3 && c_Motor_Num2 == MOTORPORT2))
	{
		while (m2_count < Rotation || m3_count < Rotation) 
		{
			if(m2_count < m3_count)
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT2, Direction2, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT3, STOP, MOTORSPEED_FAST); 
			} 
			else if(m3_count < m2_count) 
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT2, STOP, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT3, Direction1, MOTORSPEED_FAST); 
			}
			else
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT2, Direction2, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT3, Direction1, MOTORSPEED_FAST); 
			}   
		}	
	}
	else if((c_Motor_Num1 == MOTORPORT2 && c_Motor_Num2 == MOTORPORT4))
	{
		while (m2_count < Rotation || m4_count < Rotation) 
		{
			if(m2_count < m4_count)
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT2, Direction1, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT4, STOP, MOTORSPEED_FAST); 
			} 
			else if(m4_count < m2_count) 
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT2, STOP, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT4, Direction2, MOTORSPEED_FAST); 
			}
			else
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT2, Direction1, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT4, Direction2, MOTORSPEED_FAST); 
			}   
		}	
	}
	else if((c_Motor_Num1 == MOTORPORT4 && c_Motor_Num2 == MOTORPORT2))
	{
		while (m2_count < Rotation || m4_count < Rotation) 
		{
			if(m2_count < m4_count)
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT2, Direction2, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT4, STOP, MOTORSPEED_FAST); 
			} 
			else if(m4_count < m2_count) 
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT2, STOP, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT4, Direction1, MOTORSPEED_FAST); 
			}
			else
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT2, Direction2, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT4, Direction1, MOTORSPEED_FAST); 
			}   
		}	
	}
	else if((c_Motor_Num1 == MOTORPORT3 && c_Motor_Num2 == MOTORPORT4))
	{
		while (m3_count < Rotation || m4_count < Rotation) 
		{
			if(m3_count < m4_count)
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT3, Direction1, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT4, STOP, MOTORSPEED_FAST); 
			} 
			else if(m4_count < m3_count) 
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT3, STOP, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT4, Direction2, MOTORSPEED_FAST); 
			}
			else
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT3, Direction1, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT4, Direction2, MOTORSPEED_FAST); 
			}   
		}	
	}
	else if((c_Motor_Num1 == MOTORPORT4 && c_Motor_Num2 == MOTORPORT3))
	{
		while (m3_count < Rotation || m4_count < Rotation) 
		{
			if(m3_count < m4_count)
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT3, Direction2, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT4, STOP, MOTORSPEED_FAST); 
			} 
			else if(m4_count < m3_count) 
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT3, STOP, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT4, Direction1, MOTORSPEED_FAST); 
			}
			else
			{
				fn_MoveDCMotors_VariableSpeed(MOTORPORT3, Direction2, MOTORSPEED_FAST);
				fn_MoveDCMotors_VariableSpeed(MOTORPORT4, Direction1, MOTORSPEED_FAST); 
			}   
		}	
	}
	
	fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, STOP, MOTORSPEED_FAST); 
	fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, STOP, MOTORSPEED_FAST); 
	if(Count1 > Count2)
	{
		fn_MoveMotor1_RotationCount(c_Motor_Num1,Direction1, extraRotation);
		fn_MoveDCMotors_VariableSpeed(c_Motor_Num1, STOP, MOTORSPEED_FAST);
	}
	else if(Count2 > Count1)
	{
		fn_MoveMotor1_RotationCount(c_Motor_Num2,Direction2, extraRotation);
		fn_MoveDCMotors_VariableSpeed(c_Motor_Num2, STOP, MOTORSPEED_FAST);
	}
}


ISR(INT2_vect)
{
    if(m2_state==0)
    {
		if(PIND&(1<<PD2))
		{
			m2_state=1;
			m2_count++;
		}
		else{}
	
	}	 
	     
    
    else
    {
		if(PIND&(1<<PD2))
		{
		}
		else
		{
			m2_state=0;
			m2_count++;
		}
        //TCNT1=0;
    }   
}	

ISR(INT0_vect)
{
    if(m4_state==0)
    {
		if(PIND&(1<<PD0))
		{
			m4_state=1;
			m4_count++;
		}
		else{}
	}
    else
    {
		if(PIND&(1<<PD0))
		{
		}
		else
		{
			m4_state=0;
			m4_count++;
		}
        //TCNT1=0;
    }
}
ISR(INT1_vect)
{
    if(m3_state==0)
    {
		if(PIND&(1<<PD1))
		{
			m3_state = 1;
			m3_count++;
		}
		else{}
	}
    else
    {
		if(PIND&(1<<PD1))
		{
		}
		else
		{
			m3_state=0;
			m3_count++;
		}
        //TCNT1=0;
    }
}


