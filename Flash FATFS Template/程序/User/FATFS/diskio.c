/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "ff.h"
#include "flash/bsp_spi_flash.h"

/* 為每個設備定義一個物理編號 */
#define ATA			    0     // 預留SD卡使用
#define SPI_FLASH		1     // 外部SPI Flash

/*-----------------------------------------------------------------------*/
/* 獲取設備狀態                                                          */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (
	BYTE pdrv		/* 物理編號 */
)
{

	DSTATUS status = STA_NOINIT;
	
	switch (pdrv) {
		case ATA:	/* SD CARD */
			break;
    
		case SPI_FLASH:      
      /* SPI Flash狀態檢測：讀取SPI Flash 設備ID */
      if(sFLASH_ID2 == SPI_FLASH_ReadID())
      {
        /* 設備ID讀取結果正確 */
        status &= ~STA_NOINIT;
      }
      else
      {
        /* 設備ID讀取結果錯誤 */
        status = STA_NOINIT;;
      }
			break;

		default:
			status = STA_NOINIT;
	}
	return status;
}

/*-----------------------------------------------------------------------*/
/* 設備初始化                                                            */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize (
	BYTE pdrv				/* 物理編號 */
)
{
  uint16_t i;
	DSTATUS status = STA_NOINIT;	
	switch (pdrv) {
		case ATA:	         /* SD CARD */
			break;
    
		case SPI_FLASH:    /* SPI Flash */ 
      /* 初始化SPI Flash */
			SPI_FLASH_Init();
      /* 延時一小段時間 */
      i=500;
	    while(--i);	
      /* 喚醒SPI Flash */
	    SPI_Flash_WAKEUP();
      /* 獲取SPI Flash芯片狀態 */
      status=disk_status(SPI_FLASH);
			break;
      
		default:
			status = STA_NOINIT;
	}
	return status;
}


/*-----------------------------------------------------------------------*/
/* 讀扇區：讀取扇區內容到指定存儲區                                              */
/*-----------------------------------------------------------------------*/
DRESULT disk_read (
	BYTE pdrv,		/* 設備物理編號(0..) */
	BYTE *buff,		/* 數據緩存區 */
	DWORD sector,	/* 扇區首地址 */
	UINT count		/* 扇區個數(1..128) */
)
{
	DRESULT status = RES_PARERR;
	switch (pdrv) {
		case ATA:	/* SD CARD */
			break;
    
		case SPI_FLASH:
      /* 扇區偏移16MB，外部Flash文件系統空間放在SPI Flash後面16MB空間 */
      sector+=4096;      
      SPI_FLASH_BufferRead(buff, sector <<12, count<<12);
      status = RES_OK;
		break;
    
		default:
			status = RES_PARERR;
	}
	return status;
}

/*-----------------------------------------------------------------------*/
/* 寫扇區：見數據寫入指定扇區空間上                                      */
/*-----------------------------------------------------------------------*/
#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			  /* 設備物理編號(0..) */
	const BYTE *buff,	/* 欲寫入數據的緩存區 */
	DWORD sector,		  /* 扇區首地址 */
	UINT count			  /* 扇區個數(1..128) */
)
{
  uint32_t write_addr; 
	DRESULT status = RES_PARERR;
	if (!count) {
		return RES_PARERR;		/* Check parameter */
	}

	switch (pdrv) {
		case ATA:	/* SD CARD */      
		break;

		case SPI_FLASH:
      /* 扇區偏移16MB，外部Flash文件系統空間放在SPI Flash後面16MB空間 */
			sector+=4096;
      write_addr = sector<<12;    
      SPI_FLASH_SectorErase(write_addr);
      SPI_FLASH_BufferWrite((u8 *)buff,write_addr,count<<12);
      status = RES_OK;
		break;
    
		default:
			status = RES_PARERR;
	}
	return status;
}
#endif


/*-----------------------------------------------------------------------*/
/* 其他控制                                                              */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* 物理編號 */
	BYTE cmd,		  /* 控制指令 */
	void *buff		/* 寫入或者讀取數據地址指針 */
)
{
	DRESULT status = RES_PARERR;
	switch (pdrv) {
		case ATA:	/* SD CARD */
			break;
    
		case SPI_FLASH:
			switch (cmd) {
        /* 扇區數量：4096*4096/1024/1024=10(MB) */
        case GET_SECTOR_COUNT:
          *(DWORD * )buff = 4096;		
        break;
        /* 扇區大小  */
        case GET_SECTOR_SIZE :
          *(WORD * )buff = 4096;
        break;
        /* 同時擦除扇區個數 */
        case GET_BLOCK_SIZE :
          *(DWORD * )buff = 1;
        break;        
      }
      status = RES_OK;
		break;
    
		default:
			status = RES_PARERR;
	}
	return status;
}
#endif

__weak DWORD get_fattime(void) {
	/* 返回當前時間戳 */
	return	  ((DWORD)(2015 - 1980) << 25)	/* Year 2015 */
			| ((DWORD)1 << 21)				/* Month 1 */
			| ((DWORD)1 << 16)				/* Mday 1 */
			| ((DWORD)0 << 11)				/* Hour 0 */
			| ((DWORD)0 << 5)				  /* Min 0 */
			| ((DWORD)0 >> 1);				/* Sec 0 */
}



