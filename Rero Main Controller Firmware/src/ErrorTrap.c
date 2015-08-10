/*******************************************************************************
 * This file contains the error trap and FreeRTOS error hook.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "HardwareProfile.h"
#include "Background.h"
#include "TimeDelay.h"
#include "Variables.h"
#include "Log.h"



/*******************************************************************************
 * FUNCTION: vApplicationStackOverflowHook
 *
 * PARAMETERS:
 * ~ *pxTask        - Task handler for the task that causing the error.
 * ~ *pcTaskName    - Task name for the task that causing the error.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * This function is called if a task overflows its stack, it can be used
 * for diagnostics.
 *
 *******************************************************************************/
void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed portCHAR *pcTaskName )
{
    // Log the error.
    vLogStackOverflow(pcTaskName);

    // Blink LED.
    while (1) {
        vSetLed(LED_WHITE);
        DelayMs(50);
        vSetLed(LED_OFF);
        DelayMs(50);
    }
}



/*******************************************************************************
 * FUNCTION: vApplicationMallocFailedHook
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * This function is called when the FreeRTOS malloc failed.
 *
 *******************************************************************************/
void vApplicationMallocFailedHook( void )
{
    xSystemError.bMallocError = 1;

    // Log the malloc error to SD card.
    vLogMallocError("FreeRTOS malloc failed");
}



/*******************************************************************************
 * FUNCTION: _general_exception_handler
 *
 * PARAMETERS:
 * ~ cause      - Value of CAUSE register.
 * ~ status     - Value of STATUS register.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * This function is called when general exception occured.
 *
 *******************************************************************************/
void _general_exception_handler (unsigned cause, unsigned status)
{
    // Get the ExcCode field from the Cause register.
    unsigned char ucExcCode = (unsigned char)((cause >> 2) & 0x1f);

    // Log the error.
    vLogGeneralException(ucExcCode);

    // Reset.
    SoftReset();
}
