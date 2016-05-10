/*******************************************************************************
 * Functions for Bluetooth settings page in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "GUI/BtSettingsPage.h"
#include "GUI/GraphicScreens.h"
#include "Variables.h"
#include "Bluetooth/BluetoothControl.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

// Position and size for graphic objects.
// All objects in this page are aligned.
// Common position and size for the objects.
#define OBJ_WIDTH               200
#define OBJ_X1                  ((WND_LIMIT_LEFT + WND_LIMIT_RIGHT) / 2 - (OBJ_WIDTH / 2))
#define OBJ_X2                  (OBJ_X1 + OBJ_WIDTH)


// Title.
#define TXT_TITLE_HEIGHT        20
#define TXT_TITLE_X1            OBJ_X1
#define TXT_TITLE_X2            OBJ_X2
#define TXT_TITLE_Y1            (WND_LIMIT_TOP + 10)
#define TXT_TITLE_Y2            (TXT_TITLE_Y1 + TXT_TITLE_HEIGHT)


// Radio button.
#define RB_HEIGHT               26

#define RB_EDR_X1               OBJ_X1
#define RB_EDR_X2               OBJ_X2
#define RB_EDR_Y1               (TXT_TITLE_Y2 + 10)
#define RB_EDR_Y2               (RB_EDR_Y1 + RB_HEIGHT)

#define RB_BLE_X1               OBJ_X1
#define RB_BLE_X2               OBJ_X2
#define RB_BLE_Y1               (RB_EDR_Y2 + 10)
#define RB_BLE_Y2               (RB_BLE_Y1 + RB_HEIGHT)


// Button for set bluetooth pin.
#define BTN_BTPIN_HEIGHT        35
#define BTN_BTPIN_X1            OBJ_X1
#define BTN_BTPIN_X2            OBJ_X2
#define BTN_BTPIN_Y1            (RB_BLE_Y2 + 30)
#define BTN_BTPIN_Y2            (BTN_BTPIN_Y1 + BTN_BTPIN_HEIGHT)



/*******************************************************************************
* PRIVATE GLOBAL VARIABLES
*******************************************************************************/

// Initial settings.
static BLUETOOTH_MODE prv_ePrevBt4Mode;



/*******************************************************************************
 * FUNCTION: vCreateBtSettingsPage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the Bluetooth settings page.
 *
 *******************************************************************************/
void vCreateBtSettingsPage(void)
{
    GOLFree();
    vCreatePageWindow("BLUETOOTH SETTINGS");


    // Create the title.
    StCreate( GID_BTSETTINGS_TXT_BTMODE,
              TXT_TITLE_X1, TXT_TITLE_Y1,
              TXT_TITLE_X2, TXT_TITLE_Y2,
              ST_DRAW, "Bluetooth Mode :", pxLightBlueTxtScheme );

    // Create the radio buttons for Bluetooth EDR or BLE mode.
    RADIOBUTTON *pxRbEdr = RbCreate( GID_BTSETTINGS_RB_EDR,
                                     RB_EDR_X1, RB_EDR_Y1,
                                     RB_EDR_X2, RB_EDR_Y2,
                                     RB_DRAW | RB_GROUP, " EDR (Android ,  Windows)", pxRadioBtnScheme );

    RADIOBUTTON *pxRbBle = RbCreate( GID_BTSETTINGS_RB_BLE,
                                     RB_BLE_X1, RB_BLE_Y1,
                                     RB_BLE_X2, RB_BLE_Y2,
                                     RB_DRAW, " BLE (Android ,  iOS)", pxRadioBtnScheme );

    // Create button "Set Bluetooth PIN".
    BtnCreate( GID_BTSETTINGS_BTN_SETBTPIN,
               BTN_BTPIN_X1, BTN_BTPIN_Y1,
               BTN_BTPIN_X2, BTN_BTPIN_Y2,
               BTN_RADIUS, BTN_DRAW, NULL, "Set Bluetooth PIN", pxBtnScheme );
    
    
    
    // Save the initial settings.
    prv_ePrevBt4Mode = eBt4Mode;

    // Bluetooth v2.0 only supports EDR mode.
    // Disable the radio button for BLE and select the EDR.
    if (eGetBluetoothVersion() == BT_V20) {
        SetState(pxRbEdr, RB_CHECKED | RB_DRAW);
        vGraphicsObjectDisable(pxRbBle);
        
        // Set the Bluetooth mode as EDR.
        eBt4Mode = EDR;
    }

    // Bluetooth v4.0 supports both EDR and BLE mode.
    // Select the radio button according to the Bluetooth mode.
    else {
        switch (eBt4Mode) {
            case EDR:   SetState(pxRbEdr, RB_CHECKED | RB_DRAW);    break;
            case BLE:   SetState(pxRbBle, RB_CHECKED | RB_DRAW);    break;
        }
    }
}



/*******************************************************************************
 * FUNCTION: usMsgBtSettingsPage
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
 * Handle the touchscreen event of the Bluetooth Settings page.
 *
 *******************************************************************************/
WORD usMsgBtSettingsPage(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    // When the button is released...
    if (objMsg == BTN_MSG_RELEASED) {
        switch (GetObjID(pObj)) {
            case GID_BTSETTINGS_BTN_SETBTPIN:
                vSetGuiPage(PAGE_SET_BT_PIN);
                break;

            case GID_BTN_EXIT:
                // Save the bluetooth 4.0 mode value to file if it has been changed.
                if (eBt4Mode != prv_ePrevBt4Mode) {
                    usWriteConfig(pucBt4ModeFilePath, (void*)&eBt4Mode, sizeof(eBt4Mode));
                }
                
                // Reconfigure the bluetooth.
                vConfigureBluetooth();
                
                // Show the message and reset Bluetooth in TRANSPARENT mode.
                vCreateGenericMessagePage("Resetting bluetooth module...");
                vResetBluetooth(TRANSPARENT);

                // Release the Bluetooth mutex.
                xSemaphoreGive(xBluetoothMutex);

                vSetGuiPage(PAGE_SETTINGS);
                break;

            default:
                break;
        }
    }
    
    // When the radio button is checked...
    else if (objMsg == RB_MSG_CHECKED) {
        switch (GetObjID(pObj)) {
            case GID_BTSETTINGS_RB_EDR: eBt4Mode = EDR; break;
            case GID_BTSETTINGS_RB_BLE: eBt4Mode = BLE; break;
        }
    }
    
    return 1;
}
