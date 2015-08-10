/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "FatFs/SD-SPI.h"
#include "FatFs/diskio.h"


/* Definitions of physical drive number for each media */
#define SD		0


// Disk status.
static volatile DSTATUS stat = STA_NOINIT;



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
    MEDIA_INFORMATION *pxMediaInfo;

    if (pdrv == SD) {
        // Make sure the SD card is in the slot.
        if (MDD_SDSPI_MediaDetect() == FALSE) {
            stat = STA_NODISK;
        }
        else {
            // Try to mount the SD card.
            pxMediaInfo = MDD_SDSPI_MediaInitialize();
            if (pxMediaInfo->errorCode == MEDIA_NO_ERROR) {
                stat &= ~STA_NOINIT;

                // Check whether it's write protected.
                if (MDD_SDSPI_WriteProtectState() == TRUE) {
                    stat |= STA_PROTECT;
                }
            }
        }

        return stat;
        
    }

    else {
        return STA_NOINIT;
    }
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
    if (pdrv == SD) {
        return stat;
    }
    else {
        return STA_NOINIT;
    }
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,	/* Physical drive nmuber (0..) */
	BYTE *buff,	/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count	/* Number of sectors to read (1..128) */
)
{
    ASYNC_IO info;

    if ((pdrv == SD) && (count > 0)) {
        // Make sure the SD card is already initialized.
        if (stat == STA_NOINIT) {
            return RES_NOTRDY;
        }

        //Initialize info structure for using the MDD_SDSPI_AsyncReadTasks() function.
        info.wNumBytes = MEDIA_SECTOR_SIZE;
        info.dwBytesRemaining = (DWORD)count * MEDIA_SECTOR_SIZE;
        info.pBuffer = buff - MEDIA_SECTOR_SIZE;
        info.dwAddress = sector;
        info.bStateVariable = ASYNC_READ_QUEUED;

        while(1) {
            BYTE status = MDD_SDSPI_AsyncReadTasks(&info);

            if (status == ASYNC_READ_NEW_PACKET_READY) {
                info.pBuffer += MEDIA_SECTOR_SIZE;
            }
            else if (status == ASYNC_READ_COMPLETE) {
                return RES_OK;
            }
            else if (status == ASYNC_READ_ERROR) {
                return RES_ERROR;
            }
        }

    }
    else {
        return RES_PARERR;
    }
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
    ASYNC_IO info;

    if ((pdrv == SD) && (count > 0)) {
        // Make sure the SD card is already initialized.
        if (stat == STA_NOINIT) {
            return RES_NOTRDY;
        }

        // Make sure the SD card is not write protected.
        if (stat == STA_PROTECT) {
            return RES_WRPRT;
        }


        //Initialize info structure for using the MDD_SDSPI_AsyncReadTasks() function.
        info.wNumBytes = MEDIA_SECTOR_SIZE;
        info.dwBytesRemaining = (DWORD)count * MEDIA_SECTOR_SIZE;
        info.pBuffer = (BYTE *)buff - MEDIA_SECTOR_SIZE;
        info.dwAddress = sector;
        info.bStateVariable = ASYNC_WRITE_QUEUED;

        while(1) {
            BYTE status = MDD_SDSPI_AsyncWriteTasks(&info);

            if (status == ASYNC_WRITE_SEND_PACKET) {
                info.pBuffer += MEDIA_SECTOR_SIZE;
            }
            else if (status == ASYNC_WRITE_COMPLETE) {
                return RES_OK;
            }
            else if (status == ASYNC_WRITE_ERROR) {
                return RES_ERROR;
            }
        }
    }
    else {
        return RES_PARERR;
    }
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
    DRESULT res;
    BYTE n, csd[16], *ptr = buff;
    DWORD csz;



    if (pdrv == SD) {

        // Make sure the SD card is already initialized.
        if (stat == STA_NOINIT) {
            return RES_NOTRDY;
        }

        res = RES_ERROR;
        switch (cmd) {
            case CTRL_SYNC :	/* Flush write-back cache, Wait for end of internal process */
                res = RES_OK;
                break;

            case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (WORD) */
                *(DWORD*)buff = MDD_SDSPI_ReadCapacity();
                res = RES_OK;
                break;

//            case GET_BLOCK_SIZE :	/* Get erase block size in unit of sectors (DWORD) */
//                if (CardType & CT_SD2) {	/* SDv2? */
//                        if (send_cmd(ACMD13, 0) == 0) {		/* Read SD status */
//                                xchg_spi(0xFF);
//                                if (rcvr_datablock(csd, 16)) {				/* Read partial block */
//                                        for (n = 64 - 16; n; n--) xchg_spi(0xFF);	/* Purge trailing data */
//                                        *(DWORD*)buff = 16UL << (csd[10] >> 4);
//                                        res = RES_OK;
//                                }
//                        }
//                } else {					/* SDv1 or MMCv3 */
//                        if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) {	/* Read CSD */
//                                if (CardType & CT_SD1) {	/* SDv1 */
//                                        *(DWORD*)buff = (((csd[10] & 63) << 1) + ((WORD)(csd[11] & 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
//                                } else {					/* MMCv3 */
//                                        *(DWORD*)buff = ((WORD)((csd[10] & 124) >> 2) + 1) * (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);
//                                }
//                                res = RES_OK;
//                        }
//                }
//                break;
//
//        case MMC_GET_TYPE :		/* Get card type flags (1 byte) */
//                *ptr = CardType;
//                res = RES_OK;
//                break;
//
//        case MMC_GET_CSD :	/* Receive CSD as a data block (16 bytes) */
//                if ((send_cmd(CMD9, 0) == 0)	/* READ_CSD */
//                        && rcvr_datablock(buff, 16))
//                        res = RES_OK;
//                break;
//
//        case MMC_GET_CID :	/* Receive CID as a data block (16 bytes) */
//                if ((send_cmd(CMD10, 0) == 0)	/* READ_CID */
//                        && rcvr_datablock(buff, 16))
//                        res = RES_OK;
//                break;
//
//        case MMC_GET_OCR :	/* Receive OCR as an R3 resp (4 bytes) */
//                if (send_cmd(CMD58, 0) == 0) {	/* READ_OCR */
//                        for (n = 0; n < 4; n++)
//                                *((BYTE*)buff+n) = xchg_spi(0xFF);
//                        res = RES_OK;
//                }
//                break;
//
//        case MMC_GET_SDSTAT :	/* Receive SD statsu as a data block (64 bytes) */
//                if ((CardType & CT_SD2) && send_cmd(ACMD13, 0) == 0) {	/* SD_STATUS */
//                        xchg_spi(0xFF);
//                        if (rcvr_datablock(buff, 64))
//                                res = RES_OK;
//                }
//                break;

            default:
                res = RES_PARERR;
        }

        return res;
    }

    else {
        return RES_PARERR;
    }
}



#endif
