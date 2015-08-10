/*******************************************************************************
 * This file provides the functions to communicate with the rero sensors.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef SENSORS_H
#define	SENSORS_H

#include "G15 and Sensors/ExtModule.h"



/*******************************************************************************
 * PUBLIC DEFINITION
 *******************************************************************************/

// Colour sensor detected colour.
typedef enum __attribute__((packed)) {
    CL_RED          = 0,
    CL_GREEN        = 1,
    CL_BLUE         = 2,
    CL_CYAN         = 3,
    CL_MAGENTA      = 4,
    CL_YELLOW       = 5,
    CL_WHITE        = 6,
    CL_ANY_COLOUR   = 7,
    CL_BLACK        = 8
} CL_SENSOR_COLOUR;



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

EM_ERROR eSensorSetLed(unsigned char ucId, EM_MODEL eModuleType, unsigned char ucLedValue);

EM_ERROR eHeadSetLed(unsigned char ucId, unsigned char ucRgb, unsigned char ucLeftRight);
EM_ERROR eHeadSetRed(unsigned char ucId, unsigned char ucRedValue);
EM_ERROR eHeadSetGreen(unsigned char ucId, unsigned char ucGreenValue);
EM_ERROR eHeadSetBlue(unsigned char ucId, unsigned char ucBlueValue);
EM_ERROR eHeadGetIrValue(unsigned char ucId, unsigned char *pucValue);
EM_ERROR eHeadGetMicValue(unsigned char ucId, unsigned char *pucValue);

EM_ERROR eInfraredSensorGetValue(unsigned char ucId, unsigned char *pucValue);
EM_ERROR eUltrasonicSensorGetValue(unsigned char ucId, unsigned short *pusValue);
EM_ERROR eTactileSensorGetValue(unsigned char ucId, unsigned char *pucValue);

EM_ERROR eLineSensorGetAnalog(unsigned char ucId, unsigned char *pucS1, unsigned char *pucS2, unsigned char *pucS3, unsigned char *pucS4);
EM_ERROR eLineSensorGetDigital(unsigned char ucId, unsigned char *pucValue);
EM_ERROR eLineSensorGetDarkLight(unsigned char ucId, unsigned char *pucValue);
EM_ERROR eLineSensorSetDarkLight(unsigned char ucId, unsigned char ucDarkLight);
EM_ERROR eLineSensorSetAutoCal(unsigned char ucId, unsigned char ucValue);

EM_ERROR eColourSensorGetRgb(unsigned char ucId, unsigned char *pucRed, unsigned char *pucGreen, unsigned char *pucBlue);
EM_ERROR eColourSensorGetBrightness(unsigned char ucId, unsigned char *pucValue);
EM_ERROR eColourSensorGetAmbient(unsigned char ucId, unsigned char *pucValue);
EM_ERROR eColourSensorGetColour(unsigned char ucId, CL_SENSOR_COLOUR *peColour);



#endif	/* SENSORS_H */
