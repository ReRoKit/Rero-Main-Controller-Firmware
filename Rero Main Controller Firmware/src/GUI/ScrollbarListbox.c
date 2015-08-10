/*******************************************************************************
 * Functions for listbox object with scroll bar.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "HardwareProfile.h"
#include "Variables.h"
#include "Log.h"
#include "G15 and Sensors/Sensors.h"
#include "G15 and Sensors/G15.h"
#include "GUI/GraphicScreens.h"
#include "GUI/ScrollbarListbox.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

// Position and size for graphic objects.
#define TXT_HEIGHT          20

// Title.
#define TXT_TITLE_WIDTH     80
#define TXT_TITLE_L         15
#define TXT_TITLE_R         (TXT_TITLE_L + TXT_TITLE_WIDTH)
#define TXT_TITLE_T         WND_SUBPAGE_TOP
#define TXT_TITLE_B         (TXT_TITLE_T + TXT_HEIGHT)

// List box.
#define LB_WIDTH            50
#define LB_HEIGHT           120
#define LB_L                TXT_TITLE_L
#define LB_R                (LB_L + LB_WIDTH)
#define LB_T                (TXT_TITLE_B + 4)
#define LB_B                (LB_T + LB_HEIGHT)

// Up down button width and height.
#define BTN_SLD_WIDTH        30
#define BTN_SLD_HEIGHT       30

// Up button.
#define BTN_UP_L            LB_R
#define BTN_UP_R            (BTN_UP_L + BTN_SLD_WIDTH)
#define BTN_UP_T            LB_T
#define BTN_UP_B            (BTN_UP_T + BTN_SLD_HEIGHT)

// Slider.
#define SLD_WIDTH           BTN_SLD_WIDTH
#define SLD_HEIGHT          (LB_HEIGHT - (BTN_SLD_HEIGHT * 2))
#define SLD_L               LB_R
#define SLD_R               (SLD_L + SLD_WIDTH)
#define SLD_T               BTN_UP_B
#define SLD_B               (SLD_T + SLD_HEIGHT)

// Down button.
#define BTN_DOWN_L          LB_R
#define BTN_DOWN_R          (BTN_DOWN_L + BTN_SLD_WIDTH)
#define BTN_DOWN_T          SLD_B
#define BTN_DOWN_B          (BTN_DOWN_T + BTN_SLD_HEIGHT)

// Refresh button.
#define BTN_REFRESH_HEIGHT  30
#define BTN_REFRESH_L       LB_L
#define BTN_REFRESH_R       SLD_R
#define BTN_REFRESH_T       (LB_B + 10)
#define BTN_REFRESH_B       (BTN_REFRESH_T + BTN_REFRESH_HEIGHT)

// Message text.
#define TXT_MSG_L           (SLD_R + 30)
#define TXT_MSG_R           WND_LIMIT_RIGHT
#define TXT_MSG_T           LB_T
#define TXT_MSG_B           (TXT_MSG_T + TXT_HEIGHT)



/*******************************************************************************
 * FUNCTION: vCreateListboxExtModule
 *
 * PARAMETERS:
 * ~ szTitle    - Title for the list box.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the list box for external module together with the title, slider and
 * refresh button. Also create a "Searching..." text which will be removed
 * after the search complete.
 *
 *******************************************************************************/
void vCreateListboxExtModule(const char *szTitle)
{
    // Title of the list box.
    StCreate( GID_EXTMODULE_TXT_LBTITLE,
              TXT_TITLE_L, TXT_TITLE_T,
              TXT_TITLE_R, TXT_TITLE_B,
              ST_DRAW, (char*)szTitle, pxDefaultScheme );

    // List box.
    LbCreate( GID_EXTMODULE_LB,
              LB_L, LB_T,
              LB_R, LB_B,
              LB_DRAW | LB_SINGLE_SEL | LB_DISABLED, NULL, pxListboxScheme );

    
    
    // Scroll bar for the list box.
    SldCreate( GID_EXTMODULE_SLD,
               SLD_L, SLD_T,
               SLD_R, SLD_B,
               SLD_DRAW | SLD_SCROLLBAR | SLD_VERTICAL | SLD_DISABLED, 0, 1, 0, pxListboxScheme );

    // Up button for the scroll bar.
    BtnCreate( GID_EXTMODULE_BTN_UP,
               BTN_UP_L, BTN_UP_T,
               BTN_UP_R, BTN_UP_B,
               1, BTN_DRAW | BTN_DISABLED, NULL, "^", pxTransBtnScheme );

    // Down button for the scroll bar.
    BtnCreate( GID_EXTMODULE_BTN_DOWN,
               BTN_DOWN_L, BTN_DOWN_T,
               BTN_DOWN_R, BTN_DOWN_B,
               1, BTN_DRAW | BTN_DISABLED, NULL, "v", pxTransBtnScheme );


    
    // Refresh button.
    BtnCreate( GID_EXTMODULE_BTN_REFRESH,
               BTN_REFRESH_L, BTN_REFRESH_T,
               BTN_REFRESH_R, BTN_REFRESH_B,
               BTN_RADIUS, BTN_DRAW | BTN_DISABLED, NULL, "Refresh", pxTransBtnScheme );

    // Message text.
    StCreate( GID_EXTMODULE_TXT_MSG,
              TXT_MSG_L, TXT_MSG_T,
              TXT_MSG_R, TXT_MSG_B,
              ST_DRAW, "Searching. Please wait...", pxDefaultScheme );
}



/*******************************************************************************
 * FUNCTION: vEnableListboxExtModule
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Enable the list box, slider, up down button and refresh button for external
 * module.
 *
 *******************************************************************************/
void vEnableListboxExtModule(void)
{
    vGraphicsObjectEnable(GOLFindObject(GID_EXTMODULE_LB));
    vGraphicsObjectEnable(GOLFindObject(GID_EXTMODULE_SLD));
    vGraphicsObjectEnable(GOLFindObject(GID_EXTMODULE_BTN_UP));
    vGraphicsObjectEnable(GOLFindObject(GID_EXTMODULE_BTN_DOWN));
    vGraphicsObjectEnable(GOLFindObject(GID_EXTMODULE_BTN_REFRESH));
}



/*******************************************************************************
 * FUNCTION: vDisableListboxExtModule
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Disable the list box, slider, up down button and refresh button for external
 * module.
 *
 *******************************************************************************/
void vDisableListboxExtModule(void)
{
    vGraphicsObjectDisable(GOLFindObject(GID_EXTMODULE_LB));
    vGraphicsObjectDisable(GOLFindObject(GID_EXTMODULE_SLD));
    vGraphicsObjectDisable(GOLFindObject(GID_EXTMODULE_BTN_UP));
    vGraphicsObjectDisable(GOLFindObject(GID_EXTMODULE_BTN_DOWN));
    vGraphicsObjectDisable(GOLFindObject(GID_EXTMODULE_BTN_REFRESH));
}



/*******************************************************************************
 * FUNCTION: pxListboxExtModuleSelectNextItem
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ Pointer to the next selected item.
 * ~ Return now if current item is the last item.
 *
 * DESCRIPTIONS:
 * Select the next item in the list box and return the pointer of the item.
 *
 *******************************************************************************/
LISTITEM *pxListboxExtModuleSelectNextItem(void)
{
    LISTITEM *pxNextItem = NULL;
    
     // Get the next selection from the list box.
    LISTBOX *pxListBox = (LISTBOX*)GOLFindObject(GID_EXTMODULE_LB);
    if (pxListBox != NULL) {
        // Get the selected item in the list box.
        LISTITEM *pxCurrentItem = LbGetSel(pxListBox, NULL);
        if(pxCurrentItem != NULL) {
            // Make sure this is not the last item in the list.
            if (pxCurrentItem->pNextItem != NULL) {
                // Deselect the current item and select the next item.
                LbClrSel(pxListBox, pxCurrentItem);
                pxNextItem = (LISTITEM*)pxCurrentItem->pNextItem;
                LbSetSel(pxListBox, pxNextItem);

                // Set the focus to the next item as well.
                LbSetFocusedItem(pxListBox, LbGetFocusedItem(pxListBox) + 1);
                SetState(pxListBox, LB_DRAW_ITEMS);

                // Update the slider position.
                SLIDER *pxSlider = (SLIDER*)GOLFindObject(GID_EXTMODULE_SLD);
                if (pxSlider != NULL) {
                    vSliderUpdatePosition(pxSlider, SldGetPos(pxSlider) - 1);
                }
            }
        }
    }
    
    return pxNextItem;
}



/*******************************************************************************
 * FUNCTION: pxListboxExtModuleSelectPreviousItem
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ Pointer to the previous selected item.
 * ~ Return now if current item is the first item.
 *
 * DESCRIPTIONS:
 * Select the previous item in the list box and return the pointer of the item.
 *
 *******************************************************************************/
LISTITEM *pxListboxExtModuleSelectPreviousItem(void)
{
    LISTITEM *pxPreviousItem = NULL;
    
     // Get the previous selection from the list box.
    LISTBOX *pxListBox = (LISTBOX*)GOLFindObject(GID_EXTMODULE_LB);
    if (pxListBox != NULL) {
        // Get the selected item in the list box.
        LISTITEM *pxCurrentItem = LbGetSel(pxListBox, NULL);
        if(pxCurrentItem != NULL) {
            // Make sure this is not the first item in the list.
            if (pxCurrentItem->pPrevItem != NULL) {
                // Deselect the current item and select the next item.
                LbClrSel(pxListBox, pxCurrentItem);
                pxPreviousItem = (LISTITEM*)pxCurrentItem->pPrevItem;
                LbSetSel(pxListBox, pxPreviousItem);

                // Set the focus to the previous item as well.
                LbSetFocusedItem(pxListBox, LbGetFocusedItem(pxListBox) - 1);
                SetState(pxListBox, LB_DRAW_ITEMS);

                // Update the slider position.
                SLIDER *pxSlider = (SLIDER*)GOLFindObject(GID_EXTMODULE_SLD);
                if (pxSlider != NULL) {
                    vSliderUpdatePosition(pxSlider, SldGetPos(pxSlider) + 1);
                }
            }
        }
    }
    
    return pxPreviousItem;
}



/*******************************************************************************
 * FUNCTION: vClearExtModuleSubpage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Clear the subpage of the external module page (G15 and Sensor Page).
 *
 *******************************************************************************/
void vClearExtModuleSubpage(void)
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
            
            case GID_EXTMODULE_TXT_LBTITLE:
            case GID_EXTMODULE_LB:
            case GID_EXTMODULE_SLD:
            case GID_EXTMODULE_BTN_UP:
            case GID_EXTMODULE_BTN_DOWN:
                
            case GID_EXTMODULE_BTN_REFRESH:
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
    Bar(EM_SUBPAGE_LEFT, EM_SUBPAGE_TOP, EM_SUBPAGE_RIGHT, EM_SUBPAGE_BOTTOM);
}



/*******************************************************************************
 * FUNCTION: usMsgListboxExtModule
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
 * Handle the touchscreen event of the external module list box.
 *
 *******************************************************************************/
WORD usMsgListboxExtModule(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    LISTBOX *pxListbox = (LISTBOX*)GOLFindObject(GID_EXTMODULE_LB);
    SLIDER *pxSlider = (SLIDER*)GOLFindObject(GID_EXTMODULE_SLD);

    switch(GetObjID(pObj)) {

        // List box.
        case GID_EXTMODULE_LB:
            if (pMsg->uiEvent == EVENT_RELEASE) {
                // Update the slider position.
                vSliderUpdatePosition(pxSlider, LbGetCount(pxListbox) - LbGetFocusedItem(pxListbox) - 1);
            }
            break;

        // Slider.
        case GID_EXTMODULE_SLD:
            if((objMsg == SLD_MSG_INC) || (objMsg == SLD_MSG_DEC)) {
                // Set new list box position.
                LbSetFocusedItem(pxListbox, LbGetCount(pxListbox) - SldGetPos(pxSlider) - 1);
                SetState(pxListbox, LB_DRAW_ITEMS);
                ClrState(pxListbox, FOCUSED);
            }
            break;

        // Up button for the slider.
        case GID_EXTMODULE_BTN_UP:
            if(objMsg == BTN_MSG_PRESSED) {
                LbSetFocusedItem(pxListbox, LbGetFocusedItem(pxListbox) - 1);
                SetState(pxListbox, LB_DRAW_ITEMS);

                pxSlider = (SLIDER *)GOLFindObject(GID_EXTMODULE_SLD);
                vSliderUpdatePosition(pxSlider, SldGetPos(pxSlider) + 1);
            }
            break;

        // Down button for the slider.
        case GID_EXTMODULE_BTN_DOWN:
            if(objMsg == BTN_MSG_PRESSED) {
                LbSetFocusedItem(pxListbox, LbGetFocusedItem(pxListbox) + 1);
                SetState(pxListbox, LB_DRAW_ITEMS);

                pxSlider = (SLIDER *)GOLFindObject(GID_EXTMODULE_SLD);
                vSliderUpdatePosition(pxSlider, SldGetPos(pxSlider) - 1);
            }
            break;
    }

    return 1;
}


    
/*******************************************************************************
 * RTOS TASK: taskSeekExtModule
 *
 * PARAMETERS:
 * ~ pvParameters   - Pointer to the type of external module we are looking for.
 *                  - The data type is LB_FILTER.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Task to look for the connected external modules and add them to the listbox.
 *
 *******************************************************************************/
void taskSeekExtModule(void *pvParameters)
{
    const unsigned char ucTextLength = 3;
    
    LB_FILTER eListboxFilter = *((LB_FILTER*)pvParameters);

    LISTBOX *pxListbox = (LISTBOX*)GOLFindObject(GID_EXTMODULE_LB);
    SLIDER *pxSlider = (SLIDER*)GOLFindObject(GID_EXTMODULE_SLD);
    STATICTEXT *pxMsgText = (STATICTEXT*)GOLFindObject(GID_EXTMODULE_TXT_MSG);
    
    // Make sure the listbox components are available.
    if ( (pxListbox == NULL) || (pxSlider == NULL) || (pxMsgText == NULL) ) {
        vTaskDelete(NULL);
    }

    // Delete all previous list items.
    LbDelItemsList(pxListbox);

    // Search for external module.
    unsigned char ucId;
    for(ucId = 0; ucId <= EM_MAX_ID; ucId++) {

        // Make sure the module exists.
        EM_MODEL eModel;
        EM_ERROR eErrorCode = eEMGetModel(ucId, &eModel);
        if (eErrorCode == EM_NO_ERROR) {
            // Make sure the model is what we want.
            unsigned char ucFilterMatched = 0;
            switch (eListboxFilter) {
                case LB_FILTER_ALL:
                    ucFilterMatched = 1;
                    break;
                
                case LB_FILTER_SERVO:
                    switch (eModel) {
                        case EM_MODEL_G15:
                            ucFilterMatched = 1;
                            break;
                    }
                    break;
                
                case LB_FILTER_SENSOR:
                    switch (eModel) {
                        case EM_MODEL_IR:
                        case EM_MODEL_US:
                        case EM_MODEL_HEAD:
                        case EM_MODEL_TACTILE:
                        case EM_MODEL_COLOUR:
                        case EM_MODEL_LINE:
                            ucFilterMatched = 1;
                            break;
                    }
                    break;
            }
            
            if (ucFilterMatched == 1) {
                // Turn off LED for detected module.
                // Also turn off the torque and set the speed to 0 for servo.
                switch (eModel) {
                    case EM_MODEL_IR:
                    case EM_MODEL_US:
                        eSensorSetLed(ucId, eModel, 0);
                        break;
                    case EM_MODEL_HEAD:
                        eHeadSetLed(ucId, 0, 0b11);
                        break;
                    case EM_MODEL_TACTILE:
                    case EM_MODEL_COLOUR:
                    case EM_MODEL_LINE:
                        eSensorSetLed(ucId, eModel, 0);
                        vTaskDelay(10 / portTICK_RATE_MS);
                        eSensorSetLed(ucId, eModel, 2);
                        break;
                    case EM_MODEL_G15:
                        //Turn off Torque n LED, set speed to 0.
                        eG15SetTorqueLed(ucId, WRITE_NOW, 0, 0);
                        eG15SetSpeed(ucId, WRITE_NOW, 0, POSITION_SPEED_CONTROL);
                        break;
                }

                // Create a string for the ID.
                char *szText = malloc(ucTextLength + 1);
                if (szText != NULL) {
                    // Convert the ID into string.
                    snprintf(szText, ucTextLength + 1, "%03u", ucId);

                    // Add to listbox.
                    LbAddItem(pxListbox, NULL, szText, NULL, 0, ucId);
                }
                else {
                    xSystemError.bMallocError = 1;

                    // Log the malloc error to SD card.
                    vLogMallocError("Seek External Module");
                }
            }
        }
    }


#ifdef ENABLE_IMU

    // Add IMU to the list box.
    if ((eListboxFilter == LB_FILTER_ALL) || (eListboxFilter == LB_FILTER_SENSOR)) {
        // Create a string for the ID.
        char *szText = malloc(ucTextLength + 1);
        if (szText != NULL) {
            szText[0] = 'I';
            szText[1] = 'M';
            szText[2] = 'U';
            szText[3] = 0;

            LbAddItem(pxListbox, NULL, szText, NULL, 0, 255);
        }
        else {
            xSystemError.bMallocError = 1;

            // Log the malloc error to SD card.
            vLogMallocError("Adding IMU to list box");
        }
    }

#endif


    // Select the first item.
    pxListbox->pFocusItem = pxListbox->pItemList;
    SetState(pxListbox, LB_DRAW);

    // Set new scrollbar (slider) range.
    SldSetRange(pxSlider, LbGetCount(pxListbox) - 1);
    SldSetPos(pxSlider, LbGetCount(pxListbox) - 1);
    SetState(pxSlider, SLD_DRAW);
    
    // Update the message.
    switch (eListboxFilter) {
        case LB_FILTER_SERVO:   vStaticTextUpdateText(pxMsgText, "Please select a servo.");     break;
        case LB_FILTER_SENSOR:  vStaticTextUpdateText(pxMsgText, "Please select a sensor.");    break;
    }
    
    // Enable the listbox components.
    vEnableListboxExtModule();
    
    
    
    
    // Read the stack watermark and record it if it's below threshold.
    unsigned portBASE_TYPE uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    if (uxHighWaterMark < MIN_STACK_WATERMARK) {
        xSystemError.bStackLowError = 1;
        vLogStackWatermark("Seek External Module Task", (unsigned short)uxHighWaterMark);
    }
    
    
    
    vTaskDelete(NULL);
}
