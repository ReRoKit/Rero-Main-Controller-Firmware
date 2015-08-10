/*******************************************************************************
 * Functions for settings page in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include <stdio.h>
#include "Variables.h"
#include "Configuration.h"
#include "AudioAmp.h"
#include "Sound Stream/TouchTone.h"
#include "GUI/SettingsPage.h"
#include "GUI/GraphicScreens.h"
#include "Bluetooth/BluetoothControl.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

// Position and size for common graphic objects.
#define BTN_PREV_NEXT_WIDTH         60
#define BTN_PREV_NEXT_HEIGHT        24

#define TXT_PAGE_WIDTH              75
#define TXT_HEIGHT                  20

#define MARGIN_TOP                  15
#define MARGIN_BOTTOM               MARGIN_TOP
#define MARGIN_LEFT                 30
#define MARGIN_RIGHT                MARGIN_LEFT

#define SETTINGS_SUBPAGE_TOP        (WND_TITLE_HEIGHT + 2)
#define SETTINGS_SUBPAGE_BOTTOM     (GetMaxY() - MARGIN_BOTTOM - BTN_PREV_NEXT_HEIGHT - 1)
#define SETTINGS_SUBPAGE_LEFT       1
#define SETTINGS_SUBPAGE_RIGHT      (GetMaxX() - 1)
#define SETTINGS_SUBPAGE_CENTER_X   ((SETTINGS_SUBPAGE_LEFT + SETTINGS_SUBPAGE_RIGHT) / 2)
#define SETTINGS_SUBPAGE_CENTER_Y   ((SETTINGS_SUBPAGE_TOP + SETTINGS_SUBPAGE_BOTTOM) / 2)


// Position and size for graphic objects in page 1.
#define PAGE1_BTN_GAP               30
#define PAGE1_BTN_WIDTH             72
#define PAGE1_BTN_HEIGHT            72


// Position and size for graphic objects in page 2.
#define PAGE2_SLD_C2C               65
#define PAGE2_BTN_SLD_GAP           15
#define PAGE2_BTN_WIDTH             30
#define PAGE2_BTN_HEIGHT            30
#define PAGE2_TXT_HEIGHT            20
#define PAGE2_SLD_HEIGHT            30
#define PAGE2_TXT_SLD_GAP           3

#define PAGE2_SLD_CENTER_Y1         (SETTINGS_SUBPAGE_CENTER_Y - (PAGE2_SLD_C2C / 2))
#define PAGE2_SLD_CENTER_Y2         (SETTINGS_SUBPAGE_CENTER_Y + (PAGE2_SLD_C2C / 2))


// Position and size for graphic objects in page 3.
#define PAGE3_SLD_C2C               65
#define PAGE3_BTN_SLD_GAP           15
#define PAGE3_BTN_WIDTH             30
#define PAGE3_BTN_HEIGHT            30
#define PAGE3_TXT_HEIGHT            20
#define PAGE3_SLD_HEIGHT            30
#define PAGE3_TXT_SLD_GAP           3

#define PAGE3_SLD_CENTER_Y1         (SETTINGS_SUBPAGE_CENTER_Y - (PAGE3_SLD_C2C / 2))
#define PAGE3_SLD_CENTER_Y2         (SETTINGS_SUBPAGE_CENTER_Y + (PAGE3_SLD_C2C / 2))


// Position and size for graphic objects in page 4.
#define PAGE4_IMG_WIDTH             100
#define PAGE4_IMG_HEIGHT            24
#define PAGE4_IMG_Y                 75

#define PAGE4_TXT_HEIGHT            20
#define PAGE4_TXT_Y1                130
#define PAGE4_TXT_Y2                155



/*******************************************************************************
* PRIVATE GLOBAL VARIABLES
*******************************************************************************/

// Current settings page.
static unsigned char prv_ucCurrentPage = 1;


// Initial settings.
static unsigned short prv_usPrevBacklight;
static unsigned char prv_ucPrevVolume;
static AUTO_SLEEP_SETTING prv_ePrevAutoSleep;
static AUTO_SLEEP_SETTING prv_ePrevAutoShutdown;



/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 *******************************************************************************/

static void prv_vClearSettingsSubpage(void);
static void prv_vCreateSubpage(void);
static void prv_vCreateSubpage1(void);
static void prv_vCreateSubpage2(void);
static void prv_vCreateSubpage3(void);
static void prv_vCreateSubpage4(void);
static void prv_vUpdateTextAutoSleep(AUTO_SLEEP_SETTING eValue);
static void prv_vUpdateTextAutoShutdown(AUTO_SHUTDOWN_SETTING eValue);



/*******************************************************************************
 * FUNCTION: prv_vClearSettingsSubpage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Clear the subpage of the settings page.
 *
 *******************************************************************************/
static void prv_vClearSettingsSubpage(void)
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
            
            case GID_SETTINGS_BTN_PREV:
            case GID_SETTINGS_BTN_NEXT:
            case GID_SETTINGS_TXT_PAGE:
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
    Bar(SETTINGS_SUBPAGE_LEFT, SETTINGS_SUBPAGE_TOP, SETTINGS_SUBPAGE_RIGHT, SETTINGS_SUBPAGE_BOTTOM);
}



/*******************************************************************************
 * FUNCTION: prv_vCreateSubpage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the settings page subpage according to the page number.
 *
 *******************************************************************************/
static void prv_vCreateSubpage(void)
{
    // Get the graphic objects.
    BUTTON *pxBtnPrevious = (BUTTON*)GOLFindObject(GID_SETTINGS_BTN_PREV);
    BUTTON *pxBtnNext = (BUTTON*)GOLFindObject(GID_SETTINGS_BTN_NEXT);
    STATICTEXT *pxTxtPage = (STATICTEXT*)GOLFindObject(GID_SETTINGS_TXT_PAGE);

    // Clear the subpage.
    prv_vClearSettingsSubpage();
    
    switch (prv_ucCurrentPage) {
        case 1:
            // Disable the previous button and enable the next button.
            vGraphicsObjectDisable(pxBtnPrevious);
            vGraphicsObjectEnable(pxBtnNext);
            
            // Update the page text.
            vStaticTextUpdateText(pxTxtPage, "PAGE: 1/4");
            
            // Create the subpage.
            prv_vCreateSubpage1();
            break;
            
            
            
        case 2:
            // Enable the previous and next button.
            vGraphicsObjectEnable(pxBtnPrevious);
            vGraphicsObjectEnable(pxBtnNext);
            
            // Update the page text.
            vStaticTextUpdateText(pxTxtPage, "PAGE: 2/4");
            
            // Create the subpage.
            prv_vCreateSubpage2();
            break;
            
            
            
        case 3:
            // Enable the previous and next button.
            vGraphicsObjectEnable(pxBtnPrevious);
            vGraphicsObjectEnable(pxBtnNext);
            
            // Update the page text.
            vStaticTextUpdateText(pxTxtPage, "PAGE: 3/4");
            
            // Create the subpage.
            prv_vCreateSubpage3();
            break;
            
            
            
        case 4:
            // Enable the previous button and disable the next button.
            vGraphicsObjectEnable(pxBtnPrevious);
            vGraphicsObjectDisable(pxBtnNext);
            
            // Update the page text.
            vStaticTextUpdateText(pxTxtPage, "PAGE: 4/4");
            
            // Create the subpage.
            prv_vCreateSubpage4();
            break;
            
            
            
        default:
            prv_ucCurrentPage = 1;
            prv_vCreateSubpage();
            break;
    }
}



/*******************************************************************************
 * FUNCTION: prv_vCreateSubpage1
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the settings page 1.
 *
 *******************************************************************************/
static void prv_vCreateSubpage1(void)
{
    // Set ID button.
    BtnCreate( GID_SETTINGS_BTN_SETID,
               SETTINGS_SUBPAGE_CENTER_X - (PAGE1_BTN_GAP / 2) - PAGE1_BTN_WIDTH, SETTINGS_SUBPAGE_CENTER_Y - (PAGE1_BTN_HEIGHT / 2),
               SETTINGS_SUBPAGE_CENTER_X - (PAGE1_BTN_GAP / 2),                   SETTINGS_SUBPAGE_CENTER_Y + (PAGE1_BTN_HEIGHT / 2),
               0, BTN_DRAW | BTN_TEXTBOTTOM | BTN_NOPANEL, "/theme/icon/setid.ico", "SET ID", pxImgBtnScheme );
    
    
    // Bluetooth settings button.
    BtnCreate( GID_SETTINGS_BTN_BLUETOOTH,
               SETTINGS_SUBPAGE_CENTER_X + (PAGE1_BTN_GAP / 2),                   SETTINGS_SUBPAGE_CENTER_Y - (PAGE1_BTN_HEIGHT / 2),
               SETTINGS_SUBPAGE_CENTER_X + (PAGE1_BTN_GAP / 2) + PAGE1_BTN_WIDTH, SETTINGS_SUBPAGE_CENTER_Y + (PAGE1_BTN_HEIGHT / 2),
               0, BTN_DRAW | BTN_TEXTBOTTOM | BTN_NOPANEL, "/theme/icon/bluetooth.ico", "BLUETOOTH", pxImgBtnScheme );
}



/*******************************************************************************
 * FUNCTION: prv_vCreateSubpage2
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the settings page 2.
 *
 *******************************************************************************/
static void prv_vCreateSubpage2(void)
{
    // Button and slider for backlight brightness.
    BtnCreate( GID_SETTINGS_BTN_BLIGHT,
               MARGIN_LEFT,                   PAGE2_SLD_CENTER_Y1 - (PAGE2_BTN_HEIGHT / 2),
               MARGIN_LEFT + PAGE2_BTN_WIDTH, PAGE2_SLD_CENTER_Y1 + (PAGE2_BTN_HEIGHT / 2),
               0, BTN_DRAW | BTN_NOPANEL, "/theme/icon/brightness.ico", NULL, pxDefaultScheme );
    
    StCreate( GID_SETTINGS_TXT_BLIGHT,
              MARGIN_LEFT + PAGE2_BTN_WIDTH + PAGE2_BTN_SLD_GAP, PAGE2_SLD_CENTER_Y1 - (PAGE2_SLD_HEIGHT / 2) - PAGE2_TXT_SLD_GAP - PAGE2_TXT_HEIGHT,
              GetMaxX() - MARGIN_RIGHT,                          PAGE2_SLD_CENTER_Y1 - (PAGE2_SLD_HEIGHT / 2) - PAGE2_TXT_SLD_GAP,
              ST_DRAW, "Brightness:", pxDefaultScheme );

    SldCreate( GID_SETTINGS_SLD_BLIGHT,
               MARGIN_LEFT + PAGE2_BTN_WIDTH + PAGE2_BTN_SLD_GAP, PAGE2_SLD_CENTER_Y1 - (PAGE2_SLD_HEIGHT / 2),
               GetMaxX() - MARGIN_RIGHT,                          PAGE2_SLD_CENTER_Y1 + (PAGE2_SLD_HEIGHT / 2),
               SLD_DRAW, 9300, 100, BLight, pxSliderScheme );



    // Button and slider for volume.
    BtnCreate( GID_SETTINGS_BTN_VOLUME,
               MARGIN_LEFT,                   PAGE2_SLD_CENTER_Y2 - (PAGE2_BTN_HEIGHT / 2),
               MARGIN_LEFT + PAGE2_BTN_WIDTH, PAGE2_SLD_CENTER_Y2 + (PAGE2_BTN_HEIGHT / 2),
               0, BTN_DRAW | BTN_NOPANEL, "/theme/icon/volume.ico", NULL, pxDefaultScheme );
    
    StCreate( GID_SETTINGS_TXT_VOLUME,
              MARGIN_LEFT + PAGE2_BTN_WIDTH + PAGE2_BTN_SLD_GAP, PAGE2_SLD_CENTER_Y2 - (PAGE2_SLD_HEIGHT / 2) - PAGE2_TXT_SLD_GAP - PAGE2_TXT_HEIGHT,
              GetMaxX() - MARGIN_RIGHT,                          PAGE2_SLD_CENTER_Y2 - (PAGE2_SLD_HEIGHT / 2) - PAGE2_TXT_SLD_GAP,
              ST_DRAW, "Volume:", pxDefaultScheme );

    SldCreate( GID_SETTINGS_SLD_VOLUME,
               MARGIN_LEFT + PAGE2_BTN_WIDTH + PAGE2_BTN_SLD_GAP, PAGE2_SLD_CENTER_Y2 - (PAGE2_SLD_HEIGHT / 2),
               GetMaxX() - MARGIN_RIGHT,                          PAGE2_SLD_CENTER_Y2 + (PAGE2_SLD_HEIGHT / 2),
               SLD_DRAW, 115, 1, ucVolume, pxSliderScheme );
}



/*******************************************************************************
 * FUNCTION: prv_vCreateSubpage3
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the settings page 3.
 *
 *******************************************************************************/
static void prv_vCreateSubpage3(void)
{
    // Auto sleep.
    BtnCreate( GID_SETTINGS_BTN_SLEEP,
               MARGIN_LEFT,                   PAGE3_SLD_CENTER_Y1 - (PAGE3_BTN_HEIGHT / 2),
               MARGIN_LEFT + PAGE3_BTN_WIDTH, PAGE3_SLD_CENTER_Y1 + (PAGE3_BTN_HEIGHT / 2),
               0, BTN_DRAW | BTN_NOPANEL, "/theme/icon/sleep.ico", NULL, pxDefaultScheme );
    
    StCreate( GID_SETTINGS_TXT_SLEEP,
              MARGIN_LEFT + PAGE3_BTN_WIDTH + PAGE3_BTN_SLD_GAP, PAGE3_SLD_CENTER_Y1 - (PAGE3_SLD_HEIGHT / 2) - PAGE3_TXT_SLD_GAP - PAGE3_TXT_HEIGHT,
              GetMaxX() - MARGIN_RIGHT,                          PAGE3_SLD_CENTER_Y1 - (PAGE3_SLD_HEIGHT / 2) - PAGE3_TXT_SLD_GAP,
              ST_DRAW, NULL, pxDefaultScheme );

    SldCreate( GID_SETTINGS_SLD_SLEEP,
               MARGIN_LEFT + PAGE3_BTN_WIDTH + PAGE3_BTN_SLD_GAP, PAGE3_SLD_CENTER_Y1 - (PAGE3_SLD_HEIGHT / 2),
               GetMaxX() - MARGIN_RIGHT,                          PAGE3_SLD_CENTER_Y1 + (PAGE3_SLD_HEIGHT / 2),
               SLD_DRAW, (unsigned short)AUTO_SLEEP_NEVER, 1, (unsigned short)eAutoSleep, pxSliderScheme );



    // Auto shutdown.
    BtnCreate( GID_SETTINGS_BTN_SHUTDOWN,
               MARGIN_LEFT,                   PAGE3_SLD_CENTER_Y2 - (PAGE3_BTN_HEIGHT / 2),
               MARGIN_LEFT + PAGE3_BTN_WIDTH, PAGE3_SLD_CENTER_Y2 + (PAGE3_BTN_HEIGHT / 2),
               0, BTN_DRAW | BTN_NOPANEL, "/theme/icon/shutdown.ico", NULL, pxDefaultScheme );
    
    StCreate( GID_SETTINGS_TXT_SHUTDOWN,
              MARGIN_LEFT + PAGE3_BTN_WIDTH + PAGE3_BTN_SLD_GAP, PAGE3_SLD_CENTER_Y2 - (PAGE3_SLD_HEIGHT / 2) - PAGE3_TXT_SLD_GAP - PAGE3_TXT_HEIGHT,
              GetMaxX() - MARGIN_RIGHT,                          PAGE3_SLD_CENTER_Y2 - (PAGE3_SLD_HEIGHT / 2) - PAGE3_TXT_SLD_GAP,
              ST_DRAW, NULL, pxDefaultScheme );

    SldCreate( GID_SETTINGS_SLD_SHUTDOWN,
               MARGIN_LEFT + PAGE3_BTN_WIDTH + PAGE3_BTN_SLD_GAP, PAGE3_SLD_CENTER_Y2 - (PAGE3_SLD_HEIGHT / 2),
               GetMaxX() - MARGIN_RIGHT,                          PAGE3_SLD_CENTER_Y2 + (PAGE3_SLD_HEIGHT / 2),
               SLD_DRAW, (unsigned short)AUTO_SHUTDOWN_NEVER, 1, (unsigned short)eAutoShutdown, pxSliderScheme );
    
    
    
    // Update the displayed value for auto sleep and auto shutdown.
    prv_vUpdateTextAutoSleep(eAutoSleep);
    prv_vUpdateTextAutoShutdown(eAutoShutdown);
}



/*******************************************************************************
 * FUNCTION: prv_vCreateSubpage4
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the settings page 4.
 *
 *******************************************************************************/
static void prv_vCreateSubpage4(void)
{
    // Rero logo
    PictCreate( GID_SETTINGS_IMG_RERO,
                SETTINGS_SUBPAGE_CENTER_X - (PAGE4_IMG_WIDTH / 2), PAGE4_IMG_Y,
                SETTINGS_SUBPAGE_CENTER_X + (PAGE4_IMG_WIDTH / 2), PAGE4_IMG_Y + PAGE4_IMG_HEIGHT,
                PICT_DRAW | PICT_DISABLED, 1, "/theme/icon/rero.ico", pxDefaultScheme );
    
    // Title.
    StCreate( GID_SETTINGS_TXT_FIRMWARE,
              SETTINGS_SUBPAGE_LEFT,  PAGE4_TXT_Y1,
              SETTINGS_SUBPAGE_RIGHT, PAGE4_TXT_Y1 + PAGE4_TXT_HEIGHT,
              ST_CENTER_ALIGN | ST_DRAW, "Firmware Version :", pxLightBlueTxtScheme );
    
    // Firmware version.
    StCreate( GID_SETTINGS_TXT_DATE,
              SETTINGS_SUBPAGE_LEFT,  PAGE4_TXT_Y2,
              SETTINGS_SUBPAGE_RIGHT, PAGE4_TXT_Y2 + PAGE4_TXT_HEIGHT,
              ST_CENTER_ALIGN | ST_DRAW, (char *)szFirmwareVersion, pxLightGreenTxtScheme );
}



/*******************************************************************************
 * FUNCTION: prv_vUpdateTextAutoSleep
 *
 * PARAMETERS:
 * ~ eValue     - The auto sleep value.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Update the displayed value for auto sleep.
 *
 *******************************************************************************/
static void prv_vUpdateTextAutoSleep(AUTO_SLEEP_SETTING eValue)
{
    STATICTEXT *pxTxtAutoSleep = (STATICTEXT*)GOLFindObject(GID_SETTINGS_TXT_SLEEP);
    
    switch (eValue) {
        case AUTO_SLEEP_15_SEC: vStaticTextUpdateText(pxTxtAutoSleep, "Auto Sleep: 15 seconds");    break;
        case AUTO_SLEEP_30_SEC: vStaticTextUpdateText(pxTxtAutoSleep, "Auto Sleep: 30 seconds");    break;
        case AUTO_SLEEP_1_MIN:  vStaticTextUpdateText(pxTxtAutoSleep, "Auto Sleep: 1 minute");      break;
        case AUTO_SLEEP_2_MIN:  vStaticTextUpdateText(pxTxtAutoSleep, "Auto Sleep: 2 minutes");     break;
        case AUTO_SLEEP_5_MIN:  vStaticTextUpdateText(pxTxtAutoSleep, "Auto Sleep: 5 minutes");     break;
        case AUTO_SLEEP_10_MIN: vStaticTextUpdateText(pxTxtAutoSleep, "Auto Sleep: 10 minutes");    break;
        case AUTO_SLEEP_NEVER:  vStaticTextUpdateText(pxTxtAutoSleep, "Auto Sleep: Never");         break;
    }
}



/*******************************************************************************
 * FUNCTION: prv_vUpdateTextAutoShutdown
 *
 * PARAMETERS:
 * ~ eValue     - The auto shutdown value.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Update the displayed value for auto shutdown.
 *
 *******************************************************************************/
static void prv_vUpdateTextAutoShutdown(AUTO_SHUTDOWN_SETTING eValue)
{
    STATICTEXT *pxTxtAutoShutdown = (STATICTEXT*)GOLFindObject(GID_SETTINGS_TXT_SHUTDOWN);
    
    switch (eValue) {
        case AUTO_SHUTDOWN_1_MIN:   vStaticTextUpdateText(pxTxtAutoShutdown, "Auto Shutdown: 1 minute");    break;
        case AUTO_SHUTDOWN_5_MIN:   vStaticTextUpdateText(pxTxtAutoShutdown, "Auto Shutdown: 5 minutes");   break;
        case AUTO_SHUTDOWN_10_MIN:  vStaticTextUpdateText(pxTxtAutoShutdown, "Auto Shutdown: 10 minutes");  break;
        case AUTO_SHUTDOWN_20_MIN:  vStaticTextUpdateText(pxTxtAutoShutdown, "Auto Shutdown: 20 minutes");  break;
        case AUTO_SHUTDOWN_30_MIN:  vStaticTextUpdateText(pxTxtAutoShutdown, "Auto Shutdown: 30 minutes");  break;
        case AUTO_SHUTDOWN_60_MIN:  vStaticTextUpdateText(pxTxtAutoShutdown, "Auto Shutdown: 60 minutes");  break;
        case AUTO_SHUTDOWN_NEVER:   vStaticTextUpdateText(pxTxtAutoShutdown, "Auto Shutdown: Never");       break;
    }
}



/*******************************************************************************
 * FUNCTION: vCreateSettingsPage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the settings page.
 *
 *******************************************************************************/
void vCreateSettingsPage(void)
{
    // Clear the screen.
    GOLFree();
    vCreatePageWindow("SETTINGS");
    
    
    // Previous button.
    BtnCreate( GID_SETTINGS_BTN_PREV,
               MARGIN_LEFT,                       GetMaxY() - MARGIN_BOTTOM - BTN_PREV_NEXT_HEIGHT,
               MARGIN_LEFT + BTN_PREV_NEXT_WIDTH, GetMaxY() - MARGIN_BOTTOM,
               BTN_RADIUS, BTN_DRAW, NULL, "<<<", pxTransBtnScheme );
    
    // Next button.
    BtnCreate( GID_SETTINGS_BTN_NEXT,
               GetMaxX() - MARGIN_RIGHT - BTN_PREV_NEXT_WIDTH, GetMaxY() - MARGIN_BOTTOM - BTN_PREV_NEXT_HEIGHT,
               GetMaxX() - MARGIN_RIGHT,                       GetMaxY() - MARGIN_BOTTOM,
               BTN_RADIUS, BTN_DRAW, NULL, ">>>", pxTransBtnScheme );
    
    // Page text.
    StCreate( GID_SETTINGS_TXT_PAGE,
              (GetMaxX() - TXT_PAGE_WIDTH) / 2,                  GetMaxY() - MARGIN_BOTTOM - TXT_HEIGHT - ((BTN_PREV_NEXT_HEIGHT - TXT_HEIGHT) / 2),
              (GetMaxX() - TXT_PAGE_WIDTH) / 2 + TXT_PAGE_WIDTH, GetMaxY() - MARGIN_BOTTOM,
              ST_CENTER_ALIGN | ST_DRAW, NULL, pxDefaultScheme );
    
    
    // Save the initial settings.
    prv_usPrevBacklight = BLight;
    prv_ucPrevVolume = ucVolume;
    prv_ePrevAutoSleep = eAutoSleep;
    prv_ePrevAutoShutdown = eAutoShutdown;
    
    
    // Create subpage.
    prv_vCreateSubpage();
}



/*******************************************************************************
 * FUNCTION: usMsgSettingsPage
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
 * Handle the touchscreen event of the Settings page.
 *
 *******************************************************************************/
WORD usMsgSettingsPage(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    // Slider and button event.
    if (objMsg == BTN_MSG_RELEASED) {
        switch(GetObjID(pObj)) {
            case GID_SETTINGS_BTN_BLIGHT:
                BLight = 6500;
                SetDCOC1PWM(BLight);
                
                vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SETTINGS_SLD_BLIGHT), (short)BLight);
                break;

            case GID_SETTINGS_BTN_VOLUME:
                ucVolume = 30;
                vSetVolume(ucVolume);
                
                vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SETTINGS_SLD_VOLUME), (short)ucVolume);
                break;
                
            case GID_SETTINGS_BTN_SLEEP:
                eAutoSleep = AUTO_SLEEP_2_MIN;
                
                vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SETTINGS_SLD_SLEEP), (short)eAutoSleep);
                prv_vUpdateTextAutoSleep(eAutoSleep);
                break;
                
            case GID_SETTINGS_BTN_SHUTDOWN:
                eAutoShutdown = AUTO_SHUTDOWN_10_MIN;
                
                vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SETTINGS_SLD_SHUTDOWN), (short)eAutoShutdown);
                prv_vUpdateTextAutoShutdown(eAutoShutdown);
                break;

            case GID_SETTINGS_BTN_SETID:
                vSetGuiPage(PAGE_SET_ID);
                break;

            case GID_SETTINGS_BTN_BLUETOOTH:
                // Take the Bluetooth mutex.
                xSemaphoreTake(xBluetoothMutex, portMAX_DELAY);
                
                // Show the message and reset Bluetooth in AT mode.
                vCreateGenericMessagePage("Resetting bluetooth module...");
                vResetBluetooth(AT);
                
                vSetGuiPage(PAGE_BT_SETTINGS);
                break;
                
            case GID_SETTINGS_BTN_PREV:
                prv_ucCurrentPage--;
                prv_vCreateSubpage();
                break;
                
            case GID_SETTINGS_BTN_NEXT:
                prv_ucCurrentPage++;
                prv_vCreateSubpage();
                break;

            case GID_BTN_EXIT:
                // Save the backlight value to file if it has been changed.
                if (BLight != prv_usPrevBacklight) {
                    usWriteConfig(pucBacklightFilePath, (void*)&BLight, sizeof(BLight));
                }

                // Save the volume value to file if it has been changed.
                if (ucVolume != prv_ucPrevVolume) {
                    usWriteConfig(pucVolumeFilePath, (void*)&ucVolume, sizeof(ucVolume));
                }
                
                // Save the auto sleep value to file if it has been changed.
                if (eAutoSleep != prv_ePrevAutoSleep) {
                    usWriteConfig(pucSleepFilePath, (void*)&eAutoSleep, sizeof(eAutoSleep));
                }
                
                // Save the volume value to file if it has been changed.
                if (eAutoShutdown != prv_ePrevAutoShutdown) {
                    usWriteConfig(pucShutdownFilePath, (void*)&eAutoShutdown, sizeof(eAutoShutdown));
                }
                
                // Reset the subpage to page 1.
                prv_ucCurrentPage = 1;

                vSetGuiPage(PAGE_MAIN_MENU);
                break;
        }
    }
    
    
    
    // Slider event.
    else {
        unsigned short usSliderPos;
        
        switch(GetObjID(pObj)) {
            case GID_SETTINGS_SLD_BLIGHT:
                BLight = SldGetPos(pObj);
                if (BLight < 200) {
                    BLight = 200;
                }
                SetDCOC1PWM(BLight);
                break;

            case GID_SETTINGS_SLD_VOLUME:
                usSliderPos = SldGetPos(pObj);
                if (ucVolume != (unsigned char)usSliderPos) {
                    ucVolume = (unsigned char)usSliderPos;
                    vSetVolume(ucVolume);
                }
                
                // Beep when it's released.
                if (pMsg->uiEvent == EVENT_RELEASE) {
                    vPlaySoundStream(&xTouchTone);
                }
                break;

            case GID_SETTINGS_SLD_SLEEP:
                usSliderPos = SldGetPos(pObj);
                if (eAutoSleep != (AUTO_SLEEP_SETTING)usSliderPos) {
                    eAutoSleep = (AUTO_SLEEP_SETTING)usSliderPos;
                    prv_vUpdateTextAutoSleep(eAutoSleep);
                }
                break;

            case GID_SETTINGS_SLD_SHUTDOWN:
                usSliderPos = SldGetPos(pObj);
                if (eAutoShutdown != (AUTO_SHUTDOWN_SETTING)usSliderPos) {
                    eAutoShutdown = (AUTO_SHUTDOWN_SETTING)usSliderPos;
                    prv_vUpdateTextAutoShutdown(eAutoShutdown);
                }
                break;
        }
    }
    

    return 1;
}

