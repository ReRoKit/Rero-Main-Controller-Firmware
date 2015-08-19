/*******************************************************************************
 * Functions for bluetooth page in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include <string.h>
#include "GUI/BluetoothPage.h"
#include "GUI/GraphicScreens.h"
#include "GUI/PopUpMessagePage.h"

#include "Variables.h"
#include "Bluetooth/BluetoothControl.h"
#include "PlayMotion.h"
#include "Planner.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

// Position and size for graphic objects.
// Text for robot name and bluetooth address.
#define TXT_HEIGHT              20
#define TXT_Y1                  (WND_LIMIT_BOTTOM - TXT_HEIGHT)
#define TXT_Y2                  WND_LIMIT_BOTTOM

// Bluetooth logo.
#define IMG_BTLOGO_WIDTH        200
#define IMG_BTLOGO_HEIGHT       160
#define IMG_BTLOGO_X1           ((WND_LIMIT_LEFT + WND_LIMIT_RIGHT) / 2 - (IMG_BTLOGO_WIDTH / 2))
#define IMG_BTLOGO_X2           (IMG_BTLOGO_X1 + IMG_BTLOGO_WIDTH)
#define IMG_BTLOGO_Y1           ((WND_LIMIT_TOP + WND_LIMIT_BOTTOM - TXT_HEIGHT) / 2 - (IMG_BTLOGO_HEIGHT / 2))
#define IMG_BTLOGO_Y2           (IMG_BTLOGO_Y1 + IMG_BTLOGO_HEIGHT)



/*******************************************************************************
 * FUNCTION: vCreateBluetoothPage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the bluetooth page.
 *
 *******************************************************************************/
void vCreateBluetoothPage(void)
{
    short sLeft;
    short sRight;
    
    
    // Wait until the bluetooth is ready.
    if (xSystemState.bBluetoothReady == 0) {
        // Show the message.
        vCreateGenericMessagePage("Initializing bluetooth module...");
        
        while (xSystemState.bBluetoothReady == 0) {
            vTaskDelay(100 / portTICK_RATE_MS);
        }
    }
    
    
    GOLFree();
    if (eGetGuiPage() == PAGE_BT_PROGRAM) {
        vCreatePageWindow("BLUETOOTH PROGRAMMING");
    } else {
        vCreatePageWindow("REMOTE");
    }
    
    

    // Create the bluetooth logo.
    PictCreate( GID_BT_IMG_LOGO,
                IMG_BTLOGO_X1, IMG_BTLOGO_Y1,
                IMG_BTLOGO_X2, IMG_BTLOGO_Y2,
                PICT_DRAW, 1, "/Theme/BluetoothPage/BluetoothBackground.bmp", pxDefaultScheme );
    
    
    // Create text for robot name.
    char *szName = "Name:";
    sLeft = WND_LIMIT_LEFT;
    sRight = sLeft + GetTextWidth(szName, pxDefaultScheme->pFont) + (ST_INDENT * 2);
    StCreate( GID_BT_TXT_NAME1,
              sLeft,  TXT_Y1,
              sRight, TXT_Y2,
              ST_DRAW, szName, pxDefaultScheme );
    
    sLeft = sRight;
    sRight = sLeft + GetTextWidth(pucRobotName, pxLightBlueTxtScheme->pFont) + (ST_INDENT * 2);
    StCreate( GID_BT_TXT_NAME2,
              sLeft,  TXT_Y1,
              sRight, TXT_Y2,
              ST_DRAW, pucRobotName, pxLightBlueTxtScheme );
    
    
    // Create text for bluetooth address.
    sRight = WND_LIMIT_RIGHT;
    sLeft = sRight - GetTextWidth(szBluetoothAddress, pxLightBlueTxtScheme->pFont) - (ST_INDENT * 2);
    StCreate( GID_BT_TXT_ADDR2,
              sLeft,  TXT_Y1,
              sRight, TXT_Y2,
              ST_DRAW, szBluetoothAddress, pxLightBlueTxtScheme );
    
    char *szAddress = "Addr:";
    sRight = sLeft;
    sLeft = sRight - GetTextWidth(szAddress, pxDefaultScheme->pFont) - (ST_INDENT * 2);
    StCreate( GID_BT_TXT_ADDR2,
              sLeft,  TXT_Y1,
              sRight, TXT_Y2,
              ST_DRAW, szAddress, pxDefaultScheme );
    
    
    
    // We are in bluetooth mode.
    xSystemState.bBluetoothMode = 1;
}



/*******************************************************************************
 * FUNCTION: usMsgBluetoothPage
 *
 * PARAMETERS:
 * ~ objMsg     - Translated message for the object.
 * ~ pObj       - Pointer to the object.
 * ~ pMsg       - Pointer to the non-translated, raw GOL message.
 *
 * RETURN:
 * ~ If the function returns non-zero, the message will be processed by default.
 *
 * DESCRIPTIONS:
 * Handle the touchscreen event of the Bluetooth Programming page.
 *
 *******************************************************************************/
WORD usMsgBluetoothPage(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    switch (GetObjID(pObj)) {
        case GID_BTN_EXIT:
            if (objMsg == BTN_MSG_RELEASED) {
                // Stop all motion.
                // Delay a while to allow the play motion task to stop and memory to be freed.
                vPlannerStopCurrent(STOP_NOW);
                vPlayMotionStopAll(STOP_NOW);
                vTaskDelay(250 / portTICK_RATE_MS);
                
                // Disable all output.
                vEMDisableAllOutput();

                xSystemState.bBluetoothMode = 0;
                vSetGuiPage(PAGE_MAIN_MENU);
            }
            break;

        default:
            break;
    }

    return 1;
}
