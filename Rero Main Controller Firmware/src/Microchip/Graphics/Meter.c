/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Meter
 *****************************************************************************
 * FileName:        Meter.c
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 Version 3.00, MPLAB C32
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
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS" WITHOUT WARRANTY OF ANY
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
 * 11/12/07	    Version 1.0 release
 * 07/31/08	    Added arc colors options
 * 08/20/08	    Added accuracy option for displaying values
 * 04/01/11     Removed sineTable[], use the one in the Primitive.c instead.
 *****************************************************************************/

//RERO CHANGE NOTES: 
// MoveTo(pMtr->xCenter - (temp >> 1), pMtr->yCenter + pMtr->radius + 10 + GetTextHeight(pMtr->hdr.pGolScheme->pFont));
//	else if (i == 270)
//	{
//		 MoveTo(x1 - (x2 >> 1), y1);
//	}

//	pMtr->xPos = ((x2) * (((i == 0) ? x1 : y1) >> 7)) + pMtr->xCenter;
//	pMtr->yPos = ((y2) * (((i == 0) ? y1 : x1) >> 7)) + pMtr->yCenter;
//


#include "Graphics/Graphics.h"
#include <math.h>
#include <stdio.h>

#ifdef USE_METER

/* Internal Functions */
void    MtrCalcDimensions(METER *pMtr); // used to calculate the meter dimensions

// which is dependent on meter type

/*********************************************************************
* Function: METER  *MtrCreate(WORD ID, SHORT left, SHORT top, SHORT right, 
*							  SHORT bottom, WORD state, SHORT value, 
*							  SHORT minValue, SHORT maxValue, XCHAR *pText, 
*							  GOL_SCHEME *pScheme)				 
*
*
* Notes: Creates a METER object and adds it to the current active list.
*        If the creation is successful, the pointer to the created Object 
*        is returned. If not successful, NULL is returned.
*
********************************************************************/
METER *MtrCreate
(
    WORD        ID,
    SHORT       left,
    SHORT       top,
    SHORT       right,
    SHORT       bottom,
    WORD        state,
    SHORT       value,
    SHORT       minValue,
    SHORT       maxValue,
    WORD        angleLimitCCW,
    WORD        angleLimitCW,
    void        *pTitleFont,
    void        *pValueFont,
    XCHAR       *pText,
    GOL_SCHEME  *pScheme
)
{
    METER   *pMtr = NULL;

    pMtr = (METER *)GFX_malloc(sizeof(METER));
    if(pMtr == NULL)
        return (NULL);

    pMtr->hdr.ID = ID;          // unique id assigned for referencing
    pMtr->hdr.pNxtObj = NULL;   // initialize pointer to NULL
    pMtr->hdr.type = OBJ_METER; // set object type
    pMtr->hdr.left = left;      // left,top coordinate
    pMtr->hdr.top = top;        //
    pMtr->hdr.right = right;    // right,bottom coordinate
    pMtr->hdr.bottom = bottom;  //
    pMtr->minValue = minValue;
    pMtr->maxValue = maxValue;
    pMtr->angleLimitCCW = angleLimitCCW;
    pMtr->angleLimitCW = angleLimitCW;
    pMtr->value = value;
    pMtr->hdr.state = state;    // state
    pMtr->pText = pText;
    pMtr->hdr.DrawObj = MtrDraw;		// draw function	
    pMtr->hdr.MsgObj = MtrTranslateMsg;         // message function
    pMtr->hdr.MsgDefaultObj = MtrMsgDefault;    // default message function
    pMtr->hdr.FreeObj = NULL;			// free function

    // set the default scale colors
    //MtrSetScaleColors(pMtr, LIGHTGREEN, YELLOW, BRIGHTGREEN, BRIGHTBLUE, RED, BRIGHTRED);
    //RERO:
    MtrSetScaleColors(pMtr, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK);

    // Set the color scheme to be used
    if(pScheme == NULL)
        pMtr->hdr.pGolScheme = _pDefaultGolScheme;
    else
        pMtr->hdr.pGolScheme = pScheme;

    // Set the Title Font to be used
    if(pTitleFont == NULL)
        pMtr->pTitleFont = (void *) &FONTDEFAULT;
    else
        pMtr->pTitleFont = pTitleFont;

    // Set the Value Font to be used
    if(pValueFont == NULL)
        pMtr->pValueFont = (void *) &FONTDEFAULT;
    else
        pMtr->pValueFont = pValueFont;

    // calculate dimensions of the meter	
    MtrCalcDimensions(pMtr);

    GOLAddObject((OBJ_HEADER *)pMtr);

    return (pMtr);
}

/*********************************************************************
* Function: MtrCalcDimensions(void)
*
* Notes: Calculates the dimension of the meter. Dependent on the 
*		 meter type set.
*
********************************************************************/
void MtrCalcDimensions(METER *pMtr)
{
    SHORT   tempHeight, tempWidth;
    SHORT   left, top, right, bottom;
    XCHAR   tempChar[2] = {'8',0};

    left = pMtr->hdr.left;
    right = pMtr->hdr.right;
    top = pMtr->hdr.top;
    bottom = pMtr->hdr.bottom;

    // get the text width reference. This is used to scale the meter
    if(pMtr->pText != NULL)
    {
        tempHeight = (GOL_EMBOSS_SIZE << 1) + GetTextHeight(pMtr->hdr.pGolScheme->pFont);
    }
    else
    {
        tempHeight = (GOL_EMBOSS_SIZE << 1);
    }

    tempWidth = (GOL_EMBOSS_SIZE << 1) + (GetTextWidth(tempChar, pMtr->hdr.pGolScheme->pFont) * SCALECHARCOUNT);

    // Meter size is dependent on the width or height.
    // The radius is also adjusted to add space for the scales
        #if (METER_TYPE == MTR_WHOLE_TYPE)

    // choose the radius
    if((right - left - tempWidth) > (bottom - top - tempHeight - GetTextHeight(pMtr->pTitleFont)))
    {
        pMtr->radius = ((bottom - top - tempHeight - GetTextHeight(pMtr->pTitleFont)) >> 1) - ((tempHeight + bottom - top) >> 3);
    }
    else
        pMtr->radius = ((right - left) >> 1) - (tempWidth + ((right - left) >> 3));

    // center the meter on the given dimensions
    pMtr->xCenter = (left + right) >> 1;
    pMtr->yCenter = ((bottom + top) >> 1) - (tempHeight >> 1);

        #elif (METER_TYPE == MTR_HALF_TYPE)

    // choose the radius
    if((right - left) >> 1 > (bottom - top))
    {
        pMtr->radius = (bottom - top) - ((tempHeight << 1) + ((bottom - top) >> 3));
        pMtr->yCenter = ((bottom + top) >> 1) + ((pMtr->radius + ((bottom - top) >> 3)) >> 1);
    }
    else
    {
        pMtr->radius = ((right - left) >> 1) - (tempWidth + ((right - left) >> 3));
        pMtr->yCenter = ((bottom + top) >> 1) + ((pMtr->radius + ((right - left) >> 3)) >> 1);
    }

    // center the meter on the given dimensions
    pMtr->xCenter = (left + right) >> 1;

        #elif (METER_TYPE == MTR_QUARTER_TYPE)

    // choose the radius
    if
    (
        (right - left - tempWidth) >
            (bottom - top - (GetTextHeight(pMtr->pTitleFont) + GetTextHeight(pMtr->hdr.pGolScheme->pFont))) -
                (GOL_EMBOSS_SIZE << 1)
    )
    {
        pMtr->radius = bottom - top - (GetTextHeight(pMtr->pTitleFont) + GetTextHeight(pMtr->hdr.pGolScheme->pFont) + (GOL_EMBOSS_SIZE << 1));
    }
    else
    {
        pMtr->radius = right -
            left -
            (GetTextWidth(tempChar, pMtr->hdr.pGolScheme->pFont) * (SCALECHARCOUNT + 1)) -
            GOL_EMBOSS_SIZE;
    }

    pMtr->radius -= (((pMtr->radius) >> 2) + GOL_EMBOSS_SIZE);

    // center the meter on the given dimensions
    pMtr->xCenter = ((left + right) >> 1) - ((pMtr->radius + tempWidth + (pMtr->radius >> 2)) >> 1);
    pMtr->yCenter = ((top + bottom) >> 1) + ((pMtr->radius + (pMtr->radius >> 2)) >> 1);
        #endif
}

/*********************************************************************
* Function: MtrSetVal(METER *pMtr, SHORT newVal)
*
* Notes: Sets the value of the meter to newVal. If newVal is less
*		 than 0, 0 is assigned. If newVal is greater than range,
*		 range is assigned.
*
********************************************************************/
void MtrSetVal(METER *pMtr, SHORT newVal)
{
    if((newVal < 0) || (newVal < pMtr->minValue))
    {
        pMtr->value = pMtr->minValue;
        return;
    }

    if(newVal > pMtr->maxValue)
    {
        pMtr->value = pMtr->maxValue;
        return;
    }

    pMtr->value = newVal;
}

/*********************************************************************
* Function: MtrMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG* pMsg)
*
* Notes: This the default operation to change the state of the meter.
*		 Called inside GOLMsg() when GOLMsgCallback() returns a 1.
*
********************************************************************/
void MtrMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg)
{
    METER *pMtr;
    pMtr = (METER *)pObj;

    if(translatedMsg == MTR_MSG_SET)
    {
        MtrSetVal(pMtr, pMsg->param2);      // set the value
        SetState(pMtr, MTR_DRAW_UPDATE);    // update the meter
    }
//    else if(translatedMsg == MTR_MSG_CW || translatedMsg == MTR_MSG_CCW)
//    {
//        MtrSetVal(pMtr, pMtr->degree);      // set the value
//        SetState(pMtr, MTR_DRAW_UPDATE);    // update the meter
//    }
}

/*********************************************************************
* Function: WORD MtrTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
* Notes: Evaluates the message if the object will be affected by the 
*		 message or not.
*
********************************************************************/
WORD MtrTranslateMsg(void *pObj, GOL_MSG *pMsg)
{
    METER *pMtr;
    pMtr = (METER *)pObj;

    WORD    messageID = OBJ_MSG_INVALID;
    SHORT   touchRadius, touchX, touchY, x1, y1, x2, y2, s, deg;
    static SHORT    prevX = -1, prevY = -1;

    // Evaluate if the message is for the meter
    // Check if disabled first
    if(GetState(pMtr, MTR_DISABLED))
        return (OBJ_MSG_INVALID);

    if(pMsg->type == TYPE_SYSTEM)
    {
        if(pMsg->param1 == pMtr->hdr.ID)
        {
            if(pMsg->uiEvent == EVENT_SET)
            {
                messageID = MTR_MSG_SET;
            }
        }
    }
    else if(pMsg->type == TYPE_TOUCHSCREEN) //RERO:
    {
        if((pMsg->uiEvent == EVENT_PRESS) || (pMsg->uiEvent == EVENT_MOVE))
        {
            // Check if it falls in the dial's face
            // to check this the x,y position must be within the circle
            // (x - xCenter)^2 + (y - yCenter)^2 = r^2  where x and y are the points
            // to test, the distance between x,y position of touch and center must be
            // greater than or equal to the radius of the dial
            if
            (
                ((pMtr->xCenter - (pMtr->radius + 30)) < pMsg->param1) &&
                ((pMtr->xCenter + (pMtr->radius + 30)) > pMsg->param1) &&
                ((pMtr->yCenter - (pMtr->radius + 30)) < pMsg->param2) &&
                ((pMtr->yCenter + (pMtr->radius + 30)) > pMsg->param2)
            )
            {
                touchX = abs(pMsg->param1 - pMtr->xCenter);
                touchY = abs(pMsg->param2 - pMtr->yCenter);
                touchRadius = sqrt(touchX * touchX + touchY * touchY);

                // this makes the sampling area a ring where the max radius is the dial radius
                // and min radius is 5
                if((touchRadius > 5) && (touchRadius <= (pMtr->radius + 30)))
                {

//                    // difference of 3 is used to remove jitter caused by noise or sensitivity of the touchscreen
//                    if((abs(prevX - pMsg->param1) > 3) || (abs(prevY - pMsg->param2) > 3))
//                    {

                        // The first MOVE event is used to record the current position only. The second MOVE event
                        // will be used together with the previous MOVE and determine if the movement is in the
                        // clockwise or counter clockwise direction.
//                        if((prevX == -1) || (prevY == -1))
//                        {
//                            prevX = pMsg->param1;
//                            prevY = pMsg->param2;
//                            messageID = OBJ_MSG_INVALID;
//                        }
//                        else
//                        {
                            x1 = (pMtr->radius * touchX) / touchRadius;
                            y1 = (pMtr->radius * touchY) / touchRadius;

                            if(x1 >= y1)
                            {
                                x2 = pMtr->radius - x1;
                                s = sqrt(y1 * y1 + x2 * x2);
                                deg = s * 57.296 / pMtr->radius;

                                if((pMsg->param1 <= pMtr->xCenter) && (pMsg->param2 <= pMtr->yCenter))
                                {
                                    //Quadrant 2
                                    pMtr->degree = 269 - deg;
                                    messageID = MTR_MSG_TOUCHED;
//                                    if((prevX < pMsg->param1) && (prevY >= pMsg->param2))
//                                        messageID = MTR_MSG_CW;
//                                    else if((prevX >= pMsg->param1) && (prevY < pMsg->param2))
//                                        messageID = MTR_MSG_CCW;
                                }
                                else if((pMsg->param1 > pMtr->xCenter) && (pMsg->param2 <= pMtr->yCenter))
                                {
                                    //Quadrant 1
                                    pMtr->degree = 89 + deg;
                                    messageID = MTR_MSG_TOUCHED;
//                                    if((prevX < pMsg->param1) && (prevY <= pMsg->param2))
//                                        messageID = MTR_MSG_CW;
//                                    else if((prevX >= pMsg->param1) && (prevY > pMsg->param2))
//                                        messageID = MTR_MSG_CCW;
                                }
                                else if((pMsg->param1 <= pMtr->xCenter) && (pMsg->param2 > pMtr->yCenter))
                                {
                                    //Quadrant 3
                                    pMtr->degree = 269 + deg;
                                    messageID = MTR_MSG_TOUCHED;
//                                    if((prevX > pMsg->param1) && (prevY >= pMsg->param2))
//                                        messageID = MTR_MSG_CW;
//                                    else if((prevX <= pMsg->param1) && (prevY < pMsg->param2))
//                                        messageID = MTR_MSG_CCW;
                                }
                                else if((pMsg->param1 > pMtr->xCenter) && (pMsg->param2 > pMtr->yCenter))
                                {
                                    //Quadrant 4
                                    pMtr->degree = 90 - deg;
                                    messageID = MTR_MSG_TOUCHED;
//                                    if((prevX > pMsg->param1) && (prevY <= pMsg->param2))
//                                        messageID = MTR_MSG_CW;
//                                    else if((prevX <= pMsg->param1) && (prevY > pMsg->param2))
//                                        messageID = MTR_MSG_CCW;
                                }
                                else
                                    messageID = OBJ_MSG_INVALID;
                            }
                            else if(x1 < y1)
                            {
                                y2 = pMtr->radius - y1;
                                s = sqrt(y2 * y2 + x1 * x1);
                                deg = s * 57.296 / pMtr->radius;

                                if((pMsg->param1 <= pMtr->xCenter) && (pMsg->param2 <= pMtr->yCenter))
                                {
                                    //Quadrant 2
                                    pMtr->degree = 179 + deg;
                                    messageID = MTR_MSG_TOUCHED;
//                                    if((prevX < pMsg->param1) && (prevY >= pMsg->param2))
//                                        messageID = MTR_MSG_CW;
//                                    else if((prevX >= pMsg->param1) && (prevY < pMsg->param2))
//                                        messageID = MTR_MSG_CCW;
                                }
                                else if((pMsg->param1 > pMtr->xCenter) && (pMsg->param2 <= pMtr->yCenter))
                                {
                                    //Quadrant 1
                                    pMtr->degree = 179 - deg;
                                    messageID = MTR_MSG_TOUCHED;
//                                    if((prevX < pMsg->param1) && (prevY <= pMsg->param2))
//                                        messageID = MTR_MSG_CW;
//                                    else if((prevX >= pMsg->param1) && (prevY > pMsg->param2))
//                                        messageID = MTR_MSG_CCW;
                                }
                                else if((pMsg->param1 <= pMtr->xCenter) && (pMsg->param2 > pMtr->yCenter))
                                {
                                    //Quadrant 3
                                    pMtr->degree = 359 - deg;
                                    messageID = MTR_MSG_TOUCHED;
//                                    if((prevX > pMsg->param1) && (prevY >= pMsg->param2))
//                                        messageID = MTR_MSG_CW;
//                                    else if((prevX <= pMsg->param1) && (prevY < pMsg->param2))
//                                        messageID = MTR_MSG_CCW;
                                }
                                else if((pMsg->param1 > pMtr->xCenter) && (pMsg->param2 > pMtr->yCenter))
                                {
                                    //Quadrant 4
                                    pMtr->degree = deg;
                                    messageID = MTR_MSG_TOUCHED;
//                                    if((prevX > pMsg->param1) && (prevY <= pMsg->param2))
//                                        messageID = MTR_MSG_CW;
//                                    else if((prevX <= pMsg->param1) && (prevY > pMsg->param2))
//                                        messageID = MTR_MSG_CCW;
                                }
                                else
                                    messageID = OBJ_MSG_INVALID;
                            }
                            else
                                messageID = OBJ_MSG_INVALID;

//                            //save current touch position
//                            prevX = pMsg->param1;
//                            prevY = pMsg->param2;
//                        }
//                    }
               }
//                else //touch area out of range
//                {
//                    prevX = -1;
//                    prevY = -1;
//                }
            }
        }
    }
    return (messageID);
}

/*********************************************************************
* Function: WORD MtrDraw(void *pObj)
*
* Notes: This is the state machine to draw the meter.
*
********************************************************************/
WORD MtrDraw(void *pObj)
{
    typedef enum __attribute__((packed)) {
        IDLE,
        FRAME_DRAW,
        NEEDLE_DRAW,
        NEEDLE_ERASE,
        TEXT_DRAW,
        TEXT_DRAW_RUN,
        ARC_DRAW_SETUP,
        ARC_DRAW,
        SCALE_COMPUTE,
        SCALE_LABEL_DRAW,
        SCALE_DRAW,
        VALUE_ERASE,
        VALUE_DRAW,
        VALUE_DRAW_RUN,
    } MTR_DRAW_STATES;

    SHORT GreenAngleCW, GreenAngleCCW, RedAngleCW,RedAngleCCW,  uAngleLimitCW, uAngleLimitCCW;

    static MTR_DRAW_STATES state = IDLE;
    static SHORT x1, y1, x2, y2;
    static SHORT temp, j, i, angle;
    static XCHAR strVal[SCALECHARCOUNT + 1];    // add one more space here for the NULL character
//    static XCHAR tempXchar[2] = {'8',0};        // NULL is pre-defined here
    static float radian;
    static DWORD_VAL dTemp, dRes;
    METER *pMtr;

    pMtr = (METER *)pObj;

    if(IsDeviceBusy())
        return (0);

    switch(state)
    {
        case IDLE:
            if(GetState(pMtr, MTR_HIDE))
            {   // Hide the meter (remove from screen)
                SetColor(pMtr->hdr.pGolScheme->CommonBkColor);
                if(!Bar(pMtr->hdr.left, pMtr->hdr.top, pMtr->hdr.right, pMtr->hdr.bottom))
                    return (0);
                return (1);
            }

            // Check if we need to draw the whole object
            SetLineThickness(NORMAL_LINE);
            SetLineType(SOLID_LINE);
            if(GetState(pMtr, MTR_DRAW))
            {
                // set parameters to draw the frame
                GOLPanelDraw
                (
                    pMtr->hdr.left, pMtr->hdr.top, pMtr->hdr.right, pMtr->hdr.bottom, 0, pMtr->hdr.pGolScheme->Color0,
                        pMtr->hdr.pGolScheme->EmbossLtColor, pMtr->hdr.pGolScheme->EmbossDkColor, NULL,
                            GOL_EMBOSS_SIZE - 1
                );
                state = FRAME_DRAW;
            }
            else
            {
                state = NEEDLE_ERASE;
                goto mtr_needle_draw_st;
            }

        case FRAME_DRAW:
            if(!GOLPanelDrawTsk())
            {
                return (0);
            }
            state = TEXT_DRAW;

        case TEXT_DRAW:

            // draw the meter title
            SetColor(pMtr->hdr.pGolScheme->TextColor1);
            SetFont(pMtr->pTitleFont);
            temp = GetTextWidth(pMtr->pText, pMtr->pTitleFont);

            // set the start location of the meter title
                #if (METER_TYPE == MTR_WHOLE_TYPE)
                    #ifdef METER_DISPLAY_VALUES_ENABLE
                        MoveTo(pMtr->xCenter - (temp >> 1), pMtr->yCenter + pMtr->radius + GetTextHeight(pMtr->pValueFont));
                    #else
            //MoveTo(pMtr->xCenter - (temp >> 1), pMtr->yCenter + pMtr->radius + GetTextHeight(pMtr->hdr.pGolScheme->pFont));
			MoveTo(pMtr->xCenter - (temp >> 1), pMtr->yCenter + pMtr->radius + 10 + GetTextHeight(pMtr->hdr.pGolScheme->pFont));
                    #endif
                #elif (METER_TYPE == MTR_HALF_TYPE)
            MoveTo(pMtr->xCenter - (temp >> 1), pMtr->yCenter + 3);

                #elif (METER_TYPE == MTR_QUARTER_TYPE)
            MoveTo
                (
                    ((pMtr->hdr.right + pMtr->hdr.left) >> 1) - (temp >> 1), pMtr->hdr.bottom - GOL_EMBOSS_SIZE - GetTextHeight
                        (pMtr->pTitleFont)
                );
                #endif
            state = TEXT_DRAW_RUN;

        case TEXT_DRAW_RUN:

            // render the title of the meter
            if(!OutText(pMtr->pText))
                return (0);
            state = ARC_DRAW_SETUP;

        //case ARC0_DRAW:
        case ARC_DRAW_SETUP:

            // check if we need to draw the arcs
            if(!GetState(pMtr, MTR_RING))
            {

                // if meter is not RING type, for scale label colors use
                // the three colors (normal, critical and danger)
                i = DEGREE_END;
                state = SCALE_COMPUTE;
                goto mtr_scale_compute;
            }
            else
            {
                // set the arc radii: x1 smaller radius and x2 as the larger radius
                x1 = pMtr->radius + 2;
                x2 = pMtr->radius + (pMtr->radius >> 2) + 2;
                    #if (METER_TYPE == MTR_WHOLE_TYPE)
                temp = 6;
                    #elif (METER_TYPE == MTR_HALF_TYPE)
                temp = 5;
                    #elif (METER_TYPE == MTR_QUARTER_TYPE)
                temp = 3;
                    #endif
                state = ARC_DRAW;
            }

        case ARC_DRAW:

#ifndef G15CUBESERVO

            // draw the arcs
            #if (METER_TYPE == MTR_WHOLE_TYPE)
            while(temp)
            {
            #elif (METER_TYPE == MTR_HALF_TYPE)
            while(temp > 1)
            {
            #elif (METER_TYPE == MTR_QUARTER_TYPE)
            while(temp > 1)
            {
            #endif

                        // decide which arc will be drawn
                        switch(temp)
                        {
                            case 6:
                                SetColor(pMtr->arcColor1);
                                if(!Arc(pMtr->xCenter, pMtr->yCenter, pMtr->xCenter, pMtr->yCenter, x1, x2, 0x20))
                                    return (0);
                                break;

                            case 5:
                                SetColor(pMtr->arcColor2);
                                if(!Arc(pMtr->xCenter, pMtr->yCenter, pMtr->xCenter, pMtr->yCenter, x1, x2, 0x40))
                                    return (0);
                                break;

                            case 4:
                                SetColor(pMtr->arcColor3);
                                if(!Arc(pMtr->xCenter, pMtr->yCenter, pMtr->xCenter, pMtr->yCenter, x1, x2, 0x80))
                                    return (0);
                                break;

                            case 3:
                                SetColor(pMtr->arcColor4);
                                if(!Arc(pMtr->xCenter, pMtr->yCenter, pMtr->xCenter, pMtr->yCenter, x1, x2, 0x01))
                                    return (0);
                                break;

                            case 2:
                                SetColor(pMtr->arcColor5);
                                if(!Arc(pMtr->xCenter, pMtr->yCenter, pMtr->xCenter, pMtr->yCenter, x1, x2, 0x02))
                                    return (0);
                                break;

                            case 1:
                                SetColor(pMtr->arcColor6);
                                if(!Arc(pMtr->xCenter, pMtr->yCenter, pMtr->xCenter, pMtr->yCenter, x1, x2, 0x04))
                                    return (0);
                                break;

                            default:
                                break;
                        }

                        temp--;
                    }
#endif

                    // set the color for the scale labels
                    SetColor(pMtr->hdr.pGolScheme->Color1);
//RERO:                    i = DEGREE_END;
                    i = DEGREE_START;
                    state = SCALE_COMPUTE;

                case SCALE_COMPUTE:
//RERO:                    mtr_scale_compute : if(i >= DEGREE_START)
                    mtr_scale_compute : if(i <= DEGREE_END)
                    {
                        radian = i * .0175;

                        if(!GetState(pMtr, MTR_RING))
                        {
                            if(i >= ARC1_DEGREE)
                            {
                                SetColor(pMtr->arcColor1);
                            }
                            else if(i >= ARC2_DEGREE)
                            {
                                SetColor(pMtr->arcColor2);
                            }
                            else if(i >= ARC3_DEGREE)
                            {
                                SetColor(pMtr->arcColor3);
                            }
                            else if(i >= ARC4_DEGREE)
                            {
                                SetColor(pMtr->arcColor4);
                            }
                            else if(i >= ARC5_DEGREE)
                            {
                                SetColor(pMtr->arcColor5);
                            }
                            else
                            {
                                SetColor(pMtr->arcColor6);
                            }
                        }

                        // compute for the effective radius of the scales
                        if((i % 45) == 0)
                            x2 = pMtr->radius + (pMtr->radius >> 2) + 2;
                        else
                            x2 = pMtr->radius + (pMtr->radius >> 3) + 3;

                        // compute the starting x1 and y1 position of the scales
                        // x2 here is the distance from the center to the x1, y1
                        // position. Sin and cos is used here since computation speed in initial
                        // drawing is not yet critical.
                        x1 = x2 * cos(radian);
                        y1 = (-1) * (x2 * sin(radian));

                        // using ratio and proportion we get the x2,y2 position
                        dTemp.Val = 0;
                        dTemp.w[1] = (pMtr->radius + 3);
                        dTemp.Val /= x2;

                        dRes.Val = dTemp.Val * x1;
                        x2 = dRes.w[1] + pMtr->xCenter; // adjusted to center
                        dRes.Val = dTemp.Val * y1;
                        y2 = dRes.w[1] + pMtr->yCenter; // adjusted to center
                        x1 += pMtr->xCenter;
                        y1 += pMtr->yCenter;            // adjust x1, y1 to the center
                        state = SCALE_DRAW;
                    }
                    else
                    {
                        state = NEEDLE_ERASE;
                        goto mtr_needle_draw_st;
                    }

                case SCALE_DRAW:
                    if(!Line(x1, y1, x2, y2))               // now draw the scales
                        return (0);

                    if((i % 45) == 0)
                    {

                        // draw the scale labels
                        // reusing radian, x2 and y2
                        // compute for the actual angle of needle to be shown in screen				
//RERO:                        radian = (DEGREE_END - DEGREE_START) - (i - (DEGREE_START));
                        radian = i + 90;

                        // compute the values of the label to be shown per 45 degree
//RERO:                        temp = (pMtr->maxValue - pMtr->minValue) * (radian / (DEGREE_END - DEGREE_START));
                        temp = radian;

                        // adjust for the minimum or offset value
//RERO:                        temp += pMtr->minValue;

                        // this implements sprintf(strVal, "%d", temp); faster
                        // note that this is just for values >= 0, while sprintf covers negative values.
                        j = 1;

                        // get the ones value first and account for the required decimal point if enabled
                        if(GetState(pMtr, MTR_ACCURACY))
                        {
                            // round off to nearest tens
                            dTemp.w[0] = (temp % RESOLUTION) / (RESOLUTION / 10);
                            if((dTemp.w[0]) > (RESOLUTION >> 1))
                                temp += (RESOLUTION - dTemp.w[0]);
                            temp /= RESOLUTION;
                        }

                        do
                        {
                            strVal[SCALECHARCOUNT - j] = (temp % 10) + '0';
                            if(((temp /= 10) == 0) || (j >= SCALECHARCOUNT))
                                break;
                            j++;
                        } while(j <= SCALECHARCOUNT);

                        // the (&strVal[SCALECHARCOUNT-j]) removes the leading zeros.
                        // if leading zeroes will be printed change (&strVal[SCALECHARCOUNT-j])
                        // to simply strVal and remove the break statement above in the do-while loop
                        x2 = GetTextWidth((&strVal[SCALECHARCOUNT - j]), pMtr->hdr.pGolScheme->pFont);
                        y2 = GetTextHeight(pMtr->hdr.pGolScheme->pFont);

                        if(i == 0)
                        {
                            MoveTo(x1 + 3, y1 - (y2 >> 1));
                        }
                        else if(i == 45)
                        {
                            MoveTo(x1, y1 - y2);
                        }
                        else if(i == 90)
                        {
                            MoveTo(x1 - (x2 >> 1), y1 - y2);
                        }
                        else if(i == 135)
                        {
                            MoveTo(x1 - x2, y1 - y2);
                        }
                        else if(i == 180)
                        {
                            MoveTo(x1 - x2 - 2, y1 - (y2 >> 1));
                        }
                        else if(i == 225)
                        {
                            MoveTo(x1 - x2, y1);
                        }
                        else if (i == -90)
                        {
                            MoveTo(x1 - (x2 >> 1), y1);
                        }
                        else if (i == -45)
                        {
                            MoveTo(x1, y1);
                        }
                        state = SCALE_LABEL_DRAW;
                    }
                    else
                    {
//RERO:                        i -= DEGREECOUNT;
                        i += DEGREECOUNT;
                        state = SCALE_COMPUTE;          //SCALE_LABEL_DRAW;
                        goto mtr_scale_compute;
                    }

                case SCALE_LABEL_DRAW:
                    SetFont(pMtr->hdr.pGolScheme->pFont);
                    if(!OutText(&strVal[SCALECHARCOUNT - j]))
                        return (0);
//RERO:                    i -= DEGREECOUNT;
                    i += DEGREECOUNT;
                    state = SCALE_COMPUTE;
                    goto mtr_scale_compute;

                case NEEDLE_ERASE:
                    mtr_needle_draw_st : if(GetState(pMtr, MTR_DRAW_UPDATE))
                    {
                        // to update the needle, redraw the old position with background color
                        SetColor(pMtr->hdr.pGolScheme->Color0);
                        SetLineThickness(THICK_LINE);
                        if(!Line(pMtr->xCenter, pMtr->yCenter, pMtr->xPos, pMtr->yPos))
                            return (0);
                    }

                    state = NEEDLE_DRAW;

                case NEEDLE_DRAW:
                    if(IsDeviceBusy())
                        return (0);

#ifdef G15CUBESERVO

                x1 = pMtr->radius;
                
                uAngleLimitCW=pMtr->angleLimitCW;
                uAngleLimitCCW=pMtr->angleLimitCCW;

                // Wheel mode.
                if (uAngleLimitCW == uAngleLimitCCW) {
                    SetColor(pMtr->arcColor3);
                    if(!DrawArc(pMtr->xCenter, pMtr->yCenter, x1, x1-2,0,360))
                        return(0);
                }

                // Position mode.
                else {

                    if(uAngleLimitCW>90)
                    {
                        uAngleLimitCW=uAngleLimitCW-360;
                    }
                    if(uAngleLimitCCW>90)
                    {
                        uAngleLimitCCW=uAngleLimitCCW-360;
                    }

                    GreenAngleCW=-(uAngleLimitCW-90);
                    GreenAngleCCW=-(uAngleLimitCCW-90);
                    RedAngleCW=GreenAngleCW;
                    RedAngleCCW=GreenAngleCCW;

                    if(GreenAngleCCW>GreenAngleCW)      //DrawArc draw clockwise only
                    {
                        GreenAngleCCW=GreenAngleCCW-360;
                    }
                    else
                    {
                        RedAngleCW=RedAngleCW-360;
                    }

                    SetColor(pMtr->arcColor1);
                    if(!DrawArc(pMtr->xCenter, pMtr->yCenter, x1, x1-2, GreenAngleCCW,GreenAngleCW))
                            return(0);

                    SetColor(pMtr->arcColor2);
                    if(!DrawArc(pMtr->xCenter, pMtr->yCenter, x1, x1-2,RedAngleCW,RedAngleCCW))
                            return(0);
                }

                SetLineThickness(NORMAL_LINE);

#endif

                    // At this point, pMtr->value is assumed to contain the new value of the meter.
                    // calculate the new angle:
                    // equation is still ratio and proportion of angle and values.
                    dTemp.Val = 0;
                    dTemp.w[1] = pMtr->value - pMtr->minValue;
                    dTemp.Val /= (pMtr->maxValue - pMtr->minValue);
                    dTemp.Val *= (DEGREE_END - DEGREE_START);

                    angle = DEGREE_START + (dTemp.w[1]);
                    temp = angle;

                    /* The method uses a lookup table of pre-calculated sine values. The table
			   is derived from calculating sine values from 0 degrees to 90 degrees.
			   To save space, the entries are just a byte size. So 360/256 = 1.40625 degrees
			   increments is used for each entry. (i.e entry 0 is zero, entry i is 1.40625,
			   entry 2 is 2*1.40625,... entry n is n*1.40625. 
			   Get the sine of the entries yields a float value. Since we only use a 
			   byte for storage we can shift the values by 128 without overflowing the 
			   storage. Thus we need to shift the computed values by 8 bits. Shifting now 
			   permits us to work with integers which greatly reduces the execution time.
			   With this in mind, the input angles must be translated to 0-90 range with the
			   quadrant of the original angle stored to translate back the value from the 
			   table to the correct quadrant. Also the quadrant number will determine if 
			   the calculated x and y positions are to be swapped. 
			   In summary:
			   	Swap x and y when calculating points in quadrant 2 and 4
			   	Negate x when in quadrant 2 and 3
			   	Negate y when in quadrant 1 and 2
			*/

                    // translate the angle to 0-90 range
                    while(temp < 0)                     // this is needed for negative
                        temp += 90;                     // for negative values
                    while(temp > 90)
                        temp -= 90;

                    // determine which quadrant the angle is located
                    // i determines if x and y are swapped (0 no swapping, 1 swapping needed)
                    // y2 and x2 are the multiplier to negate or not the x and y values
                    if((180 < angle) && (angle < 270))
                    {               // quadrant 3
                        i = 0;
                        y2 = 1;
                        x2 = -1;
                    }
                    else if((90 < angle) && (angle <= 180))
                    {               // quadrant 2
                        i = 1;
                        y2 = -1;
                        x2 = -1;
                    }
                    else if((0 <= angle) && (angle <= 90))
                    {               // quadrant 1
                        i = 0;
                        y2 = -1;
                        x2 = 1;
                    }
                    else if((-90 <= angle) && (angle < 0))
                    {               // quadrant 4
                        i = 1;
                        y2 = 1;
                        x2 = 1;
                    }

                    // using the calculated angle we get the sine and cosine 
                    // values from the look up table, then to avoid
                    // division, the inverse is used
                    x1 = Cosine(temp) * pMtr->radius;
                    y1 = Sine(temp) * pMtr->radius;

                    // calculate new positions, check if we need to reverse x and y values
                    pMtr->xPos = ((x2) * (((i == 0) ? x1 : y1) >> 8)) + pMtr->xCenter;
                    pMtr->yPos = ((y2) * (((i == 0) ? y1 : x1) >> 8)) + pMtr->yCenter;

                    // now draw the needle with the new position
                    // Only do it for position mode.
                    if (uAngleLimitCW != uAngleLimitCCW) {
                        SetColor(pMtr->hdr.pGolScheme->Color1);
                        SetLineThickness(THICK_LINE);
                        if(!Line(pMtr->xCenter, pMtr->yCenter, pMtr->xPos, pMtr->yPos))
                            return (0);
                        SetLineThickness(NORMAL_LINE);
                    }
                        #ifdef METER_DISPLAY_VALUES_ENABLE
                    state = VALUE_ERASE;
                        #else


                    // reset the line to normal
                    SetLineThickness(NORMAL_LINE);
                    state = IDLE;
                    return (1);
                        #endif
                        #ifdef METER_DISPLAY_VALUES_ENABLE

                case VALUE_ERASE:
                    if(IsDeviceBusy())
                        return (0);

                    // reset the line to normal
                    SetLineThickness(NORMAL_LINE);

                    // display the value
                    // erase previous value first. The temp>>1 accomodates fonts with characters that has unequal widths
                    temp = GetTextWidth(tempXchar, pMtr->pValueFont);
                    temp = temp * SCALECHARCOUNT + (temp >> 1);

                    //temp = GetTextWidth(tempXchar, pMtr->pValueFont)*SCALECHARCOUNT;
                    SetColor(pMtr->hdr.pGolScheme->Color0);

                            #if (METER_TYPE == MTR_WHOLE_TYPE)
                    if
                    (
                        !Bar
                            (
                                pMtr->xCenter -
                                (temp >> 1), pMtr->yCenter +
                                pMtr->radius, pMtr->xCenter +
                                (temp >> 1), pMtr->yCenter +
                                pMtr->radius +
                                GetTextHeight(pMtr->pValueFont)
                            )
                    ) return (0);

                            #elif (METER_TYPE == MTR_HALF_TYPE)
                    if
                    (
                        !Bar
                            (
                                pMtr->xCenter -
                                (temp >> 1), pMtr->yCenter -
                                GetTextHeight(pMtr->pValueFont), pMtr->xCenter +
                                (temp >> 1), pMtr->yCenter
                            )
                    ) return (0);

                            #elif (METER_TYPE == MTR_QUARTER_TYPE)
                    if
                    (
                        !Bar
                            (
                                pMtr->xCenter -
                                1, pMtr->yCenter -
                                GetTextHeight(pMtr->pValueFont), pMtr->xCenter +
                                temp, pMtr->yCenter +
                                1
                            )
                    ) return (0);
                            #endif
                    state = VALUE_DRAW;

                case VALUE_DRAW:
                    if(IsDeviceBusy())
                        return (0);

                    if(angle >= ARC1_DEGREE)
                    {
                        SetColor(pMtr->arcColor1);
                    }
                    else if(angle >= ARC2_DEGREE)
                    {
                        SetColor(pMtr->arcColor2);
                    }
                    else if(angle >= ARC3_DEGREE)
                    {
                        SetColor(pMtr->arcColor3);
                    }
                    else if(angle >= ARC4_DEGREE)
                    {
                        SetColor(pMtr->arcColor4);
                    }
                    else if(angle >= ARC5_DEGREE)
                    {
                        SetColor(pMtr->arcColor5);
                    }
                    else
                    {
                        SetColor(pMtr->arcColor6);
                    }

                    // display the current value
                    SetFont(pMtr->pValueFont);

                    // this implements sprintf(strVal, "%03d", pMtr->value); faster
                    // note that this is just for values >= 0, while sprintf covers negative values.
                    i = pMtr->value;
                    j = 1;

                    // get the ones value first and account for the required decimal point if enabled
                    if(GetState(pMtr, MTR_ACCURACY))
                    {
                        strVal[SCALECHARCOUNT - j] = (((i % RESOLUTION)) / (RESOLUTION / 10)) + '0';
                        i /= RESOLUTION;
                        j++;
                        strVal[SCALECHARCOUNT - j] = '.';
                        j++;
                    }

                    do
                    {
                        strVal[SCALECHARCOUNT - j] = (i % 10) + '0';
                        if(((i /= 10) == 0) || (j >= SCALECHARCOUNT))
                            break;
                        j++;
                    } while(j <= SCALECHARCOUNT);

                    temp = GetTextWidth(&strVal[SCALECHARCOUNT - j], pMtr->pValueFont);

                            #if (METER_TYPE == MTR_WHOLE_TYPE)
                    MoveTo(pMtr->xCenter - (temp >> 1), pMtr->yCenter + pMtr->radius);

                            #elif (METER_TYPE == MTR_HALF_TYPE)
                    MoveTo(pMtr->xCenter - (temp >> 1), pMtr->yCenter - GetTextHeight(pMtr->pValueFont));

                            #elif (METER_TYPE == MTR_QUARTER_TYPE)
                    MoveTo(pMtr->xCenter, pMtr->yCenter - GetTextHeight(pMtr->pValueFont));
                            #endif
                    state = VALUE_DRAW_RUN;

                case VALUE_DRAW_RUN:
                    if(!OutText(&strVal[SCALECHARCOUNT - j]))
                        return (0);
                    state = IDLE;
                    return (1);
                        #endif //METER_DISPLAY_VALUES_ENABLE
                }

                return (1);
            }

            #endif // USE_METER
