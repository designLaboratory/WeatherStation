#include "MKL46Z4.H"

#include "I2C.h"
#include "TSL25721.h"

int gain_val = 0;

int8_t readL(uint8_t subadress){
	uint8_t status;
  
  I2C_Enable(I2C1);                           // Enable I2C1
  
  I2C_Start(I2C1);                            // S
  I2C_WriteByte(I2C1, TSL25721_ADDR_WRITE);   // Slave address + W
  I2C_WriteByte(I2C1, 0x80 | subadress);      // Send command
  I2C_Restart(I2C1);                          // RS
  I2C_WriteByte(I2C1, TSL25721_ADDR_READ);     // Slave address + R
  status = I2C_ReadByte(I2C1, I2C_NACK); 			
  I2C_Stop(I2C1);                             // P
  
  I2C_Disable(I2C1);                          // Disable I2C1
  
  return (int8_t) status;
}

void set_ActiveL(uint8_t gain){
	I2C_Enable(I2C1);                           // Enable I2C1
  I2C_Start(I2C1);
	I2C_WriteByte(I2C1, TSL25721_ADDR_WRITE);
	I2C_WriteByte(I2C1, 0x80 | 0x0F);
	I2C_WriteByte(I2C1, gain);
	I2C_Restart(I2C1);
	I2C_WriteByte(I2C1, TSL25721_ADDR_WRITE);
	I2C_WriteByte(I2C1, 0x80 | 0x01);
	I2C_WriteByte(I2C1, 0xED);
	I2C_Restart(I2C1);
	I2C_WriteByte(I2C1, TSL25721_ADDR_WRITE);
	I2C_WriteByte(I2C1, 0x80 | 0x00);
	I2C_WriteByte(I2C1, 0x03);
	I2C_Stop(I2C1);                             // P
  I2C_Disable(I2C1);
	
	if(GAIN_DIVIDE_6){
		I2C_Enable(I2C1);                           // Enable I2C1
		I2C_Start(I2C1);
		I2C_WriteByte(I2C1, TSL25721_ADDR_WRITE);
		I2C_WriteByte(I2C1, 0x80 | 0x0D);
		I2C_WriteByte(I2C1, 0x04);
		I2C_Stop(I2C1);                             // P
		I2C_Disable(I2C1);
	}//scale gain by 0.16
  if(gain==GAIN_1X)gain_val=1;
  else if(gain==GAIN_8X)gain_val=8;
  else if(gain==GAIN_16X)gain_val=16;
  else if(gain==GAIN_120X)gain_val=120;
}

float get_Light(){
	uint8_t data[4];
	int c0, c1;
	
	float lux1,lux2,cpl;
	
	I2C_Enable(I2C1);                           // Enable I2C1
  I2C_Start(I2C1);
	I2C_WriteByte(I2C1, TSL25721_ADDR_WRITE);
	I2C_WriteByte(I2C1, 0xA0 | 0x14);
	I2C_Stop(I2C1);                             // P
  I2C_Disable(I2C1);
	//multiread(0x28, data, 3);
	data[0] = readL(0x14);
	data[1] = readL(0x15);
	data[2] = readL(0x16);
	data[3] = readL(0x17);
	//data[2] = read(0x2a);
	
	c0  = data[0];
	c0 += data[1] << 8;
	c1 = data[2];
	c1 += data[3] << 8;

	cpl = 51.87 * (float)gain_val / 60.0;
  if(GAIN_DIVIDE_6) cpl/=6.0;
  lux1 = ((float)c0 - (1.87 * (float)c1)) / cpl;
  lux2 = ((0.63 * (float)c0) - (float)c1) / cpl;
  if(lux1 > lux2) cpl = lux1;
	else cpl = lux2;
  if(cpl > 0.0) return cpl;
	else return 0.0;
}
