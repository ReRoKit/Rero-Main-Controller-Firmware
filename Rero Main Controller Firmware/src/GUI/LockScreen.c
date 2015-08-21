/*******************************************************************************
 * Functions for lock screen in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "Variables.h"
#include "GUI/Custom Graphic Object/Battery.h"
#include "GUI/GraphicScreens.h"
#include "GUI/HideUnhideScreen.h"
#include "Flash Image/SplashScreenLogo.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

// Position and size for graphic objects.
#define TXT_HEIGHT          20

// Text for "REconfigurable RObot".
#define TXT_RERO_Y1         (RERO_LOGO_Y + SplashScreenLogo_HEIGHT + 10)
#define TXT_RERO_Y2         (TXT_RERO_Y1 + TXT_HEIGHT)

// Slider for unlock.
#define SLD_UNLOCK_WIDTH    220
#define SLD_UNLOCK_HEIGHT   50
#define SLD_UNLOCK_X1       ((GetMaxX() + 1 - SLD_UNLOCK_WIDTH) / 2)
#define SLD_UNLOCK_X2       (SLD_UNLOCK_X1 + SLD_UNLOCK_WIDTH)
#define SLD_UNLOCK_Y1       (TXT_RERO_Y2 + 20)
#define SLD_UNLOCK_Y2       (SLD_UNLOCK_Y1 + SLD_UNLOCK_HEIGHT)

// Text for "Slide to unlock".
#define TXT_UNLOCK_Y1       (SLD_UNLOCK_Y2 + 5)
#define TXT_UNLOCK_Y2       (TXT_UNLOCK_Y1 + TXT_HEIGHT)

// Battery icon.
#define BATT_X2             (GetMaxX() - 5)
#define BATT_X1             (BATT_X2 - BATT_WIDTH)
#define BATT_Y1             5
#define BATT_Y2             (BATT_Y1 + BATT_HEIGHT)



/*******************************************************************************
 * FUNCTION: vCreateLockScreen
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the lock screen.
 *
 *******************************************************************************/
void vCreateLockScreen(void)
{
    // Display the rero logo in center.
    SHORT sLeft = (GetMaxX() + 1 - SplashScreenLogo_WIDTH) / 2;
    while (PutImage(sLeft, RERO_LOGO_Y, (void *) &SplashScreenLogo, IMAGE_NORMAL) == 0);

    // Create text "REconfigurable RObot".
    StCreate( GID_LOCKSCREEN_TXT_RERO,
              0,         TXT_RERO_Y1,
              GetMaxX(), TXT_RERO_Y2,
              ST_CENTER_ALIGN | ST_DRAW, "reconfigurable robot", pxDefaultScheme );

    // Create the unlock slider.
    SLIDER *pSld = SldCreate( GID_LOCKSCREEN_SLD_UNLOCK,
                              SLD_UNLOCK_X1, SLD_UNLOCK_Y1,
                              SLD_UNLOCK_X2, SLD_UNLOCK_Y2,
                              SLD_SCROLLBAR | SLD_DRAW, 100, 1, 0, pxSliderScheme );

    // Set the slider thumb width.
    pSld->thWidth = 20;

    // Create text "Slide to unlock >>>".
    StCreate( GID_LOCKSCREEN_TXT_RERO,
              0,         TXT_UNLOCK_Y1,
              GetMaxX(), TXT_UNLOCK_Y2,
              ST_CENTER_ALIGN | ST_DRAW, "Slide to unlock >>>", pxDefaultScheme );

    // Create the battery level indicator.
    BattCreate ( GID_BATT_ICON,
                 BATT_X1, BATT_Y1,
                 BATT_X2, BATT_Y2,
                 BATT_DRAW, pxDefaultScheme );
}



/*******************************************************************************
 * FUNCTION: usMsgLockScreen
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
 * Handle the touchscreen event of the lock screen.
 *
 *******************************************************************************/
WORD usMsgLockScreen (WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    if(GetObjID(pObj) == GID_LOCKSCREEN_SLD_UNLOCK) {
        static short sStartPosition = 100;
        short sCurrentPosition = (short)SldGetPos(pObj);
        
        // Reset the start position when the slider is pressed.
        if(pMsg->uiEvent == EVENT_PRESS) {
            sStartPosition = 100;
        }

        // Get the minimum position when sliding.
        else if(pMsg->uiEvent == EVENT_MOVE) {
            if (sCurrentPosition < sStartPosition) {
                sStartPosition = sCurrentPosition;
            }
        }

        // When the slider is released...
        else if(pMsg->uiEvent == EVENT_RELEASE) {
            // If it has been slided for more than 50%, unlock the screen.
            if((sCurrentPosition - sStartPosition) > 50) {
                vUnlockScreen();
            }
            
            // Else, return the slider to the left most position and reset the start position.
            else {
                vSliderUpdatePosition((SLIDER*)pObj, 0);
                sStartPosition = 100;
            }
        }
    }

    return 1;
}
