#include <stdint.h>

#define HTS221_ADDR_WRITE   				0xBE
#define HTS221_ADDR_READ    				0xBF



int8_t readH(uint8_t subadress);
void set_ResolutionH();
void set_ActiveH();
uint16_t get_Humidity();
