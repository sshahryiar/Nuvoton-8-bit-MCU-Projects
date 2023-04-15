#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"
#include "soft_delay.h"
#include "LCD_3_Wire.h"
#include "lcd_print.h"
#include "I2C.h"
#include "DHT12.h"



void main(void)
{
	unsigned char state = 0x00;
	
	float T = 0.0;
	float RH = 0.0;
		
	P15_PushPull_Mode;
	
	LCD_init();
	LCD_clear_home();
	load_custom_symbol();
	
	LCD_goto(0, 0);
	LCD_putstr("R.H / %:");
	LCD_goto(0, 1);
	LCD_putstr("Temp/ C:");
	print_symbol(5, 1, 0);
	
	DHT12_init();
	
	while(1)
	{
	    state = DHT12_get_data(&RH, &T);
		
			switch(state)
			{
				case no_error:
				{
					print_F(11, 0, RH, 1);
          print_F(11, 1, T, 1);
					break;
				}
				
				default:
				{
					LCD_goto(12, 0);
					LCD_putstr("--.-");
					LCD_goto(12, 1);
					LCD_putstr("--.-");
				}
			}
			
			P15 ^= 0x01;
			delay_ms(400);
	};
}


