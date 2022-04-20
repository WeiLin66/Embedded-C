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
	res_flash = f_mount(&fs, "0:", 1);

	DEBUG_PRINT("[FATFS] [res_flash: %d]\n", res_flash);

	if(res_flash == FR_NO_FILESYSTEM){
		DEBUG_PRINT("[FATFS] [No file system!]\n");

		/* disk format */
		res_flash = f_mkfs("0:", 0, work, FF_MAX_SS);
		
//		DEBUG_PRINT("[FATFS] [res_flash: %d]\n", res_flash);
		
		if(res_flash == FR_OK){
      /* 格式化後，先取消掛載 */
			res_flash = f_unmount("0:");			
      /* 重新掛載	*/			
			res_flash = f_mount(&fs,"0:",1);
		}else{
//			goto err;
		}
		
		DEBUG_PRINT("[FATFS] [format success!]\n");

	}else if(res_flash != FR_OK){
//		goto err;
	}else{
		DEBUG_PRINT("[FATFS] [already format!]\n");
	}
	
	LED_GREEN;
	
	/* FATFS writing test */
	DEBUG_PRINT("************FATFS file system writing test************\n");
	res_flash = f_open(&fnew, "0:FATFS.txt", FA_WRITE|FA_CREATE_ALWAYS);
	
	DEBUG_PRINT("[FATFS] [res_flash: %d]\n", res_flash);
	
	if(res_flash == FR_OK){
		DEBUG_PRINT("[FATFS] [start writing...]\n");
		res_flash = f_write(&fnew, test, sizeof(test), &bw);
		if(res_flash == FR_OK){
			DEBUG_PRINT("[FATFS] [finish writing!]\n");
		}
		f_close(&fnew);
	}else{
//		goto err;
	}
	
	/* FATFS reading test */
	DEBUG_PRINT("************FATFS file system reading test************\n");
	res_flash = f_open(&fnew, "0:FATFS.txt", FA_READ|FA_OPEN_ALWAYS);
	if(res_flash == FR_OK){
		res_flash = f_read(&fnew, data, FF_MAX_SS, &bw);
		if(res_flash == FR_OK){
			DEBUG_PRINT("[FATFS] [read file: FATFS.txt][content: %s]\n", data);
		}
	}else{
//		goto err;
	}

	f_close(&fnew);
	f_unmount("0:");			
	
	/* Infinite loop */
  while (1){

  }
	
//	err:
//		DEBUG_PRINT("[FATFS] [fail!]\n");
//		LED_RED;

}
