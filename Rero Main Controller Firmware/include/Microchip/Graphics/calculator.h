#ifndef _CALCULATOR_H
    #define _CALCULATOR_H

    #include <Graphics/GOL.h>
	
	
/*********************************************************************
* Object States Definition: 
*********************************************************************/
    #define CALC_KEY_PRESSED  0x0004  // Bit for press state of one of the keys.
    #define CALC_DISABLED     0x0002  // Bit for disabled state.
//    #define CALC_ECHO_HIDE    0x0008  // Bit to hide the entered characters and instead echo "*" characters to the display.
    #define CALC_DRAW         0x4000  // Bit to indicate object must be redrawn.
    #define CALC_HIDE         0x8000  // Bit to indicate object must be removed from screen.
    #define CALC_UPDATE_KEY   0x2000  // Bit to indicate redraw of a key is needed.
    #define CALC_UPDATE_TEXT  0x1000  // Bit to indicate redraw of the text displayed is needed.
	
	
	
/*********************************************************************
* Optional COMMANDS assigned to keys 
*********************************************************************/
    #define CALC_DELETE_COM   0x01    // This macro is used to assign a "delete" command on a key.
//    #define CALC_SPACE_COM    0x02    // This macro is used to assign an insert "space" command on a key.
    #define CALC_ENTER_COM    0x03    // This macro is used to assign an "enter" (carriage return) command on a key.

// User can use this command to customize application code in the message

// callback function. Use the returned translated TE_MSG_ENTER to detect the key

// pressed was assigned the enter command. Refer to TeTranslateMsg() for details.

typedef struct
{
SHORT left; //left position of the key
SHORT top; //top position of the key
SHORT right; //right position of the key
SHORT bottom; //bottom position of the key
SHORT index; //index of the key in the list
WORD state; //State of the key. Either Pressed or Released.
WORD update; //flag to indicate key needs to be redrawn
WORD command; //command of the key
XCHAR *pKeyName; //text assigned to the key
SHORT textWidth; //computed text width, done at creation.
SHORT textHeight; //computed text height, done at creation.
void *pNextKey; //pointer to the next text
}CALCKEYMEMBER;

typedef struct
{
	OBJ_HEADER hdr;			//Generic header for all Objects. 
	SHORT horizontalkeys; 	// Number of Horizontal keys
	SHORT verticalkeys;		// Number of Vertical Keys
	XCHAR *pCalcOutput;		// pointer to the buffer assigned by the user
							// which holds the text shown in the editbox
	WORD CurrentLength; 	// length of array, keeps track if length<MaxSize
	WORD outputLenMax;		// Maximum length of the buffer pCalcOutput
	KEYMEMBER *pActiveKey;	// pointer to the currently active key in the KEYMEMBER list
	KEYMEMBER *pHeadOfList;	// pointer to the list of KEYMEMBER
	void *pDisplayFont;		// pointer to the font used in displaying text
	
}CALCULATOR;








	
	
