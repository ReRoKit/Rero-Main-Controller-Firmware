/*******************************************************************************
 * Functions for play page in rero GUI.
 *
 * Author: Kong Wai Weng
 * Company: Cytron Technologies Sdn. Bhd.
 * Email:  waiweng@cytron.com.my
 *******************************************************************************/

#include "Variables.h"
#include "PlayMotion.h"
#include "Planner.h"
#include "FatFs/FatFsWrapper.h"
#include "GUI/PlayPage.h"
#include "GUI/GraphicScreens.h"
#include "GUI/HideUnhideScreen.h"
#include "Sound Stream/Startup.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

// File information.
typedef struct {
    FILE_TYPE eFileType;
    char *szFileName;
    char szFullFilePath[MAX_FILENAME_LENGTH * 3];
    char szFileComment[MAX_FILENAME_LENGTH];
    unsigned char ucLock;
} FILE_INFO;



/*******************************************************************************
* PRIVATE GLOBAL VARIABLES
*******************************************************************************/

static FILE_INFO prv_xSelectedFileInfo = {0};


// Graphics object.
static RADIOBUTTON *prv_pxRb[5] = {NULL};

static STATICTEXT *prv_pxTxtMsg1 = NULL;
static STATICTEXT *prv_pxTxtMsg2 = NULL;
static STATICTEXT *prv_pxTxtMsg3 = NULL;

static DIGITALMETER *prv_pxDmFrame = NULL;
static DIGITALMETER *prv_pxDmTime = NULL;

static BUTTON *prv_pxBtnPlay = NULL;
static BUTTON *prv_pxBtnStop = NULL;

static PICTURE *prv_pxPictLock = NULL;
static BUTTON *prv_pxBtnLock = NULL;

// Lock / unlock icon.
static char *ICON_LOCKED = "/theme/icon/lock.ico";
static char *ICON_UNLOCKED = "/theme/icon/unlock.ico";



/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 *******************************************************************************/

static void prv_vUpdateFileInfo(void);



/*******************************************************************************
 * FUNCTION: prv_vUpdateFileInfo
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Update the file type and file comment for the selected file.
 * Planner file has the highest priority.
 *
 *******************************************************************************/
static void prv_vUpdateFileInfo(void)
{
    const unsigned short PLANNER_FILE_COMMENT_OFFSET = 1;
    const unsigned short MOTION_FILE_COMMENT_OFFSET = 2;
    

    FSFILE* pxFile;

    xSemaphoreTake(xSdCardMutex, portMAX_DELAY);


    // Get the full file path and add the extension for planner file.
    strcpy(prv_xSelectedFileInfo.szFullFilePath, szProgramFolder);
    strcat(prv_xSelectedFileInfo.szFullFilePath, "/");
    strcat(prv_xSelectedFileInfo.szFullFilePath, prv_xSelectedFileInfo.szFileName);
    strcat(prv_xSelectedFileInfo.szFullFilePath, szPlannerFileExt);

    // Open the planner file.
    pxFile = FSfopen(prv_xSelectedFileInfo.szFullFilePath, "r");

    // File type is planner file if the file can be opened.
    if (pxFile != NULL) {
        prv_xSelectedFileInfo.eFileType = PLANNER_FILE;
        
        // Read the file attributes and check whether the file is locked.
        xFSGetReadOnlyFlag(prv_xSelectedFileInfo.szFullFilePath, &prv_xSelectedFileInfo.ucLock);

        // Read the file comment.
        FSfseek(pxFile, PLANNER_FILE_COMMENT_OFFSET, SEEK_SET);
        FSfread(prv_xSelectedFileInfo.szFileComment, 1, MAX_FILENAME_LENGTH, pxFile);
    }

    // If the planner file cannot be opened, try with motion file.
    else {
        // Get the full file path and add the extension for motion file.
        strcpy(prv_xSelectedFileInfo.szFullFilePath, szProgramFolder);
        strcat(prv_xSelectedFileInfo.szFullFilePath, "/");
        strcat(prv_xSelectedFileInfo.szFullFilePath, prv_xSelectedFileInfo.szFileName);
        strcat(prv_xSelectedFileInfo.szFullFilePath, szMotionFileExt);

        // Open the motion file.
        pxFile = FSfopen(prv_xSelectedFileInfo.szFullFilePath, "r");

        // File type is motion file if the file can be opened.
        if (pxFile != NULL) {
            prv_xSelectedFileInfo.eFileType = MOTION_FILE;
            
            // Read the file attributes and check whether the file is locked.
            xFSGetReadOnlyFlag(prv_xSelectedFileInfo.szFullFilePath, &prv_xSelectedFileInfo.ucLock);

            // Read the file comment.
            FSfseek(pxFile, MOTION_FILE_COMMENT_OFFSET, SEEK_SET);
            FSfread(prv_xSelectedFileInfo.szFileComment, 1, MAX_FILENAME_LENGTH, pxFile);
        }

        // Set file type to not available and clear the comment if can't be opened.
        else {
            prv_xSelectedFileInfo.eFileType = NOT_AVAILABLE;
            prv_xSelectedFileInfo.ucLock = 0;

            unsigned char i;
            for (i = 0; i < MAX_FILENAME_LENGTH; i++) {
                prv_xSelectedFileInfo.szFileComment[i] = 0;
            }
        }
    }


    // Close the file if it's opened.
    if (pxFile != NULL) {
        FSfclose(pxFile);
    }

    xSemaphoreGive(xSdCardMutex);
}



/*******************************************************************************
 * FUNCTION: vCreatePlay
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the Play page.
 *
 *******************************************************************************/
void vCreatePlay(void)
{
    const unsigned short BTN_PLAY_POS_X = 125;
    const unsigned short BTN_PLAY_POS_Y = 145;
    const unsigned short BTN_STOP_POS_X = 220;
    const unsigned short BTN_STOP_POS_Y = 145;
    const unsigned short BTN_SIZE_X = 72;
    const unsigned short BTN_SIZE_Y = 72;
    
    const unsigned short BTN_LOCK_POS_X = 265;
    const unsigned short BTN_LOCK_POS_Y = 100;
    const unsigned short BTN_LOCK_SIZE_X = 40;
    const unsigned short BTN_LOCK_SIZE_Y = 40;

    const unsigned short RB_POS_X = 20;
    const unsigned short RB_POS_Y1 = 50;
    const unsigned short RB_POS_Y2 = 85;
    const unsigned short RB_POS_Y3 = 120;
    const unsigned short RB_POS_Y4 = 155;
    const unsigned short RB_POS_Y5 = 190;
    const unsigned short RB_SIZE_X = 80;
    const unsigned short RB_SIZE_Y = 30;

    const unsigned short TXT_POS_X1 = 125;
    const unsigned short TXT_POS_X2 = 220;
    const unsigned short TXT_POS_Y1 = 50;
    const unsigned short TXT_POS_Y2 = 70;
    const unsigned short TXT_POS_Y3 = 100;
    const unsigned short TXT_POS_Y4 = 120;
    const unsigned short TXT_SIZE_Y = 20;
    
    const unsigned short DM_SIZE_X = 40;


    // Deselect the file.
    prv_xSelectedFileInfo.szFileName = NULL;


    GOLFree();
    vCreatePageWindow("PLAY");
    
    
    
    // Create the lock button.
    prv_pxPictLock = PictCreate( GID_PLAY_IMG_LOCK,
                                 BTN_LOCK_POS_X,                   BTN_LOCK_POS_Y,
                                 BTN_LOCK_POS_X + BTN_LOCK_SIZE_X, BTN_LOCK_POS_Y + BTN_LOCK_SIZE_Y,
                                 PICT_DRAW | PICT_HIDE | PICT_DISABLED, 1, NULL, pxDefaultScheme );
    
    prv_pxBtnLock = BtnCreate( GID_PLAY_BTN_LOCK,
                               BTN_LOCK_POS_X,                   BTN_LOCK_POS_Y,
                               BTN_LOCK_POS_X + BTN_LOCK_SIZE_X, BTN_LOCK_POS_Y + BTN_LOCK_SIZE_Y,
                               0, BTN_DRAW | BTN_DISABLED | BTN_NOPANEL, NULL, NULL, pxImgBtnScheme );
    
    
    
    // Create the select file radio button.
    prv_pxRb[0] = RbCreate(
        GID_PLAY_RB_FILE1,
        RB_POS_X,
        RB_POS_Y1,
        RB_POS_X + RB_SIZE_X,
        RB_POS_Y1 + RB_SIZE_Y,
        RB_DRAW | RB_GROUP,
        " File 1",
        pxRadioBtnScheme
    );

    prv_pxRb[1] = RbCreate(
        GID_PLAY_RB_FILE2,
        RB_POS_X,
        RB_POS_Y2,
        RB_POS_X + RB_SIZE_X,
        RB_POS_Y2 + RB_SIZE_Y,
        RB_DRAW,
        " File 2",
        pxRadioBtnScheme
    );

    prv_pxRb[2] = RbCreate(
        GID_PLAY_RB_FILE3,
        RB_POS_X,
        RB_POS_Y3,
        RB_POS_X + RB_SIZE_X,
        RB_POS_Y3 + RB_SIZE_Y,
        RB_DRAW,
        " File 3",
        pxRadioBtnScheme
    );

    prv_pxRb[3] = RbCreate(
        GID_PLAY_RB_FILE4,
        RB_POS_X,
        RB_POS_Y4,
        RB_POS_X + RB_SIZE_X,
        RB_POS_Y4 + RB_SIZE_Y,
        RB_DRAW,
        " File 4",
        pxRadioBtnScheme
    );

    prv_pxRb[4] = RbCreate(
        GID_PLAY_RB_FILE5,
        RB_POS_X,
        RB_POS_Y5,
        RB_POS_X + RB_SIZE_X,
        RB_POS_Y5 + RB_SIZE_Y,
        RB_DRAW,
        " File 5",
        pxRadioBtnScheme
    );

    
    // Create the message text 1.
    prv_pxTxtMsg1 = StCreate(
        GID_PLAY_TXT_MSG1,
        TXT_POS_X1,
        TXT_POS_Y1,
        GetMaxX() - 3,
        TXT_POS_Y1 + TXT_SIZE_Y,
        ST_DRAW,
        "Please select a file.",
        pxLightBlueTxtScheme
    );



    // Create the message text 2.
    prv_pxTxtMsg2 = StCreate(
        GID_PLAY_TXT_MSG2,
        TXT_POS_X1,
        TXT_POS_Y2,
        TXT_POS_X1 + 75,
        TXT_POS_Y2 + TXT_SIZE_Y,
        ST_DRAW,
        "",
        pxDefaultScheme
    );



    // Create the message text 3.
    prv_pxTxtMsg3 = StCreate(
        GID_PLAY_TXT_MSG3,
        TXT_POS_X1 + 75,
        TXT_POS_Y2,
        GetMaxX() - 3,
        TXT_POS_Y2 + TXT_SIZE_Y,
        ST_DRAW,
        "",
        pxDefaultScheme
    );



    // Create the time frame text and digital meter.
    StCreate(
        GID_PLAY_TXT_FRAME,
        TXT_POS_X1,
        TXT_POS_Y3,
        TXT_POS_X2,
        TXT_POS_Y3 + TXT_SIZE_Y,
        ST_DRAW,
        "Time Frame :",
        pxDefaultScheme
    );

    prv_pxDmFrame = DmCreate(
        GID_PLAY_DM_FRAME,
        TXT_POS_X2,
        TXT_POS_Y3,
        TXT_POS_X2 + DM_SIZE_X,
        TXT_POS_Y3 + TXT_SIZE_Y,
        DM_DRAW,
        0,
        4,
        0,
        pxDefaultScheme
    );



    // Create the time text and digital meter.
    StCreate(
        GID_PLAY_TXT_TIME,
        TXT_POS_X1,
        TXT_POS_Y4,
        TXT_POS_X2,
        TXT_POS_Y4 + TXT_SIZE_Y,
        ST_DRAW,
        "Time (sec) :",
        pxDefaultScheme
    );

    prv_pxDmTime = DmCreate(
        GID_PLAY_DM_TIME,
        TXT_POS_X2,
        TXT_POS_Y4,
        TXT_POS_X2 + DM_SIZE_X,
        TXT_POS_Y4 + TXT_SIZE_Y,
        DM_DRAW,
        0,
        4,
        0,
        pxDefaultScheme
    );


    
    // Create the play button.
    prv_pxBtnPlay = BtnCreate(
        GID_PLAY_BTN_PLAY,
        BTN_PLAY_POS_X,
        BTN_PLAY_POS_Y,
        BTN_PLAY_POS_X + BTN_SIZE_X,
        BTN_PLAY_POS_Y + BTN_SIZE_Y,
        0,
        BTN_DRAW | BTN_TEXTBOTTOM | BTN_NOPANEL | BTN_DISABLED,
        "/theme/icon/play.ico",
        "PLAY",
        pxImgBtnScheme
    );



    // Create the stop button.
    prv_pxBtnStop = BtnCreate(
        GID_PLAY_BTN_STOP,
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
    
    
    
#ifdef SHOWCASE_ROBOT
    // Play File 5 automatically when power up.
    // Select File 5.
    SetState(prv_pxRb[4], RB_CHECKED);
    usMsgPlay(RB_MSG_CHECKED, (OBJ_HEADER*)prv_pxRb[4], NULL);
    
    // Press the play button.
    usMsgPlay(BTN_MSG_RELEASED, (OBJ_HEADER*)prv_pxBtnPlay, NULL);
#endif
}



/*******************************************************************************
 * FUNCTION: usMsgPlay
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
 * Handle the touchscreen event of the Play page.
 *
 *******************************************************************************/
WORD usMsgPlay(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    // Radio button checked.
    if (objMsg == RB_MSG_CHECKED) {
        // Reset the time frame and time.
        vUpdatePlayPageTimeFrame(0,0);


        // Select the file name based on radio button.
        switch (GetObjID(pObj)) {
            case GID_PLAY_RB_FILE1:
                prv_xSelectedFileInfo.szFileName = "File1";
                break;
            case GID_PLAY_RB_FILE2:
                prv_xSelectedFileInfo.szFileName = "File2";
                break;
            case GID_PLAY_RB_FILE3:
                prv_xSelectedFileInfo.szFileName = "File3";
                break;
            case GID_PLAY_RB_FILE4:
                prv_xSelectedFileInfo.szFileName = "File4";
                break;
            case GID_PLAY_RB_FILE5:
                prv_xSelectedFileInfo.szFileName = "File5";
                break;
            default:
                prv_xSelectedFileInfo.szFileName = NULL;
                break;
        }

        // Update the file information.
        prv_vUpdateFileInfo();
        
        
        
        // If the file is not available.
        if (prv_xSelectedFileInfo.eFileType == NOT_AVAILABLE) {
            // Modify the message text.
            vStaticTextUpdateText(prv_pxTxtMsg1, "File not available.");

            // Disable the play button.
            vGraphicsObjectDisable(prv_pxBtnPlay);
            
            // Hide and disable the lock button.
            vGraphicsObjectHide(prv_pxPictLock);
            vGraphicsObjectDisable(prv_pxBtnLock);
        }
        // File is available.
        else {
            // Modify the message text.
            vStaticTextUpdateText(prv_pxTxtMsg1, "Press PLAY to start playing.");

            // Enable the play button.
            vGraphicsObjectEnable(prv_pxBtnPlay);
            
            // Unhide and enable the lock/unlock button.
            if (prv_xSelectedFileInfo.ucLock == 0) {
                PictSetBitmap(prv_pxPictLock, ICON_UNLOCKED);
            } else {
                PictSetBitmap(prv_pxPictLock, ICON_LOCKED);
            }
            vGraphicsObjectUnhide(prv_pxPictLock);
            vGraphicsObjectEnable(prv_pxBtnLock);
        }


        
        // Show the file type and comment.
        if (prv_xSelectedFileInfo.eFileType == PLANNER_FILE) {
            vStaticTextUpdateText(prv_pxTxtMsg2, "Planner :");
            vStaticTextUpdateText(prv_pxTxtMsg3, prv_xSelectedFileInfo.szFileComment);
        }
        else if (prv_xSelectedFileInfo.eFileType == MOTION_FILE) {
            vStaticTextUpdateText(prv_pxTxtMsg2, "Motion :");
            vStaticTextUpdateText(prv_pxTxtMsg3, prv_xSelectedFileInfo.szFileComment);
        }
        else {
            vStaticTextUpdateText(prv_pxTxtMsg2, "");
            vStaticTextUpdateText(prv_pxTxtMsg3, "");
        }
    }



    // Button pressed and released.
    else if (objMsg == BTN_MSG_RELEASED) {

        PLAY_RESULT ePlayResult;

        switch (GetObjID(pObj)) {
            // Play button.
            case GID_PLAY_BTN_PLAY:
                // Play the planner file.
                if (prv_xSelectedFileInfo.eFileType == PLANNER_FILE) {
                    ePlayResult = ePlannerRun(prv_xSelectedFileInfo.szFileName);
                }

                // Play the motion file.
                else if (prv_xSelectedFileInfo.eFileType == MOTION_FILE) {
                    // If the motion file can be played...
                    ePlayResult = ePlayMotionStart(prv_xSelectedFileInfo.szFileName);
                }


                // If successfully open the file for play.
                if (ePlayResult == PLAY_NO_ERROR) {
                    // Modify the message text.
                    vStaticTextUpdateText(prv_pxTxtMsg1, "Now playing :");

                    // Disable the play button.
                    vGraphicsObjectDisable(prv_pxBtnPlay);

                    // Enable the stop button.
                    vGraphicsObjectEnable(prv_pxBtnStop);
                    
                    // Disable the lock button.
                    vGraphicsObjectDisable(prv_pxBtnLock);

                    // Disable all radio buttons.
                    unsigned char i;
                    for(i = 0; i < 5; i++) {
                        vGraphicsObjectDisable(prv_pxRb[i]);
                    }
                }
                break;


            // Stop button.
            case GID_PLAY_BTN_STOP:
                // Stop the planner file.
                if (prv_xSelectedFileInfo.eFileType == PLANNER_FILE) {
                    vPlannerStop(prv_xSelectedFileInfo.szFileName, STOP_NOW);
                }
                // Stop the motion file.
                else if (prv_xSelectedFileInfo.eFileType == MOTION_FILE) {
                    vPlayMotionStopAll(STOP_NOW);
                }
                break;
                
                
            // Lock button.
            case GID_PLAY_BTN_LOCK:
                // Toggle the read only flag.
                // Make sure there is no error.
                xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
                if (xFSSetReadOnlyFlag(prv_xSelectedFileInfo.szFullFilePath, !prv_xSelectedFileInfo.ucLock) == FR_OK) {
                    prv_xSelectedFileInfo.ucLock ^= 1;
                    
                    if (prv_xSelectedFileInfo.ucLock == 0) {
                        PictSetBitmap(prv_pxPictLock, ICON_UNLOCKED);
                    } else {
                        PictSetBitmap(prv_pxPictLock, ICON_LOCKED);
                    }
                    GOLRedraw(prv_pxPictLock);
                }
                xSemaphoreGive(xSdCardMutex);
                break;


            // Exit button.
            case GID_BTN_EXIT:
                // Stop the planner file.
                if (prv_xSelectedFileInfo.eFileType == PLANNER_FILE) {
                    vPlannerStop(prv_xSelectedFileInfo.szFileName, STOP_NOW);
                }
                // Stop the motion file.
                else if (prv_xSelectedFileInfo.eFileType == MOTION_FILE) {
                    vPlayMotionStopAll(STOP_NOW);
                }
                
                // Delay a while to let the motion/planner stop running.
                vTaskDelay(250 / portTICK_RATE_MS);
                
                // Disable all output.
                vEMDisableAllOutput();

                // Set all the graphics objects to null.
                unsigned char i;
                for (i = 0; i < 5; i++) {
                    prv_pxRb[i] = NULL;
                }
                prv_pxTxtMsg1 = NULL;
                prv_pxTxtMsg2 = NULL;
                prv_pxTxtMsg3 = NULL;
                prv_pxDmFrame = NULL;
                prv_pxDmTime = NULL;
                prv_pxBtnPlay = NULL;
                prv_pxBtnStop = NULL;
                prv_pxPictLock = NULL;
                prv_pxBtnLock = NULL;

                // Back to main screen.
                vSetGuiPage(PAGE_MAIN_MENU);
                break;
        }
    }

    return 1;
}



/*******************************************************************************
 * FUNCTION: vUpdatePlayPageTimeFrame
 *
 * PARAMETERS:
 * ~ usTimeFrame    - Time frame count.
 * ~ usTime         - Elapsed time in second.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Update the time frame and time in the play page.
 *
 *******************************************************************************/
void vUpdatePlayPageTimeFrame(unsigned short usTimeFrame, unsigned short usTime)
{
    if (prv_pxDmFrame != NULL) {
        DmSetValue(prv_pxDmFrame, (DWORD)usTimeFrame);
        SetState(prv_pxDmFrame, DM_DRAW);
    }

    if (prv_pxDmTime != NULL) {
        DmSetValue(prv_pxDmTime,(DWORD)usTime);
        SetState(prv_pxDmTime, DM_DRAW);
    }
}



/*******************************************************************************
 * FUNCTION: vUpdatePlayPageMsg1
 *
 * PARAMETERS:
 * ~ szText - New text.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Update the message 1 text.
 *
 *******************************************************************************/
void vUpdatePlayPageMsg1(char *szText)
{
    vStaticTextUpdateText(prv_pxTxtMsg1, szText);
}



/*******************************************************************************
 * FUNCTION: vUpdatePlayPageMsg2
 *
 * PARAMETERS:
 * ~ szText - New text.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Update the message 2 text.
 *
 *******************************************************************************/
void vUpdatePlayPageMsg2(char *szText)
{
    vStaticTextUpdateText(prv_pxTxtMsg2, szText);
}



/*******************************************************************************
 * FUNCTION: vUpdatePlayPageMsg3
 *
 * PARAMETERS:
 * ~ szText - New text.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Update the message 3 text.
 *
 *******************************************************************************/
void vUpdatePlayPageMsg3(char *szText)
{
    vStaticTextUpdateText(prv_pxTxtMsg3, szText);
}



/*******************************************************************************
 * FUNCTION: vUpdatePlayPageEndPlaying
 *
 * PARAMETERS:
 * ~ ePlayingType   - To differentiate whether this function is called from
 *                    motion or planner task.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Update the play page when finish playing the motion file.
 *
 *******************************************************************************/
void vUpdatePlayPageEndPlaying(FILE_TYPE ePlayingType)
{
    // If the screen is locked, wake it up.
    vUnlockScreen();

    // Make sure the current screen is play page.
    if (eGetGuiPage() == PAGE_PLAY) {
        // Make sure the selected file type is same with the playing type.
        if (prv_xSelectedFileInfo.eFileType == ePlayingType) {
            // Modify the message text.
            vStaticTextUpdateText(prv_pxTxtMsg1, "Press PLAY to start playing.");

            // Show the file type and comment.
            if (prv_xSelectedFileInfo.eFileType == PLANNER_FILE) {
                vStaticTextUpdateText(prv_pxTxtMsg2, "Planner :");
                vStaticTextUpdateText(prv_pxTxtMsg3, prv_xSelectedFileInfo.szFileComment);
            }
            else if (prv_xSelectedFileInfo.eFileType == MOTION_FILE) {
                vStaticTextUpdateText(prv_pxTxtMsg2, "Motion :");
                vStaticTextUpdateText(prv_pxTxtMsg3, prv_xSelectedFileInfo.szFileComment);
            }

            // Disable the stop button.
            vGraphicsObjectDisable(prv_pxBtnStop);

            // Enable the play button.
            vGraphicsObjectEnable(prv_pxBtnPlay);
            
            // Enable the lock button.
            vGraphicsObjectEnable(prv_pxBtnLock);

            // Enable the radio buttons.
            unsigned char i;
            for(i = 0; i < 5; i++) {
                vGraphicsObjectEnable(prv_pxRb[i]);
            }
        }
    }
}
