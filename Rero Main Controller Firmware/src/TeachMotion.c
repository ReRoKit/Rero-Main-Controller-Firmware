/*******************************************************************************
 * Functions for teaching motion file.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "TeachMotion.h"
#include "Variables.h"
#include "FatFs/FatFsWrapper.h"
#include "G15 and Sensors/G15.h"
#include "GUI/MotionPage.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

// Duration for 1 time frame = 1 second.
#define TIMEFRAME_DURATION      10

// Speed for wheel mode.
#define WHEEL_SPEED             500



// Servo information.
typedef struct {
    unsigned char ucServoId;
    unsigned char ucWheelMode;
    unsigned short usPreviousPosition;
} SERVO_INFO;



/*******************************************************************************
 * PUBLIC GLOBAL VARIABLES
 *******************************************************************************/

static SERVO_INFO prv_pxServoInfo[EM_MAX_ID + 1];
static unsigned char prv_ucServoCount = 0;
static unsigned short prv_usTimeframeCount = 0;
static unsigned short prv_usStartTime = 0;



/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 *******************************************************************************/

static void prv_vWriteMotionFileHeader(FSFILE *pxMotionFile);



/*******************************************************************************
 * FUNCTION: prv_vWriteMotionFileHeader
 *
 * PARAMETERS:
 * ~ pxMotionFile     - The new motion file.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Write the header information to the newly created motion file.
 *
 *******************************************************************************/
static void prv_vWriteMotionFileHeader(FSFILE *pxMotionFile)
{
    // Make sure the motion file is available.
    if (pxMotionFile != NULL) {
        unsigned char pucBuffer[16];
        
        // Write the header byte and command.
        pucBuffer[0] = 0xfa;      // Header.
        pucBuffer[1] = 0x13;      // Command: Animator_Teach = 1, Loop = 1, RD_WR = 1.
        FSfwrite (pucBuffer, 1, 2, pxMotionFile);
        
        // Write the file comment.
        char *szFileComment = "Teach Mode";
        FSfwrite (szFileComment, 1, strlen(szFileComment), pxMotionFile);
        
        // Tail-pad the file comment with null.
        unsigned char ucPadSize = MAX_FILENAME_LENGTH - strlen(szFileComment);
        unsigned char i;
        for (i = 0; i < ucPadSize; i++) {
            pucBuffer[i] = 0;
        }
        FSfwrite (pucBuffer, 1, ucPadSize, pxMotionFile);
        
        // Write servo count and servo ID.
        FSfwrite (&prv_ucServoCount, 1, 1, pxMotionFile);
        for (i = 0; i < prv_ucServoCount; i++) {
            FSfwrite (&prv_pxServoInfo[i].ucServoId, 1, 1, pxMotionFile);
        }
        
        // Write the time frame count.
        // Initialize it to 0 first.
        pucBuffer[0] = 0;
        pucBuffer[1] = 0;
        FSfwrite (pucBuffer, 1, 2, pxMotionFile);
    }
}



/*******************************************************************************
 * FUNCTION: vStartTeaching
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Start the teaching process by searching for all G15.
 * Do not overwrite the selected memory yet.
 *
 *******************************************************************************/
void vStartTeaching(void)
{
    // Reset the servo count, time frame count and start time.
    prv_ucServoCount = 0;
    prv_usTimeframeCount = 0;
    prv_usStartTime = 0;
    
    // Search for servo.
    unsigned char i;
    for(i = 0; i <= EM_MAX_ID; i++) {
        unsigned short usLimitCw;
        unsigned short usLimitCcw;

        // Read the model of the external module.
        EM_MODEL eModel;
        if (eEMGetModel(i, &eModel) == EM_NO_ERROR) {
            // Make sure the model is G15.
            if (eModel == EM_MODEL_G15) {

                // Try to read the angle limit from the servo.
                // If no error, that means a servo is found.
                if (eG15GetAngleLimit(i, &usLimitCw, &usLimitCcw) == EM_NO_ERROR) {
                    // Turn off torque and turn on LED.
                    eG15SetTorqueLed(i, WRITE_NOW, 0, 1);

                    // Save the servo ID.
                    prv_pxServoInfo[prv_ucServoCount].ucServoId = i;

                    // Indicate whether the servo is in wheel mode.
                    if(usLimitCw == usLimitCcw) {
                        prv_pxServoInfo[prv_ucServoCount].ucWheelMode = 1;

                        // Save the position in wheel mode for coparing wheel direction later.
                        eG15GetPosition(i, &prv_pxServoInfo[prv_ucServoCount].usPreviousPosition);
                    }
                    else {
                        prv_pxServoInfo[prv_ucServoCount].ucWheelMode = 0;
                    }

                    // Increase the servo count.
                    prv_ucServoCount++;
                }
            }
        }
    }
}



/*******************************************************************************
 * FUNCTION: vTeachAddPosition
 *
 * PARAMETERS:
 * ~ szSelectedFileName     - Selected filename without extension.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Add the current position of servos to the motion file.
 *
 *******************************************************************************/
void vTeachAddPosition(char *szSelectedFileName)
{
    // Get the full file path and add the extension for motion file.
    static char szFullFilePath[MAX_FILENAME_LENGTH * 3];
    strcpy(szFullFilePath, szProgramFolder);
    strcat(szFullFilePath, "/");
    strcat(szFullFilePath, szSelectedFileName);
    strcat(szFullFilePath, szMotionFileExt);
    
    // Delete all program files in the selected memory if this is the first frame.
    if (prv_usTimeframeCount == 0) {
        ucDeleteProgramFiles(szSelectedFileName);
    }
        
    // Take control of SD card.
    xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
    
    
    // Create a new motion file and write the header if this is the first frame.
    FSFILE *pxMotionFile = NULL;
    if (prv_usTimeframeCount == 0) {
        // Open the motion file for write.
        pxMotionFile = FSfopen(szFullFilePath, "w");
        
        // Write the header to the new motion file.
        prv_vWriteMotionFileHeader(pxMotionFile);
    }
    
    // Else, open the motion file for appending.
    else {
        pxMotionFile = FSfopen(szFullFilePath, "a");
    }
    
    
    
    // Make sure it's opened successfully.
    if (pxMotionFile != NULL) {
        unsigned char pucBuffer[5];
        
        // Increase the time frame count.
        prv_usTimeframeCount++;

        // Update the time and frame value in motion page.
        vUpdateMotionPageTimeFrame(prv_usTimeframeCount, prv_usTimeframeCount * (TIMEFRAME_DURATION / 10));
        
        
        // Write the start time and servo count.
        pucBuffer[0] = (unsigned char)(prv_usStartTime >> 8);
        pucBuffer[1] = (unsigned char)(prv_usStartTime & 0xff);
        pucBuffer[2] = prv_ucServoCount;
        FSfwrite (pucBuffer, 1, 3, pxMotionFile);

        // Increase the start time for next time frame.
        prv_usStartTime += TIMEFRAME_DURATION;


        // Write the servo frame.
        unsigned char i;
        for(i = 0; i < prv_ucServoCount; i++) {
            // Read the current position.
            unsigned short usCurrentPosition;
            eG15GetPosition(prv_pxServoInfo[i].ucServoId, &usCurrentPosition);

            // Wheel mode.
            if(prv_pxServoInfo[i].ucWheelMode != 0) {

                // Get the angle difference.
                signed short ssAngleDiff = usCurrentPosition - prv_pxServoInfo[i].usPreviousPosition;

                // Save current position as previous position.
                prv_pxServoInfo[i].usPreviousPosition = usCurrentPosition;

                // Make correction to the angle difference if crossing 0.
                if (ssAngleDiff < -(1088/2)) {
                    ssAngleDiff += 1088;
                }
                else if (ssAngleDiff > (1088/2)) {
                    ssAngleDiff -= 1088;
                }

                // Moving CCW if angle difference is > 10 degree.
                if (ssAngleDiff > 30) {
                    pucBuffer[0] = prv_pxServoInfo[i].ucServoId;
                    pucBuffer[1] = ((unsigned char)(WHEEL_SPEED >> 8) & 0x03) | 0x50;     // Destination(H) OR Control bits[DIR,TOR,LED,WHEEL,MUSIC(2),0,0].
                    pucBuffer[2] = (unsigned char)(WHEEL_SPEED & 0xff);                   // Destination (L).
                }
                // Moving CW if angle difference is < -10 degree.
                else if(ssAngleDiff < -30) {
                    pucBuffer[0] = prv_pxServoInfo[i].ucServoId;
                    pucBuffer[1] = ((unsigned char)(WHEEL_SPEED >> 8) & 0x03) | 0xd0;     // Destination(H) OR Control bits[DIR,TOR,LED,WHEEL,MUSIC(2),0,0].
                    pucBuffer[2] = (unsigned char)(WHEEL_SPEED & 0xff);                   // Destination (L).
                }
                // Not moving
                else {
                    pucBuffer[0] = prv_pxServoInfo[i].ucServoId;
                    pucBuffer[1] = 0xd0;      // Destination(H) OR Control bits[DIR,TOR,LED,WHEEL,MUSIC(2),0,0].
                    pucBuffer[2] = 0;         // Destination (L).
                }
            }



            // Position mode.
            else {
                // Convert current position to degree.
                usCurrentPosition = (unsigned short)((360ul * (unsigned long)usCurrentPosition) / 1088ul);

                pucBuffer[0] = prv_pxServoInfo[i].ucServoId;
                pucBuffer[1] = ((unsigned char)(usCurrentPosition >> 8) & 0x03) | 0x40;     // Destination(H) OR Control bits[DIR,TOR,LED,WHEEL,MUSIC(2),0,0].
                pucBuffer[2] = (unsigned char)(usCurrentPosition & 0xff);                   // Destination (L).
            }

            // End time.
            pucBuffer[3] = (unsigned char)(prv_usStartTime >> 8);
            pucBuffer[4] = (unsigned char)(prv_usStartTime & 0xff);

            // Write to file.
            FSfwrite(pucBuffer, 1, 5, pxMotionFile);
        }

        // Update the time frame count.
        FSfseek(pxMotionFile, prv_ucServoCount + MAX_FILENAME_LENGTH + 3, SEEK_SET);
        pucBuffer[0] = (unsigned char)(prv_usTimeframeCount >> 8);
        pucBuffer[1] = (unsigned char)(prv_usTimeframeCount & 0xff);
        FSfwrite (pucBuffer, 1, 2, pxMotionFile);

        // Close the file.
        FSfclose(pxMotionFile);
    }
    
    // Release control of SD card.
    xSemaphoreGive(xSdCardMutex);
}



/*******************************************************************************
 * FUNCTION: vTeachTurnOffServoLed
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Turn off the LED for all the servo.
 *
 *******************************************************************************/
void vTeachTurnOffServoLed(void)
{
    unsigned char i;
    for (i = 0; i < prv_ucServoCount; i++) {
        eG15SetLed(prv_pxServoInfo[i].ucServoId, WRITE_NOW, 0);
    }
}
