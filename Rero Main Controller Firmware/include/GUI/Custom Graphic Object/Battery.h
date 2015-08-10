/*******************************************************************************
 * Functions for battery object in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef _BATTERY_H
#define _BATTERY_H

#include <Graphics/GOL.h>
#include "GenericTypeDefs.h"



/*******************************************************************************
 * Object States Definition:
 *******************************************************************************/

#define BATT_HIDE       0x8000  // Bit to indicate battery must be hidden.
#define BATT_DRAW       0x4000  // Bit to indicate battery will be redrawn.



/*******************************************************************************
 * Overview: The structure contains data for battery icon
 *******************************************************************************/

typedef struct {
    OBJ_HEADER  hdr;        // Generic header for all Objects (see OBJ_HEADER).
} BATT;



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

BATT *BattCreate(WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, WORD state, GOL_SCHEME *pScheme);
WORD BattDraw(void *pObj);



#endif
