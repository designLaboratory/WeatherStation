#include "MKL46Z4.H"

#include "I2C.h"
#include "LPS331.h"
#include "transf.h"

int8_t read(uint8_t subadress){
	uint8_t status;
  
  I2C_Enable(I2C1);                           // Enable I2C1
  
  I2C_Start(I2C1);                            // S
  I2C_WriteByte(I2C1, LPS331_ADDR_WRITE);     // Slave address + W
  I2C_WriteByte(I2C1, subadress);             // Send command
  I2C_Restart(I2C1);                          // RS
  I2C_WriteByte(I2C1, LPS331_ADDR_READ);      // Slave address + R
  status = I2C_ReadByte(I2C1, I2C_NACK); 			// Get value
  I2C_Stop(I2C1);                             // P
  
  I2C_Disable(I2C1);                          // Disable I2C1
  
  return (int8_t) status;
}

void set_Resolution(uint8_t pressure_avg){
	
	I2C_Enable(I2C1);                           // Enable I2C1
  I2C_Start(I2C1);
	I2C_WriteByte(I2C1, LPS331_ADDR_WRITE);
	I2C_WriteByte(I2C1, 0x10);									// set RES_CONF register
	I2C_WriteByte(I2C1, (pressure_avg & 0x0f));
	I2C_Stop(I2C1);                             // P
  
  I2C_Disable(I2C1);
}
void set_DataRate(uint8_t datarate){
	uint8_t ctrlreg1;
	datarate &= 0x07;
	ctrlreg1 &= ~(0x07 << 4);
	ctrlreg1 |= datarate << 4;
	
	I2C_Enable(I2C1);                           // Enable I2C1
  I2C_Start(I2C1);
	I2C_WriteByte(I2C1, LPS331_ADDR_WRITE);
	I2C_WriteByte(I2C1, 0x20);
	I2C_WriteByte(I2C1, ctrlreg1);
	I2C_Stop(I2C1);                             // P
  
  I2C_Disable(I2C1);
}

void set_Active(uint8_t is_active){
	uint8_t ctrlreg1;
	if(is_active) {
        ctrlreg1 |= 0x80;
    } else {
        ctrlreg1 &= ~0x80;
    }
		
	I2C_Enable(I2C1);                           // Enable I2C1
  I2C_Start(I2C1);
	I2C_WriteByte(I2C1, LPS331_ADDR_WRITE);
	I2C_WriteByte(I2C1, 0x20);									// set CTRL_REG1 register
	I2C_WriteByte(I2C1, ctrlreg1);
	I2C_Stop(I2C1);                             // P
  
  I2C_Disable(I2C1);
}

int32_t get_Pressure(){
	uint8_t data[3];
	int32_t pressure = 0;
	
	data[0] = read(0x28);												//get PRESS_POUT_XL_REH
	data[1] = read(0x29);												//get PRESS_OUT_L
	data[2] = read(0x2a);												//get PRESS_OUT_H
	
	pressure  = data[0];
	pressure += data[1] << 8;
  pressure += data[2] << 16;
  pressure /= 4096;
	pressure += 28;
	
	set_Color(GREEN_BLUE, value_Parameter(pressure, PRESSURE), PRESSURE);
    
	return pressure;
}
