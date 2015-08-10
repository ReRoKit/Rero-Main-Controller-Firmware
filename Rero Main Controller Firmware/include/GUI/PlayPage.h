/*******************************************************************************
 * Functions for play page in rero GUI.
 *
 * Author: Kong Wai Weng
 * Company: Cytron Technologies Sdn. Bhd.
 * Email:  waiweng@cytron.com.my
 *******************************************************************************/

#ifndef PLAYPAGE_H
#define	PLAYPAGE_H

#include "Microchip/Graphics/Graphics.h"



/*******************************************************************************
 * PUBLIC DEFINITION
 *******************************************************************************/

// File type.
typedef enum __attribute__((packed)) {
    NOT_AVAILABLE,
    MOTION_FILE,
    PLANNER_FILE
} FILE_TYPE;



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

void vCreatePlay(void);
WORD usMsgPlay(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);
void vUpdatePlayPageTimeFrame(unsigned short usTimeFrame, unsigned short usTime);
void vUpdatePlayPageMsg1(char *szText);
void vUpdatePlayPageMsg2(char *szText);
void vUpdatePlayPageMsg3(char *szText);
void vUpdatePlayPageEndPlaying(FILE_TYPE ePlayingType);



#endif	/* PLAYPAGE_H */

