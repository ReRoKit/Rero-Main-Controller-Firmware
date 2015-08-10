/*******************************************************************************
 * Functions for G15 page in rero GUI.
 *
 * Author: Kong Wai Weng
 * Company: Cytron Technologies Sdn. Bhd.
 * Email:  waiweng@cytron.com.my
 *******************************************************************************/

#ifndef G15PAGE_H
#define	G15PAGE_H

#include "Microchip/Graphics/Graphics.h"



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

void vCreateG15Page(void);
WORD usMsgG15Page(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);
void vUpdateMeterAngle(unsigned short usAngle);
void vUpdateMeterLimit(unsigned short usLimitCw, unsigned short usLimitCcw);



#endif	/* G15PAGE_H */
