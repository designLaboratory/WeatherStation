/** ****************************************************************************
 *   FRDM-KL46Z - Peripherals
 *    Module: GPIO
 *    Author: Jan Szemiet
 *    Version: 0.1
 *  ****************************************************************************
 */

#include "MKL46Z4.h"

typedef enum{
  INPUT  = 0,
  OUTPUT = 1,
} GPIO_IO_Dir;

typedef enum{
  LOW  = 0,
  HIGH = 1,
} GPIO_OUT_State;

void    GPIO_ChangePortBitDirection(GPIO_Type* port, uint8_t bit, GPIO_IO_Dir dir);
void    GPIO_ClearPortBit(GPIO_Type* port, uint8_t bit);
uint8_t GPIO_ReadPortBit(GPIO_Type* port, uint8_t bit);
void    GPIO_SetPortBit(GPIO_Type* port, uint8_t bit);
void    GPIO_TogglePortBit(GPIO_Type* port, uint8_t bit);
void    GPIO_WritePortBit(GPIO_Type* port, uint8_t bit, GPIO_OUT_State state);
