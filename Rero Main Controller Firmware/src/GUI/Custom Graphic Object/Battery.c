/*******************************************************************************
 * Functions for battery object in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "Graphics/Graphics.h"
#include "GUI/Custom Graphic Object/Battery.h"
#include "GUI/GraphicScreens.h"
#include "Variables.h"



/*******************************************************************************
 * FUNCTION: BattCreate
 *
 * PARAMETERS:
 * ~ ID         - ID for the object.
 * ~ left       - Left position.
 * ~ top        - Top position.
 * ~ right      - Right position.
 * ~ bottom     - Bottom position.
 * ~ state      - Object state.
 * ~ pScheme    - Scheme for the object.
 *
 * RETURN:
 * ~ The battery object.
 *
 * DESCRIPTIONS:
 * Create the battery object.
 *
 *******************************************************************************/
BATT *BattCreate(WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, WORD state, GOL_SCHEME *pScheme)
{
    BATT *pBatt = NULL;

    pBatt = (BATT *)GFX_malloc(sizeof(BATT));
    if(pBatt == NULL)
        return (pBatt);

    pBatt->hdr.ID = ID;
    pBatt->hdr.pNxtObj = NULL;
    pBatt->hdr.type = OBJ_CUSTOM;
    pBatt->hdr.left = left;
    pBatt->hdr.top = top;
    pBatt->hdr.right = right;
    pBatt->hdr.bottom = bottom;
    pBatt->hdr.state = state;
    pBatt->hdr.DrawObj = BattDraw;			// draw function
    pBatt->hdr.MsgObj = NULL;                   // message function
    pBatt->hdr.MsgDefaultObj = NULL;		// default message function
    pBatt->hdr.FreeObj = NULL;				// free function

    // Set the style scheme to be used
    if(pScheme == NULL)
        pBatt->hdr.pGolScheme = _pDefaultGolScheme;
    else
        pBatt->hdr.pGolScheme = (GOL_SCHEME *)pScheme;

    GOLAddObject((OBJ_HEADER *)pBatt);

    return (pBatt);
}



/*******************************************************************************
 * FUNCTION: BattDraw
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ Return 0 if drawing is not completed.
 * ~ Return 1 if drawing is completed.
 *
 * DESCRIPTIONS:
 * Draw the battery object.
 *
 *******************************************************************************/
WORD BattDraw(void *pObj)
{
    BATT *pBatt;
    pBatt = (BATT *)pObj;

    if(IsDeviceBusy()) {
        return (0);
    }

    
    // Hide the object if the flag is set.
    if(GetState(pBatt, BATT_HIDE)) {
        SetColor(pBatt->hdr.pGolScheme->CommonBkColor);
        if(!Bar(pBatt->hdr.left, pBatt->hdr.top, pBatt->hdr.right, pBatt->hdr.bottom)) {
            return (0);
        }
        return (1);
    }



    // Clear the space in the body.
    SetColor(pBatt->hdr.pGolScheme->CommonBkColor);
    Bar(pBatt->hdr.left, pBatt->hdr.top + 2, pBatt->hdr.right, pBatt->hdr.bottom);

    // Draw battery body.
    SetColor(WHITE);
    Bar(pBatt->hdr.left, pBatt->hdr.top + 2, pBatt->hdr.left, pBatt->hdr.bottom);
    Bar(pBatt->hdr.right, pBatt->hdr.top + 2, pBatt->hdr.right, pBatt->hdr.bottom);
    Bar(pBatt->hdr.left, pBatt->hdr.top + 2, pBatt->hdr.right, pBatt->hdr.top + 2);
    Bar(pBatt->hdr.left, pBatt->hdr.bottom, pBatt->hdr.right, pBatt->hdr.bottom);
    
    // Draw the tip.
    Bar(pBatt->hdr.left + 3, pBatt->hdr.top, pBatt->hdr.right - 3, pBatt->hdr.top + 2);


    // Select colout based on battery percentage.
    if (uiBattPercentage > 66) {
        SetColor(RERO_LIGHT_GREEN);
    }
    else if (uiBattPercentage > 33) {
        SetColor(BRIGHTYELLOW);
    }
    else {
        SetColor(BRIGHTRED);
    }

    SHORT sHeight = (pBatt->hdr.bottom - pBatt->hdr.top - 6) * uiBattPercentage / 100;
    Bar(pBatt->hdr.left + 2, pBatt->hdr.bottom - 2 - sHeight, pBatt->hdr.right - 2, pBatt->hdr.bottom - 2);  // Content


    return (1);
}
