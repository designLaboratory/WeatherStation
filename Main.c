#include <stdio.h>
#include "GPIO.h"
#include "I2C.h"
#include "LCD.h"
#include "Main.h"
#include "TC74.h"
#include "WS2812.h"

volatile uint32_t msTicks;

static void LCD_Config(void){
  vfnLCD_Init();
  vfnLCD_Home();
}

void SLCD_HideColon(void){
  LCD->WF8B[11] &= 0xFE;
}

void SLCD_HideDecimalPointAt(uint8_t p){
  switch(p){
    case 1:
      LCD->WF8B[17] &= 0xFE;
      break;
    case 2:
      LCD->WF8B[8]  &= 0xFE;
      break;
    case 3:
      LCD->WF8B[38] &= 0xFE;
      break;
    default:
      break;
  }
}

void SLCD_ShowColon(void){
  LCD->WF8B[11] |= 0x01;
}

void SLCD_ShowDecimalPointAt(uint8_t p){
  switch(p){
    case 1:
      LCD->WF8B[17] |= 0x01;
      break;
    case 2:
      LCD->WF8B[8]  |= 0x01;
      break;
    case 3:
      LCD->WF8B[38] |= 0x01;
      break;
    default:
      break;
  }
}

void SLCD_ShowMinus(uint8_t p){
  switch(p){
    case 1:
      LCD->WF8B[37] |= 0x04;
      break;
    case 2:
      LCD->WF8B[7]  |= 0x04;
      break;
    case 3:
      LCD->WF8B[53] |= 0x04;
      break;
    case 4:
      LCD->WF8B[10] |= 0x04;
      break;
    default:
      break;
  }
}

void SLCD_ShowText(char* s){
  uint8_t c = 0;
  
  vfnLCD_All_Segments_OFF();
  vfnLCD_Home();
  
  while(c < 4 && *s != '\0'){
    if(*s != '.'){
      if(*s == '-'){
        vfnLCD_Write_Char(' ');
        SLCD_ShowMinus(c+1);
      }
      else{
        vfnLCD_Write_Char(*s);
      }
      ++c;
    }
    else if(*s == '.'){
      if(*(s-1) != '.' && c > 0){
        SLCD_ShowDecimalPointAt(c);
      }
      else{
        ++c;
        vfnLCD_Write_Char(' ');
        SLCD_ShowDecimalPointAt(c);
      }
    }
    ++s;
  }
}

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

int main(void){
  int8_t max_temp;
  int8_t min_temp;
  int8_t temp;
  char line[10];
  
  Clock_Config();
  GPIO_Config();
  I2C_Init();
  LCD_Config();
	initialize_pixel_tables();       								//initialize and clear tables			
  dma_data_init();																//set dma_data.all_ones table each element as 255																																				
  clock_init();																		//send clock to port and other peripherials
  dma_initial();																	//initialize DMA
  io_init();																			//initialize output pin
  tpm_init();
  
  temp = ReadTemperature();
  max_temp = temp;
  min_temp = temp;
  
  while(1){
    // Read current temperature
    temp = ReadTemperature();
    
    // Record max & min temperature
    if(temp < min_temp)
      min_temp = temp;
    else if(temp > max_temp)
      max_temp = temp;
    
    if(GPIO_ReadPortBit(SW_1) == 0){
      // Turn on green LED
      GPIO_ClearPortBit(LED_GREEN);
      
      // Show the lowest temperature
      sprintf(line, "%dC", min_temp);
      SLCD_ShowText(line);
    }
    else if(GPIO_ReadPortBit(SW_2) == 0){
      // Turn on red LED
      GPIO_ClearPortBit(LED_RED);
      
      // Show the highest temperature
      sprintf(line, "%dC", max_temp);
      SLCD_ShowText(line);
    }
    else{
      // Turn off green LED
      GPIO_SetPortBit(LED_GREEN);
      // Turn off red LED
      GPIO_SetPortBit(LED_RED);
      
      // Show current temperature
      sprintf(line, "%dC", temp);
      SLCD_ShowText(line);
    }
   																			//initialize clock
    if (temp < 27){
        set_pixel_color(1, 0, 100, 0);					//set elements in pixel tables for second diode
        set_pixel_color(0, 0, 100, 0);
		}
		else{
				set_pixel_color(1, 100, 0, 0);					//set elements in pixel tables for second diode
        set_pixel_color(0, 100, 0, 0);
		}//set elements in pixel tables for first diode
				start_DMA();
    Delay_ms(100);
  }
}

static void Clock_Config(void){
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/1000);
  
  SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;        // Enable clock for I2C1
  SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK        // Enable clock for Port A
              | SIM_SCGC5_PORTB_MASK        // Enable clock for Port B
              | SIM_SCGC5_PORTC_MASK        // Enable clock for Port C
              | SIM_SCGC5_PORTD_MASK        // Enable clock for Port D
              | SIM_SCGC5_PORTE_MASK;       // Enable clock for Port E
}

static void GPIO_Config(void){
  PORTC->PCR[3]  = PORT_PCR_MUX(1)          // Alternative function: 1 (GPIO)
                 | PORT_PCR_PE_MASK         // Enable pullup/pulldown resistor
                 | PORT_PCR_PS_MASK;        // Select pullup resistor
  PORTC->PCR[12] = PORT_PCR_MUX(1)          // Alternative function: 1 (GPIO)
                 | PORT_PCR_PE_MASK         // Enable pullup/pulldown resistor
                 | PORT_PCR_PS_MASK;        // Select pullup resistor
  PORTD->PCR[5]  = PORT_PCR_MUX(1);         // Alternative function: 1 (GPIO)
  PORTE->PCR[29] = PORT_PCR_MUX(1);         // Alternative function: 1 (GPIO)
  
  PORTE->PCR[0]  = PORT_PCR_MUX(6);         // Alternative function: 6 (I2C1_SDA)
  PORTE->PCR[1]  = PORT_PCR_MUX(6);         // Alternative function: 6 (I2C1_SCL)
  
  GPIO_ChangePortBitDirection(LED_GREEN, OUTPUT);
  GPIO_ChangePortBitDirection(LED_RED, OUTPUT);
  GPIO_ChangePortBitDirection(SW_1, INPUT);
  GPIO_ChangePortBitDirection(SW_2, INPUT);
  
  GPIO_SetPortBit(LED_GREEN);               // Turn off green LED
  GPIO_SetPortBit(LED_RED);                 // Turn off red LED
}

static void I2C_Init(void){
  // Set baudrate to 400 kb/s
  I2C1->F   = I2C_F_MULT(2)
            | I2C_F_ICR(0x5);
}
