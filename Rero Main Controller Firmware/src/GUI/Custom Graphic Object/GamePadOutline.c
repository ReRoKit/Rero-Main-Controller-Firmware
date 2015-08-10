/*******************************************************************************
 * Functions for game pad outline object in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "Graphics/Graphics.h"
#include "GUI/GraphicScreens.h"
#include "GUI/Custom Graphic Object/GamePadOutline.h"
#include "GUI/Scheme.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

#define LINE_THICKNESS  0



/*******************************************************************************
 * FUNCTION: GamePadCreate
 *
 * PARAMETERS:
 * ~ ID             - ID for the object.
 * ~ left           - Left position.
 * ~ top            - Top position.
 * ~ right          - Right position.
 * ~ bottom         - Bottom position.
 * ~ PadRadius      - Radius for the left and right pad.
 * ~ RowBtnHeight   - Height for the button row at the bottom.
 * ~ state          - Object state.
 * ~ Colour         - Colour for the object.
 *
 * RETURN:
 * ~ The game pad outline object.
 *
 * DESCRIPTIONS:
 * Create the game pad outline object.
 *
 *******************************************************************************/
GAMEPAD *GamePadCreate(WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, SHORT PadRadius, SHORT RowBtnHeight, WORD state, GFX_COLOR Colour)
{
    GAMEPAD *pGamePad = NULL;

    pGamePad = (GAMEPAD *)GFX_malloc(sizeof(GAMEPAD));
    if(pGamePad == NULL)
        return (pGamePad);

    pGamePad->hdr.ID = ID;
    pGamePad->hdr.pNxtObj = NULL;
    pGamePad->hdr.type = OBJ_CUSTOM;
    pGamePad->hdr.left = left;
    pGamePad->hdr.top = top;
    pGamePad->hdr.right = right;
    pGamePad->hdr.bottom = bottom;
    pGamePad->PadRadius = PadRadius;
    pGamePad->RowBtnHeight = RowBtnHeight;
    pGamePad->hdr.state = state;
    pGamePad->hdr.DrawObj = GamePadDraw;    // draw function
    pGamePad->hdr.MsgObj = NULL;            // message function
    pGamePad->hdr.MsgDefaultObj = NULL;		// default message function
    pGamePad->hdr.FreeObj = NULL;			// free function
    pGamePad->Colour = Colour;
    pGamePad->hdr.pGolScheme = _pDefaultGolScheme;

    GOLAddObject((OBJ_HEADER *)pGamePad);

    return (pGamePad);
}



/*******************************************************************************
 * FUNCTION: GamePadDraw
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ Return 0 if drawing is not completed.
 * ~ Return 1 if drawing is completed.
 *
 * DESCRIPTIONS:
 * Draw the game pad outline object.
 *
 *******************************************************************************/
WORD GamePadDraw(void *pObj)
{
    SHORT x1;
    SHORT x2;
    SHORT x3;
    SHORT x4;
    
    SHORT y1;
    SHORT y2;
    SHORT y3;
    SHORT y4;
    
    GAMEPAD *pGamePad;
    pGamePad = (GAMEPAD *)pObj;

    if(IsDeviceBusy()) {
        return (0);
    }

    
    // Set the colour to background colour if the hide object flag is set.
    if(GetState(pGamePad, GAMEPAD_HIDE)) {
        SetColor(pxDefaultScheme->CommonBkColor);
    } else {
        SetColor(pGamePad->Colour);
    }
    
    // Center of the left and right pad.
    SHORT PadCenterY = pGamePad->hdr.top + pGamePad->PadRadius;
    SHORT LeftPadCenterX = pGamePad->hdr.left + pGamePad->PadRadius;
    SHORT RightPadCenterX = pGamePad->hdr.right - pGamePad->PadRadius;
    
    // Draw the left pad.
    x1 = LeftPadCenterX - pGamePad->PadRadius;
    x2 = LeftPadCenterX - (pGamePad->PadRadius / 3) - LINE_THICKNESS;
    x3 = LeftPadCenterX + (pGamePad->PadRadius / 3) + LINE_THICKNESS;
    x4 = LeftPadCenterX + pGamePad->PadRadius;
    
    y1 = PadCenterY - pGamePad->PadRadius;
    y2 = PadCenterY - (pGamePad->PadRadius / 3) - LINE_THICKNESS;
    y3 = PadCenterY + (pGamePad->PadRadius / 3) + LINE_THICKNESS;
    y4 = PadCenterY + pGamePad->PadRadius;
    
    Bar(x2, y1, x2 + LINE_THICKNESS, y2);
    Bar(x2, y1, x3, y1 + LINE_THICKNESS);
    Bar(x3 - LINE_THICKNESS, y1, x3, y2);
    
    Bar(x3, y2, x4, y2 + LINE_THICKNESS);
    Bar(x4 - LINE_THICKNESS, y2, x4, y3);
    Bar(x3, y3 - LINE_THICKNESS, x4, y3);
    
    Bar(x3 - LINE_THICKNESS, y3, x3, y4);
    Bar(x2, y4 - LINE_THICKNESS, x3, y4);
    Bar(x2, y3, x2 + LINE_THICKNESS, y4);
    
    Bar(x1, y3 - LINE_THICKNESS, x2, y3);
    Bar(x1, y2, x1 + LINE_THICKNESS, y3);
    Bar(x1, y2, x2, y2 + LINE_THICKNESS);
    
    
    // Draw the right pad.
    Arc(RightPadCenterX, PadCenterY, RightPadCenterX, PadCenterY, pGamePad->PadRadius - LINE_THICKNESS, pGamePad->PadRadius, 0xff);
    
    
    
    // Draw the bottom button row.
    x1 = pGamePad->hdr.left;
    x2 = pGamePad->hdr.right;
    y1 = pGamePad->hdr.bottom - pGamePad->RowBtnHeight;
    y2 = pGamePad->hdr.bottom;
    
    Bar(x1, y1, x2, y1 + LINE_THICKNESS);
    Bar(x2 - LINE_THICKNESS, y1, x2, y2);
    Bar(x1, y2 - LINE_THICKNESS, x2, y2);
    Bar(x1, y1, x1 + LINE_THICKNESS, y2);
    
    
    return (1);
}
