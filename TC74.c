#include "MKL46Z4.H"

#include "I2C.h"
#include "TC74.h"

int8_t ReadTemperature(void){
  uint8_t temperature;
  
  I2C_Enable(I2C1);                           // Enable I2C1
  
  I2C_Start(I2C1);                            // S
  I2C_WriteByte(I2C1, TC74A5_ADDR_WRITE);     // Slave address + W
  I2C_WriteByte(I2C1, RTR);                   // Send "Read Temperature" command
  I2C_Restart(I2C1);                          // RS
  I2C_WriteByte(I2C1, TC74A5_ADDR_READ);      // Slave address + R
  temperature = I2C_ReadByte(I2C1, I2C_NACK); // Get temperature
  I2C_Stop(I2C1);                             // P
  
  I2C_Disable(I2C1);                          // Disable I2C1
  
  return (int8_t) temperature;
}
