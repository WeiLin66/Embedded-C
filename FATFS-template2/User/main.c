#include "stm32f4xx.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_spi.h"
#include "ff.h"
#include "stdlib.h"

uint32_t id = 0;
uint8_t data[_MAX_SS] = {0};
BYTE 		work[_MAX_SS];
uint8_t test[] = "this is a test string!";

FRESULT res_flash;
FATFS fs;
FIL fnew;
UINT bw;
	
int main(void){	
  /* LED Init() */  
	GPIO_flash_Init();
	LED_BLUE;
	
	/* USART Init() */
	USART_CFG();
	DEBUG_PRINT("************FATFS file system test************\n");
	
	/* mount file system at external flash */
	res_flash = f_mount(&fs, "1:", 1);

	DEBUG_PRINT("[FATFS] [res_flash: %d]\n", res_flash);

	if(res_flash == FR_NO_FILESYSTEM){
		DEBUG_PRINT("[FATFS] [No file system!]\n");

		/* disk format */
		res_flash = f_mkfs("1:", 0, 0);
		
		if(res_flash == FR_OK){
			DEBUG_PRINT("[FATFS] [format success]\n");

      /* 格式化後，先取消掛載 */
			res_flash = f_mount(NULL, "1:", 1);			
      /* 重新掛載	*/			
			res_flash = f_mount(&fs,"1:",1);
		}else if(res_flash != FR_OK){
			goto err;
		}
	}else{
			DEBUG_PRINT("[FATFS] [already format!]\n");
	}
	
	DEBUG_PRINT("[FATFS] [res_flash: %d]\n", res_flash);
	LED_GREEN;
	
	f_mount(NULL, "1:", 1);				
	
	/* Infinite loop */
  while (1){

  }
	
	err:
		DEBUG_PRINT("[FATFS] [fail!]\n");
		LED_RED;
}
