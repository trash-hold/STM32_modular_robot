#include "DEV_Config.h"
#include "stm32f4xx_hal_spi.h"
#include "usart.h"
#include "spi.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

/*********************************************


*********************************************/	
 uint8_t SPI_Write_Byte(uint8_t value)                                    
{    
#if 1
	HAL_SPI_Transmit_IT(&hspi3, &value, 1);
	
//	 unsigned char Read_Buf;
//	HAL_SPI_TransmitReceive(&hspi3, &value, &Read_Buf,1, 500);
//	return Read_Buf;
	
#elif 0
	char i;
	for(i = 0;i < 8;i++){		
		SPI_SCK_0;
		if(value & 0X80)
			SPI_MOSI_1;
		else
			SPI_MOSI_0;
		Driver_Delay_us(10);
		SPI_SCK_1;
		Driver_Delay_us(10);
		value = (value << 1); 	
	}
#else
	
    __HAL_SPI_ENABLE(&hspi3);
	SPI3->CR2 |= (1)<<12;
	
	while((SPI3->SR & (1<<1)) == 0)
		;
        
        *((__IO uint8_t *)(&SPI3->DR)) = value;
        
        while(SPI3->SR & (1<<7)) ; //Wait for not busy

        while((SPI3->SR & (1<<0)) == 0) ; // Wait for the receiving area to be empty
        
        return *((__IO uint8_t *)(&SPI3->DR));

#endif
}

/*********************************************


*********************************************/	

uint8_t SPI_Read_Byte(uint8_t value)                                    
{
	return SPI_Write_Byte(value);
}


/*********************************************


*********************************************/	
void Driver_Delay_ms(uint32_t xms)	
{	
	HAL_Delay(xms);
}

void Driver_Delay_us(uint32_t xus)
{
  for(int j=xus;j > 0;j--);    
}
