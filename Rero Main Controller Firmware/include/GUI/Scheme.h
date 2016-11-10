/*******************************************************************************
 * This file contains the GUI scheme and functions.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef SCHEME_H
#define	SCHEME_H

#include "Microchip/Graphics/Graphics.h"



/*******************************************************************************
 * PUBLIC DEFINITION
 *******************************************************************************/

// ReRo colour.
#define RERO_LIGHT_GREEN    RGBConvert(0x54, 0xed, 0x90)
#define RERO_LIGHT_BLUE     RGBConvert(0x45, 0xae, 0xef)
#define RERO_DARK_BLUE      RGBConvert(0x15, 0x24, 0x35)
#define RERO_DARK_GREY      RGBConvert(0x50, 0x50, 0x50)
#define RERO_LIGHT_GREY     RGBConvert(0x75, 0x75, 0x75)
#define PRODUCTION_RED      RGBConvert(0xef, 0x45, 0x45)



/*******************************************************************************
 * PUBLIC GLOBAL VARAIBLES PROTOTYPES
 *******************************************************************************/

// GUI Scheme.
extern GOL_SCHEME *pxDefaultScheme;
extern GOL_SCHEME *pxMainWndScheme;
extern GOL_SCHEME *pxBtnScheme;
extern GOL_SCHEME *pxExitBtnScheme;
extern GOL_SCHEME *pxTransBtnScheme;
extern GOL_SCHEME *pxListboxScheme;
extern GOL_SCHEME *pxAnMeterScheme;
extern GOL_SCHEME *pxCheckboxScheme;
extern GOL_SCHEME *pxImgBtnScheme;
extern GOL_SCHEME *pxEditboxScheme;
extern GOL_SCHEME *pxSliderScheme;
extern GOL_SCHEME *pxRadioBtnScheme;
extern GOL_SCHEME *pxSmallRadioBtnScheme;
extern GOL_SCHEME *pxLightBlueTxtScheme;
extern GOL_SCHEME *pxLightGreenTxtScheme;
extern GOL_SCHEME *pxCustomScheme1;
extern GOL_SCHEME *pxCustomScheme2;
extern GOL_SCHEME *pxCustomScheme3;
extern GOL_SCHEME *pxCustomScheme4;

// Font.
extern const FONT_FLASH *ptrSmallFont;
extern const FONT_FLASH *ptrMediumFont;
extern const FONT_FLASH *ptrLargeFont;



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

void vCreateScheme(void);



#endif	/* SCHEME_H */
