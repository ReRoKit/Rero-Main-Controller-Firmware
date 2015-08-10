/*******************************************************************************
 * This file contains the task for touch screen.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "HardwareProfile.h"
#include "Variables.h"
#include "Log.h"
#include "TouchScreen/TouchScreen.h"
#include "TouchScreen/TouchScreenResistive.h"

// FreeRTOS include.
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
* TASK: taskTouchScreen
*
* DESCRIPTIONS:
* Task to read from the touch screen.
*
*******************************************************************************/
void taskTouchScreen(void *pvParameters)
{
    static GOL_MSG xOldMsg;

    while(1) {
        // Screen is touched.
        if(TC_PEN_PIN == 0) {
            xSemaphoreGive(xTouchSemaphore);
        }

        // Get the touch screen message.
        GOL_MSG xNewMsg;
        TouchDetectPosition();
        TouchGetMsg(&xNewMsg);

        // Only send the message if there is new data.
        if ((xNewMsg.uiEvent != EVENT_INVALID) || (xOldMsg.uiEvent != EVENT_INVALID)) {
            // Send the message. Timeout after 1000ms.
            xQueueSendToBack(xTouchMsgQueue, &xNewMsg, 1000 / portTICK_RATE_MS);
        }

        // Save the message.
        xOldMsg = xNewMsg;

        // Delay for 20ms.
        vTaskDelay(20 / portTICK_RATE_MS);



        // Read the stack watermark and record it if it's below threshold.
        unsigned portBASE_TYPE uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        if (uxHighWaterMark < MIN_STACK_WATERMARK) {
            xSystemError.bStackLowError = 1;

            // Only log it when the watermark value changed.
            static portBASE_TYPE uxPreviousWatermark = 0;
            if (uxHighWaterMark != uxPreviousWatermark) {
                vLogStackWatermark("Touch Screen Task", (unsigned short)uxHighWaterMark);
            }
            uxPreviousWatermark = uxHighWaterMark;
        }
    }
}
