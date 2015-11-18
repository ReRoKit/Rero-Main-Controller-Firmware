/*******************************************************************************
 * Functions for playing motion file.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef PLAYMOTION_H
#define	PLAYMOTION_H



/*******************************************************************************
 * PUBLIC DEFINITION
 *******************************************************************************/

// State of the motion file.
typedef enum __attribute__((packed)) {
    STOP_NOW,
    STOP_FRAME,
    STOP_FILE,
    STOP_BT,
    PAUSE,
    PLAY
} MOTION_STATE;

// Return code for playing planner or motion file.
typedef enum __attribute__((packed)) {
    PLAY_NO_ERROR = 0,          // No error.
    PLAY_FILE_ALREADY_OPENED,   // The file is already opened.
    PLAY_CANT_OPEN_FILE,        // Can't open file of file not found.
    PLAY_CANT_CREATE_TASK       // Can't create the RTOS task to play the file.
} PLAY_RESULT;



/*******************************************************************************
* PUBLIC FUNCTION PROTOTYPES                                                   *
*******************************************************************************/

PLAY_RESULT ePlayMotionStart(const char* szMotionFileName);
void vPlayMotionStop(const char* szMotionFileName, MOTION_STATE eStopMode);
void vPlayMotionStopAll(MOTION_STATE eStopMode);
unsigned char ucIsMotionPlaying(void);
void taskPlayMotion (void *pvParameters);


#endif
