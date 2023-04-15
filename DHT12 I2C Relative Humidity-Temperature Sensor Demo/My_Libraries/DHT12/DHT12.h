#define I2C_W								0x00
#define I2C_R								0x01

#define no_of_bytes_to_read 0x05

#define DHT12_I2C_address		0xB8

#define no_error						0x00
#define CRC_error						0x01


void DHT12_init(void);
unsigned char DHT12_CRC(unsigned char *array_values);
unsigned char DHT12_read_byte(unsigned char address);
unsigned char DHT12_get_data(float *DHT12_RH, float *DHT12_T);
