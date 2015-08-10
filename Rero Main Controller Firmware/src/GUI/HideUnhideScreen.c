/*******************************************************************************
 * Functions to hide or unhide the screen.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "Variables.h"
#include "GUI/HideUnhideScreen.h"
#include "GUI/Scheme.h"



/*******************************************************************************
 * PUBLIC GLOBAL VARIABLES
 *******************************************************************************/

// Graphical Objects List for the previous screen.
static OBJ_HEADER *prv_pxPreviousGolObjects = NULL;



/*******************************************************************************
 * FUNCTION: vHideScreen
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Hide the screen.
 *
 *******************************************************************************/
void vHideScreen(void)
{
    // Save GOL objects list if it's not saved before.
    if (prv_pxPreviousGolObjects == NULL) {
        prv_pxPreviousGolObjects = GOLGetList();
    }
    
    // Create a new GOL objects list.
    GOLNewList();

    // Clear the screen.
    SetColor(pxDefaultScheme->CommonBkColor);
    ClearDevice();
}



/*******************************************************************************
 * FUNCTION: vUnhidePreviousScreen
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Unhide the screen which is hidden previously.
 *
 *******************************************************************************/
void vUnhidePreviousScreen(void)
{
    // Clear all objects on current screen.
    GOLFree();

    // Clear the screen.
    SetColor(pxDefaultScheme->CommonBkColor);
    ClearDevice();

    // Restore the previous GOL objects list.
    GOLSetList(prv_pxPreviousGolObjects);
    prv_pxPreviousGolObjects = NULL;

    // Redraw all objects.
    OBJ_HEADER *pxCurrentObject = GOLGetList();
    while (pxCurrentObject != NULL) {
        GOLRedraw(pxCurrentObject);
        pxCurrentObject = GetObjNext(pxCurrentObject);
    }
}
