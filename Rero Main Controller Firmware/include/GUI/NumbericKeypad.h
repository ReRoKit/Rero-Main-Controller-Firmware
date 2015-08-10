/*******************************************************************************
 * Functions for numberic keypad in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef NUMBERICKEYPAD_H
#define	NUMBERICKEYPAD_H

#include "Microchip/Graphics/Graphics.h"



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

void vCreateNumericKeypadPage(void);
WORD usMsgNumericKeypadPage(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);



#endif	/* NUMBERICKEYPAD_H */
