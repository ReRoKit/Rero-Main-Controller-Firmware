/*****************************************************************************
 *  Module for Microchip Graphics Library 
 *  Graphic Primitives Layer
 *****************************************************************************
 * FileName:        Primitive.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2008 Microchip Technology Inc.  All rights reserved.
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
 * Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 11/12/07	   Version 1.0 release
 * 18/05/09    Version 1.1 - All Primitive functions have
 *                           return value(To support 2d-Acceleration)
 * 05/12/10    Added sine and cosine functions.
 * 08/20/10    Modified TYPE_MEMORY enum for more types
 *			   and changed the enumeration to GFX_RESOURCE. Also removed 
 *             unused types. 
 *             Added new structure GFX_IMAGE_HEADER. 
 *             Deprecated the following (renamed):
 *                TYPE_MEMORY     
 *                EXTDATA         
 *                BITMAP_FLASH    
 *                BITMAP_RAM      
 *                BITMAP_EXTERNAL  
 * 03/09/11    Removed USE_DRV_xxx.
 * 04/01/11    Added GetSineCosine().
 *****************************************************************************/
#ifndef _PRIMITIVE_H
    #define _PRIMITIVE_H

#include "GenericTypeDefs.h"
#include "GraphicsConfig.h"
#include "gfxcolors.h"
/*********************************************************************
* Overview: Primitive lines are drawn using line type and line thickness.
*			There are 3 line styles and 2 types of line thickness.
*
*********************************************************************/

// Solid Line Style
    #define SOLID_LINE  0

// Dotted Line Style
    #define DOTTED_LINE 1

// Dashed Line Style
    #define DASHED_LINE 4

// Normal Line (thickness is 1 pixel)
    #define NORMAL_LINE 0

// Thick Line (thickness is 3 pixels)
    #define THICK_LINE  1

/*********************************************************************
* Overview: Drawing bitmaps will have two modes, normal rendering
*			and stretched rendering. Stretched rendering effectively
*			doubles the image size in the horizontal and vertical 
*			direction.
*
*********************************************************************/

// Normal image stretch code
    #define IMAGE_NORMAL    1

// Stretched image stretch code
    #define IMAGE_X2    2

// Current line style
extern SHORT    _lineType;

// Current line thickness
extern BYTE     _lineThickness;

// constants used for circle/arc computation
    #define SIN45   	46341   // sin(45) * 2^16)
    #define ONEP25  	81920   // 1.25 * 2^16
// constants used to get sine(v) and cosine(v) 
	#define GETSINE		0x00
	#define GETCOSINE	0x01

// Current cursor coordinates
extern SHORT    _cursorX;
extern SHORT    _cursorY;

// Font orientation
extern BYTE     _fontOrientation;

    #define ORIENT_HOR  0
    #define ORIENT_VER  1

/*********************************************************************
* Overview: This macro sets the data type for the Fonts.
*			There are three types
*			- #define XCHAR   unsigned short		// use multibyte characters (0-2^16 range)
*			- #define XCHAR   unsigned char		// use unsigned char (0-255 range)
*			- #define XCHAR   char				// use signed char (0-127 range)
*
*********************************************************************/
    #if defined (USE_MULTIBYTECHAR) 
        #define XCHAR   unsigned short
    #elif defined (USE_UNSIGNED_XCHAR)
        #define XCHAR   unsigned char
    #else
        #define XCHAR   char
    #endif

// bevel drawing type (0 = full bevel, 0xF0 - top bevel only, 0x0F - bottom bevel only
extern BYTE _bevelDrawType;

	#define DRAWFULLBEVEL  	0xFF 
	#define DRAWTOPBEVEL  	0xF0 
	#define DRAWBOTTOMBEVEL 0x0F 

    #ifdef __PIC32MX__
        // Flash data with 32bit pointers
        #define FLASH_BYTE  const BYTE
        #define FLASH_WORD  const WORD
        #define FLASH_DWORD const DWORD
    #else
        // Flash data with 24bit pointers
        #define FLASH_BYTE  char __prog__
        #define FLASH_WORD  short int __prog__
        #define FLASH_DWORD unsigned long __prog__
    #endif

/*********************************************************************
* Overview: Memory type enumeration to determine the source of data.
*           Used in interpreting bitmap and font from different 
*           memory sources.
*
*********************************************************************/
typedef enum
{
	FLASH         = 0x0000, // internal flash
	EXTERNAL      = 0x0001, // external memory
	FLASH_JPEG    = 0x0002, // internal flash
	EXTERNAL_JPEG = 0x0003, // external memory
	RAM           = 0x0004, // RAM 
	EDS_EPMP      = 0x0005, // memory in EPMP, base addresses are 
                        	// are set in the hardware profile

	IMAGE_MBITMAP = 0x0000, // data resource is type Microchip bitmap
	IMAGE_JPEG    = 0x0100, // data resource is type JPEG

    COMP_NONE     = 0x0000, // no compression  
	COMP_RLE      = 0x1000, // compressed with RLE
	COMP_IPU      = 0x2000, // compressed with DEFLATE (for IPU)
} GFX_RESOURCE;

#define GFX_COMP_MASK   0xF000
#define GFX_MEM_MASK    0x000F

/*********************************************************************
* Overview: This structure is used to describe external memory.
*
*********************************************************************/
typedef struct
{
    GFX_RESOURCE type;       // must be EXTERNAL or EDS_EPMP
    WORD         ID;         // memory ID, user defined value to differentiate
                             // between graphics resources of the same type	
							 //   When using EDS_EPMP the following ID values are
							 //   reserved and used by the Microchip display driver
						     //   0 - reserved (do not use)
							 //   1 - reserved for base address of EPMP CS1
							 //   2 - reserved for base address of EPMP CS2
    DWORD        address;    // Data image address (user data, bitmap or font) 
} GFX_EXTDATA;

/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */
//                         STRUCTURES FOR IMAGES
/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */

/*********************************************************************
* Overview: Structure describing the bitmap header.
*
*********************************************************************/
typedef struct
{
    BYTE    compression;     // Compression setting 
    BYTE    colorDepth;      // Color depth used
    SHORT   height;          // Image height
    SHORT   width;           // Image width
} BITMAP_HEADER;

/*********************************************************************
* Overview: Structure for images stored in FLASH memory.
*
*********************************************************************/
typedef struct
{
    GFX_RESOURCE  type;      // must be FLASH
    FLASH_BYTE    *address;  // image address in FLASH
} IMAGE_FLASH;

/*********************************************************************
* Overview: Structure for images stored in RAM memory.
*
*********************************************************************/
typedef struct
{
    GFX_RESOURCE  type;      // must be RAM
    DWORD         *address;  // image address in RAM
} IMAGE_RAM;

/*********************************************************************
* Overview: Structure for images stored in EXTERNAL memory space.
*           (example: External SPI or parallel Flash, EDS_EPMP)
*
*********************************************************************/
typedef GFX_EXTDATA IMAGE_EXTERNAL;

/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */
//                         STRUCTURES FOR FONTS
/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */

/*********************************************************************
* Overview: Structure describing the font header.
*
*********************************************************************/
typedef struct
{
    BYTE fontID;             // User assigned value
    BYTE res1        : 4;    // Reserved for future use (must be set to 0).
    BYTE orientation : 2;    // Orientation of the character glyphs (0,90,180,270 degrees)
							 //   00 - Normal
							 //   01 - Characters rotated 270 degrees clockwise
							 //   10 - Characters rotated 180 degrees
							 //   11 - Characters rotated 90 degrees clockwise
    BYTE res2        : 2;    // Reserved for future use (must be set to 0).
    WORD firstChar;          // Character code of first character (e.g. 32).
    WORD lastChar;           // Character code of last character in font (e.g. 3006).
    BYTE height;             // Font characters height in pixels.
    BYTE reserved;           // Reserved for future use (must be set to 0).
} FONT_HEADER;

/*********************************************************************
* Overview: Structure describing the glyph entry.
*
*********************************************************************/
typedef struct
{
    BYTE    width;           // width of the glyph
    BYTE    offsetLSB;       // Least Significant Byte of the glyph location offset 
    WORD    offsetMSB;       // Most Significant (2) Bytes of the glyph location offset
} GLYPH_ENTRY;

/*********************************************************************
* Overview: Structure for font stored in FLASH memory.
*
*********************************************************************/
typedef struct
{
    GFX_RESOURCE  type;      // must be FLASH
    const char    *address;  // font image address in FLASH
} FONT_FLASH;

/*********************************************************************
* Overview: Structure for font stored in RAM memory.
*
*********************************************************************/
typedef struct
{
    GFX_RESOURCE  type;      // must be RAM
    char          *address;  // font image address in RAM
} FONT_RAM;

/*********************************************************************
* Overview: Structure for font stored in EXTERNAL memory space.
*           (example: External SPI or parallel Flash, EDS_EPMP)
*
*********************************************************************/
typedef GFX_EXTDATA FONT_EXTERNAL;

/*********************************************************************
* Overview: Structure for images stored in various system memory 
*           (Flash, External Memory (SPI, Parallel Flash, 
*           or memory in EPMP).
*
*********************************************************************/
typedef struct
{
    GFX_RESOURCE     type;             // Graphics resource type, determines the type and location of data
    WORD             ID;               // memory ID, user defined value to differentiate
                                       // between graphics resources of the same type	
							           //   When using EDS_EPMP the following ID values are
							           //   reserved and used by the Microchip display driver
									   //   0 - reserved (do not use)
							           //   1 - reserved for base address of EPMP CS1
							           //   2 - reserved for base address of EPMP CS2 
	union
	{
    	DWORD        extAddress;	   // generic address	
        FLASH_BYTE   *progByteAddress; // for addresses in program section
        FLASH_WORD   *progWordAddress; // for addresses in program section
	    const char   *constAddress;    // for addresses in FLASH
	    char         *ramAddress;      // for addresses in RAM
#if defined(__PIC24F__) 	    
	    __eds__ char *edsAddress;      // for addresses in EDS
#endif	    
	} LOCATION;
    
	WORD             width;            // width of the image 
	WORD             height;           // height of the image
    DWORD            param1;           // size of the IPU compressed data
    DWORD            param2;           // size of the IPU decompressed data
	WORD             colorDepth;       // color depth of the image
} GFX_IMAGE_HEADER;

/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */
//                         DEPRECATED TYPES and VARIABLES
/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */
typedef GFX_RESOURCE TYPE_MEMORY     __attribute__ ((deprecated));
typedef GFX_EXTDATA  EXTDATA         __attribute__ ((deprecated));
typedef IMAGE_FLASH  BITMAP_FLASH    __attribute__ ((deprecated));
typedef IMAGE_RAM    BITMAP_RAM      __attribute__ ((deprecated));
typedef GFX_EXTDATA  BITMAP_EXTERNAL __attribute__ ((deprecated));

/*********************************************************************
* Overview: This defines the size of the buffer used by font functions
*			to retrieve font data from the external memory. The buffer 
*			size can be increased to accommodate large font sizes. 
*			The user must be aware of the expected glyph sizes of the 
*			characters stored in the font table.
*
********************************************************************/
    #define EXTERNAL_FONT_BUFFER_SIZE   600

// Pointer to the current font image
extern void     *_font;

// First and last characters in the font
extern WORD     _fontFirstChar;
extern WORD     _fontLastChar;

// Installed font height
extern SHORT    _fontHeight;

/*********************************************************************
* Function: WORD Arc(SHORT xL, SHORT yT, SHORT xR, SHORT yB, 
*					 SHORT r1, SHORT r2, BYTE octant)
*
* Overview: Draws the octant arc of the beveled figure with the given centers, 
*           radii and octant mask.
*           When octant = 0xFF and the following are true:
*           1. xL = xR, yT = yB , r1 = 0 and r2 = z, a filled circle is drawn with a radius of z.
*           2. radii have values (where r1 < r2), a full ring with thickness of (r2-r1) is drawn.
*           3. xL != xR, yT != yB , r1 = 0 and r2 = 0 (where xR > xL and yB > yT) a rectangle is 
*              drawn. xL, yT specifies the left top corner and xR, yB specifies the right bottom corner.
*           When octant != 0xFF the figure drawn is the subsection of the 8 section figure where 
*           each non-zero bit of the octant value specifies the octants that will be drawn.
*
* Description: 
*          <img name="Arc.jpg" />
*
* PreCondition: none
*
* Input: xL - x location of the upper left center in the x,y coordinate.
*		 yT - y location of the upper left center in the x,y coordinate.
*		 xR - x location of the lower right center in the x,y coordinate.
*		 yB - y location of the lower right center in the x,y coordinate.
*		 r1 - The smaller radius of the two concentric cicles that defines the thickness
*			  of the object.
*	     r2 - The larger of radius the two concentric circles that defines the thickness 
*	     	  of the object.
*		 octant - Bitmask of the octant that will be drawn.
*				  Moving in a clockwise direction from x = 0, y = +radius
*                 - bit0 : first octant 
*                 - bit1 : second octant
*                 - bit2 : third octant 
*                 - bit3 : fourth octant
*                 - bit4 : fifth octant
*                 - bit5 : sixth octant
*                 - bit6 : seventh octant
*                 - bit7 : eight octant
*
* Output: Returns the rendering status.
*		  1 - If the rendering was completed and 
*		  0 - If the rendering is not yet finished. 
*
* Side Effects: none
*
********************************************************************/
WORD            Arc(SHORT xL, SHORT yT, SHORT xR, SHORT yB, SHORT r1, SHORT r2, BYTE octant);

/*********************************************************************
* Function:  void InitGraph(void)
*
* Overview: This function initializes the display controller, sets 
*			the line type to SOLID_LINE, sets the screen to all BLACK, 
*			sets the current drawing color to WHITE, sets the graphic 
*			cursor position to upper left corner of the screen, sets 
*			active and visual pages to page #0, clears the active page 
*			and disables clipping. This function should be called before 
*			using the Graphics Primitive Layer.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void            InitGraph(void);

/*********************************************************************
* Macros:  GetX()
*
* Overview: This macro returns the current graphic cursor x-coordinate.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define GetX()  _cursorX

/*********************************************************************
* Macros:  GetX()
*
* Overview: This macro returns the current graphic cursor y-coordinate.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define GetY()  _cursorY

/*********************************************************************
* Macros:  MoveTo(x,y)
*
* Overview: This macro moves the graphic cursor to new x,y position.
*
* PreCondition: none
*
* Input: x - Specifies the new x position of the graphic cursor.
*        y - Specifies the new y position of the graphic cursor.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define MoveTo(x, y) \
    _cursorX = x;        \
    _cursorY = y;

/*********************************************************************
* Macros:  MoveRel(dX,dY)
*
* Overview: This macro moves the graphic cursor relative to the 
*			current location. The given dX and dY displacement can 
*			be positive or negative numbers.
*
* PreCondition: none
*
* Input: dX - Specifies the displacement of the graphic cursor for
*			  the horizontal direction.
*		 dY - Specifies the displacement of the graphic cursor for
*			  the vertical direction.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define MoveRel(dX, dY) \
    _cursorX += dX;         \
    _cursorY += dY;

/*********************************************************************
* Macro: SetFontOrientation(orient)
*
* Overview: This macro sets font orientation vertical or horizontal.
*
* PreCondition: none
*
* Input: orient - should be non-zero if the font orientation is vertical
*
* Output: none
*
********************************************************************/
    #define SetFontOrientation(orient)  _fontOrientation = orient;

/*********************************************************************
* Macro: GetFontOrientation()
*
* Overview: This macro returns font orientation (0 == horizontal, 1 == vertical).
*
* PreCondition: none
*
* Input: none
*
* Output: font orientation (0 == horizontal, 1 == vertical)
*
********************************************************************/
    #define GetFontOrientation()    _fontOrientation

/*********************************************************************
* Function: WORD OutChar(XCHAR ch)
*
* Overview: This function outputs a character from the current graphic 
*		    cursor position. OutChar() uses the current active font 
*		    set with SetFont(). 
*
* PreCondition: none
*
* Input: ch - The character code to be displayed.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the character is not yet completely drawn.
*         - Returns 1 when the character is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
*
* Side Effects: After the function is completed, the graphic cursor 
*			    position is moved in the horizontal direction by the 
*			    character width. Vertical position of the graphic cursor 
*			    is not changed.
*
********************************************************************/
WORD    OutChar(XCHAR ch);

/*********************************************************************
* Function: WORD OutText(XCHAR* textString)
*
* Overview: This function outputs a string of characters starting 
*			at the current graphic cursor position. The string must 
*			be terminated by a line feed or zero. For Non-Blocking 
*			configuration, OutText() may return control to the program 
*			due to display device busy status. When this happens zero 
*			is returned and OutText() must be called again to continue 
*			the outputting of the string. For Blocking configuration, 
*			this function always returns a 1. OutText() uses the current 
*			active font set with SetFont().
*
*
* Input: textString - Pointer to the string to be displayed.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when string is not yet outputted completely.
*         - Returns 1 when string is outputted completely.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: Current horizontal graphic cursor position will be moved 
*				to the end of the text. The vertical graphic cursor 
*				position will not be changed.
*
********************************************************************/
WORD    OutText(XCHAR *textString);

/*********************************************************************
* Function: WORD OutTextXY(SHORT x, SHORT y, XCHAR* textString)
*
* Overview: This function outputs a string of characters starting 
*			at the given x, y position. The string must be terminated 
*			by a line feed or zero. For Non-Blocking configuration, 
*			OutTextXY() may return control to the program due to 
*			display device busy status. When this happens zero is 
*			returned and OutTextXY() must be called again to continue 
*			the outputting of the string. For Blocking configuration, 
*			this function always returns a 1. OutTextXY() uses the 
*			current active font set with SetFont().
*
* Input: x - Defines the x starting position of the string.
*		 y - Defines the y starting position of the string.
*		 textString - Pointer to the string to be displayed.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when string is not yet outputted completely.
*         - Returns 1 when string is outputted completely.
*         For Blocking configuration:
*         - Always return 1.
*
* Example:
*   <CODE> 
*	void PlaceText(void)
*	{
*		SHORT width, height;
*		static const XCHAR text[] = "Touch screen to continue";
*		
*		SetColor(BRIGHTRED);                // set color
*		SetFont(pMyFont);                   // set font to my font
*		
*		// get string width & height
*		width = GetTextWidth(text, pMyFont);
*		height = GetTextHeight(pMyFont);
*		
*		// place string in the middle of the screen
*		OutTextXY(  (GetMaxX() - width) >> 1, \
*		(GetMaxY() – height) >> 1, \
*		(char*)text);
*	}
*	</CODE> 
*
* Side Effects: Current horizontal graphic cursor position will be 
*				moved to the end of the text. The vertical graphic 
*				cursor position will not be changed.
*
********************************************************************/
WORD    OutTextXY(SHORT x, SHORT y, XCHAR *textString);

/*********************************************************************
* Function: SHORT GetTextHeight(void* font)
*
* Overview: This macro returns the height of the specified font. 
*			All characters in a given font table have a constant 
*			height.
*
* Input: font - Pointer to the font image.
*
* Output: Returns the font height.
*
* Example:
*   See OutTextXY() example.
*
* Side Effects: none
*
********************************************************************/
SHORT   GetTextHeight(void *font);

/*********************************************************************
* Function: SHORT GetTextWidth(XCHAR* textString, void* font)
*
* Overview: This function returns the width of the specified string 
*			for the specified font. The string must be terminated 
*			by a line feed or zero.
*
* Input: textString - Pointer to the string.
*		 font - Pointer to the font image.
*
* Output: Returns the string width in the specified font.
*
* Example:
*   See OutTextXY() example.
*
* Side Effects: none
*
********************************************************************/
SHORT   GetTextWidth(XCHAR *textString, void *font);

/*********************************************************************
* Function: void SetFont(void* font)
*
* Overview: This function sets the current font used in OutTextXY(), 
*			OutText() and OutChar() functions.
*
* Input: font - Pointer to the new font image to be used.
*
* Output: none
*
* Example:
*   See OutTextXY() example.
*
* Side Effects: none
*
********************************************************************/
void    SetFont(void *font);

/*********************************************************************
* Macros: SetLineType(lnType)
*
* Overview: This macro sets the line type to draw.
*
* Input: lnType - The type of line to be used. 
*		 	Supported line types:
*				- SOLID_LINE
* 				- DOTTED_LINE
*          		- DASHED_LINE
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define SetLineType(lnType) _lineType = lnType;

/*********************************************************************
* Macros: SetLineThickness(lnThickness)
*
* Overview: This macro sets sets line thickness to 1 pixel or 3 pixels.
*
* Input: lnThickness - Line thickness code (0 - 1 pixel; 1 - 3 pixels)
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define SetLineThickness(lnThickness)   _lineThickness = lnThickness;

/*********************************************************************
* Function: WORD Line(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
*
* Overview: This function draws a line with the current line type 
*	        from the start point to the end point.
*
* Input: x1 - x coordinate of the start point.
*		 y1 - y coordinate of the start point.
*		 x2 - x coordinate of the end point.
*		 y2 - y coordinate of the end point.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
*
* Side Effects: The graphic cursor position is moved to the end 
*				point of the line.
*
********************************************************************/
WORD Line(SHORT x1, SHORT y1, SHORT x2, SHORT y2);

/*********************************************************************
* Macros: LineRel(dX, dY)
*
* Overview: This macro draws a line with the current line type from 
*			the current graphic cursor position to the position defined 
*			by displacement.
*
* Input: dX - Displacement from the current x position.
*		 dY - Displacement from the current y position.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: The graphic cursor position is moved to the end 
*				point of the line.
*
********************************************************************/
    #define LineRel(dX, dY) Line(GetX(), GetY(), GetX() + dX, GetY() + dY)

/*********************************************************************
* Macros: LineTo(x,y)
*
* Overview: This macro draws a line with the current line type from 
*			the current graphic cursor position to the given x, y position.
*
* Input: x - End point x position.
*		 y - End point y poisiton.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: The graphic cursor position is moved to the end 
*				point of the line.
*
********************************************************************/
    #define LineTo(x, y)    Line(_cursorX, _cursorY, x, y)

/*********************************************************************
* Macro: Circle(x, y, radius)
*
* Overview: This macro draws a circle with the given center and radius.
*
* Input: x - Center x position. 
*		 y - Center y position.
*		 radius - the radius of the circle.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
********************************************************************/
    #define Circle(x, y, radius)    Bevel(x, y, x, y, radius)

/*********************************************************************
* Macro: SetBevelDrawType(type)
*
* Overview: This macro sets the fill bevel type to be drawn.
*
* Input: type - is set using the following. 
*		 - DRAWFULLBEVEL to draw the full shape
*		 - DRAWTOPBEVEL to draw the upper half portion
*		 - DRAWBOTTOMBEVEL to draw the lower half portion
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
	#define SetBevelDrawType(type)		(_bevelDrawType = type)

/*********************************************************************
* Function: WORD Bevel(SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT rad)
*
* Overview: Draws a beveled figure on the screen. 
*           When x1 = x2 and y1 = y2, a circular object is drawn. 
*           When x1 < x2 and y1 < y2 and rad (radius) = 0, a rectangular 
*           object is drawn.
*
* Description: 
*        <img name="Bevel.jpg" />
*
* Input: x1 - x coordinate position of the upper left center of the circle that 
*			  draws the rounded corners.
*		 y1 - y coordinate position of the upper left center of the circle that 
*			  draws the rounded corners.
*		 x2 - x coordinate position of the lower right center of the circle that 
*			  draws the rounded corners.
*		 y2 - y coordinate position of the lower right center of the circle that 
*			  draws the rounded corners.
*        rad - defines the redius of the circle, that draws the rounded corners.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
*
* Side Effects: none
*
********************************************************************/
WORD    Bevel(SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT rad);

/*********************************************************************
* Function: WORD FillBevel(SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT rad)
*
* Overview: Draws a filled beveled figure on the screen. 
*           For a filled circular object x1 = x2 and y1 = y2. 
*           For a filled rectangular object radius = 0.
*
* Description: 
*        <img name="FillBevel.jpg" />
*
* Input: x1 - x coordinate position of the upper left center of the circle that 
*			  draws the rounded corners.
*		 y1 - y coordinate position of the upper left center of the circle that 
*			  draws the rounded corners.
*		 x2 - x coordinate position of the lower right center of the circle that 
*			  draws the rounded corners.
*		 y2 - y coordinate position of the lower right center of the circle that 
*			  draws the rounded corners.
*        rad - defines the redius of the circle, that draws the rounded corners.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
********************************************************************/
WORD    FillBevel(SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT rad);

/*********************************************************************
* Macro: FillCircle(SHORT x1, SHORT y1, SHORT rad)
*
* Overview: This macro draws a filled circle. Uses the FillBevel() function.
*
* Input: x1 - x coordinate position of the center of the circle.
*		 y1 - y coordinate position of the center of the circle.
*        rad - defines the redius of the circle.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
********************************************************************/
    #define FillCircle(x1, y1, rad) FillBevel(x1, y1, x1, y1, rad)

/*********************************************************************
* Macro: Rectangle(left, top, right, bottom)
*
* Overview: This macro draws a rectangle with the given left, 
*			top and right, bottom corners. Current line type is used.
*
* Input: left - x position of the left top corner.
*		 top - y position of the left top corner.
*		 right - x position of the right bottom corner.
*		 bottom - y position of the right bottom corner.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
********************************************************************/
    #define Rectangle(left, top, right, bottom) Bevel(left, top, right, bottom, 0)

/*********************************************************************
* Function: WORD DrawPoly(SHORT numPoints, SHORT* polyPoints)
*
* Overview: This function draws a polygon with the current line 
*			type using the given number of points. The polygon points 
*			are stored in an array arranged in the following order:
*   <PRE> 
*            SHORT polyPoints[numPoints] = {x0, y0, x1, y1, x2, y2 … xn, yn};
*            Where n = numPoints - 1
*	</PRE> 
*
* Input: numPoints - Defines the number of points in the polygon.
*		 polyPoints - Pointer to the array of polygon points.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
*
* Side Effects: none
*
********************************************************************/
WORD    DrawPoly(SHORT numPoints, SHORT *polyPoints);

/*********************************************************************
* Function: WORD Bar(SHORT left, SHORT top, SHORT right, SHORT bottom)
*
* Overview: This function draws a bar given the left, top and right, 
*			bottom corners with the current color.
*
* Input: left - x position of the left top corner.
*		 top - y position of the left top corner.
*		 right - x position of the right bottom corner.
*		 bottom - y position of the right bottom corner.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
*
* Side Effects: none
*
********************************************************************/
WORD Bar(SHORT left, SHORT top, SHORT right, SHORT bottom);

/*********************************************************************
* Function: void ClearDevice(void)
*
* Overview: This function clears the screen with the current color 
*			and sets the graphic cursor position to (0, 0). 
*			Clipping is NOT supported by ClearDevice().
*
* Input: none
*
* Output: none
*
* Example:
*	<CODE> 
*	void ClearScreen(void)
*	{
*		SetColor(WHITE);		// set color to WHITE
*		ClearDevice();			// set screen to all WHITE
*	}
*   </CODE>
* 
* Side Effects: none
*
********************************************************************/
void    ClearDevice(void);

/*********************************************************************
* Function: WORD PutImage(SHORT left, SHORT top, void* bitmap, BYTE stretch)
*
* Overview: This function outputs image starting from left,top coordinates.
*
* Input: left - x coordinate position of the left top corner.
*		 top - y coordinate position of the left top corner.
*        bitmap - pointer to the bitmap.
*        stretch - The image stretch factor.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the image is not yet completely drawn.
*         - Returns 1 when the image is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* 
* Side Effects: none
*
********************************************************************/
WORD    PutImage(SHORT left, SHORT top, void *bitmap, BYTE stretch);

/*********************************************************************
* Function: SHORT GetImageWidth(void* bitmap)
*
* Overview: This function returns the image width.
*
* Input: bitmap - Pointer to the bitmap.
*
* Output: Returns the image width in pixels.
* 
* Side Effects: none
*
********************************************************************/
SHORT   GetImageWidth(void *bitmap);

/*********************************************************************
* Function: SHORT GetImageHeight(void* bitmap)
*
* Overview: This function returns the image height.
*
* Input: bitmap - Pointer to the bitmap.
*
* Output: Returns the image height in pixels.
* 
* Side Effects: none
*
********************************************************************/
SHORT   GetImageHeight(void *bitmap);

/*********************************************************************
* Function: WORD ExternalMemoryCallback(GFX_EXTDATA* memory, LONG offset, WORD nCount, void* buffer)
*
* Overview: This function must be implemented in the application. 
*           The library will call this function each time when
*           the external memory data will be required. The application
*           must copy requested bytes quantity into the buffer provided.
*           Data start address in external memory is a sum of the address
*           in GFX_EXTDATA structure and offset.
*
* Input:  memory - Pointer to the external memory bitmap or font structures
*                  (FONT_EXTERNAL or BITMAP_EXTERNAL).
*         offset - Data offset.
*         nCount - Number of bytes to be transferred into the buffer.
*         buffer - Pointer to the buffer.
*
* Output: Returns the number of bytes were transferred.
* 
* Example:
*   <CODE>
*   // If there are several memories in the system they can be selected by IDs.
*   // In this example, ID for memory device used is assumed to be 0.
*   #define X_MEMORY 0
*
*   WORD ExternalMemoryCallback(GFX_EXTDATA* memory, LONG offset, WORD nCount, void* buffer) {
*   	int i;
*       long address;
*
*		// Address of the requested data is a start address of the object referred by GFX_EXTDATA structure plus offset
*		address = memory->address+offset;
*
*       if(memory->ID == X_MEMORY){
*       	// MemoryXReadByte() is some function implemented to access external memory. 
*           // Implementation will be specific to the memory used. In this example 
*			// it reads byte each time it is called. 
*           i = 0;
*           while (i < nCount) {
*           	(BYTE*)buffer = MemoryXReadByte(address++);
*               i++;
*           }
*      }
*      // return the actual number of bytes retrieved
*      return (i);
*  }
*  </CODE>
*
* Side Effects: none
*
********************************************************************/
WORD    ExternalMemoryCallback(GFX_EXTDATA *memory, LONG offset, WORD nCount, void *buffer);

/*********************************************************************
* Function:  SHORT GetSineCosine(SHORT v, WORD type)
*
* PreCondition: none
*
* Input: v - the angle used to retrieve the sine or cosine value. 
*			 The angle must be in the range of -360 to 360 degrees.
*		 type - sets if the angle calculation is for a sine or cosine
*				- GETSINE (0) - get the value of sine(v).
*				- GETCOSINE (1) - return the value of cosine(v).
*
* Output: Returns the sine or cosine of the angle given.
*
* Side Effects: none
*
* Overview: Using a lookup table, the sine or cosine values of the given angle
*           is returned.
*
* Note: none
*
********************************************************************/
SHORT GetSineCosine(SHORT v, WORD type);

/*********************************************************************
* Function:  SHORT Sine(SHORT v)
*
* PreCondition: none
*
* Input: v - the angle used to calculate the sine value. 
*			 The angle must be in the range of -360 to 360 degrees.
*
* Output: Returns the sine of the given angle.
*
* Side Effects: none
*
* Overview: This calculates the sine value of the given angle.
*
* Note: none
*
********************************************************************/
#define Sine(v)		GetSineCosine(v, GETSINE)

/*********************************************************************
* Function:  SHORT Cosine(SHORT v)
*
* PreCondition: none
*
* Input: v - the angle used to calculate the cosine value. 
*			 The angle must be in the range of -360 to 360 degrees.
*
* Output: Returns the cosine of the given angle.
*
* Side Effects: none
*
* Overview: This calculates the cosine value of the given angle.
*
* Note: none
*
********************************************************************/
#define Cosine(v)		GetSineCosine(v, GETCOSINE)


/*********************************************************************
* Function:  WORD DrawArc(SHORT cx, SHORT cy, SHORT r1, SHORT r2, SHORT startAngle, SHORT endAngle)
*
* Overview: This renders an arc with from startAngle to endAngle with the thickness 
*		    of r2-r1. The function returns 1 when the arc is rendered successfuly
* 			and returns a 0 when it is not yet finished. The next call to the 
*			function will continue the rendering.
*
* PreCondition: none
*
* Input: cx - the location of the center of the arc in the x direction. 
*	     cy - the location of the center of the arc in the y direction. 		 
*	     r1 - the smaller radius of the arc. 		 
*	     r2 - the larger radius of the arc. 		 
*	     startAngle - start angle of the arc. 		 
*	     endAngle - end angle of the arc. 		 
*
* Output: Returns 1 if the rendering is done, 0 if not yet done.
*
* Side Effects: none
*
* Note: none
*
********************************************************************/
WORD DrawArc(SHORT cx, SHORT cy, SHORT r1, SHORT r2, SHORT startAngle, SHORT endAngle);

void GetCirclePoint(SHORT radius, SHORT angle, SHORT *x, SHORT *y);

#ifdef USE_GRADIENT

/*********************************************************************
* Function:  WORD BarGradient(SHORT left, SHORT top, SHORT right, SHORT bottom, 
*                        GFX_COLOR color1, GFX_COLOR color2, DWORD length, 
*                        BYTE direction);
*
* Overview: This renders a bar onto the screen, but instead of one color a gradient is drawn
* depending on the direction (GFX_GRADIENT_TYPE), length, and colors chosen
*
* Description: 
*        <img name="BarGradient.jpg" />
*
* PreCondition: none
*
* Input: left - x position of the left top corner.
*		 top - y position of the left top corner.
*		 right - x position of the right bottom corner.
*		 bottom - y position of the right bottom corner.		 
*	     color1 - start color for the gradient
*        color2 - end color for the gradient
*        length - From 0-100%. How much of a gradient is wanted
*        direction - Gradient Direction	
*
* Output: Returns 1 if the rendering is done, 0 if not yet done.
*
* Example:
*   <CODE> 
*   // draw a full screen gradient background
*   // with color transitioning from BRIGHTRED to 
*   // BLACK in the upward direction.  
*
*   GFX_GRADIENT_STYLE  gradScheme;
*
*   gradScheme.gradientType         = GRAD_UP; 
*   gradScheme.gradientStartColor   = BRIGHTRED;
*   gradScheme.gradientEndColor     = BLACK;
* 
*    BarGradient(0,                                         //left position 
*                0,                                         //top position
*                GetMaxX(),                                 //right position
*                GetMaxY(),                                 //bottom position
*                0,                                         // no radius, since full screen
*                gradScheme.gradientStartColor,
*                gradScheme.gradientEndColor,
*                50,                                        // at the halfway point (50%) of the rectangular area 
*                                                           // defined by the first 4 parameters (full screen), 
*                                                           // the color becomes BLACK and BLACK color is used until 
*                                                           // the rectangle defined is filled up 
*                gradScheme.gradientType);                  // gradient direction is bottom->top
*	</CODE> 
*
* Side Effects: none
*
* Note: none
*
********************************************************************/
WORD        BarGradient(SHORT left, SHORT top, SHORT right, SHORT bottom, GFX_COLOR color1, GFX_COLOR color2, DWORD length, BYTE direction);

/*********************************************************************
* Function:  BevelGradient(SHORT left, SHORT top, SHORT right, SHORT bottom, 
*                          SHORT rad, GFX_COLOR color1, GFX_COLOR color2, 
*                          DWORD length, BYTE direction);
*
* Overview: This renders a gradient on the screen. It works the same as the fillbevel function, 
* except a gradient out of color1 and color2 is drawn depending on the direction (GFX_GRADIENT_TYPE).
*
* Description: 
*        <img name="BevelGradient.jpg" />
*
* PreCondition: none
*
* Input: left - x coordinate position of the upper left center of the circle that 
*			  draws the rounded corners.
*		 top - y coordinate position of the upper left center of the circle that 
*			  draws the rounded corners.
*		 right - x coordinate position of the lower right center of the circle that 
*			  draws the rounded corners.
*		 bottom - y coordinate position of the lower right center of the circle that 
*			  draws the rounded corners.
*        rad - defines the redius of the circle, that draws the rounded corners. When 
*              rad = 0, the object drawn is a rectangular gradient.
*	     color1 - start color for the gradient
*        color2 - end color for the gradient
*        length - From 0-100%. How much of a gradient is wanted
*        direction - Gradient Direction		 
*
* Output: Returns 1 if the rendering is done, 0 if not yet done.
*
* Side Effects: none
*
* Note: none
*
********************************************************************/
WORD        BevelGradient(SHORT left, SHORT top, SHORT right, SHORT bottom, SHORT rad, GFX_COLOR color1, GFX_COLOR color2, DWORD length, BYTE direction);

/*********************************************************************
* Overview: Enumeration for gradient type
*********************************************************************/
typedef enum
{
       GRAD_NONE=0,                     // No Gradients to be drawn
       GRAD_DOWN,                       // gradient changes in the vertical direction
       GRAD_RIGHT,                      // gradient change in the horizontal direction
       GRAD_UP,                         // gradient changes in the vertical direction
       GRAD_LEFT,                       // gradient change in the horizontal direction
       GRAD_DOUBLE_VER,                 // two gradient transitions in the vertical direction
       GRAD_DOUBLE_HOR,                 // two gradient transitions in the horizontal direction

} GFX_GRADIENT_TYPE;

/*********************************************************************
* Overview: This structure is used to describe the gradient style.
*
*********************************************************************/
typedef struct
{

GFX_GRADIENT_TYPE  gradientType;        // selected the gradient type 
DWORD              gradientStartColor;  // sets the starting color of gradient transition
DWORD              gradientEndColor;    // sets the ending color of gradient transition
DWORD              gradientLength;      // defines the length of the gradient transition in pixels

} GFX_GRADIENT_STYLE;
#endif

#ifdef USE_ALPHABLEND
extern SHORT _GFXForegroundPage;        // foreground page (or buffer) used in alpha blending
extern SHORT _GFXBackgroundPage;        // background page (or buffer)  used in alpha blending
extern SHORT _GFXDestinationPage;       // destination page (or buffer)  used in alpha blending


/*********************************************************************
* Function: void AlphaBlendWindow(DWORD foregroundWindowAddr, DWORD backgroundWindowAddr,
*					  DWORD destinationWindowAddr,		            
*					  WORD  width, WORD height,  	
*					  BYTE  alphaPercentage)
* PreCondition: none
*
* Input:  foregroundWindowAddr -  the starting address of the foreground window
*	    backgroundWindowAddr -  the starting address of the background window
*	    destinationWindowAddr - the starting address of the destination window
*	    width - the width of the alpha blend window
*         height - the height of the alpha blend window
*         alphaPercentage - the amount of transparency to give the foreground Window
*        		 
*
* Output: none
*
* Side Effects: none
*
* Overview: This alphablends a foreground and a background stored in frames to a destination window. The function
* uses windows insides frames. Each window shares the same width and height parameters.
*
* Note: none
********************************************************************/
extern void AlphaBlendWindow(DWORD foregroundWindowAddr, DWORD backgroundWindowAddr,
					  DWORD destinationWindowAddr,		            
					  WORD  width, WORD height,  	
					  BYTE  alphaPercentage);

/*********************************************************************
* Function: DWORD GFXGetPageXYAddress(SHORT pageNumber, WORD x, WORD y)
* PreCondition: none
*
* Input:  pageNumber - the page number 
*         x - the x (horizontal) offset from 0,0 of the pagenumber
*         y - the y (vertical) offset from the 0,0 of the pagenumber
*
* Output: The address of an XY position of a certain page in memory
*
* Side Effects: none
*
* Overview: This function calculates the address of a certain x,y location in 
* memory
*
* Note: none
********************************************************************/
extern DWORD GFXGetPageXYAddress(SHORT pageNumber, WORD x, WORD y);

/*********************************************************************
* Function: DWORD GFXGetPageOriginAddress(SHORT pageNumber)
* PreCondition: none
*
* Input:  pageNumber - the page number 
*
* Output: The address of the start of a certain page in memory
*
* Side Effects: none
*
* Overview: This function calculates the address of a certain 0,0 location of a 
* page in memory
*
* Note: none
********************************************************************/
extern DWORD GFXGetPageOriginAddress(SHORT pageNumber);

#endif


#endif // _PRIMITIVE_H
