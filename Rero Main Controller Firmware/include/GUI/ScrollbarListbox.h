/*******************************************************************************
 * Functions for listbox object with scroll bar.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef SCROLLBARLISTBOX_H
#define	SCROLLBARLISTBOX_H

#include "Microchip/Graphics/Graphics.h"



/*******************************************************************************
 * PUBLIC DEFINITION
 *******************************************************************************/

// Subpage position.
#define EM_SUBPAGE_LEFT             110
#define EM_SUBPAGE_RIGHT            WND_SUBPAGE_RIGHT
#define EM_SUBPAGE_TOP              WND_SUBPAGE_TOP
#define EM_SUBPAGE_BOTTOM           WND_SUBPAGE_BOTTOM



// External module type.
typedef enum __attribute__((packed)) {
    LB_FILTER_SERVO,
    LB_FILTER_SENSOR,
    LB_FILTER_ALL
} LB_FILTER;



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

void vCreateListboxExtModule(const char *szTitle);
void vEnableListboxExtModule(void);
void vDisableListboxExtModule(void);
LISTITEM *pxListboxExtModuleSelectPreviousItem(void);
LISTITEM *pxListboxExtModuleSelectNextItem(void);
void vClearExtModuleSubpage(void);
WORD usMsgListboxExtModule(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);
void taskSeekExtModule(void *pvParameters);



#endif	/* SCROLLBARLISTBOX_H */
