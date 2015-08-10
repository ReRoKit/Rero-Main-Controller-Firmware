/*******************************************************************************
 * Functions for game pad outline object in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef GAMEPADOUTLINE_H
#define	GAMEPADOUTLINE_H

#include <Graphics/GOL.h>
#include "GenericTypeDefs.h"



/*******************************************************************************
 * Object States Definition:
 *******************************************************************************/

#define GAMEPAD_HIDE    0x8000  // Bit to indicate object must be hidden.
#define GAMEPAD_DRAW    0x4000  // Bit to indicate object will be redrawn.



/*******************************************************************************
 * Overview: The structure contains data for game pad outline object
 *******************************************************************************/

typedef struct {
    OBJ_HEADER  hdr;            // Generic header for all Objects (see OBJ_HEADER).
    SHORT       PadRadius;      // Radius for the left and right pad.
    SHORT       RowBtnHeight;   // Height for the button row at the bottom.
    GFX_COLOR   Colour;         // Colour for the object.
} GAMEPAD;



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

GAMEPAD *GamePadCreate(WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, SHORT PadRadius, SHORT RowBtnHeight, WORD state, GFX_COLOR Colour);
WORD GamePadDraw(void *pObj);



#endif	/* GAMEPADOUTLINE_H */
