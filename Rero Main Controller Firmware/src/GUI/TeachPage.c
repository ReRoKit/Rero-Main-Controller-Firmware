/*******************************************************************************
 * Functions for teach page in rero GUI.
 *
 * Author: Kong Wai Weng
 * Company: Cytron Technologies Sdn. Bhd.
 * Email:  waiweng@cytron.com.my
 *******************************************************************************/

#include <string.h>
#include "Variables.h"
#include "Planner.h"
#include "FatFs/FatFsWrapper.h"
#include "GUI/TeachPage.h"
#include "GUI/GraphicScreens.h"
#include "G15 and Sensors/ExtModule.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

typedef struct {
    unsigned char ucServoId;
    unsigned char ucWheelMode;
    unsigned short usPreviousPosition;
} SERVO_INFO;



// File state.
typedef enum __attribute__((packed)) {
    FILE_NOT_FOUND,
    FILE_LOCKED,
    FILE_UNLOCKED
} FILE_STATUS;



/*******************************************************************************
* PRIVATE GLOBAL VARIABLES
*******************************************************************************/

// Lock / unlock icon.
static char *ICON_LOCKED = "/theme/icon/lock_small.ico";
static char *ICON_UNLOCKED = "/theme/icon/unlock_small.ico";



// Graphic objects.
static PICTURE *prv_pxPictLock[5] = {NULL};
static RADIOBUTTON *prv_pxRb[5] = {NULL};



const unsigned short TIMEFRAME_DURATION = 10;   // 1 time frame = 1 second.
const unsigned short WHEEL_SPEED = 500;         // Speed for wheel mode.

const unsigned short BTN_TEACH_POS_X = 125;
const unsigned short BTN_TEACH_POS_Y = 135;
const unsigned short BTN_STOP_POS_X = 230;
const unsigned short BTN_STOP_POS_Y = 135;
const unsigned short BTN_SIZE_X = 72;
const unsigned short BTN_SIZE_Y = 72;

const unsigned short RB_POS_X = 35;
const unsigned short RB_POS_Y1 = 50;
const unsigned short RB_POS_Y2 = 85;
const unsigned short RB_POS_Y3 = 120;
const unsigned short RB_POS_Y4 = 155;
const unsigned short RB_POS_Y5 = 190;
const unsigned short RB_SIZE_X = 80;
const unsigned short RB_SIZE_Y = 30;

const unsigned short TXT_POS_X1 = 125;
const unsigned short TXT_POS_X2 = 220;
const unsigned short TXT_POS_Y1 = 55;
const unsigned short TXT_POS_Y2 = 75;
const unsigned short TXT_POS_Y3 = 105;
const unsigned short TXT_SIZE_Y = 20;

static char *prv_szSelectedFileName = NULL;

static const char *FILE_NAME[] = { "File1",
                                   "File2",
                                   "File3",
                                   "File4",
                                   "File5" };

static FILE_STATUS prv_peFileStatus[5] = {0};



/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 *******************************************************************************/

static void prv_vCloseMotionFile(FSFILE *pxMotionFile);
static void prv_vUpdateFileStatus(void);



/*******************************************************************************
 * FUNCTION: prv_vCloseMotionFile
 *
 * PARAMETERS:
 * ~ pxMotionFile   - The motion file we want to close.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Close the opened motion file.
 *
 *******************************************************************************/
static void prv_vCloseMotionFile(FSFILE *pxMotionFile)
{
    unsigned char pucBuffer[2];

    if (pxMotionFile != NULL) {
        // Take control of SD card.
        xSemaphoreTake(xSdCardMutex,portMAX_DELAY);

        // Write the checksum.
        pucBuffer[0] = 0xaa;
        pucBuffer[1] = 0xaa;
        FSfwrite (pucBuffer, 1, 2, pxMotionFile);

        // Close the file.
        FSfclose(pxMotionFile);

        // Release control of SD card.
        xSemaphoreGive(xSdCardMutex);
    }
}



/*******************************************************************************
 * FUNCTION: prv_vUpdateFileStatus
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Check and update which motion file is locked.
 *
 *******************************************************************************/
static void prv_vUpdateFileStatus(void)
{
    unsigned char ucReadOnlyFlag;
    static char szFullFilePath[MAX_FILENAME_LENGTH * 3];
    
    
    
    xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
    
    
    // Loop for File 1 - 5.
    unsigned char i;
    for (i = 0; i < 5; i++) {
        // Get the full file path and add the extension for planner file.
        strcpy(szFullFilePath, szProgramFolder);
        strcat(szFullFilePath, "/");
        strcat(szFullFilePath, FILE_NAME[i]);
        strcat(szFullFilePath, szPlannerFileExt);

        // Try to read the attributes of the planner file.
        if (xFSGetReadOnlyFlag(szFullFilePath, &ucReadOnlyFlag) == FR_OK) {
            // Read successed. Update the file status.
            if (ucReadOnlyFlag == 0) {
                prv_peFileStatus[i] = FILE_UNLOCKED;
            } else {
                prv_peFileStatus[i] = FILE_LOCKED;
            }
        }

        // Read failed. Try with motion file.
        else {
            // Get the full file path and add the extension for motion file.
            strcpy(szFullFilePath, szProgramFolder);
            strcat(szFullFilePath, "/");
            strcat(szFullFilePath, FILE_NAME[i]);
            strcat(szFullFilePath, szMotionFileExt);
            
            // Try to read the attributes of the motion file.
            if (xFSGetReadOnlyFlag(szFullFilePath, &ucReadOnlyFlag) == FR_OK) {
                // Read successed. Update the file status.
                if (ucReadOnlyFlag == 0) {
                    prv_peFileStatus[i] = FILE_UNLOCKED;
                } else {
                    prv_peFileStatus[i] = FILE_LOCKED;
                }
            }
            
            // Read failed again. That means the file is not available.
            else {
                prv_peFileStatus[i] = FILE_NOT_FOUND;
            }
        }
        
    }   // End of loop.

    xSemaphoreGive(xSdCardMutex);
}



/*******************************************************************************
 * FUNCTION: vCreateTeach
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the Teach page.
 *
 *******************************************************************************/
void vCreateTeach(void)
{
    const unsigned short BTN_LOCK_POS_X = 10;
    const unsigned short BTN_LOCK_SIZE_X = 20;
    const unsigned short BTN_LOCK_SIZE_Y = 20;
    
    
    // Deselect the file.
    prv_szSelectedFileName = NULL;


    GOLFree();
    vCreatePageWindow("TEACH");
    
    
    // Create the lock icon.
    prv_pxPictLock[0] = PictCreate( GID_TEACH_IMG_LOCK1,
                                    BTN_LOCK_POS_X,                   RB_POS_Y1 + 5,
                                    BTN_LOCK_POS_X + BTN_LOCK_SIZE_X, RB_POS_Y1 + 5 + BTN_LOCK_SIZE_Y,
                                    PICT_DRAW | PICT_DISABLED, 1, NULL, pxDefaultScheme );
    
    prv_pxPictLock[1] = PictCreate( GID_TEACH_IMG_LOCK2,
                                    BTN_LOCK_POS_X,                   RB_POS_Y2 + 5,
                                    BTN_LOCK_POS_X + BTN_LOCK_SIZE_X, RB_POS_Y2 + 5 + BTN_LOCK_SIZE_Y,
                                    PICT_DRAW | PICT_DISABLED, 1, NULL, pxDefaultScheme );
    
    prv_pxPictLock[2] = PictCreate( GID_TEACH_IMG_LOCK3,
                                    BTN_LOCK_POS_X,                   RB_POS_Y3 + 5,
                                    BTN_LOCK_POS_X + BTN_LOCK_SIZE_X, RB_POS_Y3 + 5 + BTN_LOCK_SIZE_Y,
                                    PICT_DRAW | PICT_DISABLED, 1, NULL, pxDefaultScheme );
    
    prv_pxPictLock[3] = PictCreate( GID_TEACH_IMG_LOCK4,
                                    BTN_LOCK_POS_X,                   RB_POS_Y4 + 5,
                                    BTN_LOCK_POS_X + BTN_LOCK_SIZE_X, RB_POS_Y4 + 5 + BTN_LOCK_SIZE_Y,
                                    PICT_DRAW | PICT_DISABLED, 1, NULL, pxDefaultScheme );
    
    prv_pxPictLock[4] = PictCreate( GID_TEACH_IMG_LOCK5,
                                    BTN_LOCK_POS_X,                   RB_POS_Y5 + 5,
                                    BTN_LOCK_POS_X + BTN_LOCK_SIZE_X, RB_POS_Y5 + 5 + BTN_LOCK_SIZE_Y,
                                    PICT_DRAW | PICT_DISABLED, 1, NULL, pxDefaultScheme );
    
    
    
    // Create the select file radio button.
    prv_pxRb[0] = RbCreate(
        GID_TEACH_RB_FILE1,
        RB_POS_X,
        RB_POS_Y1,
        RB_POS_X + RB_SIZE_X,
        RB_POS_Y1 + RB_SIZE_Y,
        RB_DRAW | RB_GROUP,
        " File 1",
        pxRadioBtnScheme
    );

    prv_pxRb[1] = RbCreate(
        GID_TEACH_RB_FILE2,
        RB_POS_X,
        RB_POS_Y2,
        RB_POS_X + RB_SIZE_X,
        RB_POS_Y2 + RB_SIZE_Y,
        RB_DRAW,
        " File 2",
        pxRadioBtnScheme
    );

    prv_pxRb[2] = RbCreate(
        GID_TEACH_RB_FILE3,
        RB_POS_X,
        RB_POS_Y3,
        RB_POS_X + RB_SIZE_X,
        RB_POS_Y3 + RB_SIZE_Y,
        RB_DRAW,
        " File 3",
        pxRadioBtnScheme
    );

    prv_pxRb[3] = RbCreate(
        GID_TEACH_RB_FILE4,
        RB_POS_X,
        RB_POS_Y4,
        RB_POS_X + RB_SIZE_X,
        RB_POS_Y4 + RB_SIZE_Y,
        RB_DRAW,
        " File 4",
        pxRadioBtnScheme
    );

    prv_pxRb[4] = RbCreate(
        GID_TEACH_RB_FILE5,
        RB_POS_X,
        RB_POS_Y5,
        RB_POS_X + RB_SIZE_X,
        RB_POS_Y5 + RB_SIZE_Y,
        RB_DRAW,
        " File 5",
        pxRadioBtnScheme
    );


    // Create the message text 1.
    StCreate(
        GID_TEACH_TXT_MSG1,
        TXT_POS_X1,
        TXT_POS_Y1,
        GetMaxX() - 1,
        TXT_POS_Y1 + TXT_SIZE_Y,
        ST_DRAW,
        "Please select a file.",
        pxLightBlueTxtScheme
    );


    // Create the message text 2.
    StCreate(
        GID_TEACH_TXT_MSG2,
        TXT_POS_X1,
        TXT_POS_Y2,
        GetMaxX() - 1,
        TXT_POS_Y2 + TXT_SIZE_Y,
        ST_DRAW,
        "",
        pxLightBlueTxtScheme
    );



    // Create the time frame text and digital meter.
    StCreate(
        GID_TEACH_TXT_FRAME,
        TXT_POS_X1,
        TXT_POS_Y3,
        TXT_POS_X2,
        TXT_POS_Y3 + TXT_SIZE_Y,
        ST_DRAW,
        "Time Frame :",
        pxDefaultScheme
    );

    DmCreate(
        GID_TEACH_DM_FRAME,
        TXT_POS_X2,
        TXT_POS_Y3,
        GetMaxX() - 1,
        TXT_POS_Y3 + TXT_SIZE_Y,
        DM_DRAW,
        0,
        4,
        0,
        pxDefaultScheme
    );



    // Create the teach button.
    BtnCreate(
        GID_TEACH_BTN_TEACH,
        BTN_TEACH_POS_X,
        BTN_TEACH_POS_Y,
        BTN_TEACH_POS_X + BTN_SIZE_X,
        BTN_TEACH_POS_Y + BTN_SIZE_Y,
        0,
        BTN_DRAW | BTN_TEXTBOTTOM | BTN_NOPANEL | BTN_DISABLED,
        "/theme/icon/teach.ico",
        "TEACH",
        pxImgBtnScheme
    );



    // Create the stop button.
    BtnCreate(
        GID_TEACH_BTN_STOP,
        BTN_STOP_POS_X,
        BTN_STOP_POS_Y,
        BTN_STOP_POS_X + BTN_SIZE_X,
        BTN_STOP_POS_Y + BTN_SIZE_Y,
        0,
        BTN_DRAW | BTN_TEXTBOTTOM | BTN_NOPANEL | BTN_DISABLED,
        "/theme/icon/stop.ico",
        "STOP",
        pxImgBtnScheme
    );
    
    
    
    // Update the file status.
    prv_vUpdateFileStatus();
    
    // Set the lock icon and radio button state according to the file status.
    unsigned char i;
    for (i = 0; i < 5; i++) {
        switch (prv_peFileStatus[i]) {
            case FILE_NOT_FOUND:
            case FILE_UNLOCKED:
                PictSetBitmap(prv_pxPictLock[i], ICON_UNLOCKED);
                GOLRedraw(prv_pxPictLock[i]);
                vGraphicsObjectEnable(prv_pxRb[i]);
                break;
                
            case FILE_LOCKED:
                PictSetBitmap(prv_pxPictLock[i], ICON_LOCKED);
                GOLRedraw(prv_pxPictLock[i]);
                vGraphicsObjectDisable(prv_pxRb[i]);
                break;
        }
    }
}



/*******************************************************************************
 * FUNCTION: usMsgTeach
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
 * Handle the touchscreen event of the Teach page.
 *
 *******************************************************************************/
WORD usMsgTeach(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    static SERVO_INFO pxServoInfo[EM_MAX_ID + 1];
    static unsigned char ucServoCount = 0;
    static unsigned short usTimeframeCount = 0;
    static unsigned short usStartTime = 0;
    static FSFILE *pxMotionFile = NULL;

    unsigned char pucBuffer[10] = {0};

    // Radio button checked.
    if (objMsg == RB_MSG_CHECKED) {
        // Reset the time frame.
        DIGITALMETER *pxDm;
        pxDm = (DIGITALMETER*)GOLFindObject(GID_TEACH_DM_FRAME);
        DmSetValue(pxDm, 0);
        SetState(pxDm, DM_UPDATE);


        // Select the file name based on radio button.
        switch (GetObjID(pObj)) {
            case GID_TEACH_RB_FILE1:
                prv_szSelectedFileName = "File1";
                break;
            case GID_TEACH_RB_FILE2:
                prv_szSelectedFileName = "File2";
                break;
            case GID_TEACH_RB_FILE3:
                prv_szSelectedFileName = "File3";
                break;
            case GID_TEACH_RB_FILE4:
                prv_szSelectedFileName = "File4";
                break;
            case GID_TEACH_RB_FILE5:
                prv_szSelectedFileName = "File5";
                break;
            default:
                prv_szSelectedFileName = NULL;
                break;
        }


        
        // Modify the message text.
        STATICTEXT *pxTxtMsg1 = (STATICTEXT*)GOLFindObject(GID_TEACH_TXT_MSG1);
        vStaticTextUpdateText(pxTxtMsg1, "Press TEACH to start.");

        // Enable the teach button.
        vGraphicsObjectEnable(GOLFindObject(GID_TEACH_BTN_TEACH));
    }



    // Button pressed and released.
    else if (objMsg == BTN_MSG_RELEASED) {
        unsigned char i;
        BUTTON *pxBtnTeachNext;
        PICTURE *pxImgTeachNext;
        STATICTEXT *pxTxtMsg1 = (STATICTEXT*)GOLFindObject(GID_TEACH_TXT_MSG1);
        STATICTEXT *pxTxtMsg2 = (STATICTEXT*)GOLFindObject(GID_TEACH_TXT_MSG2);
        DIGITALMETER *pDmFrame = (DIGITALMETER*)GOLFindObject(GID_TEACH_DM_FRAME);
        BUTTON* pxBtnStop = (BUTTON*)GOLFindObject(GID_TEACH_BTN_STOP);

        switch (GetObjID(pObj)) {
            // Teach button.
            case GID_TEACH_BTN_TEACH:
                // Remove the teach button and create the next button.
                GOLDeleteObjectByID(GID_TEACH_BTN_TEACH);
                BtnCreate(
                    GID_TEACH_BTN_NEXT,
                    BTN_TEACH_POS_X,
                    BTN_TEACH_POS_Y,
                    BTN_TEACH_POS_X + BTN_SIZE_X,
                    BTN_TEACH_POS_Y + BTN_SIZE_Y,
                    0,
                    BTN_DRAW | BTN_TEXTBOTTOM | BTN_NOPANEL,
                    "/theme/icon/next.ico",
                    "NEXT",
                    pxImgBtnScheme
                );


                
                // Enable the stop button.
                vGraphicsObjectEnable(pxBtnStop);

                // Disable all radio buttons.
                for(i = 0; i < 5; i++) {
                    vGraphicsObjectDisable(GOLFindObject(GID_TEACH_RB_FILE1 + i));
                }

                // Modify the message text.
                vStaticTextUpdateText(pxTxtMsg1, "Press NEXT to add position.");
                vStaticTextUpdateText(pxTxtMsg2, "Press STOP when done.");

                

                // Reset the servo count, time frame count and start time.
                ucServoCount = 0;
                usTimeframeCount = 0;
                usStartTime = 0;

                // Update the display.
                DmSetValue(pDmFrame,(unsigned long)usTimeframeCount);
                SetState(pDmFrame, DM_UPDATE);

                // Search for servo.
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
                                pxServoInfo[ucServoCount].ucServoId = i;

                                // Indicate whether the servo is in wheel mode.
                                if(usLimitCw == usLimitCcw) {
                                    pxServoInfo[ucServoCount].ucWheelMode = 1;

                                    // Save the position in wheel mode for coparing wheel direction later.
                                    eG15GetPosition(i, &pxServoInfo[ucServoCount].usPreviousPosition);
                                }
                                else {
                                    pxServoInfo[ucServoCount].ucWheelMode = 0;
                                }

                                // Increase the servo count.
                                ucServoCount++;
                            }
                        }
                    }
                }



                // Delete all the program files in the selected memory.
                ucDeleteProgramFiles(prv_szSelectedFileName);

                
                
                // Get the full file path and add the extension for motion file.
                static char szFullFilePath[MAX_FILENAME_LENGTH * 3];
                strcpy(szFullFilePath, szProgramFolder);
                strcat(szFullFilePath, "/");
                strcat(szFullFilePath, prv_szSelectedFileName);
                strcat(szFullFilePath, szMotionFileExt);

                // Take control of SD card.
                xSemaphoreTake(xSdCardMutex,portMAX_DELAY);

                // Open the motion file for write.
                pxMotionFile = FSfopen(szFullFilePath, "w");


                
                // Write the header byte and command.
                pucBuffer[0] = 0xfa;      // Header.
                pucBuffer[1] = 0x13;      // Command: Animator_Teach = 1, Loop = 1, RD_WR = 1.
                FSfwrite (pucBuffer, 1, 2, pxMotionFile);

                // Write the file comment.
                char *szFileComment = "Teach Mode";
                FSfwrite (szFileComment, 1, strlen(szFileComment), pxMotionFile);
                
                unsigned char ucPadSize = MAX_FILENAME_LENGTH - strlen(szFileComment);
                for (i = 0; i < ucPadSize; i++) {
                    pucBuffer[i] = 0;
                }
                FSfwrite (pucBuffer, 1, ucPadSize, pxMotionFile);

                // Write servo count and servo ID.
                FSfwrite (&ucServoCount, 1, 1, pxMotionFile);
                for (i = 0; i < ucServoCount; i++) {
                    FSfwrite (&pxServoInfo[i].ucServoId, 1, 1, pxMotionFile);
                }


                // Write the time frame count.
                // Initialize it to 0 first.
                pucBuffer[0] = 0;
                pucBuffer[1] = 0;
                FSfwrite (pucBuffer, 1, 2, pxMotionFile);

                // Release control of SD card.
                xSemaphoreGive(xSdCardMutex);

                break;


            case GID_TEACH_BTN_NEXT:

                // Increase the time frame count.
                usTimeframeCount++;
                
                // Update the display.
                DmSetValue(pDmFrame,(unsigned long)usTimeframeCount);
                SetState(pDmFrame, DM_UPDATE);

                // Take control of SD card.
                xSemaphoreTake(xSdCardMutex,portMAX_DELAY);

                // Write the start time and servo count.
                pucBuffer[0] = (unsigned char)(usStartTime >> 8);
                pucBuffer[1] = (unsigned char)(usStartTime & 0xff);
                pucBuffer[2] = ucServoCount;
                FSfwrite (pucBuffer, 1, 3, pxMotionFile);
                
                // Increase the start time for next time frame.
                usStartTime += TIMEFRAME_DURATION;
                
                
                // Write the servo frame.
                for(i = 0; i < ucServoCount; i++) {
                    // Read the current position.
                    unsigned short usCurrentPosition;
                    eG15GetPosition(pxServoInfo[i].ucServoId, &usCurrentPosition);

                    // Wheel mode.
                    if(pxServoInfo[i].ucWheelMode != 0) {

                        // Get the angle difference.
                        signed short ssAngleDiff = usCurrentPosition - pxServoInfo[i].usPreviousPosition;

                        // Save current position as previous position.
                        pxServoInfo[i].usPreviousPosition = usCurrentPosition;

                        // Make correction to the angle difference if crossing 0.
                        if (ssAngleDiff < -(1088/2)) {
                            ssAngleDiff += 1088;
                        }
                        else if (ssAngleDiff > (1088/2)) {
                            ssAngleDiff -= 1088;
                        }

                        // Moving CCW if angle difference is > 10 degree.
                        if (ssAngleDiff > 30) {
                            pucBuffer[0] = pxServoInfo[i].ucServoId;
                            pucBuffer[1] = ((unsigned char)(WHEEL_SPEED >> 8) & 0x03) | 0x50;     // Destination(H) OR Control bits[DIR,TOR,LED,WHEEL,MUSIC(2),0,0].
                            pucBuffer[2] = (unsigned char)(WHEEL_SPEED & 0xff);                   // Destination (L).
                        }
                        // Moving CW if angle difference is < -10 degree.
                        else if(ssAngleDiff < -30) {
                            pucBuffer[0] = pxServoInfo[i].ucServoId;
                            pucBuffer[1] = ((unsigned char)(WHEEL_SPEED >> 8) & 0x03) | 0xd0;     // Destination(H) OR Control bits[DIR,TOR,LED,WHEEL,MUSIC(2),0,0].
                            pucBuffer[2] = (unsigned char)(WHEEL_SPEED & 0xff);                   // Destination (L).
                        }
                        // Not moving
                        else {
                            pucBuffer[0] = pxServoInfo[i].ucServoId;
                            pucBuffer[1] = 0xd0;      // Destination(H) OR Control bits[DIR,TOR,LED,WHEEL,MUSIC(2),0,0].
                            pucBuffer[2] = 0;         // Destination (L).
                        }
                    }



                    // Position mode.
                    else {
                        // Convert current position to degree.
                        usCurrentPosition = (unsigned short)((360ul * (unsigned long)usCurrentPosition) / 1088ul);

                        pucBuffer[0] = pxServoInfo[i].ucServoId;
                        pucBuffer[1] = ((unsigned char)(usCurrentPosition >> 8) & 0x03) | 0x40;     // Destination(H) OR Control bits[DIR,TOR,LED,WHEEL,MUSIC(2),0,0].
                        pucBuffer[2] = (unsigned char)(usCurrentPosition & 0xff);                   // Destination (L).
                    }
                    
                    // End time.
                    pucBuffer[3] = (unsigned char)(usStartTime >> 8);
                    pucBuffer[4] = (unsigned char)(usStartTime & 0xff);

                    // Write to file.
                    FSfwrite(pucBuffer, 1, 5, pxMotionFile);
                }

                // Update the time frame count.
                FSfseek(pxMotionFile, ucServoCount + MAX_FILENAME_LENGTH + 3, SEEK_SET);
                pucBuffer[0] = (unsigned char)(usTimeframeCount >> 8);
                pucBuffer[1] = (unsigned char)(usTimeframeCount & 0xff);
                FSfwrite (pucBuffer, 1, 2, pxMotionFile);

                // Go to end of file.
                FSfseek(pxMotionFile, 0, SEEK_END);


                // Release control of SD card.
                xSemaphoreGive(xSdCardMutex);

                break;
                


            // Stop button.
            case GID_TEACH_BTN_STOP:
                // Close the motion file.
                prv_vCloseMotionFile(pxMotionFile);
                pxMotionFile = NULL;

                // Turn off all LED.
                for (i = 0; i < ucServoCount; i++) {
                    eG15SetLed(pxServoInfo[i].ucServoId, WRITE_NOW, 0);
                }
                
                
                
                // Remove the next button and create the teach button.
                GOLDeleteObjectByID(GID_TEACH_BTN_NEXT);
                BtnCreate(
                    GID_TEACH_BTN_TEACH,
                    BTN_TEACH_POS_X,
                    BTN_TEACH_POS_Y,
                    BTN_TEACH_POS_X + BTN_SIZE_X,
                    BTN_TEACH_POS_Y + BTN_SIZE_Y,
                    0,
                    BTN_DRAW | BTN_TEXTBOTTOM | BTN_NOPANEL,
                    "/theme/icon/teach.ico",
                    "TECH",
                    pxImgBtnScheme
                );

                

                // Disable the stop button.
                vGraphicsObjectDisable(pxBtnStop);
                
                // Update the file status.
                prv_vUpdateFileStatus();

                // Enable the radio button according to the file status.
                unsigned char i;
                for (i = 0; i < 5; i++) {
                    switch (prv_peFileStatus[i]) {
                        case FILE_NOT_FOUND:
                        case FILE_UNLOCKED:
                            vGraphicsObjectEnable(prv_pxRb[i]);
                            break;

                        case FILE_LOCKED:
                            vGraphicsObjectDisable(prv_pxRb[i]);
                            break;
                    }
                }
                

                // Modify the message text.
                vStaticTextUpdateText(pxTxtMsg1, "Press TEACH to start.");
                vStaticTextUpdateText(pxTxtMsg2, "");

                break;


            // Exit button.
            case GID_BTN_EXIT:
                // Close the motion file.
                prv_vCloseMotionFile(pxMotionFile);
                pxMotionFile = NULL;

                // Turn off all LED.
                for (i = 0; i < ucServoCount; i++) {
                    eG15SetLed(pxServoInfo[i].ucServoId, WRITE_NOW, 0);
                }

                vSetGuiPage(PAGE_MAIN_MENU);
                break;
        }
    }

    return 1;
}
