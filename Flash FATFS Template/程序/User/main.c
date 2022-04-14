/**
  ******************************************************************************
  *                              頭文件
  ******************************************************************************
  */
#include "stm32f4xx.h"
#include "led/bsp_led.h" 
#include "usart/bsp_debug_usart.h"
#include "ff.h"
/**
  ******************************************************************************
  *                              定義變量
  ******************************************************************************
  */
FATFS fs;													/* FatFs文件系統對象 */
FIL fnew;													/* 文件對象 */
FRESULT res_flash;                /* 文件操作結果 */
UINT fnum;            					  /* 文件成功讀寫數量 */
BYTE ReadBuffer[1024]={0};        /* 讀緩沖區 */
BYTE WriteBuffer[] =              /* 寫緩沖區*/
"Welcome to the STM32 F429 development board. Today is a good day, new file system test file\r\n";  

/**
  ******************************************************************************
  *                                任務函數
  ******************************************************************************
  */
/**
  * @brief  主函數
  * @param  無
  * @retval 無
  */
int main(void)
{
	/* 初始化LED */
	LED_GPIO_Config();	
	LED_BLUE;
	
	/* 初始化調試串口，一般為串口1 */
	Debug_USART_Config();	
  printf("****** This is an SPI FLASH file system experiment ******\r\n");
  
	//在外部SPI Flash掛載文件系統，文件系統掛載時會對SPI設備初始化
	res_flash = f_mount(&fs,"1:",1);
	
/*----------------------- 格式化測試 ---------------------------*/  
	/* 如果沒有文件系統就格式化創建創建文件系統 */
	if(res_flash == FR_NO_FILESYSTEM)
	{
		printf("》FLASH does not have a file system yet and will be formatted soon...\r\n");
    /* 格式化 */
		res_flash=f_mkfs("1:",0,0);							
		
		if(res_flash == FR_OK)
		{
			printf("》FLASH has successfully formatted the file system.\r\n");
      /* 格式化後，先取消掛載 */
			res_flash = f_mount(NULL,"1:",1);			
      /* 重新掛載	*/			
			res_flash = f_mount(&fs,"1:",1);
		}
		else
		{
			LED_RED;
			printf("《《Formatting Failure》》\r\n");
			while(1);
		}
	}
  else if(res_flash!=FR_OK)
  {
    printf("！！External Flash mount file system failure.(%d)\r\n",res_flash);
    printf("！！Possible cause: SPI Flash initialization unsuccessful.\r\n");
		while(1);
  }
  else
  {
    printf("》The file system is successfully mounted and ready for read/write test.\r\n");
  }
  
/*----------------------- 文件系統測試：寫測試 -----------------------------*/
	/* 打開文件，如果文件不存在則創建它 */
	printf("\r\n****** File writing test will be conducted soon... ******\r\n");	
	res_flash = f_open(&fnew, "1:FatFs read and write test files.txt",FA_CREATE_ALWAYS | FA_WRITE );
	if ( res_flash == FR_OK )
	{
		printf("》Open/create FatFs read/write test file.txt file successfully, writing data to a file.\r\n");
    /* 將指定存儲區內容寫入到文件內 */
		res_flash=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
    if(res_flash==FR_OK)
    {
      printf("》File write success, write byte data: %d\n",fnum);
      printf("》The data written to the file is: \r\n%s\r\n",WriteBuffer);
    }
    else
    {
      printf("！！File write failure: (%d)\n",res_flash);
    }    
		/* 不再讀寫，關閉文件 */
    f_close(&fnew);
	}
	else
	{	
		LED_RED;
		printf("！！Failed to open/create file.\r\n");
	}
	
/*------------------- 文件系統測試：讀測試 ------------------------------------*/
	printf("****** The file read test will be conducted soon... ******\r\n");
	res_flash = f_open(&fnew, "1:FatFs read and write test files.txt", FA_OPEN_EXISTING | FA_READ); 	 
	if(res_flash == FR_OK)
	{
		LED_GREEN;
		printf("》Open file successfully.\r\n");
		res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(res_flash==FR_OK)
    {
      printf("》File read successfully, read byte dat: %d\r\n",fnum);
      printf("》The file data read is: \r\n%s \r\n", ReadBuffer);	
    }
    else
    {
      printf("！！File read failure: (%d)\n",res_flash);
    }		
	}
	else
	{
		LED_RED;
		printf("！！Failed to open the file.\r\n");
	}
	/* 不再讀寫，關閉文件 */
	f_close(&fnew);	
  
	/* 不再使用文件系統，取消掛載文件系統 */
	f_mount(NULL,"1:",1);
  
  /* 操作完成，停機 */
	while(1)
	{
	}
}

