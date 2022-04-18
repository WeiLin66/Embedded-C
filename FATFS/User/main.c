#include "stm32f4xx.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_spi.h"
#include "ff.h"
#include "stdlib.h"

uint32_t id = 0;
uint8_t data[FF_MAX_SS] = {0};
BYTE 		work[FF_MAX_SS];
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
		/* disk format */
		#if 1
		res_flash = f_mkfs("1:", 0, work, FF_MAX_SS);
		
		if(res_flash == FR_OK){
			DEBUG_PRINT("[FATFS] [format success!]\n");
			res_flash = f_mount(NULL, "1:", 1);
			res_flash = f_mount(&fs, "1:", 1);
		}
		#endif
		LED_RED;
		DEBUG_PRINT("[FATFS] [No file system!]\n");
	}else if(res_flash != FR_OK){
		DEBUG_PRINT("[FATFS] [format fail!]\n");
	}else{
		LED_GREEN;
		DEBUG_PRINT("[FATFS] [already format!]\n");
	}
	
	/* FATFS writing test */
	DEBUG_PRINT("************FATFS file system writing test************\n");
	res_flash = f_open(&fnew, "1:FATFS.txt", FA_WRITE|FA_CREATE_ALWAYS);
	
	if(res_flash == FR_OK){
		DEBUG_PRINT("[FATFS] [start writing...]\n");
		res_flash = f_write(&fnew, test, sizeof(test), &bw);
		if(res_flash == FR_OK){
			DEBUG_PRINT("[FATFS] [finish writing!]\n");
		}
		f_close(&fnew);
	}else{
		DEBUG_PRINT("[FATFS] [error: %d]\n", res_flash);
	}
	
	/* FATFS reading test */
	DEBUG_PRINT("************FATFS file system reading test************\n");
	res_flash = f_open(&fnew, "1:FATFS.txt", FA_READ|FA_OPEN_ALWAYS);
	if(res_flash == FR_OK){
		res_flash = f_read(&fnew, data, FF_MAX_SS, &bw);
		if(res_flash == FR_OK){
			DEBUG_PRINT("[FATFS] [read file: FATFS.txt][content: %s]\n", data);
		}
		f_close(&fnew);
	}else{
		DEBUG_PRINT("[FATFS] [error: %d]\n", res_flash);
	}
	
	f_mount(NULL, "1:", 1);
	
	/* Infinite loop */
  while (1){

  }
}
