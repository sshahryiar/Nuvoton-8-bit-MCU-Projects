#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"
#include "soft_delay.h"
#include "LCD_2_Wire.h"

#define Vmpp                    5.162F
#define Impp                    0.056F
#define R_actual                (Vmpp / Impp)
#define R_fixed                 100.0F //    
#define R_Error                 (R_fixed / R_actual)
#define ADC_Max                 4095.0F
#define VDD                     3.3F
#define scale_factor            2.0F

#define cell_efficiency         0.065F // 6.5% (Typical Amorphous Cell Efficiency)
#define cell_length             0.0854F // 85.4mm as per inscription on the cell
#define cell_width              0.0563F // 56.3mm as per inscription on the cell
#define effective_area_factor   0.90F // Ignoring areas without cell, i.e. boundaries, frames, links, etc
#define cell_area               (cell_length * cell_width) // 0.004816 sq.m  
#define effective_cell_area     (cell_area * effective_area_factor * cell_efficiency) // 0.000281736 sq.m

void setup(void);
unsigned int ADC_read(void);
unsigned int ADC_average(void);
void lcd_print(unsigned char x_pos, unsigned char y_pos, unsigned int value);

void main(void)
{
  unsigned int ADC = 0;
  float v = 0;
  float V = 0;
  float P = 0;
  float E = 0;
  
  setup();
  
  while(1)
  {
      ADC = ADC_average();
      v = ((VDD * ADC) / ADC_Max);        
      V = (v * scale_factor);
      P = (((V * V) / R_fixed) * R_Error);
      E = (P / effective_cell_area);
        
      lcd_print(12, 0, (unsigned int)(P * 1000.0));
      lcd_print(12, 1, (unsigned int)E);
        
      delay_ms(100);
  };
}

void setup(void)
{ 
  LCD_init();
  LCD_clear_home(); 
  LCD_goto(0, 0); 
  LCD_putstr("PV PWR mW:");
  LCD_goto(0, 1); 
  LCD_putstr("E. W/sq.m:");
  
  Enable_ADC_AIN0;
}

unsigned int ADC_read(void)
{
  register unsigned int value = 0x0000;
  
  clr_ADCF;
  set_ADCS;                                 
  while(ADCF == 0);
  
  value = ADCRH;
  value <<= 4;
  value |= ADCRL;
  
  return value;
}

unsigned int ADC_average(void)
{
    signed char samples = 16;
    unsigned long value = 0;
    
    while(samples > 0)
    {
        value += ((unsigned long)ADC_read());
        samples--;
    };
    
    value >>= 4;
    
    return ((unsigned int)value);
}

void lcd_print(unsigned char x_pos, unsigned char y_pos, unsigned int value)
{  
  unsigned char ch = 0;

    if((value > 999) && (value <= 9999))
    {
        ch = (((value % 10000) / 1000) + 0x30);
        LCD_goto(x_pos, y_pos);
        LCD_putchar(ch);
        
        ch = (((value % 1000) / 100) + 0x30);
        LCD_goto((x_pos + 1), y_pos);
        LCD_putchar(ch);
        
        ch = (((value % 100) / 10) + 0x30);
        LCD_goto((x_pos + 2), y_pos);
        LCD_putchar(ch);
        
        ch = ((value % 10) + 0x30);
        LCD_goto((x_pos + 3), y_pos);
        LCD_putchar(ch);
    }
    
    else if((value > 99) && (value <= 999))
    {
        ch = 0x20;
        LCD_goto(x_pos, y_pos);
        LCD_putchar(ch);
        
        ch = (((value % 1000) / 100) + 0x30);
        LCD_goto((x_pos + 1), y_pos);
        LCD_putchar(ch);
        
        ch = (((value % 100) / 10) + 0x30);
        LCD_goto((x_pos + 2), y_pos);
        LCD_putchar(ch);
        
        ch = ((value % 10) + 0x30);
        LCD_goto((x_pos + 3), y_pos);
        LCD_putchar(ch);
    }
    
    else if((value > 9) && (value <= 99))
    {
        ch = 0x20;
        LCD_goto(x_pos, y_pos);
        LCD_putchar(ch);
        
        ch = 0x20;
        LCD_goto((x_pos + 1), y_pos);
        LCD_putchar(ch);
        
        ch = (((value % 100) / 10) + 0x30);
        LCD_goto((x_pos + 2), y_pos);
        LCD_putchar(ch);
        
        ch = ((value % 10) + 0x30);
        LCD_goto((x_pos + 3), y_pos);
        LCD_putchar(ch);
    }
    
    else
    {
        ch = 0x20;
        LCD_goto(x_pos, y_pos);
        LCD_putchar(ch);
        
        ch = 0x20;
        LCD_goto((x_pos + 1), y_pos);
        LCD_putchar(ch);
        
        ch = 0x20;
        LCD_goto((x_pos + 2), y_pos);
        LCD_putchar(ch);
        
        ch = ((value % 10) + 0x30);
        LCD_goto((x_pos + 3), y_pos);
        LCD_putchar(ch);
    }   
}
