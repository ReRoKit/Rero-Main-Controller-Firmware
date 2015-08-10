/*******************************************************************************
 * Common functions for rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef GRAPHICSCREENS_H
#define	GRAPHICSCREENS_H

#include "Microchip/Graphics/Graphics.h"
#include "GUI/Scheme.h"



/*******************************************************************************
 * PUBLIC DEFINITION
 *******************************************************************************/

// Common page margin and limit for the window subpage.
#define WND_SUBPAGE_LEFT        3
#define WND_SUBPAGE_RIGHT       (GetMaxX() - 3)
#define WND_SUBPAGE_TOP         (WND_TITLE_HEIGHT + 4)
#define WND_SUBPAGE_BOTTOM      (GetMaxY() - 3)

#define WND_MARGIN_LEFT         5
#define WND_MARGIN_RIGHT        5
#define WND_MARGIN_TOP          5
#define WND_MARGIN_BOTTOM       5

#define WND_LIMIT_LEFT          (WND_SUBPAGE_LEFT + WND_MARGIN_LEFT)
#define WND_LIMIT_RIGHT         (WND_SUBPAGE_RIGHT - WND_MARGIN_RIGHT)
#define WND_LIMIT_TOP           (WND_SUBPAGE_TOP + WND_MARGIN_TOP)
#define WND_LIMIT_BOTTOM        (WND_SUBPAGE_BOTTOM - WND_MARGIN_BOTTOM)

// Position for the rero logo (splash screen).
#define RERO_LOGO_Y             40

// Dimension of the battery icon.
#define BATT_WIDTH              10
#define BATT_HEIGHT             22

// Radius for the button.
#define BTN_RADIUS              6



// Maximum graphic objects in one page.
#define MAX_GOB                 30



// Pages for GUI.
typedef enum __attribute__((packed)) {
    PAGE_MAIN_MENU,
    
    PAGE_BT_PROGRAM,
    PAGE_SERVO,
    PAGE_SENSOR,
    
    PAGE_TEACH,
    PAGE_PLAY,
    PAGE_BT_REMOTE,
    
    PAGE_WALLPAPER,
    PAGE_SETTINGS,
    PAGE_SET_ID,
    PAGE_BT_SETTINGS,
    PAGE_SET_BT_PIN,
    
    PAGE_USB,
    PAGE_USER_PROGRAM
} GUI_PAGE;



typedef enum __attribute__((packed)) {
    // Invalid GID.
    GID_INVALID,
    
    // General.
    GID_BATT_ICON,
    GID_WINDOW,
    GID_BTN_EXIT,


    // Main menu.
    GID_MAIN_BTN_RERO,
    GID_MAIN_BTN_BTPROGRAM,
    GID_MAIN_BTN_SERVO,
    GID_MAIN_BTN_SENSOR,
    GID_MAIN_BTN_TEACH,
    GID_MAIN_BTN_PLAY,
    GID_MAIN_BTN_REMOTE,
    GID_MAIN_BTN_SETTINGS,


    // Play page.
    GID_PLAY_IMG_LOCK,
    GID_PLAY_BTN_LOCK,
            
    GID_PLAY_RB_FILE1,
    GID_PLAY_RB_FILE2,
    GID_PLAY_RB_FILE3,
    GID_PLAY_RB_FILE4,
    GID_PLAY_RB_FILE5,

    GID_PLAY_TXT_MSG1,
    GID_PLAY_TXT_MSG2,
    GID_PLAY_TXT_MSG3,
    GID_PLAY_TXT_FRAME,
    GID_PLAY_TXT_TIME,

    GID_PLAY_DM_FRAME,
    GID_PLAY_DM_TIME,

    GID_PLAY_BTN_PLAY,
    GID_PLAY_BTN_STOP,


    // Teach page.
    GID_TEACH_IMG_LOCK1,
    GID_TEACH_IMG_LOCK2,
    GID_TEACH_IMG_LOCK3,
    GID_TEACH_IMG_LOCK4,
    GID_TEACH_IMG_LOCK5,
            
    GID_TEACH_RB_FILE1,
    GID_TEACH_RB_FILE2,
    GID_TEACH_RB_FILE3,
    GID_TEACH_RB_FILE4,
    GID_TEACH_RB_FILE5,

    GID_TEACH_TXT_MSG1,
    GID_TEACH_TXT_MSG2,

    GID_TEACH_TXT_FRAME,
    GID_TEACH_DM_FRAME,

    GID_TEACH_BTN_TEACH,
    GID_TEACH_BTN_NEXT,
    GID_TEACH_BTN_STOP,
            
            
            // Motion page.
            GID_MOTION_GAMEPAD,
            
            GID_MOTION_RB_XU,
            GID_MOTION_RB_XD,
            GID_MOTION_RB_XL,
            GID_MOTION_RB_XR,
            
            GID_MOTION_RB_CU,
            GID_MOTION_RB_CD,
            GID_MOTION_RB_CL,
            GID_MOTION_RB_CR,
            
            GID_MOTION_RB_F1,
            GID_MOTION_RB_F2,
            GID_MOTION_RB_F3,
            GID_MOTION_RB_F4,
            GID_MOTION_RB_F5,


    // Common objects for Servo and Sensor page.
    GID_EXTMODULE_TXT_MSG,
    GID_EXTMODULE_TXT_LBTITLE,
    GID_EXTMODULE_LB,
    GID_EXTMODULE_SLD,
    GID_EXTMODULE_BTN_UP,
    GID_EXTMODULE_BTN_DOWN,
    GID_EXTMODULE_BTN_REFRESH,


    // Servo page.
    GID_G15_MTR,
    GID_G15_DM,
    GID_G15_IMG_WHEEL,
    GID_G15_BTN_INC,
    GID_G15_BTN_DEC,
    GID_G15_BTN_SETPATH,
    GID_G15_TXT_MODE,
    GID_G15_RB_POS,
    GID_G15_RB_WHEEL,
    GID_G15_BTN_NEXT,
    GID_G15_BTN_SAVE,
    GID_G15_BTN_CANCEL,


    // Sensor page.
    GID_SENSOR_TXT_TYPE,
            
    GID_SENSOR_SLD_VALUE1,
    GID_SENSOR_TXT_VALUE1,
    GID_SENSOR_TXT_TITLE1,
            
    GID_SENSOR_SLD_VALUE2,
    GID_SENSOR_TXT_VALUE2,
    GID_SENSOR_TXT_TITLE2,
            
    GID_SENSOR_SLD_VALUE3,
    GID_SENSOR_TXT_VALUE3,
    GID_SENSOR_TXT_TITLE3,
            
    GID_SENSOR_SLD_VALUE4,
    GID_SENSOR_TXT_VALUE4,
    GID_SENSOR_TXT_TITLE4,
            
    GID_SENSOR_SLD_VALUE5,
    GID_SENSOR_TXT_VALUE5,
            
    GID_SENSOR_SLD_VALUE6,
    GID_SENSOR_TXT_VALUE6,
            
    GID_SENSOR_TXT_LED,
    GID_SENSOR_CB_R,
    GID_SENSOR_CB_G,
    GID_SENSOR_CB_B,

    GID_SENSOR_TXT_COLOUR1,
    GID_SENSOR_TXT_COLOUR2,
            
    GID_SENSOR_TXT_LINE,
    GID_SENSOR_RB_DARK,
    GID_SENSOR_RB_LIGHT,
            
    GID_SENSOR_BTN_CAL,
    GID_SENSOR_BTN_DONE,


    // Bluetooth settings page.
    GID_BTSETTINGS_TXT_BTMODE,
    GID_BTSETTINGS_RB_EDR,
    GID_BTSETTINGS_RB_BLE,
    GID_BTSETTINGS_BTN_SETBTPIN,


    // Numeric keypad page.
    GID_KEYPAD_TXT_TITLE1,
    GID_KEYPAD_TXT_TITLE2,
    GID_KEYPAD_TXT_RANGE,
    GID_KEYPAD_EB_VALUE1,
    GID_KEYPAD_EB_VALUE2,
    GID_KEYPAD_BTN_READ,
            
    GID_KEYPAD_0,
    GID_KEYPAD_1,
    GID_KEYPAD_2,
    GID_KEYPAD_3,
    GID_KEYPAD_4,
    GID_KEYPAD_5,
    GID_KEYPAD_6,
    GID_KEYPAD_7,
    GID_KEYPAD_8,
    GID_KEYPAD_9,
    GID_KEYPAD_DEL,
    GID_KEYPAD_OK,


    // Settings page.
    GID_SETTINGS_BTN_PREV,
    GID_SETTINGS_BTN_NEXT,
    GID_SETTINGS_TXT_PAGE,
            
    // Page 1.
    GID_SETTINGS_BTN_SETID,
    GID_SETTINGS_BTN_BLUETOOTH,

    // Page 2.
    GID_SETTINGS_BTN_BLIGHT,
    GID_SETTINGS_TXT_BLIGHT,
    GID_SETTINGS_SLD_BLIGHT,
    
    GID_SETTINGS_BTN_VOLUME,
    GID_SETTINGS_TXT_VOLUME,
    GID_SETTINGS_SLD_VOLUME,
            
    // Page 3.
    GID_SETTINGS_BTN_SLEEP,
    GID_SETTINGS_TXT_SLEEP,
    GID_SETTINGS_SLD_SLEEP,
    
    GID_SETTINGS_BTN_SHUTDOWN,
    GID_SETTINGS_TXT_SHUTDOWN,
    GID_SETTINGS_SLD_SHUTDOWN,
    
    // Page 4.
    GID_SETTINGS_IMG_RERO,
    GID_SETTINGS_TXT_FIRMWARE,
    GID_SETTINGS_TXT_DATE,
    

    // Bluetooth page.
    GID_BT_IMG_LOGO,
    GID_BT_TXT_NAME1,
    GID_BT_TXT_NAME2,
    GID_BT_TXT_ADDR1,
    GID_BT_TXT_ADDR2,


    // USB page.
    GID_USB_IMG_RERO,
    GID_USB_IMG_USB,
    GID_USB_BTN_MSD,


    // Wallpaper page.
    GID_IMG_WALLPAPPER,


    // Lock screen.
    GID_LOCKSCREEN_SLD_UNLOCK,
    GID_LOCKSCREEN_TXT_RERO,
    GID_LOCKSCREEN_TXT_SLIDE
} GOB_ID;



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

void vDisplayLoadingText(char *szText, unsigned char ucLine);
void vDisplayShutdownText(char *szText);
void vShowStartScreen(void);
void vClearScreenWithFadeEffect(void);

void vCreatePageWindow(char *szTitle);
void vClearWindowSubpage(void);
void vGraphicsObjectEnable(void *pxObject);
void vGraphicsObjectDisable(void *pxObject);
void vGraphicsObjectUnhide(void *pxObject);
void vGraphicsObjectHide(void *pxObject);
void vStaticTextUpdateText(STATICTEXT *pxStaticText, char *szText);
void vEditBoxUpdateText(EDITBOX *pxEditBox, char *szText);
void vSliderUpdatePosition(SLIDER *pxSlider, short sNewPosition);

void vSetGuiPage(GUI_PAGE eGuiPage);
GUI_PAGE eGetGuiPage(void);
void vLockScreen(void);
void vUnlockScreen(void);

void taskGraphicDisplay(void *pvParameters );
WORD GOLDrawCallback(void);
WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);



#endif	/* GRAPHICSCREENS_H */
