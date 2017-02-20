#include <stdint.h>

#define LPS331_I2C_SA0_HIGH         true
#define LPS331_I2C_SA0_LOW          false
#define LPS331_ADDR_WRITE   				0xBA
#define LPS331_ADDR_READ    				0xBB

// Pressure configuration values.
#define LPS331_I2C_PRESSURE_AVG_1   0x00
#define LPS331_I2C_PRESSURE_AVG_2   0x01
#define LPS331_I2C_PRESSURE_AVG_4   0x02
#define LPS331_I2C_PRESSURE_AVG_8   0x03
#define LPS331_I2C_PRESSURE_AVG_16  0x04
#define LPS331_I2C_PRESSURE_AVG_32  0x05
#define LPS331_I2C_PRESSURE_AVG_64  0x06
#define LPS331_I2C_PRESSURE_AVG_128 0x07
#define LPS331_I2C_PRESSURE_AVG_256 0x08
#define LPS331_I2C_PRESSURE_AVG_384 0x09
#define LPS331_I2C_PRESSURE_AVG_512 0x0a

#define LPS331_I2C_DATARATE_7HZ     0x02

int8_t read(uint8_t subadress);
void set_Resolution(uint8_t pressure_avg);
void set_DataRate(uint8_t datarate);
void set_Active(uint8_t is_active);
int32_t get_Pressure();
