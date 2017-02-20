#include "MKL46Z4.h"

#include "SysTick.h"

volatile uint32_t msTicks;

/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void){
  msTicks++;
}

/*------------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *------------------------------------------------------------------------------*/
void Delay_ms(uint32_t dlyTicks){
  uint32_t curTicks;
  
  curTicks = msTicks;                       // Zachowaj aktualna wartosc zmiennej
  while ((msTicks - curTicks) < dlyTicks);  // Czekaj dopoki roznica aktualnej i
                                            // zachowanej wartosci jest mniejsza od
                                            // wartosci zmiennej dlyTicks
}
