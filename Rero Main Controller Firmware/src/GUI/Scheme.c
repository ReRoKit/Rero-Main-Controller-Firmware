/*******************************************************************************
 * This file contains the GUI scheme and functions.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "GUI/Scheme.h"
#include "Font/Segoe_UI_Semibold_11.h"
#include "Font/Segoe_UI_Semibold_14.h"
#include "Font/Segoe_UI_Semibold_18.h"
#include "Variables.h"


/*******************************************************************************
 * PUBLIC GLOBAL VARIABLES
 *******************************************************************************/

// GUI Scheme.
GOL_SCHEME *pxDefaultScheme;
GOL_SCHEME *pxMainWndScheme;
GOL_SCHEME *pxBtnScheme;
GOL_SCHEME *pxExitBtnScheme;
GOL_SCHEME *pxTransBtnScheme;
GOL_SCHEME *pxListboxScheme;
GOL_SCHEME *pxAnMeterScheme;
GOL_SCHEME *pxCheckboxScheme;
GOL_SCHEME *pxImgBtnScheme;
GOL_SCHEME *pxEditboxScheme;
GOL_SCHEME *pxSliderScheme;
GOL_SCHEME *pxRadioBtnScheme;
GOL_SCHEME *pxSmallRadioBtnScheme;
GOL_SCHEME *pxLightBlueTxtScheme;
GOL_SCHEME *pxLightGreenTxtScheme;
GOL_SCHEME *pxCustomScheme1;
GOL_SCHEME *pxCustomScheme2;
GOL_SCHEME *pxCustomScheme3;
GOL_SCHEME *pxCustomScheme4;


// Font.
const FONT_FLASH *ptrSmallFont = &Segoe_UI_Semibold_11;
const FONT_FLASH *ptrMediumFont = &Segoe_UI_Semibold_14;
const FONT_FLASH *ptrLargeFont = &Segoe_UI_Semibold_18;



/*******************************************************************************
 * FUNCTION: vCreateScheme
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create and initialize the GUI scheme.
 *
 *******************************************************************************/
void vCreateScheme(void)
{
    // Default scheme.
    pxDefaultScheme = GOLCreateScheme();

#ifdef PRODUCTION
    pxDefaultScheme->EmbossDkColor      = PRODUCTION_RED;     	// Outline colour when not pressed / selected.
    pxDefaultScheme->EmbossLtColor      = PRODUCTION_RED;     	// Outline colour when pressed / selected.
#else  
    pxDefaultScheme->EmbossDkColor      = RERO_LIGHT_GREEN;     // Outline colour when not pressed / selected.
    pxDefaultScheme->EmbossLtColor      = RERO_LIGHT_GREEN;     // Outline colour when pressed / selected.
#endif

    pxDefaultScheme->TextColor0         = WHITE;                // Text colour when not pressed / selected.
    pxDefaultScheme->TextColor1         = WHITE;                // Text colour when pressed / selected.
    pxDefaultScheme->TextColorDisabled  = WHITE;                // Text colour when disabled.
    pxDefaultScheme->Color0             = RERO_LIGHT_BLUE;      // Fill colour when not pressed / selected.
    pxDefaultScheme->Color1             = RERO_LIGHT_BLUE;      // Fill colour when pressed / selected.
    pxDefaultScheme->ColorDisabled      = RERO_DARK_GREY;       // Fill colour when disabled.
    pxDefaultScheme->CommonBkColor      = RERO_DARK_BLUE;       // Background colour when hidden.
    pxDefaultScheme->pFont              = (void*)ptrMediumFont; // Font for text.



    // Main menu window scheme.
    pxMainWndScheme = GOLCreateScheme();
    memcpy(pxMainWndScheme, pxDefaultScheme, sizeof(GOL_SCHEME));
    pxMainWndScheme->Color0             = pxDefaultScheme->CommonBkColor;
    pxMainWndScheme->Color1             = pxDefaultScheme->CommonBkColor;


    // Default button scheme.
    pxBtnScheme = GOLCreateScheme();
    memcpy(pxBtnScheme, pxDefaultScheme, sizeof(GOL_SCHEME));
    pxBtnScheme->EmbossDkColor          = RERO_LIGHT_GREY;
    pxBtnScheme->EmbossLtColor          = RERO_LIGHT_GREY;
    pxBtnScheme->Color1                 = pxDefaultScheme->CommonBkColor;


    // Image button scheme.
    // Used for button with image.
    pxImgBtnScheme = GOLCreateScheme();
    memcpy(pxImgBtnScheme, pxDefaultScheme, sizeof(GOL_SCHEME));
    pxImgBtnScheme->TextColor1          = pxDefaultScheme->CommonBkColor;
    pxImgBtnScheme->TextColorDisabled   = RERO_LIGHT_GREY;
    pxImgBtnScheme->pFont               = (void *)ptrSmallFont;


    // Exit button scheme.
    pxExitBtnScheme = GOLCreateScheme();
    memcpy(pxExitBtnScheme, pxDefaultScheme, sizeof(GOL_SCHEME));
    pxExitBtnScheme->EmbossDkColor      = pxDefaultScheme->CommonBkColor;
    pxExitBtnScheme->EmbossLtColor      = pxDefaultScheme->CommonBkColor;
    pxExitBtnScheme->TextColor0         = pxDefaultScheme->CommonBkColor;;
    pxExitBtnScheme->TextColor1         = RERO_LIGHT_BLUE;
    pxExitBtnScheme->Color1             = pxDefaultScheme->CommonBkColor;


    // Transparent button scheme.
    pxTransBtnScheme = GOLCreateScheme();
    memcpy(pxTransBtnScheme, pxDefaultScheme, sizeof(GOL_SCHEME));
    pxTransBtnScheme->EmbossDkColor        = RERO_LIGHT_BLUE;
    pxTransBtnScheme->EmbossLtColor        = RERO_LIGHT_GREY;
    pxTransBtnScheme->TextColor1           = pxDefaultScheme->CommonBkColor;
    pxTransBtnScheme->TextColorDisabled    = RERO_LIGHT_GREY;
    pxTransBtnScheme->Color0               = pxDefaultScheme->CommonBkColor;
    pxTransBtnScheme->Color1               = RERO_LIGHT_BLUE;
    pxTransBtnScheme->ColorDisabled        = pxDefaultScheme->CommonBkColor;


    // Listbox scheme.
    pxListboxScheme = GOLCreateScheme();
    memcpy(pxListboxScheme, pxDefaultScheme, sizeof(GOL_SCHEME));
    pxListboxScheme->EmbossDkColor      = RERO_LIGHT_BLUE;
    pxListboxScheme->EmbossLtColor      = RERO_LIGHT_BLUE;
    pxListboxScheme->TextColor1         = RERO_LIGHT_GREEN;
    pxListboxScheme->TextColorDisabled  = RERO_LIGHT_GREY;
    pxListboxScheme->Color0             = pxDefaultScheme->CommonBkColor;
    pxListboxScheme->Color1             = pxDefaultScheme->CommonBkColor;
    pxListboxScheme->ColorDisabled      = pxDefaultScheme->CommonBkColor;
    pxListboxScheme->pFont              = (void*)ptrLargeFont;


    // Analog meter scheme.
    pxAnMeterScheme = GOLCreateScheme();
    memcpy(pxAnMeterScheme, pxDefaultScheme, sizeof(GOL_SCHEME));
    pxAnMeterScheme->EmbossDkColor      = pxDefaultScheme->CommonBkColor;
    pxAnMeterScheme->EmbossLtColor      = pxDefaultScheme->CommonBkColor;
    pxAnMeterScheme->TextColor1         = RERO_LIGHT_BLUE;
    pxAnMeterScheme->Color0             = pxDefaultScheme->CommonBkColor;
    pxAnMeterScheme->Color1             = WHITE;
    pxAnMeterScheme->pFont              = (void *)ptrSmallFont;


    // Checkbox scheme.
    pxCheckboxScheme = GOLCreateScheme();
    memcpy(pxCheckboxScheme, pxDefaultScheme, sizeof(GOL_SCHEME));
    pxCheckboxScheme->EmbossDkColor     = RERO_LIGHT_BLUE;
    pxCheckboxScheme->EmbossLtColor     = RERO_LIGHT_BLUE;
    pxCheckboxScheme->TextColorDisabled = RERO_LIGHT_BLUE;
    pxCheckboxScheme->Color0            = pxDefaultScheme->CommonBkColor;
    pxCheckboxScheme->Color1            = pxDefaultScheme->CommonBkColor;
    pxCheckboxScheme->ColorDisabled     = pxDefaultScheme->CommonBkColor;


    // Editbox scheme.
    pxEditboxScheme = GOLCreateScheme();
    memcpy(pxEditboxScheme, pxDefaultScheme, sizeof(GOL_SCHEME));
    pxEditboxScheme->EmbossDkColor      = RERO_LIGHT_BLUE;
    pxEditboxScheme->EmbossLtColor      = RERO_LIGHT_BLUE;
    pxEditboxScheme->TextColorDisabled  = RERO_LIGHT_GREY;
    pxEditboxScheme->Color0             = pxDefaultScheme->CommonBkColor;
    pxEditboxScheme->Color1             = pxDefaultScheme->CommonBkColor;
    pxEditboxScheme->ColorDisabled      = pxDefaultScheme->CommonBkColor;


    // Slider scheme.
    pxSliderScheme = GOLCreateScheme();
    memcpy(pxSliderScheme, pxDefaultScheme, sizeof(GOL_SCHEME));
    pxSliderScheme->EmbossDkColor      = RERO_LIGHT_BLUE;
    pxSliderScheme->EmbossLtColor      = RERO_LIGHT_BLUE;
    pxSliderScheme->Color0             = pxDefaultScheme->CommonBkColor;
    pxSliderScheme->Color1             = pxDefaultScheme->CommonBkColor;
    pxSliderScheme->ColorDisabled      = pxDefaultScheme->CommonBkColor;


    // Radio button scheme.
    pxRadioBtnScheme = GOLCreateScheme();
    memcpy(pxRadioBtnScheme, pxDefaultScheme, sizeof(GOL_SCHEME));
    pxRadioBtnScheme->EmbossDkColor     = RERO_LIGHT_BLUE;
    pxRadioBtnScheme->EmbossLtColor     = RERO_LIGHT_BLUE;
    pxRadioBtnScheme->TextColorDisabled = RERO_LIGHT_GREY;
    pxRadioBtnScheme->Color0            = pxDefaultScheme->CommonBkColor;
    pxRadioBtnScheme->Color1            = pxDefaultScheme->CommonBkColor;
    pxRadioBtnScheme->ColorDisabled     = pxDefaultScheme->CommonBkColor;
    
    
    // Small radio button scheme.
    pxSmallRadioBtnScheme = GOLCreateScheme();
    memcpy(pxSmallRadioBtnScheme, pxDefaultScheme, sizeof(GOL_SCHEME));
    pxSmallRadioBtnScheme->EmbossDkColor        = RERO_LIGHT_BLUE;
    pxSmallRadioBtnScheme->EmbossLtColor        = RERO_LIGHT_BLUE;
    pxSmallRadioBtnScheme->TextColorDisabled    = RERO_LIGHT_GREY;
    pxSmallRadioBtnScheme->Color0               = pxDefaultScheme->CommonBkColor;
    pxSmallRadioBtnScheme->Color1               = pxDefaultScheme->CommonBkColor;
    pxSmallRadioBtnScheme->ColorDisabled        = pxDefaultScheme->CommonBkColor;
    pxSmallRadioBtnScheme->pFont                = (void*)ptrSmallFont;


    // Light blue text scheme.
    pxLightBlueTxtScheme = GOLCreateScheme();
    memcpy(pxLightBlueTxtScheme, pxDefaultScheme, sizeof(GOL_SCHEME));
    pxLightBlueTxtScheme->TextColor0        = RERO_LIGHT_BLUE;
    pxLightBlueTxtScheme->TextColor1        = RERO_LIGHT_BLUE;
    pxLightBlueTxtScheme->TextColorDisabled = RERO_LIGHT_BLUE;


    // Light green text scheme.
    pxLightGreenTxtScheme = GOLCreateScheme();
    memcpy(pxLightGreenTxtScheme, pxDefaultScheme, sizeof(GOL_SCHEME));
    pxLightGreenTxtScheme->TextColor0        = RERO_LIGHT_GREEN;
    pxLightGreenTxtScheme->TextColor1        = RERO_LIGHT_GREEN;
    pxLightGreenTxtScheme->TextColorDisabled = RERO_LIGHT_GREEN;
    
    
    // Custom scheme.
    pxCustomScheme1 = GOLCreateScheme();
    memcpy(pxCustomScheme1, pxDefaultScheme, sizeof(GOL_SCHEME));
    
    pxCustomScheme2 = GOLCreateScheme();
    memcpy(pxCustomScheme2, pxDefaultScheme, sizeof(GOL_SCHEME));
    
    pxCustomScheme3 = GOLCreateScheme();
    memcpy(pxCustomScheme3, pxDefaultScheme, sizeof(GOL_SCHEME));
    
    pxCustomScheme4 = GOLCreateScheme();
    memcpy(pxCustomScheme4, pxDefaultScheme, sizeof(GOL_SCHEME));
}
