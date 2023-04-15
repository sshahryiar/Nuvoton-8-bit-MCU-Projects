#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"
#include "soft_delay.h"
#include "LCD_2_Wire.h"
#include "lcd_print.h"
#include "MAX31855.h"


void setup(void);


void main(void)
{	
	float t_in = 0.0;
	float t_out = 0.0;
	signed long t = 0;
	unsigned char fault_msg = 0x00;
	
	setup();
	
	LCD_goto(3, 0);
	LCD_putstr("MAX31855 Demo");
	
	LCD_goto(2, 1);
	LCD_putstr("Tj/ C:");
	print_symbol(5, 1, 0);
	
	LCD_goto(2, 2);
	LCD_putstr("Ta/ C:");
	print_symbol(5, 2, 0);
	
	while(1)
	{
		fault_msg = MAX31855_get_data(&t_out, &t_in);
		
		switch(fault_msg)
		{
			case thermocouple_not_detected:
			{
				LCD_goto(0, 3);
				LCD_putstr("   Check Sensor!    ");
				break;
			}
			
			case thermocouple_short_circuited_to_GND:
			{
				LCD_goto(0, 3);
				LCD_putstr("   Check Jn-GND!    ");				
				break;
			}
			
			case thermocouple_short_circuited_to_VCC:
			{
				LCD_goto(0, 3);
				LCD_putstr("   Check Jn-VCC!    ");
				break;
			}
			
			default:
			{
				print_F(11, 1, t_out, 2);
				print_F(11, 2, t_in, 2);				
				LCD_goto(0, 3);
				LCD_putstr("    System Okay.    ");				
				break;
			}
		};
		
		delay_ms(400);
	};
}

void setup(void)
{
	LCD_init();
	LCD_clear_home();
	load_custom_symbol();
	MAX31855_init();
}
