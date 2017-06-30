#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>

#define MOTORPORT		PORTC		// Motor Port on MicroController

#define CW    '2'	   // Clockwise Direction of Motor
#define ACW	  '1'	  // Anti - Clockwise Direction of Motor
#define STOP  '0'	 // To Stop Motor

#define MOTORPORT1		'1'		// Motor Port 1
#define MOTORPORT2		'2'		// Motor Port 2
#define MOTORPORT3		'3'		// Motor Port 3
#define MOTORPORT4		'4'		// Motor Port 4

#define MOTORSPEED_SLOW     '1'			  // To Move motor at Slow Speed
#define MOTORSPEED_MEDIUM	'2'			 // To Move motor at Medium Speed
#define MOTORSPEED_FAST		'3'			// To Move motor at Fast Speed

#define MOTORPORT2_PWMPIN   OCR4A			  // PWM Pin of Motor at Port 2
#define MOTORPORT3_PWMPIN   OCR4C			 // PWM Pin of Motor at Port 3
#define MOTORPORT4_PWMPIN   OCR4B			// PWM Pin of Motor at Port 4

#define SLOWPWM_VALUE  170				  // PWM Value for Slow Movement
#define MEDPWM_VALUE   200				 // PWM Value for Medium Movement
#define FASTPWM_VALUE  255				// PWM Value for Fast Movement

/* Pins of Motor1  */

#define MOTOR1_PIN1   PC4  
#define MOTOR1_PIN2   PC5

/* Pins of Motor2  */

#define MOTOR2_PIN1   PC6  
#define MOTOR2_PIN2   PC7

/* Pins of Motor3  */

#define MOTOR3_PIN1   PC0  
#define MOTOR3_PIN2   PC1

/* Pins of Motor4  */

#define MOTOR4_PIN1   PC2  
#define MOTOR4_PIN2   PC3

// Macros Regarding Motor Rotation

#define		Rotate_45        4
#define		Rotate_90     	 8
#define		Rotate_135  	 12
#define		Rotate_180  	 16
#define		Rotate_225  	 20
#define		Rotate_270  	 24
#define		Rotate_315 		 28
#define		Rotate_360  	 32

/********Method Declaration Motors for SR Brain ******/
 
void fn_Initialize_DCMotors(void); 			 // To initialize Motor Ports
void fn_Initialize_PWMForMotors(void); 		// To initialize PWM Pins of Motors at Port 2, 3, 4

void fn_MoveDCMotors_VariableSpeed(unsigned char c_Motor_Num, unsigned char c_Dir,unsigned char c_Speed); // function for variable speed movement of motor
void fn_MoveDCMotors_ConstantSpeed(unsigned char c_Motor_Num, unsigned char c_Dir); 					// function for constant speed movement of motor

void fn_Execute_RobotronicsRemote(void);		// Executes Robotronics Hardware Remote 
void fn_Stop_FullFeatures(void);	// Stop all the running motors
void fn_Motors_Interrupt_Enable();

void fn_MoveMotor1_RotationDegree(unsigned char c_Motor_Num, unsigned char c_Dir, unsigned int Degree_Rotation);
void fn_MoveMotor1_RotationCount(unsigned char c_Motor_Num, unsigned char c_Dir, unsigned int rotations);

void fn_Forward_Rotation(unsigned char c_Motor_Num1, unsigned char c_Motor_Num2,unsigned char c_Speed, unsigned int Degree_Rotation);
void fn_Backward_Rotation(unsigned char c_Motor_Num1, unsigned char c_Motor_Num2,unsigned char c_Speed, unsigned int Degree_Rotation);
void fn_Left_Rotation(unsigned char c_Motor_Num1, unsigned char c_Motor_Num2,unsigned char c_Speed, unsigned int Degree_Rotation);
void fn_Right_Rotation(unsigned char c_Motor_Num1, unsigned char c_Motor_Num2,unsigned char c_Speed, unsigned int Degree_Rotation);

void fn_2Motors_Rotation_ByDegree(unsigned char c_Motor_Num1,unsigned char Direction1, unsigned int Degree1, unsigned char c_Motor_Num2,unsigned char Direction2, unsigned int Degree2);
void fn_2Motors_Rotation_ByCount(unsigned char c_Motor_Num1,unsigned char Direction1, unsigned int Count1, unsigned char c_Motor_Num2,unsigned char Direction2, unsigned int Count2);


// End of Motor.h File  *************