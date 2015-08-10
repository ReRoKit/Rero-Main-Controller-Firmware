/*******************************************************************************
 * Functions for USB page in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef USBPAGE_H
#define	USBPAGE_H

#include "Microchip/Graphics/Graphics.h"



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

void vCreateUsbPage(void);
void vDisplayUsbPage(void);
WORD usMsgUsbPage(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);



#endif	/* USBPAGE_H */

