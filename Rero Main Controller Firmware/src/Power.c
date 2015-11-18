/*******************************************************************************
 * Functions to handle the power.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "Power.h"
#include "UserProgram.h"
#include "Planner.h"
#include "TimeDelay.h"
#include "Variables.h"
#include "HardwareProfile.h"
#include "Background.h"
#include "Log.h"
#include "GUI/GraphicScreens.h"
#include "G15 and Sensors/G15.h"
#include "FatFs/FatFsWrapper.h"
#include "Bluetooth/uart2.h"
#include "AudioAmp.h"
#include "Sound Stream/Shutdown.h"
#include "Sound Stream/Error.h"
#include "Sound Stream/TouchTone.h"

#include "Graphics/Graphics.h"
#include "GUI/Custom Graphic Object/Battery.h"
#include "GUI/PopUpMessagePage.h"
#include "Image Decoders/ImageDecoder.h"
#include "Image Decoders/BmpDecoder.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

//-------------------------------------------
//  Vsense = Vbatt * 2.7k / (10k + 2.7k)
//  ADCval = Vsense * 1023 / 3.3
//-------------------------------------------
//  Eg: Vsense1 = 9.9 * 2.7 / 12.7
//              = 2.1047v
//      ADCval  = 2.1047 * 1023 / 3.3
//              = 652
//-------------------------------------------

// ADC value for battery percentage.
#define BATT_ADC_0      635        // 0% - 3.21V
#define BATT_ADC_10     728        // 10% - 3.68V
#define BATT_ADC_20     741        // 20% - 3.75V
#define BATT_ADC_30     749        // 30% - 3.79V
#define BATT_ADC_40     753        // 40% - 3.81V
#define BATT_ADC_50     759        // 50% - 3.84V
#define BATT_ADC_60     767        // 60% - 3.88V
#define BATT_ADC_70     779        // 70% - 3.94V
#define BATT_ADC_80     791        // 80% - 4.00V
#define BATT_ADC_90     809        // 90% - 4.09V
#define BATT_ADC_100    830        // 100% - 4.2V



/*******************************************************************************
* TASK: taskBattMonitor
*
* DESCRIPTIONS:
* Task to monitor the battery voltage.
*
*******************************************************************************/
void taskBattMonitor( void *pvParameters )
{
    unsigned int uiShutdownSample = 0;
    unsigned int uiWarningSample = 0;

    portTickType xLastWakeTime = xTaskGetTickCount();

    while (1) {
        // Read the battery voltage.
        unsigned int uiOffset = 8 * ((~ReadActiveBufferADC10() & 0x01));    // Determine which buffer is idle and create an offset.
        unsigned int uiBattAdc = ReadADC10(uiOffset);                       // Read the result of channel 4 conversion from the idle buffer.

        // Determine the battery percentage.
        unsigned int uiBattPercentageTemp;
        if (uiBattAdc > BATT_ADC_90) {
            uiBattPercentageTemp = 100;
        }
        else if (uiBattAdc > BATT_ADC_80) {
            uiBattPercentageTemp = 90;
        }
        else if (uiBattAdc > BATT_ADC_70) {
            uiBattPercentageTemp = 80;
        }
        else if (uiBattAdc > BATT_ADC_60) {
            uiBattPercentageTemp = 70;
        }
        else if (uiBattAdc > BATT_ADC_50) {
            uiBattPercentageTemp = 60;
        }
        else if (uiBattAdc > BATT_ADC_40) {
            uiBattPercentageTemp = 50;
        }
        else if (uiBattAdc > BATT_ADC_30) {
            uiBattPercentageTemp = 40;
        }
        else if (uiBattAdc > BATT_ADC_20) {
            uiBattPercentageTemp = 30;
        }
        else if (uiBattAdc > BATT_ADC_10) {
            uiBattPercentageTemp = 20;
        }
        else if (uiBattAdc > BATT_ADC_0) {
            uiBattPercentageTemp = 10;
        }
        else {
            uiBattPercentageTemp = 0;
        }

        // Low pass filter the battery percentage for display and redraw the battery icon.
        // Use the value directly for the first time.
        if (uiBattPercentage > 100) {
            uiBattPercentage = uiBattPercentageTemp;
        }
        else {
            uiBattPercentage = ((uiBattPercentage * 9) + (uiBattPercentageTemp * 1)) / 10;
        }
        
        
        OBJ_HEADER  *pObj = GOLFindObject(GID_BATT_ICON);
        if (pObj != NULL) {
            SetState(pObj, BATT_DRAW);
        }
        
        

        // Create shut down screen if voltage is <= 10%.
        // Make sure the value is consistant by sampling 5 times.
        if (uiBattPercentage <= 10) {
            if (++uiShutdownSample >= 5) {
                uiShutdownSample = 0;

                // Display the low batt shutdown screen.
                vCreateLowBattShutdownMessage();
                
                // Play the error sound for 5 seconds and shut down.
                unsigned char i;
                for (i = 0; i < 4; i++) {
                    vPlaySoundStream(&xErrorSound);
                    DelayMs(1250);
                }
                
                // Shutting down.
                vPowerOff();
                vTaskSuspend(NULL);
                // Program should stop here.
            }
        }
        else {
            uiShutdownSample = 0;
            uiWarningSample = 0;
        }



        // Read the stack watermark and record it if it's below threshold.
        unsigned portBASE_TYPE uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        if (uxHighWaterMark < MIN_STACK_WATERMARK) {
            xSystemError.bStackLowError = 1;

            // Only log it when the watermark value changed.
            static portBASE_TYPE uxPreviousWatermark = 0;
            if (uxHighWaterMark != uxPreviousWatermark) {
                vLogStackWatermark("Battery Monitor Task", (unsigned short)uxHighWaterMark);
            }
            uxPreviousWatermark = uxHighWaterMark;
        }


        // Loop every 1s.
        vTaskDelayUntil(&xLastWakeTime, 1000 / portTICK_RATE_MS);
    }
}



/*******************************************************************************
 * FUNCTION: vPowerOn
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Turn on power to both main board and sub board.
 *
 *******************************************************************************/
void vPowerOn(void)
{
    POWER_MAIN = 1;
    POWER_SUB = 1;
}



/*******************************************************************************
 * FUNCTION: vPowerOff
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the task to show the shutdown screen and power off.
 * If the RTOS is not yet running, call the task directly.
 *
 *******************************************************************************/
void vPowerOff(void)
{
    xTaskCreate(taskPowerOff, "Off", 400, NULL, configMAX_PRIORITIES - 1, NULL);
    
    // Start the scheduler if the RTOS task is not running.
    if (xSystemState.bRtosRunning == 0) {
        // Start the RTOS running, this function should never return
        vTaskStartScheduler();

        // Insufficient memory.
        while (1) {
            vSetLed(LED_WHITE);
            DelayMs(500);
            vSetLed(LED_OFF);
            DelayMs(500);
        }
    }
}



/*******************************************************************************
 * TASK: taskPowerOff
 *
 * DESCRIPTIONS:
 * Task to show the shutdown screen and power off.
 *
 *******************************************************************************/
void taskPowerOff (void *pvParameters)
{
    vSetGuiPage(PAGE_SHUTDOWN);
    
    // Stop the set path task and discard the limits if set path is running.
    if (xSystemState.bSetPath == 1) {
        vSetPathDiscard();

        // Delay 200ms to wait for set path complete.
        vTaskDelay(200 / portTICK_RATE_MS);
    }
    
    // Stop the running planner file.
    if (ucIsPlannerPlaying() != 0) {
        vPlannerStopCurrent(STOP_NOW);
        
        // Wait until the planner file stop running.
        while (ucIsPlannerPlaying() != 0) {
            vTaskDelay(50 / portTICK_RATE_MS);
        }
    }
    
    // Stop the running motion file.
    if (ucIsMotionPlaying() != 0) {
        vPlayMotionStopAll(STOP_NOW);
        
        // Wait until the motion file stop running.
        while (ucIsMotionPlaying() != 0) {
            vTaskDelay(50 / portTICK_RATE_MS);
        }
    }
    
    
    
    // Turn on red LED.
    vSetLed(LED_RED);
    
    // Turn on the backlight.
    SetDCOC1PWM(BLight);

    // Clear the GOL objects and screen.
    GOLFree();
    SetColor(pxDefaultScheme->CommonBkColor);
    ClearDevice();

    // Show the rero logo.
    xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
    FSFILE *pImageFile = FSfopen("/Theme/Rero.bmp", "r");
    if (pImageFile != NULL) {
        WORD wWidth = 98;
        WORD wHeight = 24;
        WORD wStartX = (GetMaxX() - wWidth) / 2;
        WORD wStartY = 80;
        ImageDecode(pImageFile, IMG_BMP, wStartX, wStartY, wWidth, wHeight, IMG_ALIGN_CENTER, NULL, NULL);
        FSfclose(pImageFile);
    }
    xSemaphoreGive(xSdCardMutex);
    
    // Show the shut down message.
    vDisplayShutdownText("Shutting down...");
    
    // Play shutdown sound.
    vPlaySoundStream(&xShutdownSound);
    
    
    
    // Power off the Sub-PCB.
    POWER_SUB = 0;
    DelayMs(200);
    
    // Try to ping the external module and see whether the Sub-PCB is already off.
    // We will ping with broadcast ID. If there is multiple module connected, the response will be corrupted.
    // But this doesn't matter. We just want to check is there any respond from the module.
    unsigned char ucId;
    for (ucId = 0; ucId <= EM_MAX_ID; ucId++) {
        if (eEMPing(ucId) != EM_ERR_RX_TIMEOUT) {
            // We still get the response from external module.
            // Ask the user to unplug the battery instead.
            vCreateShutdownErrorMessage();

            // Play error sound.
            unsigned char i;
            for (i = 0; i < 4; i++) {
                vPlaySoundStream(&xErrorSound);
                DelayMs(1300);
            }
            
            // Suspend all other task and stop here.
            vTaskSuspendAll();
            while (1);
        }
    }
    
    
    
    // Show the shut down message.
    vDisplayShutdownText("Bye bye!");

    // Wait until shutdown sound is finish playing.
    // Timeout after 2 second.
    unsigned short usTimeout = 0;
    while (ucIsAudioPlaying() != 0) {
        DelayMs(10);
        if (++usTimeout > 200) {
            break;
        }
    }
    
    // Turn off the backlight.
    SetDCOC1PWM(0);

    // Power off.
    POWER_MAIN = 0;
    
    // Suspend all other task and stop here.
    vTaskSuspendAll();
    while (1);
}



/*******************************************************************************
 * ISR: INT1 External Interrupt.
 *
 * DESCRIPTIONS:
 * This ISR will be run whenever there is falling edge on the INT1 pin.
 *
 *******************************************************************************/
void __ISR(_EXTERNAL_1_VECTOR, IPL7AUTO) INT1Interrupt(void)
{
    mINT1ClearIntFlag();

    // Enable Timer 1 interrupt.
    mT1IntEnable(1);
}



/*******************************************************************************
 * ISR: Timer 1 Interrupt.
 *
 * DESCRIPTIONS:
 * This ISR is run every 100ms.
 * Used to keep track of how long the power button is pressed.
 * Turn off the board if it's pressed for > 500ms.
 * Force turn off the board if it's pressed for > 5 seconds.
 *
 *******************************************************************************/
void __ISR(_TIMER_1_VECTOR, IPL6AUTO) TMR1Interrupt(void)
{
    static unsigned char ucShuttingDown = 0;
    static unsigned short i = 0;

    mT1ClearIntFlag();

    // If the power button is still pressed.
    if (POWER_BUTTON == 0) {
        i++;
        
        // Force turn off the board if it's pressed for > 5 seconds.
        if (i > 50) {
            // Turn off LED.
            vSetLed(LED_OFF);
            
            // Turn off the backlight.
            SetDCOC1PWM(0);
            
            // Turn off the power.
            POWER_SUB = 0;
            POWER_MAIN = 0;
            while (1);
        }
        
        // Turn off the board if it's pressed for > 500ms.
        else if (i > 5) {
            // Make sure we are not shutting down.
            if (ucShuttingDown == 0) {
                ucShuttingDown = 1;
                vPowerOff();
            }
        }
    }
    
    // If the power button is released.
    else {
         // Reset the counter and disable the Timer 1 interrupt
        i = 0;
        mT1IntEnable(0);
        

        // Make sure the RTOS is already running and the shutdown task is not running.
        if ((xSystemState.bRtosRunning == 1) && (ucShuttingDown == 0)) {
            // If the program button is pressed, save the screenshot.
            if (PROGRAM_BUTTON == 0) {
                vCaptureScreenshot();
            }
            else {
                // Lock the screen.
                vLockScreen();

                // Reset the taskStandbyWatchdog.
                xSemaphoreGive(xTouchSemaphore);
            }
        }

    }
}
