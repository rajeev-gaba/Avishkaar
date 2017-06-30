#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>

volatile unsigned char sleep_flag=0;
void main()
{
 
    while(1)
   {
        if(sleep_flag==0)
		{
		
		}
		
		else
		{
		    
			DDRF|=(1<<PF1);
			PORTF|=(1<<PF1);
			DDRG|=(1<<PG0);
			PORTG|=(1<<PG0);
			if((PING&(1<<PG0)))   
			{
			    if(!((PINF&(1<<PF1)))  
                {
                }

                else
                {
                } 
			
			}
			
		
		
		}
		
		
		
		
		
		
		
   }

}




ISR(INT3_vect)
{

}


