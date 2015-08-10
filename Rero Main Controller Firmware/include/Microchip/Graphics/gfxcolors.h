/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  default color definitions.
 *****************************************************************************
 * FileName:        gfxcolors.h
 * Dependencies:    HardwareProfile.h, Graphics.h
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2011 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *
 * Date			Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 02/03/11		...
 *****************************************************************************/

#ifndef _GFX_GCOLORS_H
#define _GFX_GCOLORS_H


/*********************************************************************
* Section: Includes
*********************************************************************/
#include "GenericTypeDefs.h"
#include "GraphicsConfig.h"

#ifndef COLOR_DEPTH
    #error "Please define COLOR_DEPTH in GraphicsConfig.h; valid values (dependent on the driver used): 1, 4, 8, 16, 24)"
#endif

/*********************************************************************
* Overview: Data type that defines the color data. This type is dependent 
*           on the COLOR_DEPTH setting. See COLOR_DEPTH.
*           - GFX_COLOR is type BYTE  if COLOR_DEPTH <= 8   
*           - GFX_COLOR is type WORD if COLOR_DEPTH = 16
*           - GFX_COLOR is type DWORD if COLOR_DEPTH = 24  
*
*********************************************************************/
#if (COLOR_DEPTH <= 8)
    typedef BYTE GFX_COLOR;   
#elif (COLOR_DEPTH == 16)
    typedef WORD GFX_COLOR;   
#elif (COLOR_DEPTH == 24)
    typedef DWORD GFX_COLOR;  
#endif

// color depths 1, 2 or 4 are usually for monochrome format so we are not defining
// any RGB conversion here.
#if (COLOR_DEPTH == 8)

    #ifndef RGBConvert
		/*********************************************************************
		* Macros:  RGBConvert(red, green, blue)
		*
		* Overview: This macro converts the color data to the selected 
		*           COLOR_DEPTH.
		*
		*	<TABLE>
		*    	COLOR_DEPTH            Conversion
		*     	##################     ############
		*     	1                      8-8-8 to 8-8-8 conversion or no conversion
		*     	8                      8-8-8 to 3-3-2 conversion
		*     	16                     8-8-8 to to 5-6-5 conversion
		*     	24                     8-8-8 to 8-8-8 conversion or no conversion						 
		*	</TABLE>
		*
		* PreCondition: none
		*
		* Input: red - red component of the color.
		*        green - green component of the color.
		*        blue - blue component of the color.
		*
		* Output: none
		*
		* Side Effects: none
		*
		********************************************************************/	
        #define RGBConvert(red, green, blue)    (GFX_COLOR) (((GFX_COLOR)(red) & 0xE0) | (((GFX_COLOR)(green) & 0xE0) >> 3) | (((GFX_COLOR)(blue)) >> 6))
    #endif

#elif (COLOR_DEPTH == 16)

    #ifndef RGBConvert
		/*********************************************************************
		* Macros:  RGBConvert(red, green, blue)
		*
		* Overview: This macro converts the color data to the selected 
		*           COLOR_DEPTH.
		*
		*	<TABLE>
		*    	COLOR_DEPTH            Conversion
		*     	##################     ############
		*     	1                      8-8-8 to 8-8-8 conversion or no conversion
		*     	8                      8-8-8 to 3-3-2 conversion
		*     	16                     8-8-8 to to 5-6-5 conversion
		*     	24                     8-8-8 to 8-8-8 conversion or no conversion						 
		*	</TABLE>
		*
		* PreCondition: none
		*
		* Input: red - red component of the color.
		*        green - green component of the color.
		*        blue - blue component of the color.
		*
		* Output: none
		*
		* Side Effects: none
		*
		********************************************************************/	
        #define RGBConvert(red, green, blue)    (GFX_COLOR) (((((GFX_COLOR)(red) & 0xF8) >> 3) << 11) | ((((GFX_COLOR)(green) & 0xFC) >> 2) << 5) | (((GFX_COLOR)(blue) & 0xF8) >> 3))                                               
    #endif

#elif (COLOR_DEPTH == 24)

    #ifndef RGBConvert
 		/*********************************************************************
		* Macros:  RGBConvert(red, green, blue)
		*
		* Overview: This macro converts the color data to the selected 
		*           COLOR_DEPTH.
		*
		*	<TABLE>
		*    	COLOR_DEPTH            Conversion
		*     	##################     ############
		*     	1                      8-8-8 to 8-8-8 conversion or no conversion
		*     	8                      8-8-8 to 3-3-2 conversion
		*     	16                     8-8-8 to to 5-6-5 conversion
		*     	24                     8-8-8 to 8-8-8 conversion or no conversion						 
		*	</TABLE>
		*
		* PreCondition: none
		*
		* Input: red - red component of the color.
		*        green - green component of the color.
		*        blue - blue component of the color.
		*
		* Output: none
		*
		* Side Effects: none
		*
		********************************************************************/	
         #define RGBConvert(red, green, blue)    (GFX_COLOR) (((GFX_COLOR)(red) << 16) | ((GFX_COLOR)(green) << 8) | (GFX_COLOR)(blue))
    #endif

#elif (COLOR_DEPTH == 1)

    #ifndef RGBConvert
	
		/*********************************************************************
		* Macros:  RGBConvert(red, green, blue)
		*
		* Overview: This macro converts the color data to the selected 
		*           COLOR_DEPTH.
		*
		*	<TABLE>
		*    	COLOR_DEPTH            Conversion
		*     	##################     ############
		*     	1                      8-8-8 to 8-8-8 conversion or no conversion
		*     	8                      8-8-8 to 3-3-2 conversion
		*     	16                     8-8-8 to to 5-6-5 conversion
		*     	24                     8-8-8 to 8-8-8 conversion or no conversion						 
		*	</TABLE>
		*
		* PreCondition: none
		*
		* Input: red - red component of the color.
		*        green - green component of the color.
		*        blue - blue component of the color.
		*
		* Output: none
		*
		* Side Effects: none
		*
		********************************************************************/	
        #define RGBConvert(red, green, blue)    (GFX_COLOR) ((GFX_COLOR)red | (GFX_COLOR)green| (GFX_COLOR)blue)
    #endif

#endif

// this macro is deprecated, use RGBConvert() instead
#define RGB565CONVERT(red, green, blue)         (WORD) (((((GFX_COLOR)(red) & 0xF8) >> 3) << 11) | ((((GFX_COLOR)(green) & 0xFC) >> 2) << 5) | (((GFX_COLOR)(blue) & 0xF8) >> 3))   
                                            
/*********************************************************************
* Overview: Some basic colors definitions.
*           The colors defined here are very basic and are defined
*           to fulfill some basic demo requirements. It is recommended
*           that applications define their own set of colors and load
*           the definition before gfxcolors.h is loaded. In case
*           the same color macro name are used, the colors defined 
*           here are skipped.           
*********************************************************************/

#if (COLOR_DEPTH == 1)

    #ifndef BLACK
        #define BLACK     	RGBConvert(0,0,0)    
    #endif
    #ifndef WHITE
       	#define WHITE		RGBConvert(0xFF,0xFF,0xFF)    
    #endif       	    
    	
#elif (COLOR_DEPTH == 4)

    #ifndef BLACK
        #define BLACK   0
    #endif
    #ifndef GRAY000
        #define GRAY000 0
    #endif
    #ifndef GRAY001
        #define GRAY001 1
    #endif
    #ifndef GRAY002
        #define GRAY002 2
    #endif
    #ifndef GRAY003
        #define GRAY003 3 
    #endif
    #ifndef GRAY004
        #define GRAY004 4 
    #endif
    #ifndef GRAY005
        #define GRAY005 5 
    #endif
    #ifndef GRAY006
        #define GRAY001 6
    #endif
    #ifndef GRAY007
        #define GRAY007 7
    #endif
    #ifndef GRAY008
        #define GRAY008 8 
    #endif
    #ifndef GRAY009
        #define GRAY009 9 
    #endif
    #ifndef GRAY0010
        #define GRAY0010 10 
    #endif
    #ifndef GRAY0011
        #define GRAY0011 11 
    #endif
    #ifndef GRAY0012
        #define GRAY0012 12 
    #endif
    #ifndef GRAY0013
        #define GRAY0013 13 
    #endif
    #ifndef GRAY0014
        #define GRAY0014 14 
    #endif
    #ifndef GRAY0015
        #define GRAY0015 15 
    #endif
    #ifndef WHITE
        #define WHITE   15
    #endif
    
#elif (COLOR_DEPTH == 8) || (COLOR_DEPTH == 16) || (COLOR_DEPTH == 24)

    #ifdef USE_PALETTE
    
        #include "PaletteColorDefines.h"
    
    #endif // USE_PALETTE

    #ifndef ORANGE
    	#define ORANGE          RGBConvert(255, 127, 39)
    #endif
    #ifndef BLACK
    	#define BLACK           RGBConvert(0, 0, 0)
    #endif	
    #ifndef BRIGHTBLUE
        #define BRIGHTBLUE      RGBConvert(0, 0, 255)
    #endif	
    #ifndef BRIGHTGREEN
        #define BRIGHTGREEN     RGBConvert(0, 255, 0)
    #endif	
    #ifndef BRIGHTCYAN
        #define BRIGHTCYAN      RGBConvert(0, 255, 255)
    #endif	
    #ifndef PINK
        #define PINK            RGBConvert(255, 100, 170)
    #endif
    #ifndef BRIGHTRED
	    #define BRIGHTRED       RGBConvert(255, 0, 0)
    #endif	
    #ifndef BRIGHTMAGENTA
        #define BRIGHTMAGENTA   RGBConvert(255, 0, 255)
    #endif	
    #ifndef BRIGHTYELLOW
        #define BRIGHTYELLOW    RGBConvert(255, 255, 0)
    #endif	
    #ifndef BLUE
        #define BLUE            RGBConvert(0, 0, 128)
    #endif	
    #ifndef BLUE2
        #define BLUE2            RGBConvert(0, 158, 204)
    #endif	
    #ifndef GREEN
        #define GREEN           RGBConvert(0, 128, 0)
    #endif	
    #ifndef CYAN
        #define CYAN            RGBConvert(0, 128, 128)
    #endif	
    #ifndef RED
        #define RED             RGBConvert(128, 0, 0)
    #endif	
    #ifndef MAGENTA
        #define MAGENTA         RGBConvert(128, 0, 128)
    #endif	
    #ifndef BROWN
    	#define BROWN           RGBConvert(255, 128, 0)
    #endif	
    #ifndef LIGHTBLUE
        #define LIGHTBLUE       RGBConvert(128, 128, 255)
    #endif	
    #ifndef LIGHTGREEN
        #define LIGHTGREEN      RGBConvert(128, 255, 128)
    #endif	
    #ifndef LIGHTCYAN
        #define LIGHTCYAN       RGBConvert(128, 255, 255)
    #endif	
    #ifndef LIGHTRED
        #define LIGHTRED        RGBConvert(255, 128, 128)
    #endif
    #ifndef LIGHTMAGENTA
        #define LIGHTMAGENTA    RGBConvert(255, 128, 255)
    #endif	
    #ifndef YELLOW
        #define YELLOW          RGBConvert(255, 255, 128)
    #endif	
    #ifndef GOLD
        #define GOLD            RGBConvert(160, 160, 40)
    #endif
    #ifndef WHITE
        #define WHITE           RGBConvert(255, 255, 255)
    #endif	
    #ifndef SADDLEBROWN
        #define SADDLEBROWN 	RGBConvert(139, 69, 19)  	// Saddle Color
    #endif	
    #ifndef SIENNA
        #define SIENNA      	RGBConvert(160, 82, 45)  	// Sienna Color
    #endif	
    #ifndef PERU
        #define PERU        	RGBConvert(205, 133, 63) 	// Peru Color
    #endif	
    #ifndef BURLYWOOD
        #define BURLYWOOD  	 	RGBConvert(222, 184, 135)    // Burly wood Color
    #endif	
    #ifndef WHEAT
        #define WHEAT       	RGBConvert(245, 245, 220)    // Wheat Color
    #endif	
    #ifndef TAN
        #define TAN         	RGBConvert(210, 180, 140)    // Tan Color
    #endif	
    #ifndef LIGHTGRAY
        #define LIGHTGRAY       RGBConvert(128, 128, 128)
    #endif
    #ifndef DARKGRAY
        #define DARKGRAY        RGBConvert(64, 64, 64)
    #endif
    #ifndef GRAY95
        #define GRAY95      	RGBConvert(242, 242, 242)    // Gray95 Color
    #endif	
    #ifndef GRAY90
        #define GRAY90      	RGBConvert(229, 229, 229)    // Gray90 Color
    #endif	
    #ifndef GRAY0
        #define GRAY0           RGBConvert(224, 224, 224)
    #endif	
    #ifndef GRAY80
        #define GRAY80      	RGBConvert(204, 204, 204)    // Gray80 Color
    #endif	
    #ifndef GRAY1
        #define GRAY1           RGBConvert(192, 192, 192)
    #endif	
    #ifndef GRAY2
        #define GRAY2           RGBConvert(160, 160, 160)
    #endif	
    #ifndef GRAY4
        #define GRAY4           RGBConvert(96, 96, 96)
    #endif	
    #ifndef GRAY6
        #define GRAY6           RGBConvert(32, 32, 32)
    #endif 
#endif

    
#endif  // #ifndef _GFX_GCOLORS_H

