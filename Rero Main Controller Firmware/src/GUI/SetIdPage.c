/*******************************************************************************
 * Functions for set ID page in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include <stdlib.h>
#include "Variables.h"
#include "GUI/SetIdPage.h"
#include "GUI/GraphicScreens.h"
#include "GUI/NumbericKeypad.h"
#include "G15 and Sensors/ExtModule.h"



/*******************************************************************************
 * FUNCTION: vCreateSetIdPage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the Set ID page.
 *
 *******************************************************************************/
void vCreateSetIdPage(void)
{
    vCreateNumericKeypadPage();
}



/*******************************************************************************
 * FUNCTION: usMsgSetIdPage
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
 * Handle the touchscreen event of the Set ID page.
 *
 *******************************************************************************/
WORD usMsgSetIdPage(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    unsigned char ucTotalExtModule = 0;
    unsigned char i;

    unsigned short usNewExtModuleId;
    unsigned short usOldExtModuleId;
    unsigned short usObjectId = GetObjID(pObj);
    EDITBOX *pxEditbox1 = (EDITBOX*)GOLFindObject(GID_KEYPAD_EB_VALUE1);
    EDITBOX *pxEditbox2 = (EDITBOX*)GOLFindObject(GID_KEYPAD_EB_VALUE2);
    
    // Process the keypad message.
    usMsgNumericKeypadPage(objMsg, pObj, pMsg);

    // Make sure button is released.
    if (objMsg == BTN_MSG_RELEASED) {
        
        switch (usObjectId) {
            // OK or read button.
            case GID_KEYPAD_OK:
            case GID_KEYPAD_BTN_READ:
                // Get the total number of connected external module.
                ucTotalExtModule = 0;
                for (i = 0; i <= EM_MAX_ID; i++) {
                    if (eEMPing(i) == EM_NO_ERROR) {
                        usOldExtModuleId = i;
                        ucTotalExtModule++;
                    }
                }
                
                // No external module detected.
                if (ucTotalExtModule == 0) {
                    // Display message at editbox 1.
                    vEditBoxUpdateText(pxEditbox1, "Not Found");
                }

                // More than 1 external module detected.
                else if(ucTotalExtModule > 1) {
                    // Display message at editbox 1.
                    vEditBoxUpdateText(pxEditbox1, "Too many units");
                }

                // Only 1 external module detected.
                else {
                    // If OK button is pressed.
                    if (usObjectId == GID_KEYPAD_OK) {
                        // Convert the string in editbox 2 to int.
                        usNewExtModuleId = (unsigned short)atoi(EbGetText(pxEditbox2));

                        // If editbox 2 is blank or the value is out of range, show error message.
                        if ((*EbGetText(pxEditbox2) == 0) || (usNewExtModuleId > EM_MAX_ID)) {
                            vEditBoxUpdateText(pxEditbox1, "Invalid");
                        }
                        
                        else {
                            // Set the ID.
                            // Make sure it's successful.
                            if (eEMSetId((unsigned char)usOldExtModuleId, (unsigned char)usNewExtModuleId) == EM_NO_ERROR) {
                                if (eEMPing((unsigned char)usNewExtModuleId) == EM_NO_ERROR) {
                                    // Display message at editbox 1.
                                    vEditBoxUpdateText(pxEditbox1, "Successful");
                                }
                                else {
                                    vEditBoxUpdateText(pxEditbox1, "Failed");
                                }
                            }

                            // If failed, display message at editbox 1.
                            else {
                                vEditBoxUpdateText(pxEditbox1, "Failed");
                            }
                        }
                        
                        // Clear the editbox 2.
                        vEditBoxUpdateText(pxEditbox2, "");
                    }


                    // Read ID button is pressed.
                    else {
                        // Display the external module ID at editbox 1.
                        static char szExtModuleId[4];
                        utoa(szExtModuleId, usOldExtModuleId, 10);
                        vEditBoxUpdateText(pxEditbox1, szExtModuleId);
                    }

                }
                
                break;



            // Exit button.
            case GID_BTN_EXIT:
                vSetGuiPage(PAGE_SETTINGS);
                break;
        }
    }


    return 1;
}
