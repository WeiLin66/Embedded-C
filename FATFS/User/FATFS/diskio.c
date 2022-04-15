/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "bsp_spi.h"

/* Definitions of physical drive number for each drive */
#define DEV_MMC		  0	/* Example: Map MMC/SD card to physical drive 1 */
#define SPI_FLASH		1	/* SPI Flash Interface */


/*-----------------------------------------------------------------------*/
/* ·µ»Ø®”Ç°•rég´Á                                                      */
/*-----------------------------------------------------------------------*/

__weak DWORD get_fattime(void) {
	return	  ((DWORD)(2015 - 1980) << 25)	/* Year 2015 */
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
		
	case DEV_MMC : /* SD Card */
	break;

	case SPI_FLASH :
		if(Flash_Read_WRITE_JEDEC_ID() == FLASH_ID){ /* check if Flash is alive */
			stat &= ~STA_NOINIT;
		}

		break;
	}
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Initialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	uint16_t t;
	DSTATUS stat = STA_NOINIT;

	switch (pdrv) {
		
	case DEV_MMC : /* SD Card */
	break;

	case SPI_FLASH :
		Flash_Init();
		/* delay for a short period of time */
		t = 500;
		while(t--);
		/* Power up */
		Flash_Release_Power_Down();
		stat = disk_status(pdrv);
		break;
	}
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;

	switch (pdrv) {

	case DEV_MMC : /* SD Card */
	break;

	case SPI_FLASH :
		sector += 4096; /* Flash offset */
		FLASH_Read_MutiData(buff, sector << 12, count << 12); /* re-caculate address & count */
		res = RES_OK;
	break;
	
	}

	return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	uint32_t addr;
	DRESULT res = RES_PARERR;

	switch (pdrv) {

	case DEV_MMC : /* SD Card */
	break;

	case SPI_FLASH :
		sector += 4096; /* Flash offset */
		addr = sector << 12;
		Flash_Erase_Sector(addr);
		FLASH_Write_MutiData((uint8_t*)buff, addr, count << 12);
		res = RES_OK;
	break;
	
	}

	return res;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;

	switch (pdrv) {

	case DEV_MMC :

		// Process of the command for the MMC/SD card

		return res;

	case SPI_FLASH :

		// Process of the command the USB drive

		return res;
	}

	return RES_PARERR;
}


