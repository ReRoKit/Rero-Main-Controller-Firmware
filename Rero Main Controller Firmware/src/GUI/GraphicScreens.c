/*******************************************************************************
 * Common functions for rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "Variables.h"
#include "Log.h"
#include "Sound Stream/TouchTone.h"
#include "Sound Stream/Startup.h"
#include "Flash Image/SplashScreenLogo.h"
#include "GUI/Custom Graphic Object/Battery.h"

#include "GUI/GraphicScreens.h"
#include "GUI/MainMenu.h"
#include "GUI/SetIdPage.h"
#include "GUI/MotionPage.h"
#include "GUI/PlayPage.h"
#include "GUI/TeachPage.h"
#include "GUI/SettingsPage.h"
#include "GUI/G15Page.h"
#include "GUI/SensorPage.h"
#include "GUI/LockScreen.h"
#include "GUI/UsbPage.h"
#include "GUI/BtSettingsPage.h"
#include "GUI/BluetoothPage.h"
#include "GUI/SetBtPinPage.h"
#include "GUI/WallpaperPage.h"
#include "GUI/UserProgramPage.h"

// FreeRTOS include.
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

// Position and size for graphic objects.
// Exit button.
#define BTN_EXIT_WIDTH      40
#define BTN_EXIT_GAP        5       // Gap between the exit button and the edge of the title bar.
#define BTN_EXIT_X2         (GetMaxX() - GOL_EMBOSS_SIZE - 4 - BTN_EXIT_GAP)
#define BTN_EXIT_X1         (BTN_EXIT_X2 - BTN_EXIT_WIDTH)
#define BTN_EXIT_Y1         (GOL_EMBOSS_SIZE + 4 + BTN_EXIT_GAP)
#define BTN_EXIT_Y2         (WND_TITLE_HEIGHT + GOL_EMBOSS_SIZE - BTN_EXIT_GAP)

// Battery icon.
#define BATT_X2             (BTN_EXIT_X1 - 10)
#define BATT_X1             (BATT_X2 - BATT_WIDTH)
#define BATT_Y1             ((BTN_EXIT_Y1 + BTN_EXIT_Y2) / 2 - (BATT_HEIGHT / 2))
#define BATT_Y2             (BATT_Y1 + BATT_HEIGHT)



/*******************************************************************************
 * PRIVATE GLOBAL VARIABLES
 *******************************************************************************/

static unsigned char prv_ucCaptureScreenshot = 0;
static unsigned char prv_ucRedrawGui = 1;
static GUI_PAGE prv_eGuiPage = PAGE_MAIN_MENU;



/*******************************************************************************
 * FUNCTION: vDisplayLoadingText
 *
 * PARAMETERS:
 * ~ szText     - The text to display.
 * ~ ucLine     - First or second line (1 or 2).
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Show the text message when booting up.
 * Font size = Medium.
 *
 *******************************************************************************/
void vDisplayLoadingText(char *szText, unsigned char ucLine)
{
    unsigned short usTxtPosY = 200;

    switch (ucLine) {
        case 1: usTxtPosY = 200; break;
        case 2: usTxtPosY = 220; break;
    }

    // Calculate the center position.
    SHORT sLeft = (GetMaxX() + 1 - GetTextWidth(szText, (void *)ptrMediumFont)) / 2;

    // Clear the background.
    SetColor(pxDefaultScheme->CommonBkColor);
    Bar(0, usTxtPosY, GetMaxX(), usTxtPosY + GetTextHeight((void *)ptrMediumFont));

    // Show the text.
    SetFont((void *)ptrMediumFont);
    SetColor(pxDefaultScheme->TextColor0);
    OutTextXY(sLeft, usTxtPosY, szText);

    DelayMs(100);
}



/*******************************************************************************
 * FUNCTION: vDisplayShutdownText
 *
 * PARAMETERS:
 * ~ szText     - The text to display.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Show the text message during shut down.
 * Font size = Large.
 *
 *******************************************************************************/
void vDisplayShutdownText(char *szText)
{
    // Calculate the center position.
    SHORT sLeft = (GetMaxX() + 1 - GetTextWidth(szText, (void *)ptrLargeFont)) / 2;
    SHORT sTop = 145;

    // Clear the background.
    SetColor(pxDefaultScheme->CommonBkColor);
    Bar(0, sTop, GetMaxX(), sTop + GetTextHeight((void *)ptrLargeFont));

    // Show the text.
    SetFont((void *)ptrLargeFont);
    SetColor(pxLightGreenTxtScheme->TextColor0);
    OutTextXY(sLeft, sTop, szText);
}



/*******************************************************************************
 * FUNCTION: vShowStartScreen
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Show the splash screen during boot up.
 *
 *******************************************************************************/
void vShowStartScreen(void)
{
    GOLFree();
    SetColor(pxDefaultScheme->CommonBkColor);
    ClearDevice();

    // Calculate the center position.
    SHORT sLeft = (GetMaxX() + 1 - SplashScreenLogo_WIDTH) / 2;

    // Show the splash screen.
    while (PutImage(sLeft, RERO_LOGO_Y, (void *) &SplashScreenLogo, IMAGE_NORMAL) == 0);
}



/*******************************************************************************
 * FUNCTION: vShowStartScreen
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Clear the screen with fading effect.
 *
 *******************************************************************************/
void vClearScreenWithFadeEffect(void)
{
    WORD    i, j, k;
    WORD    ytemp, xtemp, srRes = 0x0001;

    SetClipRgn( 0,         0,
                GetMaxX(), GetMaxY() );

    SetClip(CLIP_ENABLE);

    // Random fade effect using a Linear Feedback Shift Register (LFSR)
    SetColor(pxDefaultScheme->CommonBkColor);
    for (i = 2500; i > 0; i--) {
        // For a 16 bit LFSR variable the taps are at bits: 1, 2, 4, and 15
        srRes = (srRes >> 1) ^ (-(int)(srRes & 1) & 0x8016);
        xtemp = (srRes & 0x00FF) % 40;
        ytemp = (srRes >> 8) % 30;

        // by replicating the white (1x1) bars into 8 areas fading is faster
        for (j = 0; j < 8; j++) {
            for (k = 0; k < 8; k++) {
                PutPixel(0 + xtemp + (j * 40), ytemp + (k * 30));
            }
        }
    }
    
    SetClip(CLIP_DISABLE);
}



/*******************************************************************************
 * FUNCTION: vCreatePageWindow
 *
 * PARAMETERS:
 * ~ szTitle    - Title for the page.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Clear the standard page window frame.
 *
 *******************************************************************************/
void vCreatePageWindow(char *szTitle)
{
    // Create the window.
    WndCreate( GID_WINDOW,
               0,         0,
               GetMaxX(), GetMaxY(),
               WND_DRAW, NULL, szTitle, pxDefaultScheme );

    // Create the battery level indicator.
    BattCreate ( GID_BATT_ICON,
                 BATT_X1, BATT_Y1,
                 BATT_X2, BATT_Y2,
                 BATT_DRAW, pxDefaultScheme );

    // Create the exit button.
    BtnCreate( GID_BTN_EXIT,
               BTN_EXIT_X1, BTN_EXIT_Y1,
               BTN_EXIT_X2, BTN_EXIT_Y2,
               0, BTN_DRAW, NULL, "X", pxExitBtnScheme );
}



/*******************************************************************************
 * FUNCTION: vClearWindowSubpage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Clear the subpage of a window page.
 *
 *******************************************************************************/
void vClearWindowSubpage(void)
{
    // Remove all subpage objects.
    OBJ_HEADER *pxCurrentObject = GOLGetList();
    while (pxCurrentObject != NULL) {
        unsigned short usObjectId = GetObjID(pxCurrentObject);
        switch (usObjectId) {
            // Do not delete these objects.
            case GID_WINDOW:
            case GID_BATT_ICON:
            case GID_BTN_EXIT:
                break;
            
            // Delete the remaining objects.
            default:
                GOLDeleteObject(pxCurrentObject);
                break;
        }
        pxCurrentObject = GetObjNext(pxCurrentObject);
    }
    
    // Clear the subpage.
    SetColor(pxDefaultScheme->CommonBkColor);
    Bar(WND_SUBPAGE_LEFT, WND_SUBPAGE_TOP, WND_SUBPAGE_RIGHT, WND_SUBPAGE_BOTTOM);
}



/*******************************************************************************
 * FUNCTION: vGraphicsObjectEnable
 *
 * PARAMETERS:
 * ~ pxObject     - The object to enable.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Enable the graphics object and redraw it.
 *
 *******************************************************************************/
void vGraphicsObjectEnable(void *pxObject)
{
    if (pxObject != NULL) {
        ClrState(pxObject, DISABLED);
        SetState(pxObject, DRAW);
    }
}



/*******************************************************************************
 * FUNCTION: vGraphicsObjectDisable
 *
 * PARAMETERS:
 * ~ pxObject     - The object to disable.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Disable the graphics object and redraw it.
 *
 *******************************************************************************/
void vGraphicsObjectDisable(void *pxObject)
{
    if (pxObject != NULL) {
        SetState(pxObject, DISABLED | DRAW);
    }
}



/*******************************************************************************
 * FUNCTION: vGraphicsObjectUnhide
 *
 * PARAMETERS:
 * ~ pxObject     - The object to unhide.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Unhide the graphics object and redraw it.
 *
 *******************************************************************************/
void vGraphicsObjectUnhide(void *pxObject)
{
    if (pxObject != NULL) {
        ClrState(pxObject, HIDE);
        SetState(pxObject, DRAW);
    }
}



/*******************************************************************************
 * FUNCTION: vGraphicsObjectHide
 *
 * PARAMETERS:
 * ~ pxObject     - The object to hide.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Hide the graphics object and redraw it.
 *
 *******************************************************************************/
void vGraphicsObjectHide(void *pxObject)
{
    if (pxObject != NULL) {
        SetState(pxObject, HIDE | DRAW);
    }
}



/*******************************************************************************
 * FUNCTION: vStaticTextUpdateText
 *
 * PARAMETERS:
 * ~ pxStaticText   - The static text to update.
 * ~ szText         - New text.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Change the text and redraw the static text.
 *
 *******************************************************************************/
void vStaticTextUpdateText(STATICTEXT *pxStaticText, char *szText)
{
    if ((pxStaticText != NULL) && (szText != NULL)) {
        StSetText(pxStaticText, szText);
        SetState(pxStaticText, ST_UPDATE);
    }
}



/*******************************************************************************
 * FUNCTION: vEditBoxUpdateText
 *
 * PARAMETERS:
 * ~ pxEditBox      - The edit box to update.
 * ~ szText         - New text.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Change the text and redraw the edit box.
 *
 *******************************************************************************/
void vEditBoxUpdateText(EDITBOX *pxEditBox, char *szText)
{
    if ((pxEditBox != NULL) && (szText != NULL)) {
        EbSetText(pxEditBox, szText);
        SetState(pxEditBox, EB_DRAW);
    }
}



/*******************************************************************************
 * FUNCTION: vSliderUpdatePosition
 *
 * PARAMETERS:
 * ~ pxSlider       - The slider to update.
 * ~ sNewPosition   - New position.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Change the slider position and redraw the slider thumb.
 *
 *******************************************************************************/
void vSliderUpdatePosition(SLIDER *pxSlider, short sNewPosition)
{
    if (pxSlider != NULL) {
        SldSetPos(pxSlider, sNewPosition);
        SetState(pxSlider, SLD_DRAW_THUMB);
    }
}



/*******************************************************************************
 * FUNCTION: vSetGuiPage
 *
 * PARAMETERS:
 * ~ eGuiPage    - The new GUI page.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Display the new page in GUI.
 *
 *******************************************************************************/
void vSetGuiPage(GUI_PAGE eGuiPage)
{
    prv_eGuiPage = eGuiPage;
    prv_ucRedrawGui = 1;
}



/*******************************************************************************
 * FUNCTION: eGetGuiPage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ The curent GUI page.
 *
 * DESCRIPTIONS:
 * Get the current displayed GUI page.
 *
 *******************************************************************************/
GUI_PAGE eGetGuiPage(void)
{
    return prv_eGuiPage;
}



/*******************************************************************************
 * FUNCTION: vCaptureScreenshot
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Set the flag to capture the screenshot.
 *
 *******************************************************************************/
void vCaptureScreenshot(void)
{
    prv_ucCaptureScreenshot = 1;
}



/*******************************************************************************
 * FUNCTION: vLockScreen
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Lock the screen.
 *
 *******************************************************************************/
void vLockScreen(void)
{
    // Make sure the screen is not locked before.
    if (xSystemState.bStandby == 0) {
        xSystemState.bStandby = 1;
        prv_ucRedrawGui = 1;
    }
}



/*******************************************************************************
 * FUNCTION: vUnlockScreen
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Unlock the screen.
 *
 *******************************************************************************/
void vUnlockScreen(void)
{
    // Make sure the screen is locked before.
    if (xSystemState.bStandby == 1) {
        xSystemState.bStandby = 0;
        
        vUnhidePreviousScreen();
        
        // Only play the sound effect if no sound is playing.
        if (ucIsAudioPlaying() == 0) {
            vPlaySoundStream(&xStartupSound);
        }
    }
}



/*******************************************************************************
 * TASK: taskGraphicDisplay
 *
 * DESCRIPTIONS:
 * Handle the state machine of the GUI as well as the touch event.
 *
 *******************************************************************************/
void taskGraphicDisplay(void *pvParameters )
{
    while(1) {
        // Draw the screen.
        // If drawing is completed...
        if (GOLDraw() != 0) {
            // Process the touch event if there is any.
            GOL_MSG xTouchMsg;
            if (xQueueReceive(xTouchMsgQueue, &xTouchMsg, 100 / portTICK_RATE_MS) == pdTRUE) {
                GOLMsg(&xTouchMsg);
            }
        }

        // If drawing is not completed, delay a while to let other task running.
        else {
            vTaskDelay(50 / portTICK_RATE_MS);
        }

        // Read the stack watermark and record it if it's below threshold.
        unsigned portBASE_TYPE uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        if (uxHighWaterMark < MIN_STACK_WATERMARK) {
            xSystemError.bStackLowError = 1;

            // Only log it when the watermark value changed.
            static portBASE_TYPE uxPreviousWatermark = 0;
            if (uxHighWaterMark != uxPreviousWatermark) {
                vLogStackWatermark("Screen Display Task", (unsigned short)uxHighWaterMark);
            }
            uxPreviousWatermark = uxHighWaterMark;
        }
    }
}



/******************************************************************************
 * Function: WORD GOLDrawCallback()
 * Output:   if the function returns non-zero the draw control will be passed to GOL
 * Overview: this function must be implemented by user. GOLDraw() function calls it each
 *           time when GOL objects drawing is completed. User drawing should be done here.
 *           GOL will not change color, line type and clipping region settings while
 *           this function returns zero.
 ******************************************************************************/
WORD GOLDrawCallback(void)
{
    // Show the lock screen page if the screen is locked.
    if (xSystemState.bStandby == 1) {
        // Only redraw the page when there is changes.
        if (prv_ucRedrawGui != 0) {
            prv_ucRedrawGui = 0;
            
            vHideScreen();
            vCreateLockScreen();
            
            // Only play the sound effect if no sound is playing.
            if (ucIsAudioPlaying() == 0) {
                vPlaySoundStream(&xTouchTone);
            }
        }
    }
    
    // Screen is not locked.
    else {
        // Only redraw the page when there is changes.
        if (prv_ucRedrawGui != 0) {
            prv_ucRedrawGui = 0;
        
            switch (prv_eGuiPage) {
                case PAGE_MAIN_MENU:    vCreateMainMenu();          break;
                
                case PAGE_BT_REMOTE:
                case PAGE_BT_PROGRAM:   vCreateBluetoothPage();     break;
                
                case PAGE_SERVO:        vCreateG15Page();           break;
                case PAGE_SENSOR:       vCreateSensorPage();        break;
                
                case PAGE_TEACH:        vCreateTeach();             break;
                case PAGE_PLAY:         vCreatePlay();              break;
                
                case PAGE_WALLPAPER:    vCreateMotionPage();     break;
//                case PAGE_WALLPAPER:    vCreateWallpaperPage();     break;
                case PAGE_SETTINGS:     vCreateSettingsPage();      break;
                case PAGE_SET_ID:       vCreateSetIdPage();         break;
                case PAGE_BT_SETTINGS:  vCreateBtSettingsPage();    break;
                case PAGE_SET_BT_PIN:   vCreateSetBtPinPage();      break;
                
                case PAGE_USB:          vCreateUsbPage();           break;
                case PAGE_USER_PROGRAM: vCreateUserProgramPage();   break;
            }   // End of switch-case.
            
        }
        
        // There is no changes on the GUI page.
        else {
            switch (prv_eGuiPage) {
                case PAGE_SENSOR:   vUpdateSensorPage();    break;
                case PAGE_USB:      vDisplayUsbPage();      break;
            }
        }
    
    }   // End of "Screen is not locked".
    
    
    
    // Capture screenshot if the flag is set.
    if (prv_ucCaptureScreenshot == 1) {
        prv_ucCaptureScreenshot = 0;
        
        // Only play the sound effect if no sound is playing.
        if (ucIsAudioPlaying() == 0) {
            vPlaySoundStream(&xTouchTone);
        }
        
        vCreateScreenshot();
    }
    
    return 1;
}



/*******************************************************************************
 * Function: WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
 * Input:    objMsg - translated message for the object,
 *           pObj - pointer to the object,
 *           pMsg - pointer to the non-translated, raw GOL message
 * Output:   if the function returns non-zero the message will be processed by default
 * Overview: this function must be implemented by user. GOLMsg() function calls it each
 *           time the valid message for the object received
 ******************************************************************************/
WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    // Beep.
    switch (objMsg) {
        case BTN_MSG_PRESSED:
        case RB_MSG_CHECKED:
        case LB_MSG_SEL:
            vPlaySoundStream(&xTouchTone);
            break;
    }
    
    
    // Process the touch event.
    if (xSystemState.bStandby == 1) {
        return usMsgLockScreen(objMsg, pObj, pMsg);
    }
    else {
        switch (prv_eGuiPage) {
            case PAGE_MAIN_MENU:    return usMsgMainMenu(objMsg, pObj, pMsg);
            case PAGE_USB:          return usMsgUsbPage(objMsg, pObj, pMsg);
            
            case PAGE_BT_REMOTE:
            case PAGE_BT_PROGRAM:   return usMsgBluetoothPage(objMsg, pObj, pMsg);
            
            case PAGE_SERVO:        return usMsgG15Page(objMsg, pObj, pMsg);
            case PAGE_SENSOR:       return usMsgSensorPage(objMsg, pObj, pMsg);
            
            case PAGE_TEACH:        return usMsgTeach(objMsg, pObj, pMsg);
            case PAGE_PLAY:         return usMsgPlay(objMsg, pObj, pMsg);
            
            case PAGE_WALLPAPER:    return usMsgMotionPage(objMsg, pObj, pMsg);
//            case PAGE_WALLPAPER:    return usMsgWallpaperPage(objMsg, pObj, pMsg);
            case PAGE_SETTINGS:     return usMsgSettingsPage(objMsg, pObj, pMsg);
            case PAGE_SET_ID:       return usMsgSetIdPage(objMsg, pObj, pMsg);
            case PAGE_BT_SETTINGS:  return usMsgBtSettingsPage(objMsg, pObj, pMsg);
            case PAGE_SET_BT_PIN:   return usMsgSetBtPinPage(objMsg, pObj, pMsg);
            
            case PAGE_USER_PROGRAM: return usMsgUserProgramPage(objMsg, pObj, pMsg);
            
            default:                return 1;
        }
    }
}
