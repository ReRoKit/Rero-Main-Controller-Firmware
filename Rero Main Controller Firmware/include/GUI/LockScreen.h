/*******************************************************************************
 * Functions for lock screen in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef LOCKSCREEN_H
#define	LOCKSCREEN_H

#include "Microchip/Graphics/Graphics.h"



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

void vCreateLockScreen(void);
WORD usMsgLockScreen (WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);



#endif	/* LOCKSCREEN_H */
