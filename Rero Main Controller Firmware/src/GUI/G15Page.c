/*******************************************************************************
 * Functions for G15 page in rero GUI.
 *
 * Author: Kong Wai Weng
 * Company: Cytron Technologies Sdn. Bhd.
 * Email:  waiweng@cytron.com.my
 *******************************************************************************/

#include "Variables.h"
#include "Microchip/Graphics/Graphics.h"
#include "GUI/G15Page.h"
#include "GUI/GraphicScreens.h"
#include "GUI/ScrollbarListbox.h"
#include "G15 and Sensors/G15.h"
#include "setpath.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

// Servo speed when testing the servo.
const unsigned short G15_POS_SPEED = 130;
const unsigned short G15_WHEEL_SPEED = 250;



// Subpage position and margin.
#define G15_MARGIN_LEFT             0
#define G15_MARGIN_RIGHT            8
#define G15_MARGIN_TOP              3
#define G15_MARGIN_BOTTOM           10

#define G15_LIMIT_LEFT              (EM_SUBPAGE_LEFT + G15_MARGIN_LEFT)
#define G15_LIMIT_RIGHT             (EM_SUBPAGE_RIGHT - G15_MARGIN_RIGHT)
#define G15_LIMIT_TOP               (EM_SUBPAGE_TOP + G15_MARGIN_TOP)
#define G15_LIMIT_BOTTOM            (EM_SUBPAGE_BOTTOM - G15_MARGIN_BOTTOM)

#define TXT_HEIGHT                  20
#define TXT_TYPE_X1                 G15_LIMIT_LEFT
#define TXT_TYPE_X2                 G15_LIMIT_RIGHT
#define TXT_TYPE_Y1                 G15_LIMIT_TOP
#define TXT_TYPE_Y2                 (TXT_TYPE_Y1 + TXT_HEIGHT)


// Graphic objects size and position.
// Meter.
#define MTR_WIDTH                   130
#define MTR_HEIGHT                  140
#define MTR_X1                      G15_LIMIT_LEFT
#define MTR_X2                      (MTR_X1 + MTR_WIDTH)
#define MTR_Y1                      (G15_LIMIT_TOP + 2)
#define MTR_Y2                      (MTR_Y1 + MTR_HEIGHT)

#define DMTR_WIDTH                  30
#define DMTR_HEIGHT                 20

// Increase button.
#define BTN_INC_WIDTH               35
#define BTN_INC_HEIGHT              BTN_INC_WIDTH
#define BTN_INC_X1                  (MTR_X2 + 20)
#define BTN_INC_X2                  (BTN_INC_X1 + BTN_INC_WIDTH)
#define BTN_INC_Y1                  (MTR_Y1 + 26)
#define BTN_INC_Y2                  (BTN_INC_Y1 + BTN_INC_HEIGHT)

// Decrease button.
#define BTN_DEC_WIDTH               BTN_INC_WIDTH
#define BTN_DEC_HEIGHT              BTN_INC_HEIGHT
#define BTN_DEC_X1                  BTN_INC_X1
#define BTN_DEC_X2                  (BTN_DEC_X1 + BTN_DEC_WIDTH)
#define BTN_DEC_Y1                  (BTN_INC_Y2 + 10)
#define BTN_DEC_Y2                  (BTN_DEC_Y1 + BTN_DEC_HEIGHT)

// Set path button.
#define BTN_SETPATH_WIDTH           (G15_LIMIT_RIGHT - G15_LIMIT_LEFT - 40)
#define BTN_SETPATH_HEIGHT          30
#define BTN_SETPATH_X1              ((G15_LIMIT_LEFT + G15_LIMIT_RIGHT) / 2 - (BTN_SETPATH_WIDTH / 2))
#define BTN_SETPATH_X2              (BTN_SETPATH_X1 + BTN_SETPATH_WIDTH)
#define BTN_SETPATH_Y1              (G15_LIMIT_BOTTOM - BTN_SETPATH_HEIGHT)
#define BTN_SETPATH_Y2              (BTN_SETPATH_Y1 + BTN_SETPATH_HEIGHT)

// Mode label.
#define TXT_MODE_WIDTH              (G15_LIMIT_RIGHT - MTR_X2)
#define TXT_MODE_HEIGHT             20
#define TXT_MODE_X1                 (MTR_X2 + 7)
#define TXT_MODE_X2                 (TXT_MODE_X1 + TXT_MODE_WIDTH)
#define TXT_MODE_Y1                 (G15_LIMIT_TOP + 22)
#define TXT_MODE_Y2                 (TXT_MODE_Y1 + TXT_MODE_HEIGHT)

// Position button.
#define RB_POS_WIDTH               TXT_MODE_WIDTH
#define RB_POS_HEIGHT              26
#define RB_POS_X1                  (MTR_X2 + 5)
#define RB_POS_X2                  (RB_POS_X1 + RB_POS_WIDTH)
#define RB_POS_Y1                  (TXT_MODE_Y2 + 10)
#define RB_POS_Y2                  (RB_POS_Y1 + RB_POS_HEIGHT)

// Wheel button.
#define RB_WHEEL_WIDTH             RB_POS_WIDTH
#define RB_WHEEL_HEIGHT            RB_POS_HEIGHT
#define RB_WHEEL_X1                RB_POS_X1
#define RB_WHEEL_X2                (RB_WHEEL_X1 + RB_WHEEL_WIDTH)
#define RB_WHEEL_Y1                (RB_POS_Y2 + 10)
#define RB_WHEEL_Y2                (RB_WHEEL_Y1 + RB_WHEEL_HEIGHT)

// Gap between the next, save and cancel button.
#define GAP_NEXT_SAVE_CANCEL_BTN    5

// Next button.
#define BTN_NEXT_WIDTH              90
#define BTN_NEXT_HEIGHT             BTN_SETPATH_HEIGHT
#define BTN_NEXT_X1                 G15_LIMIT_LEFT
#define BTN_NEXT_X2                 (BTN_NEXT_X1 + BTN_NEXT_WIDTH)
#define BTN_NEXT_Y1                 (G15_LIMIT_BOTTOM - BTN_NEXT_HEIGHT)
#define BTN_NEXT_Y2                 (BTN_NEXT_Y1 + BTN_NEXT_HEIGHT)

// Save button.
#define BTN_SAVE_WIDTH              ((G15_LIMIT_RIGHT - G15_LIMIT_LEFT - (2 * GAP_NEXT_SAVE_CANCEL_BTN) - BTN_NEXT_WIDTH) / 2)
#define BTN_SAVE_HEIGHT             BTN_SETPATH_HEIGHT
#define BTN_SAVE_X1                 (BTN_NEXT_X2 + GAP_NEXT_SAVE_CANCEL_BTN)
#define BTN_SAVE_X2                 (BTN_SAVE_X1 + BTN_SAVE_WIDTH)
#define BTN_SAVE_Y1                 (G15_LIMIT_BOTTOM - BTN_SAVE_HEIGHT)
#define BTN_SAVE_Y2                 (BTN_SAVE_Y1 + BTN_SAVE_HEIGHT)

// Cancel button.
#define BTN_CANCEL_WIDTH            BTN_SAVE_WIDTH
#define BTN_CANCEL_HEIGHT           BTN_SETPATH_HEIGHT
#define BTN_CANCEL_X1               (BTN_SAVE_X2 + GAP_NEXT_SAVE_CANCEL_BTN)
#define BTN_CANCEL_X2               (BTN_CANCEL_X1 + BTN_CANCEL_WIDTH)
#define BTN_CANCEL_Y1               (G15_LIMIT_BOTTOM - BTN_CANCEL_HEIGHT)
#define BTN_CANCEL_Y2               (BTN_CANCEL_Y1 + BTN_CANCEL_HEIGHT)



/*******************************************************************************
* PRIVATE GLOBAL VARIABLES
*******************************************************************************/

static unsigned char prv_ucSelectedG15Id = 0xff;
static unsigned char prv_ucWheelMode = 0;
static char prv_szFirmwareRevision[] = "rev:--";



/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 *******************************************************************************/

static void prv_vSelectServo(unsigned char ucServoId);
static void prv_vCreateMeterOrWheel(void);
static void prv_vRemoveMeterOrWheel(void);
static void prv_vCreateTestServoPageButtons(void);
static void prv_vRemoveTestServoPageButtons(void);
static void prv_vCreateSetPathPageButtons(void);
static void prv_vRemoveSetPathPageButtons(void);
static void prv_vCreateG15Subpage(void);



/*******************************************************************************
 * FUNCTION: prv_vSelectServo
 *
 * PARAMETERS:
 * ~ ucServoId  - ID for the selected servo.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Disable the torque and LED for previously selected servo,
 * then turn on the torque and LED for newly selected servo.
 *
 *******************************************************************************/
static void prv_vSelectServo(unsigned char ucServoId)
{
    // If the servo is not already selected.
    if (prv_ucSelectedG15Id != ucServoId) {
        // Turn off Torque n LED, set speed to 0 for previous selection.
        if (prv_ucSelectedG15Id <= EM_MAX_ID) {
            eG15SetTorqueLed(prv_ucSelectedG15Id, WRITE_NOW, 0, 0);
            eG15SetSpeed(prv_ucSelectedG15Id, WRITE_NOW, 0, POSITION_SPEED_CONTROL);
        }

        // Change the selected servo to the new one.
        prv_ucSelectedG15Id = ucServoId;
    }
    

    // Make sure the newly selected servo ID is valid.
    if (prv_ucSelectedG15Id <= EM_MAX_ID) {
        // Set servo speed to 0, turn on torque and LED.
        eG15SetSpeed(prv_ucSelectedG15Id, WRITE_NOW, 0, POSITION_SPEED_CONTROL);
        eG15SetTorqueLed(prv_ucSelectedG15Id, WRITE_NOW, 1, 1);

        // Get the angle limit of the G15.
        unsigned short usLimitCw;
        unsigned short usLimitCcw;
        eG15GetAngleLimit(prv_ucSelectedG15Id, &usLimitCw, &usLimitCcw);

        // Determine the wheel or position mode.
        if (usLimitCw != usLimitCcw) {
            prv_ucWheelMode = 0;
        } else {
            prv_ucWheelMode = 1;
        }
        
        // Get Firmware version
        unsigned char ucFirmwareRevision = 0xff;
        eEMGetFirmwareRevision(prv_ucSelectedG15Id, &ucFirmwareRevision);
        // Update the text with selected sensor firmware revision.
        snprintf(prv_szFirmwareRevision, sizeof(prv_szFirmwareRevision), "rev:%02X", ucFirmwareRevision);
    }
}



/*******************************************************************************
 * FUNCTION: prv_vCreateMeterOrWheel
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the meter for servo position.
 *
 *******************************************************************************/
static void prv_vCreateMeterOrWheel(void)
{
    // Display the meter in position mode.
    if (prv_ucWheelMode == 0) {
        // Remove the wheel icon.
        GOLDeleteObjectByID(GID_G15_IMG_WHEEL);
        
        // Create the meter if it's not found.
        if (GOLFindObject(GID_G15_MTR) == NULL) {
            // Create the analog meter.
            METER *pxAnalogMeter =
                    MtrCreate( GID_G15_MTR,
                               MTR_X1, MTR_Y1,
                               MTR_X2, MTR_Y2,
                               MTR_DRAW | MTR_RING, 0, 0, 359, 0, 0,
                               (void *)ptrSmallFont, (void *)ptrSmallFont, "POSITION MODE", pxAnMeterScheme );

            pxAnalogMeter->arcColor1 = RERO_LIGHT_GREEN;
            pxAnalogMeter->arcColor2 = BRIGHTRED;
            pxAnalogMeter->arcColor3 = BRIGHTYELLOW;


            // Create the digital meter at the center of meter.
            DmCreate( GID_G15_DM,
                      pxAnalogMeter->xCenter - (DMTR_WIDTH / 2), pxAnalogMeter->yCenter - (DMTR_HEIGHT / 2),
                      pxAnalogMeter->xCenter + (DMTR_WIDTH / 2), pxAnalogMeter->yCenter + (DMTR_HEIGHT / 2),
                      DM_CENTER_ALIGN | DM_DRAW, 0, 3, 0, pxDefaultScheme );
        }
        
        
        // Read the servo position.
        unsigned short usPosition;
        eG15GetPosition(prv_ucSelectedG15Id, &usPosition);
        unsigned short usAngle = (unsigned short)((unsigned long)usPosition * 360ul / 1088ul);
        
        // Get the angle limit of the G15 if set path is not running.
        // If set path is running, assign the angle limit to current position.
        unsigned short usLimitCw;
        unsigned short usLimitCcw;
        if (xSystemState.bSetPath == 0) {
            eG15GetAngleLimit(prv_ucSelectedG15Id, &usLimitCw, &usLimitCcw);
        } else {
            usLimitCw = usAngle;
            usLimitCcw = usAngle + 1;
        }

        // Update the angle limit and position.
        vUpdateMeterLimit(usLimitCw, usLimitCcw);
        vUpdateMeterAngle(usAngle);
    }
    
    
    // Display the wheel icon in wheel mode.
    else {
        // Remove the meter objects.
        GOLDeleteObjectByID(GID_G15_MTR);
        GOLDeleteObjectByID(GID_G15_DM);

        // Create the wheel icon if it's not found.
        if (GOLFindObject(GID_G15_IMG_WHEEL) == NULL) {
            PictCreate( GID_G15_IMG_WHEEL,
                        MTR_X1, MTR_Y1,
                        MTR_X2, MTR_Y2,
                        PICT_DRAW | PICT_DISABLED, 1, "/Theme/ServoPage/Wheel.bmp", pxDefaultScheme );
        }
    }
}



/*******************************************************************************
 * FUNCTION: prv_vRemoveMeterOrWheel
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Remove the meter for servo position.
 *
 *******************************************************************************/
static void prv_vRemoveMeterOrWheel(void)
{
    // Remove the meter objects.
    GOLDeleteObjectByID(GID_G15_MTR);
    GOLDeleteObjectByID(GID_G15_DM);
    GOLDeleteObjectByID(GID_G15_IMG_WHEEL);
    
    // Clear the background.
    SetColor(pxDefaultScheme->CommonBkColor);
    Bar( MTR_X1, MTR_Y1,
         MTR_X2, MTR_Y2 );
}



/*******************************************************************************
 * FUNCTION: prv_vCreateTestServoPageButtons
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the following buttons in test servo page.
 * - Increase button.
 * - Decrease button.
 * - Set path button.
 *
 *******************************************************************************/
static void prv_vCreateTestServoPageButtons(void)
{
    // Create the increase button.
    BtnCreate( GID_G15_BTN_INC,
               BTN_INC_X1, BTN_INC_Y1,
               BTN_INC_X2, BTN_INC_Y2,
               0, BTN_DRAW, NULL, "<+", pxBtnScheme );

    // Create the decrease button.
    BtnCreate( GID_G15_BTN_DEC,
               BTN_DEC_X1, BTN_DEC_Y1,
               BTN_DEC_X2, BTN_DEC_Y2,
               0, BTN_DRAW, NULL, ">-", pxBtnScheme );
    
    // Create the Set Path button.
    BtnCreate( GID_G15_BTN_SETPATH,
               BTN_SETPATH_X1, BTN_SETPATH_Y1,
               BTN_SETPATH_X2, BTN_SETPATH_Y2,
               BTN_RADIUS, BTN_DRAW, NULL, "Servo Mode / Set Path", pxBtnScheme );
}



/*******************************************************************************
 * FUNCTION: prv_vRemoveTestServoPageButtons
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Remove the following buttons in test servo page.
 * - Increase button.
 * - Decrease button.
 * - Set path button.
 *
 *******************************************************************************/
static void prv_vRemoveTestServoPageButtons(void)
{
    // Remove the button objects.
    GOLDeleteObjectByID(GID_G15_BTN_INC);
    GOLDeleteObjectByID(GID_G15_BTN_DEC);
    GOLDeleteObjectByID(GID_G15_BTN_SETPATH);
    
    // Clear the background.
    SetColor(pxDefaultScheme->CommonBkColor);
    
    Bar( BTN_INC_X1, BTN_INC_Y1,
         BTN_INC_X2, BTN_INC_Y2 );
    
    Bar( BTN_DEC_X1, BTN_DEC_Y1,
         BTN_DEC_X2, BTN_DEC_Y2 );
    
    Bar( BTN_SETPATH_X1, BTN_SETPATH_Y1,
         BTN_SETPATH_X2, BTN_SETPATH_Y2 );
}



/*******************************************************************************
 * FUNCTION: prv_vCreateSetPathPageButtons
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the following buttons in set path page.
 * - Position button.
 * - Wheel button.
 * - Save and next button.
 * - Save button.
 * - Cancel button.
 *
 *******************************************************************************/
static void prv_vCreateSetPathPageButtons(void)
{
    // Mode label.
    StCreate( GID_G15_TXT_MODE,
              TXT_MODE_X1, TXT_MODE_Y1,
              TXT_MODE_X2, TXT_MODE_Y2,
              ST_DRAW, "Mode:", pxLightGreenTxtScheme );
    
    // Position radio button.
    RbCreate( GID_G15_RB_POS,
              RB_POS_X1, RB_POS_Y1,
              RB_POS_X2, RB_POS_Y2,
              RB_DRAW | RB_GROUP | RB_CHECKED, " Pos", pxSmallRadioBtnScheme );
    
    // Wheel radio button.
    RbCreate( GID_G15_RB_WHEEL,
              RB_WHEEL_X1, RB_WHEEL_Y1,
              RB_WHEEL_X2, RB_WHEEL_Y2,
              RB_DRAW, " Wheel", pxSmallRadioBtnScheme );
    
    
    
    // Next button.
    BtnCreate( GID_G15_BTN_NEXT,
               BTN_NEXT_X1, BTN_NEXT_Y1,
               BTN_NEXT_X2, BTN_NEXT_Y2,
               BTN_RADIUS, BTN_DRAW, NULL, "Save & Next", pxBtnScheme );
    
    // Save button.
    BtnCreate( GID_G15_BTN_SAVE,
               BTN_SAVE_X1, BTN_SAVE_Y1,
               BTN_SAVE_X2, BTN_SAVE_Y2,
               BTN_RADIUS, BTN_DRAW, NULL, "Save", pxBtnScheme );
    
    // Cancel button.
    BtnCreate( GID_G15_BTN_CANCEL,
               BTN_CANCEL_X1, BTN_CANCEL_Y1,
               BTN_CANCEL_X2, BTN_CANCEL_Y2,
               BTN_RADIUS, BTN_DRAW, NULL, "Cancel", pxBtnScheme );
}



/*******************************************************************************
 * FUNCTION: prv_vRemoveSetPathPageButtons
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Remove the following buttons in set path page.
 * - Position button.
 * - Wheel button.
 * - Save and next button.
 * - Save button.
 * - Cancel button.
 *
 *******************************************************************************/
static void prv_vRemoveSetPathPageButtons(void)
{
    // Remove the button objects.
    GOLDeleteObjectByID(GID_G15_TXT_MODE);
    GOLDeleteObjectByID(GID_G15_RB_POS);
    GOLDeleteObjectByID(GID_G15_RB_WHEEL);
    GOLDeleteObjectByID(GID_G15_BTN_NEXT);
    GOLDeleteObjectByID(GID_G15_BTN_SAVE);
    GOLDeleteObjectByID(GID_G15_BTN_CANCEL);
    
    // Clear the background.
    SetColor(pxDefaultScheme->CommonBkColor);
    
    Bar( TXT_MODE_X1, TXT_MODE_Y1,
         TXT_MODE_X2, TXT_MODE_Y2 );
    
    Bar( RB_POS_X1, RB_POS_Y1,
         RB_POS_X2, RB_POS_Y2 );
    
    Bar( RB_WHEEL_X1, RB_WHEEL_Y1,
         RB_WHEEL_X2, RB_WHEEL_Y2 );
    
    Bar( BTN_NEXT_X1, BTN_NEXT_Y1,
         BTN_NEXT_X2, BTN_NEXT_Y2 );
    
    Bar( BTN_SAVE_X1, BTN_SAVE_Y1,
         BTN_SAVE_X2, BTN_SAVE_Y2 );
    
    Bar( BTN_CANCEL_X1, BTN_CANCEL_Y1,
         BTN_CANCEL_X2, BTN_CANCEL_Y2 );
}



/*******************************************************************************
 * FUNCTION: prv_vCreateG15Subpage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the G15 sub page.
 *
 *******************************************************************************/
static void prv_vCreateG15Subpage(void)
{
    // Clear the subpage.
    vClearExtModuleSubpage();
    
    // Create the meter or wheel icon depending on the servo mode.
    prv_vCreateMeterOrWheel();
    
    // Create the increase, decrease and set path button.
    prv_vCreateTestServoPageButtons();
    
    // Create text for sensor firmware revision.
    StCreate( GID_G15_TXT_FIRMWAREREVISION,
              TXT_TYPE_X2 - 40, TXT_TYPE_Y1,
              TXT_TYPE_X2, TXT_TYPE_Y2,
              ST_DRAW, prv_szFirmwareRevision, pxDefaultScheme );
}



/*******************************************************************************
 * FUNCTION: vCreateG15Page
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the G15 page.
 *
 *******************************************************************************/
void vCreateG15Page(void)
{
    GOLFree();
    vCreatePageWindow("SERVO MOTOR");
    vCreateListboxExtModule("Servo:");
    
    // Search for connected servo.
    static LB_FILTER eListboxFilter = LB_FILTER_SERVO;
    xTaskCreate(taskSeekExtModule, "SeekModule", configMINIMAL_STACK_SIZE, &eListboxFilter, tskIDLE_PRIORITY, NULL);
    
    // Deselect the servo.
    prv_vSelectServo(0xff);
}



/*******************************************************************************
 * FUNCTION: usMsgG15Page
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
 * Handle the touchscreen event of the G15 page.
 *
 *******************************************************************************/
WORD usMsgG15Page(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    static unsigned char ucWheelRunning = 0;


    // Process the list box event.
    usMsgListboxExtModule(objMsg, pObj, pMsg);
    
    
    // Button released event.
    if (objMsg == BTN_MSG_RELEASED) {
        switch(GetObjID(pObj)) {
            // Refresh button.
            case GID_EXTMODULE_BTN_REFRESH: {
                // Deselect the servo.
                prv_vSelectServo(0xff);

                // Reload the subpage again.
                vClearWindowSubpage();
                vCreateListboxExtModule("Servo:");

                // Search for connected servo.
                static LB_FILTER eListboxFilter = LB_FILTER_SERVO;
                xTaskCreate(taskSeekExtModule, "SeekModule", configMINIMAL_STACK_SIZE, &eListboxFilter, tskIDLE_PRIORITY, NULL);
                
                break;
            }
                
                
                
            // Increase button.
            case GID_G15_BTN_INC: {
                // Wheel mode.
                if (prv_ucWheelMode == 1) {
                    // If the G15 is not rotating, rotate it counter clockwise.
                    if (ucWheelRunning == 0) {
                        ucWheelRunning = 1;
                        eG15SetSpeed(prv_ucSelectedG15Id, WRITE_NOW, G15_WHEEL_SPEED, WHEEL_CCW);
                    }
                    // Else, stop it.
                    else {
                        ucWheelRunning = 0;
                        eG15SetSpeed(prv_ucSelectedG15Id, WRITE_NOW, 0, WHEEL_CCW);
                    }
                }

                // Increase the position for 1 degree in position mode.
                else {
                    METER *pxAnalogMeter = (METER*)GOLFindObject(GID_G15_MTR);
                    if (pxAnalogMeter != NULL) {
                        unsigned short usPositionDeg = pxAnalogMeter->value;
                        if (++usPositionDeg > 359) {
                            usPositionDeg = 0;
                        }

                        unsigned short usPosition = (unsigned short)((unsigned long)usPositionDeg * 1088ul / 360ul);

                        // send command, return != 0 if out of range
                        if (eG15SetPositionSpeed(prv_ucSelectedG15Id, WRITE_NOW, usPosition, NORMAL_POSITIONING, G15_POS_SPEED, POSITION_SPEED_CONTROL) == EM_NO_ERROR) {
                            vUpdateMeterAngle(usPositionDeg);
                        }
                    }
                }
                
                break;
            }



            // Decrease button.
            case GID_G15_BTN_DEC: {
                // Wheel mode.
                if (prv_ucWheelMode == 1) {
                    // If the G15 is not rotating, rotate it clockwise.
                    if (ucWheelRunning == 0) {
                        ucWheelRunning = 1;
                        eG15SetSpeed(prv_ucSelectedG15Id, WRITE_NOW, G15_WHEEL_SPEED, WHEEL_CW);
                    }
                    // Else, stop it.
                    else {
                        ucWheelRunning = 0;
                        eG15SetSpeed(prv_ucSelectedG15Id, WRITE_NOW, 0, WHEEL_CW);
                    }
                }

                // Decrease the position for 1 degree in position mode.
                else {
                    METER *pxAnalogMeter = (METER*)GOLFindObject(GID_G15_MTR);
                    if (pxAnalogMeter != NULL) {
                        unsigned short usPositionDeg = pxAnalogMeter->value;
                        if (usPositionDeg > 0) {
                            usPositionDeg--;
                        }
                        else {
                            usPositionDeg = 359;
                        }

                        unsigned short usPosition = (unsigned short)((unsigned long)usPositionDeg * 1088ul / 360ul);

                        // send command, return != 0 if out of range
                        if (eG15SetPositionSpeed(prv_ucSelectedG15Id, WRITE_NOW, usPosition, NORMAL_POSITIONING, G15_POS_SPEED, POSITION_SPEED_CONTROL) == EM_NO_ERROR) {
                            vUpdateMeterAngle(usPositionDeg);
                        }
                    }
                }
                
                break;
            }



            // Set Path button.
            case GID_G15_BTN_SETPATH: {
                // Stop the running wheel.
                ucWheelRunning = 0;
                
                // Create and start the set path task.
                vSetPathStart(prv_ucSelectedG15Id);
                
                // Disable the listbox and its objects.
                vDisableListboxExtModule();
                
                // Reset to position mode and create the meter.
                prv_ucWheelMode = 0;
                prv_vCreateMeterOrWheel();
                
                // Remove the buttons in test servo page.
                prv_vRemoveTestServoPageButtons();

                // Create the buttons in set path page.
                prv_vCreateSetPathPageButtons();
                
                break;
            }
                
                
                
            // Next button.
            case GID_G15_BTN_NEXT: {
                // Stop the set path task and save the limits.
                vSetPathSave();

                // Delay 200ms to wait for saving complete.
                vTaskDelay(200 / portTICK_RATE_MS);
                
                
                
                // Get the next selection from the list box.
                LISTITEM *pxNextItem = pxListboxExtModuleSelectNextItem();
                
                // If this is not the last item in the list...
                if (pxNextItem != NULL) {
                    // Select the next servo.
                    prv_vSelectServo(pxNextItem->data);
                    
                    // Create and start the set path task.
                    vSetPathStart(prv_ucSelectedG15Id);
                    
                    // Reset to position mode and create the meter.
                    prv_ucWheelMode = 0;
                    prv_vCreateMeterOrWheel();

                    // Checked the position radio button.
                    RADIOBUTTON *pxRbPos = (RADIOBUTTON*)GOLFindObject(GID_G15_RB_POS);
                    RADIOBUTTON *pxRbWheel = (RADIOBUTTON*)GOLFindObject(GID_G15_RB_WHEEL);
                    if ((pxRbPos != NULL) && (pxRbWheel != NULL)) {
                        ClrState(pxRbWheel, RB_CHECKED);
                        SetState(pxRbWheel, RB_DRAW);
                        SetState(pxRbPos, RB_CHECKED | RB_DRAW);
                    }
                }

                // If this is the last item in the list...
                else {
                    // Reselect the current servo.
                    prv_vSelectServo(prv_ucSelectedG15Id);
                
                    // Enable back the listbox and its objects.
                    vEnableListboxExtModule();

                    // Re-create the sub page.
                    prv_vCreateG15Subpage();
                }
                
                break;
            }



            // Save button.
            case GID_G15_BTN_SAVE: {
                // Stop the set path task and save the limits.
                vSetPathSave();

                // Delay 200ms to wait for saving complete.
                vTaskDelay(200 / portTICK_RATE_MS);
                
                // Reselect the current servo.
                prv_vSelectServo(prv_ucSelectedG15Id);

                // Enable back the listbox and its objects.
                vEnableListboxExtModule();

                // Re-create the sub page.
                prv_vCreateG15Subpage();
                
                break;
            }



            // Cancel button.
            case GID_G15_BTN_CANCEL: {
                // Stop the set path task and discard the limits.
                vSetPathDiscard();

                // Delay 200ms to wait for set path complete.
                vTaskDelay(200 / portTICK_RATE_MS);
                
                // Reselect the current servo.
                prv_vSelectServo(prv_ucSelectedG15Id);

                // Enable back the listbox and its objects.
                vEnableListboxExtModule();

                // Re-create the sub page.
                prv_vCreateG15Subpage();
                
                break;
            }



            // Exit button.
            case GID_BTN_EXIT: {
                // Stop the running wheel.
                ucWheelRunning = 0;
                
                // Stop the set path task and discard the limits if set path is running.
                if(xSystemState.bSetPath == 1) {
                    vSetPathDiscard();

                    // Delay 200ms to wait for set path complete.
                    vTaskDelay(200 / portTICK_RATE_MS);
                }
                
                // Deselect the servo.
                prv_vSelectServo(0xff);
                
                // Wait for the searching to complete.
                xSemaphoreTake(xSeekExtModuleSemaphore, portMAX_DELAY);

                // Clear the list box.
                LISTBOX *pxListBox = (LISTBOX*)GOLFindObject(GID_EXTMODULE_LB);
                if (pxListBox != NULL) {
                    LbDelItemsList(pxListBox);
                }

                // Go back to main menu.
                vSetGuiPage(PAGE_MAIN_MENU);
                
                break;
            }
        }
    }   // End of button released event.
    
    
    
    // Other event.
    else {
        switch(GetObjID(pObj)) {
            // List box.
            case GID_EXTMODULE_LB: {
                if (pMsg->uiEvent == EVENT_RELEASE) {
                    // Stop the running wheel.
                    ucWheelRunning = 0;
                
                    // Get the selected item in the list box.
                    LISTITEM* pxLbItem = LbGetSel((LISTBOX *)pObj, NULL);

                    // If an item is selected...
                    if(pxLbItem != NULL) {
                        // Get the ID and select the servo.
                        prv_vSelectServo(pxLbItem->data);

                        // Create the sub page.
                        prv_vCreateG15Subpage();
                    }
                }
                break;
            }



            // Analog meter.
            case GID_G15_MTR: {
                // Make sure set path is not running.
                if (xSystemState.bSetPath == 0) {
                    if(objMsg == MTR_MSG_TOUCHED) {
                        METER *pxAnalogMeter = (METER*)GOLFindObject(GID_G15_MTR);
                        if (pxAnalogMeter != NULL) {
                            unsigned short usPosition = (unsigned short)((unsigned long)pxAnalogMeter->degree * 1088ul / 360ul);

                            // Write new position to G15 and update the meter.
                            if (eG15SetPositionSpeed(prv_ucSelectedG15Id, WRITE_NOW, usPosition, NORMAL_POSITIONING, G15_POS_SPEED, POSITION_SPEED_CONTROL) == EM_NO_ERROR) {
                                vUpdateMeterAngle(pxAnalogMeter->degree);
                            }
                        }
                    }
                }
                break;
            }
                
                
                
            // Position radio button.
            case GID_G15_RB_POS: {
                // Create the meter.
                prv_ucWheelMode = 0;
                prv_vCreateMeterOrWheel();
                break;
            }
                
                
                
            // Wheel radio button.
            case GID_G15_RB_WHEEL: {
                // Create the wheel icon.
                prv_ucWheelMode = 1;
                prv_vCreateMeterOrWheel();
                break;
            }
        }
    }   // End of other event.
    
    return 1;
}



/*******************************************************************************
 * FUNCTION: vUpdateMeterAngle
 *
 * PARAMETERS:
 * ~ usAngle    - Position of G15 (0 - 359 degree).
 *              - >= 360 degree = wheel mode.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Update the G15 position for analog and digital meter.
 *
 *******************************************************************************/
void vUpdateMeterAngle(unsigned short usAngle)
{
    METER *pxAnalogMeter = (METER*)GOLFindObject(GID_G15_MTR);
    DIGITALMETER *pxDigitalMeter = (DIGITALMETER*)GOLFindObject(GID_G15_DM);
    
    if ((pxAnalogMeter != NULL) && (pxDigitalMeter != NULL)) {
        // Position mode.
        if (usAngle < 360) {
            MtrSetVal(pxAnalogMeter, (SHORT)usAngle);

            // Only draw update if the object is not being drawn.
            if (IsObjUpdated(pxAnalogMeter) == 0) {
                SetState(pxAnalogMeter, MTR_DRAW_UPDATE);
            }


            DmSetValue(pxDigitalMeter, (DWORD)usAngle);

            // Always redraw to retain the black rectangular box.
            ClrState(pxDigitalMeter, DM_HIDE);
            SetState(pxDigitalMeter, DM_DRAW);
        }

        // Wheel mode.
        else {
            // Hide the digital meter.
            SetState(pxDigitalMeter, DM_HIDE | DM_DRAW);
        }
    }
}



/*******************************************************************************
 * FUNCTION: vUpdateMeterLimit
 *
 * PARAMETERS:
 * ~ usLimitCw    - CW angle limit (0 - 1087).
 * ~ usLimitCcw   - CCW angle limit (0 - 1087).
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Update the G15 angle limit for analog meter.
 *
 *******************************************************************************/
void vUpdateMeterLimit(unsigned short usLimitCw, unsigned short usLimitCcw)
{
    // Convert the angle limit to degree.
    unsigned short usLimitDegCw = (unsigned short)((unsigned long)usLimitCw * 360ul / 1088ul);
    unsigned short usLimitDegCcw = (unsigned short)((unsigned long)usLimitCcw * 360ul / 1088ul);


    // Minimum resolution for the meter is 1 degree.
    // We need to make sure the meter still showing the correct angle limit when the limit is < 1 degree.
    if(usLimitDegCw == usLimitDegCcw) {
        if(usLimitCw > usLimitCcw) {
            if (usLimitDegCw < 359) {
                usLimitDegCw++;
            }
            else {
                usLimitDegCcw--;
            }
        }
        else if(usLimitCcw > usLimitCw) {
            if (usLimitDegCcw < 359) {
                usLimitDegCcw++;
            }
            else {
                usLimitDegCw--;
            }
        }
    }

    METER *pxAnalogMeter = (METER*)GOLFindObject(GID_G15_MTR);
    if (pxAnalogMeter != NULL) {
        pxAnalogMeter->angleLimitCW = usLimitDegCw;
        pxAnalogMeter->angleLimitCCW = usLimitDegCcw;

        // Only draw update if the object is not being drawn.
        if (IsObjUpdated(pxAnalogMeter) == 0) {
            SetState(pxAnalogMeter, MTR_DRAW_UPDATE);
        }
    }
    
}
