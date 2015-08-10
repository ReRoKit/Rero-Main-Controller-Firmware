/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Graphic Primitives Layer
 *****************************************************************************
 * FileName:        Primitive.c
 * Processor:       PIC24, dsPIC, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
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
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 11/12/07     Version 1.0 release
 * 05/11/10     Added dynamic Arc() where start and end
 *									angle can be specified.
 * 03/04/11     - Removed SetColor(WHITE) in InitGraph(). Default color
 *                after InitGraph() is not 0 (BLACK in most displays).
 *              - removed USE_DRV_XX checks, replaced them with 
 *                weak attributes.
 * 05/13/11     Add Transparent Color support in PutImage() and PutImageRLE()
 *              functions defined in this layer. 
 * 05/20/11     Added GetCirclePoint() commonly used in Widgets.
 *           
 *****************************************************************************/
#include "HardwareProfile.h"              // needed to provide values for GetMaxX() and GetMaxY() macros
#include "Graphics/DisplayDriver.h"
#include "Graphics/Primitive.h"
#include "Compiler.h" 

/////////////////////// LOCAL FUNCTIONS PROTOTYPES ////////////////////////////
#ifdef USE_BITMAP_FLASH
    void    PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch);
    void    PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch);
    void    PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch);
    void    PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch);        

#ifdef USE_COMP_RLE
    void    PutImageRLE4BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch);
    void    PutImageRLE8BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch);
#endif
#endif

#ifdef USE_BITMAP_EXTERNAL
    void    PutImage1BPPExt(SHORT left, SHORT top, void *image, BYTE stretch);
    void    PutImage4BPPExt(SHORT left, SHORT top, void *image, BYTE stretch);
    void    PutImage8BPPExt(SHORT left, SHORT top, void *image, BYTE stretch);
    void    PutImage16BPPExt(SHORT left, SHORT top, void *image, BYTE stretch);

#ifdef USE_COMP_RLE
    void    PutImageRLE4BPPExt(SHORT left, SHORT top, void *image, BYTE stretch);
    void    PutImageRLE8BPPExt(SHORT left, SHORT top, void *image, BYTE stretch);
#endif
#endif

// Current line type
SHORT   _lineType;

// Current line thickness
BYTE    _lineThickness;

// Font orientation
BYTE    _fontOrientation;

// Current cursor x-coordinates
SHORT   _cursorX;

// Current cursor y-coordinates
SHORT   _cursorY;

// Pointer to the current font
void    *_font;

#ifdef USE_PALETTE
void    *_palette;
#endif

// First and last characters in the font
WORD    _fontFirstChar; // First character in the font table.
WORD    _fontLastChar;  // Last character in the font table.

// Installed font height
SHORT   _fontHeight;

// bevel drawing type (0 = full bevel, 0xF0 - top bevel only, 0x0F - bottom bevel only
BYTE _bevelDrawType; 


#define COSINETABLEENTRIES	90
// Cosine table used to calculate angles when rendering circular objects and  arcs  
// Make cosine values * 256 instead of 100 for easier math later
const SHORT   _CosineTable[COSINETABLEENTRIES+1] __attribute__((aligned(2))) = 
						{	
							256, 256, 256, 256, 255, 255, 255, 254, 254, 253,
							252, 251, 250, 249, 248, 247, 246, 245, 243, 242,
							241, 239, 237, 236, 234, 232, 230, 228, 226, 224,
							222, 219, 217, 215, 212, 210, 207, 204, 202, 199,
							196, 193, 190, 187, 184, 181, 178, 175, 171, 168,
							165, 161, 158, 154, 150, 147, 143, 139, 136, 132,
							128, 124, 120, 116, 112, 108, 104, 100, 96,  92,
							88,  83,  79,  75,  71,  66,  62,  58,  53,  49,
							44,  40,  36,  31,  27,  22,  18,  13,  9,   4, 
							0 
						};

/*********************************************************************
* Function: WORD Bar(SHORT left, SHORT top, SHORT right, SHORT bottom)
*
* PreCondition: none
*

* Input: left,top - top left corner coordinates,
*        right,bottom - bottom right corner coordinates
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: draws rectangle filled with current color
*
* Note: none
*
********************************************************************/
WORD __attribute__((weak)) Bar(SHORT left, SHORT top, SHORT right, SHORT bottom)
{
    SHORT   x, y;

        #ifndef USE_NONBLOCKING_CONFIG
    while(IsDeviceBusy() != 0) Nop();

    /* Ready */
        #else
    if(IsDeviceBusy() != 0)
        return (0);
        #endif
    for(y = top; y < bottom + 1; y++)
        for(x = left; x < right + 1; x++)
            PutPixel(x, y);

    return (1);
}

/*********************************************************************
* Function: WORD Line(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
*
* PreCondition: none
*
* Input: x1,y1 - starting coordinates, x2,y2 - ending coordinates
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: draws line
*
* Note: none
*
********************************************************************/
WORD __attribute__((weak)) Line(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
{
    SHORT   deltaX, deltaY;
    SHORT   error, stepErrorLT, stepErrorGE;
    SHORT   stepX, stepY;
    SHORT   steep;
    SHORT   temp;
    SHORT   style, type;

        #ifndef USE_NONBLOCKING_CONFIG
    while(IsDeviceBusy() != 0) Nop();

    /* Ready */
        #else
    if(IsDeviceBusy() != 0)
        return (0);
        #endif

    // Move cursor
    MoveTo(x2, y2);

    if(x1 == x2)
    {
        if(y1 > y2)
        {
            temp = y1;
            y1 = y2;
            y2 = temp;
        }

        style = 0;
        type = 1;
        for(temp = y1; temp < y2 + 1; temp++)
        {
            if((++style) == _lineType)
            {
                type ^= 1;
                style = 0;
            }

            if(type)
            {
                PutPixel(x1, temp);
                if(_lineThickness)
                {
                    PutPixel(x1 + 1, temp);
                    PutPixel(x1 - 1, temp);
                }
            }
        }

        return (1);
    }

    if(y1 == y2)
    {
        if(x1 > x2)
        {
            temp = x1;
            x1 = x2;
            x2 = temp;
        }

        style = 0;
        type = 1;
        for(temp = x1; temp < x2 + 1; temp++)
        {
            if((++style) == _lineType)
            {
                type ^= 1;
                style = 0;
            }

            if(type)
            {
                PutPixel(temp, y1);
                if(_lineThickness)
                {
                    PutPixel(temp, y1 + 1);
                    PutPixel(temp, y1 - 1);
                }
            }
        }

        return (1);
    }

    stepX = 0;
    deltaX = x2 - x1;
    if(deltaX < 0)
    {
        deltaX = -deltaX;
        --stepX;
    }
    else
    {
        ++stepX;
    }

    stepY = 0;
    deltaY = y2 - y1;
    if(deltaY < 0)
    {
        deltaY = -deltaY;
        --stepY;
    }
    else
    {
        ++stepY;
    }

    steep = 0;
    if(deltaX < deltaY)
    {
        ++steep;
        temp = deltaX;
        deltaX = deltaY;
        deltaY = temp;
        temp = x1;
        x1 = y1;
        y1 = temp;
        temp = stepX;
        stepX = stepY;
        stepY = temp;
        PutPixel(y1, x1);
    }
    else
    {
        PutPixel(x1, y1);
    }

    // If the current error greater or equal zero
    stepErrorGE = deltaX << 1;

    // If the current error less than zero
    stepErrorLT = deltaY << 1;

    // Error for the first pixel
    error = stepErrorLT - deltaX;

    style = 0;
    type = 1;

    while(--deltaX >= 0)
    {
        if(error >= 0)
        {
            y1 += stepY;
            error -= stepErrorGE;
        }

        x1 += stepX;
        error += stepErrorLT;

        if((++style) == _lineType)
        {
            type ^= 1;
            style = 0;
        }

        if(type)
        {
            if(steep)
            {
                PutPixel(y1, x1);
                if(_lineThickness)
                {
                    PutPixel(y1 + 1, x1);
                    PutPixel(y1 - 1, x1);
                }
            }
            else
            {
                PutPixel(x1, y1);
                if(_lineThickness)
                {
                    PutPixel(x1, y1 + 1);
                    PutPixel(x1, y1 - 1);
                }
            }
        }
    }   // end of while

    return (1);
}

/*********************************************************************
* Function: void ClearDevice(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: clears screen with current color and sets cursor to 0,0
*
* Note: none
*
********************************************************************/
void __attribute__((weak)) ClearDevice(void)
{
    while(Bar(0, 0, GetMaxX(), GetMaxY()) == 0);
    MoveTo(0, 0);
}

/*********************************************************************
* Function:  void InitGraph(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: initializes LCD controller,
*           sets cursor position to upper left corner,
*           sets active and visual pages to page 0,
*           clears active page with BLACK,
*           sets color to WHITE,
*           disables clipping
*
* Note: none
*
********************************************************************/
void InitGraph(void)
{

    // Current line type
    SetLineType(SOLID_LINE);

    // Current line thickness
    SetLineThickness(NORMAL_LINE);

    // Current cursor coordinates to 0,0
    MoveTo(0, 0);

    // Reset device
    ResetDevice();

    // Set color to BLACK
    SetColor(0);

    // set the transparent color check to be disabled
#ifdef USE_TRANSPARENT_COLOR
    TransparentColorDisable();
#endif    

    // Clear screen
    ClearDevice();

    // Disable clipping
    SetClip(CLIP_DISABLE);

    // Set font orientation
    SetFontOrientation(ORIENT_HOR);
    
    // set Bevel drawing 
    SetBevelDrawType(DRAWFULLBEVEL);
}

/*********************************************************************
* Function: WORD Arc(SHORT xL, SHORT yT, SHORT xR, SHORT yB, SHORT r1, SHORT r2, BYTE octant);
*
* PreCondition: none
*
* Input: xL, yT - location of the upper left center in the x,y coordinate
*		 xR, yB - location of the lower right left center in the x,y coordinate
*		 r1, r2 - the two concentric circle radii, r1 as the radius 
*				  of the smaller circle and and r2 as the radius of the 
*				  larger circle.
*		 octant - bitmask of the octant that will be drawn.
*				  Moving in a clockwise direction from x = 0, y = +radius
*                 bit0 : first octant 	bit4 : fifth octant
*                 bit1 : second octant  bit5 : sixth octant
*                 bit2 : third octant   bit6 : seventh octant
*                 bit3 : fourth octant  bit7 : eigth octant
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: Draws the octant arc of a beveled figure with given centers, radii
*			and octant mask. When r1 is zero and r2 has some value, a filled 
*			circle is drawn; when the radii have values, an arc of
*			thickness (r2-r1) is drawn; when octant = 0xFF, a full ring 
*			is drawn. When r1 and r2 are zero, a rectangular object is drawn, where
*			xL, yT specifies the left top corner; xR, yB specifies the right bottom
*			corner.
*
* Note: none
*
********************************************************************/
WORD __attribute__((weak)) Arc(SHORT xL, SHORT yT, SHORT xR, SHORT yB, SHORT r1, SHORT r2, BYTE octant)
{

    // this is using a variant of the Midpoint (Bresenham's) Algorithm
    #ifndef USE_NONBLOCKING_CONFIG

    SHORT       y1Limit, y2Limit;
    SHORT       x1, x2, y1, y2;
    SHORT       err1, err2;
    SHORT       x1Cur, y1Cur, y1New;
    SHORT       x2Cur, y2Cur, y2New;
    DWORD_VAL   temp;

    temp.Val = SIN45 * r1;
    y1Limit = temp.w[1];
    temp.Val = SIN45 * r2;
    y2Limit = temp.w[1];

    temp.Val = (DWORD) (ONEP25 - ((LONG) r1 << 16));
    err1 = (SHORT) (temp.w[1]);

    temp.Val = (DWORD) (ONEP25 - ((LONG) r2 << 16));
    err2 = (SHORT) (temp.w[1]);

    x1 = r1;
    x2 = r2;
    y1 = 0;
    y2 = 0;

    x1Cur = x1;
    y1Cur = y1;
    y1New = y1;
    x2Cur = x2;
    y2Cur = y2;
    y2New = y2;

    while(y2 <= y2Limit)
    {   // just watch for y2 limit since outer circle
        // will have greater value.
        // Drawing of the rounded panel is done only when there is a change in the
        // x direction. Bars are drawn to be efficient.
        // detect changes in the x position. Every change will mean a bar will be drawn
        // to cover the previous area. y1New records the last position of y before the
        // change in x position.
        // y1New & y2New records the last y positions, must remember this to
        // draw the correct bars (non-overlapping).
        y1New = y1;
        y2New = y2;

        if(y1 <= y1Limit)
        {
            if(err1 > 0)
            {
                x1--;
                err1 += 5;
                err1 += (y1 - x1) << 1;
            }
            else
            {
                err1 += 3;
                err1 += y1 << 1;
            }

            y1++;
        }
        else
        {
            y1++;
            if(x1 < y1)
                x1 = y1;
        }

        if(err2 > 0)
        {
            x2--;
            err2 += 5;
            err2 += (y2 - x2) << 1;
        }
        else
        {
            err2 += 3;
            err2 += y2 << 1;
        }

        y2++;

        if((x1Cur != x1) || (x2Cur != x2))
        {
            if(octant & 0x01)
            {
                Bar(xR + y2Cur, yT - x2Cur, xR + y1New, yT - x1Cur);    // 1st octant
            }

            if(octant & 0x02)
            {
                Bar(xR + x1Cur, yT - y1New, xR + x2Cur, yT - y2Cur);    // 2nd octant
            }

            if(octant & 0x04)
            {
                Bar(xR + x1Cur, yB + y1Cur, xR + x2Cur, yB + y2New);    // 3rd octant
            }

            if(octant & 0x08)
            {
                Bar(xR + y1Cur, yB + x1Cur, xR + y2New, yB + x2Cur);    // 4th octant
            }

            if(octant & 0x10)
            {
                Bar(xL - y1New, yB + x1Cur, xL - y2Cur, yB + x2Cur);    // 5th octant
            }

            if(octant & 0x20)
            {
                Bar(xL - x2Cur, yB + y2Cur, xL - x1Cur, yB + y1New);    // 6th octant
            }

            if(octant & 0x40)
            {
                Bar(xL - x2Cur, yT - y2New, xL - x1Cur, yT - y1Cur);    // 7th octant
            }

            if(octant & 0x80)
            {
                Bar(xL - y2New, yT - x2Cur, xL - y1Cur, yT - x1Cur);    // 8th octant
            }

            // update current values
            x1Cur = x1;
            y1Cur = y1;
            x2Cur = x2;
            y2Cur = y2;
        }
    }                           // end of while loop

    // draw the width and height
    if((xR - xL) || (yB - yT))
    {

        // draw right
        if(octant & 0x02)
        {
            Bar(xR + r1, yT, xR + r2, (yB + yT) >> 1);
        }

        if(octant & 0x04)
        {
            Bar(xR + r1, ((yB + yT) >> 1), xR + r2, yB);
        }

        // draw bottom
        if(octant & 0x10)
        {
            Bar(xL, yB + r1, ((xR + xL) >> 1), yB + r2);
        }

        if(octant & 0x08)
        {
            Bar(((xR + xL) >> 1), yB + r1, xR, yB + r2);
        }

        if(xR - xL)
        {

            // draw top
            if(octant & 0x80)
            {
                Bar(xL, yT - r2, ((xR + xL) >> 1), yT - r1);
            }

            if(octant & 0x01)
            {
                Bar(((xR + xL) >> 1), yT - r2, xR, yT - r1);
            }
        }

        if(yT - yB)
        {

            // draw left
            if(octant & 0x40)
            {
                Bar(xL - r2, yT, xL - r1, ((yB + yT) >> 1));
            }

            if(octant & 0x20)
            {
                Bar(xL - r2, ((yB + yT) >> 1), xL - r1, yB);
            }
        }
    }

    return (1);
    #else

    typedef enum __attribute__((packed)) {
        BEGIN,
        QUAD11,
        BARRIGHT1,
        QUAD12,
        BARRIGHT2,
        QUAD21,
        BARLEFT1,
        QUAD22,
        BARLEFT2,
        QUAD31,
        BARTOP1,
        QUAD32,
        BARTOP2,
        QUAD41,
        BARBOTTOM1,
        QUAD42,
        BARBOTTOM2,
        CHECK,
    } OCTANTARC_STATES;

    DWORD_VAL temp;

    //	LONG temp1;
    static SHORT y1Limit, y2Limit;
    static SHORT x1, x2, y1, y2;
    static SHORT err1, err2;
    static SHORT x1Cur, y1Cur, y1New;
    static SHORT x2Cur, y2Cur, y2New;
    static OCTANTARC_STATES state = BEGIN;

    while(1)
    {
        if(IsDeviceBusy())
            return (0);
        switch(state)
        {
            case BEGIN:
                temp.Val = SIN45 * r1;
                y1Limit = temp.w[1];
                temp.Val = SIN45 * r2;
                y2Limit = temp.w[1];

                temp.Val = (DWORD) (ONEP25 - ((LONG) r1 << 16));
                err1 = (SHORT) (temp.w[1]);

                temp.Val = (DWORD) (ONEP25 - ((LONG) r2 << 16));
                err2 = (SHORT) (temp.w[1]);

                x1 = r1;
                x2 = r2;
                y1 = 0;
                y2 = 0;

                x1Cur = x1;
                y1Cur = y1;
                y1New = y1;
                x2Cur = x2;
                y2Cur = y2;
                y2New = y2;
                state = CHECK;

            case CHECK:
                arc_check_state : if(y2 > y2Limit)
                {
                    state = BARRIGHT1;
                    goto arc_draw_width_height_state;
                }

                // y1New & y2New records the last y positions
                y1New = y1;
                y2New = y2;

                if(y1 <= y1Limit)
                {
                    if(err1 > 0)
                    {
                        x1--;
                        err1 += 5;
                        err1 += (y1 - x1) << 1;
                    }
                    else
                    {
                        err1 += 3;
                        err1 += y1 << 1;
                    }

                    y1++;
                }
                else
                {
                    y1++;
                    if(x1 < y1)
                        x1 = y1;
                }

                if(err2 > 0)
                {
                    x2--;
                    err2 += 5;
                    err2 += (y2 - x2) << 1;
                }
                else
                {
                    err2 += 3;
                    err2 += y2 << 1;
                }

                y2++;

                state = QUAD11;
                break;

            case QUAD11:
                if((x1Cur != x1) || (x2Cur != x2))
                {

                    // 1st octant
                    if(octant & 0x01)
                    {
                        if(Bar(xR + y2Cur, yT - x2Cur, xR + y1New, yT - x1Cur) == 0)
                            return (0);
                    }
                }
                else
                {
                    state = CHECK;
                    goto arc_check_state;
                }

                state = QUAD12;
                break;

            case QUAD12:

                // 2nd octant
                if(octant & 0x02)
                {
                    if(Bar(xR + x1Cur, yT - y1New, xR + x2Cur, yT - y2Cur) == 0)
                        return (0);
                }

                state = QUAD21;
                break;

            case QUAD21:

                // 3rd octant
                if(octant & 0x04)
                {
                    if(Bar(xR + x1Cur, yB + y1Cur, xR + x2Cur, yB + y2New) == 0)
                        return (0);
                }

                state = QUAD22;
                break;

            case QUAD22:

                // 4th octant
                if(octant & 0x08)
                {
                    if(Bar(xR + y1Cur, yB + x1Cur, xR + y2New, yB + x2Cur) == 0)
                        return (0);
                }

                state = QUAD31;
                break;

            case QUAD31:

                // 5th octant
                if(octant & 0x10)
                {
                    if(Bar(xL - y1New, yB + x1Cur, xL - y2Cur, yB + x2Cur) == 0)
                        return (0);
                }

                state = QUAD32;
                break;

            case QUAD32:

                // 6th octant
                if(octant & 0x20)
                {
                    if(Bar(xL - x2Cur, yB + y2Cur, xL - x1Cur, yB + y1New) == 0)
                        return (0);
                }

                state = QUAD41;
                break;

            case QUAD41:

                // 7th octant
                if(octant & 0x40)
                {
                    if(Bar(xL - x2Cur, yT - y2New, xL - x1Cur, yT - y1Cur) == 0)
                        return (0);
                }

                state = QUAD42;
                break;

            case QUAD42:

                // 8th octant
                if(octant & 0x80)
                {
                    if(Bar(xL - y2New, yT - x2Cur, xL - y1Cur, yT - x1Cur) == 0)
                        return (0);
                }

                // update current values
                x1Cur = x1;
                y1Cur = y1;
                x2Cur = x2;
                y2Cur = y2;
                state = CHECK;
                break;

            case BARRIGHT1:     // draw upper right
                arc_draw_width_height_state : if((xR - xL) || (yB - yT))
                {

                    // draw right
                    if(octant & 0x02)
                    {
                        if(Bar(xR + r1, yT, xR + r2, (yB + yT) >> 1) == 0)
                            return (0);
                    }
                }
                else
                {
                    state = BEGIN;
                    return (1);
                }

                state = BARRIGHT2;
                break;

            case BARRIGHT2:     // draw lower right
                if(octant & 0x04)
                {
                    if(Bar(xR + r1, ((yB + yT) >> 1), xR + r2, yB) == 0)
                        return (0);
                }

                state = BARBOTTOM1;
                break;

            case BARBOTTOM1:    // draw left bottom
                // draw bottom
                if(octant & 0x10)
                {
                    if(Bar(xL, yB + r1, ((xR + xL) >> 1), yB + r2) == 0)
                        return (0);
                }

                state = BARBOTTOM2;
                break;

            case BARBOTTOM2:    // draw right bottom
                if(octant & 0x08)
                {
                    if(Bar(((xR + xL) >> 1), yB + r1, xR, yB + r2) == 0)
                        return (0);
                }

                state = BARTOP1;
                break;

            case BARTOP1:       // draw left top
                if(xR - xL)
                {

                    // draw top
                    if(octant & 0x80)
                    {
                        if(Bar(xL, yT - r2, ((xR + xL) >> 1), yT - r1) == 0)
                            return (0);
                    }

                    state = BARTOP2;
                }
                else
                    state = BARLEFT1;   // no width go directly to height bar
                break;

            case BARTOP2:               // draw right top
                if(octant & 0x01)
                {
                    if(Bar(((xR + xL) >> 1), yT - r2, xR, yT - r1) == 0)
                        return (0);
                }

                state = BARLEFT1;
                break;

            case BARLEFT1:              // draw upper left
                if(yT - yB)
                {

                    // draw left
                    if(octant & 0x40)
                    {
                        if(Bar(xL - r2, yT, xL - r1, ((yB + yT) >> 1)) == 0)
                            return (0);
                    }

                    state = BARLEFT2;
                }
                else
                {
                    state = BEGIN;      // no height go back to BEGIN
                    return (1);
                }

                break;

            case BARLEFT2:              // draw lower left
                if(octant & 0x20)
                {
                    if(Bar(xL - r2, ((yB + yT) >> 1), xL - r1, yB) == 0)
                        return (0);
                }

                state = BEGIN;
                return (1);
        }                               // end of switch
    }   // end of while
    #endif // USE_NONBLOCKING_CONFIG
}

/*********************************************************************
* Function: WORD Bevel(SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT rad)
*
* PreCondition: None
*
* Input: x1, y1 - coordinate position of the upper left center of the 
* 				  circle that draws the rounded corners,
*		 x2, y2 - coordinate position of the lower right center of the 
* 				  circle that draws the rounded corners,
*        rad - defines the redius of the circle,
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Overview: Draws a beveled figure on the screen. 
*           For a pure circular object x1 = x2 and y1 = y2. 
*           For a rectangular object radius = 0.
*
* Note: none
*
********************************************************************/
WORD __attribute__((weak)) Bevel(SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT rad)
{
    SHORT       style, type, xLimit, xPos, yPos, error;
    DWORD_VAL   temp;

    #ifndef USE_NONBLOCKING_CONFIG
    while(IsDeviceBusy() != 0) Nop();

    /* Ready */
    #else
    if(IsDeviceBusy() != 0)
        return (0);
    #endif
    temp.Val = SIN45 * rad;
    xLimit = temp.w[1] + 1;
    temp.Val = (DWORD) (ONEP25 - ((LONG) rad << 16));
    error = (SHORT) (temp.w[1]);
    yPos = rad;

    style = 0;
    type = 1;

    if(rad)
    {
        for(xPos = 0; xPos <= xLimit; xPos++)
        {
            if((++style) == _lineType)
            {
                type ^= 1;
                style = 0;
            }

            if(type)
            {
                PutPixel(x2 + xPos, y1 - yPos);         // 1st quadrant
                PutPixel(x2 + yPos, y1 - xPos);
                PutPixel(x2 + xPos, y2 + yPos);         // 2nd quadrant
                PutPixel(x2 + yPos, y2 + xPos);
                PutPixel(x1 - xPos, y2 + yPos);         // 3rd quadrant
                PutPixel(x1 - yPos, y2 + xPos);
                PutPixel(x1 - yPos, y1 - xPos);         // 4th quadrant
                PutPixel(x1 - xPos, y1 - yPos);

                if(_lineThickness)
                {
                    PutPixel(x2 + xPos, y1 - yPos - 1); // 1st quadrant
                    PutPixel(x2 + xPos, y1 - yPos + 1);
                    PutPixel(x2 + yPos + 1, y1 - xPos);
                    PutPixel(x2 + yPos - 1, y1 - xPos);
                    PutPixel(x2 + xPos, y2 + yPos - 1); // 2nd quadrant
                    PutPixel(x2 + xPos, y2 + yPos + 1);
                    PutPixel(x2 + yPos + 1, y2 + xPos);
                    PutPixel(x2 + yPos - 1, y2 + xPos);
                    PutPixel(x1 - xPos, y2 + yPos - 1); // 3rd quadrant
                    PutPixel(x1 - xPos, y2 + yPos + 1);
                    PutPixel(x1 - yPos + 1, y2 + xPos);
                    PutPixel(x1 - yPos - 1, y2 + xPos);
                    PutPixel(x1 - yPos + 1, y1 - xPos); // 4th quadrant
                    PutPixel(x1 - yPos - 1, y1 - xPos);
                    PutPixel(x1 - xPos, y1 - yPos + 1);
                    PutPixel(x1 - xPos, y1 - yPos - 1);
                }
            }

            if(error > 0)
            {
                yPos--;
                error += 5 + ((xPos - yPos) << 1);
            }
            else
                error += 3 + (xPos << 1);
        }
    }
    // Must use lines here since this can also be used to draw focus of round buttons
    if(x2 - x1)
    {
        while(!Line(x1, y1 - rad, x2, y1 - rad));

        // draw top
    }

    if(y2 - y1)
    {
        while(!Line(x1 - rad, y1, x1 - rad, y2));

        // draw left
    }

    if((x2 - x1) || (y2 - y1))
    {
        while(!Line(x2 + rad, y1, x2 + rad, y2));

        // draw right
        while(!Line(x1, y2 + rad, x2, y2 + rad));

        // draw bottom
    }

    return (1);
}

/*********************************************************************
* Function: WORD FillBevel(SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT rad)
*
* PreCondition: None
*
* Input: x1, y1 - coordinate position of the upper left center of the 
* 				  circle that draws the rounded corners,
*		 x2, y2 - coordinate position of the lower right center of the 
* 				  circle that draws the rounded corners,
*        rad - defines the redius of the circle,
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Overview: Draws a filled beveled figure on the screen. 
*           For a filled circular object x1 = x2 and y1 = y2. 
*           For a filled rectangular object radius = 0.
*
* Note: none
*
********************************************************************/
WORD FillBevel(SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT rad)
{
    #ifndef USE_NONBLOCKING_CONFIG

    SHORT       yLimit, xPos, yPos, err;
    SHORT       xCur, yCur, yNew;
    DWORD_VAL   temp;

    // note that octants here is defined as:
    // from yPos=-radius, xPos=0 in the clockwise direction octant 1 to 8 are labeled
    // assumes an origin at 0,0. Quadrants are defined in the same manner
    if(rad)
    {
        temp.Val = SIN45 * rad;
        yLimit = temp.w[1];
        temp.Val = (DWORD) (ONEP25 - ((LONG) rad << 16));
        err = (SHORT) (temp.w[1]);
        xPos = rad;
        yPos = 0;

        xCur = xPos;
        yCur = yPos;
        yNew = yPos;

        while(yPos <= yLimit)
        {

            // Drawing of the rounded panel is done only when there is a change in the
            // x direction. Bars are drawn to be efficient.
            // detect changes in the x position. Every change will mean a bar will be drawn
            // to cover the previous area. y1New records the last position of y before the
            // change in x position.
            // y1New records the last y position
            yNew = yPos;

            if(err > 0)
            {
                xPos--;
                err += 5 + ((yPos - xPos) << 1);
            }
            else
                err += 3 + (yPos << 1);
            yPos++;

            if(xCur != xPos)
            {
	            if (_bevelDrawType & DRAWBOTTOMBEVEL) 
	            { 
	                // 6th octant to 3rd octant
	                Bar(x1 - xCur, y2 + yCur, x2 + xCur, y2 + yNew);
	
	                // 5th octant to 4th octant
	                Bar(x1 - yNew, y2 + xPos, x2 + yNew, y2 + xCur);
				}

	            if (_bevelDrawType & DRAWTOPBEVEL) 
				{
	                // 8th octant to 1st octant
	                Bar(x1 - yNew, y1 - xCur, x2 + yNew, y1 - xPos);
	
	                // 7th octant to 2nd octant
	                Bar(x1 - xCur, y1 - yNew, x2 + xCur, y1 - yCur);
				}
                // update current values
                xCur = xPos;
                yCur = yPos;
            }
        }
    }

    // this covers both filled rounded object and filled rectangle.
    if((x2 - x1) || (y2 - y1))
    {
		if (_bevelDrawType == DRAWFULLBEVEL) 
	        Bar(x1 - rad, y1, x2 + rad, y2);
	    else if (_bevelDrawType == DRAWTOPBEVEL)    
	        Bar(x1 - rad, y1, x2 + rad, y1+((y2-y1)>>1));
	    else    
	        Bar(x1 - rad, y1+((y2-y1)>>1), x2 + rad, y2);
	}     
	        
    return (1);
    #else

    typedef enum __attribute__((packed)) {
        BEGIN,
        CHECK,
        Q8TOQ1,
        Q7TOQ2,
        Q6TOQ3,
        Q5TOQ4,
        WAITFORDONE,
        FACE
    } FILLCIRCLE_STATES;

    DWORD_VAL temp;
    static LONG err;
    static SHORT yLimit, xPos, yPos;
    static SHORT xCur, yCur, yNew;

    static FILLCIRCLE_STATES state = BEGIN;

    while(1)
    {
        if(IsDeviceBusy())
            return (0);
        switch(state)
        {
            case BEGIN:
                if(!rad)
                {   // no radius object is a filled rectangle
                    state = FACE;
                    break;
                }

                // compute variables
                temp.Val = SIN45 * rad;
                yLimit = temp.w[1];
                temp.Val = (DWORD) (ONEP25 - ((LONG) rad << 16));
                err = (SHORT) (temp.w[1]);
                xPos = rad;
                yPos = 0;
                xCur = xPos;
                yCur = yPos;
                yNew = yPos;
                state = CHECK;

            case CHECK:
                bevel_fill_check : if(yPos > yLimit)
                {
                    state = FACE;
                    break;
                }

                // y1New records the last y position
                yNew = yPos;

                // calculate the next value of x and y
                if(err > 0)
                {
                    xPos--;
                    err += 5 + ((yPos - xPos) << 1);
                }
                else
                    err += 3 + (yPos << 1);
                yPos++;
                state = Q6TOQ3;

            case Q6TOQ3:
                if(xCur != xPos)
                {

                    // 6th octant to 3rd octant
	            	if (_bevelDrawType & DRAWBOTTOMBEVEL) 
	            	{ 
                    	if(Bar(x1 - xCur, y2 + yCur, x2 + xCur, y2 + yNew) == 0)
                        	return (0);
              		}          	
                    state = Q5TOQ4;
                    break;
                }

                state = CHECK;
                goto bevel_fill_check;

            case Q5TOQ4:

            	if (_bevelDrawType & DRAWBOTTOMBEVEL) 
            	{ 
	                // 5th octant to 4th octant
	                if(Bar(x1 - yNew, y2 + xPos, x2 + yNew, y2 + xCur) == 0)
	                    return (0);
				}
                state = Q8TOQ1;
                break;

            case Q8TOQ1:

                // 8th octant to 1st octant
	            if (_bevelDrawType & DRAWTOPBEVEL) 
				{
					if(Bar(x1 - yNew, y1 - xCur, x2 + yNew, y1 - xPos) == 0)
                    	return (0);
    			}                	
                state = Q7TOQ2;
                break;

            case Q7TOQ2:

                // 7th octant to 2nd octant
	            if (_bevelDrawType & DRAWTOPBEVEL) 
				{
	                if(Bar(x1 - xCur, y1 - yNew, x2 + xCur, y1 - yCur) == 0)
    	                return (0);
				}
                // update current values
                xCur = xPos;
                yCur = yPos;
                state = CHECK;
                break;

 

            case FACE:
                if((x2 - x1) || (y2 - y1))
                {
					if (_bevelDrawType == DRAWFULLBEVEL)
					{ 
				        if(Bar(x1 - rad, y1, x2 + rad, y2) == 0)
	                        return (0);
				 	}
				    else if (_bevelDrawType == DRAWTOPBEVEL)
				    {    
				        if(Bar(x1 - rad, y1, x2 + rad, y1+((y2-y1)>>1)) == 0)
	                        return (0);
				    }
				    else    
				    {
				        if(Bar(x1 - rad, y1+((y2-y1)>>1), x2 + rad, y2) == 0)
	                        return (0);
        			}            

                    state = WAITFORDONE;
                }
                else
                {
                    state = BEGIN;
                    return (1);
                }

            case WAITFORDONE:
                if(IsDeviceBusy())
                    return (0);
                state = BEGIN;
                return (1);
        }           // end of switch
    }               // end of while
    #endif // end of USE_NONBLOCKING_CONFIG
}

/*********************************************************************
* Function: WORD DrawPoly(SHORT numPoints, SHORT* polyPoints)
*
* PreCondition: none
*
* Input: numPoints - points number, polyPoints - pointer to points array
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: draws line polygon
*
* Note: none
*
********************************************************************/
WORD __attribute__((weak)) DrawPoly(SHORT numPoints, SHORT *polyPoints)
{
    #ifndef USE_NONBLOCKING_CONFIG

    SHORT   counter;
    SHORT   sx, sy, ex, ey;

        #ifndef USE_NONBLOCKING_CONFIG
    while(IsDeviceBusy() != 0) Nop();

    /* Ready */
        #else
    if(IsDeviceBusy() != 0)
        return (0);
        #endif
    sx = *polyPoints++;
    sy = *polyPoints++;
    for(counter = 0; counter < numPoints - 1; counter++)
    {
        ex = *polyPoints++;
        ey = *polyPoints++;
        while(Line(sx, sy, ex, ey) == 0);
        sx = ex;
        sy = ey;
    }

    #else

    typedef enum __attribute__((packed)) {
        POLY_BEGIN,
        POLY_DRAWING,
    } DRAWPOLY_STATES;

    static DRAWPOLY_STATES state = POLY_BEGIN;
    static SHORT counter;
    SHORT sx, sy, ex, ey;
    while(1)
    {
        if(IsDeviceBusy())
            return (0);
        switch(state)
        {
            case POLY_BEGIN:
                counter = 1;
                state = POLY_DRAWING;

            case POLY_DRAWING:
                if(counter == 0 || counter >= numPoints)
                {
                    state = POLY_BEGIN;
                    break;
                }

                while(counter < numPoints)
                {
                    sx = polyPoints[(counter - 1) * 2];
                    sy = polyPoints[(counter * 2) - 1];
                    ex = polyPoints[counter * 2];
                    ey = polyPoints[counter * 2 + 1];
                    if(Line(sx, sy, ex, ey) == 0)
                    {
                        return (0);
                    }

                    counter++;
                }

                state = POLY_BEGIN;
                return (1);
        }
    }
    #endif
    return (1);
}

/*********************************************************************
* Function: void SetFont(void* font)
*
* PreCondition: none
*
* Input: pointer to the font image
*
* Output: none
*
* Side Effects: none
*
* Overview: defines current font
*
* Note: none
*
********************************************************************/
void __attribute__((weak)) SetFont(void *font)
{
    FONT_HEADER *pHeader;

        #ifdef USE_FONT_EXTERNAL
    FONT_HEADER header;
        #endif
    _font = font;
    switch(*((SHORT *)font))
    {
                #ifdef USE_FONT_FLASH

        case FLASH:
            pHeader = (FONT_HEADER *) ((FONT_FLASH *)font)->address;
            break;

				#endif // USE_FONT_FLASH
                #ifdef USE_FONT_EXTERNAL
				
        case EXTERNAL:
            pHeader = &header;
            ExternalMemoryCallback(font, 0, sizeof(FONT_HEADER), pHeader);
            break;

				#endif // USE_FONT_EXTERNAL

        default:
            return;
    }

    _fontFirstChar = pHeader->firstChar;
    _fontLastChar = pHeader->lastChar;
    _fontHeight = pHeader->height;
}

/*********************************************************************
* Function: WORD OutChar(XCHAR ch)
*
* PreCondition: none
*
* Input: character code
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the character is not yet completely drawn.
*         - Returns 1 when the character is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: outputs a character
*
* Note: none
*
********************************************************************/
WORD __attribute__((weak)) OutChar(XCHAR ch)
{
        #ifdef USE_FONT_FLASH
    GLYPH_ENTRY *pChTable = NULL;
    	#endif
    BYTE        *pChImage = NULL;

        #ifdef USE_FONT_EXTERNAL
    GLYPH_ENTRY chTable;
    BYTE        chImage[EXTERNAL_FONT_BUFFER_SIZE];
    WORD        imageSize;
    DWORD_VAL   glyphOffset;
        #endif
    SHORT       chWidth = 0;
    SHORT       xCnt, yCnt, x = 0, y;
    BYTE        temp = 0, mask;

        #ifndef USE_NONBLOCKING_CONFIG
    while(IsDeviceBusy() != 0) Nop();

    /* Ready */
        #else
    if(IsDeviceBusy() != 0)
        return (0);
        #endif
    if((XCHAR)ch < (XCHAR)_fontFirstChar)
        return (-1);
    if((XCHAR)ch > (XCHAR)_fontLastChar)
        return (-1);

    switch(*((SHORT *)_font))
    {
                #ifdef USE_FONT_FLASH

        case FLASH:
            pChTable = (GLYPH_ENTRY *) (((FONT_FLASH *)_font)->address + sizeof(FONT_HEADER)) + ((XCHAR)ch - (XCHAR)_fontFirstChar);

            pChImage = (BYTE *) (((FONT_FLASH *)_font)->address + ((DWORD)(pChTable->offsetMSB) << 8) + pChTable->offsetLSB);

            chWidth = pChTable->width;

            break;
                #endif
                #ifdef USE_FONT_EXTERNAL
			
        case EXTERNAL:

            // get glyph entry
            ExternalMemoryCallback
            (
                _font,
                sizeof(FONT_HEADER) + ((XCHAR)ch - (XCHAR)_fontFirstChar) * sizeof(GLYPH_ENTRY),
                sizeof(GLYPH_ENTRY),
                &chTable
            );

            chWidth = chTable.width;

            // width of glyph in bytes
            imageSize = 0;
            if(chWidth & 0x0007)
                imageSize = 1;
            imageSize += (chWidth >> 3);

            // glyph image size
            imageSize *= _fontHeight;

            // get glyph image
            glyphOffset.w[1] = (chTable.offsetMSB >> 8);
            glyphOffset.w[0] = (chTable.offsetMSB << 8) + (chTable.offsetLSB);

            ExternalMemoryCallback(_font, glyphOffset.Val, imageSize, &chImage);
            pChImage = (BYTE *) &chImage;

            break;
                #endif

        default:
            break;
    }

    if(_fontOrientation == ORIENT_HOR)
    {
        y = GetY();
        for(yCnt = 0; yCnt < _fontHeight; yCnt++)
        {
            x = GetX();
            mask = 0;
            for(xCnt = 0; xCnt < chWidth; xCnt++)
            {
                if(mask == 0)
                {
                    temp = *pChImage++;
                    mask = 0x01;
                }

                if(temp & mask)
                {
                    PutPixel(x, y);
                }

                x++;
                mask <<= 1;
            }

            y++;
        }

        // move cursor
        _cursorX = x;
    }
    else
    {
        y = GetX();
        for(yCnt = 0; yCnt < _fontHeight; yCnt++)
        {
            x = GetY();
            mask = 0; 
            for(xCnt = 0; xCnt < chWidth; xCnt++)
            {
                if(mask == 0)
                {
                    temp = *pChImage++;
                    mask = 0x01; 
                }

                if(temp & mask)
                {
                    PutPixel(y, x);
                }

                x--;
                mask <<= 1;
            }

            y++;
        }

        // move cursor
        _cursorY = x;
    }

    return (1);
}

/*********************************************************************
* Function: WORD OutText(XCHAR* textString)
*
* PreCondition: none
*
* Input: textString - pointer to text string
*
* Output: non-zero if drawing done (used for NON-BLOCKING configuration)
*
* Side Effects: none
*
* Overview: outputs text from current position
*
* Note: none
*
********************************************************************/
WORD __attribute__((weak)) OutText(XCHAR *textString)
{
    #ifndef USE_NONBLOCKING_CONFIG

    XCHAR   ch;
    while((XCHAR)15 < (XCHAR)(ch = *textString++))
        while(OutChar(ch) == 0);
    return (1);

    #else

    XCHAR       ch;
    static WORD counter = 0;

    while((XCHAR)(ch = *(textString + counter)) > (XCHAR)15)
    {
        if(OutChar(ch) == 0)
            return (0);
        counter++;
    }

    counter = 0;
    return (1);
    #endif
}

/*********************************************************************
* Function: WORD OutTextXY(SHORT x, SHORT y, XCHAR* textString)
*
* PreCondition: none
*
* Input: x,y - starting coordinates, textString - pointer to text string
*
* Output: non-zero if drawing done (used for NON-BLOCKING configuration)
*
* Side Effects: none
*
* Overview: outputs text from x,y position
*
* Note: none
*
********************************************************************/
WORD __attribute__((weak)) OutTextXY(SHORT x, SHORT y, XCHAR *textString)
{
    #ifndef USE_NONBLOCKING_CONFIG
    MoveTo(x, y);
    OutText(textString);
    return (1);

    #else

    static BYTE start = 1;

    if(start)
    {
        MoveTo(x, y);
        start = 0;
    }

    if(OutText(textString) == 0)
    {
        return (0);
    }
    else
    {
        start = 1;
        return (1);
    }

    #endif
}

/*********************************************************************
* Function: SHORT GetTextWidth(XCHAR* textString, void* font)
*
* PreCondition: none
*
* Input: textString - pointer to the text string,
*        font - pointer to the font
*
* Output: text width in pixels
*
* Side Effects: none
*
* Overview: returns text width for the font
*
* Note: none
*
********************************************************************/
SHORT __attribute__((weak)) GetTextWidth(XCHAR *textString, void *font)
{
		#if defined (USE_FONT_RAM) || defined (USE_FONT_FLASH) 
    GLYPH_ENTRY *pChTable;
    FONT_HEADER *pHeader;
    	#endif
        #ifdef USE_FONT_EXTERNAL
    GLYPH_ENTRY chTable;
    FONT_HEADER header;
        #endif

    	#if defined (USE_FONT_RAM) || defined (USE_FONT_FLASH) || defined (USE_FONT_EXTERNAL)
    SHORT       textWidth;
    XCHAR       ch;
    XCHAR       fontFirstChar;
    XCHAR       fontLastChar;
    	#endif

    switch(*((SHORT *)font))
    {
                #ifdef USE_FONT_RAM

        case RAM:
            pHeader = (FONT_HEADER *) ((FONT_RAM *)font)->address;
            fontFirstChar = pHeader->firstChar;
            fontLastChar = pHeader->lastChar;
            pChTable = (GLYPH_ENTRY *) (pHeader + 1);
            textWidth = 0;
            while((unsigned XCHAR)15 < (unsigned XCHAR)(ch = *textString++))
            {
                if((unsigned XCHAR)ch < (unsigned XCHAR)fontFirstChar)
                    continue;
                if((unsigned XCHAR)ch > (unsigned XCHAR)fontLastChar)
                    continue;
                textWidth += (pChTable + ((unsigned XCHAR)ch - (unsigned XCHAR)fontFirstChar))->width;
            }

            return (textWidth);
                #endif
                	    
                #ifdef USE_FONT_FLASH

        case FLASH:
            pHeader = (FONT_HEADER *) ((FONT_FLASH *)font)->address;
            fontFirstChar = pHeader->firstChar;
            fontLastChar = pHeader->lastChar;
            pChTable = (GLYPH_ENTRY *) (pHeader + 1);
            textWidth = 0;
            while((XCHAR)15 < (XCHAR)(ch = *textString++))
            {
                if((XCHAR)ch < (XCHAR)fontFirstChar)
                    continue;
                if((XCHAR)ch > (XCHAR)fontLastChar)
                    continue;
                textWidth += (pChTable + ((XCHAR)ch - (XCHAR)fontFirstChar))->width;
            }

            return (textWidth);
                #endif
                #ifdef USE_FONT_EXTERNAL

        case EXTERNAL:
            ExternalMemoryCallback(font, 0, sizeof(FONT_HEADER), &header);
            fontFirstChar = header.firstChar;
            fontLastChar = header.lastChar;
            textWidth = 0;
            while((XCHAR)15 < (XCHAR)(ch = *textString++))
            {
                if((XCHAR)ch < (XCHAR)fontFirstChar)
                    continue;
                if((XCHAR)ch > (XCHAR)fontLastChar)
                    continue;
                ExternalMemoryCallback
                (
                    font,
                    sizeof(FONT_HEADER) + sizeof(GLYPH_ENTRY) * ((XCHAR)ch - (XCHAR)fontFirstChar),
                    sizeof(GLYPH_ENTRY),
                    &chTable
                );
                textWidth += chTable.width;
            }

            return (textWidth);
                #endif

        default:
            return (0);
    }
}

/*********************************************************************
* Function: SHORT GetTextHeight(void* font)
*
* PreCondition: none
*
* Input: pointer to the font
*
* Output: character height in pixels
*
* Side Effects: none
*
* Overview: returns characters height for the font
*
* Note: none
*
********************************************************************/
SHORT __attribute__((weak)) GetTextHeight(void *font)
{
        #ifdef USE_FONT_EXTERNAL

    char    height;
        #endif
    switch(*((SHORT *)font))
    {
                #ifdef USE_FONT_RAM
        case RAM:
            return ((FONT_HEADER *) ((FONT_RAM *)font)->address)->height;
                #endif
                
                #ifdef USE_FONT_FLASH
        case FLASH:
            return ((FONT_HEADER *) ((FONT_FLASH *)font)->address)->height;
                #endif
                
                #ifdef USE_FONT_EXTERNAL
        case EXTERNAL:
            ExternalMemoryCallback(font, sizeof(FONT_HEADER) - 2, 1, &height);
            return (height);
                #endif

        default:
            return (0);
    }
}

/*********************************************************************
* Function: SHORT GetImageWidth(void* image)
*
* PreCondition: none
*
* Input: image - image pointer
*
* Output: none
*
* Side Effects: none
*
* Overview: returns image width
*
* Note: none
*
********************************************************************/
SHORT __attribute__((weak)) GetImageWidth(void *image)
{
    #ifdef USE_BITMAP_EXTERNAL

    SHORT   width;
    #endif
    switch(*((SHORT *)image))
    {
            #ifdef USE_BITMAP_FLASH

        case FLASH:
            return (*((FLASH_WORD *) ((IMAGE_FLASH *)image)->address + 2));
            #endif
            #ifdef USE_BITMAP_EXTERNAL

        case EXTERNAL:
            ExternalMemoryCallback(image, 4, 2, &width);
            return (width);
            #endif

        default:
            return (0);
    }
}

/*********************************************************************
* Function: SHORT GetImageHeight(void* image)
*
* PreCondition: none
*
* Input: image - image pointer
*
* Output: none
*
* Side Effects: none
*
* Overview: returns image height
*
* Note: none
*
********************************************************************/
SHORT __attribute__((weak)) GetImageHeight(void *image)
{
    #ifdef USE_BITMAP_EXTERNAL

    SHORT   height;
    #endif
    switch(*((SHORT *)image))
    {
            #ifdef USE_BITMAP_FLASH

        case FLASH:
            return (*((FLASH_WORD *) ((IMAGE_FLASH *)image)->address + 1));
            #endif
            #ifdef USE_BITMAP_EXTERNAL
            
        case EXTERNAL:
            ExternalMemoryCallback(image, 2, 2, &height);
            return (height);
            #endif


        default:
            return (0);
    }
}

    #ifdef USE_BITMAP_FLASH

/*********************************************************************
* Function: void PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        image - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void __attribute__((weak)) PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch)
{
    register FLASH_BYTE *flashAddress;
    BYTE                temp = 0;
    WORD                sizeX, sizeY;
    WORD                x, y;
    WORD                xc, yc;
    WORD                pallete[2];
    BYTE                mask;

    // Move pointer to size information
    flashAddress = image + 2;

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    pallete[0] = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    pallete[1] = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;

    yc = top;
    
    // Note: For speed the code for loops are repeated. A small code size increase for performance
    if (stretch == IMAGE_NORMAL)
    {
        for(y = 0; y < sizeY; y++)
        {
            xc = left;
            mask = 0;
            for(x = 0; x < sizeX; x++)
            {

                // Read 8 pixels from flash
                if(mask == 0)
                {
                    temp = *flashAddress++;
                    mask = 0x80;
                }

                // Set color
                if(mask & temp)
                {
                // Set color
                    #ifdef USE_PALETTE
                        SetColor(1);
                    #else
                        SetColor(pallete[1]);
                    #endif                
                }
                else
                {
                    // Set color
                    #ifdef USE_PALETTE
                        SetColor(0);
                    #else
                        SetColor(pallete[0]);
                    #endif
                }

                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                        // Write pixel(s) to screen
                        PutPixel(xc, yc);
                    // adjust to next location
                    xc++;
                    // Shift to the next pixel
                    mask >>= 1;    
            }            
            yc++;
        }
    }
    else
    {
        for(y = 0; y < sizeY; y++)
        {
            xc = left;
            mask = 0;

            for(x = 0; x < sizeX; x++)
            {

                // Read 8 pixels from flash
                if(mask == 0)
                {
                    temp = *flashAddress++;
                    mask = 0x80;
                }

                    // Set color
                if(mask & temp)
                {
                    // Set color
                    #ifdef USE_PALETTE
                        SetColor(1);
                    #else
                        SetColor(pallete[1]);
                    #endif                
                }
                else
                {
                    // Set color
                    #ifdef USE_PALETTE
                        SetColor(0);
                    #else
                        SetColor(pallete[0]);
                    #endif
                }
                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                    {
                        // Write pixel(s) to screen, basically writes a tile of 4x4 pixels to the screen
                        PutPixel(xc,   yc);
                        PutPixel(xc,   yc+1);  
                        PutPixel(xc+1, yc);
                        PutPixel(xc+1, yc+1);  
                    }
                    xc += 2;
                // Shift to the next pixel
                mask >>= 1;    
            }
            yc+=2;
        }
    }    

}

/*********************************************************************
* Function: void PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs 16 color image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
        #if (COLOR_DEPTH >= 4)

/* */
void __attribute__((weak)) PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch)
{
    register FLASH_BYTE *flashAddress;
    WORD                sizeX, sizeY;
    register WORD       x, y;
    WORD                xc, yc;
    BYTE                temp = 0;
    WORD                pallete[16];
    WORD                counter;

    // Move pointer to size information
    flashAddress = image + 2;

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;

    // Read pallete
    for(counter = 0; counter < 16; counter++)
    {
        pallete[counter] = *((FLASH_WORD *)flashAddress);
        flashAddress += 2;
    }

    yc = top;

    // Note: For speed the code for loops are repeated. A small code size increase for performance
    if (stretch == IMAGE_NORMAL)
    {
        for(y = 0; y < sizeY; y++)
        {
            xc = left;
            for(x = 0; x < sizeX; x++)
            {

                // Read 2 pixels from flash
                if(x & 0x0001)
                {

                // second pixel in byte
                // Set color
                #ifdef USE_PALETTE
                    SetColor(temp >> 4);
                #else
                    SetColor(pallete[temp >> 4]);
                #endif
                }
                else
                {
                    temp = *flashAddress++;

                // first pixel in byte
                // Set color
                #ifdef USE_PALETTE
                    SetColor(temp & 0x0f);
                #else
                    SetColor(pallete[temp & 0x0f]);
                #endif
                }

                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                        // Write pixel(s) to screen
                        PutPixel(xc, yc);
                    xc++;
            }            
            yc++;
        }
    }
    else
    {
        for(y = 0; y < sizeY; y++)
        {
            xc = left;
            for(x = 0; x < sizeX; x++)
            {

               if(x & 0x0001)
               {

                // second pixel in byte
                // Set color
                #ifdef USE_PALETTE
                    SetColor(temp >> 4);
                #else
                    SetColor(pallete[temp >> 4]);
                #endif
                }
                else
                {
                    temp = *flashAddress++;

                // first pixel in byte
                // Set color
                #ifdef USE_PALETTE
                    SetColor(temp & 0x0f);
                #else
                    SetColor(pallete[temp & 0x0f]);
                #endif
                }

                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                    {
                        // Write pixel(s) to screen, basically writes a tile of 4x4 pixels to the screen
                        PutPixel(xc,   yc);
                        PutPixel(xc,   yc+1);  
                        PutPixel(xc+1, yc);
                        PutPixel(xc+1, yc+1);  
                    }
                    xc += 2;
            }
            yc+=2;
        }
    }    
}
        #endif // #if (COLOR_DEPTH >= 4)

/*********************************************************************
* Function: void PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs 256 color image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
        #if (COLOR_DEPTH >= 8)

/* */
void __attribute__((weak)) PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch)
{
    register FLASH_BYTE *flashAddress;
    WORD                sizeX, sizeY;
    WORD                x, y;
    WORD                xc, yc;
    BYTE                temp;
    WORD                pallete[256];
    WORD                counter;

    // Move pointer to size information
    flashAddress = image + 2;

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;

    // Read pallete
    for(counter = 0; counter < 256; counter++)
    {
        pallete[counter] = *((FLASH_WORD *)flashAddress);
        flashAddress += 2;
    }

    yc = top;

    // Note: For speed the code for loops are repeated. A small code size increase for performance
    if (stretch == IMAGE_NORMAL)
    {
        for(y = 0; y < sizeY; y++)
        {
            xc = left;
            for(x = 0; x < sizeX; x++)
            {
                temp = *flashAddress++;

                // Set color
                #ifdef USE_PALETTE
                    SetColor(temp);
                #else
                    SetColor(pallete[temp]);
                #endif

                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                        // Write pixel(s) to screen
                        PutPixel(xc, yc);
                    xc++;
            }            
            yc++;
        }
    }
    else
    {
        for(y = 0; y < sizeY; y++)
        {
            xc = left;
            for(x = 0; x < sizeX; x++)
            {
                temp = *flashAddress++;

                // Set color
                #ifdef USE_PALETTE
                    SetColor(temp);
                #else
                    SetColor(pallete[temp]);
                #endif
    
                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                    {
                        // Write pixel(s) to screen, basically writes a tile of 4x4 pixels to the screen
                        PutPixel(xc,   yc);
                        PutPixel(xc,   yc+1);  
                        PutPixel(xc+1, yc);
                        PutPixel(xc+1, yc+1);  
                    }
                    xc += 2;
            }            
            yc+=2;
        }
    }    

}
        #endif // #if (COLOR_DEPTH >= 8)

/*********************************************************************
* Function: void PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs hicolor image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
        #if (COLOR_DEPTH == 16)

/* */
void __attribute__((weak)) PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch)
{
    register FLASH_WORD *flashAddress;
    WORD                sizeX, sizeY;
    register WORD       x, y;
    WORD                xc, yc;
    WORD                temp;

    // Move pointer to size information
    flashAddress = (FLASH_WORD *)image + 1;

    // Read image size
    sizeY = *flashAddress;
    flashAddress++;
    sizeX = *flashAddress;
    flashAddress++;

    yc = top;

    // Note: For speed the code for loops are repeated. A small code size increase for performance
    if (stretch == IMAGE_NORMAL)
    {
        for(y = 0; y < sizeY; y++)
        {
            xc = left;
            for(x = 0; x < sizeX; x++)
            {
                temp = *flashAddress++;
                SetColor(temp);

                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                        // Write pixel(s) to screen
                        PutPixel(xc, yc);
                    xc++;
            }            
            yc++;
        }
    }
    else
    {
        for(y = 0; y < sizeY; y++)
        {
            xc = left;
            for(x = 0; x < sizeX; x++)
            {
                temp = *flashAddress++;
                SetColor(temp);
    
                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                    {
                        // Write pixel(s) to screen, basically writes a tile of 4x4 pixels to the screen
                        PutPixel(xc,   yc);
                        PutPixel(xc,   yc+1);  
                        PutPixel(xc+1, yc);
                        PutPixel(xc+1, yc+1);  
                    } 
                    xc += 2;   
            }            
            yc+=2;
        }
    }    

}

        #endif //#if (COLOR_DEPTH == 16)
    #endif // #ifdef USE_BITMAP_FLASH

    #ifdef USE_BITMAP_EXTERNAL

/*********************************************************************
* Function: void PutImage1BPPExt(SHORT left, SHORT top, void* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
void __attribute__((weak)) PutImage1BPPExt(SHORT left, SHORT top, void *image, BYTE stretch)
{
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    WORD            pallete[2];
    BYTE            lineBuffer[((GetMaxX() + 1) / 8) + 1];
    BYTE            *pData;
    SHORT           byteWidth;

    BYTE            temp = 0;
    BYTE            mask;
    WORD            sizeX, sizeY;
    WORD            x, y;
    WORD            xc, yc;

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Get pallete (2 entries)
    ExternalMemoryCallback(image, sizeof(BITMAP_HEADER), 2 * sizeof(WORD), pallete);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 2 * sizeof(WORD);

    // Line width in bytes
    byteWidth = bmp.width >> 3;
    if(bmp.width & 0x0007)
        byteWidth++;

    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;

    yc = top;
    
    // Note: For speed the code for loops are repeated. A small code size increase for performance
    if (stretch == IMAGE_NORMAL)
    {
        for(y = 0; y < sizeY; y++)
        {
            // Get line
            ExternalMemoryCallback(image, memOffset, byteWidth, lineBuffer);
            memOffset += byteWidth;
            
            pData = lineBuffer;
            mask = 0;
            
            xc = left;
            for(x = 0; x < sizeX; x++)
            {

                    // Read 8 pixels from flash
                    if(mask == 0)
                    {
                        temp = *pData++;
                        mask = 0x80;
                    }
                
                    // Set color
                    if(mask & temp)
                    {
                        // Set color
                        #ifdef USE_PALETTE
                            SetColor(1);
                        #else
                            SetColor(pallete[1]);
                        #endif
                    }
                    else
                    {
                        // Set color
                        #ifdef USE_PALETTE
                            SetColor(0);
                        #else
                            SetColor(pallete[0]);
                        #endif
                    }


                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                        // Write pixel(s) to screen
                        PutPixel(xc, yc);
                    xc++;
                // Shift to the next pixel
                mask >>= 1;
            }            
            yc++;
        }
    }
    else
    {
        for(y = 0; y < sizeY; y++)
        {
            // Get line
            ExternalMemoryCallback(image, memOffset, byteWidth, lineBuffer);
            memOffset += byteWidth;
            
            pData = lineBuffer;
            mask = 0;
            
            xc = left;
            for(x = 0; x < sizeX; x++)
            {

                      // Read 8 pixels from flash
                    if(mask == 0)
                    {
                        temp = *pData++;
                        mask = 0x80;
                    }
                
                    // Set color
                    if(mask & temp)
                    {
                        // Set color
                        #ifdef USE_PALETTE
                            SetColor(1);
                        #else
                            SetColor(pallete[1]);
                        #endif
                    }
                    else
                    {
                        // Set color
                        #ifdef USE_PALETTE
                            SetColor(0);
                        #else
                            SetColor(pallete[0]);
                        #endif
                    }

    
                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                    {
                        // Write pixel(s) to screen, basically writes a tile of 4x4 pixels to the screen
                        PutPixel(xc,   yc);
                        PutPixel(xc,   yc+1);  
                        PutPixel(xc+1, yc);
                        PutPixel(xc+1, yc+1);  
                    }
                    xc += 2;
                // Shift to the next pixel
                mask >>= 1;

            }            
            yc+=2;
        }
    }    

}

/*********************************************************************
* Function: void PutImage4BPPExt(SHORT left, SHORT top, void* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
        #if (COLOR_DEPTH >= 4)

/* */
void __attribute__((weak)) PutImage4BPPExt(SHORT left, SHORT top, void *image, BYTE stretch)
{
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    WORD            pallete[16];
    BYTE            lineBuffer[((GetMaxX() + 1) / 2) + 1];
    BYTE            *pData;
    SHORT           byteWidth;

    BYTE            temp = 0;
    WORD            sizeX, sizeY;
    WORD            x, y;
    WORD            xc, yc;

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Get pallete (16 entries)
    ExternalMemoryCallback(image, sizeof(BITMAP_HEADER), 16 * sizeof(WORD), pallete);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 16 * sizeof(WORD);

    // Line width in bytes
    byteWidth = bmp.width >> 1;
    if(bmp.width & 0x0001)
        byteWidth++;

    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;

    yc = top;

    // Note: For speed the code for loops are repeated. A small code size increase for performance
    if (stretch == IMAGE_NORMAL)
    {
        for(y = 0; y < sizeY; y++)
        {
            // Get line
            ExternalMemoryCallback(image, memOffset, byteWidth, lineBuffer);
            memOffset += byteWidth;
            
            pData = lineBuffer;
            
            xc = left;
            for(x = 0; x < sizeX; x++)
            {
                
                // Read 2 pixels from flash
                if(x & 0x0001)
                {

                    // second pixel in byte
                    // Set color
                    #ifdef USE_PALETTE
                        SetColor(temp >> 4);
                    #else
                        SetColor(pallete[temp >> 4]);
                    #endif
                }
                else
                {
                    temp = *pData++;

                    // first pixel in byte
                    // Set color
                    #ifdef USE_PALETTE
                        SetColor(temp & 0x0f);
                    #else
                        SetColor(pallete[temp & 0x0f]);
                    #endif
                }

                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                        // Write pixel(s) to screen
                        PutPixel(xc, yc);
                    xc++;
            }            
            yc++;
        }
    }
    else
    {
        for(y = 0; y < sizeY; y++)
        {
            // Get line
            ExternalMemoryCallback(image, memOffset, byteWidth, lineBuffer);
            memOffset += byteWidth;
            
            pData = lineBuffer;
            
            xc = left;
            for(x = 0; x < sizeX; x++)
            {

                // Read 2 pixels from flash
                if(x & 0x0001)
                {

                    // second pixel in byte
                    // Set color
                    #ifdef USE_PALETTE
                        SetColor(temp >> 4);
                    #else
                        SetColor(pallete[temp >> 4]);
                    #endif
                }
                else
                {
                    temp = *pData++;

                    // first pixel in byte
                    // Set color
                    #ifdef USE_PALETTE
                        SetColor(temp & 0x0f);
                    #else
                        SetColor(pallete[temp & 0x0f]);
                    #endif
                }

                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                    {    
                        // Write pixel(s) to screen, basically writes a tile of 4x4 pixels to the screen
                        PutPixel(xc,   yc);
                        PutPixel(xc,   yc+1);  
                        PutPixel(xc+1, yc);
                        PutPixel(xc+1, yc+1);  
                    }
                    xc += 2;
            }            
            yc+=2;
        }
    }    
}

        #endif

/*********************************************************************
* Function: void PutImage8BPPExt(SHORT left, SHORT top, void* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
        #if (COLOR_DEPTH >= 8)

/* */
void __attribute__((weak)) PutImage8BPPExt(SHORT left, SHORT top, void *image, BYTE stretch)
{
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    WORD            pallete[256];
    BYTE            lineBuffer[(GetMaxX() + 1)];
    BYTE            *pData;

    BYTE            temp;
    WORD            sizeX, sizeY;
    WORD            x, y;
    WORD            xc, yc;

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Get pallete (256 entries)
    ExternalMemoryCallback(image, sizeof(BITMAP_HEADER), 256 * sizeof(WORD), pallete);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 256 * sizeof(WORD);

    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;

    yc = top;

    // Note: For speed the code for loops are repeated. A small code size increase for performance
    if (stretch == IMAGE_NORMAL)
    {
        for(y = 0; y < sizeY; y++)
        {
            // Get line
            ExternalMemoryCallback(image, memOffset, sizeX, lineBuffer);
            memOffset += sizeX;
            
            pData = lineBuffer;
            
            xc = left;
            for(x = 0; x < sizeX; x++)
            {
                temp = *pData++;
                
                // Set color
                #ifdef USE_PALETTE
                    SetColor(temp);
                #else
                    SetColor(pallete[temp]);
                #endif

                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                        // Write pixel(s) to screen
                        PutPixel(xc, yc);
                    xc++;
            }            
            yc++;
        }
    }
    else
    {
        for(y = 0; y < sizeY; y++)
        {
            // Get line
            ExternalMemoryCallback(image, memOffset, sizeX, lineBuffer);
            memOffset += sizeX;
            
            pData = lineBuffer;
            
            xc = left;
            for(x = 0; x < sizeX; x++)
            {
                temp = *pData++;

                // Set color
                #ifdef USE_PALETTE
                    SetColor(temp);
                #else
                    SetColor(pallete[temp]);
                #endif
    
                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                    {
                        // Write pixel(s) to screen, basically writes a tile of 4x4 pixels to the screen
                        PutPixel(xc,   yc);
                        PutPixel(xc,   yc+1);  
                        PutPixel(xc+1, yc);
                        PutPixel(xc+1, yc+1);  
                    }
                    xc += 2;
            }            
            yc+=2;
        }
    }    

}

        #endif

/*********************************************************************
* Function: void PutImage16BPPExt(SHORT left, SHORT top, void* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
        #if (COLOR_DEPTH == 16)

/* */
void __attribute__((weak)) PutImage16BPPExt(SHORT left, SHORT top, void *image, BYTE stretch)
{
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    WORD            lineBuffer[(GetMaxX() + 1)];
    WORD            *pData;
    WORD            byteWidth;

    WORD            temp;
    WORD            sizeX, sizeY;
    WORD            x, y;
    WORD            xc, yc;

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER);

    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;

    byteWidth = sizeX << 1;

    yc = top;

    // Note: For speed the code for loops are repeated. A small code size increase for performance
    if (stretch == IMAGE_NORMAL)
    {
        for(y = 0; y < sizeY; y++)
        {
            // Get line
            ExternalMemoryCallback(image, memOffset, byteWidth, lineBuffer);
            memOffset += byteWidth;
            
            pData = lineBuffer;
            
            xc = left;
            for(x = 0; x < sizeX; x++)
                {
                temp = *pData++;
                SetColor(temp);

                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                        // Write pixel(s) to screen
                        PutPixel(xc, yc);
                    xc++;
            }            
            yc++;
        }
    }
    else
    {
        for(y = 0; y < sizeY; y++)
        {
            // Get line
            ExternalMemoryCallback(image, memOffset, byteWidth, lineBuffer);
            memOffset += byteWidth;
            
            pData = lineBuffer;
            
            xc = left;
            for(x = 0; x < sizeX; x++)
                {
                temp = *pData++;
                SetColor(temp);
    
                #ifdef USE_TRANSPARENT_COLOR
                    if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                #endif
                    {
                        // Write pixel(s) to screen, basically writes a tile of 4x4 pixels to the screen
                        PutPixel(xc,   yc);
                        PutPixel(xc,   yc+1);  
                        PutPixel(xc+1, yc);
                        PutPixel(xc+1, yc+1);  
                    }
                    xc += 2;
            }            
            yc+=2;
        }
    }    
}

        #endif
    #endif

/*********************************************************************
* Function: WORD PutImage(SHORT left, SHORT top, void* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        image - image pointer,
*        stretch - image stretch factor
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the image is not yet completely drawn.
*         - Returns 1 when the image is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: outputs image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
WORD __attribute__((weak)) PutImage(SHORT left, SHORT top, void *image, BYTE stretch)
{
    FLASH_BYTE  *flashAddress;
    BYTE        colorDepth;
    WORD        colorTemp;
    WORD        resType;

#if defined(USE_COMP_RLE) 
    GFX_IMAGE_HEADER *pimghdr = (GFX_IMAGE_HEADER *)image;
#endif
    
#ifndef USE_NONBLOCKING_CONFIG
    while(IsDeviceBusy() != 0) Nop();

    /* Ready */
#else
    if(IsDeviceBusy() != 0)
        return (0);
#endif

    // Save current color
    colorTemp = GetColor();
    resType = *((WORD *)image);


    switch(resType & (GFX_MEM_MASK | GFX_COMP_MASK))
    {

                #ifdef USE_COMP_RLE
                #ifdef USE_BITMAP_FLASH
        case (FLASH | COMP_RLE):
            
            // Image address
            flashAddress = pimghdr->LOCATION.progByteAddress;
            // Read color depth
            colorDepth = pimghdr->colorDepth;
        
            // Draw picture
            switch(colorDepth)
            {
                    #if (COLOR_DEPTH >= 4)
                case 4: 
                    PutImageRLE4BPP(left, top, flashAddress, stretch); break;
                    #endif
  
                    #if (COLOR_DEPTH >= 8)
                case 8: 
                    PutImageRLE8BPP(left, top, flashAddress, stretch); break;
                    #endif
                        
                    default:    break;
            }
            break;
                #endif //#ifdef USE_BITMAP_FLASH

                #ifdef USE_BITMAP_EXTERNAL
        case (EXTERNAL | COMP_RLE):
        
            // Get color depth
            ExternalMemoryCallback(image, 1, 1, &colorDepth);
    
            // Draw picture
            switch(colorDepth)
            {
                    #if (COLOR_DEPTH >= 4)
                case 4:     
                    PutImageRLE4BPPExt(left, top, image, stretch); break;
                    #endif
                            
                    #if (COLOR_DEPTH >= 8)
                case 8:     
                    PutImageRLE8BPPExt(left, top, image, stretch); break;
                    #endif
                            
                default:    break;
            }
            break;
                #endif //#ifdef USE_BITMAP_EXTERNAL
                #endif // #ifdef USE_COMP_RLE
        
                #ifdef USE_BITMAP_FLASH

        case (FLASH | COMP_NONE):

            // Image address
            flashAddress = ((IMAGE_FLASH *)image)->address;

            // Read color depth
            colorDepth = *(flashAddress + 1);

            // Draw picture
            switch(colorDepth)
            {
                case 1:     
                    PutImage1BPP(left, top, flashAddress, stretch); break;
                
                    #if (COLOR_DEPTH >= 4)
                case 4:     
                    PutImage4BPP(left, top, flashAddress, stretch); break;
                    #endif
                    
                    #if (COLOR_DEPTH >= 8)
                case 8:     
                    PutImage8BPP(left, top, flashAddress, stretch); break;
                    #endif
                    
                    #if (COLOR_DEPTH == 16)
                case 16:    
                    PutImage16BPP(left, top, flashAddress, stretch); break;
                    #endif
                    
                default:    break;

            }

            break;
                #endif // #ifdef USE_BITMAP_FLASH
                
                #ifdef USE_BITMAP_EXTERNAL

        case (EXTERNAL | COMP_NONE):

            // Get color depth
            ExternalMemoryCallback(image, 1, 1, &colorDepth);

            // Draw picture
            switch(colorDepth)
            {
                case 1:     
                    PutImage1BPPExt(left, top, image, stretch); break;
                
                    #if (COLOR_DEPTH >= 4)
                case 4:     
                    PutImage4BPPExt(left, top, image, stretch); break;
                    #endif
                    
                    #if (COLOR_DEPTH >= 8)
                case 8:     
                    PutImage8BPPExt(left, top, image, stretch); break;
                    #endif
                    
                    #if (COLOR_DEPTH == 16)
                case 16:    
                    PutImage16BPPExt(left, top, image, stretch); break;
                    #endif
                    
                default:    break;
            }

            break;
                #endif //#ifdef USE_BITMAP_EXTERNAL

                #if defined (GFX_USE_DISPLAY_CONTROLLER_MCHP_DA210)
                #ifdef USE_COMP_IPU
        case (FLASH | COMP_IPU):
        case (EXTERNAL | COMP_IPU):
        case (EDS_EPMP | COMP_IPU):
                #endif // #ifdef USE_COMP_IPU
        case (EDS_EPMP | COMP_NONE):
        
            // this requires special processing of images in Extended Data Space
            // call the driver specific function to perform the processing
            PutImageDrv(left, top, image, stretch); 
            break;

                #endif //#if defined (__PIC24FJ256DA210__)
            
        default:
            break;
    }

    // Restore current color
    SetColor(colorTemp);
    return (1);
}

/*********************************************************************
* Function:  SHORT GetSineCosine(SHORT v, WORD type)
*
* PreCondition: none
*
* Input: v - the angle used to calculate the sine or cosine value. 
*			 The angle must be in the range of -360 to 360 degrees.
*		 type - sets if the angle calculation is for a sine or cosine
*				- GETSINE (0) - get the value of sine(v).
*				- GETCOSINE (1) - return the value of cosine(v).
*
* Output: Returns the sine or cosine of the angle given.
*
* Side Effects: none
*
* Overview: This calculates the sine or cosine value of the given angle.
*
* Note: none
*
********************************************************************/
SHORT GetSineCosine(SHORT v, WORD type)
{
	// if angle is neg, convert to pos equivalent
    if (v < 0) 
    	v += 360;   					
    //v /= DEGREECOUNT;                 // convert into ticks from degrees, tick = 3 deg
           
    if(v >= COSINETABLEENTRIES * 3)
    {
        v -= COSINETABLEENTRIES * 3;
        return ((type == GETSINE) ? -(_CosineTable[v]) : (_CosineTable[COSINETABLEENTRIES - v]));
    }
    else if(v >= COSINETABLEENTRIES * 2)
    {
        v -= COSINETABLEENTRIES * 2;
        return ((type == GETSINE) ? -(_CosineTable[(COSINETABLEENTRIES - v)]) : -(_CosineTable[v]));
    }
    else if(v >= COSINETABLEENTRIES)
    {
        v -= COSINETABLEENTRIES;
        return ((type == GETSINE) ? (_CosineTable[v]) : -(_CosineTable[COSINETABLEENTRIES - v]));
    }
    else
    {
        return ((type == GETSINE) ? (_CosineTable[COSINETABLEENTRIES - v]) : (_CosineTable[v]));
    }


}

/*********************************************************************
* Function:  WORD DrawArc(SHORT cx, SHORT cy, SHORT r1, SHORT r2, SHORT startAngle, SHORT endAngle)
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
* Overview: This renders an arc with from startAngle to endAngle with the thickness 
*		    of r2-r1. The function returns 1 when the arc is rendered successfuly
* 			and returns a 0 when it is not yet finished. The next call to the 
*			function will continue the rendering.
*
* Note: none
*
********************************************************************/
WORD __attribute__((weak)) DrawArc(SHORT cx, SHORT cy, SHORT r1, SHORT r2, SHORT startAngle, SHORT endAngle)
{

	SHORT i;
	SHORT x1,y1,x2,y2;

	for (i=startAngle; i <= endAngle; i++)
	{
		// get inner arc x,y position
		y1 = (r1*Sine(i))/256;
		x1 = (r1*Cosine(i))/256;

		// get outer arc x,y position
		if (r1 != r2)
		{
			y2 = (r2*Sine(i))/256;
			x2 = (r2*Cosine(i))/256;

			// check if we need to double the line to cover all pixels
			if ((x1 == x2) || (y1 == y2))
			{
				SetLineThickness(NORMAL_LINE);
			}	
			else
			{
				SetLineThickness(THICK_LINE);
			}	
			// draw the lines to fill the arc
			while(!Line(cx+x1, cy+y1, cx+x2, cy+y2));
		}
		else 
		{	
			PutPixel(cx+x1, cy+y1);
		}	
		
	}
	return 1;	
}	


#ifdef USE_GRADIENT
#define WAIT_UNTIL_FINISH(x)    while(!x)

#if (COLOR_DEPTH == 24)
#define GetRed(color)       (((color) & 0xFF0000) >> 16)
#define GetGreen(color)     (((color) & 0x00FF00) >> 8)
#define GetBlue(color)      ((color) & 0x0000FF)
#else
#define GetRed(color)       (((color) & 0xF800) >> 8)
#define GetGreen(color)     (((color) & 0x07E0) >> 3)
#define GetBlue(color)      (((color) & 0x001F) << 3)
#endif

WORD BarGradient(SHORT left, SHORT top, SHORT right, SHORT bottom, GFX_COLOR color1, GFX_COLOR color2, DWORD length, BYTE direction)
{

    WORD startRed, startBlue, startGreen;
    WORD endRed, endBlue, endGreen;

    long rdiff=0,gdiff=0,bdiff=0;
    short i,steps;

    SetColor(color2);

    // if length is 100, why waste the bar call?
    switch(direction)
    {
    case GRAD_UP:
        length = length * (bottom - top);
        length /= 100;
        steps = length;
        WAIT_UNTIL_FINISH(Bar(left,top,right,bottom-steps));
        break;

    case GRAD_DOWN: 
        length = length * (bottom - top); 
        length /= 100; 
        steps = length;    
        WAIT_UNTIL_FINISH(Bar(left,top+steps,right,bottom));    
        break;

    case GRAD_RIGHT:
        length = length * (right - left);
        length /= 100;
        steps = length;
        WAIT_UNTIL_FINISH(Bar(left+steps,top,right,bottom)); 
        break;

    case GRAD_LEFT:
        length = length * (right - left);
        length /= 100;
        steps = length;
        WAIT_UNTIL_FINISH(Bar(left,top,right-steps,bottom)); 
        break;

    case GRAD_DOUBLE_VER:
        steps = (right - left) >> 1;
        break;

    case GRAD_DOUBLE_HOR:
        steps = (bottom - top) >> 1;
        break; 

    default: 
        return 1;
    }

    startRed    = GetRed(color1);
    startGreen  = GetGreen(color1);
    startBlue   = GetBlue(color1);

    endRed      = GetRed(color2);
    endGreen    = GetGreen(color2);
    endBlue     = GetBlue(color2);

    ///////////////////////////////////

    //Find the step size for the red portion//
    rdiff = ((long)endRed - (long)startRed) << 8;
    rdiff /= steps;

    //Find the step size for the green portion//
    gdiff = ((long)endGreen - (long)startGreen) << 8;
    gdiff /= steps;

    //Find the step size for the blue portion//
    bdiff = ((long)endBlue - (long)startBlue) << 8;
    bdiff /= steps;

    short barSize = 1;
    color1 = RGBConvert(startRed, startGreen, startBlue);

    // PERCENTAGE BASED CODE
    for(i=0; i < steps; i++)
    {
        //Calculate the starting RGB values
        endRed      = startRed + ((rdiff*i) >> 8);
        endGreen    = startGreen + ((gdiff*i) >> 8);
        endBlue     = startBlue + ((bdiff*i) >> 8);

        color2 = RGBConvert(endRed, endGreen, endBlue);

        if(color2 == color1)
        {
            barSize++;
            continue;
        }

        SetColor(color2);
        color1 = color2;

        switch(direction)          //This switch statement draws the gradient depending on direction chosen
        {
        case GRAD_DOWN:
            WAIT_UNTIL_FINISH(Bar(left, top, right, top + barSize));
            top += barSize;
        break;
        
        case GRAD_UP:
            WAIT_UNTIL_FINISH(Bar(left,bottom - barSize,right,bottom));
            bottom -= barSize;
        break;

        case GRAD_RIGHT:
            WAIT_UNTIL_FINISH(Bar(left, top, left + barSize, bottom));
            left += barSize;
        break;
        
        case GRAD_LEFT:
            WAIT_UNTIL_FINISH(Bar(right - barSize, top, right, bottom));
            right -= barSize;
        break;

        case GRAD_DOUBLE_VER:
            WAIT_UNTIL_FINISH(Bar(right - barSize, top, right, bottom));
            right -= barSize;
            WAIT_UNTIL_FINISH(Bar(left, top, left + barSize, bottom));
            left += barSize;
        break;

        case GRAD_DOUBLE_HOR:
            WAIT_UNTIL_FINISH(Bar(left, bottom - barSize, right, bottom));
            bottom -= barSize;
            WAIT_UNTIL_FINISH(Bar(left, top, right, top + barSize));
            top += barSize;
        break; 

        default: 
            break;
        }

        barSize = 1;
    }

    if(barSize > 1)
    {

        SetColor(RGBConvert(endRed, endGreen, endBlue));

        switch(direction)          //This switch statement draws the gradient depending on direction chosen
        {
        case GRAD_DOWN:
            WAIT_UNTIL_FINISH(Bar(left, top, right, top + barSize));
        break;
        
        case GRAD_UP:
            WAIT_UNTIL_FINISH(Bar(left,bottom - barSize,right,bottom));
        break;

        case GRAD_RIGHT:
            WAIT_UNTIL_FINISH(Bar(left, top, left + barSize, bottom));
        break;
        
        case GRAD_LEFT:
            WAIT_UNTIL_FINISH(Bar(right - barSize, top, right, bottom));
        break;

        case GRAD_DOUBLE_VER:
            WAIT_UNTIL_FINISH(Bar(right - barSize, top, right, bottom));
            WAIT_UNTIL_FINISH(Bar(left, top, left + barSize, bottom));
        break;

        case GRAD_DOUBLE_HOR:
            WAIT_UNTIL_FINISH(Bar(left, bottom - barSize, right, bottom));
            WAIT_UNTIL_FINISH(Bar(left, top, right, top + barSize));
        break; 

        default: 
            break;
        }

    }
    return 1;
}

WORD BevelGradient(SHORT left, SHORT top, SHORT right, SHORT bottom, SHORT rad, GFX_COLOR color1, GFX_COLOR color2, DWORD length, BYTE direction)
{
    WORD i;
    WORD sred,sblue,sgreen;
    WORD ered,eblue,egreen;
    GFX_COLOR EndColor;
    long rdiff=0,gdiff=0,bdiff=0;
    short steps;
    EndColor = color2;

    switch(direction)     //This switch statement calculates the amount of transitions needed
    {
    case GRAD_UP:
    case GRAD_DOWN:
        length = length * (bottom - top +(rad << 1));
        length /= 100;
        steps = length;
        break;

    case GRAD_RIGHT:
    case GRAD_LEFT:
        length = length * (right - left +(rad << 1));
        length /= 100;
        steps = length;
        break;

    case GRAD_DOUBLE_VER:
        steps = (right - left +(rad << 1)) >> 1;
        break;

    case GRAD_DOUBLE_HOR:
        steps = (bottom - top +(rad << 1)) >> 1;
        break; 

    default: 
        return (1);
    }

    //Calculate the starting RGB values
    sred    = GetRed(color1);
    sgreen  = GetGreen(color1);
    sblue   = GetBlue(color1);
            
    ered    = GetRed(color2);
    egreen  = GetGreen(color2);
    eblue   = GetBlue(color2);

    ///////////////////////////////////

    //Find the step size for the red portion//
    rdiff = ((long)ered - (long)sred) << 8;
    rdiff /= steps;

    //Find the step size for the green portion//
    gdiff = ((long)egreen - (long)sgreen) << 8;
    gdiff /= steps;

    //Find the step size for the blue portion//
    bdiff = ((long)eblue - (long)sblue) << 8;
    bdiff /= steps;

    typedef enum __attribute__((packed)) {
        BEGIN,
        CHECK,
        Q8TOQ1,
        Q7TOQ2,
        Q6TOQ3,
        Q5TOQ4,
        WAITFORDONE,
        FACE
    } FILLCIRCLE_STATES;

    DWORD_VAL temp;
    static LONG err;
    static SHORT yLimit, xPos, yPos;
    static SHORT xCur, yCur, yNew;

    static FILLCIRCLE_STATES state = BEGIN;

    while(1)
    {
        if(IsDeviceBusy())
            return (0);
        switch(state)
        {
            case BEGIN:
                if(!rad)
                {   // no radius object is a filled rectangle
                    state = FACE;
                    break;
                }

                // compute variables
                temp.Val = SIN45 * rad;
                yLimit = temp.w[1];
                temp.Val = (DWORD) (ONEP25 - ((LONG) rad << 16));
                err = (SHORT) (temp.w[1]);
                xPos = rad;
                yPos = 0;
                xCur = xPos;
                yCur = yPos;
                yNew = yPos;
                state = CHECK;

            case CHECK:
                bevel_fill_check : if(yPos > yLimit)
                {
                    state = FACE;
                    break;
                }

                // New records the last y position
                yNew = yPos;

                // calculate the next value of x and y
                if(err > 0)
                {
                    xPos--;
                    err += 5 + ((yPos - xPos) << 1);
                }
                else
                    err += 3 + (yPos << 1);
                yPos++;
                state = Q6TOQ3;

            case Q6TOQ3:
                if(xCur != xPos)
                {

                    // 6th octant to 3rd octant
	            	//if (_bevelDrawType & DRAWBOTTOMBEVEL) 
	            	{ 
                        if(direction == GRAD_DOUBLE_VER || direction == GRAD_DOUBLE_HOR)
                            i = (top - yCur) - top + rad;
                        else
                            i = (bottom + yCur) - top + rad;
                
                        ered = sred + ((rdiff*i) >> 8);
                        egreen = sgreen + ((gdiff*i) >> 8);
                        eblue = sblue + ((bdiff*i) >> 8);
 
                        color2 = RGBConvert(ered,egreen,eblue);
                                         
                        SetColor(color2);
            
                        switch(direction)    //Direction matter because different portions of the circle are drawn
                        {
                           case GRAD_LEFT:
                               if(i>length) SetColor(EndColor);
                               if(Bar(left - yNew, top - xCur, left - yCur, bottom + xCur) == 0) return (0);
                                break;
                           
                           case GRAD_RIGHT:                     
                           case GRAD_DOUBLE_VER:
                                if(i>length) 
                                    SetColor(EndColor);
                                
                                if(Bar(right + yCur, top - xCur, right + yNew, bottom + xCur) == 0)
                                    return (0);           
                                break;
                           
                           case GRAD_UP:
                                if(i>length) 
                                    SetColor(EndColor);
                                
                                if(Bar(left - xCur, top - yNew, right + xCur, top - yCur) == 0) 
                                    return (0);
                                break;

                           case GRAD_DOWN:
                           case GRAD_DOUBLE_HOR:
                                if(i>length) 
                                    SetColor(EndColor);
                                if(Bar(left - xCur, bottom + yCur, right + xCur, bottom + yNew) == 0)
                                    return (0);
                           
                           default: 
                               break;
                        }

                  	}          	
                    state = Q5TOQ4;
                    break;
                }

                state = CHECK;
                goto bevel_fill_check;

            case Q5TOQ4:

            	//if (_bevelDrawType & DRAWBOTTOMBEVEL) 
            	{ 
                    if(direction == GRAD_DOUBLE_VER || direction == GRAD_DOUBLE_HOR)
                    i = top + xPos - top + rad;
                    else

	                // 5th octant to 4th octant
                    i = (bottom + xPos) - top + rad ;

                    //Calculate the starting RGB values
                    ered = sred + ((rdiff*i) >> 8);
                    egreen = sgreen + ((gdiff*i) >> 8);
                    eblue = sblue + ((bdiff*i) >> 8);
                    
                    color2 = RGBConvert(ered,egreen,eblue);
                    SetColor(color2);

                    switch(direction)    //Direction matter because different portions of the circle are drawn
                    {
                       case GRAD_LEFT:
                           if(i>length) 
                               SetColor(EndColor);
                           if(Bar(left - xCur, top - yNew, left - xPos, bottom + yNew) == 0) 
                               return (0);
                            break;
                       
                       case GRAD_RIGHT:                     
                       case GRAD_DOUBLE_VER:
                           if(i>length) 
                               SetColor(EndColor);
                           if(Bar(right + xPos, top - yNew, right + xCur, bottom + yNew) == 0)
                               return (0);           
                            break;
                       
                       case GRAD_UP:
                            if(i>length) 
                                SetColor(EndColor);
                            if(Bar(left - yNew, top - xCur, right + yNew, top - xPos) == 0) 
                                return (0);
                            break;

                       case GRAD_DOWN:
                       case GRAD_DOUBLE_HOR:
                            if(i>length) 
                                SetColor(EndColor);
                            if(Bar(left - yNew, bottom + xPos, right + yNew, bottom + xCur) == 0)
                                return (0);
                       default: 
                           break;
                    }

				}
                state = Q8TOQ1;
                break;

            case Q8TOQ1:

                // 8th octant to 1st octant
	            //if (_bevelDrawType & DRAWTOPBEVEL) 
				{
                    i = (top - xCur) - top + rad;

                    //Calculate the starting RGB values
                    ered = sred + ((rdiff*i) >> 8);
                    egreen = sgreen + ((gdiff*i) >> 8);
                    eblue = sblue + ((bdiff*i) >> 8);

                    color2 = RGBConvert(ered,egreen,eblue);
                    SetColor(color2);

                    switch(direction)    //Direction matter because different portions of the circle are drawn
                    {
                       case GRAD_LEFT:
                           if(i>length) 
                               SetColor(EndColor);
                           if(Bar(right + xPos, top - yNew, right + xCur, bottom + yNew) == 0) 
                               return (0);
                            break;
                       
                       case GRAD_RIGHT:                     
                       case GRAD_DOUBLE_VER:
                           if(i>length) 
                               SetColor(EndColor);
                           if(Bar(left - xCur, top - yNew, left - xPos, bottom + yNew) == 0) 
                               return (0);           
                            break;
                       
                       case GRAD_UP:
                            if(i>length) 
                                SetColor(EndColor);
                            if(Bar(left - yNew, bottom + xPos, right + yNew, bottom + xCur) == 0) 
                                return (0);
                            break;

                       case GRAD_DOWN:
                       case GRAD_DOUBLE_HOR:
                            if(i>length) 
                                SetColor(EndColor);
                            if(Bar(left - yNew, top - xCur, right + yNew, top - xPos) == 0)
                                return (0);
                            break;
                       
                       default: 
                           break;
                    }

    			}                	
                state = Q7TOQ2;
                break;

            case Q7TOQ2:

                // 7th octant to 2nd octant
	            //if (_bevelDrawType & DRAWTOPBEVEL) 
				{             
                    i = (top - yNew) - top + rad;

                    //Calculate the starting RGB values
                    ered = sred + ((rdiff*i) >> 8);
                    egreen = sgreen + ((gdiff*i) >> 8);
                    eblue = sblue + ((bdiff*i) >> 8);

                    color2 = RGBConvert(ered,egreen,eblue);
                    
                    SetColor(color2);
         
                    switch(direction)    //Direction matter because different portions of the circle are drawn
                    {
                       case GRAD_LEFT:
                           if(i>length) 
                               SetColor(EndColor);
                           if(Bar(right + yCur, top - xCur, right + yNew, bottom + xCur) == 0) 
                               return (0);
                            break;
                       
                       case GRAD_RIGHT:                     
                       case GRAD_DOUBLE_VER:
                           if(i>length) 
                               SetColor(EndColor);
                           if(Bar(left - yNew, top - xCur, left - yCur, bottom + xCur) == 0) 
                               return (0);           
                            break;
                       
                       case GRAD_UP:
                            if(i>length) 
                                SetColor(EndColor);
                            if(Bar(left - xCur, bottom + yCur, right + xCur, bottom + yNew) == 0) 
                                return (0);
                            break;

                       case GRAD_DOWN:
                       case GRAD_DOUBLE_HOR:
                            if(i>length) 
                                SetColor(EndColor);
                            if(Bar(left - xCur, top - yNew, right + xCur, top - yCur) == 0) 
                                return (0);
                            break;
                       
                       default: 
                           break;
                    }

				}
                // update current values
                xCur = xPos;
                yCur = yPos;
                state = CHECK;
                break;

 

            case FACE:
                if((right - left) || (bottom - top))
                {
                i = (top) - top + rad;
                //Calculate the starting RGB values
                ered = sred + ((rdiff*i) >> 8);
                egreen = sgreen + ((gdiff*i) >> 8);
                eblue = sblue + ((bdiff*i) >> 8);

                color1 = RGBConvert(ered,egreen,eblue);
 
                if(i>length) 
                    color1 = EndColor;  

                i = (bottom) - top + rad;
                //Calculate the ending RGB values
                ered = sred + ((rdiff*i) >> 8);
                egreen = sgreen + ((gdiff*i) >> 8);
                eblue = sblue + ((bdiff*i) >> 8);

                color2 = RGBConvert(ered,egreen,eblue);
 
                if(i>length) 
                    color2 = EndColor;
      
                length -= rad;   //Subtract the radius from the length needed for gradient   
              
                if(direction == GRAD_UP || direction == GRAD_DOWN || direction == GRAD_DOUBLE_HOR)
                {
                   if(length>= bottom-top)
                   {
                       length = 100;
                   }
                   else
                   {
                       length *= 100;
                       length /= (bottom -top);
                   }               
                   BarGradient(left-rad, top, right+rad, bottom,color1,color2,length,direction);
                }
                else
                {
                  if(length>=right-left)
                  {
                      length = 100;
                  }
                  else
                  {
                       length *= 100;
                       length /= (right - left);
                  }
                   BarGradient(left, top-rad, right, bottom+rad,color1,color2,length,direction);
                }
            
                    state = WAITFORDONE;
                }
                else
                {
                    state = BEGIN;
                    return (1);
                }

            case WAITFORDONE:
                if(IsDeviceBusy())
                    return (0);
                state = BEGIN;
                return (1);
        }           // end of switch
    }               // end of while
  
}
#endif


#ifdef USE_COMP_RLE

    #ifdef USE_BITMAP_FLASH

        #if (COLOR_DEPTH >= 8)
/*********************************************************************
* Function: WORD DecodeRLE8(FLASH_BYTE *flashAddress, BYTE *pixelrow, WORD size)
*
* PreCondition: tempFlashAddress should point to the beginning of a RLE compressed block
*
* Input: flashAddress - Address of the beginning of a RLE compressed block
*        pixelrow - Pointer to an array where the decoded row must be stored
*        size - Size of the decoded data in bytes
*
* Output: Number of source bytes traversed
*
* Side Effects: none
*
* Overview: Decodes the data
*
********************************************************************/
WORD DecodeRLE8(FLASH_BYTE *flashAddress, BYTE *pixel_row, WORD size)
{
    WORD sourceOffset = 0;
    WORD decodeSize = 0;

    while(decodeSize < size)
    {
        BYTE code = *flashAddress++;
        BYTE value = *flashAddress++;
        sourceOffset += 2;
        
        if(code > 0)
        {
            decodeSize += code;
            
            if(decodeSize > size) //To avoid writing oustide the array bounds
            {
                code -= (decodeSize - size);
            }
            
            while(code)
            {
                *pixel_row++ = value;
                code--;
            }
        }
        else
        {
            decodeSize += value;
            sourceOffset += value;
            
            if(decodeSize > size) //To avoid writing oustide the array bounds
            {
                value -= (decodeSize - size);
            }
            
            while(value)
            {
                *pixel_row++ = *flashAddress++;
                value--;
            }
        }
    }    
    return (sourceOffset);
}

/*********************************************************************
* Function: void PutImageRLE8BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - Should be NORMAL when RLE is used
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs the image starting from left,top coordinates
*
* Note: image must be located in internal memory
*
********************************************************************/
void PutImageRLE8BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch)
{
    register FLASH_BYTE *flashAddress;
    register FLASH_BYTE *tempFlashAddress;
    WORD                sizeX, sizeY;
    WORD                x, y;
    WORD                xc, yc;
    BYTE                temp;
    BYTE                stretchX, stretchY;
    GFX_COLOR           pallete[256];
    WORD                counter;
    BYTE                pixelrow[GetMaxX() + 1];
    WORD                offset;
    BYTE                *pixelAddress;

    // Move pointer to size information
    flashAddress = image + 2;

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;

    #if COLOR_DEPTH == 24
    flashAddress += 2;      // pad word for alignment
    #endif

    // Read pallete
    for(counter = 0; counter < 256; counter++)
    {
        #if COLOR_DEPTH == 16
        pallete[counter] = *((FLASH_WORD *)flashAddress);
        flashAddress += 2;
        #endif

        #if COLOR_DEPTH == 24
        pallete[counter] = *((FLASH_DWORD *)flashAddress);
        flashAddress += 4;
        #endif
    }

    yc = top;
    tempFlashAddress = flashAddress;
    for(y = 0; y < sizeY; y++)
    {
        offset = DecodeRLE8(tempFlashAddress, pixelrow, sizeX);
        tempFlashAddress += offset;
        
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            pixelAddress = pixelrow;
            xc = left;
            for(x = 0; x < sizeX; x++)
            {

                // Read pixels from flash
                temp = *pixelAddress;
                pixelAddress++;

                // Set color
            #ifdef USE_PALETTE
                SetColor(temp);
            #else
                SetColor(pallete[temp]);
            #endif

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    #ifdef USE_TRANSPARENT_COLOR
                        if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                    #endif
                            PutPixel(xc, yc);
                        xc++;    
                }
            }

            yc++;
        }
    }
}
        #endif //#if (COLOR_DEPTH >= 8)

        #if (COLOR_DEPTH >= 4)
        
/*********************************************************************
* Function: WORD DecodeRLE4(FLASH_BYTE *flashAddress, BYTE *pixelrow, WORD size)
*
* PreCondition: tempFlashAddress should point to the beginning of a RLE compressed block
*
* Input: flashAddress - Address of the beginning of a RLE compressed block
*        pixelrow - Pointer to an array where the decoded row must be stored
*        size - Size of the decoded data in bytes
*
* Output: Number of source bytes traversed
*
* Side Effects: none
*
* Overview: Decodes the data
*
********************************************************************/
WORD DecodeRLE4(FLASH_BYTE *flashAddress, BYTE *pixel_row, WORD size)
{
    WORD sourceOffset = 0;
    WORD decodeSize = 0;

    while(decodeSize < size)
    {
        BYTE code = *flashAddress++;
        BYTE value = *flashAddress++;
        BYTE counter;
        
        sourceOffset += 2;
        
        if(code > 0)
        {
            decodeSize += code;
            
            if(decodeSize > size) //To avoid writing oustide the array bounds
            {
                code -= (decodeSize - size);
            }
            
            for(counter = 0; counter < code; counter++)
            {
                if(counter & 0x01)
                {
                    *pixel_row++ = (value >> 4) & 0x0F;
                }
                else
                {
                    *pixel_row++ = (value) & 0x0F;
                }
            }
        }
        else
        {
            decodeSize += value;
            sourceOffset += (value + 1) >> 1;
            
            if(decodeSize > size) //To avoid writing oustide the array bounds
            {
                value -= (decodeSize - size);
            }
            
            for(counter = 0; counter < value; counter++)
            {
                if(counter & 0x01)
                {
                    *pixel_row++ = (*flashAddress >> 4) & 0x0F;
                    flashAddress++;
                }
                else
                {
                    *pixel_row++ = (*flashAddress) & 0x0F;
                }
            }
        }
    }

    return (sourceOffset);
}

/*********************************************************************
* Function: void PutImageRLE4BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - Should be NORMAL when RLE is used
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs the image starting from left,top coordinates
*
* Note: image must be located in internal memory
*
********************************************************************/
void PutImageRLE4BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch)
{
    register FLASH_BYTE *flashAddress;
    register FLASH_BYTE *tempFlashAddress;
    WORD                sizeX, sizeY;
    register WORD       x, y;
    WORD                xc, yc;
    BYTE                temp = 0;
    register BYTE       stretchX, stretchY;
    GFX_COLOR           pallete[16];
    WORD                counter;
    BYTE                pixelrow[GetMaxX() + 1];
    WORD                offset;
    BYTE                *pixelAddress;

    // Move pointer to size information
    flashAddress = image + 2;

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;

    #if COLOR_DEPTH == 24
    flashAddress += 2;          // pad for alignment
    #endif

    // Read pallete
    for(counter = 0; counter < 16; counter++)
    {
        #if COLOR_DEPTH == 16    
        pallete[counter] = *((FLASH_WORD *)flashAddress);
        flashAddress += 2;
        #endif

        #if COLOR_DEPTH == 24
        pallete[counter] = *((FLASH_DWORD *)flashAddress);
        flashAddress += 4;
        #endif
    }

    yc = top;
    tempFlashAddress = flashAddress;
    for(y = 0; y < sizeY; y++)
    {
        offset = DecodeRLE4(tempFlashAddress, pixelrow, sizeX);
        tempFlashAddress += offset;

        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            pixelAddress = pixelrow;
            xc = left;
            for(x = 0; x < sizeX; x++)
            {
                temp = *pixelAddress;
                pixelAddress++;

                #ifdef USE_PALETTE
                    SetColor(temp);
                #else
                    SetColor(pallete[temp]);
                #endif

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    #ifdef USE_TRANSPARENT_COLOR
                        if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                    #endif
                            PutPixel(xc, yc);
                        xc++;    
                }
            }

            yc++;
        }
    }
}
        #endif //#if (COLOR_DEPTH >= 4)

    #endif //    #ifdef USE_BITMAP_FLASH

    #ifdef USE_BITMAP_EXTERNAL
    
        #if (COLOR_DEPTH >= 8)

/*********************************************************************
* Function: WORD DecodeRLE8Ext(void *image, DWORD  memAddress, BYTE *pixelrow, WORD size)
*
* PreCondition: tempFlashAddress should point to the beginning of a RLE compressed block
*
* Input: image - External memory image pointer
*        memAddress - Address of the beginning of a RLE compressed block
*        pixelrow - Pointer to an array where the decoded row must be stored
*        size - Size of the decoded data in bytes
*
* Output: Number of source bytes traversed
*
* Side Effects: none
*
* Overview: Decodes the data from the external flash
*
********************************************************************/
WORD DecodeRLE8Ext(void *image, DWORD memAddress, BYTE *pixel_row, WORD size)
{
    WORD sourceOffset = 0;
    WORD decodeSize = 0;

    while(decodeSize < size)
    {
        BYTE code, value;
        
        ExternalMemoryCallback(image, memAddress, sizeof(BYTE), &code);
        memAddress++;
        ExternalMemoryCallback(image, memAddress, sizeof(BYTE), &value);
        memAddress++;

        sourceOffset += 2;
        
        if(code > 0)
        {
            decodeSize += code;
            
            if(decodeSize > size) //To avoid writing oustide the array bounds
            {
                code -= (decodeSize - size);
            }
            
            while(code)
            {
                *pixel_row++ = value;
                code--;
            }
        }
        else
        {
            decodeSize += value;
            sourceOffset += value;
            
            if(decodeSize > size) //To avoid writing oustide the array bounds
            {
                value -= (decodeSize - size);
            }
            
            ExternalMemoryCallback(image, memAddress, value * sizeof(BYTE), pixel_row);
            pixel_row += value;
            memAddress += value;
        }
    }    
    return (sourceOffset);
}

/*********************************************************************
* Function: void PutImageRLE8BPPExt(SHORT left, SHORT top, void* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
void PutImageRLE8BPPExt(SHORT left, SHORT top, void *image, BYTE stretch)
{
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    WORD            pallete[256];
    BYTE            pixelrow[(GetMaxX() + 1)];
    BYTE            *pixelAddress;

    BYTE            temp;
    WORD            sizeX, sizeY;
    WORD            x, y;
    WORD            xc, yc;
    BYTE            stretchX, stretchY;
    WORD            offset;

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Get pallete (256 entries)
    ExternalMemoryCallback(image, sizeof(BITMAP_HEADER), 256 * sizeof(WORD), pallete);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 256 * sizeof(WORD);

    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;

    yc = top;
    for(y = 0; y < sizeY; y++)
    {
        offset = DecodeRLE8Ext(image, memOffset, pixelrow, sizeX);
        memOffset += offset;
        
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            pixelAddress = pixelrow;
            xc = left;
            for(x = 0; x < sizeX; x++)
            {

                // Read pixels from flash
                temp = *pixelAddress;
                pixelAddress++;

                // Set color
            #ifdef USE_PALETTE
                SetColor(temp);
            #else
                SetColor(pallete[temp]);
            #endif

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    #ifdef USE_TRANSPARENT_COLOR
                        if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                    #endif
                            PutPixel(xc, yc);
                        xc++;    
                }
            }

            yc++;
        }
    }
}
        #endif //#if (COLOR_DEPTH >= 8)
        
        #if (COLOR_DEPTH >= 4)

/*********************************************************************
* Function: WORD DecodeRLE4Ext(void *image, DWORD memAddress, BYTE *pixelrow, WORD size)
*
* PreCondition: tempFlashAddress should point to the beginning of a RLE compressed block
*
* Input: image - External memory image pointer
*        memAddress - Address of the beginning of a RLE compressed block
*        pixelrow - Pointer to an array where the decoded row must be stored
*        size - Size of the decoded data in bytes
*
* Output: Number of source bytes traversed
*
* Side Effects: none
*
* Overview: Decodes the data
*
********************************************************************/
WORD DecodeRLE4Ext(void *image, DWORD memAddress, BYTE *pixel_row, WORD size)
{
    WORD sourceOffset = 0;
    WORD decodeSize = 0;

    while(decodeSize < size)
    {
        BYTE code, value;
        BYTE counter, temp;
        
        ExternalMemoryCallback(image, memAddress, sizeof(BYTE), &code);
        memAddress++;
        ExternalMemoryCallback(image, memAddress, sizeof(BYTE), &value);
        memAddress++;
        
        sourceOffset += 2;
        
        if(code > 0)
        {
            decodeSize += code;
            
            if(decodeSize > size) //To avoid writing oustide the array bounds
            {
                code -= (decodeSize - size);
            }
            
            for(counter = 0; counter < code; counter++)
            {
                if(counter & 0x01)
                {
                    *pixel_row++ = (value >> 4) & 0x0F;
                }
                else
                {
                    *pixel_row++ = (value) & 0x0F;
                }
            }
        }
        else
        {
            decodeSize += value;
            sourceOffset += (value + 1) >> 1;
            
            if(decodeSize > size) //To avoid writing oustide the array bounds
            {
                value -= (decodeSize - size);
            }
            
            for(counter = 0; counter < value; counter++)
            {
                if(counter & 0x01)
                {
                    *pixel_row++ = (temp >> 4) & 0x0F;
                    memAddress++;
                }
                else
                {
                    ExternalMemoryCallback(image, memAddress, sizeof(BYTE), &temp);
                    *pixel_row++ = temp & 0x0F;
                }
            }
        }
    }

    return (sourceOffset);
}

/*********************************************************************
* Function: void PutImageRLE4BPPExt(SHORT left, SHORT top, void* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
void PutImageRLE4BPPExt(SHORT left, SHORT top, void *image, BYTE stretch)
{
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    WORD            pallete[16];
    BYTE            pixelrow[(GetMaxX() + 1)];
    BYTE            *pixelAddress;

    BYTE            temp;
    WORD            sizeX, sizeY;
    WORD            x, y;
    WORD            xc, yc;
    BYTE            stretchX, stretchY;
    WORD            offset;

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Get pallete (16 entries)
    ExternalMemoryCallback(image, sizeof(BITMAP_HEADER), 16 * sizeof(WORD), pallete);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 16 * sizeof(WORD);

    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;

    yc = top;
    for(y = 0; y < sizeY; y++)
    {
        offset = DecodeRLE4Ext(image, memOffset, pixelrow, sizeX);
        memOffset += offset;
        
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            pixelAddress = pixelrow;
            xc = left;
            for(x = 0; x < sizeX; x++)
            {

                // Read pixels from flash
                temp = *pixelAddress;
                pixelAddress++;

                // Set color
            #ifdef USE_PALETTE
                SetColor(temp);
            #else
                SetColor(pallete[temp]);
            #endif

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    #ifdef USE_TRANSPARENT_COLOR
                        if (!((GetTransparentColor() == GetColor()) && (GetTransparentColorStatus() == TRANSPARENT_COLOR_ENABLE)))
                    #endif
                            PutPixel(xc, yc);
                        xc++;    
                }
            }

            yc++;
        }
    }
}
        #endif //#if (COLOR_DEPTH >= 4)

    #endif //#ifdef USE_BITMAP_EXTERNAL
    
#endif //#ifdef USE_COMP_RLE

void GetCirclePoint(SHORT radius, SHORT angle, SHORT *x, SHORT *y)
{
    DWORD   rad;
    SHORT   ang;
    SHORT   temp;

    ang = angle % 45;
    if((angle / 45) & 0x01)
        ang = 45 - ang;

    rad = radius;
    // there is a shift by 8 bits here since this function assumes a shift on the calculations for accuracy
    // and to avoid long and complex multiplications.
    rad *= ((DWORD) GetSineCosine(ang, GETCOSINE) << 8);

    *x = ((DWORD_VAL) rad).w[1];
    rad = radius;
    rad *= ((DWORD) GetSineCosine(ang, GETSINE) << 8);

    *y = ((DWORD_VAL) rad).w[1];

    if(((angle > 45) && (angle < 135)) || ((angle > 225) && (angle < 315)))
    {
        temp = *x;
        *x = *y;
        *y = temp;
    }

    if((angle > 90) && (angle < 270))
    {
        *x = -*x;
    }

    if((angle > 180) && (angle < 360))
    {
        *y = -*y;
    }
}
