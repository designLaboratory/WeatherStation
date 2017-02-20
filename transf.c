#include "MKL46Z4.H"

#include "transf.h"
#include "WS2812.h"

uint8_t value_Parameter(int value, uint8_t parameter){
	uint8_t univ_Value;
	switch(parameter)
	{
		case TEMP:
		{
				if(value < 7) univ_Value = 15;
				else if((value <= 22) & (value >= 7)){
					int i = 0;
					for (i = 0; i < 16; i++){
						if(value == (7 + i)) univ_Value = (15 - i);
					}
				}
				else if((value <= 38) & (value >= 23)){
					int i = 0;
					for (i = 0; i < 16; i++){
						if(value == (23 + i)) univ_Value = i;
					}
				}
				else univ_Value = 15;
				break;
		}
		case PRESSURE:
				if(value < 976) univ_Value = 0;
				else if((value < 1040) & (value >= 976)){
					int i = 0;
					for(i = 0; i < 16; i++){
						if((value >= 976 + 4*i) & (value < 980 + 4*i)) univ_Value = i;
					}
				}
				else univ_Value = 15;
				break;
				
				//...
		case LIGHT:
				if(value < 0) univ_Value = 0;
				else if((value < 1008) & (value >= 0)){
					int i = 0;
					for(i = 0; i < 16; i++){
						if((value >= 0 + 63*i) & (value < 63 + 63*i)) univ_Value = i;
					}
				}
				else univ_Value = 15;
				break;

		case HUMIDITY:
				if(value < 32) univ_Value = 0;
				else if((value < 96) & (value >= 32)){
					int i = 0;
					for(i = 0; i < 16; i++){
						if((value >= 32 + 4*i) & (value < 36 + 4*i)) univ_Value = i;
					}
				}
				else univ_Value = 15;
				break;
				
		default:
				break;
	}
	return univ_Value;
}

void set_Color(uint8_t scale, uint8_t value, uint8_t parameter){
	int j = 0;
	if(parameter == TEMP) j = 0;
	else if(parameter == PRESSURE) j = 16;
	else if(parameter == LIGHT) j = 32;
	else j = 48;
	switch(scale)
	{
		case RED:
		{
			int i = 0;
			for(i = 0; i <= value; i++)
			{
				set_pixel_color(j, 255, 255-(16 * i), 0);
				j++;
			}
			for(i; i < 16; i++)
			{
				set_pixel_color(j, 0, 0, 0);
				j++;
			}
				start_DMA();
				break;
		}
		case BLUE:
		{
			int i = 0;
			for(i = 0; i <= value; i++)
			{
				set_pixel_color(j, 0, 255-(16 * i), 255);
				j++;
			}
			for(i; i < 16; i++)
			{
				set_pixel_color(j, 0, 0, 0);
				j++;
			}
				start_DMA();
				break;
		}
		case GREEN_BLUE:
		{
			int i = 0;
			for(i = 0; i <= value; i++)
			{
				set_pixel_color(j, 0, 255, 255-(16 * i));
				j++;
			}
			for(i; i < 16; i++)
			{
				set_pixel_color(j, 0, 0, 0);
				j++;
			}
				start_DMA();
				break;
		}
		case GREEN_YELLOW:
		{
			int i = 0;
			for(i = 0; i <= value; i++)
			{
				set_pixel_color(j, 16 * i, 255, 0);
				j++;
			}
			for(i; i < 16; i++)
			{
				set_pixel_color(j, 0, 0, 0);
				j++;
			}
				start_DMA();
				break;
		}
				
		default:
				break;
	}
}
