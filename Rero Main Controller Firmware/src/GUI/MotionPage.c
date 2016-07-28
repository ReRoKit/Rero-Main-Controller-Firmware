/*******************************************************************************
 * Functions for motion page in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "GUI/MotionPage.h"
#include "GUI/GraphicScreens.h"
#include "Variables.h"
#include "PlayMotion.h"
#include "TeachMotion.h"
#include "Planner.h"
#include "FatFs/FatFsWrapper.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

// Position and size for graphic objects.
#define TXT_HEIGHT                  20

// Gamepad.
#define IMG_GAMEPAD_WIDTH           90
#define IMG_GAMEPAD_HEIGHT          90
#define IMG_GAMEPAD_C2C_X           190

#define IMG_GAMEPAD_LOCK_WIDTH      14
#define IMG_GAMEPAD_LOCK_HEIGHT     20
#define IMG_GAMEPAD_LOCK_OFFSET_X   24
#define IMG_GAMEPAD_LOCK_OFFSET_Y   20

#define BTN_GAMEPAD_LENGTH          35

// Left gamepad.
#define IMG_GAMEPAD_LEFT_L      ((WND_LIMIT_LEFT + WND_LIMIT_RIGHT) / 2 - (IMG_GAMEPAD_C2C_X / 2) - (IMG_GAMEPAD_WIDTH / 2))
#define IMG_GAMEPAD_LEFT_R      (IMG_GAMEPAD_LEFT_L + IMG_GAMEPAD_WIDTH)
#define IMG_GAMEPAD_LEFT_T      (WND_SUBPAGE_TOP + 15)
#define IMG_GAMEPAD_LEFT_B      (IMG_GAMEPAD_LEFT_T + IMG_GAMEPAD_HEIGHT)

#define GAMEPAD_LEFT_CENTER_X   ((IMG_GAMEPAD_LEFT_L + IMG_GAMEPAD_LEFT_R) / 2)
#define GAMEPAD_LEFT_CENTER_Y   ((IMG_GAMEPAD_LEFT_T + IMG_GAMEPAD_LEFT_B) / 2)

#define CENTER_SQUARE_LEFT_L    (IMG_GAMEPAD_LEFT_L + 30)
#define CENTER_SQUARE_LEFT_R    (IMG_GAMEPAD_LEFT_R - 30)
#define CENTER_SQUARE_LEFT_T    (IMG_GAMEPAD_LEFT_T + 30)
#define CENTER_SQUARE_LEFT_B    (IMG_GAMEPAD_LEFT_B - 30)

#define BTN_UP_L                CENTER_SQUARE_LEFT_L
#define BTN_UP_R                CENTER_SQUARE_LEFT_R
#define BTN_UP_T                (CENTER_SQUARE_LEFT_T - BTN_GAMEPAD_LENGTH)
#define BTN_UP_B                CENTER_SQUARE_LEFT_T

#define BTN_DOWN_L              CENTER_SQUARE_LEFT_L
#define BTN_DOWN_R              CENTER_SQUARE_LEFT_R
#define BTN_DOWN_T              CENTER_SQUARE_LEFT_B
#define BTN_DOWN_B              (CENTER_SQUARE_LEFT_B + BTN_GAMEPAD_LENGTH)

#define BTN_LEFT_L              (CENTER_SQUARE_LEFT_L - BTN_GAMEPAD_LENGTH)
#define BTN_LEFT_R              CENTER_SQUARE_LEFT_L
#define BTN_LEFT_T              CENTER_SQUARE_LEFT_T
#define BTN_LEFT_B              CENTER_SQUARE_LEFT_B

#define BTN_RIGHT_L             CENTER_SQUARE_LEFT_R
#define BTN_RIGHT_R             (CENTER_SQUARE_LEFT_R + BTN_GAMEPAD_LENGTH)
#define BTN_RIGHT_T             CENTER_SQUARE_LEFT_T
#define BTN_RIGHT_B             CENTER_SQUARE_LEFT_B

#define IMG_LOCK_UP_L           (GAMEPAD_LEFT_CENTER_X - (IMG_GAMEPAD_LOCK_WIDTH / 2))
#define IMG_LOCK_UP_R           (IMG_LOCK_UP_L + IMG_GAMEPAD_LOCK_WIDTH)
#define IMG_LOCK_UP_B           (GAMEPAD_LEFT_CENTER_Y - IMG_GAMEPAD_LOCK_OFFSET_Y)
#define IMG_LOCK_UP_T           (IMG_LOCK_UP_B - IMG_GAMEPAD_LOCK_HEIGHT)

#define IMG_LOCK_DOWN_L         IMG_LOCK_UP_L
#define IMG_LOCK_DOWN_R         IMG_LOCK_UP_R
#define IMG_LOCK_DOWN_T         (GAMEPAD_LEFT_CENTER_Y + IMG_GAMEPAD_LOCK_OFFSET_Y)
#define IMG_LOCK_DOWN_B         (IMG_LOCK_DOWN_T + IMG_GAMEPAD_LOCK_HEIGHT)

#define IMG_LOCK_LEFT_R         (GAMEPAD_LEFT_CENTER_X - IMG_GAMEPAD_LOCK_OFFSET_X)
#define IMG_LOCK_LEFT_L         (IMG_LOCK_LEFT_R - IMG_GAMEPAD_LOCK_WIDTH)
#define IMG_LOCK_LEFT_T         (GAMEPAD_LEFT_CENTER_Y - (IMG_GAMEPAD_LOCK_HEIGHT / 2))
#define IMG_LOCK_LEFT_B         (IMG_LOCK_LEFT_T + IMG_GAMEPAD_LOCK_HEIGHT)

#define IMG_LOCK_RIGHT_L        (GAMEPAD_LEFT_CENTER_X + IMG_GAMEPAD_LOCK_OFFSET_X)
#define IMG_LOCK_RIGHT_R        (IMG_LOCK_RIGHT_L + IMG_GAMEPAD_LOCK_WIDTH)
#define IMG_LOCK_RIGHT_T        IMG_LOCK_LEFT_T
#define IMG_LOCK_RIGHT_B        IMG_LOCK_LEFT_B

// Right gamepad.
#define IMG_GAMEPAD_RIGHT_L     ((WND_LIMIT_LEFT + WND_LIMIT_RIGHT) / 2 + (IMG_GAMEPAD_C2C_X / 2) - (IMG_GAMEPAD_WIDTH / 2))
#define IMG_GAMEPAD_RIGHT_R     (IMG_GAMEPAD_RIGHT_L + IMG_GAMEPAD_WIDTH)
#define IMG_GAMEPAD_RIGHT_T     IMG_GAMEPAD_LEFT_T
#define IMG_GAMEPAD_RIGHT_B     IMG_GAMEPAD_LEFT_B

#define GAMEPAD_RIGHT_CENTER_X  ((IMG_GAMEPAD_RIGHT_L + IMG_GAMEPAD_RIGHT_R) / 2)
#define GAMEPAD_RIGHT_CENTER_Y  ((IMG_GAMEPAD_RIGHT_T + IMG_GAMEPAD_RIGHT_B) / 2)

#define CENTER_SQUARE_RIGHT_L   (IMG_GAMEPAD_RIGHT_L + 30)
#define CENTER_SQUARE_RIGHT_R   (IMG_GAMEPAD_RIGHT_R - 30)
#define CENTER_SQUARE_RIGHT_T   (IMG_GAMEPAD_RIGHT_T + 30)
#define CENTER_SQUARE_RIGHT_B   (IMG_GAMEPAD_RIGHT_B - 30)

#define BTN_X_L                 (CENTER_SQUARE_RIGHT_L - BTN_GAMEPAD_LENGTH)
#define BTN_X_R                 CENTER_SQUARE_RIGHT_L
#define BTN_X_T                 CENTER_SQUARE_RIGHT_T
#define BTN_X_B                 CENTER_SQUARE_RIGHT_B

#define BTN_Y_L                 CENTER_SQUARE_RIGHT_L
#define BTN_Y_R                 CENTER_SQUARE_RIGHT_R
#define BTN_Y_T                 (CENTER_SQUARE_RIGHT_T - BTN_GAMEPAD_LENGTH)
#define BTN_Y_B                 CENTER_SQUARE_RIGHT_T

#define BTN_A_L                 CENTER_SQUARE_RIGHT_L
#define BTN_A_R                 CENTER_SQUARE_RIGHT_R
#define BTN_A_T                 CENTER_SQUARE_RIGHT_B
#define BTN_A_B                 (CENTER_SQUARE_RIGHT_B + BTN_GAMEPAD_LENGTH)

#define BTN_B_L                 CENTER_SQUARE_RIGHT_R
#define BTN_B_R                 (CENTER_SQUARE_RIGHT_R + BTN_GAMEPAD_LENGTH)
#define BTN_B_T                 CENTER_SQUARE_RIGHT_T
#define BTN_B_B                 CENTER_SQUARE_RIGHT_B

#define IMG_LOCK_X_R            (GAMEPAD_RIGHT_CENTER_X - IMG_GAMEPAD_LOCK_OFFSET_X)
#define IMG_LOCK_X_L            (IMG_LOCK_X_R - IMG_GAMEPAD_LOCK_WIDTH)
#define IMG_LOCK_X_T            (GAMEPAD_RIGHT_CENTER_Y - (IMG_GAMEPAD_LOCK_HEIGHT / 2))
#define IMG_LOCK_X_B            (IMG_LOCK_X_T + IMG_GAMEPAD_LOCK_HEIGHT)

#define IMG_LOCK_Y_L            (GAMEPAD_RIGHT_CENTER_X - (IMG_GAMEPAD_LOCK_WIDTH / 2))
#define IMG_LOCK_Y_R            (IMG_LOCK_Y_L + IMG_GAMEPAD_LOCK_WIDTH)
#define IMG_LOCK_Y_B            (GAMEPAD_RIGHT_CENTER_Y - IMG_GAMEPAD_LOCK_OFFSET_Y)
#define IMG_LOCK_Y_T            (IMG_LOCK_Y_B - IMG_GAMEPAD_LOCK_HEIGHT)

#define IMG_LOCK_A_L            IMG_LOCK_Y_L
#define IMG_LOCK_A_R            IMG_LOCK_Y_R
#define IMG_LOCK_A_T            (GAMEPAD_RIGHT_CENTER_Y + IMG_GAMEPAD_LOCK_OFFSET_Y)
#define IMG_LOCK_A_B            (IMG_LOCK_A_T + IMG_GAMEPAD_LOCK_HEIGHT)

#define IMG_LOCK_B_L            (GAMEPAD_RIGHT_CENTER_X + IMG_GAMEPAD_LOCK_OFFSET_X)
#define IMG_LOCK_B_R            (IMG_LOCK_B_L + IMG_GAMEPAD_LOCK_WIDTH)
#define IMG_LOCK_B_T            IMG_LOCK_X_T
#define IMG_LOCK_B_B            IMG_LOCK_X_B

// Bottom button row.
#define BTN_BOTTOM_WIDTH        52
#define BTN_BOTTOM_HEIGHT       30
#define BTN_BOTTOM_C2C_X        ((IMG_GAMEPAD_RIGHT_R - IMG_GAMEPAD_LEFT_L) / 5)

// Bottom button 1.
#define BTN_1_L                 (IMG_GAMEPAD_LEFT_L + (BTN_BOTTOM_C2C_X / 2) - (BTN_BOTTOM_WIDTH / 2))
#define BTN_1_R                 (BTN_1_L + BTN_BOTTOM_WIDTH)
#define BTN_1_T                 (IMG_GAMEPAD_LEFT_B + 11)
#define BTN_1_B                 (BTN_1_T + BTN_BOTTOM_HEIGHT)

// Bottom button 2.
#define BTN_2_L                 (BTN_1_L + BTN_BOTTOM_C2C_X)
#define BTN_2_R                 (BTN_2_L + BTN_BOTTOM_WIDTH)
#define BTN_2_T                 BTN_1_T
#define BTN_2_B                 BTN_1_B

// Bottom button 3.
#define BTN_3_L                 (BTN_2_L + BTN_BOTTOM_C2C_X)
#define BTN_3_R                 (BTN_3_L + BTN_BOTTOM_WIDTH)
#define BTN_3_T                 BTN_1_T
#define BTN_3_B                 BTN_1_B

// Bottom button 4.
#define BTN_4_L                 (BTN_3_L + BTN_BOTTOM_C2C_X)
#define BTN_4_R                 (BTN_4_L + BTN_BOTTOM_WIDTH)
#define BTN_4_T                 BTN_1_T
#define BTN_4_B                 BTN_1_B

// Bottom button 5.
#define BTN_5_L                 (BTN_4_L + BTN_BOTTOM_C2C_X)
#define BTN_5_R                 (BTN_5_L + BTN_BOTTOM_WIDTH)
#define BTN_5_T                 BTN_1_T
#define BTN_5_B                 BTN_1_B



// Teach, play, next and stop button.
#define BTN_TEACH_PLAY_GAP      10

// Teach button.
#define BTN_TEACH_HEIGHT        40
#define BTN_TEACH_L             (IMG_GAMEPAD_LEFT_R + 13)
#define BTN_TEACH_R             (IMG_GAMEPAD_RIGHT_L - 13)
#define BTN_TEACH_B             (GAMEPAD_LEFT_CENTER_Y - (BTN_TEACH_PLAY_GAP / 2))
#define BTN_TEACH_T             (BTN_TEACH_B - BTN_TEACH_HEIGHT)

#define IMG_TEACH_HEIGHT        20
#define IMG_TEACH_L             BTN_TEACH_L
#define IMG_TEACH_R             BTN_TEACH_R
#define IMG_TEACH_B             BTN_TEACH_B
#define IMG_TEACH_T             (IMG_TEACH_B - IMG_TEACH_HEIGHT)

// Play button.
#define BTN_PLAY_HEIGHT         BTN_TEACH_HEIGHT
#define BTN_PLAY_L              BTN_TEACH_L
#define BTN_PLAY_R              BTN_TEACH_R
#define BTN_PLAY_T              (BTN_TEACH_B + BTN_TEACH_PLAY_GAP)
#define BTN_PLAY_B              (BTN_PLAY_T + BTN_PLAY_HEIGHT)

#define IMG_PLAY_HEIGHT         IMG_TEACH_HEIGHT
#define IMG_PLAY_L              BTN_PLAY_L
#define IMG_PLAY_R              BTN_PLAY_R
#define IMG_PLAY_T              BTN_PLAY_T
#define IMG_PLAY_B              (IMG_PLAY_T + IMG_PLAY_HEIGHT)

// Next button.
#define BTN_NEXT_HEIGHT         BTN_TEACH_HEIGHT
#define BTN_NEXT_L              BTN_TEACH_L
#define BTN_NEXT_R              BTN_TEACH_R
#define BTN_NEXT_T              BTN_TEACH_T
#define BTN_NEXT_B              BTN_TEACH_B

#define IMG_NEXT_HEIGHT         IMG_TEACH_HEIGHT
#define IMG_NEXT_L              BTN_NEXT_L
#define IMG_NEXT_R              BTN_NEXT_R
#define IMG_NEXT_B              BTN_NEXT_B
#define IMG_NEXT_T              (IMG_NEXT_B - IMG_NEXT_HEIGHT)

// Stop teaching button.
#define BTN_STOPTEACH_HEIGHT    BTN_TEACH_HEIGHT
#define BTN_STOPTEACH_L         BTN_TEACH_L
#define BTN_STOPTEACH_R         BTN_TEACH_R
#define BTN_STOPTEACH_T         BTN_PLAY_T
#define BTN_STOPTEACH_B         BTN_PLAY_B

#define IMG_STOPTEACH_HEIGHT    IMG_TEACH_HEIGHT
#define IMG_STOPTEACH_L         BTN_STOPTEACH_L
#define IMG_STOPTEACH_R         BTN_STOPTEACH_R
#define IMG_STOPTEACH_T         BTN_STOPTEACH_T
#define IMG_STOPTEACH_B         (IMG_STOPTEACH_T + IMG_STOPTEACH_HEIGHT)

// Stop playing button.
#define BTN_STOPPLAY_HEIGHT     BTN_TEACH_HEIGHT
#define BTN_STOPPLAY_L          BTN_TEACH_L
#define BTN_STOPPLAY_R          BTN_TEACH_R
#define BTN_STOPPLAY_T          (GAMEPAD_LEFT_CENTER_Y - (BTN_STOPPLAY_HEIGHT / 2))
#define BTN_STOPPLAY_B          (BTN_STOPPLAY_T + BTN_STOPPLAY_HEIGHT)

#define IMG_STOPPLAY_HEIGHT     IMG_TEACH_HEIGHT
#define IMG_STOPPLAY_L          BTN_STOPPLAY_L
#define IMG_STOPPLAY_R          BTN_STOPPLAY_R
#define IMG_STOPPLAY_T          (GAMEPAD_LEFT_CENTER_Y - (IMG_STOPPLAY_HEIGHT / 2))
#define IMG_STOPPLAY_B          (IMG_STOPPLAY_T + IMG_STOPPLAY_HEIGHT)



// Message 1
#define TXT_MSG1_WIDTH          85
#define TXT_MSG1_L              (WND_SUBPAGE_LEFT + 10)
#define TXT_MSG1_R              (TXT_MSG1_L + TXT_MSG1_WIDTH)
#define TXT_MSG1_B              (WND_SUBPAGE_BOTTOM - 13)
#define TXT_MSG1_T              (TXT_MSG1_B - TXT_HEIGHT)

// Message 2
#define TXT_MSG2_WIDTH          125
#define TXT_MSG2_L              TXT_MSG1_R
#define TXT_MSG2_R              (TXT_MSG2_L + TXT_MSG2_WIDTH)
#define TXT_MSG2_T              TXT_MSG1_T
#define TXT_MSG2_B              TXT_MSG1_B



// Time value.
#define DM_TIME_VALUE_WIDTH     36
#define DM_TIME_VALUE_R         (WND_SUBPAGE_RIGHT - 10)
#define DM_TIME_VALUE_L         (DM_TIME_VALUE_R - DM_TIME_VALUE_WIDTH)
#define DM_TIME_VALUE_B         (WND_SUBPAGE_BOTTOM - 3)
#define DM_TIME_VALUE_T         (DM_TIME_VALUE_B - TXT_HEIGHT)

// Time label.
#define TXT_TIME_LABEL_WIDTH    46
#define TXT_TIME_LABEL_R        DM_TIME_VALUE_L
#define TXT_TIME_LABEL_L        (TXT_TIME_LABEL_R - TXT_TIME_LABEL_WIDTH)
#define TXT_TIME_LABEL_B        DM_TIME_VALUE_B
#define TXT_TIME_LABEL_T        DM_TIME_VALUE_T



// Frame value.
#define DM_FRAME_VALUE_R        DM_TIME_VALUE_R
#define DM_FRAME_VALUE_L        DM_TIME_VALUE_L
#define DM_FRAME_VALUE_B        DM_TIME_VALUE_T
#define DM_FRAME_VALUE_T        (DM_FRAME_VALUE_B - TXT_HEIGHT)

// Frame label.
#define TXT_FRAME_LABEL_R       TXT_TIME_LABEL_R
#define TXT_FRAME_LABEL_L       TXT_TIME_LABEL_L
#define TXT_FRAME_LABEL_B       DM_FRAME_VALUE_B
#define TXT_FRAME_LABEL_T       DM_FRAME_VALUE_T



// Edit button.
#define BTN_EDIT_WIDTH          40
#define BTN_EDIT_HEIGHT         20
#define BTN_EDIT_R              (WND_SUBPAGE_RIGHT - 10)
#define BTN_EDIT_L              (BTN_EDIT_R - BTN_EDIT_WIDTH)
#define BTN_EDIT_B              TXT_MSG1_B
#define BTN_EDIT_T              (BTN_EDIT_B - BTN_EDIT_HEIGHT)

// Lock icon.
#define IMG_LOCK_WIDTH          14
#define IMG_LOCK_HEIGHT         20
#define IMG_LOCK_R              (BTN_EDIT_L - 5)
#define IMG_LOCK_L              (IMG_LOCK_R - IMG_LOCK_WIDTH)
#define IMG_LOCK_B              BTN_EDIT_B
#define IMG_LOCK_T              (IMG_LOCK_B - IMG_LOCK_HEIGHT)

// Done button.
#define BTN_DONE_L              IMG_LOCK_L
#define BTN_DONE_R              BTN_EDIT_R
#define BTN_DONE_T              BTN_EDIT_T
#define BTN_DONE_B              BTN_EDIT_B



// Selected button.
typedef enum __attribute__((packed)) {
    BUTTON_NONE,

    BUTTON_1,
    BUTTON_2,
    BUTTON_3,
    BUTTON_4,
    BUTTON_5,

    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_RIGHT,

    BUTTON_X,
    BUTTON_Y,
    BUTTON_A,
    BUTTON_B
} SELECTED_BUTTON;



// File information.
typedef struct {
    FILE_TYPE eFileType;
    char *szFileName;
    char szFullFilePath[MAX_FILENAME_LENGTH * 3];
    char szFileComment[MAX_FILENAME_LENGTH];
    unsigned char ucLock;
} FILE_INFO;



/*******************************************************************************
 * PUBLIC GLOBAL VARIABLES
 *******************************************************************************/

static SELECTED_BUTTON prv_eSelectedButton = BUTTON_NONE;
static FILE_INFO prv_xSelectedFileInfo = {0};



/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 *******************************************************************************/

static void prv_vToggleFileLock(SELECTED_BUTTON eSelectedButton);
static unsigned char prv_ucGetFileLockState(SELECTED_BUTTON eSelectedButton);
static void prv_vUpdateFileInfo(SELECTED_BUTTON eSelectedButton);
static void prv_vSelectButton(SELECTED_BUTTON eNewSelectedButton);
static void prv_vCreateMainPage(void);
static void prv_vCreateEditLockPage(void);
static void prv_vUpdateEditLockIcon(SELECTED_BUTTON eSelectedButton);
static void prv_vEnableGamepadButton(void);
static void prv_vDisableGamepadButton(void);
static void prv_vCreateMainPageButton(void);
static void prv_vRemoveMainPageButton(void);
static void prv_vCreateTimeFrame(void);
static void prv_vRemoveTimeFrame(void);



/*******************************************************************************
 * FUNCTION: prv_vToggleFileLock
 *
 * PARAMETERS:
 * ~ eSelectedButton    - The selected button.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Toggle the read-only flag of the selected file.
 *
 *******************************************************************************/
static void prv_vToggleFileLock(SELECTED_BUTTON eSelectedButton)
{
    // Get the file information.
    prv_vUpdateFileInfo(eSelectedButton);
    
    // Toggle the read only flag.
    xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
    if (xFSSetReadOnlyFlag(prv_xSelectedFileInfo.szFullFilePath, !prv_xSelectedFileInfo.ucLock) == FR_OK) {
        prv_xSelectedFileInfo.ucLock ^= 1;
        
        // Clear the error message.
        vUpdateMotionPageMsg1("");
        vUpdateMotionPageMsg2("");
    }
    else {
        // Show the error message if failed to toggle the flag.
        vUpdateMotionPageMsg1("Error:");
        vUpdateMotionPageMsg2("Empty file");
    }
    xSemaphoreGive(xSdCardMutex);
}



/*******************************************************************************
 * FUNCTION: prv_ucGetFileLockState
 *
 * PARAMETERS:
 * ~ eSelectedButton    - The selected button.
 *
 * RETURN:
 * ~ The state of the file.
 * ~ 0 = Unlocked.
 * ~ 1 = Locked.
 *
 * DESCRIPTIONS:
 * Get the lock/unlock state of a file.
 * Planner file has the highest priority.
 *
 *******************************************************************************/
static unsigned char prv_ucGetFileLockState(SELECTED_BUTTON eSelectedButton)
{
    char szFullFilePath[MAX_FILENAME_LENGTH * 3];
    char *szFileName;
    unsigned char ucLock = 0;
    
    switch (eSelectedButton) {
        case BUTTON_UP:     szFileName = "XU";      break;
        case BUTTON_DOWN:   szFileName = "XD";      break;
        case BUTTON_LEFT:   szFileName = "XL";      break;
        case BUTTON_RIGHT:  szFileName = "XR";      break;
        
        case BUTTON_X:      szFileName = "CL";      break;
        case BUTTON_Y:      szFileName = "CU";      break;
        case BUTTON_A:      szFileName = "CD";      break;
        case BUTTON_B:      szFileName = "CR";      break;
        
        case BUTTON_1:      szFileName = "File1";   break;
        case BUTTON_2:      szFileName = "File2";   break;
        case BUTTON_3:      szFileName = "File3";   break;
        case BUTTON_4:      szFileName = "File4";   break;
        case BUTTON_5:      szFileName = "File5";   break;
        
        default:            szFileName = "";        break;
    }
    
    xSemaphoreTake(xSdCardMutex, portMAX_DELAY);

    // Get the full file path and add the extension for planner file.
    strcpy(szFullFilePath, szProgramFolder);
    strcat(szFullFilePath, "/");
    strcat(szFullFilePath, szFileName);
    strcat(szFullFilePath, szPlannerFileExt);
    
    // Try to read the file attributes and check whether the file is locked.
    if (xFSGetReadOnlyFlag(szFullFilePath, &ucLock) != FR_OK) {
        // Failed to get the attributes for planner file.
        // Try again with motion file.
        
        // Get the full file path and add the extension for motion file.
        strcpy(szFullFilePath, szProgramFolder);
        strcat(szFullFilePath, "/");
        strcat(szFullFilePath, szFileName);
        strcat(szFullFilePath, szMotionFileExt);
        
        // Try to read the file attributes and check whether the file is locked.
        if (xFSGetReadOnlyFlag(szFullFilePath, &ucLock) != FR_OK) {
            // Failed again. This means the file does not exist.
            // Clear the lock bit.
            ucLock = 0;
        }
    }
    xSemaphoreGive(xSdCardMutex);
    
    return ucLock;
}



/*******************************************************************************
 * FUNCTION: prv_vUpdateFileInfo
 *
 * PARAMETERS:
 * ~ eSelectedButton    - The selected button.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Update the file type and file comment for the selected file.
 * Planner file has the highest priority.
 *
 *******************************************************************************/
static void prv_vUpdateFileInfo(SELECTED_BUTTON eSelectedButton)
{
    const unsigned short PLANNER_FILE_COMMENT_OFFSET = 1;
    const unsigned short MOTION_FILE_COMMENT_OFFSET = 2;
    
    switch (eSelectedButton) {
        case BUTTON_UP:     prv_xSelectedFileInfo.szFileName = "XU";        break;
        case BUTTON_DOWN:   prv_xSelectedFileInfo.szFileName = "XD";        break;
        case BUTTON_LEFT:   prv_xSelectedFileInfo.szFileName = "XL";        break;
        case BUTTON_RIGHT:  prv_xSelectedFileInfo.szFileName = "XR";        break;
        
        case BUTTON_X:      prv_xSelectedFileInfo.szFileName = "CL";        break;
        case BUTTON_Y:      prv_xSelectedFileInfo.szFileName = "CU";        break;
        case BUTTON_A:      prv_xSelectedFileInfo.szFileName = "CD";        break;
        case BUTTON_B:      prv_xSelectedFileInfo.szFileName = "CR";        break;
        
        case BUTTON_1:      prv_xSelectedFileInfo.szFileName = "File1";     break;
        case BUTTON_2:      prv_xSelectedFileInfo.szFileName = "File2";     break;
        case BUTTON_3:      prv_xSelectedFileInfo.szFileName = "File3";     break;
        case BUTTON_4:      prv_xSelectedFileInfo.szFileName = "File4";     break;
        case BUTTON_5:      prv_xSelectedFileInfo.szFileName = "File5";     break;
        
        default:            prv_xSelectedFileInfo.szFileName = "";          break;
    }
    
    

    xSemaphoreTake(xSdCardMutex, portMAX_DELAY);


    // Get the full file path and add the extension for planner file.
    strcpy(prv_xSelectedFileInfo.szFullFilePath, szProgramFolder);
    strcat(prv_xSelectedFileInfo.szFullFilePath, "/");
    strcat(prv_xSelectedFileInfo.szFullFilePath, prv_xSelectedFileInfo.szFileName);
    strcat(prv_xSelectedFileInfo.szFullFilePath, szPlannerFileExt);

    // Open the planner file.
    FSFILE *pxFile = FSfopen(prv_xSelectedFileInfo.szFullFilePath, "r");

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
 * FUNCTION: prv_vSelectButton
 *
 * PARAMETERS:
 * ~ eNewSelectedButton     - The newly selected button.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Update the button state for selected button and get the selected file
 * information.
 *
 *******************************************************************************/
static void prv_vSelectButton(SELECTED_BUTTON eNewSelectedButton)
{
    // Only update the button if the selected button has changed.
    if (eNewSelectedButton != prv_eSelectedButton) {
        // Update the button state for left gamepad.
        switch (eNewSelectedButton) {
            case BUTTON_UP:     vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_LEFTPAD), "/Theme/MotionPage/LeftPad-Up.bmp");     break;
            case BUTTON_DOWN:   vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_LEFTPAD), "/Theme/MotionPage/LeftPad-Down.bmp");   break;
            case BUTTON_LEFT:   vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_LEFTPAD), "/Theme/MotionPage/LeftPad-Left.bmp");   break;
            case BUTTON_RIGHT:  vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_LEFTPAD), "/Theme/MotionPage/LeftPad-Right.bmp");  break;
            
            default:
                // Only clear the gamepad if one of the button was selected previously.
                switch (prv_eSelectedButton) {
                    case BUTTON_UP:
                    case BUTTON_DOWN:
                    case BUTTON_LEFT:
                    case BUTTON_RIGHT:
                        vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_LEFTPAD), "/Theme/MotionPage/LeftPad-None.bmp");
                        break;
                }
                break;
        }

        // Update the button state for right gamepad.
        switch (eNewSelectedButton) {
            case BUTTON_X:  vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_RIGHTPAD), "/Theme/MotionPage/RightPad-X.bmp");    break;
            case BUTTON_Y:  vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_RIGHTPAD), "/Theme/MotionPage/RightPad-Y.bmp");    break;
            case BUTTON_A:  vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_RIGHTPAD), "/Theme/MotionPage/RightPad-A.bmp");    break;
            case BUTTON_B:  vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_RIGHTPAD), "/Theme/MotionPage/RightPad-B.bmp");    break;
            
            default:
                // Only clear the gamepad if one of the button was selected previously.
                switch (prv_eSelectedButton) {
                    case BUTTON_X:
                    case BUTTON_Y:
                    case BUTTON_A:
                    case BUTTON_B:
                        vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_RIGHTPAD), "/Theme/MotionPage/RightPad-None.bmp");
                        break;
                }
                break;
        }

        // Highlight the selected button for bottom button row.
        switch (eNewSelectedButton) {
            case BUTTON_1:  vButtonUpdateBitmap((BUTTON*)GOLFindObject(GID_MOTION_BTN_1), "/Theme/MotionPage/Button1-Pressed.bmp");    break;
            case BUTTON_2:  vButtonUpdateBitmap((BUTTON*)GOLFindObject(GID_MOTION_BTN_2), "/Theme/MotionPage/Button2-Pressed.bmp");    break;
            case BUTTON_3:  vButtonUpdateBitmap((BUTTON*)GOLFindObject(GID_MOTION_BTN_3), "/Theme/MotionPage/Button3-Pressed.bmp");    break;
            case BUTTON_4:  vButtonUpdateBitmap((BUTTON*)GOLFindObject(GID_MOTION_BTN_4), "/Theme/MotionPage/Button4-Pressed.bmp");    break;
            case BUTTON_5:  vButtonUpdateBitmap((BUTTON*)GOLFindObject(GID_MOTION_BTN_5), "/Theme/MotionPage/Button5-Pressed.bmp");    break;
        }

        // Clear the deselected button for bottom button row.
        if (prv_eSelectedButton == BUTTON_1) vButtonUpdateBitmap((BUTTON*)GOLFindObject(GID_MOTION_BTN_1), "/Theme/MotionPage/Button1-Released.bmp");
        if (prv_eSelectedButton == BUTTON_2) vButtonUpdateBitmap((BUTTON*)GOLFindObject(GID_MOTION_BTN_2), "/Theme/MotionPage/Button2-Released.bmp");
        if (prv_eSelectedButton == BUTTON_3) vButtonUpdateBitmap((BUTTON*)GOLFindObject(GID_MOTION_BTN_3), "/Theme/MotionPage/Button3-Released.bmp");
        if (prv_eSelectedButton == BUTTON_4) vButtonUpdateBitmap((BUTTON*)GOLFindObject(GID_MOTION_BTN_4), "/Theme/MotionPage/Button4-Released.bmp");
        if (prv_eSelectedButton == BUTTON_5) vButtonUpdateBitmap((BUTTON*)GOLFindObject(GID_MOTION_BTN_5), "/Theme/MotionPage/Button5-Released.bmp");
        
    }
    
    
    
    // Update the file information.
    prv_vUpdateFileInfo(eNewSelectedButton);


    // If the file is not available.
    if (prv_xSelectedFileInfo.eFileType == NOT_AVAILABLE) {
        // Disable the play button.
        vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_PLAY), "/Theme/MotionPage/Play-Disabled.bmp");
        vGraphicsObjectDisable((BUTTON*)GOLFindObject(GID_MOTION_BTN_PLAY));

        // Update the file type and comment.
        if (eNewSelectedButton == BUTTON_NONE) {
            vUpdateMotionPageMsg1("");
        } else{
            vUpdateMotionPageMsg1("Empty File");
        }
        vUpdateMotionPageMsg2("");
    }

    // File is available.
    else {
        // Enable the play button.
        vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_PLAY), "/Theme/MotionPage/Play-Released.bmp");
        vGraphicsObjectEnable((BUTTON*)GOLFindObject(GID_MOTION_BTN_PLAY));

        // Update the file type and comment.
        if (prv_xSelectedFileInfo.eFileType == PLANNER_FILE) {
            vUpdateMotionPageMsg1("Planner File:");
        } else {
            vUpdateMotionPageMsg1("Motion File:");
        }
        vUpdateMotionPageMsg2(prv_xSelectedFileInfo.szFileComment);
    }
    
    
// Lock all files except File 1 if this is for showcase.
#ifdef SHOWCASE_ROBOT
    if (eNewSelectedButton == BUTTON_1) {
        vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_TEACH), "/Theme/MotionPage/Teach-Released.bmp");
        vGraphicsObjectEnable((BUTTON*)GOLFindObject(GID_MOTION_BTN_TEACH));
        
        vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_LOCK), "/Theme/MotionPage/Unlocked.bmp");
        
    } else {
        vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_TEACH), "/Theme/MotionPage/Teach-Disabled.bmp");
        vGraphicsObjectDisable((BUTTON*)GOLFindObject(GID_MOTION_BTN_TEACH));
        
        vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_LOCK), "/Theme/MotionPage/Locked.bmp");
    }
#else
    // If the file is locked, disable the teach button and show the locked icon.
    if (prv_xSelectedFileInfo.ucLock != 0) {
        vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_TEACH), "/Theme/MotionPage/Teach-Disabled.bmp");
        vGraphicsObjectDisable((BUTTON*)GOLFindObject(GID_MOTION_BTN_TEACH));
        
        vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_LOCK), "/Theme/MotionPage/Locked.bmp");
    }
    // Else, enable the teach button and show the unlocked icon.
    else {
        if (eNewSelectedButton == BUTTON_NONE) {
            vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_TEACH), "/Theme/MotionPage/Teach-Disabled.bmp");
            vGraphicsObjectDisable((BUTTON*)GOLFindObject(GID_MOTION_BTN_TEACH));
        } else {
            vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_TEACH), "/Theme/MotionPage/Teach-Released.bmp");
            vGraphicsObjectEnable((BUTTON*)GOLFindObject(GID_MOTION_BTN_TEACH));
        }
        vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_LOCK), "/Theme/MotionPage/Unlocked.bmp");
    }
#endif
    
    
    
    // Save the selected button.
    prv_eSelectedButton = eNewSelectedButton;
}



/*******************************************************************************
 * FUNCTION: prv_vCreateMainPage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the main page for motion page.
 *
 *******************************************************************************/
static void prv_vCreateMainPage(void)
{
    vClearWindowSubpage();
    
    // Left gamepad image.
    PictCreate( GID_MOTION_IMG_LEFTPAD,
                IMG_GAMEPAD_LEFT_L, IMG_GAMEPAD_LEFT_T,
                IMG_GAMEPAD_LEFT_R, IMG_GAMEPAD_LEFT_B,
                PICT_DRAW, 1, "/Theme/MotionPage/LeftPad-None.bmp", pxDefaultScheme );
    
    // Up button.
    BtnCreate( GID_MOTION_BTN_UP,
               BTN_UP_L, BTN_UP_T,
               BTN_UP_R, BTN_UP_B,
               0, BTN_DRAW | BTN_NOPANEL, NULL, NULL, pxImgBtnScheme );
    
    // Down button.
    BtnCreate( GID_MOTION_BTN_DOWN,
               BTN_DOWN_L, BTN_DOWN_T,
               BTN_DOWN_R, BTN_DOWN_B,
               0, BTN_DRAW | BTN_NOPANEL, NULL, NULL, pxImgBtnScheme );
    
    // Left button.
    BtnCreate( GID_MOTION_BTN_LEFT,
               BTN_LEFT_L, BTN_LEFT_T,
               BTN_LEFT_R, BTN_LEFT_B,
               0, BTN_DRAW | BTN_NOPANEL, NULL, NULL, pxImgBtnScheme );
    
    // Right button.
    BtnCreate( GID_MOTION_BTN_RIGHT,
               BTN_RIGHT_L, BTN_RIGHT_T,
               BTN_RIGHT_R, BTN_RIGHT_B,
               0, BTN_DRAW | BTN_NOPANEL, NULL, NULL, pxImgBtnScheme );
    
    
    
    // Right gamepad image.
    PictCreate( GID_MOTION_IMG_RIGHTPAD,
                IMG_GAMEPAD_RIGHT_L, IMG_GAMEPAD_RIGHT_T,
                IMG_GAMEPAD_RIGHT_R, IMG_GAMEPAD_RIGHT_B,
                PICT_DRAW, 1, "/Theme/MotionPage/RightPad-None.bmp", pxDefaultScheme );
    
    // X button.
    BtnCreate( GID_MOTION_BTN_X,
               BTN_X_L, BTN_X_T,
               BTN_X_R, BTN_X_B,
               0, BTN_DRAW | BTN_NOPANEL, NULL, NULL, pxImgBtnScheme );
    
    // Y button.
    BtnCreate( GID_MOTION_BTN_Y,
               BTN_Y_L, BTN_Y_T,
               BTN_Y_R, BTN_Y_B,
               0, BTN_DRAW | BTN_NOPANEL, NULL, NULL, pxImgBtnScheme );
    
    // A button.
    BtnCreate( GID_MOTION_BTN_A,
               BTN_A_L, BTN_A_T,
               BTN_A_R, BTN_A_B,
               0, BTN_DRAW | BTN_NOPANEL, NULL, NULL, pxImgBtnScheme );
    
    // B button.
    BtnCreate( GID_MOTION_BTN_B,
               BTN_B_L, BTN_B_T,
               BTN_B_R, BTN_B_B,
               0, BTN_DRAW | BTN_NOPANEL, NULL, NULL, pxImgBtnScheme );
    
    
    
    // Bottom button 1.
    BtnCreate( GID_MOTION_BTN_1,
               BTN_1_L, BTN_1_T,
               BTN_1_R, BTN_1_B,
               0, BTN_DRAW | BTN_NOPANEL, "/Theme/MotionPage/Button1-Released.bmp", NULL, pxImgBtnScheme );
    
    // Bottom button 2.
    BtnCreate( GID_MOTION_BTN_2,
               BTN_2_L, BTN_2_T,
               BTN_2_R, BTN_2_B,
               0, BTN_DRAW | BTN_NOPANEL, "/Theme/MotionPage/Button2-Released.bmp", NULL, pxImgBtnScheme );
    
    // Bottom button 3.
    BtnCreate( GID_MOTION_BTN_3,
               BTN_3_L, BTN_3_T,
               BTN_3_R, BTN_3_B,
               0, BTN_DRAW | BTN_NOPANEL, "/Theme/MotionPage/Button3-Released.bmp", NULL, pxImgBtnScheme );
    
    // Bottom button 4.
    BtnCreate( GID_MOTION_BTN_4,
               BTN_4_L, BTN_4_T,
               BTN_4_R, BTN_4_B,
               0, BTN_DRAW | BTN_NOPANEL, "/Theme/MotionPage/Button4-Released.bmp", NULL, pxImgBtnScheme );
    
    // Bottom button 5.
    BtnCreate( GID_MOTION_BTN_5,
               BTN_5_L, BTN_5_T,
               BTN_5_R, BTN_5_B,
               0, BTN_DRAW | BTN_NOPANEL, "/Theme/MotionPage/Button5-Released.bmp", NULL, pxImgBtnScheme );
    
    
    
    // Message 1.
    StCreate( GID_MOTION_TXT_MSG1,
              TXT_MSG1_L, TXT_MSG1_T,
              TXT_MSG1_R, TXT_MSG1_B,
              ST_DRAW, "", pxDefaultScheme );
    
    // Message 2.
    StCreate( GID_MOTION_TXT_MSG2,
              TXT_MSG2_L, TXT_MSG2_T,
              TXT_MSG2_R, TXT_MSG2_B,
              ST_DRAW, "", pxLightBlueTxtScheme );
    
    
    
    // Create the teach, play and edit button.
    prv_vCreateMainPageButton();
    
    
    
    // Clear the selected button.
    prv_vSelectButton(BUTTON_NONE);
    
    
    
#ifdef SHOWCASE_ROBOT
    // Play File 5 automatically when power up.
    // Select File 5.
    usMsgMotionPage(BTN_MSG_PRESSED, GOLFindObject(GID_MOTION_BTN_5), NULL);
    usMsgMotionPage(BTN_MSG_RELEASED, GOLFindObject(GID_MOTION_BTN_5), NULL);
    
    // Press the play button.
    usMsgMotionPage(BTN_MSG_PRESSED, GOLFindObject(GID_MOTION_BTN_PLAY), NULL);
    usMsgMotionPage(BTN_MSG_RELEASED, GOLFindObject(GID_MOTION_BTN_PLAY), NULL);
#endif
}



/*******************************************************************************
 * FUNCTION: prv_vCreateEditLockPage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the edit lock page for motion page.
 *
 *******************************************************************************/
static void prv_vCreateEditLockPage(void)
{
    vClearWindowSubpage();
    
    // Left gamepad image.
    PictCreate( GID_MOTION_IMG_LEFTPAD,
                IMG_GAMEPAD_LEFT_L, IMG_GAMEPAD_LEFT_T,
                IMG_GAMEPAD_LEFT_R, IMG_GAMEPAD_LEFT_B,
                PICT_DRAW, 1, "/Theme/MotionPage/PadOutline.bmp", pxDefaultScheme );
    
    // Up button.
    PictCreate( GID_MOTION_IMG_LOCK_UP,
                IMG_LOCK_UP_L, IMG_LOCK_UP_T,
                IMG_LOCK_UP_R, IMG_LOCK_UP_B,
                PICT_DRAW, 1, NULL, pxDefaultScheme );
    
    BtnCreate( GID_MOTION_BTN_LOCK_UP,
               BTN_UP_L, BTN_UP_T,
               BTN_UP_R, BTN_UP_B,
               0, BTN_DRAW | BTN_NOPANEL, NULL, NULL, pxImgBtnScheme );
    
    // Down button.
    PictCreate( GID_MOTION_IMG_LOCK_DOWN,
                IMG_LOCK_DOWN_L, IMG_LOCK_DOWN_T,
                IMG_LOCK_DOWN_R, IMG_LOCK_DOWN_B,
                PICT_DRAW, 1, NULL, pxDefaultScheme );
    
    BtnCreate( GID_MOTION_BTN_LOCK_DOWN,
               BTN_DOWN_L, BTN_DOWN_T,
               BTN_DOWN_R, BTN_DOWN_B,
               0, BTN_DRAW | BTN_NOPANEL, NULL, NULL, pxImgBtnScheme );
    
    // Left button.
    PictCreate( GID_MOTION_IMG_LOCK_LEFT,
                IMG_LOCK_LEFT_L, IMG_LOCK_LEFT_T,
                IMG_LOCK_LEFT_R, IMG_LOCK_LEFT_B,
                PICT_DRAW, 1, NULL, pxDefaultScheme );
    
    BtnCreate( GID_MOTION_BTN_LOCK_LEFT,
               BTN_LEFT_L, BTN_LEFT_T,
               BTN_LEFT_R, BTN_LEFT_B,
               0, BTN_DRAW | BTN_NOPANEL, NULL, NULL, pxImgBtnScheme );
    
    // Right button.
    PictCreate( GID_MOTION_IMG_LOCK_RIGHT,
                IMG_LOCK_RIGHT_L, IMG_LOCK_RIGHT_T,
                IMG_LOCK_RIGHT_R, IMG_LOCK_RIGHT_B,
                PICT_DRAW, 1, NULL, pxDefaultScheme );
    
    BtnCreate( GID_MOTION_BTN_LOCK_RIGHT,
               BTN_RIGHT_L, BTN_RIGHT_T,
               BTN_RIGHT_R, BTN_RIGHT_B,
               0, BTN_DRAW | BTN_NOPANEL, NULL, NULL, pxImgBtnScheme );
    
    
    
    // Right gamepad image.
    PictCreate( GID_MOTION_IMG_RIGHTPAD,
                IMG_GAMEPAD_RIGHT_L, IMG_GAMEPAD_RIGHT_T,
                IMG_GAMEPAD_RIGHT_R, IMG_GAMEPAD_RIGHT_B,
                PICT_DRAW, 1, "/Theme/MotionPage/PadOutline.bmp", pxDefaultScheme );
    
    // X button.
    PictCreate( GID_MOTION_IMG_LOCK_X,
                IMG_LOCK_X_L, IMG_LOCK_X_T,
                IMG_LOCK_X_R, IMG_LOCK_X_B,
                PICT_DRAW, 1, NULL, pxDefaultScheme );
    
    BtnCreate( GID_MOTION_BTN_LOCK_X,
               BTN_X_L, BTN_X_T,
               BTN_X_R, BTN_X_B,
               0, BTN_DRAW | BTN_NOPANEL, NULL, NULL, pxImgBtnScheme );
    
    // Y button.
    PictCreate( GID_MOTION_IMG_LOCK_Y,
                IMG_LOCK_Y_L, IMG_LOCK_Y_T,
                IMG_LOCK_Y_R, IMG_LOCK_Y_B,
                PICT_DRAW, 1, NULL, pxDefaultScheme );
    
    BtnCreate( GID_MOTION_BTN_LOCK_Y,
               BTN_Y_L, BTN_Y_T,
               BTN_Y_R, BTN_Y_B,
               0, BTN_DRAW | BTN_NOPANEL, NULL, NULL, pxImgBtnScheme );
    
    // A button.
    PictCreate( GID_MOTION_IMG_LOCK_A,
                IMG_LOCK_A_L, IMG_LOCK_A_T,
                IMG_LOCK_A_R, IMG_LOCK_A_B,
                PICT_DRAW, 1, NULL, pxDefaultScheme );
    
    BtnCreate( GID_MOTION_BTN_LOCK_A,
               BTN_A_L, BTN_A_T,
               BTN_A_R, BTN_A_B,
               0, BTN_DRAW | BTN_NOPANEL, NULL, NULL, pxImgBtnScheme );
    
    // B button.
    PictCreate( GID_MOTION_IMG_LOCK_B,
                IMG_LOCK_B_L, IMG_LOCK_B_T,
                IMG_LOCK_B_R, IMG_LOCK_B_B,
                PICT_DRAW, 1, NULL, pxDefaultScheme );
    
    BtnCreate( GID_MOTION_BTN_LOCK_B,
               BTN_B_L, BTN_B_T,
               BTN_B_R, BTN_B_B,
               0, BTN_DRAW | BTN_NOPANEL, NULL, NULL, pxImgBtnScheme );
    
    
    
    // Bottom button 1.
    BtnCreate( GID_MOTION_BTN_LOCK_1,
               BTN_1_L, BTN_1_T,
               BTN_1_R, BTN_1_B,
               0, BTN_DRAW | BTN_NOPANEL, NULL, NULL, pxImgBtnScheme );
    
    // Bottom button 2.
    BtnCreate( GID_MOTION_BTN_LOCK_2,
               BTN_2_L, BTN_2_T,
               BTN_2_R, BTN_2_B,
               0, BTN_DRAW | BTN_NOPANEL, NULL, NULL, pxImgBtnScheme );
    
    // Bottom button 3.
    BtnCreate( GID_MOTION_BTN_LOCK_3,
               BTN_3_L, BTN_3_T,
               BTN_3_R, BTN_3_B,
               0, BTN_DRAW | BTN_NOPANEL, NULL, NULL, pxImgBtnScheme );
    
    // Bottom button 4.
    BtnCreate( GID_MOTION_BTN_LOCK_4,
               BTN_4_L, BTN_4_T,
               BTN_4_R, BTN_4_B,
               0, BTN_DRAW | BTN_NOPANEL, NULL, NULL, pxImgBtnScheme );
    
    // Bottom button 5.
    BtnCreate( GID_MOTION_BTN_LOCK_5,
               BTN_5_L, BTN_5_T,
               BTN_5_R, BTN_5_B,
               0, BTN_DRAW | BTN_NOPANEL, NULL, NULL, pxImgBtnScheme );
    
    
    
    // Message 1.
    StCreate( GID_MOTION_TXT_MSG1,
              TXT_MSG1_L, TXT_MSG1_T,
              TXT_MSG1_R, TXT_MSG1_B,
              ST_DRAW, "", pxDefaultScheme );
    
    // Message 2.
    StCreate( GID_MOTION_TXT_MSG2,
              TXT_MSG2_L, TXT_MSG2_T,
              TXT_MSG2_R, TXT_MSG2_B,
              ST_DRAW, "", pxLightBlueTxtScheme );
    
    
    
    // Done button.
    BtnCreate( GID_MOTION_BTN_DONE,
               BTN_DONE_L, BTN_DONE_T,
               BTN_DONE_R, BTN_DONE_B,
               1, BTN_DRAW, NULL, "Done", pxTransBtnScheme );
    
    
    
    // Update all the icon.
    prv_vUpdateEditLockIcon(BUTTON_UP);
    prv_vUpdateEditLockIcon(BUTTON_DOWN);
    prv_vUpdateEditLockIcon(BUTTON_LEFT);
    prv_vUpdateEditLockIcon(BUTTON_RIGHT);
    
    prv_vUpdateEditLockIcon(BUTTON_X);
    prv_vUpdateEditLockIcon(BUTTON_Y);
    prv_vUpdateEditLockIcon(BUTTON_A);
    prv_vUpdateEditLockIcon(BUTTON_B);
    
    prv_vUpdateEditLockIcon(BUTTON_1);
    prv_vUpdateEditLockIcon(BUTTON_2);
    prv_vUpdateEditLockIcon(BUTTON_3);
    prv_vUpdateEditLockIcon(BUTTON_4);
    prv_vUpdateEditLockIcon(BUTTON_5);
}



/*******************************************************************************
 * FUNCTION: prv_vUpdateEditLockIcon
 *
 * PARAMETERS:
 * ~ eSelectedButton    - The selected button.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Update the icon in the edit lock page based on the read-only flag.
 *
 *******************************************************************************/
static void prv_vUpdateEditLockIcon(SELECTED_BUTTON eSelectedButton)
{
    // Get the object ID for the icon to update.
    GOB_ID eObjectId;
    switch (eSelectedButton) {
        case BUTTON_UP:     eObjectId = GID_MOTION_IMG_LOCK_UP;     break;
        case BUTTON_DOWN:   eObjectId = GID_MOTION_IMG_LOCK_DOWN;   break;
        case BUTTON_LEFT:   eObjectId = GID_MOTION_IMG_LOCK_LEFT;   break;
        case BUTTON_RIGHT:  eObjectId = GID_MOTION_IMG_LOCK_RIGHT;  break;
        
        case BUTTON_X:      eObjectId = GID_MOTION_IMG_LOCK_X;      break;
        case BUTTON_Y:      eObjectId = GID_MOTION_IMG_LOCK_Y;      break;
        case BUTTON_A:      eObjectId = GID_MOTION_IMG_LOCK_A;      break;
        case BUTTON_B:      eObjectId = GID_MOTION_IMG_LOCK_B;      break;
        
        case BUTTON_1:      eObjectId = GID_MOTION_BTN_LOCK_1;      break;
        case BUTTON_2:      eObjectId = GID_MOTION_BTN_LOCK_2;      break;
        case BUTTON_3:      eObjectId = GID_MOTION_BTN_LOCK_3;      break;
        case BUTTON_4:      eObjectId = GID_MOTION_BTN_LOCK_4;      break;
        case BUTTON_5:      eObjectId = GID_MOTION_BTN_LOCK_5;      break;
    }
    
    
    
    // Get the lock state.
    unsigned char ucLock = prv_ucGetFileLockState(eSelectedButton);
    
    // Update the icon based on the read-only flag of the file.
    switch (eSelectedButton) {
        case BUTTON_UP:
        case BUTTON_DOWN:
        case BUTTON_LEFT:
        case BUTTON_RIGHT:
        case BUTTON_X:
        case BUTTON_Y:
        case BUTTON_A:
        case BUTTON_B:
            if (ucLock == 0) {
                vPictureUpdateBitmap((PICTURE*)GOLFindObject(eObjectId), "/Theme/MotionPage/Unlocked.bmp");
            } else {
                vPictureUpdateBitmap((PICTURE*)GOLFindObject(eObjectId), "/Theme/MotionPage/Locked.bmp");
            }
            break;
            
        case BUTTON_1:
        case BUTTON_2:
        case BUTTON_3:
        case BUTTON_4:
        case BUTTON_5:
            if (ucLock == 0) {
                vButtonUpdateBitmap((BUTTON*)GOLFindObject(eObjectId), "/Theme/MotionPage/ButtonUnlocked.bmp");
            } else {
                vButtonUpdateBitmap((BUTTON*)GOLFindObject(eObjectId), "/Theme/MotionPage/ButtonLocked.bmp");
            }
            break;
    }
}



/*******************************************************************************
 * FUNCTION: prv_vEnableGamepadButton
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Enable the gamepad button.
 *
 *******************************************************************************/
static void prv_vEnableGamepadButton(void)
{
    vGraphicsObjectEnable(GOLFindObject(GID_MOTION_BTN_UP));
    vGraphicsObjectEnable(GOLFindObject(GID_MOTION_BTN_DOWN));
    vGraphicsObjectEnable(GOLFindObject(GID_MOTION_BTN_LEFT));
    vGraphicsObjectEnable(GOLFindObject(GID_MOTION_BTN_RIGHT));

    vGraphicsObjectEnable(GOLFindObject(GID_MOTION_BTN_X));
    vGraphicsObjectEnable(GOLFindObject(GID_MOTION_BTN_Y));
    vGraphicsObjectEnable(GOLFindObject(GID_MOTION_BTN_A));
    vGraphicsObjectEnable(GOLFindObject(GID_MOTION_BTN_B));

    vGraphicsObjectEnable(GOLFindObject(GID_MOTION_BTN_1));
    vGraphicsObjectEnable(GOLFindObject(GID_MOTION_BTN_2));
    vGraphicsObjectEnable(GOLFindObject(GID_MOTION_BTN_3));
    vGraphicsObjectEnable(GOLFindObject(GID_MOTION_BTN_4));
    vGraphicsObjectEnable(GOLFindObject(GID_MOTION_BTN_5));
}



/*******************************************************************************
 * FUNCTION: prv_vDisableGamepadButton
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Disable the gamepad button.
 *
 *******************************************************************************/
static void prv_vDisableGamepadButton(void)
{
    vGraphicsObjectDisable(GOLFindObject(GID_MOTION_BTN_UP));
    vGraphicsObjectDisable(GOLFindObject(GID_MOTION_BTN_DOWN));
    vGraphicsObjectDisable(GOLFindObject(GID_MOTION_BTN_LEFT));
    vGraphicsObjectDisable(GOLFindObject(GID_MOTION_BTN_RIGHT));

    vGraphicsObjectDisable(GOLFindObject(GID_MOTION_BTN_X));
    vGraphicsObjectDisable(GOLFindObject(GID_MOTION_BTN_Y));
    vGraphicsObjectDisable(GOLFindObject(GID_MOTION_BTN_A));
    vGraphicsObjectDisable(GOLFindObject(GID_MOTION_BTN_B));

    vGraphicsObjectDisable(GOLFindObject(GID_MOTION_BTN_1));
    vGraphicsObjectDisable(GOLFindObject(GID_MOTION_BTN_2));
    vGraphicsObjectDisable(GOLFindObject(GID_MOTION_BTN_3));
    vGraphicsObjectDisable(GOLFindObject(GID_MOTION_BTN_4));
    vGraphicsObjectDisable(GOLFindObject(GID_MOTION_BTN_5));
}



/*******************************************************************************
 * FUNCTION: prv_vCreateMainPageButton
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the teach, play and edit button in main page.
 *
 *******************************************************************************/
static void prv_vCreateMainPageButton(void)
{
    // Teach button.
    PictCreate( GID_MOTION_IMG_TEACH,
                IMG_TEACH_L, IMG_TEACH_T,
                IMG_TEACH_R, IMG_TEACH_B,
                PICT_DRAW, 1, "/Theme/MotionPage/Teach-Disabled.bmp", pxDefaultScheme );
    
    BtnCreate( GID_MOTION_BTN_TEACH,
               BTN_TEACH_L, BTN_TEACH_T,
               BTN_TEACH_R, BTN_TEACH_B,
               BTN_RADIUS, BTN_DRAW | BTN_NOPANEL | BTN_DISABLED, NULL, NULL, pxBtnScheme );
    
    // Play button.
    PictCreate( GID_MOTION_IMG_PLAY,
                IMG_PLAY_L, IMG_PLAY_T,
                IMG_PLAY_R, IMG_PLAY_B,
                PICT_DRAW, 1, "/Theme/MotionPage/Play-Disabled.bmp", pxDefaultScheme );
    
    BtnCreate( GID_MOTION_BTN_PLAY,
               BTN_PLAY_L, BTN_PLAY_T,
               BTN_PLAY_R, BTN_PLAY_B,
               BTN_RADIUS, BTN_DRAW | BTN_NOPANEL | BTN_DISABLED, NULL, NULL, pxBtnScheme );
               
    // Lock image.
    PictCreate( GID_MOTION_IMG_LOCK,
                IMG_LOCK_L, IMG_LOCK_T,
                IMG_LOCK_R, IMG_LOCK_B,
                PICT_DRAW, 1, "/Theme/MotionPage/Unlocked.bmp", pxDefaultScheme );
    
    // Edit button.
    BtnCreate( GID_MOTION_BTN_EDIT,
               BTN_EDIT_L, BTN_EDIT_T,
               BTN_EDIT_R, BTN_EDIT_B,
               1, BTN_DRAW, NULL, "Edit", pxTransBtnScheme );
    
// Disable the edit button if this is for showcase robot.
#ifdef SHOWCASE_ROBOT
    vGraphicsObjectDisable((BUTTON*)GOLFindObject(GID_MOTION_BTN_EDIT));
#endif
}



/*******************************************************************************
 * FUNCTION: prv_vRemoveMainPageButton
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Remove the teach, play and edit button in main page.
 *
 *******************************************************************************/
static void prv_vRemoveMainPageButton(void)
{
    GOLDeleteObjectByID(GID_MOTION_IMG_TEACH);
    GOLDeleteObjectByID(GID_MOTION_BTN_TEACH);
    
    GOLDeleteObjectByID(GID_MOTION_IMG_PLAY);
    GOLDeleteObjectByID(GID_MOTION_BTN_PLAY);
    
    GOLDeleteObjectByID(GID_MOTION_IMG_LOCK);
    GOLDeleteObjectByID(GID_MOTION_BTN_EDIT);

    SetColor(pxDefaultScheme->CommonBkColor);
    Bar(BTN_TEACH_L, BTN_TEACH_T, BTN_TEACH_R, BTN_TEACH_B);
    Bar(BTN_PLAY_L, BTN_PLAY_T, BTN_PLAY_R, BTN_PLAY_B);
    Bar(IMG_LOCK_L, IMG_LOCK_T, IMG_LOCK_R, IMG_LOCK_B);
    Bar(BTN_EDIT_L, BTN_EDIT_T, BTN_EDIT_R, BTN_EDIT_B);
}



/*******************************************************************************
 * FUNCTION: prv_vCreateTimeFrame
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the label and value for time and frame.
 *
 *******************************************************************************/
static void prv_vCreateTimeFrame(void)
{
    // Frame label.
    StCreate( GID_MOTION_TXT_FRAME_LABEL,
              TXT_FRAME_LABEL_L, TXT_FRAME_LABEL_T,
              TXT_FRAME_LABEL_R, TXT_FRAME_LABEL_B,
              ST_DRAW, "Frame:", pxDefaultScheme );
    
    // Frame value.
    DmCreate( GID_MOTION_DM_FRAME_VALUE,
              DM_FRAME_VALUE_L, DM_FRAME_VALUE_T,
              DM_FRAME_VALUE_R, DM_FRAME_VALUE_B,
              DM_DRAW, 0, 4, 0, pxLightGreenTxtScheme );



    // Time label.
    StCreate( GID_MOTION_TXT_TIME_LABEL,
              TXT_TIME_LABEL_L, TXT_TIME_LABEL_T,
              TXT_TIME_LABEL_R, TXT_TIME_LABEL_B,
              ST_DRAW, "Time:", pxDefaultScheme );

    // Time value.
    DmCreate( GID_MOTION_DM_TIME_VALUE,
              DM_TIME_VALUE_L, DM_TIME_VALUE_T,
              DM_TIME_VALUE_R, DM_TIME_VALUE_B,
              DM_DRAW, 0, 4, 0, pxLightGreenTxtScheme );
}



/*******************************************************************************
 * FUNCTION: prv_vRemoveTimeFrame
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Remove the label and value for time and frame.
 *
 *******************************************************************************/
static void prv_vRemoveTimeFrame(void)
{
    GOLDeleteObjectByID(GID_MOTION_TXT_FRAME_LABEL);
    GOLDeleteObjectByID(GID_MOTION_DM_FRAME_VALUE);
    GOLDeleteObjectByID(GID_MOTION_TXT_TIME_LABEL);
    GOLDeleteObjectByID(GID_MOTION_DM_TIME_VALUE);

    SetColor(pxDefaultScheme->CommonBkColor);
    Bar(TXT_FRAME_LABEL_L, TXT_FRAME_LABEL_T, TXT_FRAME_LABEL_R, TXT_FRAME_LABEL_B);
    Bar(DM_FRAME_VALUE_L, DM_FRAME_VALUE_T, DM_FRAME_VALUE_R, DM_FRAME_VALUE_B);
    Bar(TXT_TIME_LABEL_L, TXT_TIME_LABEL_T, TXT_TIME_LABEL_R, TXT_TIME_LABEL_B);
    Bar(DM_TIME_VALUE_L, DM_TIME_VALUE_T, DM_TIME_VALUE_R, DM_TIME_VALUE_B);
}



/*******************************************************************************
 * FUNCTION: vCreateMotionPage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the motion page.
 *
 *******************************************************************************/
void vCreateMotionPage(void)
{
    GOLFree();
    vCreatePageWindow("MOTION");
    
    prv_vCreateMainPage();
}



/*******************************************************************************
 * FUNCTION: usMsgMotionPage
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
WORD usMsgMotionPage(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    // Button is pressed.
    if (objMsg == BTN_MSG_PRESSED) {
        switch (GetObjID(pObj)) {
            case GID_MOTION_BTN_UP:     prv_vSelectButton(BUTTON_UP);       break;
            case GID_MOTION_BTN_DOWN:   prv_vSelectButton(BUTTON_DOWN);     break;
            case GID_MOTION_BTN_LEFT:   prv_vSelectButton(BUTTON_LEFT);     break;
            case GID_MOTION_BTN_RIGHT:  prv_vSelectButton(BUTTON_RIGHT);    break;
            
            case GID_MOTION_BTN_X:      prv_vSelectButton(BUTTON_X);        break;
            case GID_MOTION_BTN_Y:      prv_vSelectButton(BUTTON_Y);        break;
            case GID_MOTION_BTN_A:      prv_vSelectButton(BUTTON_A);        break;
            case GID_MOTION_BTN_B:      prv_vSelectButton(BUTTON_B);        break;
            
            case GID_MOTION_BTN_1:      prv_vSelectButton(BUTTON_1);        break;
            case GID_MOTION_BTN_2:      prv_vSelectButton(BUTTON_2);        break;
            case GID_MOTION_BTN_3:      prv_vSelectButton(BUTTON_3);        break;
            case GID_MOTION_BTN_4:      prv_vSelectButton(BUTTON_4);        break;
            case GID_MOTION_BTN_5:      prv_vSelectButton(BUTTON_5);        break;
            
            
            
            case GID_MOTION_BTN_LOCK_UP:    prv_vToggleFileLock(BUTTON_UP);     prv_vUpdateEditLockIcon(BUTTON_UP);     break;
            case GID_MOTION_BTN_LOCK_DOWN:  prv_vToggleFileLock(BUTTON_DOWN);   prv_vUpdateEditLockIcon(BUTTON_DOWN);   break;
            case GID_MOTION_BTN_LOCK_LEFT:  prv_vToggleFileLock(BUTTON_LEFT);   prv_vUpdateEditLockIcon(BUTTON_LEFT);   break;
            case GID_MOTION_BTN_LOCK_RIGHT: prv_vToggleFileLock(BUTTON_RIGHT);  prv_vUpdateEditLockIcon(BUTTON_RIGHT);  break;
            
            case GID_MOTION_BTN_LOCK_X:     prv_vToggleFileLock(BUTTON_X);      prv_vUpdateEditLockIcon(BUTTON_X);      break;
            case GID_MOTION_BTN_LOCK_Y:     prv_vToggleFileLock(BUTTON_Y);      prv_vUpdateEditLockIcon(BUTTON_Y);      break;
            case GID_MOTION_BTN_LOCK_A:     prv_vToggleFileLock(BUTTON_A);      prv_vUpdateEditLockIcon(BUTTON_A);      break;
            case GID_MOTION_BTN_LOCK_B:     prv_vToggleFileLock(BUTTON_B);      prv_vUpdateEditLockIcon(BUTTON_B);      break;
            
            case GID_MOTION_BTN_LOCK_1:     prv_vToggleFileLock(BUTTON_1);      prv_vUpdateEditLockIcon(BUTTON_1);      break;
            case GID_MOTION_BTN_LOCK_2:     prv_vToggleFileLock(BUTTON_2);      prv_vUpdateEditLockIcon(BUTTON_2);      break;
            case GID_MOTION_BTN_LOCK_3:     prv_vToggleFileLock(BUTTON_3);      prv_vUpdateEditLockIcon(BUTTON_3);      break;
            case GID_MOTION_BTN_LOCK_4:     prv_vToggleFileLock(BUTTON_4);      prv_vUpdateEditLockIcon(BUTTON_4);      break;
            case GID_MOTION_BTN_LOCK_5:     prv_vToggleFileLock(BUTTON_5);      prv_vUpdateEditLockIcon(BUTTON_5);      break;
            
            
            
            case GID_MOTION_BTN_TEACH:      vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_TEACH), "/Theme/MotionPage/Teach-Pressed.bmp");     break;
            case GID_MOTION_BTN_NEXT:       vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_NEXT), "/Theme/MotionPage/Next-Pressed.bmp");       break;
            case GID_MOTION_BTN_STOPTEACH:  vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_STOPTEACH), "/Theme/MotionPage/Stop-Pressed.bmp");  break;
            
            case GID_MOTION_BTN_PLAY:       vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_PLAY), "/Theme/MotionPage/Play-Pressed.bmp");       break;
            case GID_MOTION_BTN_STOPPLAY:   vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_STOPPLAY), "/Theme/MotionPage/Stop-Pressed.bmp");   break;
        }
    }
    
    // Button is released.
    else if (objMsg == BTN_MSG_RELEASED) {
        PLAY_RESULT ePlayResult;
        
        switch (GetObjID(pObj)) {
            // Teach button.
            case GID_MOTION_BTN_TEACH:
                // Creating the motion file and start the teaching process.
                vStartTeaching();
                
                // Show playing motion message.
                vUpdateMotionPageMsg1("Teaching . . .");
                vUpdateMotionPageMsg2("");

                // Disable the gamepad button.
                prv_vDisableGamepadButton();

                // Delete the teach, play, and edit button.
                prv_vRemoveMainPageButton();

                // Create the label and value for time and frame.
                prv_vCreateTimeFrame();
                
                // Create the next button.
                PictCreate( GID_MOTION_IMG_NEXT,
                            IMG_NEXT_L, IMG_NEXT_T,
                            IMG_NEXT_R, IMG_NEXT_B,
                            PICT_DRAW, 1, "/Theme/MotionPage/Next-Released.bmp", pxDefaultScheme );
                
                BtnCreate( GID_MOTION_BTN_NEXT,
                           BTN_NEXT_L, BTN_NEXT_T,
                           BTN_NEXT_R, BTN_NEXT_B,
                           BTN_RADIUS, BTN_DRAW | BTN_NOPANEL, NULL, NULL, pxBtnScheme );

                // Create the stop teaching button.
                PictCreate( GID_MOTION_IMG_STOPTEACH,
                            IMG_STOPTEACH_L, IMG_STOPTEACH_T,
                            IMG_STOPTEACH_R, IMG_STOPTEACH_B,
                            PICT_DRAW, 1, "/Theme/MotionPage/Stop-Released.bmp", pxDefaultScheme );
                
                BtnCreate( GID_MOTION_BTN_STOPTEACH,
                           BTN_STOPTEACH_L, BTN_STOPTEACH_T,
                           BTN_STOPTEACH_R, BTN_STOPTEACH_B,
                           BTN_RADIUS, BTN_DRAW | BTN_NOPANEL, NULL, NULL, pxBtnScheme );
                
                break;
                
            // Next button.
            case GID_MOTION_BTN_NEXT:
                // Update the button image.
                vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_NEXT), "/Theme/MotionPage/Next-Released.bmp");
                
                // Record the servo position.
                vTeachAddPosition(prv_xSelectedFileInfo.szFileName);
                break;
                
            // Stop teaching button.
            case GID_MOTION_BTN_STOPTEACH:
//                // Turn off all servo LED.
//                vTeachTurnOffServoLed();
                
                // Turn on Sub-PCB.
                vTeachTurnOnSubPcb();
                
                // Remove the next and stop button.
                GOLDeleteObjectByID(GID_MOTION_IMG_NEXT);
                GOLDeleteObjectByID(GID_MOTION_BTN_NEXT);
                
                GOLDeleteObjectByID(GID_MOTION_IMG_STOPTEACH);
                GOLDeleteObjectByID(GID_MOTION_BTN_STOPTEACH);
                
                SetColor(pxDefaultScheme->CommonBkColor);
                Bar(BTN_NEXT_L, BTN_NEXT_T, BTN_NEXT_R, BTN_NEXT_B);
                Bar(BTN_STOPTEACH_L, BTN_STOPTEACH_T, BTN_STOPTEACH_R, BTN_STOPTEACH_B);

                // Remove the label and value for time and frame.
                prv_vRemoveTimeFrame();

                // Create the teach, play, and edit button.
                prv_vCreateMainPageButton();

                // Enable the gamepad button.
                prv_vEnableGamepadButton();

                // Update the state of the buttons and file information.
                prv_vSelectButton(prv_eSelectedButton);
                break;
                
            // Play button.
            case GID_MOTION_BTN_PLAY:
                // Show playing motion message.
                vUpdateMotionPageMsg1("Running . . .");

                // Disable the gamepad button.
                prv_vDisableGamepadButton();

                // Delete the teach, play, and edit button.
                prv_vRemoveMainPageButton();

                // Create the label and value for time and frame.
                prv_vCreateTimeFrame();

                // Create the stop playing button.
                PictCreate( GID_MOTION_IMG_STOPPLAY,
                            IMG_STOPPLAY_L, IMG_STOPPLAY_T,
                            IMG_STOPPLAY_R, IMG_STOPPLAY_B,
                            PICT_DRAW, 1, "/Theme/MotionPage/Stop-Released.bmp", pxDefaultScheme );

                BtnCreate( GID_MOTION_BTN_STOPPLAY,
                           BTN_STOPPLAY_L, BTN_STOPPLAY_T,
                           BTN_STOPPLAY_R, BTN_STOPPLAY_B,
                           BTN_RADIUS, BTN_DRAW | BTN_NOPANEL, NULL, NULL, pxBtnScheme );


                // Play the planner or motion file.
                if (prv_xSelectedFileInfo.eFileType == PLANNER_FILE) {
                    ePlayResult = ePlannerRun(prv_xSelectedFileInfo.szFileName);
                } else {
                    ePlayResult = ePlayMotionStart(prv_xSelectedFileInfo.szFileName);
                }
                
                // If failed to open the file for play.
                if (ePlayResult != PLAY_NO_ERROR) {
                    vUpdateMotionPageEndPlaying(prv_xSelectedFileInfo.eFileType);
                }
                break;
                
            // Stop playing button.
            case GID_MOTION_BTN_STOPPLAY:
                // Stop the planner file.
                if (prv_xSelectedFileInfo.eFileType == PLANNER_FILE) {
                    vPlannerStop(prv_xSelectedFileInfo.szFileName, STOP_NOW);
                }
                // Stop the motion file.
                else if (prv_xSelectedFileInfo.eFileType == MOTION_FILE) {
                    vPlayMotionStopAll(STOP_NOW);
                }
                break;
                
            // Edit button.
            case GID_MOTION_BTN_EDIT:
                prv_vCreateEditLockPage();
                break;
                
            // Done button.
            case GID_MOTION_BTN_DONE:
                prv_vCreateMainPage();
                break;
                
            // Exit button.
            case GID_BTN_EXIT:
                // Turn on Sub-PCB.
                // In case we are in teach mode.
                vTeachTurnOnSubPcb();
                
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
                
                // Back to main screen.
                vSetGuiPage(PAGE_MAIN_MENU);
                break;
        }
    }
    
    
    
    // Button is cancelled.
    else if (objMsg == BTN_MSG_CANCELPRESS) {
        switch (GetObjID(pObj)) {
            case GID_MOTION_BTN_TEACH:      vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_TEACH), "/Theme/MotionPage/Teach-Released.bmp");    break;
            case GID_MOTION_BTN_NEXT:       vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_NEXT), "/Theme/MotionPage/Next-Released.bmp");      break;
            case GID_MOTION_BTN_STOPTEACH:  vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_STOPTEACH), "/Theme/MotionPage/Stop-Released.bmp"); break;
            
            case GID_MOTION_BTN_PLAY:       vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_PLAY), "/Theme/MotionPage/Play-Released.bmp");      break;
            case GID_MOTION_BTN_STOPPLAY:   vPictureUpdateBitmap((PICTURE*)GOLFindObject(GID_MOTION_IMG_STOPPLAY), "/Theme/MotionPage/Stop-Released.bmp");  break;
        }
    }
    
    return 1;
}



/*******************************************************************************
 * FUNCTION: vUpdateMotionPageMsg1
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
void vUpdateMotionPageMsg1(char *szText)
{
    vStaticTextUpdateText((STATICTEXT*)GOLFindObject(GID_MOTION_TXT_MSG1), szText);
}



/*******************************************************************************
 * FUNCTION: vUpdateMotionPageMsg2
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
void vUpdateMotionPageMsg2(char *szText)
{
    vStaticTextUpdateText((STATICTEXT*)GOLFindObject(GID_MOTION_TXT_MSG2), szText);
}



/*******************************************************************************
 * FUNCTION: vUpdateMotionPageTimeFrame
 *
 * PARAMETERS:
 * ~ usFrame    - Frame count.
 * ~ usTime     - Elapsed time in second.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Update the frame and time in the motion page.
 *
 *******************************************************************************/
void vUpdateMotionPageTimeFrame(unsigned short usFrame, unsigned short usTime)
{
    vDigitalMeterUpdateValue((DIGITALMETER*)GOLFindObject(GID_MOTION_DM_FRAME_VALUE), (unsigned long)usFrame);
    vDigitalMeterUpdateValue((DIGITALMETER*)GOLFindObject(GID_MOTION_DM_TIME_VALUE), (unsigned long)usTime);
}



/*******************************************************************************
 * FUNCTION: vUpdateMotionPageEndPlaying
 *
 * PARAMETERS:
 * ~ ePlayingType   - To differentiate whether this function is called from
 *                    motion or planner task.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Update the motion page when finish playing the motion/planner file.
 *
 *******************************************************************************/
void vUpdateMotionPageEndPlaying(FILE_TYPE ePlayingType)
{
    // Make sure the current screen is motion page.
    if (eGetGuiPage() == PAGE_MOTION) {
        // Make sure the selected file type is same with the playing type.
        if (prv_xSelectedFileInfo.eFileType == ePlayingType) {
            // If the screen is locked, wake it up.
            vUnlockScreen();

            // Remove the stop button.
            GOLDeleteObjectByID(GID_MOTION_IMG_STOPPLAY);
            GOLDeleteObjectByID(GID_MOTION_BTN_STOPPLAY);
            SetColor(pxDefaultScheme->CommonBkColor);
            Bar(BTN_STOPPLAY_L, BTN_STOPPLAY_T, BTN_STOPPLAY_R, BTN_STOPPLAY_B);
            
            // Remove the label and value for time and frame.
            prv_vRemoveTimeFrame();
            
            // Create the teach, play, and edit button.
            prv_vCreateMainPageButton();
            
            // Enable the gamepad button.
            prv_vEnableGamepadButton();
            
            
            // Update the state of the buttons and file information.
            prv_vSelectButton(prv_eSelectedButton);
        }
    }
}
