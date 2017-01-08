#include <stdint.h>

#define TC74A5_ADDR_WRITE   0x90
#define TC74A5_ADDR_READ    0x91
#define RTR                 0x00

int8_t ReadTemperature(void);
