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
#include "bsp_spi.h"
//#include "usbdisk.h"	/* Example: Header file of existing USB MSD control module */
//#include "atadrive.h"	/* Example: Header file of existing ATA harddisk control module */
//#include "sdcard.h"		/* Example: Header file of existing MMC/SDC contorl module */

/* Definitions of physical drive number for each drive */
#define ATA					0				/* Example: Map ATA harddisk to physical drive 0 */
#define SPI_FLASH		1				/* SPI Flash Interface */



/*-----------------------------------------------------------------------*/
/* 返回當前時間戳                                                      */
/*-----------------------------------------------------------------------*/

__weak DWORD get_fattime(void) {
	return	  ((DWORD)(2022 - 1980) << 25)	/* Year 2022 */
			| ((DWORD)1 << 21)				/* Month 1 */
			| ((DWORD)1 << 16)				/* Mday 1 */
			| ((DWORD)0 << 11)				/* Hour 0 */
			| ((DWORD)0 << 5)				  /* Min 0 */
			| ((DWORD)0 >> 1);				/* Sec 0 */
}



/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;

	switch (pdrv) {
		
	case ATA:	/* SD CARD */
		break;

	case SPI_FLASH :
		if(Flash_Read_WRITE_JEDEC_ID() == FLASH_ID){ /* check if Flash is alive */
			stat &= ~STA_NOINIT;
		}else{
			stat = STA_NOINIT;
		}

		break;
		
	default:
		stat = RES_PARERR;		
	}
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	uint16_t t;
	DSTATUS stat = STA_NOINIT;

	switch (pdrv) {
	case ATA:	/* SD CARD */
		break;

	case SPI_FLASH :
		SPI_FLASH_Init();
		/* delay for a short period of time */
		t = 500;
		while(t--);
		/* Power up */
		Flash_Release_Power_Down();
		stat = disk_status(SPI_FLASH);
		break;
	
	default:
		stat = RES_PARERR;
	}
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;

	switch (pdrv) {
	case ATA:	/* SD CARD */
		break;
	
	case SPI_FLASH :
		sector += 4096; /* Flash offset */
		FLASH_Read_MutiData(buff, sector << 12, count << 12); /* re-caculate address & count */
		res = RES_OK;
	break;
	
	default:
		res = RES_PARERR;	
	}

	return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	uint32_t addr;
	DRESULT res = RES_PARERR;

	switch (pdrv) {

	case ATA:	/* SD CARD */
		break;

	case SPI_FLASH :
		sector += 4096; /* Flash offset */
		addr = sector << 12;
		Flash_Erase_Sector(addr);
		FLASH_Write_MutiData((uint8_t*)buff, addr, count << 12);
		res = RES_OK;
	break;
	
	default:
	res = RES_PARERR;
	
	}

	return res;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_PARERR;

	switch (pdrv) {

	case ATA:	/* SD CARD */
	break;

	case SPI_FLASH :
		
			switch (cmd) {
        /* 扇區數量：4096*4096/1024/1024=16(MB) */
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
			res = RES_OK;
		break;
			
	default:
		res = RES_PARERR;
	}

	return res;
}
#endif
