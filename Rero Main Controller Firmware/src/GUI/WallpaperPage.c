/*******************************************************************************
 * Functions for the wallpaper page.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "GUI/WallpaperPage.h"
#include "GUI/GraphicScreens.h"
#include "Variables.h"
#include "AudioAmp.h"



/*******************************************************************************
 * FUNCTION: vCreateWallpaperPage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the page to show the wallpaper.
 *
 *******************************************************************************/
void vCreateWallpaperPage (void)
{
    // Clear the GOL objects and screen.
    GOLFree();
    SetColor(BLACK);
    ClearDevice();

    // Create the wallpaper image.
    PictCreate( GID_IMG_WALLPAPPER,
                0,         0,
                GetMaxX(), GetMaxY(),
                PICT_DRAW, 1, "/Theme/Wallpaper.bmp", pxDefaultScheme );
}



/*******************************************************************************
 * FUNCTION: usMsgWallpaperPage
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
 * Handle the touchscreen event of the wallpaper page.
 *
 *******************************************************************************/
WORD usMsgWallpaperPage(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    // Go back to the main menu if the screen is touched.
    if (pMsg->uiEvent == EVENT_RELEASE) {
        vSetGuiPage(PAGE_MAIN_MENU);
    }

    return 1;
}
