/*******************************************************************************
 * Functions for motion page in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef MOTIONPAGE_H
#define	MOTIONPAGE_H

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

void vCreateMotionPage(void);
WORD usMsgMotionPage(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);
void vUpdateMotionPageMsg1(char *szText);
void vUpdateMotionPageMsg2(char *szText);
void vUpdateMotionPageTimeFrame(unsigned short usFrame, unsigned short usTime);
void vUpdateMotionPageEndPlaying(FILE_TYPE ePlayingType);



#endif	/* MOTIONPAGE_H */
