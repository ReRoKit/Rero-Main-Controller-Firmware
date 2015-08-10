/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  GOL Layer 
 *  Round Dial
 *****************************************************************************
 * FileName:        RoundDial.c
 * Dependencies:    math.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 Version 3.00, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
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
 * Author                           Date            Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * PAT				  	            11/12/07	    Version 1.0 release
 * Pradeep Budagutta                03 Dec 2009     Added Object Header for Double Buffering Support
 *****************************************************************************/
#include "Graphics/Graphics.h"
#include <math.h>

#ifdef USE_G15

/*********************************************************************
* Function: ROUNDDIAL  *RdiaCreate(	WORD ID, SHORT x, SHORT y, SHORT radius, 
*						WORD state, SHORT res, SHORT value, SHORT max, 
*						GOL_SCHEME *pScheme)
*
*
* Notes: Creates a ROUNDDIAL object and adds it to the current active list.
*        If the creation is successful, the pointer to the created Object 
*        is returned. If not successful, NULL is returned.
*
********************************************************************/
G15 *G15Create
(
    WORD        ID,
    SHORT       x,
    SHORT       y,
    SHORT       radius,
    WORD        state,
    SHORT       res,
    SHORT       value,
    SHORT       max,
    GOL_SCHEME  *pScheme
)
{
    G15   *pG15 = NULL;

    pG15 = (G15 *)GFX_malloc(sizeof(G15));
    if(pG15 == NULL)
        return (NULL);

    pG15->hdr.ID = ID;              // unique id assigned for referencing
    pG15->hdr.pNxtObj = NULL;       // initialize pointer to NULL
    pG15->hdr.type = OBJ_G15; // set object type
    pG15->xCenter = x;				// x coordinate of center
    pG15->yCenter = y;				// y coordinate of center
    pG15->radius = radius;			// radius of dial
    pG15->res = res;
    pG15->value = value;
    pG15->max = max;
    pG15->hdr.state = state;        // state
    pG15->curr_xPos = x + radius * 2 / 3;
    pG15->curr_yPos = y;
    pG15->hdr.DrawObj = G15Draw;				// draw function
    pG15->hdr.MsgObj = G15TranslateMsg;		// message function
    pG15->hdr.MsgDefaultObj = G15MsgDefault;	// default message function
    pG15->hdr.FreeObj = NULL;					// free function

    pG15->hdr.left = x - radius;    // left position
    pG15->hdr.top = y - radius;     // top position
    pG15->hdr.right = x + radius;   // right position
    pG15->hdr.bottom = y + radius;  // bottom position

    // Set the color scheme to be used
    if(pScheme == NULL)
        pG15->hdr.pGolScheme = _pDefaultGolScheme;
    else
        pG15->hdr.pGolScheme = (GOL_SCHEME *)pScheme;

    GOLAddObject((OBJ_HEADER *)pG15);

    return (pG15);
}

/*********************************************************************
* Function: RdiaMsgDefault(WORD translatedMsg, void *pObj, GOL_MSG* pMsg)
*
*
* Notes: This the default operation to change the state of the dial.
*		 Called inside GOLMsg() when GOLMsgCallback() returns a 1.
*
********************************************************************/
void G15MsgDefault(WORD translatedMsg, void *pObj, GOL_MSG *pMsg)
{
    G15 *pG15;

    pG15 = (G15 *)pObj;

    switch(translatedMsg)
    {
        case G15_MSG_CLOCKWISE:      SetState(pG15, G15_ROT_CW | G15_DRAW);    // set rotate left and redraw
            break;
        case G15_MSG_CTR_CLOCKWISE:  SetState(pG15, G15_ROT_CCW | G15_DRAW);   // set rotate right and redraw
            break;
    }
}

/*********************************************************************
* Function: SHORT RdiaCosine( SHORT v )
*
*
* Notes: Returns the cosine of the dial position.
*
********************************************************************/
    #ifdef USE_KEYBOARD

// Dimple position table for 15 degree increments
//    #define QUADRANT_POSITIONS     6
//    SHORT   _cosine[QUADRANT_POSITIONS] = { 100, 97, 87, 71, 50, 26 };
SHORT   _cosine[G15_QUADRANT_POSITIONS] = { 100, 97, 87, 71, 50, 26};
    #endif
/* */
SHORT G15Cosine(SHORT v)
{
    if(v >= G15_QUADRANT_POSITIONS * 3)
    {
        v -= G15_QUADRANT_POSITIONS * 3;
        return (_cosine[G15_QUADRANT_POSITIONS - 1 - v]);
    }
    else if(v >= G15_QUADRANT_POSITIONS * 2)
    {
        v -= G15_QUADRANT_POSITIONS * 2;
        return (-(_cosine[v]));
    }
    else if(v >= G15_QUADRANT_POSITIONS)
    {
        v -= G15_QUADRANT_POSITIONS;
        return (-(_cosine[G15_QUADRANT_POSITIONS - 1 - v]));
    }
    else
    {
        return (_cosine[v]);
    }
}

const unsigned char _ACOS[61]={90,89,88,87,86,85,84,83,82,81,80,79,78,77,76,76,75,74,73,72,71,70,68,67,66,65,64,63,62,61,60,59,58,57,55,54,53,52,51,49,48,47,46,44,43,41,40,38,37,35,34,32,30,28,26,24,21,18,15,10,0};
/*********************************************************************
* Function: SHORT RdiaSine( SHORT v )
*
*
* Notes: Returns the sine of the dial position.
*
********************************************************************/
SHORT G15Sine(SHORT v)
{
    if(v >= G15_QUADRANT_POSITIONS * 3)
    {
        v -= G15_QUADRANT_POSITIONS * 3;
        return (-(_cosine[v]));
    }
    else if(v >= G15_QUADRANT_POSITIONS * 2)
    {
        v -= G15_QUADRANT_POSITIONS * 2;
        return (-(_cosine[G15_QUADRANT_POSITIONS - 1 - v]));
    }
    else if(v >= G15_QUADRANT_POSITIONS)
    {
        v -= G15_QUADRANT_POSITIONS;
        return (_cosine[v]);
    }
    else
    {
        return (_cosine[G15_QUADRANT_POSITIONS - 1 - v]);
    }
}

/*********************************************************************
* Function: WORD RdiaTranslateMsg(void *pObj, GOL_MSG *pMsg)
*
*
* Notes: Evaluates the message if the object will be affected by the 
*		 message or not.
*
********************************************************************/
WORD G15TranslateMsg(void *pObj, GOL_MSG *pMsg)
{
    G15 *pG15;

    pG15 = (G15 *)pObj;

        #ifdef USE_TOUCHSCREEN

    SHORT           touchRadius, touchX, touchY;
    static SHORT    prevX = -1, prevY = -1;
    WORD            messageID = OBJ_MSG_INVALID;

	SHORT temp;
	

    // Evaluate if the message is for the button
    // Check if disabled first
    if(GetState(pG15, G15_DISABLED))
        return (OBJ_MSG_INVALID);

    if((pMsg->type == TYPE_TOUCHSCREEN) && (pMsg->uiEvent == EVENT_MOVE))
    {

        // Check if it falls in the dial's face
        // to check this the x,y position must be within the circle
        // (x - xCenter)^2 + (y - yCenter)^2 = r^2  where x and y are the points
        // to test, the distance between x,y position of touch and center must be
        // greater than or equal to the radius of the dial
        if
        (
            ((pG15->xCenter - pG15->radius) < pMsg->param1) &&			//on left inside the dial
            ((pG15->xCenter + pG15->radius) > pMsg->param1) &&			//on right inside the dial
            ((pG15->yCenter - pG15->radius) < pMsg->param2) &&			//bottom inside the dial 
            ((pG15->yCenter + pG15->radius) > pMsg->param2)				//top inside the dial
        )
        {

            // first get the radius of the touch point
            touchX = pMsg->param1 - pG15->xCenter;						//distance of x from center, can be positive or negative
            touchY = pMsg->param2 - pG15->yCenter;						//distance of y from center, can be positive or negative
            touchRadius = sqrt(touchX * touchX + touchY * touchY);		//radius of the touch 

            if(touchRadius <= pG15->radius)								//redundant check of the touch inside the dial surface?
            {

                // difference of 3 is used to remove jitter caused by noise or sensitivity of the touchscreen
               // if((abs(prevX - pMsg->param1) > 3) || (abs(prevY - pMsg->param2) > 3))			//checking there is different in touch position current with the previous
                {

                    // The first MOVE event is used to record the current position only. The second MOVE event
                    // will be used together with the previous MOVE and determine if the movement is in the
                    // clockwise or counter clockwise direction.
                    if((prevX == -1) || (prevY == -1))		//1st move event recorded as current position not action
                    {
                        prevX = pMsg->param1;
                        prevY = pMsg->param2;
                        messageID = OBJ_MSG_INVALID;
                    }
                    else
                    {

                        // this makes the sampling area a ring where the max radius is the dial radius
                        // and min radius is 5
                        if(touchRadius > 5)
                        {
						
                            pG15->new_xPos = (pG15->radius * 2 / 3) * (pMsg->param1 - pG15->xCenter) / touchRadius;			//offset from center, dimple x position calculation 2/3*radius*cosA
                            pG15->new_yPos = (pG15->radius * 2 / 3) * (pMsg->param2 - pG15->yCenter) / touchRadius;			//offset from center, dimple y position calculation 2/3*radius*sinA
														
							temp=abs(60 * (pMsg->param1 - pG15->xCenter) / touchRadius);	
							pG15->value=_ACOS[temp];						


                            // check if moving in clockwise direction or counter clockwise direction
                            if((pG15->xCenter >= pMsg->param1) && (pG15->yCenter > pMsg->param2))
                            {
                                if((prevX < pMsg->param1) && (prevY >= pMsg->param2))
                                    messageID = G15_MSG_CLOCKWISE;
                                else if((prevX >= pMsg->param1) && (prevY < pMsg->param2))
                                    messageID = G15_MSG_CTR_CLOCKWISE;
                            }

                            if((pG15->xCenter < pMsg->param1) && (pG15->yCenter > pMsg->param2))
                            {
                                if((prevX < pMsg->param1) && (prevY <= pMsg->param2))
                                    messageID = G15_MSG_CLOCKWISE;
                                else if((prevX >= pMsg->param1) && (prevY > pMsg->param2))
                                    messageID = G15_MSG_CTR_CLOCKWISE;
                            }

                            if((pG15->xCenter < pMsg->param1) && (pG15->yCenter <= pMsg->param2))
                            {
                                if((prevX > pMsg->param1) && (prevY <= pMsg->param2))
                                    messageID = G15_MSG_CLOCKWISE;
                                else if((prevX <= pMsg->param1) && (prevY > pMsg->param2))
                                    messageID = G15_MSG_CTR_CLOCKWISE;
                            }

                            if((pG15->xCenter >= pMsg->param1) && (pG15->yCenter <= pMsg->param2))
                            {
                                if((prevX > pMsg->param1) && (prevY >= pMsg->param2))
                                    messageID = G15_MSG_CLOCKWISE;
                                else if((prevX <= pMsg->param1) && (prevY < pMsg->param2))
                                    messageID = G15_MSG_CTR_CLOCKWISE;
                            }
                        }
                        else
                            messageID = OBJ_MSG_INVALID;

                        prevX = pMsg->param1;
                        prevY = pMsg->param2;
                    }
                }
               // else
               //    messageID = OBJ_MSG_INVALID;

                // determine the movement clockwise or counter clockwise
                // this is important to update the value variable
                return (messageID);
            }
            else
            {
                prevX = -1;
                prevY = -1;
            }
        }
    }

        #endif		
		#ifdef USE_KEYBOARD

    SHORT   newValue;

    // Evaluate if the message is for the button
    // Check if disabled first
    if(GetState(pG15, G15_DISABLED))
        return (OBJ_MSG_INVALID);

    if((pMsg->type == TYPE_KEYBOARD) && (pMsg->param1 == pG15->hdr.ID) && (pMsg->uiEvent == EVENT_KEYSCAN))
    {
        if(pMsg->param2 == SCAN_RIGHT_PRESSED)
        {
            newValue = pG15->value + pG15->res;
            if(newValue > pG15->max)
            {
                newValue -= (pG15->max + 1);
            }

            pG15->new_xPos = pG15->radius * 2 * G15Cosine(newValue) / 100 / 3;
            pG15->new_yPos = pG15->radius * 2 * G15Sine(newValue) / 100 / 3;
            return (G15_MSG_CLOCKWISE);
        }

        if(pMsg->param2 == SCAN_LEFT_PRESSED)
        {
            newValue = pG15->value - pG15->res;
            if(newValue < 0)
            {
                newValue += (pG15->max + 1);
            }

            pG15->new_xPos = pG15->radius * 2 * G15Cosine(newValue) / 100 / 3;
            pG15->new_yPos = pG15->radius * 2 * G15Sine(newValue) / 100 / 3;
            return (G15_MSG_CTR_CLOCKWISE);
        }
    }

        #endif
 
    return (OBJ_MSG_INVALID);
}

/*********************************************************************
* Function: WORD RdiaDraw(void *pObj)
*
*
* Notes: This is the state machine to draw the dial.
*
********************************************************************/
WORD G15Draw(void *pObj)
{
    typedef enum
    {
        REMOVE,
        G15_PANEL_DRAW,
        G15_PANEL_TASK,
        ERASE_POSITION,
        DRAW_POSITION
    } G15_DRAW_STATES;

    static G15_DRAW_STATES state = REMOVE;
    static SHORT dimpleRadius;
    WORD faceClr;
    G15 *pG15;

    pG15 = (G15 *)pObj;

    switch(state)
    {
        case REMOVE:
            if(IsDeviceBusy())
                return (0);

            if(GetState(pG15, G15_HIDE))
            {   // Hide the dial (remove from screen)
                SetColor(pG15->hdr.pGolScheme->CommonBkColor);
                if
                (
                    !Bar
                        (
                            pG15->xCenter - pG15->radius,
                            pG15->yCenter - pG15->radius,
                            pG15->xCenter + pG15->radius,
                            pG15->yCenter + pG15->radius
                        )
                ) return (0);
                return (1);
            }

            dimpleRadius = (pG15->radius >> 3) + 1;

            if(GetState(pG15, G15_ROT_CCW | G15_ROT_CW))
            {
                state = ERASE_POSITION;
                goto erase_current_pos;
            }

            state = G15_PANEL_DRAW;

        case G15_PANEL_DRAW:
            if(!GetState(pG15, G15_DISABLED))
            {
                faceClr = pG15->hdr.pGolScheme->Color0;
            }
            else
            {
                faceClr = pG15->hdr.pGolScheme->ColorDisabled;
            }

            SetLineThickness(NORMAL_LINE);
            SetLineType(SOLID_LINE);
            GOLPanelDraw
            (
                pG15->xCenter,
                pG15->yCenter,
                pG15->xCenter,
                pG15->yCenter,
                pG15->radius,
                faceClr,
                pG15->hdr.pGolScheme->EmbossLtColor,
                pG15->hdr.pGolScheme->EmbossDkColor,
                NULL,
                GOL_EMBOSS_SIZE
            );
            state = G15_PANEL_TASK;

        case G15_PANEL_TASK:
            if(!GOLPanelDrawTsk())
            {
                return (0);
            }

            state = DRAW_POSITION;
            goto draw_current_pos;

        case ERASE_POSITION:
            erase_current_pos : SetColor(pG15->hdr.pGolScheme->Color0);
            if
            (
                !Bar
                    (
                        pG15->curr_xPos - dimpleRadius,
                        pG15->curr_yPos - dimpleRadius,
                        pG15->curr_xPos + dimpleRadius,
                        pG15->curr_yPos + dimpleRadius
                    )
            ) return (0);

            // determine if the value will increment or decrement
                #if defined USE_TOUCHSCREEN
            if(GetState(pG15, G15_ROT_CCW))
            {
                //pG15->value = pG15->value + pG15->res;
                //if(pG15->value > pG15->max)
                    //pG15->value = pG15->max;
				//	pG15->value=0;
            }
            else if(GetState(pG15, G15_ROT_CW))
            {
                //pG15->value = pG15->value - pG15->res;
                //if(pG15->value < 0)
                   // pG15->value = 0;
				//    pG15->value = pG15->max;
            }

			#elif defined USE_KEYBOARD
            if(GetState(pG15, G15_ROT_CW))
            {
                pG15->value = pG15->value + pG15->res;
                if(pG15->value > pG15->max)
                {
                    pG15->value -= (pG15->max + 1);
                }
            }
            else if(GetState(pG15, G15_ROT_CCW))
            {
                pG15->value = pG15->value - pG15->res;
                if(pG15->value < 0)
                {
                    pG15->value += (pG15->max + 1);
                }
            }
                #endif
 

            // else do not update counter yet
            //locate the new position of the dimple	
			pG15->curr_xPos = pG15->xCenter + pG15->new_xPos;
            pG15->curr_yPos = pG15->yCenter + pG15->new_yPos;
			// pG15->curr_xPos = pG15->xCenter+(pG15->radius * 2 / 3)*cosf(pG15->value*3.142/180);
			// pG15->curr_yPos = pG15->yCenter-(pG15->radius * 2 / 3)*sinf(pG15->value*3.142/180);

            ClrState(pG15, G15_ROT_CW | G15_ROT_CCW); // make sure this is cleared to avoid

            // unwanted redraw
            state = DRAW_POSITION;

        case DRAW_POSITION:
            draw_current_pos : if(IsDeviceBusy()) return (0);

            SetColor(pG15->hdr.pGolScheme->EmbossLtColor);
            SetLineThickness(NORMAL_LINE);
            SetLineType(SOLID_LINE);
            if(!Circle(pG15->curr_xPos, pG15->curr_yPos, dimpleRadius))
                return (0);
            SetColor(pG15->hdr.pGolScheme->EmbossDkColor);
            if(!FillCircle(pG15->curr_xPos, pG15->curr_yPos, dimpleRadius - 1))
                return (0);

            state = REMOVE;
            return (1);
    }

    return (1);
}

#endif // USE_G15
