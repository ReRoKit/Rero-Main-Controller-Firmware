/*******************************************************************************
 * Functions to handle the rero planner file.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef PLANNER_H
#define	PLANNER_H

#include "PlayMotion.h"



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

PLAY_RESULT ePlannerRun(const char* szPlannerFileName);
void vPlannerStop(const char* szPlannerFileName, MOTION_STATE eStopMode);
void vPlannerStopCurrent(MOTION_STATE eStopMode);
unsigned char ucDeleteProgramFiles(const char *szFilenameHead);
unsigned char ucIsPlannerPlaying(void);
void taskPlanner(void *pvParameters);



#endif	/* PLANNER_H */
