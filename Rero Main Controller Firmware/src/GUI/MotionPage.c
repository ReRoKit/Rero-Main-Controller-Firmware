/*******************************************************************************
 * Functions for motion page in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "GUI/MotionPage.h"
#include "GUI/GraphicScreens.h"
#include "GUI/Custom Graphic Object/GamePadOutline.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

// Position and size for graphic objects.
#define TXT_HEIGHT              20
#define RB_HEIGHT               26
#define RB_WIDTH                (RB_HEIGHT + (RB_INDENT * 2))

// Game pad.
#define GAMEPAD_HEIGHT          140
#define GAMEPAD_PAD_RADIUS      48
#define GAMEPAD_ROW_BTN_HEIGHT  32
#define GAMEPAD_L               (WND_SUBPAGE_LEFT + 25)
#define GAMEPAD_R               (WND_SUBPAGE_RIGHT - 25)
#define GAMEPAD_T               (WND_SUBPAGE_TOP + 5)
#define GAMEPAD_B               (GAMEPAD_T + GAMEPAD_HEIGHT)


// Common parameters for round pad.
#define PAD_CENTER_Y            (GAMEPAD_T + GAMEPAD_PAD_RADIUS)
#define PAD_RADIUS              (GAMEPAD_PAD_RADIUS - 5)

#define LEFT_PAD_CENTER_X       (GAMEPAD_L + GAMEPAD_PAD_RADIUS)
#define RIGHT_PAD_CENTER_X      (GAMEPAD_R - GAMEPAD_PAD_RADIUS)


// Left pad up button.
#define RB_XU_L                 (LEFT_PAD_CENTER_X - (RB_WIDTH / 2))
#define RB_XU_R                 (RB_XU_L + RB_WIDTH)
#define RB_XU_T                 (PAD_CENTER_Y - PAD_RADIUS)
#define RB_XU_B                 (RB_XU_T + RB_HEIGHT)

// Left pad down button.
#define RB_XD_L                 RB_XU_L
#define RB_XD_R                 RB_XU_R
#define RB_XD_B                 (PAD_CENTER_Y + PAD_RADIUS)
#define RB_XD_T                 (RB_XD_B - RB_HEIGHT)

// Left pad left button.
#define RB_XL_L                 (LEFT_PAD_CENTER_X - PAD_RADIUS)
#define RB_XL_R                 (RB_XL_L + RB_WIDTH)
#define RB_XL_T                 (PAD_CENTER_Y - (RB_HEIGHT / 2))
#define RB_XL_B                 (RB_XL_T + RB_HEIGHT)

// Left pad right button.
#define RB_XR_R                 (LEFT_PAD_CENTER_X + PAD_RADIUS)
#define RB_XR_L                 (RB_XR_R - RB_WIDTH)
#define RB_XR_T                 RB_XL_T
#define RB_XR_B                 RB_XL_B



// Right pad up button.
#define RB_CU_L                 (RIGHT_PAD_CENTER_X - (RB_WIDTH / 2))
#define RB_CU_R                 (RB_CU_L + RB_WIDTH)
#define RB_CU_T                 (PAD_CENTER_Y - PAD_RADIUS)
#define RB_CU_B                 (RB_CU_T + RB_HEIGHT)

// Right pad down button.
#define RB_CD_L                 RB_CU_L
#define RB_CD_R                 RB_CU_R
#define RB_CD_B                 (PAD_CENTER_Y + PAD_RADIUS)
#define RB_CD_T                 (RB_CD_B - RB_HEIGHT)

// Right pad left button.
#define RB_CL_L                 (RIGHT_PAD_CENTER_X - PAD_RADIUS)
#define RB_CL_R                 (RB_CL_L + RB_WIDTH)
#define RB_CL_T                 (PAD_CENTER_Y - (RB_HEIGHT / 2))
#define RB_CL_B                 (RB_CL_T + RB_HEIGHT)

// Right pad right button.
#define RB_CR_R                 (RIGHT_PAD_CENTER_X + PAD_RADIUS)
#define RB_CR_L                 (RB_CR_R - RB_WIDTH)
#define RB_CR_T                 RB_CL_T
#define RB_CR_B                 RB_CL_B



// Bottom button common parameters.
#define ROW_BTN_C2C             ((GAMEPAD_R - GAMEPAD_L) / 5)

// Bottom button 1.
#define RB_F1_L                 (GAMEPAD_L + (ROW_BTN_C2C / 2) - (RB_WIDTH / 2))
#define RB_F1_R                 (RB_F1_L + RB_WIDTH)
#define RB_F1_T                 (GAMEPAD_B - (GAMEPAD_ROW_BTN_HEIGHT / 2) - (RB_HEIGHT / 2))
#define RB_F1_B                 (RB_F1_T + RB_HEIGHT)

// Bottom button 2.
#define RB_F2_L                 (RB_F1_L + ROW_BTN_C2C)
#define RB_F2_R                 (RB_F2_L + RB_WIDTH)
#define RB_F2_T                 RB_F1_T
#define RB_F2_B                 RB_F1_B

// Bottom button 3.
#define RB_F3_L                 (RB_F2_L + ROW_BTN_C2C)
#define RB_F3_R                 (RB_F3_L + RB_WIDTH)
#define RB_F3_T                 RB_F1_T
#define RB_F3_B                 RB_F1_B

// Bottom button 4.
#define RB_F4_L                 (RB_F3_L + ROW_BTN_C2C)
#define RB_F4_R                 (RB_F4_L + RB_WIDTH)
#define RB_F4_T                 RB_F1_T
#define RB_F4_B                 RB_F1_B

// Bottom button 5.
#define RB_F5_L                 (RB_F4_L + ROW_BTN_C2C)
#define RB_F5_R                 (RB_F5_L + RB_WIDTH)
#define RB_F5_T                 RB_F1_T
#define RB_F5_B                 RB_F1_B



// Select button.
#define BTN_SELECT_WIDTH        65
#define BTN_SELECT_HEIGHT       20
#define BTN_SELECT_L            ((WND_LIMIT_LEFT + WND_LIMIT_RIGHT) / 2 - (BTN_SELECT_WIDTH / 2))
#define BTN_SELECT_R            (BTN_SELECT_L + BTN_SELECT_WIDTH)
#define BTN_SELECT_T            (GAMEPAD_T + 75)
#define BTN_SELECT_B            (BTN_SELECT_T + BTN_SELECT_HEIGHT)



// File type.
#define TXT_FILETYPE_WIDTH      85
#define TXT_FILETYPE_L          WND_LIMIT_LEFT
#define TXT_FILETYPE_R          (TXT_FILETYPE_L + TXT_FILETYPE_WIDTH)
#define TXT_FILETYPE_B          (WND_SUBPAGE_BOTTOM - 10)
#define TXT_FILETYPE_T          (TXT_FILETYPE_B - TXT_HEIGHT)

// File name.
#define TXT_FILENAME_WIDTH      115
#define TXT_FILENAME_L          TXT_FILETYPE_R
#define TXT_FILENAME_R          (TXT_FILENAME_L + TXT_FILENAME_WIDTH)
#define TXT_FILENAME_T          TXT_FILETYPE_T
#define TXT_FILENAME_B          TXT_FILETYPE_B

// Edit lock button.
#define BTN_LOCK_WIDTH          45
#define BTN_LOCK_HEIGHT         20
#define BTN_LOCK_R              (WND_SUBPAGE_RIGHT - 15)
#define BTN_LOCK_L              (BTN_LOCK_R - BTN_LOCK_WIDTH)
#define BTN_LOCK_B              TXT_FILETYPE_B
#define BTN_LOCK_T              (BTN_LOCK_B - BTN_LOCK_HEIGHT)

// Lock icon.
#define IMG_LOCK_WIDTH          40
#define IMG_LOCK_HEIGHT         40
#define IMG_LOCK_R              BTN_LOCK_L
#define IMG_LOCK_L              (IMG_LOCK_R - IMG_LOCK_WIDTH)
#define IMG_LOCK_T              (BTN_LOCK_T - 11)
#define IMG_LOCK_B              (IMG_LOCK_T + IMG_LOCK_HEIGHT)



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
    
    // Game pad outline.
    GamePadCreate( GID_MOTION_GAMEPAD,
                   GAMEPAD_L, GAMEPAD_T,
                   GAMEPAD_R, GAMEPAD_B,
                   GAMEPAD_PAD_RADIUS, GAMEPAD_ROW_BTN_HEIGHT, GAMEPAD_DRAW, RERO_LIGHT_GREEN);
    
    
    // Radio buttons for left pad.
    RbCreate( GID_MOTION_RB_XU,
              RB_XU_L, RB_XU_T,
              RB_XU_R, RB_XU_B,
              RB_DRAW | RB_GROUP, NULL, pxRadioBtnScheme );
    
    RbCreate( GID_MOTION_RB_XD,
              RB_XD_L, RB_XD_T,
              RB_XD_R, RB_XD_B,
              RB_DRAW, NULL, pxRadioBtnScheme );
    
    RbCreate( GID_MOTION_RB_XL,
              RB_XL_L, RB_XL_T,
              RB_XL_R, RB_XL_B,
              RB_DRAW, NULL, pxRadioBtnScheme );
    
    RbCreate( GID_MOTION_RB_XR,
              RB_XR_L, RB_XR_T,
              RB_XR_R, RB_XR_B,
              RB_DRAW, NULL, pxRadioBtnScheme );
    
    
    
    // Radio buttons for right pad.
    RbCreate( GID_MOTION_RB_CU,
              RB_CU_L, RB_CU_T,
              RB_CU_R, RB_CU_B,
              RB_DRAW, NULL, pxRadioBtnScheme );
    
    RbCreate( GID_MOTION_RB_CD,
              RB_CD_L, RB_CD_T,
              RB_CD_R, RB_CD_B,
              RB_DRAW, NULL, pxRadioBtnScheme );
    
    RbCreate( GID_MOTION_RB_CL,
              RB_CL_L, RB_CL_T,
              RB_CL_R, RB_CL_B,
              RB_DRAW, NULL, pxRadioBtnScheme );
    
    RbCreate( GID_MOTION_RB_CR,
              RB_CR_L, RB_CR_T,
              RB_CR_R, RB_CR_B,
              RB_DRAW, NULL, pxRadioBtnScheme );
    
    
    
    // Radio button for the bottom row.
    RbCreate( GID_MOTION_RB_F1,
              RB_F1_L, RB_F1_T,
              RB_F1_R, RB_F1_B,
              RB_DRAW, NULL, pxRadioBtnScheme );
    
    RbCreate( GID_MOTION_RB_F2,
              RB_F2_L, RB_F2_T,
              RB_F2_R, RB_F2_B,
              RB_DRAW, NULL, pxRadioBtnScheme );
    
    RbCreate( GID_MOTION_RB_F3,
              RB_F3_L, RB_F3_T,
              RB_F3_R, RB_F3_B,
              RB_DRAW, NULL, pxRadioBtnScheme );
    
    RbCreate( GID_MOTION_RB_F4,
              RB_F4_L, RB_F4_T,
              RB_F4_R, RB_F4_B,
              RB_DRAW, NULL, pxRadioBtnScheme );
    
    RbCreate( GID_MOTION_RB_F5,
              RB_F5_L, RB_F5_T,
              RB_F5_R, RB_F5_B,
              RB_DRAW, NULL, pxRadioBtnScheme );
    
    
    
    // Select button.
    BtnCreate( GID_MOTION_BTN_SELECT,
               BTN_SELECT_L, BTN_SELECT_T,
               BTN_SELECT_R, BTN_SELECT_B,
               BTN_RADIUS, BTN_DRAW, NULL, "Select", pxTransBtnScheme );
    
    
    
    // File type.
    StCreate( GID_MOTION_TXT_FILETYPE,
              TXT_FILETYPE_L, TXT_FILETYPE_T,
              TXT_FILETYPE_R, TXT_FILETYPE_B,
              ST_DRAW, "Planner File:", pxDefaultScheme );
    
    // Filename.
    StCreate( GID_MOTION_TXT_FILENAME,
              TXT_FILENAME_L, TXT_FILENAME_T,
              TXT_FILENAME_R, TXT_FILENAME_B,
              ST_DRAW, "PlannerFileName", pxLightBlueTxtScheme );
    
    
    
    // Lock image.
    PictCreate( GID_MOTION_IMG_LOCK,
                IMG_LOCK_L, IMG_LOCK_T,
                IMG_LOCK_R, IMG_LOCK_B,
                PICT_DRAW | PICT_DISABLED, 1, "/theme/icon/lock.ico", pxDefaultScheme );
    
    // Edit lock button.
    BtnCreate( GID_MOTION_BTN_LOCK,
               BTN_LOCK_L, BTN_LOCK_T,
               BTN_LOCK_R, BTN_LOCK_B,
               1, BTN_DRAW, NULL, "Edit", pxTransBtnScheme );
    
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
    // Button is released.
    if (objMsg == BTN_MSG_RELEASED) {
        switch (GetObjID(pObj)) {
            // Exit button.
            case GID_BTN_EXIT:
                // Back to main screen.
                vSetGuiPage(PAGE_MAIN_MENU);
                break;
        }
    }
}
