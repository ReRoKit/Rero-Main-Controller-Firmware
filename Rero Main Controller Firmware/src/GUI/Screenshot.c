/*******************************************************************************
 * Functions to capture the screenshot.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "GUI/Screenshot.h"
#include "Variables.h"
#include "FatFs/FatFsWrapper.h"
#include "Microchip/Graphics/Graphics.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
* PRIVATE GLOBAL VARIABLES
*******************************************************************************/

const char prv_pucBitmapHeader320x240[] = {
    0x42, 0x4D, 0x46, 0x58, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00, 0x38, 0x00, 
    0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x00, 0x03, 0x00, 
    0x00, 0x00, 0x00, 0x58, 0x02, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0x1F, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };



/*******************************************************************************
 * FUNCTION: vCreateScreenshot
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create a bitmap file for the screenshot .
 *
 *******************************************************************************/
void vCreateScreenshot(void)
{
    // File handle.
    FSFILE *pxFile;
    
    xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
    
    // Filename.
    char szFilename[] = "Screenshot999.bmp";
    
    // Try to find the filename which is not used yet.
    unsigned short i;
    for (i = 0; i < 1000; i++) {
        // Get the filename.
        sprintf(szFilename, "Screenshot%u.bmp", i);
        
        // Make sure the file is not available.
        pxFile = FSfopen(szFilename, "r");
        if (pxFile == NULL) {
            break;
        }
        
        // Close the opened file.
        FSfclose(pxFile);
    }
    
    // Make sure there isn't too many file.
    if (i < 1000) {
        // Open the file for write.
        pxFile = FSfopen(szFilename, "w");
        if (pxFile != NULL) {
            // Write the bitmap header.
            FSfwrite(prv_pucBitmapHeader320x240, 1, sizeof(prv_pucBitmapHeader320x240), pxFile);
            
            // Write the pixel data.
            signed short x;
            signed short y;
            for (y = GetMaxY(); y >= 0; y--) {
                for (x = 0; x <= GetMaxX(); x++) {
                    unsigned short usColour = GetPixel(x, y);
                    FSfwrite(&usColour, 1, sizeof(usColour), pxFile);
                }
                
                LEDR ^= 1;
                LEDG ^= 1;
                LEDB = L_OFF;
            }
            
            // Close the opened file.
            FSfclose(pxFile);
        }
    }
    
    xSemaphoreGive(xSdCardMutex);
}
