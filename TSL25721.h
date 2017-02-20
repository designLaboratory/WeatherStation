#include <stdint.h>

#define TSL25721_ADDR_WRITE   				0x72
#define TSL25721_ADDR_READ    				0x73

#define GAIN_1X 0
#define GAIN_8X 1
#define GAIN_16X 2
#define GAIN_120X 3

//only use this with 1x and 8x gain settings
#define GAIN_DIVIDE_6 1

int8_t readL(uint8_t subadress);
void set_ActiveL(uint8_t gain);
float get_Light();
