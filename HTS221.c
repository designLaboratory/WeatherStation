#include "MKL46Z4.H"

#include "I2C.h"
#include "HTS221.h"
#include "transf.h"

int8_t readH(uint8_t subadress){
	uint8_t status;
  
  I2C_Enable(I2C1);                           // Enable I2C1
  
  I2C_Start(I2C1);                            // S
  I2C_WriteByte(I2C1, HTS221_ADDR_WRITE);     // Slave address + W
  I2C_WriteByte(I2C1, subadress);             // Send command
  I2C_Restart(I2C1);                          // RS
  I2C_WriteByte(I2C1, HTS221_ADDR_READ);      // Slave address + R
  status = I2C_ReadByte(I2C1, I2C_NACK); 			// Get value
  I2C_Stop(I2C1);                             // P
  
  I2C_Disable(I2C1);                          // Disable I2C1
  
  return (int8_t) status;
}

void set_ResolutionH(){
	
	I2C_Enable(I2C1);                           // Enable I2C1
  I2C_Start(I2C1);
	I2C_WriteByte(I2C1, HTS221_ADDR_WRITE);
	I2C_WriteByte(I2C1, 0x10);									// register AV_CONF
	I2C_WriteByte(I2C1, (0x18|0x03));						// set average to 32
	I2C_Stop(I2C1);                             // P
  
  I2C_Disable(I2C1);
}

void set_ActiveH(){
	I2C_Enable(I2C1);                           // Enable I2C1
  I2C_Start(I2C1);
	I2C_WriteByte(I2C1, HTS221_ADDR_WRITE);
	I2C_WriteByte(I2C1, 0x20);									//CTRL_REG1
	I2C_WriteByte(I2C1, 0x80|0x04|0x01);
	I2C_Restart(I2C1);
	I2C_WriteByte(I2C1, HTS221_ADDR_WRITE);
	I2C_WriteByte(I2C1, 0x21);									//CTRL_REG2
	I2C_WriteByte(I2C1, 0x00|0x00|0x00);
	I2C_Restart(I2C1);
	I2C_WriteByte(I2C1, HTS221_ADDR_WRITE);
	I2C_WriteByte(I2C1, 0x22);									//CTRL_REG3
	I2C_WriteByte(I2C1, 0x00|0x00|0x04);
	I2C_Stop(I2C1);                             // P
  I2C_Disable(I2C1);
}

uint16_t get_Humidity(){
	uint8_t data[2];
	uint16_t humidity = 0;

	data[0] = readH(0x28);											//HUMIDITY_OUT_L
	data[1] = readH(0x29);											//HUMIDITY_OUT_H
	
	humidity  = data[0];
	humidity += data[1] << 8;
	if (humidity&0x8000) humidity = -(0x8000-(0x7fff&humidity));
	humidity = humidity/1024;
	
	set_Color(BLUE, value_Parameter(humidity, HUMIDITY), HUMIDITY);
    
	return humidity;
	}
