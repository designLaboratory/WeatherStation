#include "MKL46Z4.H"

#include "I2C.h"
#include "STLM75.h"
#include "transf.h"

int8_t read_Temperature(void){
  uint8_t temperature;
  
  I2C_Enable(I2C1);                           // Enable I2C1
  
  I2C_Start(I2C1);                            // S
  I2C_WriteByte(I2C1, STLM75_ADDR_WRITE);     // Slave address + W
  I2C_WriteByte(I2C1, RTR);                   // Send "Read Temperature" command
  I2C_Restart(I2C1);                          // RS
  I2C_WriteByte(I2C1, STLM75_ADDR_READ);      // Slave address + R
  temperature = I2C_ReadByte(I2C1, I2C_NACK); // Get temperature
  I2C_Stop(I2C1);                             // P
  
  I2C_Disable(I2C1);                          // Disable I2C1
  if(temperature >= 23) set_Color(RED, value_Parameter(temperature, TEMP), TEMP);
	else set_Color(BLUE, value_Parameter(temperature, TEMP), TEMP);
	
	return temperature;
}
