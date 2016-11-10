/*******************************************************************************
 * This file provides the logging function to the SD card.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include <string.h>
#include "FatFs/FatFsWrapper.h"
#include "Variables.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
* PRIVATE FUNCTION PROTOTYPES
*******************************************************************************/

static void prv_vUShortToString(unsigned short usValue, char* szString);



/*******************************************************************************
* FUNCTION: prv_vUShortToString
*
* PARAMETERS:
* ~ usValue   - The value we want to convert.
* ~ szString  - Buffer to store the string result.
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Convert the value in unsigned short to string. The result is 5-Digit character
* and zero padded. Reserve 6-space array for the result (Extra one for null ended).
*
*******************************************************************************/
static void prv_vUShortToString(unsigned short usValue, char* szString)
{
    *szString++ = (char)(usValue / 10000) + 0x30;
    usValue %= 10000;

    *szString++ = (char)(usValue / 1000) + 0x30;
    usValue %= 1000;

    *szString++ = (char)(usValue / 100) + 0x30;
    usValue %= 100;

    *szString++ = (char)(usValue / 10) + 0x30;
    usValue %= 10;

    *szString++ = (char)usValue + 0x30;

    *szString = 0;
}



/*******************************************************************************
 * FUNCTION: vLogStackWatermark
 *
 * PARAMETERS:
 * ~ szTaskName     - Task name.
 * ~ usWatermark    - Value for watermark.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Log the RTOS low stack error.
 *
 *******************************************************************************/
void vLogStackWatermark(const char* szTaskName, unsigned short usWatermark)
{
    // Convert the watermark to string.
    char szWatermark[6];
    prv_vUShortToString(usWatermark, szWatermark);



    // Open the log file.
    xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
    portENTER_CRITICAL();
    FSFILE *pxLogFile = FSfopen(pucLogFilePath, "a");


    // Write the message.
    const char szTxt1[] = "[Stack Error] ";
    FSfwrite(szTxt1, 1, sizeof(szTxt1) - 1, pxLogFile);
    FSfwrite(szTaskName, 1, strlen(szTaskName), pxLogFile);

    const char szTxt2[] = "    Watermark: ";
    FSfwrite(szTxt2, 1, sizeof(szTxt2) - 1, pxLogFile);
    FSfwrite(szWatermark, 1, strlen(szWatermark), pxLogFile);
    
    const char szTxt3[] = "    Firmware: ";
    FSfwrite(szTxt3, 1, sizeof(szTxt3) - 1, pxLogFile);
    FSfwrite(szFirmwareVersion, 1, strlen(szFirmwareVersion), pxLogFile);
    
    const char szCrLf[] = "\r\n";
    FSfwrite(szCrLf, 1, sizeof(szCrLf) - 1, pxLogFile);


    // Close the log file.
    FSfclose(pxLogFile);
    portEXIT_CRITICAL();
    xSemaphoreGive(xSdCardMutex);
}



/*******************************************************************************
 * FUNCTION: vLogStackOverflow
 *
 * PARAMETERS:
 * ~ szTaskName     - Task name.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Log the RTOS stack overflow error.
 *
 *******************************************************************************/
void vLogStackOverflow(const char* szTaskName)
{
    FSFILE *pxLogFile = FSfopen(pucLogFilePath, "a");
    
    const char szLogMsg[] = "[Severe Stack Error] Task Name: ";
    FSfwrite(szLogMsg, 1, sizeof(szLogMsg) - 1, pxLogFile);
    FSfwrite(szTaskName, 1, strlen(szTaskName), pxLogFile);
    
    const char szFirmwareTitle[] = "    Firmware: ";
    FSfwrite(szFirmwareTitle, 1, sizeof(szFirmwareTitle) - 1, pxLogFile);
    FSfwrite(szFirmwareVersion, 1, strlen(szFirmwareVersion), pxLogFile);
    
    FSfwrite("\r\n", 1, 2, pxLogFile);
    FSfclose(pxLogFile);
}



/*******************************************************************************
 * FUNCTION: vLogMallocError
 *
 * PARAMETERS:
 * ~ szSource     - Location where malloc failed.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Log the malloc error.
 *
 *******************************************************************************/
void vLogMallocError(const char* szSource)
{
    // Open the log file.
    xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
    portENTER_CRITICAL();
    FSFILE *pxLogFile = FSfopen(pucLogFilePath, "a");


    // Write the message.
    const char szTxt1[] = "[Malloc Error] ";
    FSfwrite(szTxt1, 1, sizeof(szTxt1) - 1, pxLogFile);
    FSfwrite(szSource, 1, strlen(szSource), pxLogFile);
    
    const char szTxt2[] = "    Firmware: ";
    FSfwrite(szTxt2, 1, sizeof(szTxt2) - 1, pxLogFile);
    FSfwrite(szFirmwareVersion, 1, strlen(szFirmwareVersion), pxLogFile);

    const char szCrLf[] = "\r\n";
    FSfwrite(szCrLf, 1, sizeof(szCrLf) - 1, pxLogFile);


    // Close the log file.
    FSfclose(pxLogFile);
    portEXIT_CRITICAL();
    xSemaphoreGive(xSdCardMutex);
}



/*******************************************************************************
 * FUNCTION: vLogGeneralException
 *
 * PARAMETERS:
 * ~ ucExcCode  - ExcCode field of the Cause register.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Log the general exception.
 *
 *******************************************************************************/
void vLogGeneralException(unsigned char ucExcCode)
{
    // Convert the ExcCode to ASCII in hex.
    if (ucExcCode < 0x0a) {
        ucExcCode += 0x30;
    }
    else {
        ucExcCode += 0x37;
    }


    // Open the log file.
    FSFILE *pxLogFile = FSfopen(pucLogFilePath, "a");

    // Write the message.
    const char szTxt1[] = "[General Exception] Cause: ";
    FSfwrite(szTxt1, 1, sizeof(szTxt1) - 1, pxLogFile);
    FSfwrite(&ucExcCode, 1, 1, pxLogFile);
    
    const char szTxt2[] = "    Firmware: ";
    FSfwrite(szTxt2, 1, sizeof(szTxt2) - 1, pxLogFile);
    FSfwrite(szFirmwareVersion, 1, strlen(szFirmwareVersion), pxLogFile);

    const char szCrLf[] = "\r\n";
    FSfwrite(szCrLf, 1, sizeof(szCrLf) - 1, pxLogFile);


    // Close the log file.
    FSfclose(pxLogFile);
}
