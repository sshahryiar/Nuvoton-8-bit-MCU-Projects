#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"
#include "soft_delay.h"
#include "DHT12.h"
#include "I2C.h"


void DHT12_init(void)
{
	I2C_init();
  	delay_ms(100);
}


unsigned char DHT12_CRC(unsigned char *array_values)
{
	signed char i = 0x03;
	unsigned char crc_result = 0x00;

	while(i > -1)
	{
		crc_result += array_values[i];
		i--;
	}

	return crc_result;
}



unsigned char DHT12_read_byte(unsigned char address)
{
	unsigned char value = 0x00;

	I2C_start();
	I2C_write(DHT12_I2C_address);
	I2C_write(address);

	I2C_start();
	I2C_write(DHT12_I2C_address | I2C_R);
	value = I2C_read(I2C_NACK);
	I2C_stop();
	
	return value;
}



unsigned char DHT12_get_data(float *DHT12_RH, float *DHT12_T)
{
	signed char i = no_of_bytes_to_read;

	unsigned char values[5] = {0x00, 0x00, 0x00, 0x00, 0x00};

	while(i > 0x00)
	{
		values[(no_of_bytes_to_read - i)] = DHT12_read_byte((no_of_bytes_to_read - i));
		i--;
	};

	if(values[0x04] == DHT12_CRC(values))
	{
		*DHT12_RH = (((float)values[0x00]) + (((float)values[0x01]) * 0.1));
		*DHT12_T = (((float)values[0x02]) + (((float)values[0x03]) * 0.1));

		return no_error;
	}

	else
	{
		return CRC_error;
	}
}


