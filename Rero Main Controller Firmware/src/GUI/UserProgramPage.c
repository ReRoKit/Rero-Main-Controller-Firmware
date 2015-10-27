/*******************************************************************************
 * Functions for user program page in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "GUI/UserProgramPage.h"
#include "GUI/GraphicScreens.h"
#include "GUI/Custom Graphic Object/Battery.h"
#include "UserProgram.h"
#include "HardwareProfile.h"
#include "Variables.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

// Position and size for graphic objects.
// Size for the button.
#define BTN_WIDTH           72
#define BTN_HEIGHT          72

// Gap between each button.
#define BTN_GAP             50

// Play button.
#define BTN_PLAY_R          (GetMaxX() / 2 - (BTN_GAP / 2))
#define BTN_PLAY_L          (BTN_PLAY_R - BTN_WIDTH)
#define BTN_PLAY_T          ((WND_SUBPAGE_TOP + WND_SUBPAGE_BOTTOM) / 2 - (BTN_HEIGHT / 2))
#define BTN_PLAY_B          (BTN_PLAY_T + BTN_HEIGHT)

// Stop button.
#define BTN_STOP_L          (GetMaxX() / 2 + (BTN_GAP / 2))
#define BTN_STOP_R          (BTN_STOP_L + BTN_WIDTH)
#define BTN_STOP_T          BTN_PLAY_T
#define BTN_STOP_B          BTN_PLAY_B

// Battery icon.
#define BATT_R              (GetMaxX() - 15)
#define BATT_L              (BATT_R - BATT_WIDTH)
#define BATT_T              15
#define BATT_B              (BATT_T + BATT_HEIGHT)



/*******************************************************************************
 * FUNCTION: vCreateUserProgramPage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the user program page.
 *
 *******************************************************************************/
void vCreateUserProgramPage(void)
{
    // Clear the GOL objects and screen.
    GOLFree();
    SetColor(pxDefaultScheme->CommonBkColor);
    ClearDevice();


    // Create the window frame outline.
    vCreatePageWindow("CUSTOM C PROGRAM");
    
    // Play button.
    BtnCreate( GID_USER_BTN_PLAY,
               BTN_PLAY_L, BTN_PLAY_T,
               BTN_PLAY_R, BTN_PLAY_B,
               0, BTN_DRAW | BTN_TEXTBOTTOM | BTN_NOPANEL, "/Theme/UserProgramPage/Play.bmp", "PLAY", pxImgBtnScheme );
    
    // Stop button.
    BtnCreate( GID_USER_BTN_STOP,
               BTN_STOP_L, BTN_STOP_T,
               BTN_STOP_R, BTN_STOP_B,
               0, BTN_DRAW | BTN_TEXTBOTTOM | BTN_NOPANEL | BTN_DISABLED, "/Theme/UserProgramPage/Stop.bmp", "STOP", pxImgBtnScheme );
    
    
    
    // Disable the play button if the RUN_USER_PROGRAM is not defined.
#ifndef RUN_USER_PROGRAM
    vGraphicsObjectDisable((void*)GOLFindObject(GID_USER_BTN_PLAY));
#endif
}



/*******************************************************************************
 * FUNCTION: usMsgUserProgramPage
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
 * Handle the touchscreen event of the user program page.
 *
 *******************************************************************************/
WORD usMsgUserProgramPage(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    static xTaskHandle xUserProgramTask = NULL;
    
    // When the button is released...
    if (objMsg == BTN_MSG_RELEASED) {
        switch(GetObjID(pObj)) {
            case GID_USER_BTN_PLAY:
                // Disable the play button and enable the stop button.
                vGraphicsObjectDisable(GOLFindObject(GID_USER_BTN_PLAY));
                vGraphicsObjectEnable(GOLFindObject(GID_USER_BTN_STOP));
                
                // Run the user program.
                xTaskCreate( taskUserProgram, "USER", 512, NULL, tskIDLE_PRIORITY, &xUserProgramTask );
                
                break;
                
            case GID_USER_BTN_STOP:
                // Enable the play button and disable the stop button.
                vGraphicsObjectEnable(GOLFindObject(GID_USER_BTN_PLAY));
                vGraphicsObjectDisable(GOLFindObject(GID_USER_BTN_STOP));
                
                // Stop the user program.
                if (xUserProgramTask != NULL) {
                    vTaskDelete(xUserProgramTask);
                    xUserProgramTask = NULL;
                    
                    // Wait until the task is deleted.
                    vTaskDelay(50 / portTICK_RATE_MS);
                    
                    // Release the mutex in case of they are taken.
                    xSemaphoreGive(xSdCardMutex);
                    xSemaphoreGive(xExternalUartMutex);
                    xSemaphoreGive(xBluetoothMutex);
                }
                
                // Clear the LED on main controller.
                LEDR = L_OFF;
                LEDG = L_OFF;
                LEDB = L_OFF;
                
                
                // Disable all output.
                vEMDisableAllOutput();
                
                break;
                
                // Exit button.
            case GID_BTN_EXIT:
                // Stop the user program.
                if (xUserProgramTask != NULL) {
                    vTaskDelete(xUserProgramTask);
                    xUserProgramTask = NULL;
                    
                    // Wait until the task is deleted.
                    vTaskDelay(50 / portTICK_RATE_MS);
                    
                    // Release the mutex in case of they are taken.
                    xSemaphoreGive(xSdCardMutex);
                    xSemaphoreGive(xExternalUartMutex);
                    xSemaphoreGive(xBluetoothMutex);
                }
                
                // Disable all output.
                vEMDisableAllOutput();
                
                // Back to main screen.
                vSetGuiPage(PAGE_MAIN_MENU);
                break;
        }
    }

    return 1;
}
