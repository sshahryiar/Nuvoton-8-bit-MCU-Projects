#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"
#include "soft_delay.h"
#include "MAX31855.h"


void SPI_HW_Init(unsigned char clk_value)
{
	switch(clk_value)
	{
		case 1:
		{
			clr_SPR1;
			set_SPR0;
			break;
		}
		case 2:
		{
			set_SPR1;
			clr_SPR0;
			break;
		}
		case 3:
		{
			set_SPR1;
			set_SPR0;
			break;
		}
		default:
		{
			clr_SPR1;
			clr_SPR0;
			break;
		}
	}		

	set_DISMODF;
	set_MSTR;
	clr_CPOL;
	clr_CPHA;
	set_SPIEN;
}


unsigned char SPI_transfer(unsigned char tx_data)
{
	unsigned char rx_data = 0x00;
	
	SPDR = tx_data;
	while(!(SPSR & 0x80));  
	rx_data = SPDR;
	clr_SPIF;  

	return rx_data;
}


void MAX31855_init(void)
{
	P11_PushPull_Mode;
	SPI_HW_Init(0);
	delay_ms(100);
}


unsigned long MAX31855_read(void)
{
  signed char i = 0x04;
  unsigned long value = 0x00000000;

  MAX31855_SS_pin_LOW;

  while(i > 0)
  {
		value <<= 0x08;
    value |= SPI_transfer(0x00);    
    i--;
  };

  MAX31855_SS_pin_HIGH;

  return value;
}


unsigned char MAX31855_get_data(float *T_r, float *T_amb)
{
	unsigned long tmp = 0x00000000;
  unsigned long value = 0x00000000;
  float t = 0.0;

  value = MAX31855_read();

	tmp = (value & 0x7FFC0000);
	tmp >>= 18;	
	t = ((float)((float)tmp) * 0.25);
	
	if((value & 0x80000000) != 0x00000000)
	{
		t = -t;
	}
	
	*T_r = t;   

	tmp = (value & 0x0000FFF0);
	tmp >>= 4;
	t = ((float)((float)tmp) * 0.0625);
	
	if((value & 0x00008000) != 0x00000000)
	{
		t = -t;
	}
	
	*T_amb = t;

	return ((unsigned char)(value & 0x00000007));
}