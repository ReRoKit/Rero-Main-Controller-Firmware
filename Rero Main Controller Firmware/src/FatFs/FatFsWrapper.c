/*******************************************************************************
 * Wrapper functions for the FatFs library to replace the Microchip MDD library.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "FatFs/FatFsWrapper.h"
#include "Variables.h"



/*******************************************************************************
* PRIVATE GLOBAL VARIABLES
*******************************************************************************/

// Timestamp variables.
static unsigned char prv_ucYear     = (unsigned char)(2013 - 1980);
static unsigned char prv_ucMonth    = 1;
static unsigned char prv_ucDay      = 1;
static unsigned char prv_ucHour     = 0;
static unsigned char prv_ucMinute   = 0;
static unsigned char prv_ucSecond   = 0;



/*******************************************************************************
 * FUNCTION: get_fattime
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ RTC value.
 *
 * DESCRIPTIONS:
 * This is a real time clock service to be called from FatFs module.
 * Return the packed RTC value.
 *
 *******************************************************************************/
unsigned long get_fattime(void)
{
	unsigned long ulRtcValue;
    
    ulRtcValue = (unsigned long)prv_ucYear << 25;
    ulRtcValue |= (unsigned long)prv_ucMonth << 21;
    ulRtcValue |= (unsigned long)prv_ucDay << 16;
    ulRtcValue |= (unsigned long)prv_ucHour << 11;
    ulRtcValue |= (unsigned long)prv_ucMinute << 5;
    ulRtcValue |= (unsigned long)prv_ucSecond >> 1;

	return ulRtcValue;
}



/*******************************************************************************
 * FUNCTION: vSetClock
 *
 * PARAMETERS:
 * ~ uiYear     - Year (1980 to 2107).
 * ~ ucMonth    - Month (1 to 12).
 * ~ ucDay      - Day (1 to 31).
 * ~ ucHour     - Hour (0 to 23).
 * ~ ucMinute   - Minute (0 to 59).
 * ~ ucSecond   - Second (0 to 59).
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Manually set the timestamp variables.
 * Call this before creating a file or directory (set create time) and
 * before closing a file (set last access time, last modified time).
 *
 *******************************************************************************/
void vSetClock(unsigned int uiYear, unsigned char ucMonth, unsigned char ucDay, unsigned char ucHour, unsigned char ucMinute, unsigned char ucSecond)
{
    // Make sure the data is valid.
    if ( (uiYear < 1980) || (uiYear > 2107) ||
         (ucMonth < 1) || (ucMonth > 12) ||
         (ucDay < 1) || (ucDay > 31) ||
         (ucHour > 23) || (ucMinute > 59) || (ucSecond > 59) ) {
        return;
    }

    prv_ucYear = (unsigned char)(uiYear - 1980);
    prv_ucMonth = ucMonth;
    prv_ucDay = ucDay;
    prv_ucHour = ucHour;
    prv_ucMinute = ucMinute;
    prv_ucSecond = ucSecond;
}



/*********************************************************************
  Function:
    FSFILE * FSfopen (const char * fileName, const char *mode)
  Summary:
    Open a Ascii file
  Conditions:
    For read modes, file exists; FSInit performed
  Input:
    fileName -  The name of the file to open
    mode -
         - WRITE -      Create a new file or replace an existing file
         - READ -       Read data from an existing file
         - APPEND -     Append data to an existing file
         - WRITEPLUS -  Create a new file or replace an existing file (reads also enabled)
         - READPLUS -   Read data from an existing file (writes also enabled)
         - APPENDPLUS - Append data to an existing file (reads also enabled)
  Return Values:
    FSFILE * - The pointer to the file object
    NULL -     The file could not be opened
  Description:
    Wrapper function for Microchip MDDFS Library.
  *********************************************************************/
FSFILE *FSfopen(const char * fileName, const char* mode)
{
    BYTE bMode;

    switch (*mode) {
        case 'w':
            bMode = FA_CREATE_ALWAYS | FA_WRITE;
            break;
        case 'r':
            bMode = FA_OPEN_EXISTING | FA_READ;
            break;
        case 'a':
            bMode = FA_OPEN_ALWAYS | FA_WRITE;
            break;
        default:
            bMode = FA_OPEN_EXISTING | FA_READ;
            break;
    }

    if (*++mode == '+') {
        bMode |= FA_READ | FA_WRITE;
    }

    FIL* pFile = (FIL*)malloc(sizeof(FIL));
    if (pFile != NULL) {
        if (f_open(pFile, fileName, bMode) == FR_OK) {
            if (bMode & FA_OPEN_ALWAYS) {
                f_lseek(pFile, f_size(pFile));
            }
            return pFile;
        }
        else {
            free((void*)pFile);
            return NULL;
        }
    }
    else {
        xSystemError.bMallocError = 1;

        // Log the malloc error to SD card.
        vLogMallocError("FSfopen");

        return NULL;
    }
}



/************************************************************
  Function:
    int FSfclose(FSFILE *fo)
  Summary:
    Update file information and free FSFILE objects
  Conditions:
    File opened
  Input:
    fo -  Pointer to the file to close
  Return Values:
    0 -   File closed successfully
    EOF - Error closing the file
  Description:
    Wrapper function for Microchip MDDFS Library.
  ************************************************************/
int FSfclose(FSFILE *fo)
{
    if (f_close(fo) == FR_OK) {
        free((void*)fo);
        return 0;
    }
    else {
        return 1;
    }
}



/**************************************************************************
  Function:
    size_t FSfread(void *ptr, size_t size, size_t n, FSFILE *stream)
  Summary:
    Read data from a file
  Conditions:
    File is opened in a read mode
  Input:
    ptr -     Destination buffer for read bytes
    size -    Size of units in bytes
    n -       Number of units to be read
    stream -  File to be read from
  Return:
    size_t - number of units read
  Description:
    Wrapper function for Microchip MDDFS Library.
  Remarks:
    None.
  **************************************************************************/
size_t FSfread (void *ptr, size_t size, size_t n, FSFILE *stream)
{
    UINT br = 0;
    f_read(stream, ptr, size * n, &br);
    return br;
}



/*********************************************************************************
  Function:
    size_t FSfwrite(const void *data_to_write, size_t size, size_t n, FSFILE *stream)
  Summary:
    Write data to a file
  Conditions:
    File opened in WRITE, APPEND, WRITE+, APPEND+, READ+ mode
  Input:
    data_to_write -     Pointer to source buffer
    size -              Size of units in bytes
    n -                 Number of units to transfer
    stream -  Pointer to file structure
  Return:
    size_t - number of units written
  Side Effects:
    The FSerrno variable will be changed.
  Description:
    Wrapper function for Microchip MDDFS Library.
  *********************************************************************************/
size_t FSfwrite(const void *data_to_write, size_t size, size_t n, FSFILE *stream)
{
    UINT bw = 0;
    f_write(stream, data_to_write, size * n, &bw);
    return bw;
}



/**********************************************************************
  Function:
    int FSfseek(FSFILE *stream, long offset, int whence)
  Summary:
    Change the current position in a file
  Conditions:
    File opened
  Input:
    stream -    Pointer to file structure
    offset -    Offset from base location
    whence -    SEEK_SET - Seek from the begining of the file.
                SEEK_CUR - Seek from current position.
                SEEK_END - Seek from the end of the file.
           
  Return Values:
    0 -  Operation successful
    -1 - Operation unsuccesful
  Description:
    Wrapper function for Microchip MDDFS Library.
  **********************************************************************/
int FSfseek(FSFILE *stream, long offset, int whence)
{
    int result;

    switch (whence) {
        case SEEK_SET:
            result = (f_lseek(stream, offset) == FR_OK)? 0 : 1;
            break;

        case SEEK_CUR:
            result = (f_lseek(stream, f_tell(stream) + offset) == FR_OK)? 0 : 1;
            break;

        case SEEK_END:
            result = (f_lseek(stream, f_size(stream) - offset) == FR_OK)? 0 : 1;
            break;

        default:
            result = 1;
    }

    return result;
}



/****************************************************
  Function:
    int FSfeof( FSFILE * stream )
  Summary:
    Indicate whether the current file position is at the end
  Conditions:
    File is open in a read mode
  Input:
    stream -  Pointer to the target file
  Return Values:
    Non-Zero - EOF reached
    0 - Not at end of File
  Description:
    Wrapper function for Microchip MDDFS Library.
  ****************************************************/
int FSfeof( FSFILE * stream )
{
    return f_eof(stream);
}



/*******************************************************************************
* FUNCTION: xFSGetReadOnlyFlag
*
* PARAMETERS:
* ~ szFileName      - The path to the file that we want to check.
* ~ pucReadOnlyFlag - Buffer to store the read only flag.
*
* RETURN:
* ~ Error code.
*
* DESCRIPTIONS:
* Check the file attributes and examine the read only flag.
*
*******************************************************************************/
FRESULT xFSGetReadOnlyFlag(const char *szFileName, unsigned char *pucReadOnlyFlag)
{
    char szLongFileName[(MAX_FILENAME_LENGTH * 2) + 4] = {0};
    
    FILINFO xFileInfo;
    xFileInfo.lfname = szLongFileName;
    xFileInfo.lfsize = sizeof(szLongFileName);
    
    FRESULT eErrorCode = f_stat(szFileName, &xFileInfo);
    if ((xFileInfo.fattrib & AM_RDO) == 0) {
        *pucReadOnlyFlag = 0;
    }
    else {
        *pucReadOnlyFlag = 1;
    }
    return eErrorCode;
}



/*******************************************************************************
* FUNCTION: xFSSetReadOnlyFlag
*
* PARAMETERS:
* ~ szFileName      - The path to the file that we want to check.
* ~ ucReadOnlyFlag  - The read only flag.
*
* RETURN:
* ~ Error code.
*
* DESCRIPTIONS:
* Set the read only flag of a file.
*
*******************************************************************************/
FRESULT xFSSetReadOnlyFlag(const char *szFileName, unsigned char ucReadOnlyFlag)
{
    return f_chmod(szFileName, ucReadOnlyFlag, AM_RDO);
}
