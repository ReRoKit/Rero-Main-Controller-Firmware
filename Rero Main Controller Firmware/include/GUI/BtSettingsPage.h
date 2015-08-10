/*******************************************************************************
 * Functions for Bluetooth settings page in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef BTSETTINGSPAGE_H
#define	BTSETTINGSPAGE_H

#include "Microchip/Graphics/Graphics.h"



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

void vCreateBtSettingsPage(void);
WORD usMsgBtSettingsPage(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);



#endif	/* BTSETTINGSPAGE_H */
