#include "robog.h"

void main(void)
{
	while(1)
	{
		if(g_SleepFlag == true)
		{
			if(g_FirstSleep == true)
			{
				set_sleep_mode(SLEEP_MODE_PWR_DOWN);
				_delay_ms(500);
				sleep_enable();
				fn_Enable_Interrupt_Sleep();
				sleep_cpu();
			}
			else
			{
				fn_Shutdown();				
			}
		}
		else
		{
			fn_init_wifi_lcd();
			
			while(g_SleepFlag == false)
			{
				POWER_GREEN_LED_OFF
				test_variable = uifn_Read_Remote(6);
				
				if(g_Recvd_Start == 0 && test_variable > 1000)
				{
					g_WifiMode = None;
					fn_SR_RoboGMode();
				}
				else if(g_Recvd_Start == 0 &&  test_variable <= 1000)
				{
					fn_Execute_RobotronicsRemote();
					lcd_clear(10);
					g_Stop_RoboG =false;
					g_WifiMode = Remote;
					print_text("Remote MODE", 3, 2);
				}
				else
				{
					uifn_Execute_RobotronicsRemoteApp();
					lcd_clear(10);
					g_Stop_RoboG = false;
					g_WifiMode = RemoteApp;
				    print_text("Avishkaar App", 3, 2);
				}
			}
		}
	}
}

void fn_SR_RoboGMode(void)
{
	fn_All_Initializations_for_RoboG();

#ifdef __NORMAL__
	if(g_Stop_RoboG == false)
	{
	   
		lcd_clear(10);
		print_text("Motors test", 1, 1);
		print_text("Motors CW Fast", 2, 1);
		
		fn_MoveDCMotors_VariableSpeed(MOTORPORT1, CW,MOTORSPEED_FAST);
		fn_MoveDCMotors_VariableSpeed(MOTORPORT2, CW,MOTORSPEED_FAST);
		fn_MoveDCMotors_VariableSpeed(MOTORPORT3, CW,MOTORSPEED_FAST);
		fn_MoveDCMotors_VariableSpeed(MOTORPORT4, CW,MOTORSPEED_FAST);
		
		fn_Time(3000);
		print_text("Motors ACW Fast", 2, 1);
		
		fn_MoveDCMotors_VariableSpeed(MOTORPORT1, ACW,MOTORSPEED_FAST);
		fn_MoveDCMotors_VariableSpeed(MOTORPORT2, ACW,MOTORSPEED_FAST);
		fn_MoveDCMotors_VariableSpeed(MOTORPORT3, ACW,MOTORSPEED_FAST);
		fn_MoveDCMotors_VariableSpeed(MOTORPORT4, ACW,MOTORSPEED_FAST);
		
		fn_Time(3000);
		
		fn_MoveDCMotors_VariableSpeed(MOTORPORT1, STOP,MOTORSPEED_FAST);
		fn_MoveDCMotors_VariableSpeed(MOTORPORT2, STOP,MOTORSPEED_FAST);
		fn_MoveDCMotors_VariableSpeed(MOTORPORT3, STOP,MOTORSPEED_FAST);
		fn_MoveDCMotors_VariableSpeed(MOTORPORT4, STOP,MOTORSPEED_FAST);
		
		
		while(g_Recvd_Start == 0 && g_SleepFlag == false && uifn_Read_Remote(6)>1000)
	    {
			lcd_clear(10);

			Sensor_1 = uifn_Read_Analog_sensor(SENSOR_PORT_1);
			Sensor_2 = uifn_Read_Analog_sensor(SENSOR_PORT_2);
			Sensor_3 = uifn_Read_Analog_sensor(SENSOR_PORT_3);
			ColorSensor_4 = cfn_Read_Color_Sensor(SENSOR_PORT_4);
			fn_Read_Ultrasonic_Sensor(SENSOR_PORT_5);

			print_text("IR_1", 2, 1);
			print_int(Sensor_1 , 2, 6);
			print_text("IR_2", 3, 1);
			print_int(Sensor_2 , 3, 6);

			print_text("Tch_3", 4, 1);
			print_int(Sensor_3 , 4, 7);

			print_text("Color_4", 5, 1);
			print_char(ColorSensor_4 , 5, 10);

			print_text("PORT_5", 6, 1);
			print_int(UltraSensor_5 , 6, 10);
			fn_Time(500);	
	    }
		g_Stop_RoboG = true;
		lcd_clear(10);
		fn_Stop_FullFeatures();
	}
#else
	if(g_Stop_RoboG == false)
	{

		 while(g_SleepFlag == false && uifn_Read_Remote(6)>1000  && (g_WifiMode == Comm || g_WifiMode == None))
		{
		fn_Read_Ultrasonic_Sensor(SENSOR_PORT_3); 

		fn_Read_Ultrasonic_Sensor(SENSOR_PORT_4); 

		fn_Read_Ultrasonic_Sensor(SENSOR_PORT_5); 

		print_text("Not in use ",2, 1);
		print_text("Not in use ",3, 1);
		print_text("       USONIC3",4, 1);
		print_text("       USONIC4",5, 1);
		print_text("       USONIC5",6, 1);

		print_int(US_3 , 4, 1);

		print_int(US_4 , 5, 1);

		print_int(US_5 , 6, 1);

		fn_Time(500);

		}


		g_Stop_RoboG = true;
		lcd_clear(10);
		fn_Stop_FullFeatures();
	}

#endif
}
