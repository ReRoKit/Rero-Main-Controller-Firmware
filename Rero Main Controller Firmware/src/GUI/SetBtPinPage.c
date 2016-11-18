/*******************************************************************************
 * Functions for set bluetooth pin page in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include <string.h>
#include "Variables.h"
#include "GUI/SetBtPinPage.h"
#include "GUI/GraphicScreens.h"
#include "GUI/NumbericKeypad.h"
#include "Bluetooth/BluetoothControl.h"



/*******************************************************************************
 * FUNCTION: vCreateSetBtPinPage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the page to change the bluetooth pin.
 *
 *******************************************************************************/
void vCreateSetBtPinPage(void)
{
   vCreateNumericKeypadPage();
}



/*******************************************************************************
 * FUNCTION: usMsgSetBtPinPage
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
 * Handle the touchscreen event of the Set Bluetooth Pin page.
 *
 *******************************************************************************/
WORD usMsgSetBtPinPage(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    static char szOldPin[5];
    unsigned short usObjectId = GetObjID(pObj);
    EDITBOX *pxEditbox1 = (EDITBOX*)GOLFindObject(GID_KEYPAD_EB_VALUE1);
    EDITBOX *pxEditbox2 = (EDITBOX*)GOLFindObject(GID_KEYPAD_EB_VALUE2);
    
    // Process the keypad message.
    usMsgNumericKeypadPage(objMsg, pObj, pMsg);

    // Make sure button is released.
    if (objMsg == BTN_MSG_RELEASED) {
        switch (usObjectId) {
            // OK button.
            case GID_KEYPAD_OK:
                // Make sure the new pin has 4 character.
                if (strlen(EbGetText(pxEditbox2)) == 4) {
                    // Set the new pin.
                    if (ucSetBluetoothPin(EbGetText(pxEditbox2)) == 0) {
                        vEditBoxUpdateText(pxEditbox1, "Successful");
                    }
                    // If there is error, show it in editbox 1.
                    else {
                        vEditBoxUpdateText(pxEditbox1, "Failed");
                    }
                }
                else {
                    vEditBoxUpdateText(pxEditbox1, "Invalid");
                }


                // Clear the editbox 2.
                vEditBoxUpdateText(pxEditbox2, "");

                break;



            // Read PIN button.
            case GID_KEYPAD_BTN_READ:
                // Read the bluetooth pin.
                vGetBluetoothPin(szOldPin);

                // Make sure the length is correct.
                if (strlen(szOldPin) == 4) {
                    // Update the editbox 1.
                    vEditBoxUpdateText(pxEditbox1, szOldPin);
                }
                // Else, show error message.
                else {
                    // Update the editbox 1.
                    vEditBoxUpdateText(pxEditbox1, "Failed");
                }

                break;



            // Exit button.
            case GID_BTN_EXIT:
                // Reconfigure the bluetooth.
                vConfigureBluetooth();
                
                // Show the message and reset Bluetooth in TRANSPARENT mode.
                vCreateGenericMessagePage("Resetting bluetooth module...");
                vResetBluetooth(TRANSPARENT);

                // Release the Bluetooth mutex.
                xSemaphoreGive(xBluetoothMutex);

                vSetGuiPage(PAGE_SETTINGS);
                break;
        }

    }


    return 1;
}
