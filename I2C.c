#include "MKL46Z4.h"

#include "I2C.h"

void I2C_Disable(I2C_Type* i2c){
  i2c->C1 &= ~I2C_C1_IICEN_MASK;
}

void I2C_Enable(I2C_Type* i2c){
  i2c->C1 |= I2C_C1_IICEN_MASK;
}

uint8_t I2C_ReadByte(I2C_Type* i2c, uint8_t ack){
  // Select receive mode
  i2c->C1 &= ~I2C_C1_TX_MASK;
  
  // Prepare ACK/NACK bit if FACK == 0
  if((i2c->SMB & I2C_SMB_FACK_MASK) == 0)
    i2c->C1 = (ack == I2C_NACK) ? i2c->C1 | I2C_C1_TXAK_MASK : i2c->C1 & ~I2C_C1_TXAK_MASK;
  
  // Clear IICIF flag
  i2c->S |= I2C_S_IICIF_MASK;
  
  // Initiate data transfer
  (void)i2c->D;
  // Wait for transfer completion
  while((i2c->S & I2C_S_IICIF_MASK) == 0);
  
  // Send ACK/NACK bit if FACK == 1
  if((i2c->SMB & I2C_SMB_FACK_MASK) != 0)
    i2c->C1 = (ack == I2C_NACK) ? i2c->C1 | I2C_C1_TXAK_MASK : i2c->C1 & ~I2C_C1_TXAK_MASK;
  
  // Select transmit mode - to omit next read of data byte
  i2c->C1 |= I2C_C1_TX_MASK;
  
  // Return received data
  return i2c->D;
}

void I2C_Restart(I2C_Type* i2c){
  i2c->C1 |= I2C_C1_RSTA_MASK;
}

void I2C_Start(I2C_Type* i2c){
  i2c->C1 |= I2C_C1_MST_MASK;
}

void I2C_Stop(I2C_Type * i2c){
  // Clear STOPF flag
  i2c->FLT |= I2C_FLT_STOPF_MASK;
  
  // Write STOP bit
  i2c->C1 &= ~I2C_C1_MST_MASK;
  // Wait until STOP bit is send
  while((i2c->FLT & I2C_FLT_STOPF_MASK) == 0){
    i2c->C1 &= ~I2C_C1_MST_MASK;
  }
}

uint8_t I2C_WriteByte(I2C_Type* i2c, uint8_t data){
  // Select transmit mode
  i2c->C1 |= I2C_C1_TX_MASK;
  
  // Clear IICIF flag
  i2c->S |= I2C_S_IICIF_MASK;
  
  // Initiate data transfer
  i2c->D = data;
  // Wait for transfer completion
  while((i2c->S & I2C_S_IICIF_MASK) == 0);
  
  // Return received ACK bit
  return ((i2c->S & I2C_S_RXAK_MASK) == I2C_S_RXAK_MASK ? I2C_NACK : I2C_ACK);
}

void I2C_Init(void){
  // Set baudrate to 400 kb/s
  I2C1->F   = I2C_F_MULT(2)
            | I2C_F_ICR(0x5);
}

void GPIO_Config(void){
  PORTE->PCR[0]  = PORT_PCR_MUX(6);         // Alternative function: 6 (I2C1_SDA)
  PORTE->PCR[1]  = PORT_PCR_MUX(6);         // Alternative function: 6 (I2C1_SCL)
}

void clock_Config(void){
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/1000);
  
  SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;        // Enable clock for I2C1
  SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;       // Enable clock for Port E
}
