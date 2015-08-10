/*******************************************************************************
 * Functions for the G15 set path process.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "SetPath.h"
#include "HardwareProfile.h"
#include "Variables.h"
#include "Log.h"
#include "G15 and Sensors/G15.h"
#include "GUI/G15Page.h"
#include "GUI/GraphicScreens.h"

// Header files for FreeRTOS.
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
* PRIVATE GLOBAL VARIABLES                                                     *
*******************************************************************************/

// Indicate to save or discard the limits.
// 0 = Discard the limits.
// 1 = Save the limits.
// 2 = Setting path in progress.
static volatile unsigned char ucSaveDiscard = 0;

// ID for selected G15.
static volatile unsigned char ucG15Id = 0;



/*******************************************************************************
 * FUNCTION: vSetPathStart
 *
 * PARAMETERS:
 * ~ ucId   - The ID of selected G15.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Start setting path.
 *
 *******************************************************************************/
void vSetPathStart(unsigned char ucId)
{
    if (xSystemState.bSetPath == 0) {
        xSystemState.bSetPath = 1;
        
        ucG15Id = ucId;
        xTaskCreate(taskSetPath, "Set Path", 256, NULL, tskIDLE_PRIORITY + 1, NULL);
    }
}



/*******************************************************************************
 * FUNCTION: vSetPathSave
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Stop setting path and save the limits.
 *
 *******************************************************************************/
void vSetPathSave(void)
{
    ucSaveDiscard = 1;
}



/*******************************************************************************
 * FUNCTION: vSetPathDiscard
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Stop setting path and discard the limits.
 *
 *******************************************************************************/
void vSetPathDiscard(void)
{
    ucSaveDiscard = 0;
}



/*******************************************************************************
 * TASK: taskSetPath
 *
 * DESCRIPTIONS:
 * Read the position from the G15 and determine its angle limit.
 *
 *******************************************************************************/
void taskSetPath (void *pvParameters)
{
    signed short ssCurrentAngle = 0;
    signed short ssPreviousAngle = 0;
    signed short ssLinearPosition = 0;
    signed short ssCwLimit = 0;
    signed short ssCcwLimit = 0;
    signed short ssLinearCwLimit = 0;
    signed short ssLinearCcwLimit = 0;
    signed short ssOriCwLimit = 0;
    signed short ssOriCcwLimit = 0;
    signed short ssTargetPosition = 0;
    signed short ssTorqueLimit = 0;
    unsigned char ucPreviousWheelMode = 0;
    unsigned char ucWheelMode = 0;
    
    
    
    // Running set path.
    ucSaveDiscard = 2;



    // Get the original angle limit.
    eG15GetAngleLimit(ucG15Id, (unsigned short*)&ssOriCwLimit, (unsigned short*)&ssOriCcwLimit);

    // Get the first position from the G15.
    eG15GetPosition(ucG15Id, (unsigned short*)&ssCurrentAngle);
    ssPreviousAngle = ssCurrentAngle;
    ssLinearPosition = ssCurrentAngle;
    ssLinearCwLimit = ssCurrentAngle;
    ssLinearCcwLimit = ssCurrentAngle + 1;      // +1 to make sure it won't set as wheel mode.
    
    
    
    // Temporary set the angle limit to make sure the G15 is in position mode.
    eG15SetAngleLimit(ucG15Id, 0, 1);

    // Set the speed to maximum.
    eG15SetSpeed(ucG15Id, WRITE_NOW, 1023, POSITION_SPEED_CONTROL);
    

    
    // Get the current tick time.
    portTickType prevTick = xTaskGetTickCount();

    // Read the position and convert the path limit to linear value until asked to save or discard.
    while (ucSaveDiscard == 2) {
        // Loop every 100ms.
        vTaskDelayUntil( &prevTick, 100 / portTICK_RATE_MS );



        // Read the G15 position.
        eG15GetPosition(ucG15Id, (unsigned short*)&ssCurrentAngle);

        // Get the angle difference.
        signed short ssAngleDiff = ssCurrentAngle - ssPreviousAngle;

        // Make correction to the angle difference if crossing 0.
        if (ssAngleDiff < -(1088/2)) {
            ssAngleDiff += 1088;
        }
        else if (ssAngleDiff > (1088/2)) {
            ssAngleDiff -= 1088;
        }



        // Moving CCW.
        if (ssAngleDiff > 0) {
           ssLinearPosition += ssAngleDiff;

           // update the CCW limit.
           if (ssLinearCcwLimit < ssLinearPosition) {
               ssLinearCcwLimit = ssLinearPosition;
           }

           // Drag the CW limit along if overlapped.
           if ((ssLinearCcwLimit - ssLinearCwLimit) > 1087) {
               ssLinearCwLimit = ssLinearCcwLimit - 1087;
           }



           // Torque limit is depending on how fast we move.
           ssTorqueLimit = (ssAngleDiff * 7) + 30;
           if (ssTorqueLimit > 500) {
               ssTorqueLimit = 500;
           }
           eG15SetTorqueLimit(ucG15Id, (unsigned short)ssTorqueLimit);
           eG15SetTorque(ucG15Id, WRITE_NOW, 1);

           // Set the target position to assist moving.
           ssTargetPosition = ssCurrentAngle + (1088 / 2);
           if (ssTargetPosition > 1087) {
               ssTargetPosition -= 1088;
           }
           eG15SetPosition(ucG15Id, WRITE_NOW, (unsigned short)ssTargetPosition, DIRECTION_POSITIONING_CCW);
        }

        // Moving CW.
        else if (ssAngleDiff < 0) {
           ssLinearPosition += ssAngleDiff;

           // update the CW limit.
           if (ssLinearCwLimit > ssLinearPosition) {
               ssLinearCwLimit = ssLinearPosition;
           }

           // Drag the CCW limit along if overlapped.
           if ((ssLinearCwLimit - ssLinearCcwLimit) < -1087) {
               ssLinearCcwLimit = ssLinearCwLimit + 1087;
           }



           // Torque limit is depending on how fast we move.
           ssTorqueLimit = (-ssAngleDiff * 7) + 30;
           if (ssTorqueLimit > 500) {
               ssTorqueLimit = 500;
           }

           eG15SetTorqueLimit(ucG15Id, (unsigned short)ssTorqueLimit);
           eG15SetTorque(ucG15Id, WRITE_NOW, 1);

           // Set the target position to assist moving.
           ssTargetPosition = ssCurrentAngle - (1088 / 2);
           if (ssTargetPosition < 0) {
               ssTargetPosition += 1088;
           }
           eG15SetPosition(ucG15Id, WRITE_NOW, (unsigned short)ssTargetPosition, DIRECTION_POSITIONING_CW);
        }

        // Not moving.
        else {
            eG15SetTorque(ucG15Id, WRITE_NOW, 0);
        }



        // Convert the path limit to within the range of 0 - 1087.
        ssCcwLimit = ssLinearCcwLimit % 1088;
        if (ssCcwLimit < 0) {
            ssCcwLimit += 1088;
        }

        ssCwLimit = ssLinearCwLimit % 1088;
        if (ssCwLimit < 0) {
            ssCwLimit += 1088;
        }
        
        
        // Check whether the wheel mode is selected.
        RADIOBUTTON *pxRbWheel = (RADIOBUTTON*)GOLFindObject(GID_G15_RB_WHEEL);
        if (pxRbWheel != NULL) {
            if (GetState(pxRbWheel, RB_CHECKED) != 0) {
                ucWheelMode = 1;
            }
            else {
                ucWheelMode = 0;
            }
        }

        

        // Update the GUI when there is changes.
        // Make sure the meter is there.
        vTaskEnterCritical();
        if (GOLFindObject(GID_G15_MTR) != NULL) {
            if ((ssPreviousAngle != ssCurrentAngle) || (ucPreviousWheelMode != ucWheelMode)) {
                // Update the angle limit and position.
                vUpdateMeterLimit((unsigned short)ssCwLimit, (unsigned short)ssCcwLimit);
                vUpdateMeterAngle((unsigned short)((unsigned long)ssCurrentAngle * 360ul / 1088ul));
            }
        }
        vTaskExitCritical();
        
        // Save the current angle as previous angle.
        ssPreviousAngle = ssCurrentAngle;

        // Save the current wheel mode as previous wheel mode.
        ucPreviousWheelMode = ucWheelMode;
        


        // Read the stack watermark and record it if it's below threshold.
        unsigned portBASE_TYPE uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        if (uxHighWaterMark < MIN_STACK_WATERMARK) {
            xSystemError.bStackLowError = 1;

            // Only log it when the watermark value changed.
            static portBASE_TYPE uxPreviousWatermark = 0;
            if (uxHighWaterMark != uxPreviousWatermark) {
                vLogStackWatermark("Set Path Task", (unsigned short)uxHighWaterMark);
            }
            uxPreviousWatermark = uxHighWaterMark;
        }
    }   // End of loop.

    

    // Turn off the torque and set the limit to maximum.
    eG15SetTorque(ucG15Id, WRITE_NOW, 0);
    eG15SetTorqueLimit(ucG15Id, 1023);


    // If we need to save the limits...
    if (ucSaveDiscard == 1) {
        // Reset the limit if it's wheel mode.
        if (ucWheelMode != 0) {
            ssCwLimit = 0;
            ssCcwLimit = 0;
        }
        
        // Send the command to G15 to set the limit.
        eG15SetAngleLimit(ucG15Id, (unsigned short)ssCwLimit, (unsigned short)ssCcwLimit);
    }
    // Diacard the angle limit.
    else if (ucSaveDiscard == 0) {
        // Send the command to G15 to set the limit back to its original value.
        eG15SetAngleLimit(ucG15Id, (unsigned short)ssOriCwLimit, (unsigned short)ssOriCcwLimit);
    }

    // Clear the setting path flag.
    xSystemState.bSetPath = 0;

    // Delete this task after complete.
    vTaskDelete(NULL);
}
