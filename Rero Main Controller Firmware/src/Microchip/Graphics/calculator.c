#include "Graphics/Graphics.h"

#ifdef USE_CALCULATOR

/*********************************************************************
* Function: TEXTENTRY *TeCreate(WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, WORD state
*					SHORT horizontalKeys, SHORT verticalKeys, XCHAR *pText[], 
*					void *pBuffer, WORD bufferLength, void *pDisplayFont, 
*					GOL_SCHEME *pScheme)
*
*
* Notes: 
*
********************************************************************/
CALCULATOR *CalcCreate
(
    WORD        ID,
    SHORT       left,
    SHORT       top,
    SHORT       right,
    SHORT       bottom,
    WORD        state,
    SHORT       horizontalKeys,
    SHORT       verticalKeys,
    XCHAR       *pText[],
    void        *pBuffer,
    WORD        bufferLength,
    void        *pDisplayFont,
    GOL_SCHEME  *pScheme
)
{
    CALCULATOR   *pCalc = NULL;                    //Text entry
    pCalc = (CALCULATOR *)GFX_malloc(sizeof(CALCULATOR));
    if(pCalc == NULL)
        return (NULL);

    pCalc->hdr.ID = ID;
    pCalc->hdr.pNxtObj = NULL;
    pCalc->hdr.type = OBJ_CALCULATOR;              // set object type
    pCalc->hdr.left = left;                       // left parameter of the text-entry
    pCalc->hdr.top = top;                         // top parameter of the text-entry
    pCalc->hdr.right = right;                     // right parameter of the text-entry
    pCalc->hdr.bottom = bottom;                   // bottom parameter of the text-entry
    pCalc->hdr.state = state;                     // State of the Text-Entry
    pCalc->horizontalKeys = horizontalKeys;       // number of horizontal keys
    pCalc->verticalKeys = verticalKeys;           // number of vertical keys
    pCalc->CurrentLength = 0;                     // current length of text
    pCalc->pHeadOfList = NULL;
    TeSetBuffer(pCalc, pBuffer, bufferLength);    // set the text to be displayed buffer length is also initialized in this call
    pCalc->pActiveKey = NULL;
    pCalc->hdr.DrawObj = CalcDraw;					// draw function
    pCalc->hdr.MsgObj = CalcTranslateMsg;			// message function
    pCalc->hdr.MsgDefaultObj = CalcMsgDefault;		// default message function
    pCalc->hdr.FreeObj = CalcDelKeyMembers;			// free function

    // Set the color scheme to be used
    if(pScheme == NULL)
        pCalc->hdr.pGolScheme = _pDefaultGolScheme;
    else
        pCalc->hdr.pGolScheme = (GOL_SCHEME *)pScheme;

    // Set the font to be used
    if(pDisplayFont == NULL)
        pCalc->pDisplayFont = (void *) &GOLFontDefault;
    else
        pCalc->pDisplayFont = pDisplayFont;

    //check if either values of horizontal keys and vertical keys are equal to zero
    if((pCalc->horizontalKeys != 0) || (pCalc->verticalKeys != 0))
    {

        //create the key members, return null if not successful
        if(CalcCreateKeyMembers(pCalc, pText) == 0)
        {
            GFX_free(pCalc);
            return (NULL);
        }
    }

    //Add this new widget object to the GOL list	
    GOLAddObject((OBJ_HEADER *)pCalc);
    return (pCalc);
}   //end CalcCreate()

/*********************************************************************
* Function: WORD TeDraw(void *pObj)
*
* Notes: This function draws the keys with their appropriate text
*
********************************************************************/
WORD TeDraw(void *pObj)
{
    static WORD         faceClr, embossLtClr, embossDkClr, xText, yText;
    static XCHAR        XcharTmp;
    static KEYMEMBER    *pKeyTemp = NULL;

    static WORD         CountOfKeys = 0;
    static WORD         counter = 0;
    static XCHAR        hideChar[2] = {0x2A, 0x00};

    typedef enum
    {
        CALC_START,
        CALC_HIDE_WIDGET,
        CALC_DRAW_PANEL,
        CALC_INIT_DRAW_EDITBOX,
        CALC_DRAW_EDITBOX,
        CALC_DRAW_KEY_INIT,
        CALC_DRAW_KEY_SET_PANEL,
        CALC_DRAW_KEY_DRAW_PANEL,
        CALC_DRAW_KEY_TEXT,
        CALC_DRAW_KEY_UPDATE,
        CALC_UPDATE_STRING_INIT,
        CALC_UPDATE_STRING,
        CALC_WAIT_ERASE_EBOX_AREA,
        CALC_UPDATE_CHARACTERS,
    } CALC_DRAW_STATES;

    static CALC_DRAW_STATES state = CALC_START;
    CALCULTATOR *pCalc;

    pCalc = (CALCULATOR *)pObj;

    if(IsDeviceBusy())
        return (0);

    switch(state)
    {
        case CALC_START:
            if(IsDeviceBusy())
                return (0);

            if(GetState(pCalc, CALC_HIDE))
            {
                SetColor(pCalc->hdr.pGolScheme->CommonBkColor);
                Bar(pCalc->hdr.left, pCalc->hdr.top, pCalc->hdr.right, pCalc->hdr.bottom);
                state = CALC_HIDE_WIDGET;
                goto Calc_hide_widget;
            }
            else
            {
                if(GetState(pCalc, CALC_DRAW))
                {

                    /************DRAW THE WIDGET PANEL*****************************/
                    GOLPanelDraw
                    (
                        pCalc->hdr.left,
                        pCalc->hdr.top,
                        pCalc->hdr.right,
                        pCalc->hdr.bottom,
                        0,
                        pCalc->hdr.pGolScheme->Color0,        //face color of panel
                        pCalc->hdr.pGolScheme->EmbossDkColor, //emboss dark color
                        pCalc->hdr.pGolScheme->EmbossLtColor, //emboss light color
                        NULL,
                        GOL_EMBOSS_SIZE
                    );
                    state = CALC_DRAW_PANEL;
                    goto Calc_draw_panel;
                }

                // update the keys (if TE_UPDATE_TEXT is also set it will also be redrawn)
                // at the states after the keys are updated
                else if(GetState(pCalc, CALC_UPDATE_KEY))
                {
                    state = CALC_DRAW_KEY_INIT;
                    goto Calc_draw_key_init_st;
                }

                // check if updating only the text displayed
                else if(GetState(pCalc, CALC_UPDATE_TEXT))
                {
                    state = CALC_UPDATE_STRING_INIT;
                    goto Calc_update_string_init_st;
                }
            }

        /*hide the widget*/
        case CALC_HIDE_WIDGET:
            Calc_hide_widget : if(IsDeviceBusy()) return (0);
            state = CALC_START;
            return (1);

        /*Draw the widget of the Text-Entry*/
        case CALC_DRAW_PANEL:
            Calc_draw_panel : if(!GOLPanelDrawTsk()) return (0);
            state = CALC_INIT_DRAW_EDITBOX;

        case CALC_INIT_DRAW_EDITBOX:

            //Draw the editbox
            GOLPanelDraw
            (
            pCalc->hdr.left,
            pCalc->hdr.top,
            pCalc->hdr.right,
            pCalc->hdr.top + GetTextHeight(pCalc->pDisplayFont) + (GOL_EMBOSS_SIZE << 1),
            0,
            pCalc->hdr.pGolScheme->Color1; ,
        pCalc->hdr.pGolScheme->EmbossDkColor,
        pCalc->hdr.pGolScheme->EmbossLtColor,
        NULL,
        GOL_EMBOSS_SIZE
            );

            state = CALC_DRAW_EDITBOX;

        case CALC_DRAW_EDITBOX:
            if(!GOLPanelDrawTsk())
                return (0);
            state = CALC_DRAW_KEY_INIT;

        /* ********************************************************************* */

        /*                  Update the keys                                      */

        /* ********************************************************************* */
        case CALC_DRAW_KEY_INIT:
            Calc_draw_key_init_st : embossLtClr = pCalc->hdr.pGolScheme->EmbossLtColor;
            embossDkClr = pCalc->hdr.pGolScheme->EmbossDkColor;
            faceClr = pCalc->hdr.pGolScheme->Color0;

            // if the active key update flag is set, only one needs to be redrawn
            if((GetState(pCalc, CALC_DRAW) != CALC_DRAW) && (pCalc->pActiveKey->update == TRUE))
            {
                CountOfKeys = (pCalc->horizontalKeys * pCalc->verticalKeys) -
                1;
                pKeyTemp = pCalc->pActiveKey;
            }
            else
            {
                CountOfKeys = 0;
                pKeyTemp = pTe->pHeadOfList;
            }

            state = CALC_DRAW_KEY_SET_PANEL;

        case CALC_DRAW_KEY_SET_PANEL:
            Calc_draw_key_set_panel_st : if(CountOfKeys < (pCalc->horizontalKeys * pCalc->verticalKeys))
            {

                // check if we need to draw the panel
                if(GetState(pCalc, CALC_DRAW) != CALC_DRAW)
                {
                    if(pKeyTemp->update == TRUE)
                    {

                        // set the colors needed
                        if(GetState(pCalc, CALC_KEY_PRESSED))
                        {
                            embossLtClr = pCalc->hdr.pGolScheme->EmbossDkColor;
                            embossDkClr = pCalc->hdr.pGolScheme->EmbossLtColor;
                            faceClr = pCalc->hdr.pGolScheme->Color1;
                        }
                        else
                        {
                            embossLtClr = pCalc->hdr.pGolScheme->EmbossLtColor;
                            embossDkClr = pCalc->hdr.pGolScheme->EmbossDkColor;
                            faceClr = pCalc->hdr.pGolScheme->Color0;
                        }
                    }
                    else
                    {
                        state = CALC_DRAW_KEY_UPDATE;
                        goto Calc_draw_key_update_st;
                    }
                }

                if(GetState(pCalc, CALC_DISABLED) == CALC_DISABLED)
                {
                    faceClr = SetColor(pCalc->hdr.pGolScheme->ColorDisabled);
                }

                // set up the panel
                GOLPanelDraw
                (
                    pKeyTemp->left,
                    pKeyTemp->top,
                    pKeyTemp->right,
                    pKeyTemp->bottom,
                    0,
                    faceClr,
                    embossLtClr,
                    embossDkClr,
                    NULL,
                    GOL_EMBOSS_SIZE
                );

                state = CALC_DRAW_KEY_DRAW_PANEL;
            }
            else
            {
                state = CALC_UPDATE_STRING_INIT;
                goto Calc_update_string_init_st;
            }

        case CALC_DRAW_KEY_DRAW_PANEL:
            if(!GOLPanelDrawTsk())
                return (0);

            // reset the update flag since the key panel is already redrawn   	
            pKeyTemp->update = FALSE;

            //set the text coordinates of the drawn key
            xText = ((pKeyTemp->left) + (pKeyTemp->right) - (pKeyTemp->textWidth)) >>
            1;
            yText = ((pKeyTemp->bottom) + (pKeyTemp->top) - (pKeyTemp->textHeight)) >>
            1;

            //set color of text
            // if the object is disabled, draw the disabled colors
            if(GetState(pCalc, CALC_DISABLED) == CALC_DISABLED)
            {
                SetColor(pCalc->hdr.pGolScheme->TextColorDisabled);
            }
            else
            {
                if((GetState(pCalc, CALC_DRAW) != CALC_DRAW) && (GetState(pTe, CALC_KEY_PRESSED)) == CALC_KEY_PRESSED)
                {
                    SetColor(pCalc->hdr.pGolScheme->TextColor1);
                }
                else
                {
                    SetColor(pCalc->hdr.pGolScheme->TextColor0);
                }
            }

            //output the text
            MoveTo(xText, yText);

            // set the font to be used
            SetFont(pCalc->hdr.pGolScheme->pFont);

            state = CALC_DRAW_KEY_TEXT;

        case CALC_DRAW_KEY_TEXT:
            if(!OutText(pKeyTemp->pKeyName))
                return (0);
            state = TE_DRAW_KEY_UPDATE;

        case TE_DRAW_KEY_UPDATE:
            te_draw_key_update_st :

            // update loop variables
            CountOfKeys++;
            pKeyTemp = pKeyTemp->pNextKey;

            state = TE_DRAW_KEY_SET_PANEL;
            goto te_draw_key_set_panel_st;

        /* ********************************************************************* */

        /*                  Update the displayed string                          */

        /* ********************************************************************* */
        case TE_UPDATE_STRING_INIT:
            te_update_string_init_st :

            // check if there are characters to remove
            if(pTe->pActiveKey != NULL)
            {
                if(pTe->pActiveKey->command == TE_DELETE_COM)
                {
                    if(pTe->CurrentLength == 0)
                    {
                        state = TE_START;
                        return (1);
                    }
                }
            }
            else
            {

                // check if text indeed needs to be updated
                if(pTe->CurrentLength == pTe->outputLenMax)
                {
                    state = TE_START;
                    return (1);
                }
            }

            //set the clipping region
            SetClipRgn
            (
                pTe->hdr.left + GOL_EMBOSS_SIZE,
                pTe->hdr.top + GOL_EMBOSS_SIZE,
                pTe->hdr.right - GOL_EMBOSS_SIZE,
                pTe->hdr.top + GOL_EMBOSS_SIZE + GetTextHeight(pTe->pDisplayFont)
            );

            SetClip(1);     //set the clipping
            if(GetState(pTe, TE_DRAW))
            {

                // update only the displayed text
                // position the string rendering on the right position
                if(GetState(pTe, TE_ECHO_HIDE))
                {

                    // fill the area with '*' character so we use the width of this character
                    MoveTo
                    (
                        pTe->hdr.right - 4 - GOL_EMBOSS_SIZE - (GetTextWidth(hideChar, pTe->pDisplayFont) * pTe->CurrentLength),
                        pTe->hdr.top + GOL_EMBOSS_SIZE
                    );
                }
                else
                {
                    MoveTo
                    (
                        pTe->hdr.right - 4 - GOL_EMBOSS_SIZE - GetTextWidth(pTe->pTeOutput, pTe->pDisplayFont),
                        pTe->hdr.top + GOL_EMBOSS_SIZE
                    );
                }
            }
            else if(GetState(pTe, TE_UPDATE_TEXT))
            {

                // erase the current text by drawing a bar over the edit box area
                SetColor(pTe->hdr.pGolScheme->Color1);
                Bar
                (
                    pTe->hdr.left + GOL_EMBOSS_SIZE,
                    pTe->hdr.top + GOL_EMBOSS_SIZE,
                    pTe->hdr.right - GOL_EMBOSS_SIZE,
                    pTe->hdr.top + GOL_EMBOSS_SIZE + GetTextHeight(pTe->pDisplayFont)
                );

                // we have to make sure we finish the Bar() first before we continue.
                state = TE_WAIT_ERASE_EBOX_AREA;
                goto te_wait_erase_ebox_area;
            }
            else
            {
                SetClip(0); //reset the clipping
                state = TE_START;
                return (1);
            }

            counter = 0;
            state = TE_UPDATE_STRING;
            goto te_update_string;

        case TE_WAIT_ERASE_EBOX_AREA:
            te_wait_erase_ebox_area : if(IsDeviceBusy()) return (0);

            // check if the command given is delete a character
            if(pTe->pActiveKey->command == TE_DELETE_COM)
            {
                *(pTe->pTeOutput + (--pTe->CurrentLength)) = 0;
            }

            // position the cursor to the start of string rendering
            // notice that we need to remove the characters first before we position the cursor when
            // deleting characters
            if(GetState(pTe, TE_ECHO_HIDE))
            {

                // fill the area with '*' character so we use the width of this character
                MoveTo
                (
                    pTe->hdr.right - 4 - GOL_EMBOSS_SIZE - (GetTextWidth(hideChar, pTe->pDisplayFont) * (pTe->CurrentLength)),
                    pTe->hdr.top + GOL_EMBOSS_SIZE
                );
            }
            else
            {
                MoveTo
                (
                    pTe->hdr.right - 4 - GOL_EMBOSS_SIZE - GetTextWidth(pTe->pTeOutput, pTe->pDisplayFont),
                    pTe->hdr.top + GOL_EMBOSS_SIZE
                );
            }

            counter = 0;
            state = TE_UPDATE_STRING;

        case TE_UPDATE_STRING:
            te_update_string :

            //output the text
            SetColor(pTe->hdr.pGolScheme->TextColor1);
            SetFont(pTe->pDisplayFont);

            // this is manually doing the OutText() function but with the capability to replace the
            // characters to the * character when hide echo is enabled.							
            XcharTmp = *((pTe->pTeOutput) + counter);
            if(XcharTmp < (XCHAR)15)
            {

                // update is done time to return to start and exit with success
                SetClip(0); //reset the clipping								
                state = TE_START;
                return (1);
            }
            else
            {
                if(GetState(pTe, TE_ECHO_HIDE))
                    OutChar(0x2A);
                else
                    OutChar(XcharTmp);
                state = TE_UPDATE_CHARACTERS;
            }

        case TE_UPDATE_CHARACTERS:
            if(IsDeviceBusy()) return (0);
            counter++;
            state = TE_UPDATE_STRING;
            goto te_update_string;
    }                       //end switch

    return (1);
}                           //end TeDraw()