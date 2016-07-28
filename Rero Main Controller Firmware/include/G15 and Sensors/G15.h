/*******************************************************************************
 * This file provides the functions to communicate with the G15.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef G15_H
#define	G15_H

#include "G15 and Sensors/ExtModule.h"



/*******************************************************************************
 * PUBLIC DEFINITION
 *******************************************************************************/

// Position Control Mode.
typedef enum __attribute__((packed)) {
    NORMAL_POSITIONING          = 0x0000,
    DIRECTION_POSITIONING_CCW   = 0x8000,
    DIRECTION_POSITIONING_CW    = 0xc000
} G15_POSITIONING_MODE;

// Speed Control Mode.
typedef enum __attribute__((packed)) {
    POSITION_SPEED_CONTROL      = 0x0000,
    POSITION_TIME_CONTROL       = 0x8000,
    WHEEL_CCW                   = 0x0000,
    WHEEL_CW                    = 0x0400
} G15_SPEED_CONTROL_MODE;



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

EM_ERROR eG15SetPositionSpeed(unsigned char ucId, EM_WRITE_MODE eWriteMode,
                                 unsigned short usPosition, G15_POSITIONING_MODE ePositioningMode,
                                 unsigned short usSpeed, G15_SPEED_CONTROL_MODE eSpeedControlMode);

EM_ERROR eG15SetPosition(unsigned char ucId, EM_WRITE_MODE eWriteMode,
                           unsigned short usPosition, G15_POSITIONING_MODE ePositioningMode);

EM_ERROR eG15GetPosition(unsigned char ucId, unsigned short *pusPosition);

EM_ERROR eG15SetSpeed(unsigned char ucId, EM_WRITE_MODE eWriteMode,
                        unsigned short usSpeed, G15_SPEED_CONTROL_MODE eSpeedControlMode);

EM_ERROR eG15SetTorqueLed(unsigned char ucId, EM_WRITE_MODE eWriteMode, unsigned char ucTorque, unsigned char ucLed);
EM_ERROR eG15SetTorque(unsigned char ucId, EM_WRITE_MODE eWriteMode, unsigned char ucTorque);
EM_ERROR eG15SetLed(unsigned char ucId, EM_WRITE_MODE eWriteMode, unsigned char ucLed);
EM_ERROR eG15SetAngleLimit(unsigned char ucId, unsigned short usLimitCw, unsigned short usLimitCcw);
EM_ERROR eG15GetAngleLimit(unsigned char ucId, unsigned short *pusLimitCw, unsigned short *pusLimitCcw);
EM_ERROR eG15SetComplianceSlope(unsigned char ucId, unsigned char ucComplianceSlopeCw, unsigned char ucComplianceSlopeCcw);
EM_ERROR eG15SetTorqueLimit(unsigned char ucId, unsigned short usTorqueLimit);
EM_ERROR eG15ResetRegister(unsigned char ucId);



#endif	/* G15_H */
