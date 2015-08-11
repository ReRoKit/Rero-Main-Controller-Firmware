/*******************************************************************************
 * Functions for user program page in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "GUI/UserProgramPage.h"
#include "GUI/GraphicScreens.h"
#include "UserProgram.h"
#include "HardwareProfile.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

// Position and size for graphic objects.
// Title.
#define TXT_TITLE_WIDTH     190
#define TXT_TITLE_HEIGHT    20
#define TXT_TITLE_L         ((GetMaxX() - TXT_TITLE_WIDTH) / 2)
#define TXT_TITLE_R         (TXT_TITLE_L + TXT_TITLE_WIDTH)
#define TXT_TITLE_T         25
#define TXT_TITLE_B         (TXT_TITLE_T + TXT_TITLE_HEIGHT)

// Size for the button.
#define BTN_WIDTH           72
#define BTN_HEIGHT          72

// Gap between each button.
#define BTN_GAP             50

// Play button.
#define BTN_PLAY_R          (GetMaxX() / 2 - (BTN_GAP / 2))
#define BTN_PLAY_L          (BTN_PLAY_R - BTN_WIDTH)
#define BTN_PLAY_T          ((GetMaxY() + TXT_TITLE_B) / 2 - (BTN_HEIGHT / 2))
#define BTN_PLAY_B          (BTN_PLAY_T + BTN_HEIGHT)

// Stop button.
#define BTN_STOP_L          (GetMaxX() / 2 + (BTN_GAP / 2))
#define BTN_STOP_R          (BTN_STOP_L + BTN_WIDTH)
#define BTN_STOP_T          BTN_PLAY_T
#define BTN_STOP_B          BTN_PLAY_B



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
    WndCreate( GID_WINDOW,
               0,         0,
               GetMaxX(), GetMaxY(),
               WND_DRAW, NULL, NULL, pxMainWndScheme );
    
    // Title.
    StCreate( GID_USER_TXT_TITLE,
              TXT_TITLE_L, TXT_TITLE_T,
              TXT_TITLE_R, TXT_TITLE_B,
              ST_DRAW | ST_CENTER_ALIGN | ST_FRAME, "CUSTOM USER PROGRAM", pxDefaultScheme );
    
    // Play button.
    BtnCreate( GID_USER_BTN_PLAY,
               BTN_PLAY_L, BTN_PLAY_T,
               BTN_PLAY_R, BTN_PLAY_B,
               0, BTN_DRAW | BTN_TEXTBOTTOM | BTN_NOPANEL, "/theme/icon/play.ico", "PLAY", pxImgBtnScheme );
    
    // Stop button.
    BtnCreate( GID_USER_BTN_STOP,
               BTN_STOP_L, BTN_STOP_T,
               BTN_STOP_R, BTN_STOP_B,
               0, BTN_DRAW | BTN_TEXTBOTTOM | BTN_NOPANEL | BTN_DISABLED, "/theme/icon/stop.ico", "STOP", pxImgBtnScheme );
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
                xTaskCreate( taskUserProgram, "USER", 512, NULL, tskIDLE_PRIORITY + 2, &xUserProgramTask );
                
                break;
                
            case GID_USER_BTN_STOP:
                // Enable the play button and disable the stop button.
                vGraphicsObjectEnable(GOLFindObject(GID_USER_BTN_PLAY));
                vGraphicsObjectDisable(GOLFindObject(GID_USER_BTN_STOP));
                
                // Stop the user program.
                if (xUserProgramTask != NULL) {
                    vTaskDelete(xUserProgramTask);
                }
                
                // Clear the LED on main controller.
                LEDR = L_OFF;
                LEDG = L_OFF;
                LEDB = L_OFF;
                
                
                // Disable all output.
                vEMDisableAllOutput();
                
                break;
        }
    }

    return 1;
}
