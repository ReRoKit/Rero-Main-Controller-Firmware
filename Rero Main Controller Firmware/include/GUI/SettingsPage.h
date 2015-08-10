/*******************************************************************************
 * Functions for settings page in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef SETTINGSPAGE_H
#define	SETTINGSPAGE_H

#include "Microchip/Graphics/Graphics.h"



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

void vCreateSettingsPage(void);
WORD usMsgSettingsPage(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);


#endif	/* SETTINGSPAGE_H */
