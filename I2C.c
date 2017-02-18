/** ****************************************************************************
 *   FRDM-KL46Z - Peripherals
 *    Module: I2C
 *    Version: 0.1
 *  ****************************************************************************
 */

#include "MKL46Z4.h"

#include "I2C.h"

void I2C_Disable(I2C_Type* i2c){
  i2c->C1 &= ~I2C_C1_IICEN_MASK;
}

void I2C_DisableInt(I2C_Type* i2c){
  i2c->C1 &= ~I2C_C1_IICIE_MASK;
}

void I2C_Enable(I2C_Type* i2c){
  i2c->C1 |= I2C_C1_IICEN_MASK;
}

void I2C_EnableInt(I2C_Type* i2c){
  i2c->C1 |= I2C_C1_IICIE_MASK;
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
