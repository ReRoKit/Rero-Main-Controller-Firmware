/*******************************************************************************
 * Functions for playing motion file.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include <string.h>
#include <strings.h>
#include "PlayMotion.h"
#include "HardwareProfile.h"
#include "Variables.h"
#include "FatFs/FatFsWrapper.h"
#include "AudioAmp.h"
#include "Log.h"
#include "GUI/GraphicScreens.h"
#include "GUI/MotionPage.h"
#include "G15 and Sensors/G15.h"
#include "G15 and Sensors/Sensors.h"

// Header files for FreeRTOS.
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
* DEFINITION                                                                   *
*******************************************************************************/

// Link list for the opened motion file.
typedef struct {
    void* pvNextObj;                        // Pointer to the next object.
    char szFileName[MAX_FILENAME_LENGTH];   // Motion file name without the extension.
    FSFILE *pxMotionFile;                   // Motion file handle.
    MOTION_STATE eMotionState;              // State of the motion.
} MOTION_OBJ;


// Command byte.
typedef union {
    BYTE byte;
    struct {
        unsigned RD_WR		:1;     // Read = 0, Write = 1.
        unsigned LOOP		:1;
        unsigned RESET		:1;
        unsigned RUN_SERVO	:1;
        unsigned TIME_SPEED :1;     // File created by Animator = 0; Teach mode = 1;
        unsigned RESERVED	:3;
    };
} COMMAND;


// Control byte.
typedef union {
    BYTE byte;
    struct {
        unsigned DEST_HIGH  :2;
        unsigned TYPE       :2;
        unsigned WHEEL		:1;
        unsigned LED		:1;
        unsigned TORQUE		:1;
        unsigned DIR		:1;
    };
} CONTROL;



/*******************************************************************************
 * PRIVATE GLOBAL VARIABLES
 *******************************************************************************/

// The motion object playing list.
static volatile MOTION_OBJ *prv_pxFirstMotionObj = NULL;

// Flag to indicate whether the G15 is currently in use.
static volatile unsigned char prv_pucG15Lock[EM_MAX_ID + 1] = {0};

static unsigned char prv_ucPlaying = 0;


/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 *******************************************************************************/

static void prv_vAddMotionObj(MOTION_OBJ* pxNewMotionObj);
static MOTION_OBJ* prv_pxFindMotionObj(const char* szFileName);
static void prv_vRemoveMotionObj(MOTION_OBJ* pxMotionObj);

static void prv_vTrapServoError(unsigned char ucId);
static void prv_vDisableUsedOutput(void);


/*******************************************************************************
 * FUNCTION: prv_vAddMotionObj
 *
 * PARAMETERS:
 * ~ pxNewMotionObj   - Pointer to the new motion object.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Add a new object to the motion object list.
 *
 *******************************************************************************/
static void prv_vAddMotionObj(MOTION_OBJ* pxNewMotionObj)
{
    MOTION_OBJ* pxCurrentObj;

    // If the list is empty, add it to the first item.
    if (prv_pxFirstMotionObj == NULL) {
        prv_pxFirstMotionObj = pxNewMotionObj;
    }

    // Else, add it to the last item.
    else {
        pxCurrentObj = (MOTION_OBJ *)prv_pxFirstMotionObj;

        while (pxCurrentObj->pvNextObj != NULL) {
            pxCurrentObj = (MOTION_OBJ*)pxCurrentObj->pvNextObj;
        }

        pxCurrentObj->pvNextObj = (void*)pxNewMotionObj;
    }

    pxNewMotionObj->pvNextObj = NULL;
}



/*******************************************************************************
 * FUNCTION: prv_pxFindMotionObj
 *
 * PARAMETERS:
 * ~ szFileName   - Motion file name without extension.
 *
 * RETURN:
 * ~ Pointer to the motion object.
 *
 * DESCRIPTIONS:
 * Find the motion object by its file name.
 *
 *******************************************************************************/
static MOTION_OBJ* prv_pxFindMotionObj(const char* szFileName)
{
    MOTION_OBJ* pxMotionObj = (MOTION_OBJ *)prv_pxFirstMotionObj;

    // Loop for all items in the list.
    while (pxMotionObj != NULL) {
        // Return the object if the file name matches
        if (strncasecmp(szFileName, pxMotionObj->szFileName, MAX_FILENAME_LENGTH) == 0) {
            return pxMotionObj;
        }

        pxMotionObj = (MOTION_OBJ*)pxMotionObj->pvNextObj;
    }

    return NULL;
}



/*******************************************************************************
 * FUNCTION: prv_vRemoveMotionObj
 *
 * PARAMETERS:
 * ~ pxMotionObj   - Pointer to the motion object that we want to remove.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Remove an object from the motion object list.
 *
 *******************************************************************************/
static void prv_vRemoveMotionObj(MOTION_OBJ* pxMotionObj)
{
    // Make sure the list is not empty.
    if (prv_pxFirstMotionObj != NULL) {
        // If the object is the first item in the list,
        // Point the list to the next item.
        if (pxMotionObj == prv_pxFirstMotionObj) {
            prv_pxFirstMotionObj = prv_pxFirstMotionObj->pvNextObj;
        }
        else {
            MOTION_OBJ* pxPreviousObj = (MOTION_OBJ *)prv_pxFirstMotionObj;
            MOTION_OBJ* pxCurrentObj = prv_pxFirstMotionObj->pvNextObj;

            while (pxCurrentObj != NULL) {
                if (pxMotionObj == pxCurrentObj) {
                    pxPreviousObj->pvNextObj = pxCurrentObj->pvNextObj;
                    break;
                }

                pxPreviousObj = pxCurrentObj;
                pxCurrentObj = pxCurrentObj->pvNextObj;
            }
        }
    }

    // Close the file.
    xSemaphoreTake(xSdCardMutex,portMAX_DELAY);
    FSfclose(pxMotionObj->pxMotionFile);
    xSemaphoreGive(xSdCardMutex);

    // Free the motion object.
    free((void*)pxMotionObj);
}



/*******************************************************************************
 * FUNCTION: prv_vTrapServoError
 *
 * PARAMETERS:
 * ~ ucId   - ID for the servo that causes the error.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Display the error message and trap the servo error until stop button
 * is pressed.
 *
 *******************************************************************************/
static void prv_vTrapServoError(unsigned char ucId)
{
    static char cIdText[] = "ID: 000";
    sprintf(cIdText, "ID = %03u", ucId);

    vUpdateMotionPageMsg1("Servo Error:");
    vUpdateMotionPageMsg2(cIdText);

    // Turn off torque and LED for used output modules.
    prv_vDisableUsedOutput();

    // Wait until no more motion is playing - stop button is pressed?
    while (prv_ucPlaying != 0) {
        vTaskDelay(100 / portTICK_RATE_MS);
    }
}



/*******************************************************************************
 * FUNCTION: prv_vDisableUsedOutput
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Turn off torque and LED for used output module.
 *
 *******************************************************************************/
static void prv_vDisableUsedOutput(void)
{
    unsigned char i;
    for (i = 0; i <= EM_MAX_ID; i++) {
        if (prv_pucG15Lock[i] == 1) {
            // Turn off torque and LED for servo.
            eG15SetTorqueLed(i, WRITE_NOW, 0, 0);
            eG15SetSpeed(i, WRITE_NOW, 0, POSITION_SPEED_CONTROL);
            
            // We don't check for other output modules here because Animator 
            // only supports Cube Servos at the time of writing this function.
        }
    }
}



/*******************************************************************************
 * PUBLIC FUNCTION: ePlayMotionStart
 *
 * PARAMETERS:
 * ~ szMotionFileName   - File name for the motion file without the extension.
 *
 * RETURN:
 * ~ Play result.
 *
 * DESCRIPTIONS:
 * Start playing motion.
 *
 *******************************************************************************/
PLAY_RESULT ePlayMotionStart(const char* szMotionFileName)
{
    // Return error if the motion file is already in the playing list.
    if (prv_pxFindMotionObj(szMotionFileName) != NULL) {
        return PLAY_FILE_ALREADY_OPENED;
    }



    // Get the full file path and add the extension for motion file.
    static char szFullFilePath[MAX_FILENAME_LENGTH * 3];
    strcpy(szFullFilePath, szProgramFolder);
    strcat(szFullFilePath, "/");
    strcat(szFullFilePath, szMotionFileName);
    strcat(szFullFilePath, szMotionFileExt);

    // Open the motion file.
    xSemaphoreTake(xSdCardMutex,portMAX_DELAY);
    FSFILE* pxMotionFile = FSfopen(szFullFilePath, "r");
    xSemaphoreGive(xSdCardMutex);

    // Return error if fail to open the motion file.
    if (pxMotionFile == NULL) {
        return PLAY_CANT_OPEN_FILE;
    }


    
    // Create a motion object and add to the playing list.
    MOTION_OBJ* pxMotionObject = (MOTION_OBJ*)malloc(sizeof(MOTION_OBJ));
    if (pxMotionObject == NULL) {
        xSystemError.bMallocError = 1;

        // Log the malloc error to SD card.
        vLogMallocError("Play Motion - Motion Object");
    }

    strncpy(pxMotionObject->szFileName, szMotionFileName, MAX_FILENAME_LENGTH);
    pxMotionObject->pxMotionFile = pxMotionFile;
    pxMotionObject->eMotionState = PLAY;

    prv_vAddMotionObj(pxMotionObject);



    // Create the motion task.
    // Remove the motion object if task cannot be created.
    if (xTaskCreate(taskPlayMotion, "PlayMotion", 300, (void*)pxMotionObject, tskIDLE_PRIORITY + 3, NULL) != pdPASS) {
        prv_vRemoveMotionObj(pxMotionObject);
        return PLAY_CANT_CREATE_TASK;
    }

    prv_ucPlaying = 1;
    
    return PLAY_NO_ERROR;
}



/*******************************************************************************
 * PUBLIC FUNCTION: vPlayMotionStop
 *
 * PARAMETERS:
 * ~ szMotionFileName   - File name for the motion file.
 * ~ eStopMode          - STOP_NOW   : Stop immediately.
 *                      - STOP_FRAME : Stop after the current time frame.
 *                      - STOP_FILE  : Stop after the whole file.
 *                      - STOP_BT    : Stop immediately for wheel mode, stop file
 *                                     for position mode.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Stop the motion.
 *
 *******************************************************************************/
void vPlayMotionStop(const char* szMotionFileName, MOTION_STATE eStopMode)
{
    if (szMotionFileName != NULL) {
        MOTION_OBJ* pxMotionObject = prv_pxFindMotionObj(szMotionFileName);

        // Make sure the motion file is in the playing list.
        if (pxMotionObject != NULL) {
            pxMotionObject->eMotionState = eStopMode;
        }
        prv_ucPlaying = 0;
    }
}



/*******************************************************************************
 * PUBLIC FUNCTION: vPlayMotionStopAll
 *
 * PARAMETERS:
 * ~ eStopMode          - STOP_NOW   : Stop immediately.
 *                      - STOP_FRAME : Stop after the current time frame.
 *                      - STOP_FILE  : Stop after the whole file.
 *                      - STOP_BT    : Stop immediately for wheel mode, stop file
 *                                     for position mode.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Stop all the motion in the playing list.
 *
 *******************************************************************************/
void vPlayMotionStopAll(MOTION_STATE eStopMode)
{
    MOTION_OBJ* pxMotionObject = (MOTION_OBJ *)prv_pxFirstMotionObj;

    // Loop for all the motion object in the playing list.
    while (pxMotionObject != NULL) {
        pxMotionObject->eMotionState = eStopMode;

        pxMotionObject = pxMotionObject->pvNextObj;
    }
    prv_ucPlaying = 0;
}



/*******************************************************************************
 * PUBLIC FUNCTION: ucIsMotionPlaying
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ Return 0 if no motion file is playing.
 * ~ Return 1 if at least one motion file is playing.
 *
 * DESCRIPTIONS:
 * Check whether is there any motion file playing.
 *
 *******************************************************************************/
unsigned char ucIsMotionPlaying(void)
{
    if (prv_pxFirstMotionObj == NULL) {
        return 0;
    } else {
        return 1;
    }
}



/*******************************************************************************
* TASK: taskPlayMotion
*
* DESCRIPTIONS:
* Play the motion file.
*
*******************************************************************************/
void taskPlayMotion (void *pvParameters)
{
    unsigned short i;

    MOTION_OBJ* pxMotionObject = (MOTION_OBJ*)pvParameters;
    unsigned short usDelay = 0;
    
    unsigned char pucBuffer[5] = {0};

    COMMAND xCommand;
    CONTROL xControl;
    
    unsigned char ucTotalServo;
    unsigned short usTotalTimeFrame;
    unsigned short usStartTime;
    unsigned short usMaxDuration;
    unsigned short usMinDuration;
    unsigned char ucIsAllServoWheelMode = 1;

    EM_MODEL eModel;

    // The timer tick is in 100ms.
    unsigned short usPlayTimer = 0;

    // The timer tick needed to read from SD card and send command to G15.
    portTickType ulProcessingTick = 0;

    
    

    // Read the first 2 bytes of data (Header, Command).
    xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
    FSfread(pucBuffer, 1, 2, pxMotionObject->pxMotionFile);
    xSemaphoreGive(xSdCardMutex);

    // Make sure the header is 0xFA.
    // Exit task if it's not.
    if(pucBuffer[0] != 0xFA) {
        // Delay a while.
        vTaskDelay(100 / portTICK_RATE_MS);

        // Remove the object from the playing list.
        prv_vRemoveMotionObj(pxMotionObject);

        // Update the play page GUI when finish playing.
        vUpdateMotionPageEndPlaying(MOTION_FILE);

        // Give the semaphore to indicate finish playing the motion.
        xSemaphoreGive(xPlayingMotionSemaphore);

        vTaskDelete(NULL);
    }

    // Get the command.
    xCommand.byte = pucBuffer[1];



    // Skip the file comment and read the total servo.
    xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
    FSfseek(pxMotionObject->pxMotionFile, MAX_FILENAME_LENGTH, SEEK_CUR);
    FSfread(&ucTotalServo, 1, 1, pxMotionObject->pxMotionFile);
    xSemaphoreGive(xSdCardMutex);

    // Exit task if # of servo = 0;
    if(ucTotalServo == 0) {
        // Delay a while.
        vTaskDelay(100 / portTICK_RATE_MS);

        // Remove the object from the playing list.
        prv_vRemoveMotionObj(pxMotionObject);

        // Update the play page GUI when finish playing.
        vUpdateMotionPageEndPlaying(MOTION_FILE);

        // Give the semaphore to indicate finish playing the motion.
        xSemaphoreGive(xPlayingMotionSemaphore);

        vTaskDelete(NULL);
    }

    // Reserve the memory for servo ID.
    unsigned char *pucG15Id = (unsigned char *)malloc(ucTotalServo);
    if (pucG15Id == NULL) {
        xSystemError.bMallocError = 1;

        // Log the malloc error to SD card.
        vLogMallocError("Play Motion - G15 ID");

        // Delay a while.
        vTaskDelay(100 / portTICK_RATE_MS);

        // Remove the object from the playing list.
        prv_vRemoveMotionObj(pxMotionObject);

        // Update the play page GUI when finish playing.
        vUpdateMotionPageEndPlaying(MOTION_FILE);

        // Give the semaphore to indicate finish playing the motion.
        xSemaphoreGive(xPlayingMotionSemaphore);

        vTaskDelete(NULL);
    }



    xSemaphoreTake(xSdCardMutex, portMAX_DELAY);

    // Read the servo ID.
    FSfread (pucG15Id, 1, ucTotalServo, pxMotionObject->pxMotionFile);
    
    // Read the # of time frame.
    FSfread (pucBuffer, 1, 2, pxMotionObject->pxMotionFile);
    usTotalTimeFrame = (unsigned short)pucBuffer[0] << 8 ;
    usTotalTimeFrame |= (unsigned short)pucBuffer[1];

    xSemaphoreGive(xSdCardMutex);



    // Take control of all the servo used.
    // If one of the servo is used by other motion, wait until it finishes.
    unsigned char ucLock;
    do {
        ucLock = 0;

        portENTER_CRITICAL();

        // Is the G15 locked?
        for (i = 0; i < ucTotalServo; i++) {
            ucLock |= prv_pucG15Lock[pucG15Id[i]];
        }

        // If none of the G15 is locked by other motion, lock them.
        if (ucLock == 0) {
            for (i = 0; i < ucTotalServo; i++) {
                prv_pucG15Lock[pucG15Id[i]] = 1;
            }
            portEXIT_CRITICAL();
        }

        // Delay and check again after 100ms.
        else {
            portEXIT_CRITICAL();
            vTaskDelay(100 / portTICK_RATE_MS);
        }
        
    } while (ucLock != 0);



    // Get the current tick time.
    portTickType prevTick = xTaskGetTickCount();


    // Loop for a complete motion file.
    while ((pxMotionObject->eMotionState == PLAY) || (pxMotionObject->eMotionState == PAUSE)) {
        // Reset the timer tick.
        usPlayTimer = 0;

        // Go to the beginning of the first time frame and read the start time.
        xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
        FSfseek(pxMotionObject->pxMotionFile, ucTotalServo + MAX_FILENAME_LENGTH + 5, SEEK_SET);
        FSfread (pucBuffer, 1, 2, pxMotionObject->pxMotionFile);
        usStartTime = (unsigned short)pucBuffer[0] << 8 ;
        usStartTime |= (unsigned short)pucBuffer[1];
        xSemaphoreGive(xSdCardMutex);



        // Loop for each time frame.
        unsigned short usTimeFrameCount;
        for (usTimeFrameCount = 0; usTimeFrameCount < usTotalTimeFrame; usTimeFrameCount++) {

            // Read the # of servo for this time frame.
            unsigned char ucTimeFrameTotalServo;
            xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
            FSfread (&ucTimeFrameTotalServo, 1, 1, pxMotionObject->pxMotionFile);
            xSemaphoreGive(xSdCardMutex);



            // Loop for each servo.
            unsigned char ucServoCount;
            for (ucServoCount = 0; ucServoCount < ucTimeFrameTotalServo; ucServoCount++) {
                // Read 5 bytes of data for this servo (ID, Control, Control, End time MSB, End time LSB).
                xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
                FSfread (pucBuffer, 1, 5, pxMotionObject->pxMotionFile);
                xSemaphoreGive(xSdCardMutex);

                pucG15Id[ucServoCount] = pucBuffer[0];
                xControl.byte = pucBuffer[1];

                unsigned short usEndTime;
                usEndTime = (unsigned short)pucBuffer[3] << 8;
                usEndTime |= (unsigned short)pucBuffer[4];

                // Calculate the duration.
                unsigned short usDuration = usEndTime - usStartTime;

                // Calculate the maximum and minimum duration.
                // Use the duration directly if this is the first servo.
                if (ucServoCount == 0) {
                    usMaxDuration = usDuration;
                    usMinDuration = usDuration;
                }
                else {
                    // Get the minimum duration.
                    if (usMinDuration > usDuration) {
                        usMinDuration = usDuration;
                    }

                    // Get the maximum duration.
                    if (usMaxDuration < usDuration) {
                        usMaxDuration = usDuration;
                    }
                }
                
                // Play wave file 0-10 if servo ID <= 10 and LED is on.
                if ((pucG15Id[ucServoCount] <= 10) && (xControl.LED != 0)) {
                    switch (pucG15Id[ucServoCount]) {
                        case 0: vPlayWaveFile("/Program/Wave0.wav");    break;
                        case 1: vPlayWaveFile("/Program/Wave1.wav");    break;
                        case 2: vPlayWaveFile("/Program/Wave2.wav");    break;
                        case 3: vPlayWaveFile("/Program/Wave3.wav");    break;
                        case 4: vPlayWaveFile("/Program/Wave4.wav");    break;
                        case 5: vPlayWaveFile("/Program/Wave5.wav");    break;
                        case 6: vPlayWaveFile("/Program/Wave6.wav");    break;
                        case 7: vPlayWaveFile("/Program/Wave7.wav");    break;
                        case 8: vPlayWaveFile("/Program/Wave8.wav");    break;
                        case 9: vPlayWaveFile("/Program/Wave9.wav");    break;
                        case 10: vPlayWaveFile("/Program/Wave10.wav");  break;
                    }
                }
                
                

                // Get the model of the module.
                eEMGetModel(pucG15Id[ucServoCount], &eModel);


                // If this frame is servo...
                if ((xControl.TYPE == 0b00) && (eModel == EM_MODEL_G15)) {
                    // On/Off Torque & LED.
                    // Do not wait for action command.
                    eG15SetTorqueLed(pucG15Id[ucServoCount], WRITE_NOW, xControl.TORQUE, xControl.LED);

                    // Only update the other parameters if Torque is turned on.
                    if(xControl.TORQUE) {
                        // Wheel mode.
                        if (xControl.WHEEL == 1) {
                            // Set the servo speed and direction.
                            unsigned short usSpeed = (unsigned short)pucBuffer[2] + ((unsigned short)xControl.DEST_HIGH << 8);
                            G15_SPEED_CONTROL_MODE eSpeedMode = (xControl.DIR == 1)? WHEEL_CW : WHEEL_CCW;

                            eG15SetSpeed(pucG15Id[ucServoCount], WAIT_ACTION, usSpeed, eSpeedMode);
                        }

                        // Position mode.
                        else {
                            // Indicate we have servo which is not wheel mode.
                            ucIsAllServoWheelMode = 0;

                            // Get the destination in degree.
                            unsigned short usDestination;
                            usDestination = (unsigned short)xControl.DEST_HIGH << 8;
                            usDestination |= (unsigned short)pucBuffer[2];

                            // Convert it to 11-bit data.
                            usDestination = (unsigned short)((unsigned long)usDestination * 1088ul / 360ul);

                            // Set the position and speed.
                            // Write to register and wait for action command.
#ifdef SHOWCASE_ROBOT
                            // The error checking only applies to servos in position mode.
                            EM_ERROR eErrorCode = EM_ERR_INVALID_MODULE;
                            eErrorCode = eG15SetPositionSpeed(  pucG15Id[ucServoCount], WAIT_ACTION,
                                                                usDestination, NORMAL_POSITIONING,
                                                                usDuration, POSITION_TIME_CONTROL   );
                            // If there is error, stop playing and display error message until stop button is pressed.
                            if (eErrorCode != EM_NO_ERROR) {
                                prv_vTrapServoError(pucG15Id[ucServoCount]);
                            }
#else
                            eG15SetPositionSpeed(  pucG15Id[ucServoCount], WAIT_ACTION,
                                                                usDestination, NORMAL_POSITIONING,
                                                                usDuration, POSITION_TIME_CONTROL   );
#endif
                        }
                    }
                }

                // If this frame is head module...
                else if ((xControl.TYPE == 0b01) && (eModel == EM_MODEL_HEAD)) {
                    // Set the RGB LED.
                    eHeadSetLed(pucG15Id[ucServoCount], pucBuffer[2], (unsigned char)xControl.DEST_HIGH);
                }

            }   // End of loop for each servo.
 
            // Run all servos.
            eEMSetAction();

            // Calculate the processing tick if it's not done yet.
            if (ulProcessingTick == 0) {
                ulProcessingTick = xTaskGetTickCount() - prevTick;
            }



            // If this is last time frame, the delay is the longest duration.
            if (usTimeFrameCount == (usTotalTimeFrame - 1)) {
                usDelay = usMaxDuration;
            }

            // Else, read the start time of next time frame and calculate the delay.
            else {
                // Read the start time of next time frame.
                unsigned short usNextStartTime;
                xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
                FSfread (pucBuffer, 1, 2, pxMotionObject->pxMotionFile);
                usNextStartTime = (unsigned short)pucBuffer[0] << 8 ;
                usNextStartTime |= (unsigned short)pucBuffer[1];
                xSemaphoreGive(xSdCardMutex);
                

                // Calculate the delay.
                usDelay = usNextStartTime - usStartTime;
                usStartTime = usNextStartTime;
            }



            // Wait until the next time frame.
            // The delay value is in 100ms.
            for (i = 0; i < usDelay; i++) {
                // Break the loop if the motion is stopped immediately.
                if (pxMotionObject->eMotionState == STOP_NOW) {
                    break;
                }
                // Break the loop if the motion is stopped in bluetooth mode and all servos are in wheel mode.
                else if ((pxMotionObject->eMotionState == STOP_BT) && (ucIsAllServoWheelMode == 1)) {
                    break;
                }

                //Display the time frame and time.
                vUpdateMotionPageTimeFrame(usTimeFrameCount + 1, usPlayTimer / 10);

                // Increase the timer tick.
                usPlayTimer++;

                // Delay for 100ms.
                vTaskDelayUntil(&prevTick, 100 / portTICK_RATE_MS);
            }

            // Break the time frame loop if the motion is stopped.
            if (    (pxMotionObject->eMotionState == STOP_NOW) ||
                    (pxMotionObject->eMotionState == STOP_FRAME)    ) {
                break;
            }
            // Also break the time frame loop if the motion is stopped in bluetooth mode and all servos are in wheel mode.
            else if ((pxMotionObject->eMotionState == STOP_BT) && (ucIsAllServoWheelMode == 1)) {
                break;
            }

        }   // End of loop for each time frame.



        // Break the motion loop if the loop flag is cleared.
        if (xCommand.LOOP == 0) {
            break;
        }

        


        // Read the stack watermark and record it if it's below threshold.
        unsigned portBASE_TYPE uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        if (uxHighWaterMark < MIN_STACK_WATERMARK) {
            xSystemError.bStackLowError = 1;

            // Only log it when the watermark value changed.
            static portBASE_TYPE uxPreviousWatermark = 0;
            if (uxHighWaterMark != uxPreviousWatermark) {
                vLogStackWatermark("Play Motion Task", (unsigned short)uxHighWaterMark);
            }
            uxPreviousWatermark = uxHighWaterMark;
        }

    }   // End of loop for complete motion file.



    // Delay for the processing time to allow the servo complete it's current frame.
    if (    (pxMotionObject->eMotionState != STOP_NOW) &&
            ((pxMotionObject->eMotionState != STOP_BT) || (ucIsAllServoWheelMode != 1))    ) {
        vTaskDelayUntil(&prevTick, ulProcessingTick);
    }



    // Read the servo ID for all servos.
    xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
    FSfseek(pxMotionObject->pxMotionFile, MAX_FILENAME_LENGTH + 3, SEEK_SET);
    FSfread (pucG15Id, 1, ucTotalServo, pxMotionObject->pxMotionFile);
    xSemaphoreGive(xSdCardMutex);


    // Turn off LED for all modules.
    // Maintain the torque and current position for all servos.
    for (i = 0; i < ucTotalServo; i++) {
        // Get the model of the module.
        eEMGetModel(pucG15Id[i], &eModel);

        if (eModel == EM_MODEL_G15) {
            // Set the target position to the current position.
            // Only do this if the motion is not yet finish playing.
            if (pxMotionObject->eMotionState != PLAY) {
                unsigned short usPosition;
                eG15GetPosition(pucG15Id[i], &usPosition);
                eG15SetPosition(pucG15Id[i], WRITE_NOW, usPosition, NORMAL_POSITIONING);
            }
            
            // Set speed to 0. This is to stop the G15 if it's wheel mode.
            eG15SetSpeed(pucG15Id[i], WRITE_NOW, 0, POSITION_SPEED_CONTROL);
        }

        else if (EM_MODEL_HEAD) {
            // Turn off LED for head module.
            eHeadSetLed(pucG15Id[i], 0, 0);
        }
    }



    // Release control of all the servo used.
    for (i = 0; i < ucTotalServo; i++) {
        prv_pucG15Lock[pucG15Id[i]] = 0;
    }


    // Release the memory for G15 ID.
    free((void*)pucG15Id);

    // Update the play page GUI when finish playing.
    vUpdateMotionPageEndPlaying(MOTION_FILE);

    // Give the semaphore to indicate finish playing the motion.
    xSemaphoreGive(xPlayingMotionSemaphore);
    
    // Remove the object from the playing list.
    prv_vRemoveMotionObj(pxMotionObject);

    prv_ucPlaying = 0;
    
    // Delete this task after complete.
    vTaskDelete(NULL);
}
