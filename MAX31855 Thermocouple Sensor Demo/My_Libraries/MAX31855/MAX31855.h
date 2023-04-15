#define MAX31855_SS_pin_HIGH                               set_P11
#define MAX31855_SS_pin_LOW                                clr_P11

#define no_fault                                           0x00
#define thermocouple_not_detected                          0x01
#define thermocouple_short_circuited_to_GND                0x02
#define thermocouple_short_circuited_to_VCC                0x04


void SPI_HW_Init(unsigned char clk_value);
unsigned char SPI_transfer(unsigned char tx_data);
void MAX31855_init(void);
unsigned long MAX31855_read(void);
unsigned char MAX31855_get_data(float *T_r, float *T_amb);