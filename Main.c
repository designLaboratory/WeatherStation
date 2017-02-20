#include <stdio.h>
#include "MKL46Z4.h"
#include "I2C.h"
#include "LCD.h"
#include "STLM75.h"
#include "WS2812.h"
#include "LPS331.h"
#include "HTS221.h"
#include "TSL25721.h"
#include "transf.h"
#include "SysTick.h"

int main(void){
  int8_t temp;
	int32_t pressure;
	uint16_t humidity;
	float light;
	int16_t light1;
  char line[10];
  
  
	clock_Config();
  GPIO_Config();
  I2C_Init();
  LCD_Config();
	//////////////////RGB config/////////////
	initialize_pixel_tables();       								//initialize and clear tables			
  dma_data_init();																//set dma_data.all_ones table each element as 255																																				
  clock_init();																		//send clock to port and other peripherials
  dma_initial();																	//initialize DMA
  io_init();																			//initialize output pin
  tpm_init();
	diodes_default();
	////////////////////////////////////////
	
	//initialize pressure
	set_Resolution(LPS331_I2C_PRESSURE_AVG_512);
	set_DataRate(LPS331_I2C_DATARATE_7HZ);
	set_Active(1);
	
	//initialize humidity
	set_ResolutionH();
	set_ActiveH();
	
	//initialize light
	set_ActiveL(GAIN_1X);
  
  while(1){
    // Read current temperature
    temp = read_Temperature();
		pressure = get_Pressure();
		humidity = get_Humidity();
		light = get_Light();
		light1 = light;
		set_Color(GREEN_YELLOW, value_Parameter(light1, LIGHT), LIGHT);
		sprintf(line, "%d", temp);
    SLCD_ShowText(line);
		Delay_ms(1000);
		sprintf(line, "%d", pressure);
    SLCD_ShowText(line);
		Delay_ms(1000);
		sprintf(line, "%d", humidity);
    SLCD_ShowText(line);
		Delay_ms(1000);
		sprintf(line, "%d", light1);
    SLCD_ShowText(line);
		Delay_ms(1000);
  }
}
