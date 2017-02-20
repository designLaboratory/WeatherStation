#include <stdint.h>

#define TEMP   							0x01
#define PRESSURE    				0x02
#define LIGHT  							0x03
#define HUMIDITY    				0x04

#define RED   							0x05
#define BLUE								0x06
#define GREEN_BLUE					0x07
#define GREEN_YELLOW				0x08

uint8_t value_Parameter(int value, uint8_t parameter);
void set_Color(uint8_t scale, uint8_t value, uint8_t parameter);
