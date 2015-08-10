/*******************************************************************************
 * Functions for pop up message in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef POPUPMESSAGEPAGE_H
#define	POPUPMESSAGEPAGE_H

#include "Microchip/Graphics/Graphics.h"



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

void vCreateGenericMessagePage(char *szText);
void vCreateLowBattShutdownMessage(void);
void vCreateShutdownErrorMessage(void);



#endif	/* POPUPMESSAGEPAGE_H */
