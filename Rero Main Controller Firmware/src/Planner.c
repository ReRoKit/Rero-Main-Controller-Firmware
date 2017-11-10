/*******************************************************************************
 * Functions to handle the rero planner file.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "Planner.h"
#include "PlayMotion.h"
#include "Variables.h"
#include "Log.h"
#include "FatFs/FatFsWrapper.h"
#include "G15 and Sensors/Sensors.h"
#include "G15 and Sensors/G15.h"
#include "GUI/MotionPage.h"
#include "GUI/GraphicScreens.h"

// Header files for FreeRTOS.
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

// Block type.
typedef enum __attribute__((packed)) {
    MOTION_BLOCK            = 0x00,
    AUDIO_BLOCK             = 0x10,
    SENSOR_BLOCK_IR         = 0x20,
    SENSOR_BLOCK_CL_AMBIENT = 0x21,
    SENSOR_BLOCK_ULTRASONIC = 0x22,
    SENSOR_BLOCK_HEAD_IR    = 0x24,
    SENSOR_BLOCK_HEAD_MIC   = 0x26,
    SENSOR_BLOCK_COLOUR     = 0x27,
    SENSOR_BLOCK_LINE       = 0x28,
    SENSOR_BLOCK_TACTILE    = 0x29,
    SENSOR_BLOCK_ANALOG_IN  = 0x2a,
    COUNTER_BLOCK           = 0x40,
    COUNTER_BLOCK_2         = 0x41,
    DELAY_BLOCK             = 0x60,
    LINE_FOLLOW_BLOCK       = 0x70,
    CONTROL_SERVO_BLOCK     = 0x80,
    CONTROL_HEAD_BLOCK      = 0x81,
    CONTROL_RGB_LIGHT_BLOCK = 0x82,
    CONTROL_2WD_BLOCK       = 0x83
} BLOCK_TYPE;



/*******************************************************************************
* PRIVATE GLOBAL VARIABLES
*******************************************************************************/

static char prv_szMotionFilename[MAX_FILENAME_LENGTH * 2] = {0};
static char prv_szOpenedFilename[MAX_FILENAME_LENGTH] = {0};
static char prv_szFullFilePath[MAX_FILENAME_LENGTH * 3];
static FSFILE *prv_pxOpenedFile = NULL;
static unsigned char prv_ucLock = 0;
static unsigned char prv_ucPlaying = 0;
static EM_MODEL prv_peUsedOutputModule[EM_MAX_ID + 1] = {0};



/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 *******************************************************************************/

static void prv_vTrapSensorError(unsigned char ucId);
static void prv_vTrapServoError(unsigned char ucId);
static void prv_vDisableUsedOutput(void);



/*******************************************************************************
 * FUNCTION: prv_vTrapSensorError
 *
 * PARAMETERS:
 * ~ ucId   - ID for the sensor that causes the error.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Display the error message and trap the sensor error until stop button
 * is pressed.
 *
 *******************************************************************************/
static void prv_vTrapSensorError(unsigned char ucId)
{
    static char cIdText[] = "ID: 000";
    sprintf(cIdText, "ID = %03u", ucId);

    vUpdateMotionPageMsg1("SensorError:");
    vUpdateMotionPageMsg2(cIdText);

    // Turn off torque and LED for used output modules.
    prv_vDisableUsedOutput();

    // Wait until stop button is pressed.
    while (prv_ucPlaying != 0) {
        vTaskDelay(100 / portTICK_RATE_MS);
    }
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

    // Wait until stop button is pressed.
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
        // Turn off torque and LED for servo.
        if (prv_peUsedOutputModule[i] == EM_MODEL_G15) {
            eG15SetTorqueLed(i, WRITE_NOW, 0, 0);
            eG15SetSpeed(i, WRITE_NOW, 0, POSITION_SPEED_CONTROL);
        }

        // Turn off LED for head module.
        else if (prv_peUsedOutputModule[i] == EM_MODEL_HEAD) {
            eHeadSetLed(i, 0, 0);
        }
		
        // Turn off RGB LED for Light module.
        else if (prv_peUsedOutputModule[i] == EM_MODEL_RGBLIGHT) {
            eRgbLightModuleSetRgb(i, 0, 0, 0);
        }
    }
}



/*******************************************************************************
* PUBLIC FUNCTION: ePlannerRun
*
* PARAMETERS:
* ~ szPlannerFileName   - File name for the planner file (without extension).
*
* RETURN:
* ~ Play result.
*
* DESCRIPTIONS:
* Start running the planner file.
*
*******************************************************************************/
PLAY_RESULT ePlannerRun(const char* szPlannerFileName)
{
    // Return error code if another planner file is running.
    // Only one planner file can run at one time.
    if (prv_pxOpenedFile != NULL) {
        return PLAY_FILE_ALREADY_OPENED;
    }



    // Get the planner file name and add the extension.
    strcpy(prv_szFullFilePath, szProgramFolder);
    strcat(prv_szFullFilePath, "/");
    strcat(prv_szFullFilePath, szPlannerFileName);
    strcat(prv_szFullFilePath, szPlannerFileExt);

    
    
    xSemaphoreTake(xSdCardMutex,portMAX_DELAY);
    // Save the planner file lock state.
    if (xFSGetReadOnlyFlag(prv_szFullFilePath, &prv_ucLock) != FR_OK) {
        prv_ucLock = 0;
    }
    
    // Clear the read-only flag.
    xFSSetReadOnlyFlag(prv_szFullFilePath, 0);
    
    // Open the planner file.
    prv_pxOpenedFile = FSfopen(prv_szFullFilePath, "r+");
    xSemaphoreGive(xSdCardMutex);

    
    
    // Return error code if fail to open the planner file.
    if (prv_pxOpenedFile == NULL) {
        // Restore the original read-only flag.
        xSemaphoreTake(xSdCardMutex,portMAX_DELAY);
        xFSSetReadOnlyFlag(prv_szFullFilePath, prv_ucLock);
        xSemaphoreGive(xSdCardMutex);
        
        return PLAY_CANT_OPEN_FILE;
    }

    // Return error code if can't create the planner task.
    if (xTaskCreate(taskPlanner, "Planner", 400, NULL, tskIDLE_PRIORITY, NULL) == pdFALSE) {
        xSemaphoreTake(xSdCardMutex,portMAX_DELAY);
        // Restore the original read-only flag.
        xFSSetReadOnlyFlag(prv_szFullFilePath, prv_ucLock);
        
        // Close the planner file.
        FSfclose(prv_pxOpenedFile);
        prv_pxOpenedFile = NULL;
        xSemaphoreGive(xSdCardMutex);

        return PLAY_CANT_CREATE_TASK;
    }

    // Save the planner file name and set the playing flag.
    strcpy(prv_szOpenedFilename, szPlannerFileName);
    prv_ucPlaying = 1;

    return PLAY_NO_ERROR;
}



/*******************************************************************************
 * PUBLIC FUNCTION: vPlannerStop
 *
 * PARAMETERS:
 * ~ szPlannerFileName  - File name for the planner file.
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
 * Stop running the planner file.
 *
 *******************************************************************************/
void vPlannerStop(const char* szPlannerFileName, MOTION_STATE eStopMode)
{
    // Make sure there is planner file playing.
    if (prv_szOpenedFilename[0] != 0) {
        // Make sure the planner file name matched.
        if (strcmp(szPlannerFileName, prv_szOpenedFilename) == 0) {
            // Stop playing.
            prv_ucPlaying = 0;

            // Stop the playing motion.
            vPlayMotionStop(prv_szMotionFilename, eStopMode);
        }
    }
}



/*******************************************************************************
 * PUBLIC FUNCTION: vPlannerStop
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
 * Stop running the current planner file.
 *
 *******************************************************************************/
void vPlannerStopCurrent(MOTION_STATE eStopMode)
{
    // Make sure there is planner file playing.
    if (prv_szOpenedFilename[0] != 0) {
        // Stop playing.
        prv_ucPlaying = 0;

        // Stop the playing motion.
        vPlayMotionStop(prv_szMotionFilename, eStopMode);
    }
}



/*******************************************************************************
 * PUBLIC FUNCTION: ucDeleteProgramFiles
 *
 * PARAMETERS:
 * ~ szFilenameHead  - Begining of the file name that we want to delete.
 *
 * RETURN:
 * ~ Return 0 if all files are deleted successfully.
 * ~ Return 1 if some / all files are read only.
 *
 * DESCRIPTIONS:
 * Delete all the files in the program folder start with the specified file name.
 *
 *******************************************************************************/
unsigned char ucDeleteProgramFiles(const char *szFilenameHead)
{
    unsigned char ucReadOnly = 0;
    static char szLongFileName[(MAX_FILENAME_LENGTH * 2) + 4] = {0};
    static char szFullFilePath[(MAX_FILENAME_LENGTH * 3) + 4] = {0};

    static DIR xDirectory;
    static FILINFO xFileInfo;
    xFileInfo.lfname = szLongFileName;
    xFileInfo.lfsize = sizeof(szLongFileName);

    xSemaphoreTake(xSdCardMutex, portMAX_DELAY);

    // Search for and delete the associated motion file.
    // The motion file name starts with the planner file name.
    // Open the directory of the planner file and make sure there is no error.
    if (f_opendir(&xDirectory, szProgramFolder) == FR_OK) {
        // Loop through the list of file and check the read only flag.
        do {
            // Read the file list.
            if (f_readdir(&xDirectory, &xFileInfo) != FR_OK) {
                break;
            }

            // Get the filename.
            char *szFileName = (xFileInfo.lfname[0] != 0)? xFileInfo.lfname : xFileInfo.fname;

            // If the file name begins with the planner file name,
            // Check the read-only flag.
            if (strncmp(szFilenameHead, szFileName, strlen(szFilenameHead)) == 0) {
                if ((xFileInfo.fattrib & AM_RDO) != 0) {
                    ucReadOnly = 1;
                }
            }
        } while (xFileInfo.fname[0] != 0);
        
        
        
        // Delete the files if all the files are not read only.
        if (ucReadOnly == 0) {
            // Reset the directory index.
            f_readdir(&xDirectory, NULL);

            // Loop through the list of file again.
            // Delete the file if the filename begins with the filename head.
            do {
                // Read the file list.
                if (f_readdir(&xDirectory, &xFileInfo) != FR_OK) {
                    break;
                }

                // Get the filename.
                char *szFileName = (xFileInfo.lfname[0] != 0)? xFileInfo.lfname : xFileInfo.fname;

                // If the file name begins with the planner file name,
                // Delete the file.
                if (strncmp(szFilenameHead, szFileName, strlen(szFilenameHead)) == 0) {
                    // Get the full path for the file.
                    strcpy(szFullFilePath, szProgramFolder);
                    strcat(szFullFilePath, "/");
                    strcat(szFullFilePath, szFileName);

                    // Delete the file.
                    f_unlink(szFullFilePath);
                }
            } while (xFileInfo.fname[0] != 0);
        }
        
        f_closedir(&xDirectory);
    }

    xSemaphoreGive(xSdCardMutex);
    
    return ucReadOnly;
}



/*******************************************************************************
 * PUBLIC FUNCTION: ucIsPlannerPlaying
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ Return 0 if no planner file is playing.
 * ~ Return 1 if planner file is playing.
 *
 * DESCRIPTIONS:
 * Check whether is there any planner file playing.
 *
 *******************************************************************************/
unsigned char ucIsPlannerPlaying(void)
{
    if (prv_szOpenedFilename[0] == 0) {
        return 0;
    } else {
        return 1;
    }
}



/*******************************************************************************
* TASK: taskPlanner
*
* DESCRIPTIONS:
* Task to run the planner file.
*
*******************************************************************************/
void taskPlanner(void *pvParameters)
{
    unsigned short usCounter[10] = {0};
    unsigned long ulBlockAddress = 0x00000013;
    unsigned char pucBuffer[20] = {0};
    
    
    
    // Reset the used output module list.
    unsigned int i;
    for (i = 0; i <= EM_MAX_ID; i++) {
        prv_peUsedOutputModule[i] = EM_MODEL_NONE;
    }


    // Read the first byte of data (Header).
    xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
    FSfread(pucBuffer, 1, 1, prv_pxOpenedFile);
    xSemaphoreGive(xSdCardMutex);

    // Make sure the header is 0xFB.
    // Exit task if it's not.
    if(pucBuffer[0] != 0xFB) {
        xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
        // Restore the original read-only flag.
        xFSSetReadOnlyFlag(prv_szFullFilePath, prv_ucLock);
        
        // Close the planner file and set the state to closed.
        FSfclose(prv_pxOpenedFile);
        prv_pxOpenedFile = NULL;
        xSemaphoreGive(xSdCardMutex);

        vTaskDelete(NULL);
    }



    // Loop until next block address is 0x00ffffff.
    while ((ulBlockAddress != 0) && (ulBlockAddress != 0x00ffffff) && (prv_ucPlaying != 0)) {
        // Go to next block and read the whole block into the buffer.
        xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
        FSfseek(prv_pxOpenedFile, ulBlockAddress, SEEK_SET);
        FSfread (pucBuffer, 1, 20, prv_pxOpenedFile);
        xSemaphoreGive(xSdCardMutex);

        
        
        // Check the block type.
        unsigned char ucBlockType = pucBuffer[0];
        switch (ucBlockType) {
            // Play motion block.
            case MOTION_BLOCK: {
                // Make sure the semaphore is taken.
                // It will be given when the motion finish playing.
                xSemaphoreTake(xPlayingMotionSemaphore, 0);


                // Motion file name starts with the planner file name.
                // Ex: Planner = File1.rpl, Motion = File1_Motion1.rmo.
                // Make sure it doesn't switch task when creating the file name until start playing the motion file.
                vTaskEnterCritical();
                char *szMotionBlockName = &pucBuffer[1];
                strcpy(prv_szMotionFilename, prv_szOpenedFilename);
                strcat(prv_szMotionFilename, "_");
                strcat(prv_szMotionFilename, szMotionBlockName);
                
                
                // Update the message text in play page.
                vUpdateMotionPageMsg2(szMotionBlockName);

                // Play the motion file.
                PLAY_RESULT ePlayResult = ePlayMotionStart(prv_szMotionFilename);
                vTaskExitCritical();
                
                // If successful to open the motion file, wait until the motion finish playing.
                if (ePlayResult == PLAY_NO_ERROR) {
                    xSemaphoreTake(xPlayingMotionSemaphore, portMAX_DELAY);
                    
                    // Clear the time and frame.
                    vUpdateMotionPageTimeFrame(0, 0);
                } else {
                    // Update the message text in play page.
                    vUpdateMotionPageMsg1("MotionError:");

                    // Turn off torque and LED for used output modules.
                    prv_vDisableUsedOutput();

                    // Wait until stop button is pressed.
                    while (prv_ucPlaying != 0) {
                        vTaskDelay(100 / portTICK_RATE_MS);
                    }
                }

                // Get the next block address.
                ulBlockAddress = ((unsigned long)pucBuffer[17] << 16) + ((unsigned long)pucBuffer[18] << 8) + (unsigned long)pucBuffer[19];
                break;
            }


            // Play audio block.
            case AUDIO_BLOCK: {
                // Make sure the semaphore is taken.
                // It will be given when the motion finish playing.
                xSemaphoreTake(xPlayingMotionSemaphore, 0);


                // Motion file name starts with the planner file name.
                // Ex: Planner = File1.rpl, Motion = File1_Motion1.rmo.
                // Make sure it doesn't switch task when creating the file name until start playing the motion file.
                vTaskEnterCritical();
                char *szAudioBlockName = &pucBuffer[1];
                strcpy(prv_szMotionFilename, "/Program/");
                strcat(prv_szMotionFilename, prv_szOpenedFilename);
                strcat(prv_szMotionFilename, "_");
                strcat(prv_szMotionFilename, szAudioBlockName);
                
                // Update the message text in play page.
                vUpdateMotionPageMsg2(szAudioBlockName);
                
                // Play the motion file.
                vPlayWaveFile(prv_szMotionFilename);
                vTaskExitCritical();
                
                // Get the next block address.
                ulBlockAddress = ((unsigned long)pucBuffer[17] << 16) + ((unsigned long)pucBuffer[18] << 8) + (unsigned long)pucBuffer[19];
                break;
            }
            
            
            
            // Sensor block for IR, ultrasonic, head IR and head mic.
            case SENSOR_BLOCK_IR:
            case SENSOR_BLOCK_ULTRASONIC:
            case SENSOR_BLOCK_HEAD_IR:
            case SENSOR_BLOCK_HEAD_MIC:
            case SENSOR_BLOCK_CL_AMBIENT: {
                // Update the message text in play page.
                vUpdateMotionPageMsg2("Reading sensor");

                // Get the sensor ID and threshold.
                unsigned char ucSensorId = pucBuffer[1];
                unsigned short usSensorThreshold = ((unsigned short)pucBuffer[2] << 8) + (unsigned short)pucBuffer[3];

                // Read the sensor value.
                EM_ERROR eErrorCode = EM_ERR_INVALID_MODULE;
                unsigned short usSensorValue = 0;
                switch (ucBlockType) {
                    case SENSOR_BLOCK_IR:           eErrorCode = eInfraredSensorGetValue(ucSensorId, (unsigned char*)&usSensorValue);   break;
                    case SENSOR_BLOCK_ULTRASONIC:   eErrorCode = eUltrasonicSensorGetValue(ucSensorId, &usSensorValue);                 break;
                    case SENSOR_BLOCK_HEAD_IR:      eErrorCode = eHeadGetIrValue(ucSensorId, (unsigned char*)&usSensorValue);           break;
                    case SENSOR_BLOCK_HEAD_MIC:     eErrorCode = eHeadGetMicValue(ucSensorId, (unsigned char*)&usSensorValue);          break;
                    case SENSOR_BLOCK_CL_AMBIENT:   eErrorCode = eColourSensorGetAmbient(ucSensorId, (unsigned char*)&usSensorValue);   break;
                }
                
                // If there is error reading the sensor, stop playing and display error message until stop button is pressed.
                if (eErrorCode != EM_NO_ERROR) {
                    prv_vTrapSensorError(ucSensorId);
                }

                // Compare the sensor value with the threshold and get the next block address.
                if (usSensorValue > usSensorThreshold) {
                    ulBlockAddress = ((unsigned long)pucBuffer[4] << 16) + ((unsigned long)pucBuffer[5] << 8) + (unsigned long)pucBuffer[6];
                } else {
                    ulBlockAddress = ((unsigned long)pucBuffer[7] << 16) + ((unsigned long)pucBuffer[8] << 8) + (unsigned long)pucBuffer[9];
                }

                break;
            }
            
            
            
            // Sensor block for colour sensor.
            case SENSOR_BLOCK_COLOUR: {
                // Update the message text in play page.
                vUpdateMotionPageMsg2("Reading sensor");

                // Get the sensor ID and target colour.
                unsigned char ucSensorId = pucBuffer[1];
                CL_SENSOR_COLOUR eTargetColour = pucBuffer[2];
                
                
                
                // Read the colour sensor for a few times and make sure the reading is consistent.
                CL_SENSOR_COLOUR eDetectedColour;
                CL_SENSOR_COLOUR eLastDetectedColour;
                unsigned char i;
                for (i = 0; i < 10; i++) {
                    // Read the colour from the sensor.
                    // If there is error reading the sensor, stop playing and display error message until stop button is pressed.
                    if (eColourSensorGetColour(ucSensorId, &eDetectedColour) != EM_NO_ERROR) {
                        prv_vTrapSensorError(ucSensorId);
                    }
                    
                    // Ignore the first reading.
                    // For the remaining reading, make sure it's the same.
                    if (i > 0) {
                        // If the colour is not consistent, assume nothing is detected.
                        if (eDetectedColour != eLastDetectedColour) {
                            eDetectedColour = CL_BLACK;
                            break;
                        }
                    }
                    
                    // Save the current reading as last reading.
                    eLastDetectedColour = eDetectedColour;
                    
                    // Delay before taking the next reading.
                    vTaskDelay(10 / portTICK_RATE_MS);
                }
                
                
                
                // Get the next block address.
                if ( (eDetectedColour == eTargetColour) ||
                     ((eTargetColour == CL_ANY_COLOUR) && (eDetectedColour != CL_BLACK)) ) {
                    ulBlockAddress = ((unsigned long)pucBuffer[3] << 16) + ((unsigned long)pucBuffer[4] << 8) + (unsigned long)pucBuffer[5];
                } else {
                    ulBlockAddress = ((unsigned long)pucBuffer[6] << 16) + ((unsigned long)pucBuffer[7] << 8) + (unsigned long)pucBuffer[8];
                }
                
                break;
            }
            
            
            
            // Sensor block for line sensor.
            case SENSOR_BLOCK_LINE: {
                // Update the message text in play page.
                vUpdateMotionPageMsg2("Reading sensor");

                // Get the sensor ID and target sensor value.
                unsigned char ucSensorId = pucBuffer[1];
                unsigned char ucTargetValue = pucBuffer[2] & 0x0f;
                
                // Read the digital value from the line sensor.
                // If there is error reading the sensor, stop playing and display error message until stop button is pressed.
                unsigned char ucSensorValue;
                if (eLineSensorGetDigital(ucSensorId, &ucSensorValue) != EM_NO_ERROR) {
                    prv_vTrapSensorError(ucSensorId);
                }
                ucSensorValue &= 0x0f;
                
                // Get the next block address.
                if (ucSensorValue == ucTargetValue) {
                    ulBlockAddress = ((unsigned long)pucBuffer[3] << 16) + ((unsigned long)pucBuffer[4] << 8) + (unsigned long)pucBuffer[5];
                } else {
                    ulBlockAddress = ((unsigned long)pucBuffer[6] << 16) + ((unsigned long)pucBuffer[7] << 8) + (unsigned long)pucBuffer[8];
                }
                
                break;
            }
            
            
            
            // Sensor block for tactile sensor.
            case SENSOR_BLOCK_TACTILE: {
                // Update the message text in play page.
                vUpdateMotionPageMsg2("Reading sensor");

                // Get the sensor ID.
                unsigned char ucSensorId = pucBuffer[1];
                
                // Read the value from the tactile sensor.
                // If there is error reading the sensor, stop playing and display error message until stop button is pressed.
                unsigned char ucSensorValue;
                if (eTactileSensorGetValue(ucSensorId, &ucSensorValue) != EM_NO_ERROR) {
                    prv_vTrapSensorError(ucSensorId);
                }
                
                // Get the next block address.
                if (ucSensorValue > 0) {
                    ulBlockAddress = ((unsigned long)pucBuffer[2] << 16) + ((unsigned long)pucBuffer[3] << 8) + (unsigned long)pucBuffer[4];
                } else {
                    ulBlockAddress = ((unsigned long)pucBuffer[5] << 16) + ((unsigned long)pucBuffer[6] << 8) + (unsigned long)pucBuffer[7];
                }
                
                break;
            }
            
            
            
            // Sensor block for analog input module.
            case SENSOR_BLOCK_ANALOG_IN: {
                // Update the message text in play page.
                vUpdateMotionPageMsg2("Reading sensor");

                // Get the sensor ID.
                unsigned char ucSensorId = pucBuffer[1];
                
                // Get the sensor channel.
                unsigned char ucSensorChannel = pucBuffer[2];
                
                // Get the sensor threshold.
                unsigned short usSensorThreshold = ((unsigned short)pucBuffer[3] << 8) + (unsigned short)pucBuffer[4];
                
                // Read the value from the analog input module.
                // If there is error reading the sensor, stop playing and display error message until stop button is pressed.
                unsigned short usSensorValue = 0;
                if (eAnalogInputModuleGetValue(ucSensorId, ucSensorChannel, (unsigned char*)&usSensorValue) != EM_NO_ERROR) {
                    prv_vTrapSensorError(ucSensorId);
                }
                
                // Compare the sensor value with the threshold and get the next block address.
                if (usSensorValue > usSensorThreshold) {
                    ulBlockAddress = ((unsigned long)pucBuffer[5] << 16) + ((unsigned long)pucBuffer[6] << 8) + (unsigned long)pucBuffer[7];
                } else {
                    ulBlockAddress = ((unsigned long)pucBuffer[8] << 16) + ((unsigned long)pucBuffer[9] << 8) + (unsigned long)pucBuffer[10];
                }
                
                break;
            }
            
            
            
            // Counter block.
            case COUNTER_BLOCK: {
                // Update the message text in play page.
                vUpdateMotionPageMsg2("Counter");
                
                // Get the counter ID and limit.
                unsigned char ucCounterId = pucBuffer[1];
                unsigned short usLimit = ((unsigned short)pucBuffer[2] << 8) + (unsigned short)pucBuffer[3];
                
                // Increase the counter value.
                usCounter[ucCounterId]++;
                
                // Get the next block address.
                if (usCounter[ucCounterId] > usLimit) {
                    ulBlockAddress = ((unsigned long)pucBuffer[4] << 16) + ((unsigned long)pucBuffer[5] << 8) + (unsigned long)pucBuffer[6];
                    
                    // Reset the counter value.
                    usCounter[ucCounterId] = 0;
                } else {
                    ulBlockAddress = ((unsigned long)pucBuffer[7] << 16) + ((unsigned long)pucBuffer[8] << 8) + (unsigned long)pucBuffer[9];
                }
                
                break;
            }
            
            
            
            // Counter block 2 (with Reset option).
            case COUNTER_BLOCK_2: {
                // Update the message text in play page.
                vUpdateMotionPageMsg2("Counter");
                
                // Get the counter ID and limit.
                unsigned char ucCounterId = pucBuffer[1];
                unsigned short usLimit = ((unsigned short)pucBuffer[2] << 8) + (unsigned short)pucBuffer[3];
                unsigned char ucReset = pucBuffer[4];
                
                // Increase the counter value.
                usCounter[ucCounterId]++;
                
                // Get the next block address.
                if (usCounter[ucCounterId] > usLimit) {
                    ulBlockAddress = ((unsigned long)pucBuffer[5] << 16) + ((unsigned long)pucBuffer[6] << 8) + (unsigned long)pucBuffer[7];
                    
                    if (ucReset == 1) {
                        // Reset the counter value.
                        usCounter[ucCounterId] = 0;
                    } else {
                        // No reset
                        usCounter[ucCounterId]--;
                    }
                } else {
                    ulBlockAddress = ((unsigned long)pucBuffer[8] << 16) + ((unsigned long)pucBuffer[9] << 8) + (unsigned long)pucBuffer[10];
                }
                
                break;
            }
            
            
            
            // Delay block.
            case DELAY_BLOCK: {
                // Update the message text in play page.
                vUpdateMotionPageMsg2("Delay");

                // Get the delay period. It's in multiple of 0.1 second.
                unsigned long ulDelay = ((unsigned long)pucBuffer[1] << 8) + (unsigned long)pucBuffer[2];

                // Delay
                while (ulDelay-- > 0) {
                    vTaskDelay(100 / portTICK_RATE_MS);

                    // Break if stop playing.
                    if (prv_ucPlaying == 0) {
                        break;
                    }
                }

                // Get the next block address.
                ulBlockAddress = ((unsigned long)pucBuffer[3] << 16) + ((unsigned long)pucBuffer[4] << 8) + (unsigned long)pucBuffer[5];
                break;
            }


            
            // Control Servo block.
            case CONTROL_SERVO_BLOCK: {
                // Update the message text in play page.
                vUpdateMotionPageMsg2("Control servo");

                // Get the servo ID and indicate that it's used.
                unsigned char ucServoId = pucBuffer[1];
                prv_peUsedOutputModule[ucServoId] = EM_MODEL_G15;

                // Turn On/Off Torque & LED.
                EM_ERROR eErrorCode = EM_ERR_INVALID_MODULE;
                eErrorCode = eG15SetTorqueLed(ucServoId, WRITE_NOW, pucBuffer[4], pucBuffer[5]);
                
#ifdef SHOWCASE_ROBOT
                // The error checking only applies to servos in position mode.
                // If there is error, stop playing and display error message until stop button is pressed.
                if (eErrorCode != EM_NO_ERROR) {
                    prv_vTrapServoError(ucServoId);
                }
#endif
                
                // Wheel mode.
                if (pucBuffer[2] == 1) {
                    // Set the servo speed and direction.
                    unsigned short usSpeed = ((unsigned short)pucBuffer[6] << 8) + (unsigned short)pucBuffer[7];
                    G15_SPEED_CONTROL_MODE eSpeedMode = (pucBuffer[3] == 0)? WHEEL_CCW : WHEEL_CW;
                    eG15SetSpeed(ucServoId, WRITE_NOW, usSpeed, eSpeedMode);
                }

                // Position mode.
                else {
                    // Get the duration and destination.
                    unsigned short usDuration = ((unsigned short)pucBuffer[6] << 8) + (unsigned short)pucBuffer[7];
                    unsigned short usDestination = ((unsigned short)pucBuffer[8] << 8) + (unsigned short)pucBuffer[9];

                    // Convert the destination to 11-bit data.
                    usDestination = (unsigned short)((unsigned long)usDestination * 1088ul / 360ul);

                    // Set the position and speed.
                    eG15SetPositionSpeed(ucServoId, WRITE_NOW, usDestination, NORMAL_POSITIONING, usDuration, POSITION_TIME_CONTROL);
                }

                // Get the next block address.
                ulBlockAddress = ((unsigned long)pucBuffer[10] << 16) + ((unsigned long)pucBuffer[11] << 8) + (unsigned long)pucBuffer[12];

                break;
            }



            // Control Head Module LED block.
            case CONTROL_HEAD_BLOCK: {
                // Update the message text in play page.
                vUpdateMotionPageMsg2("Control head LED");

                // Get the head module ID and indicate that it's used.
                unsigned char ucHeadId = pucBuffer[1];
                prv_peUsedOutputModule[ucHeadId] = EM_MODEL_HEAD;

                // Set the LED colour for head module.
                unsigned char ucLeftRight = pucBuffer[2];
                unsigned char ucRgb = pucBuffer[3];
                eHeadSetLed(ucHeadId, ucRgb, ucLeftRight);

                // Get the next block address.
                ulBlockAddress = ((unsigned long)pucBuffer[4] << 16) + ((unsigned long)pucBuffer[5] << 8) + (unsigned long)pucBuffer[6];
                
                break;
            }



            // Control Light Module.
            case CONTROL_RGB_LIGHT_BLOCK: {
                // Update the message text in play page.
                vUpdateMotionPageMsg2("Control RGB light");

                // Get the light module ID and indicate that it's used.
                unsigned char ucModuleId = pucBuffer[1];
                prv_peUsedOutputModule[ucModuleId] = EM_MODEL_RGBLIGHT;

                // Set the RGB LED colour for light module.
                unsigned char ucRed = pucBuffer[2];
                unsigned char ucGreen = pucBuffer[3];
                unsigned char ucBlue = pucBuffer[4];
                eRgbLightModuleSetRgb(ucModuleId, ucRed, ucGreen, ucBlue);

                // Get the next block address.
                ulBlockAddress = ((unsigned long)pucBuffer[5] << 16) + ((unsigned long)pucBuffer[6] << 8) + (unsigned long)pucBuffer[7];
                
                break;
            }



            // Control 2WD block.
            case CONTROL_2WD_BLOCK: {
                // Update the message text in play page.
                vUpdateMotionPageMsg2("Control 2WD");

                // Get the servo IDs and indicate that it's used.
                unsigned char ucLeftServoId = pucBuffer[1];
                unsigned char ucRightServoId = pucBuffer[2];
                prv_peUsedOutputModule[ucLeftServoId] = EM_MODEL_G15;
                prv_peUsedOutputModule[ucRightServoId] = EM_MODEL_G15;

                // Turn On/Off Torque & LED.
                eG15SetTorqueLed(ucLeftServoId, WRITE_NOW, pucBuffer[4], pucBuffer[5]);
                eG15SetTorqueLed(ucRightServoId, WRITE_NOW, pucBuffer[4], pucBuffer[5]);

                // Set the servos speed and direction.
                unsigned short usSpeed = ((unsigned short)pucBuffer[6] << 8) + (unsigned short)pucBuffer[7];
                
                G15_SPEED_CONTROL_MODE eLeftSpeedMode = WHEEL_CCW;
                G15_SPEED_CONTROL_MODE eRightSpeedMode = WHEEL_CW;
                        
                // Set the servos direction
                // 0 - Stop
                // 1 - Forward
                // 2 - Turn Left
                // 3 - Turn Right
                // 4 - Reverse
                switch (pucBuffer[3]) {
                    case 0: {
                        usSpeed = 0;
                        break;
                    }
                    case 1: {
                        eLeftSpeedMode = WHEEL_CCW;
                        eRightSpeedMode =  WHEEL_CW;
                        break;
                    }
                    case 2: { 
                        eLeftSpeedMode = WHEEL_CW;
                        eRightSpeedMode =  WHEEL_CW;
                        break;
                    }
                    case 3: { 
                        eLeftSpeedMode = WHEEL_CCW;
                        eRightSpeedMode =  WHEEL_CCW;
                        break;
                    }
                    case 4: {
                        eLeftSpeedMode = WHEEL_CW;
                        eRightSpeedMode =  WHEEL_CCW;
                        break;
                    }
                }
                
                eG15SetSpeed(ucLeftServoId, WRITE_NOW, usSpeed, eLeftSpeedMode);
                eG15SetSpeed(ucRightServoId, WRITE_NOW, usSpeed, eRightSpeedMode);
                
                // Get the next block address.
                ulBlockAddress = ((unsigned long)pucBuffer[8] << 16) + ((unsigned long)pucBuffer[9] << 8) + (unsigned long)pucBuffer[10];

                break;
            }



            // Line Follow block.
            case LINE_FOLLOW_BLOCK: {
                // Update the message text in play page.
                vUpdateMotionPageMsg2("Line Follow");

                // Get the servo IDs and indicate that it's used.
                unsigned char ucLineSensorId = pucBuffer[1];
                unsigned char ucLeftServoId = pucBuffer[2];
                unsigned char ucRightServoId = pucBuffer[3];
                prv_peUsedOutputModule[ucLeftServoId] = EM_MODEL_G15;
                prv_peUsedOutputModule[ucRightServoId] = EM_MODEL_G15;

                // Get the desired max speed
                unsigned short usSpeed_Max = ((unsigned short)pucBuffer[4] << 8) + (unsigned short)pucBuffer[5];
                
                // Turn On/Off Torque & LED.
                eG15SetTorqueLed(ucLeftServoId, WRITE_NOW, 1, pucBuffer[6]);
                eG15SetTorqueLed(ucRightServoId, WRITE_NOW, 1, pucBuffer[6]);

                // Set servos direction.
                G15_SPEED_CONTROL_MODE eLeftSpeedMode = WHEEL_CCW;
                G15_SPEED_CONTROL_MODE eRightSpeedMode = WHEEL_CW;
                        
                // Set junction counter
                unsigned char ucStopAtJunction = pucBuffer[7];
                unsigned char ucJunctionCount = 0;
                _Bool bJunctionFlag = 0;
                
                do{
                    unsigned short usSpeed_Left, usSpeed_Right;
                
                    // Read the digital value for each sensor from line sensor.
                    unsigned char ucLineValue;
                    eLineSensorGetDigital(ucLineSensorId, &ucLineValue);

                    // sensor:  1   2   3   4
                    // bit:     0   1   2   3
                    if (ucLineValue == 0b00001111)
                    {   // at junction
                        bJunctionFlag = 1;

                        if ((ucStopAtJunction != 0) && (ucJunctionCount >= (ucStopAtJunction - 1)))
                        {
                            usSpeed_Left = 0;
                            usSpeed_Right = 0;
                            ucJunctionCount = 0;

                            eG15SetSpeed(ucLeftServoId, WRITE_NOW, usSpeed_Left, WHEEL_CCW);
                            eG15SetSpeed(ucRightServoId, WRITE_NOW, usSpeed_Right, WHEEL_CW);
                            break;
                        }
                        else
                        {
                            usSpeed_Left = usSpeed_Max;
                            usSpeed_Right = usSpeed_Max;
                        }
                    }
                    else
                    {
                        if (bJunctionFlag == 1)
                        {
                            bJunctionFlag = 0;
                            ucJunctionCount++;
                        }

                        if (ucLineValue == 0b00000110)
                        {   // at the center of the line
                            usSpeed_Left = usSpeed_Max;
                            usSpeed_Right = usSpeed_Max;
                        }
                        else if (ucLineValue == 0b00000111)
                        {   // robot slightly on the right
                            usSpeed_Left = (usSpeed_Max >> 1) + (usSpeed_Max >> 3);
                            usSpeed_Right = usSpeed_Max;
                        }
                        else if (ucLineValue == 0b00001110)
                        {   // robot slightly on the left
                            usSpeed_Left = usSpeed_Max;
                            usSpeed_Right = (usSpeed_Max >> 1) + (usSpeed_Max >> 3);
                        }
                        else if (ucLineValue == 0b00000011)
                        {   // robot on the right
                            usSpeed_Left = (usSpeed_Max >> 1) - (usSpeed_Max >> 3);
                            usSpeed_Right = usSpeed_Max;
                        }
                        else if (ucLineValue == 0b00001100)
                        {   // robot on the left
                            usSpeed_Left = usSpeed_Max;
                            usSpeed_Right = (usSpeed_Max >> 1) - (usSpeed_Max >> 3);
                        }
                        else if (ucLineValue == 0b00000001)
                        {   // robot on far right
                            usSpeed_Left = 0;
                            usSpeed_Right = usSpeed_Max;
                        }
                        else if (ucLineValue == 0b00001000)
                        {   // robot on far left
                            usSpeed_Left = usSpeed_Max;
                            usSpeed_Right = 0;
                        }
                    }
                    eG15SetSpeed(ucLeftServoId, WRITE_NOW, usSpeed_Left, WHEEL_CCW);
                    eG15SetSpeed(ucRightServoId, WRITE_NOW, usSpeed_Right, WHEEL_CW);

                    // Delay for 20ms.
                    vTaskDelay(20 / portTICK_RATE_MS);
                
                    // Break if stop playing.
                    if (prv_ucPlaying == 0) {
                        break;
                    }
                } while (ucStopAtJunction > 0);
                
                // Get the next block address.
                ulBlockAddress = ((unsigned long)pucBuffer[8] << 16) + ((unsigned long)pucBuffer[9] << 8) + (unsigned long)pucBuffer[10];

                break;
            }

            
            default: {
                ulBlockAddress = 0x00ffffff;
                break;
            }

        }   // End of switch-case.


        
        // Read the stack watermark and record it if it's below threshold.
        unsigned portBASE_TYPE uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        if (uxHighWaterMark < MIN_STACK_WATERMARK) {
            xSystemError.bStackLowError = 1;

            // Only log it when the watermark value changed.
            static portBASE_TYPE uxPreviousWatermark = 0;
            if (uxHighWaterMark != uxPreviousWatermark) {
                vLogStackWatermark("Planner Task", (unsigned short)uxHighWaterMark);
            }
            uxPreviousWatermark = uxHighWaterMark;
        }

    }   // End of while loop.



    // Turn off torque and LED for used output modules.
    prv_vDisableUsedOutput();
    
    
    
    xSemaphoreTake(xSdCardMutex,portMAX_DELAY);
    // Restore the original read-only flag.
    xFSSetReadOnlyFlag(prv_szFullFilePath, prv_ucLock);
    
    // Close the planner file.
    FSfclose(prv_pxOpenedFile);
    prv_pxOpenedFile = NULL;
    xSemaphoreGive(xSdCardMutex);

    // Update the play page GUI when finish playing.
    vUpdateMotionPageEndPlaying(PLANNER_FILE);

    // Indicate stop playing.
    prv_szOpenedFilename[0] = 0;
    prv_ucPlaying = 0;

    
    // Delete this task after complete.
    vTaskDelete(NULL);
}

