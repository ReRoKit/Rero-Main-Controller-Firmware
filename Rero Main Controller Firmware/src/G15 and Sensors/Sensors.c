/*******************************************************************************
 * This file provides the functions to communicate with the rero sensors.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "G15 and Sensors/Sensors.h"
#include "Variables.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

// Threshold for colour sensor.
#define CL_SENSOR_THRESHOLD     6

// IR Sensor Control Table Address.
typedef enum __attribute__((packed)) {
    IR_ADD_RETURN_ENABLE    = 0x06,
    IR_ADD_THRESHOLD        = 0x07,
    IR_ADD_ALARM_LED        = 0x08,
    IR_ADD_LED              = 0x09,
    IR_ADD_VALUE            = 0x0a,
    IR_ADD_TRIGGER          = 0x0b,
    IR_ADD_AUTO_CAL         = 0x0c,
    IR_ADD_LOCK             = 0x0d
} IR_CT_ADD;


// Ultrasonic Sensor Control Table Address.
typedef enum __attribute__((packed)) {
    US_ADD_RETURN_ENABLE    = 0x06,
    US_ADD_THRESHOLD_L      = 0x07,
    US_ADD_THRESHOLD_H      = 0x08,
    US_ADD_ALARM_LED        = 0x09,
    US_ADD_LED              = 0x0a,
    US_ADD_VALUE_L          = 0x0b,
    US_ADD_VALUE_H          = 0x0c,
    US_ADD_PING             = 0x0d,
    US_ADD_TRIGGER          = 0x0e,
    US_ADD_LOCK             = 0x0f
} US_CT_ADD;


// Head Module Control Table Address.
typedef enum __attribute__((packed)) {
    HM_ADD_RETURN_ENABLE    = 0x06,
    HM_ADD_IR_THRESHOLD     = 0x07,
    HM_ADD_MIC_THRESHOLD    = 0x08,
    HM_ADD_ALARM_LED        = 0x09,
    HM_ADD_RGB              = 0x0a,
    HM_ADD_LED_L_R          = 0x0b,
    HM_ADD_MIC_MAX          = 0x0c,
    HM_ADD_MIC_CNT          = 0x0d,
    HM_ADD_MIC_VALUE        = 0x0e,
    HM_ADD_MIC_TRIGGER      = 0x0f,
    HM_ADD_IR_VALUE         = 0x10,
    HM_ADD_IR_TRIGGER       = 0x11,
    HM_ADD_IR_AUTO_CAL      = 0x12,
    HM_ADD_REGISTERED       = 0x13,
    HM_ADD_LOCK             = 0x14
} HM_CT_ADD;


// Tactile Sensor Control Table Address.
typedef enum __attribute__((packed)) {
    TT_ADD_RETURN_ENABLE    = 0x06,
    TT_ADD_IR_THRESHOLD     = 0x07,
    TT_ADD_ALARM_LED        = 0x08,
    TT_ADD_LED              = 0x09,
    TT_ADD_IR_VALUE         = 0x0a,
    TT_ADD_TRIGGER          = 0x0b,
    TT_ADD_RESERVED         = 0x0c,
    TT_ADD_LOCK             = 0x0d
} TC_CT_ADD;


// Analog Input Module Control Table Address.
typedef enum __attribute__((packed)) {
    AI_ADD_RETURN_ENABLE    = 0x06,
    AI_ADD_ALARM_LED        = 0x0c,
    AI_ADD_LED              = 0x0d,
    AI_ADD_AN_S1            = 0x0e,
    AI_ADD_AN_S2            = 0x0f,
    AI_ADD_AN_S3            = 0x10,
    AI_ADD_AN_S4            = 0x11,
    AI_ADD_LOCK             = 0x15
} AI_CT_ADD;


// Colour Sensor Control Table Address.
typedef enum __attribute__((packed)) {
    CL_ADD_RETURN_ENABLE    = 0x06,
    CL_ADD_RESERVED         = 0x07,
    CL_ADD_ALARM_LED        = 0x08,
    CL_ADD_LED              = 0x09,
    CL_ADD_AN_RED           = 0x0a,
    CL_ADD_AN_GREEN         = 0x0b,
    CL_ADD_AN_BLUE          = 0x0c,
    CL_ADD_BRIGHTNESS       = 0x0d,
    CL_ADD_AMBIENT          = 0x0e,
    CL_ADD_LOCK             = 0x0f
} CL_CT_ADD;


// Line Sensor Control Table Address.
typedef enum __attribute__((packed)) {
    LN_ADD_RETURN_ENABLE    = 0x06,
    LN_ADD_DARK_LIGHT       = 0x07,
    LN_ADD_THRESHOLD_S1     = 0x08,
    LN_ADD_THRESHOLD_S2     = 0x09,
    LN_ADD_THRESHOLD_S3     = 0x0a,
    LN_ADD_THRESHOLD_S4     = 0x0b,
    LN_ADD_ALARM_LED        = 0x0c,
    LN_ADD_LED              = 0x0d,
    LN_ADD_AN_S1            = 0x0e,
    LN_ADD_AN_S2            = 0x0f,
    LN_ADD_AN_S3            = 0x10,
    LN_ADD_AN_S4            = 0x11,
    LN_ADD_DIGITAL          = 0x12,
    LN_ADD_LINE_POSITION    = 0x13,
    LN_ADD_AUTO_CAL         = 0x14,
    LN_ADD_LOCK             = 0x15
} LN_CT_ADD;


// RGB Light Module Control Table Address.
typedef enum __attribute__((packed)) {
    RL_ADD_RETURN_ENABLE    = 0x06,
    RL_ADD_RESERVED         = 0x07,
    RL_ADD_ALARM_LED        = 0x08,
    RL_ADD_LED              = 0x09,
    RL_ADD_RED              = 0x0a,
    RL_ADD_GREEN            = 0x0b,
    RL_ADD_BLUE             = 0x0c,
    RL_ADD_BRIGHTNESS       = 0x0d, // not in use
    RL_ADD_LOCK             = 0x0e
} RL_CT_ADD;


/*******************************************************************************
 * FUNCTION: eSensorSetLed
 *
 * PARAMETERS:
 * ~ ucId           - Sensor ID.
 * ~ eModuleType    - Module type.
 * ~ ucLedValue     - Sensor LED value.
 *                    0 - Off
 *                    1 - On
 *                    2 - Trigger
 *
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Turn on/off the sensor LED.
 *
 *******************************************************************************/
EM_ERROR eSensorSetLed(unsigned char ucId, EM_MODEL eModuleType, unsigned char ucLedValue)
{
    unsigned char pucTxParam[2];

    // Determine the control table address base on sensor type.
    switch (eModuleType) {
        case EM_MODEL_IR:       pucTxParam[0] = IR_ADD_LED;     break;
        case EM_MODEL_US:       pucTxParam[0] = US_ADD_LED;     break;
        case EM_MODEL_TACTILE:  pucTxParam[0] = TT_ADD_LED;     break;
        case EM_MODEL_COLOUR:   pucTxParam[0] = CL_ADD_LED;     break;
        case EM_MODEL_LINE:     pucTxParam[0] = LN_ADD_LED;     break;
        case EM_MODEL_RGBLIGHT: pucTxParam[0] = RL_ADD_LED;     break;
        default:                return EM_NO_ERROR;             break;
    }

    // LED value.
    pucTxParam[1] = ucLedValue;

    return eEMSendReceivePacket(ucId, EM_INST_WRITE_DATA, sizeof(pucTxParam), pucTxParam, NULL);
}



/*******************************************************************************
 * FUNCTION: eHeadSetLed
 *
 * PARAMETERS:
 * ~ ucId           - Sensor ID.
 * ~ ucRgb          - RGB value for the LED (RRRGGGBB).
 * ~ ucLeftRight    - Turn on/off left and right LED individually.
 *                    Bit 0 - Right
 *                    Bit 1 - Left
 *
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Turn on/off and set the colour of the Head LED.
 *
 *******************************************************************************/
EM_ERROR eHeadSetLed(unsigned char ucId, unsigned char ucRgb, unsigned char ucLeftRight)
{
    unsigned char pucTxParam[3];

    // Set the RGB value and left-right LED control value.
    pucTxParam[0] = HM_ADD_RGB;         // Control table address for RGB value.
    pucTxParam[1] = ucRgb;          // RGB value.
    pucTxParam[2] = ucLeftRight;    // Left right LED control value.
    
    return eEMSendReceivePacket(ucId, EM_INST_WRITE_DATA, sizeof(pucTxParam), pucTxParam, NULL);
}



/*******************************************************************************
 * FUNCTION: eHeadSetRed
 *
 * PARAMETERS:
 * ~ ucId           - Sensor ID.
 * ~ ucRedValue     - Red LED value (3 bits).
 *
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Set the red colour LED of the head module.
 *
 *******************************************************************************/
EM_ERROR eHeadSetRed(unsigned char ucId, unsigned char ucRedValue)
{
    EM_ERROR eErrorCode = EM_NO_ERROR;
    unsigned char ucRgb;
    unsigned char pucTxParam[2];

    // Read the RGB value.
    pucTxParam[0] = HM_ADD_RGB;         // Control table address for RGB value.
    pucTxParam[1] = 1;              // Number of bytes to read.
    eErrorCode |= eEMSendReceivePacket(ucId, EM_INST_READ_DATA, sizeof(pucTxParam), pucTxParam, &ucRgb);


    // Set the red colour.
    ucRgb &= 0b00011111;
    ucRgb |= (ucRedValue & 0b111) << 5;

    // Set the RGB value.
    pucTxParam[0] = HM_ADD_RGB;         // Control table address for RGB value.
    pucTxParam[1] = ucRgb;          // RGB value.

    eErrorCode |= eEMSendReceivePacket(ucId, EM_INST_WRITE_DATA, sizeof(pucTxParam), pucTxParam, NULL);
    return eErrorCode;
}



/*******************************************************************************
 * FUNCTION: eHeadSetGreen
 *
 * PARAMETERS:
 * ~ ucId           - Sensor ID.
 * ~ ucGreenValue   - Green LED value (3 bits).
 *
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Set the green colour LED of the head module.
 *
 *******************************************************************************/
EM_ERROR eHeadSetGreen(unsigned char ucId, unsigned char ucGreenValue)
{
    EM_ERROR eErrorCode = EM_NO_ERROR;
    unsigned char ucRgb;
    unsigned char pucTxParam[2];

    // Read the RGB value.
    pucTxParam[0] = HM_ADD_RGB;         // Control table address for RGB value.
    pucTxParam[1] = 1;              // Number of bytes to read.
    eErrorCode |= eEMSendReceivePacket(ucId, EM_INST_READ_DATA, sizeof(pucTxParam), pucTxParam, &ucRgb);


    // Set the green colour.
    ucRgb &= 0b11100011;
    ucRgb |= (ucGreenValue & 0b111) << 2;

    // Set the RGB value.
    pucTxParam[0] = HM_ADD_RGB;         // Control table address for RGB value.
    pucTxParam[1] = ucRgb;          // RGB value.

    eErrorCode |= eEMSendReceivePacket(ucId, EM_INST_WRITE_DATA, sizeof(pucTxParam), pucTxParam, NULL);
    return eErrorCode;
}



/*******************************************************************************
 * FUNCTION: eHeadSetBlue
 *
 * PARAMETERS:
 * ~ ucId           - Sensor ID.
 * ~ ucBlueValue    - Blue LED value (2 bits).
 *
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Set the blue colour LED of the head module.
 *
 *******************************************************************************/
EM_ERROR eHeadSetBlue(unsigned char ucId, unsigned char ucBlueValue)
{
    EM_ERROR eErrorCode = EM_NO_ERROR;
    unsigned char ucRgb;
    unsigned char pucTxParam[2];

    // Read the RGB value.
    pucTxParam[0] = HM_ADD_RGB;         // Control table address for RGB value.
    pucTxParam[1] = 1;              // Number of bytes to read.
    eErrorCode |= eEMSendReceivePacket(ucId, EM_INST_READ_DATA, sizeof(pucTxParam), pucTxParam, &ucRgb);


    // Set the blue colour.
    ucRgb &= 0b11111100;
    ucRgb |= ucBlueValue & 0b11;

    // Set the RGB value.
    pucTxParam[0] = HM_ADD_RGB;         // Control table address for RGB value.
    pucTxParam[1] = ucRgb;          // RGB value.

    eErrorCode |= eEMSendReceivePacket(ucId, EM_INST_WRITE_DATA, sizeof(pucTxParam), pucTxParam, NULL);
    return eErrorCode;
}



/*******************************************************************************
 * FUNCTION: eHeadGetIrValue
 *
 * PARAMETERS:
 * ~ ucId           - Head ID.
 * ~ pucValue       - Buffer to return the value.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Get the IR sensor value on head module.
 *
 *******************************************************************************/
EM_ERROR eHeadGetIrValue(unsigned char ucId, unsigned char *pucValue)
{
    unsigned char pucTxParam[2];

    // Control table address.
    pucTxParam[0] = HM_ADD_IR_VALUE;
    
    // Number of bytes to read.
    pucTxParam[1] = 1;

    // Send the packet and return the error code.
    return eEMSendReceivePacket(ucId, EM_INST_READ_DATA, sizeof(pucTxParam), pucTxParam, pucValue);
}



/*******************************************************************************
 * FUNCTION: eHeadGetMicValue
 *
 * PARAMETERS:
 * ~ ucId           - Head ID.
 * ~ pucValue       - Buffer to return the value.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Get the mic value on head module.
 *
 *******************************************************************************/
EM_ERROR eHeadGetMicValue(unsigned char ucId, unsigned char *pucValue)
{
    unsigned char pucTxParam[2];

    // Control table address.
    pucTxParam[0] = HM_ADD_MIC_VALUE;
    
    // Number of bytes to read.
    pucTxParam[1] = 1;

    // Send the packet and return the error code.
    return eEMSendReceivePacket(ucId, EM_INST_READ_DATA, sizeof(pucTxParam), pucTxParam, pucValue);
}



/*******************************************************************************
 * FUNCTION: eInfraredSensorGetValue
 *
 * PARAMETERS:
 * ~ ucId           - Sensor ID.
 * ~ pucValue       - Buffer to return the value.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Get the infrared sensor value.
 *
 *******************************************************************************/
EM_ERROR eInfraredSensorGetValue(unsigned char ucId, unsigned char *pucValue)
{
    unsigned char pucTxParam[2];

    // Control table address.
    pucTxParam[0] = IR_ADD_VALUE;
    
    // Number of bytes to read.
    pucTxParam[1] = 1;

    // Send the packet and return the error code.
    return eEMSendReceivePacket(ucId, EM_INST_READ_DATA, sizeof(pucTxParam), pucTxParam, pucValue);
}



/*******************************************************************************
 * FUNCTION: eUltrasonicSensorGetValue
 *
 * PARAMETERS:
 * ~ ucId           - Sensor ID.
 * ~ pusValue       - Buffer to return the value.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Get the ultrasonic sensor value.
 *
 *******************************************************************************/
EM_ERROR eUltrasonicSensorGetValue(unsigned char ucId, unsigned short *pusValue)
{
    EM_ERROR eErrorCode = EM_NO_ERROR;
    unsigned char pucTxParam[2];

    // Trigger the ultrasonic sensor to start pinging.
    pucTxParam[0] = US_ADD_PING;    // Control table address.
    pucTxParam[1] = 1;              // Number of bytes to write.
    eErrorCode |= eEMSendReceivePacket(ucId, EM_INST_WRITE_DATA, sizeof(pucTxParam), pucTxParam, NULL);

    // Wait for the pinging process to complete.
    if (xSystemState.bRtosRunning == 1) {
        vTaskDelay(30 / portTICK_RATE_MS);
    } else {
        DelayMs(30);
    }

    // Read a double byte value.
    pucTxParam[0] = US_ADD_VALUE_L;     // Control table address.
    pucTxParam[1] = 2;                  // Number of bytes to read.
    eErrorCode |= eEMSendReceivePacket(ucId, EM_INST_READ_DATA, sizeof(pucTxParam), pucTxParam, (unsigned char*)pusValue);
    
    // Return the error code.
    return eErrorCode;
}



/*******************************************************************************
 * FUNCTION: eTactileSensorGetValue
 *
 * PARAMETERS:
 * ~ ucId           - Sensor ID.
 * ~ pucValue       - Buffer to return the value.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Get the tactile sensor value.
 * ~ 0 - Released
 * ~ 1 - Pressed
 *
 *******************************************************************************/
EM_ERROR eTactileSensorGetValue(unsigned char ucId, unsigned char *pucValue)
{
    unsigned char pucTxParam[2];

    // Control table address.
    pucTxParam[0] = TT_ADD_TRIGGER;
    
    // Number of bytes to read.
    pucTxParam[1] = 1;

    // Send the packet and return the error code.
    return eEMSendReceivePacket(ucId, EM_INST_READ_DATA, sizeof(pucTxParam), pucTxParam, pucValue);
}



/*******************************************************************************
 * FUNCTION: eLineSensorGetAnalog
 *
 * PARAMETERS:
 * ~ ucId           - Sensor ID.
 * ~ pucS1          - Buffer to return the value for sensor 1.
 * ~ pucS2          - Buffer to return the value for sensor 2.
 * ~ pucS3          - Buffer to return the value for sensor 3.
 * ~ pucS4          - Buffer to return the value for sensor 4.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Get the analog value from line sensor.
 *
 *******************************************************************************/
EM_ERROR eLineSensorGetAnalog(unsigned char ucId, unsigned char *pucS1, unsigned char *pucS2, unsigned char *pucS3, unsigned char *pucS4)
{
    unsigned char pucTxParam[2];
    unsigned char pucRxBuffer[4];

    // Control table address.
    pucTxParam[0] = LN_ADD_AN_S1;
    
    // Number of bytes to read.
    pucTxParam[1] = 4;

    // Send the packet and 
    EM_ERROR eErrorCode = eEMSendReceivePacket(ucId, EM_INST_READ_DATA, sizeof(pucTxParam), pucTxParam, pucRxBuffer);
    
    // Save the RGB value.
    *pucS1 = pucRxBuffer[0];
    *pucS2 = pucRxBuffer[1];
    *pucS3 = pucRxBuffer[2];
    *pucS4 = pucRxBuffer[3];
    
    // Return the error code.
    return eErrorCode;
}



/*******************************************************************************
 * FUNCTION: eLineSensorGetDigital
 *
 * PARAMETERS:
 * ~ ucId           - Sensor ID.
 * ~ pucValue       - Buffer to return the value.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Get the line sensor digital value.
 * ~ Bit 0 - S1
 * ~ Bit 1 - S2
 * ~ Bit 2 - S3
 * ~ Bit 3 - S4
 *
 *******************************************************************************/
EM_ERROR eLineSensorGetDigital(unsigned char ucId, unsigned char *pucValue)
{
    unsigned char pucTxParam[2];

    // Control table address.
    pucTxParam[0] = LN_ADD_DIGITAL;
    
    // Number of bytes to read.
    pucTxParam[1] = 1;

    // Send the packet and return the error code.
    return eEMSendReceivePacket(ucId, EM_INST_READ_DATA, sizeof(pucTxParam), pucTxParam, pucValue);
}



/*******************************************************************************
 * FUNCTION: eLineSensorGetDarkLight
 *
 * PARAMETERS:
 * ~ ucId       - Sensor ID.
 * ~ pucValue   - Buffer to return the value.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Get the line sensor setting whether to detect light or dark line.
 * ~ 0 = Dark on.
 * ~ 1 = Light on.
 *
 *******************************************************************************/
EM_ERROR eLineSensorGetDarkLight(unsigned char ucId, unsigned char *pucValue)
{
    unsigned char pucTxParam[2];

    // Control table address.
    pucTxParam[0] = LN_ADD_DARK_LIGHT;
    
    // Number of bytes to read.
    pucTxParam[1] = 1;

    return eEMSendReceivePacket(ucId, EM_INST_READ_DATA, sizeof(pucTxParam), pucTxParam, pucValue);
}



/*******************************************************************************
 * FUNCTION: eLineSensorSetDarkLight
 *
 * PARAMETERS:
 * ~ ucId           - Sensor ID.
 * ~ ucDarkLight    - Light on or dark on.
 *                  - 0 = Dark on.
 *                  - 1 = Light on.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Set the line sensor to detect light or dark line.
 *
 *******************************************************************************/
EM_ERROR eLineSensorSetDarkLight(unsigned char ucId, unsigned char ucDarkLight)
{
    unsigned char pucTxParam[2];

    // Control table address.
    pucTxParam[0] = LN_ADD_DARK_LIGHT;

    // Value.
    pucTxParam[1] = ucDarkLight;

    return eEMSendReceivePacket(ucId, EM_INST_WRITE_DATA, sizeof(pucTxParam), pucTxParam, NULL);
}



/*******************************************************************************
 * FUNCTION: eLineSensorSetAutoCal
 *
 * PARAMETERS:
 * ~ ucId       - Sensor ID.
 * ~ ucValue    - Value to start or stop the auto calibration.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Start or stop the auto calibration for line sensor.
 *
 *******************************************************************************/
EM_ERROR eLineSensorSetAutoCal(unsigned char ucId, unsigned char ucValue)
{
    unsigned char pucTxParam[2];

    // Control table address.
    pucTxParam[0] = LN_ADD_AUTO_CAL;

    // Value.
    pucTxParam[1] = ucValue;

    return eEMSendReceivePacket(ucId, EM_INST_WRITE_DATA, sizeof(pucTxParam), pucTxParam, NULL);
}



/*******************************************************************************
 * FUNCTION: eColourSensorGetRgb
 *
 * PARAMETERS:
 * ~ ucId           - Sensor ID.
 * ~ pucRed         - Buffer to return the red value.
 * ~ pucGreen       - Buffer to return the green value.
 * ~ pucBlue        - Buffer to return the blue value.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Get the RGB value from colour sensor.
 *
 *******************************************************************************/
EM_ERROR eColourSensorGetRgb(unsigned char ucId, unsigned char *pucRed, unsigned char *pucGreen, unsigned char *pucBlue)
{
    unsigned char pucTxParam[2];
    unsigned char pucRxBuffer[3];

    // Control table address.
    pucTxParam[0] = CL_ADD_AN_RED;
    
    // Number of bytes to read.
    pucTxParam[1] = 3;

    // Send the packet and 
    EM_ERROR eErrorCode = eEMSendReceivePacket(ucId, EM_INST_READ_DATA, sizeof(pucTxParam), pucTxParam, pucRxBuffer);
    
    // Save the RGB value.
    *pucRed = pucRxBuffer[0];
    *pucGreen = pucRxBuffer[1];
    *pucBlue = pucRxBuffer[2];
    
    // Return the error code.
    return eErrorCode;
}



/*******************************************************************************
 * FUNCTION: eColourSensorGetBrightness
 *
 * PARAMETERS:
 * ~ ucId           - Sensor ID.
 * ~ pucValue       - Buffer to return the value.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Get the brightness from colour sensor.
 *
 *******************************************************************************/
EM_ERROR eColourSensorGetBrightness(unsigned char ucId, unsigned char *pucValue)
{
    unsigned char pucTxParam[2];

    // Control table address.
    pucTxParam[0] = CL_ADD_BRIGHTNESS;
    
    // Number of bytes to read.
    pucTxParam[1] = 1;

    // Send the packet and return the error code.
    return eEMSendReceivePacket(ucId, EM_INST_READ_DATA, sizeof(pucTxParam), pucTxParam, pucValue);
}



/*******************************************************************************
 * FUNCTION: eColourSensorGetAmbient
 *
 * PARAMETERS:
 * ~ ucId           - Sensor ID.
 * ~ pucValue       - Buffer to return the value.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Get the ambient light value from colour sensor.
 *
 *******************************************************************************/
EM_ERROR eColourSensorGetAmbient(unsigned char ucId, unsigned char *pucValue)
{
    unsigned char pucTxParam[2];

    // Control table address.
    pucTxParam[0] = CL_ADD_AMBIENT;
    
    // Number of bytes to read.
    pucTxParam[1] = 1;

    // Send the packet and return the error code.
    return eEMSendReceivePacket(ucId, EM_INST_READ_DATA, sizeof(pucTxParam), pucTxParam, pucValue);
}



/*******************************************************************************
 * FUNCTION: eColourSensorGetColour
 *
 * PARAMETERS:
 * ~ ucId           - Sensor ID.
 * ~ peColour       - Buffer to return the detected colour.
 *
 * RETURN:
 * ~ Error code.
 *
 * DESCRIPTIONS:
 * Get the detected colour from colour sensor.
 *
 *******************************************************************************/
EM_ERROR eColourSensorGetColour(unsigned char ucId, CL_SENSOR_COLOUR *peColour)
{
    // Reset the colour.
    *peColour = CL_BLACK;
    
    // Get the RGB value from the sensor.
    unsigned char ucRed;
    unsigned char ucGreen;
    unsigned char ucBlue;
    EM_ERROR eErrorCode = eColourSensorGetRgb(ucId, &ucRed, &ucGreen, &ucBlue);
    
    // Make sure there is no error.
    if (eErrorCode == EM_NO_ERROR) {
        // Get the maximum value in 3 colours.
        unsigned char ucMax = 0;
        if (ucMax < ucRed) {
            ucMax = ucRed;
        }
        if (ucMax < ucGreen) {
            ucMax = ucGreen;
        }
        if (ucMax < ucBlue) {
            ucMax = ucBlue;
        }


        // Make sure the colour is detected.
        if (ucMax > CL_SENSOR_THRESHOLD) {
            // Calculate the threshold.
            unsigned char ucThreshold = (unsigned char)((unsigned int)ucMax * 3 / 5);

            // Determine the colour.
            if (ucRed > ucThreshold) {
                if (ucGreen > ucThreshold) {
                    if (ucBlue > ucThreshold) {
                        *peColour = CL_WHITE;
                    } else {
                        *peColour = CL_YELLOW;
                    }
                } else if (ucBlue > ucThreshold) {
                    *peColour = CL_MAGENTA;
                } else {
                    *peColour = CL_RED;
                }
            }
            else if (ucGreen > ucThreshold) {
                if (ucBlue > ucThreshold) {
                    *peColour = CL_CYAN;
                } else {
                    *peColour = CL_GREEN;
                }
            }
            else if (ucBlue > ucThreshold) {
                *peColour = CL_BLUE;
            }
        }
    }
    
    // Return the error code.
    return eErrorCode;
}



/*******************************************************************************
 * FUNCTION: eAnalogInputModuleGetValue
 *
 * PARAMETERS:
 * ~ ucId           - Sensor ID.
 * ~ ucChannel      - Input channel.
 * ~ pucValue       - Buffer to return the value.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Get the analog input module value.
 *
 *******************************************************************************/
EM_ERROR eAnalogInputModuleGetValue(unsigned char ucId, unsigned char ucChannel, unsigned char *pucValue)
{
    unsigned char pucTxParam[2];

    // Control table address.
    switch (ucChannel) {
        case 1: pucTxParam[0] = AI_ADD_AN_S1; break;
        case 2: pucTxParam[0] = AI_ADD_AN_S2; break;
        case 3: pucTxParam[0] = AI_ADD_AN_S3; break;
        case 4: pucTxParam[0] = AI_ADD_AN_S4; break;
        
        default:
            return EM_ERR_INSTRUCTION;
    }
    
    
    // Number of bytes to read.
    pucTxParam[1] = 1;

    // Send the packet and return the error code.
    return eEMSendReceivePacket(ucId, EM_INST_READ_DATA, sizeof(pucTxParam), pucTxParam, pucValue);
}



/*******************************************************************************
 * FUNCTION: eRgbLightModuleSetRgb
 *
 * PARAMETERS:
 * ~ ucId           - Sensor ID.
 * ~ ucRed          - Red value for the RGB LED.
 * ~ ucGreen        - Green value for the RGB LED.
 * ~ ucBlue         - Blue value for the RGB LED.
 *
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Set the LED colour of the RGB Light Module.
 *
 *******************************************************************************/
EM_ERROR eRgbLightModuleSetRgb(unsigned char ucId, unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue)
{
    unsigned char pucTxParam[4];

    // Set the RGB value and left-right LED control value.
    pucTxParam[0] = RL_ADD_RED;         // Control table address for Red value.
    pucTxParam[1] = ucRed;              // Red value.
    pucTxParam[2] = ucGreen;            // Green value.
    pucTxParam[3] = ucBlue;             // Blue value.
    
    return eEMSendReceivePacket(ucId, EM_INST_WRITE_DATA, sizeof(pucTxParam), pucTxParam, NULL);
}



/*******************************************************************************
 * FUNCTION: eRgbLightModuleGetRgb
 *
 * PARAMETERS:
 * ~ ucId           - Sensor ID.
 * ~ pucRed         - Buffer to return the red value.
 * ~ pucGreen       - Buffer to return the green value.
 * ~ pucBlue        - Buffer to return the blue value.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Get the RGB value from RGB Light Module.
 *
 *******************************************************************************/
EM_ERROR eRgbLightModuleGetRgb(unsigned char ucId, unsigned char *pucRed, unsigned char *pucGreen, unsigned char *pucBlue)
{
    unsigned char pucTxParam[2];
    unsigned char pucRxBuffer[3];

    // Control table address.
    pucTxParam[0] = RL_ADD_RED;
    
    // Number of bytes to read.
    pucTxParam[1] = 3;

    // Send the packet and 
    EM_ERROR eErrorCode = eEMSendReceivePacket(ucId, EM_INST_READ_DATA, sizeof(pucTxParam), pucTxParam, pucRxBuffer);
    
    // Save the RGB value.
    *pucRed = pucRxBuffer[0];
    *pucGreen = pucRxBuffer[1];
    *pucBlue = pucRxBuffer[2];
    
    // Return the error code.
    return eErrorCode;
}



///*******************************************************************************
// * FUNCTION: eRgbLightModuleSetBrightness
// *
// * PARAMETERS:
// * ~ ucId           - Sensor ID.
// * ~ ucValue        - Brightness value.
// *
// * RETURN:
// * ~ Error Code.
// *
// * DESCRIPTIONS:
// * Set the brightness value of RGB Light Module.
// *
// *******************************************************************************/
//EM_ERROR eRgbLightModuleSetBrightness(unsigned char ucId, unsigned char ucValue)
//{
//    unsigned char pucTxParam[2];
//
//    // Control table address.
//    pucTxParam[0] = LM_ADD_BRIGHTNESS;
//
//    // Value.
//    pucTxParam[1] = ucValue;
//
//    return eEMSendReceivePacket(ucId, EM_INST_WRITE_DATA, sizeof(pucTxParam), pucTxParam, NULL);
//}
//
//
//
///*******************************************************************************
// * FUNCTION: eRgbLightModuleGetBrightness
// *
// * PARAMETERS:
// * ~ ucId           - Sensor ID.
// * ~ pucValue       - Buffer to return the value.
// *
// * RETURN:
// * ~ Error Code.
// *
// * DESCRIPTIONS:
// * Get the brightness value from RGB Light Module.
// *
// *******************************************************************************/
//EM_ERROR eRgbLightModuleGetBrightness(unsigned char ucId, unsigned char *pucValue)
//{
//    unsigned char pucTxParam[2];
//
//    // Control table address.
//    pucTxParam[0] = LM_ADD_BRIGHTNESS;
//    
//    // Number of bytes to read.
//    pucTxParam[1] = 1;
//
//    // Send the packet and return the error code.
//    return eEMSendReceivePacket(ucId, EM_INST_READ_DATA, sizeof(pucTxParam), pucTxParam, pucValue);
//}


