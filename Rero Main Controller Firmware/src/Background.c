/*******************************************************************************
 * This file is for the background running tasks.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "Background.h"
#include "HardwareProfile.h"
#include "Variables.h"
#include "Log.h"
#include "Graphics/Graphics.h"

// Header files for FreeRTOS.
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

// LED blinking period.
// Unit = ms.
#define LED_BLINKING_PERIOD     200

// Standby timeout period.
// Unit = ms / portTICK_RATE_MS.
#define DIM_SCREEN_PERIOD       (3000 / portTICK_RATE_MS)   // 3 seconds.
#define SHORT_STANDBY_TIMEOUT   (8000 / portTICK_RATE_MS)   // 8 seconds.



/*******************************************************************************
 * FUNCTION: vSetLed
 *
 * PARAMETERS:
 * ~ eColor         - Color of the LED.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Set the LED color on rero controller.
 *
 *******************************************************************************/
void vSetLed(LED_COLOR eColor)
{
    switch (eColor) {
        case LED_OFF:       LEDR = L_OFF;   LEDG = L_OFF;   LEDB = L_OFF;   break;
        case LED_RED:       LEDR = L_ON;    LEDG = L_OFF;   LEDB = L_OFF;   break;
        case LED_GREEN:     LEDR = L_OFF;   LEDG = L_ON;    LEDB = L_OFF;   break;
        case LED_BLUE:      LEDR = L_OFF;   LEDG = L_OFF;   LEDB = L_ON;    break;
        case LED_YELLOW:    LEDR = L_ON;    LEDG = L_ON;    LEDB = L_OFF;   break;
        case LED_CYAN:      LEDR = L_OFF;   LEDG = L_ON;    LEDB = L_ON;    break;
        case LED_MAGENTA:   LEDR = L_ON;    LEDG = L_OFF;   LEDB = L_ON;    break;
        case LED_WHITE:     LEDR = L_ON;    LEDG = L_ON;    LEDB = L_ON;    break;
        
        default:            LEDR = L_OFF;   LEDG = L_OFF;   LEDB = L_OFF;   break;
    }
}



/*******************************************************************************
 * FUNCTION: vBlinkLed
 *
 * PARAMETERS:
 * ~ eColor         - Color of the LED.
 * ~ ucBlinkCount   - Number of time to blink.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Blink the LED on rero controller.
 *
 *******************************************************************************/
void vBlinkLed(LED_COLOR eColor, unsigned char ucBlinkCount)
{
    while (ucBlinkCount-- > 0) {
        // Turn on LED.
        vSetLed(eColor);
        if (xSystemState.bRtosRunning == 1) {
            vTaskDelay(LED_BLINKING_PERIOD / portTICK_RATE_MS);
        } else {
            DelayMs(LED_BLINKING_PERIOD);
        }
        
        // Turn Off LED.
        vSetLed(LED_OFF);
        if (xSystemState.bRtosRunning == 1) {
            vTaskDelay(LED_BLINKING_PERIOD / portTICK_RATE_MS);
        } else {
            DelayMs(LED_BLINKING_PERIOD);
        }
    }
}



/*******************************************************************************
 * TASK: taskShutdownWatchdog
 *
 * DESCRIPTIONS:
 * Watchdog for auto shutdown.
 * This task should be killed if the touch screen is touched before timeout.
 *
 *******************************************************************************/
void taskShutdownWatchdog (void *pvParameters)
{
    portTickType xTimeoutPeriod;
    
    
    
    while (1) {
        // Read the stack watermark and record it if it's below threshold.
        unsigned portBASE_TYPE uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        if (uxHighWaterMark < MIN_STACK_WATERMARK) {
            xSystemError.bStackLowError = 1;

            // Only log it when the watermark value changed.
            static portBASE_TYPE uxPreviousWatermark = 0;
            if (uxHighWaterMark != uxPreviousWatermark) {
                vLogStackWatermark("Shutdown Watchdog Task", (unsigned short)uxHighWaterMark);
            }
            uxPreviousWatermark = uxHighWaterMark;
        }
        
        
    
        // Get the timeout period base on auto shutdown setting.
        switch (eAutoShutdown) {
            case AUTO_SHUTDOWN_1_MIN:   xTimeoutPeriod = 60000 / portTICK_RATE_MS;      break;
            case AUTO_SHUTDOWN_5_MIN:   xTimeoutPeriod = 300000 / portTICK_RATE_MS;     break;
            case AUTO_SHUTDOWN_10_MIN:  xTimeoutPeriod = 600000 / portTICK_RATE_MS;     break;
            case AUTO_SHUTDOWN_20_MIN:  xTimeoutPeriod = 1200000 / portTICK_RATE_MS;    break;
            case AUTO_SHUTDOWN_30_MIN:  xTimeoutPeriod = 1800000 / portTICK_RATE_MS;    break;
            case AUTO_SHUTDOWN_60_MIN:  xTimeoutPeriod = 3600000 / portTICK_RATE_MS;    break;
            case AUTO_SLEEP_NEVER:      xTimeoutPeriod = portMAX_DELAY;                 break;
        }
        
        
        
        // Wait for timeout.
        vTaskDelay(xTimeoutPeriod);
        
        // After timeout, shut down if these conditions are met.
        // - Bluetooth is not connected.
        // - USB is not connected.
        if ( (BT_CONNECTED == 0) &&
             (xSystemState.bUsbMode == 0) ) {

            // Shutting down.
            vPowerOff();
            vTaskSuspend(NULL);
            // Program should stop here.
        }
        
        // Program will loop again if the shutdown conditions are not met.
    }
}



/*******************************************************************************
 * TASK: taskStandbyWatchdog
 *
 * DESCRIPTIONS:
 * Watchdog for system standby. Lock the screen after timeout.
 *
 *******************************************************************************/
void taskStandbyWatchdog (void *pvParameters)
{
    xTaskHandle xShutdownWatchdogTask = NULL;
    portTickType xTimeoutPeriod;
    
    
    
    while (1) {
        // Read the stack watermark and record it if it's below threshold.
        unsigned portBASE_TYPE uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        if (uxHighWaterMark < MIN_STACK_WATERMARK) {
            xSystemError.bStackLowError = 1;

            // Only log it when the watermark value changed.
            static portBASE_TYPE uxPreviousWatermark = 0;
            if (uxHighWaterMark != uxPreviousWatermark) {
                vLogStackWatermark("Standby Watchdog Task", (unsigned short)uxHighWaterMark);
            }
            uxPreviousWatermark = uxHighWaterMark;
        }
        
        
        
        // Kill the shutdown watchdog task if it's running.
        if (xShutdownWatchdogTask != NULL) {
            vTaskDelete(xShutdownWatchdogTask);
            xShutdownWatchdogTask = NULL;
        }
        
        
        
        // Timeout should be shorter while the screen is still locked.
        // Delay a while before we check for the flag to let it get updated. The flag is only updated when the screen is not pressed.
        vTaskDelay(100 / portTICK_RATE_MS);
        if (xSystemState.bStandby != 0) {
            xTimeoutPeriod = SHORT_STANDBY_TIMEOUT - DIM_SCREEN_PERIOD;
        }
        
        // Get the timeout period base on auto sleep setting.
        else {
            switch (eAutoSleep) {
                case AUTO_SLEEP_15_SEC: xTimeoutPeriod = (15000 / portTICK_RATE_MS) - DIM_SCREEN_PERIOD;    break;
                case AUTO_SLEEP_30_SEC: xTimeoutPeriod = (30000 / portTICK_RATE_MS) - DIM_SCREEN_PERIOD;    break;
                case AUTO_SLEEP_1_MIN:  xTimeoutPeriod = (60000 / portTICK_RATE_MS) - DIM_SCREEN_PERIOD;    break;
                case AUTO_SLEEP_2_MIN:  xTimeoutPeriod = (120000 / portTICK_RATE_MS) - DIM_SCREEN_PERIOD;   break;
                case AUTO_SLEEP_5_MIN:  xTimeoutPeriod = (300000 / portTICK_RATE_MS) - DIM_SCREEN_PERIOD;   break;
                case AUTO_SLEEP_10_MIN: xTimeoutPeriod = (600000 / portTICK_RATE_MS) - DIM_SCREEN_PERIOD;   break;
                
                case AUTO_SLEEP_NEVER:
                    // Create the shutdown watchdog task if auto sleep is disabled.
                    xTaskCreate(taskShutdownWatchdog, "Shutdown", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, &xShutdownWatchdogTask);
                    xTimeoutPeriod = portMAX_DELAY;
                    break;
            }
        }
        
        
        
        // Turn on the backlight.
        SetDCOC1PWM(BLight);
        
        // Wait for touch semaphore.
        // Loop back if the screen is touched before timeout.
        if (xSemaphoreTake(xTouchSemaphore, xTimeoutPeriod) == pdTRUE) continue;
        
        
        
        // Dim the backlight.
        SetDCOC1PWM(300);
        
        // Wait for touch semaphore again.
        // Loop back if the screen is touched before timeout.
        if (xSemaphoreTake(xTouchSemaphore, DIM_SCREEN_PERIOD) == pdTRUE) continue;
        
        
        
        // Turn off backlight and lock the screen.
        SetDCOC1PWM(0);
        vLockScreen();
        
        // Start the auto shutdown task.
        xTaskCreate(taskShutdownWatchdog, "Shutdown", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, &xShutdownWatchdogTask);
        
        // Wait forever until the screen is touched.
        xSemaphoreTake(xTouchSemaphore, portMAX_DELAY);
        
    }   // End of while(1).
}



/*******************************************************************************
 * TASK: taskBlinkLed
 *
 * DESCRIPTIONS:
 * Blink the LED according to the system flag.
 *
 *******************************************************************************/
void taskBlinkLed (void *pvParameters)
{
    while (1) {
        if (xSystemError.bMallocError == 1) {
            // Blink Red LED for 2 times.
            vBlinkLed(LED_RED, 2);
            vTaskDelay(800 / portTICK_RATE_MS);
        }
        else if (xSystemError.bStackLowError == 1) {
            // Blink Red LED for 3 times.
            vBlinkLed(LED_RED, 3);
            vTaskDelay(800 / portTICK_RATE_MS);
        }
        else if (xSystemError.bUartError == 1) {
            // Clear the error.
            xSystemError.bUartError = 0;
            
            // Blink Red LED for 4 times.
            vBlinkLed(LED_RED, 4);
            vTaskDelay(800 / portTICK_RATE_MS);
        }
        else if (xSystemError.bBluetoothError == 1) {
            // Blink Red LED for 5 times.
            vBlinkLed(LED_RED, 5);
            vTaskDelay(800 / portTICK_RATE_MS);
        }
        else if (xSystemError.bObjectsOverflowError == 1) {
            // Blink Red LED for 6 times.
            vBlinkLed(LED_RED, 6);
            vTaskDelay(800 / portTICK_RATE_MS);
        }

        

        // SD card is busy.
        else if (xSystemState.bSdBusy == 1) {
            xSystemState.bSdBusy = 0;

            // Turn on yellow LED.
            vSetLed(LED_YELLOW);
            vTaskDelay(50 / portTICK_RATE_MS);
        }
        
        // Communicationg with bluetooth module.
        else if (xSystemState.bBluetoothBusy == 1) {
            xSystemState.bBluetoothBusy = 0;

            // Blink the cyan LED quickly.
            vSetLed(LED_CYAN);
            vTaskDelay(50 / portTICK_RATE_MS);
            vSetLed(LED_OFF);
            vTaskDelay(50 / portTICK_RATE_MS);
        }
        
        // Bluetooth Mode.
        else if ((xSystemState.bBluetoothMode == 1) && (BT_CONNECTED == 1)) {
            // Turn on blue LED.
            vSetLed(LED_BLUE);
            vTaskDelay(200 / portTICK_RATE_MS);
        }

        // Standby mode.
        else if (xSystemState.bStandby == 1) {
            // Blink Green LED every second.
            vBlinkLed(LED_GREEN, 1);
            vTaskDelay(600 / portTICK_RATE_MS);
        }



        // Default state, delay for 200ms to let other task to run.
        else {
            // Turn off LED.
            vSetLed(LED_OFF);
            vTaskDelay(200 / portTICK_RATE_MS);
        }



        // Read the stack watermark and record it if it's below threshold.
        unsigned portBASE_TYPE uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        if (uxHighWaterMark < MIN_STACK_WATERMARK) {
            xSystemError.bStackLowError = 1;

            // Only log it when the watermark value changed.
            static portBASE_TYPE uxPreviousWatermark = 0;
            if (uxHighWaterMark != uxPreviousWatermark) {
                vLogStackWatermark("LED Blinking Task", (unsigned short)uxHighWaterMark);
            }
            uxPreviousWatermark = uxHighWaterMark;
        }
    }
}



/*******************************************************************************
 * TASK: taskMonitorUsb
 *
 * DESCRIPTIONS:
 * Check if the USB is connected/disconnected.
 *
 *******************************************************************************/
void taskMonitorUsb (void *pvParameters)
{
    while (1) {
        // USB is plugged in.
        if(USB_BUS_SENSE == 1) {
            if (xSystemState.bUsbMode == 0) {
                SoftReset();
            }
        }
        // USB is unpluged.
        else {
            if (xSystemState.bUsbMode == 1) {
                SoftReset();
            }
        }


        // Loop again after 500ms.
        vTaskDelay(500 / portTICK_RATE_MS);



        // Read the stack watermark and record it if it's below threshold.
        unsigned portBASE_TYPE uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        if (uxHighWaterMark < MIN_STACK_WATERMARK) {
            xSystemError.bStackLowError = 1;

            // Only log it when the watermark value changed.
            static portBASE_TYPE uxPreviousWatermark = 0;
            if (uxHighWaterMark != uxPreviousWatermark) {
                vLogStackWatermark("USB Monitor Task", (unsigned short)uxHighWaterMark);
            }
            uxPreviousWatermark = uxHighWaterMark;
        }
    }
}
