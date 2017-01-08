/** ****************************************************************************
 *   FRDM-KL46Z - Peripherals
 *    Module: GPIO
 *    Author: Jan Szemiet
 *    Version: 0.1
 *  ****************************************************************************
 */

#include "MKL46Z4.h"

#include "GPIO.h"

void GPIO_ChangePortBitDirection(GPIO_Type* port, uint8_t bit, GPIO_IO_Dir dir){
  if(dir == INPUT){
    port->PDDR &= ~((uint32_t)1<<bit);
  }
  else if(dir == OUTPUT){
    port->PDDR |= ((uint32_t)1<<bit);
  }
}

void GPIO_ClearPortBit(GPIO_Type* port, uint8_t bit){
  port->PCOR = (uint32_t)1<<bit;
}

uint8_t GPIO_ReadPortBit(GPIO_Type* port, uint8_t bit){
  return (uint8_t)(((port->PDIR)>>bit)&0x1);
}

void GPIO_SetPortBit(GPIO_Type* port, uint8_t bit){
  port->PSOR = (uint32_t)1<<bit;
}

void GPIO_TogglePortBit(GPIO_Type* port, uint8_t bit){
  port->PTOR = (uint32_t)1<<bit;
}

void GPIO_WritePortBit(GPIO_Type* port, uint8_t bit, GPIO_OUT_State state){
  port->PDOR = (port->PDOR & ~((uint32_t)1<<bit)) | ((uint32_t)state<<bit);
}
