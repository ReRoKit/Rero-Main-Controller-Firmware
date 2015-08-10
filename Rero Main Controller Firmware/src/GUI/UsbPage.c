/*******************************************************************************
 * Functions for USB page in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "Variables.h"
#include "GUI/UsbPage.h"
#include "GUI/GraphicScreens.h"
#include "Flash Image/SplashScreenLogo.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

// Position and size for graphic objects.
// USB logo.
#define IMG_USB_WIDTH       76
#define IMG_USB_HEIGHT      34
#define IMG_USB_L           ((GetMaxX() - IMG_USB_WIDTH) / 2)
#define IMG_USB_R           (IMG_USB_L + IMG_USB_WIDTH)
#define IMG_USB_T           125
#define IMG_USB_B           (IMG_USB_T + IMG_USB_HEIGHT)

// USB mass storage button.
#define BTN_WIDTH           180
#define BTN_HEIGHT          40
#define BTN_L               ((GetMaxX() - BTN_WIDTH) / 2)
#define BTN_R               (BTN_L + BTN_WIDTH)
#define BTN_T               (IMG_USB_B + 20)
#define BTN_B               (BTN_T + BTN_HEIGHT)



/*******************************************************************************
 * FUNCTION: vCreateUsbPage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the USB page.
 *
 *******************************************************************************/
void vCreateUsbPage(void)
{
    // Clear screen.
    GOLFree();
    SetColor(pxDefaultScheme->CommonBkColor);
    ClearDevice();


    // Create the rero logo.
    PictCreate( GID_USB_IMG_RERO,
                0,         RERO_LOGO_Y,
                GetMaxX(), RERO_LOGO_Y + SplashScreenLogo_HEIGHT,
                PICT_DRAW, 1, (void*)&SplashScreenLogo, pxDefaultScheme );

    // Create the USB icon.
    PictCreate( GID_USB_IMG_USB,
                IMG_USB_L, IMG_USB_T,
                IMG_USB_R, IMG_USB_B,
                PICT_HIDE | PICT_DRAW, 1, "/theme/icon/usb.ico", pxDefaultScheme );

    // Create the mass storage button.
    BtnCreate( GID_USB_BTN_MSD,
               BTN_L, BTN_T,
               BTN_R, BTN_B,
               BTN_RADIUS, BTN_HIDE, NULL, "Turn on USB storage", pxBtnScheme );
}



/*******************************************************************************
 * FUNCTION: vDisplayUsbPage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Update the graphic object state depending on the USB and mass storage state.
 * This function is to be called periodically in USB page.
 *
 *******************************************************************************/
void vDisplayUsbPage(void)
{
    OBJ_HEADER *pObj;
    static unsigned char ucPrevUsbState = 0;
    static unsigned char ucPrevMassStorageState = 0;


    // Unhide the USB icon and button after USB is connected.
    // Only do this 1 time.
    if((xSystemState.bUsbConnected == 1) && (ucPrevUsbState == 0)) {
        OBJ_HEADER *pObj;

        pObj = GOLFindObject(GID_USB_IMG_USB);
        ClrState(pObj, PICT_HIDE);
        SetState(pObj, PICT_DRAW);

        pObj = GOLFindObject(GID_USB_BTN_MSD);
        ClrState(pObj, BTN_HIDE);
        SetState(pObj, BTN_DRAW);

        ucPrevUsbState = 1;
    }

    // If the mass storage is turned on/off...
    if(ucPrevMassStorageState != xSystemState.bUsbMassStorage) {
        pObj = GOLFindObject(GID_USB_BTN_MSD);

        // Mass storage enabled.
        if (xSystemState.bUsbMassStorage == 1) {
            // Change the button text, disable the button and redraw.
            BtnSetText((BUTTON*)pObj, "USB storage turned on");
            vGraphicsObjectDisable(pObj);
        }
        // Mass storage disabled.
        else {
            // Change the button text, enable the button and redraw.
            BtnSetText((BUTTON*)pObj, "Turn on USB storage");
            vGraphicsObjectEnable(pObj);
        }

        ucPrevMassStorageState = xSystemState.bUsbMassStorage;
    }
}



/*******************************************************************************
 * FUNCTION: usMsgUsbPage
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
 * Handle the touchscreen event of the USB page.
 *
 *******************************************************************************/
WORD usMsgUsbPage(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    // Enable Mass Storage Device.
    if(GetObjID(pObj) == GID_USB_BTN_MSD) {
        if(objMsg == BTN_MSG_RELEASED) {
            xSystemState.bUsbMassStorage = 1;
        }
    }

    return 1;
}
