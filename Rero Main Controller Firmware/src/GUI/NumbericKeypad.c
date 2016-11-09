/*******************************************************************************
 * Functions for numberic keypad in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "GUI/NumbericKeypad.h"
#include "GUI/GraphicScreens.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

// Position and size for graphic objects.
#define TXT_HEIGHT          20
#define EB_HEIGHT           36
#define TXT_EB_GAP          2

// Keypad.
#define BTN_KEY_WIDTH       53
#define BTN_KEY_HEIGHT      40
#define BTN_KEY_GAP_X       5
#define BTN_KEY_GAP_Y       5

#define KEYPAD_WIDTH        ((BTN_KEY_WIDTH * 3) + (BTN_KEY_GAP_X * 2))
#define KEYPAD_HEIGHT       ((BTN_KEY_HEIGHT * 4) + (BTN_KEY_GAP_Y * 3))
#define KEYPAD_R            WND_LIMIT_RIGHT
#define KEYPAD_L            (KEYPAD_R - KEYPAD_WIDTH)
#define KEYPAD_T            ((WND_SUBPAGE_TOP + WND_SUBPAGE_BOTTOM) / 2 - (KEYPAD_HEIGHT / 2))
#define KEYPAD_B            (KEYPAD_T + KEYPAD_HEIGHT)

// Title 1.
#define TXT_TITLE1_L        (WND_LIMIT_LEFT + 5)
#define TXT_TITLE1_R        (KEYPAD_L - 10)
#define TXT_TITLE1_T        KEYPAD_T
#define TXT_TITLE1_B        (TXT_TITLE1_T + TXT_HEIGHT)

// Value 1 edit box.
#define EB_VALUE1_L         TXT_TITLE1_L
#define EB_VALUE1_R         TXT_TITLE1_R
#define EB_VALUE1_T         (TXT_TITLE1_B + TXT_EB_GAP)
#define EB_VALUE1_B         (EB_VALUE1_T + EB_HEIGHT)

// Read button.
#define BTN_READ_HEIGHT     20
#define BTN_READ_L          TXT_TITLE1_L
#define BTN_READ_R          TXT_TITLE1_R
#define BTN_READ_T          (EB_VALUE1_B + 4)
#define BTN_READ_B          (BTN_READ_T + BTN_READ_HEIGHT)

// Range.
#define TXT_RANGE_L         TXT_TITLE1_L
#define TXT_RANGE_R         TXT_TITLE1_R
#define TXT_RANGE_B         KEYPAD_B
#define TXT_RANGE_T         (TXT_RANGE_B - TXT_HEIGHT)

// Value 2 edit box.
#define EB_VALUE2_L        TXT_TITLE1_L
#define EB_VALUE2_R        TXT_TITLE1_R
#define EB_VALUE2_B        (TXT_RANGE_T - TXT_EB_GAP)
#define EB_VALUE2_T        (EB_VALUE2_B - EB_HEIGHT)

// Title 2.
#define TXT_TITLE2_L       TXT_TITLE1_L
#define TXT_TITLE2_R       TXT_TITLE1_R
#define TXT_TITLE2_B       (EB_VALUE2_T - TXT_EB_GAP)
#define TXT_TITLE2_T       (TXT_TITLE2_B - TXT_HEIGHT)



/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 *******************************************************************************/

static void prv_vCreateKeypad(void);



/*******************************************************************************
 * FUNCTION: prv_vCreateKeypad
 *
 * PARAMETERS:
 * ~ void
 * 
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create a numberic keypad.
 *
 *******************************************************************************/
static void prv_vCreateKeypad(void)
{
    // Create key 1 - 9.
    static const char aszNumber[][2] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
    unsigned char ucRow = 0;
    unsigned char ucColumm = 0;
    unsigned char ucNumber = 0;

    signed short ssLeftTemp = KEYPAD_L;
    signed short ssTopTemp = KEYPAD_T;
    
    for (ucRow = 0; ucRow < 3; ucRow++) {
        for (ucColumm = 0; ucColumm < 3; ucColumm++) {
            BtnCreate(  GID_KEYPAD_1 + ucNumber,
                        ssLeftTemp,              ssTopTemp,
                        ssLeftTemp + BTN_KEY_WIDTH, ssTopTemp + BTN_KEY_HEIGHT,
                        BTN_RADIUS, BTN_DRAW, NULL, (char *)aszNumber[ucNumber], pxBtnScheme
                     );

            // Increase the X position and number.
            ssLeftTemp += BTN_KEY_WIDTH + BTN_KEY_GAP_X;
            ucNumber++;
        }

        // Reset X position and increase Y position.
        ssLeftTemp = KEYPAD_L;
        ssTopTemp += BTN_KEY_HEIGHT + BTN_KEY_GAP_Y;
    }



    // Create key "DEL".
    BtnCreate(  GID_KEYPAD_DEL,
                ssLeftTemp,              ssTopTemp,
                ssLeftTemp + BTN_KEY_WIDTH, ssTopTemp + BTN_KEY_HEIGHT,
                BTN_RADIUS, BTN_DRAW, NULL, "DEL", pxBtnScheme
             );

    // Increase the X position.
    ssLeftTemp += BTN_KEY_WIDTH + BTN_KEY_GAP_X;



    // Create key 0.
    BtnCreate(  GID_KEYPAD_0,
                ssLeftTemp,              ssTopTemp,
                ssLeftTemp + BTN_KEY_WIDTH, ssTopTemp + BTN_KEY_HEIGHT,
                BTN_RADIUS, BTN_DRAW, NULL, "0", pxBtnScheme
             );

    // Increase the X position.
    ssLeftTemp += BTN_KEY_WIDTH + BTN_KEY_GAP_X;



    // Create key "OK".
    BtnCreate(  GID_KEYPAD_OK,
                ssLeftTemp,              ssTopTemp,
                ssLeftTemp + BTN_KEY_WIDTH, ssTopTemp + BTN_KEY_HEIGHT,
                BTN_RADIUS, BTN_DRAW, NULL, "OK", pxBtnScheme
             );
}



/*******************************************************************************
 * FUNCTION: vCreateNumericKeypadPage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the numeric keypad page.
 *
 *******************************************************************************/
void vCreateNumericKeypadPage(void)
{
    // Determine the text based on the current page.
    char *szPageTitle = "";
    char *szReadButton = "";
    char *szTitle2 = "";
    char *szRange = "";
    unsigned short usEditBox2Length = 0;
    
    switch (eGetGuiPage()) {
        case PAGE_SET_ID:
            szPageTitle = "SET ID";
            szReadButton = "Read ID";
            szTitle2 = "Set ID to :";
            szRange = "( 0 - 253 )";
            usEditBox2Length = 3;
            break;
            
        case PAGE_SET_BT_PIN:
            szPageTitle = "SET BLUETOOTH PIN (EDR)";
            szReadButton = "Read PIN";
            szTitle2 = "Set PIN to :";
            szRange = "( 0000 - 9999 )";
            usEditBox2Length = 4;
            break;
    }
    
    
    
    GOLFree();
    vCreatePageWindow(szPageTitle);


    // Create text "Message :".
    StCreate( GID_KEYPAD_TXT_TITLE1,
              TXT_TITLE1_L, TXT_TITLE1_T,
              TXT_TITLE1_R, TXT_TITLE1_B,
              ST_DRAW, "Message :", pxDefaultScheme );
            
    // Create editbox for current value or message.
    EbCreate( GID_KEYPAD_EB_VALUE1,
              EB_VALUE1_L, EB_VALUE1_T,
              EB_VALUE1_R, EB_VALUE1_B,
              EB_DRAW | EB_CENTER_ALIGN, NULL, 15, pxEditboxScheme );

    // Create the read value button.
    BtnCreate( GID_KEYPAD_BTN_READ,
               BTN_READ_L, BTN_READ_T,
               BTN_READ_R, BTN_READ_B,
               1, BTN_DRAW, NULL, szReadButton, pxBtnScheme );



    // Create title 2.
    StCreate( GID_KEYPAD_TXT_TITLE2,
              TXT_TITLE2_L, TXT_TITLE2_T,
              TXT_TITLE2_R, TXT_TITLE2_B,
              ST_DRAW, szTitle2, pxDefaultScheme );
    
    // Create editbox for new value.
    EbCreate( GID_KEYPAD_EB_VALUE2,
              EB_VALUE2_L, EB_VALUE2_T,
              EB_VALUE2_R, EB_VALUE2_B,
              EB_DRAW | EB_CENTER_ALIGN, NULL, usEditBox2Length, pxEditboxScheme );
    
    // Create text for new ID range.
    StCreate( GID_KEYPAD_TXT_RANGE,
              TXT_RANGE_L, TXT_RANGE_T,
              TXT_RANGE_R, TXT_RANGE_B,
              ST_DRAW | ST_CENTER_ALIGN, szRange, pxDefaultScheme );
    
    
    
    // Create keypad.
    prv_vCreateKeypad();
}



/*******************************************************************************
 * FUNCTION: usMsgNumericKeypadPage
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
 * Handle the touchscreen event of the Set ID page.
 *
 *******************************************************************************/
WORD usMsgNumericKeypadPage(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    unsigned short usObjectId = GetObjID(pObj);
    EDITBOX *pxEditbox1 = (EDITBOX*)GOLFindObject(GID_KEYPAD_EB_VALUE1);
    EDITBOX *pxEditbox2 = (EDITBOX*)GOLFindObject(GID_KEYPAD_EB_VALUE2);
    
    // Make sure button is released.
    if (objMsg == BTN_MSG_RELEASED) {
        // Clear message on editbox 1.
        vEditBoxUpdateText(pxEditbox1, "");

        // If key 0 - 9 is pressed...
        if ((usObjectId >= GID_KEYPAD_0) && (usObjectId <= GID_KEYPAD_9)) {
            // Add the pressed key to editbox 2.
            if (pxEditbox2 != NULL) {
                EbAddChar(pxEditbox2, '0' + (usObjectId - GID_KEYPAD_0));
                SetState(pxEditbox2, EB_DRAW);
            }
        }

        // DEL button.
        else if (usObjectId == GID_KEYPAD_DEL) {
            // Remove 1 character from editbox 2.
            if (pxEditbox2 != NULL) {
                EbDeleteChar(pxEditbox2);
                SetState(pxEditbox2, EB_DRAW);
            }
        }

    }
    
    
    return 1;
}
