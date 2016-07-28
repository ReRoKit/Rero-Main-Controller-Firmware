/*******************************************************************************
 * This file provides the functions to communicate with the G15.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "G15 and Sensors/G15.h"
#include "TimeDelay.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

// G15 Control Table Address.
typedef enum __attribute__((packed)) {
    G15_ADD_CW_ANGLE_LIMIT_L        = 0x06,
    G15_ADD_CW_ANGLE_LIMIT_H        = 0x07,
    G15_ADD_CCW_ANGLE_LIMIT_L       = 0x08,
    G15_ADD_CCW_ANGLE_LIMIT_H       = 0x09,
    G15_ADD_RESERVED1               = 0x0a,
    G15_ADD_LIMIT_TEMPERATURE       = 0x0b,
    G15_ADD_DOWN_LIMIT_VOLTAGE      = 0x0c,
    G15_ADD_UP_LIMIT_VOLTAGE        = 0x0d,
    G15_ADD_MAX_TORQUE_L            = 0x0e,
    G15_ADD_MAX_TORQUE_H            = 0x0f,
    G15_ADD_STATUS_RETURN_LEVEL     = 0x10,
    G15_ADD_ALARM_LED               = 0x11,
    G15_ADD_ALARM_SHUTDOWN          = 0x12,
    G15_ADD_RESERVED2               = 0x13,
    G15_ADD_DOWN_CALIBRATION_L      = 0x14,
    G15_ADD_DOWN_CALIBRATION_H      = 0x15,
    G15_ADD_UP_CALIBRATION_L        = 0x16,
    G15_ADD_UP_CALIBRATION_H        = 0x17,

    G15_ADD_TORQUE_ENABLE           = 0x18,
    G15_ADD_LED                     = 0x19,
    G15_ADD_CW_COMPLIANCE_MARGIN    = 0x1a,
    G15_ADD_CCW_COMPLIANCE_MARGIN   = 0x1b,
    G15_ADD_CW_COMPLIANCE_SLOPE     = 0x1c,
    G15_ADD_CCW_COMPLIANCE_SLOPE    = 0x1d,
    G15_ADD_GOAL_POSITION_L         = 0x1e,
    G15_ADD_GOAL_POSITION_H         = 0x1f,
    G15_ADD_MOVING_SPEED_L          = 0x20,
    G15_ADD_MOVING_SPEED_H          = 0x21,
    G15_ADD_TORQUE_LIMIT_L          = 0x22,
    G15_ADD_TORQUE_LIMIT_H          = 0x23,
    G15_ADD_PRESENT_POSITION_L      = 0x24,
    G15_ADD_PRESENT_POSITION_H      = 0x25,
    G15_ADD_PRESENT_SPEED_L         = 0x26,
    G15_ADD_PRESENT_SPEED_H         = 0x27,
    G15_ADD_PRESENT_LOAD_L          = 0x28,
    G15_ADD_PRESENT_LOAD_H          = 0x29,
    G15_ADD_PRESENT_VOLTAGE         = 0x2a,
    G15_ADD_PRESENT_TEMPERATURE     = 0x2b,
    G15_ADD_REGISTERED_INSTRUCTION  = 0x2c,
    G15_ADD_RESERVE3                = 0x2d,
    G15_ADD_MOVING                  = 0x2e,
    G15_ADD_LOCK                    = 0x2f,
    G15_ADD_PUNCH_L                 = 0x30,
    G15_ADD_PUNCH_H                 = 0x31,
} G15_CT_ADD;



/*******************************************************************************
 * FUNCTION: eG15SetPositionSpeed
 *
 * PARAMETERS:
 * ~ ucId               - G15 ID.
 * ~ eWriteMode         - Write immediately or wait for action command.
 * ~ usPosition         - Goal position (0 - 1087).
 * ~ ePositioningMode   - Positioning mode.
 * ~ usSpeed            - Goal speed.
 *                        Position mode speed control: 1 to 1023, 0 for maximum.
 *                        Position mode time control:  0 to 4095.
 *                        Wheel mode: 0 to 1023.
 * ~ eSpeedControlMode  - Speed control mode.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Set the goal position and speed.
 *
 *******************************************************************************/
EM_ERROR eG15SetPositionSpeed(unsigned char ucId, EM_WRITE_MODE eWriteMode,
                                 unsigned short usPosition, G15_POSITIONING_MODE ePositioningMode,
                                 unsigned short usSpeed, G15_SPEED_CONTROL_MODE eSpeedControlMode)
{
    unsigned char pucTxParam[5];

    // Control table address.
    pucTxParam[0] = (unsigned char)G15_ADD_GOAL_POSITION_L;

    // Goal position.
    usPosition |= (unsigned short)ePositioningMode;
    pucTxParam[1] = (unsigned char)(usPosition & 0x00ff);
    pucTxParam[2] = (unsigned char)(usPosition >> 8);

    // Goal speed.
    usSpeed |= (unsigned short)eSpeedControlMode;
    pucTxParam[3] = (unsigned char)(usSpeed & 0x00ff);
    pucTxParam[4] = (unsigned char)(usSpeed >> 8);

    // Send the packet and return the error code.
    EM_INSTRUCTION eInstruction = (eWriteMode == WRITE_NOW)? EM_INST_WRITE_DATA : EM_INST_REG_WRITE;
    return eEMSendReceivePacket(ucId, eInstruction, sizeof(pucTxParam), pucTxParam, NULL);
}



/*******************************************************************************
 * FUNCTION: eG15SetPosition
 *
 * PARAMETERS:
 * ~ ucId               - G15 ID.
 * ~ eWriteMode         - Write immediately or wait for action command.
 * ~ usPosition         - Goal position (0 - 1087).
 * ~ ePositioningMode   - Positioning mode.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Set the goal position.
 *
 *******************************************************************************/
EM_ERROR eG15SetPosition(unsigned char ucId, EM_WRITE_MODE eWriteMode,
                           unsigned short usPosition, G15_POSITIONING_MODE ePositioningMode)
{
    unsigned char pucTxParam[3];

    // Control table address.
    pucTxParam[0] = (unsigned char)G15_ADD_GOAL_POSITION_L;

    // Goal position.
    usPosition |= (unsigned short)ePositioningMode;
    pucTxParam[1] = (unsigned char)(usPosition & 0x00ff);
    pucTxParam[2] = (unsigned char)(usPosition >> 8);

    // Send the packet and return the error code.
    EM_INSTRUCTION eInstruction = (eWriteMode == WRITE_NOW)? EM_INST_WRITE_DATA : EM_INST_REG_WRITE;
    return eEMSendReceivePacket(ucId, eInstruction, sizeof(pucTxParam), pucTxParam, NULL);
}



/*******************************************************************************
 * FUNCTION: eG15GetPosition
 *
 * PARAMETERS:
 * ~ ucId               - G15 ID.
 * ~ pusPosition        - Buffer to return the current position.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Get the current position (0 - 1087).
 *
 *******************************************************************************/
EM_ERROR eG15GetPosition(unsigned char ucId, unsigned short *pusPosition)
{
    unsigned char pucTxParam[2];

    // Control table address.
    pucTxParam[0] = (unsigned char)G15_ADD_PRESENT_POSITION_L;

    // Number of bytes to read.
    pucTxParam[1] = 2;

    // Send the packet and return the error code.
    return eEMSendReceivePacket(ucId, EM_INST_READ_DATA, sizeof(pucTxParam), pucTxParam, (unsigned char*)pusPosition);
}



/*******************************************************************************
 * FUNCTION: eG15SetSpeed
 *
 * PARAMETERS:
 * ~ ucId               - G15 ID.
 * ~ eWriteMode         - Write immediately or wait for action command.
 * ~ usSpeed            - Goal speed.
 *                        Position mode speed control: 1 to 1023, 0 for maximum.
 *                        Position mode time control:  0 to 4095.
 *                        Wheel mode: 0 to 1023.
 * ~ eSpeedControlMode  - Speed control mode.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Set the goal speed.
 *
 *******************************************************************************/
EM_ERROR eG15SetSpeed(unsigned char ucId, EM_WRITE_MODE eWriteMode,
                        unsigned short usSpeed, G15_SPEED_CONTROL_MODE eSpeedControlMode)
{
    unsigned char pucTxParam[3];

    // Control table address.
    pucTxParam[0] = (unsigned char)G15_ADD_MOVING_SPEED_L;

    // Goal speed.
    usSpeed |= (unsigned short)eSpeedControlMode;
    pucTxParam[1] = (unsigned char)(usSpeed & 0x00ff);
    pucTxParam[2] = (unsigned char)(usSpeed >> 8);

    // Send the packet and return the error code.
    EM_INSTRUCTION eInstruction = (eWriteMode == WRITE_NOW)? EM_INST_WRITE_DATA : EM_INST_REG_WRITE;
    return eEMSendReceivePacket(ucId, eInstruction, sizeof(pucTxParam), pucTxParam, NULL);
}



/*******************************************************************************
 * FUNCTION: eG15SetTorqueLed
 *
 * PARAMETERS:
 * ~ ucId           - G15 ID.
 * ~ eWriteMode     - Write immediately or wait for action command.
 * ~ ucTorque       - Torque (0 = Off, 1 = On).
 * ~ ucLed          - LED (0 = Off, 1 = On).
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Set the state of torque and LED.
 *
 *******************************************************************************/
EM_ERROR eG15SetTorqueLed(unsigned char ucId, EM_WRITE_MODE eWriteMode, unsigned char ucTorque, unsigned char ucLed)
{
    unsigned char pucTxParam[3];

    // Control table address.
    pucTxParam[0] = (unsigned char)G15_ADD_TORQUE_ENABLE;

    // Torque.
    pucTxParam[1] = ucTorque;

    // LED.
    pucTxParam[2] = ucLed;

    // Send the packet and return the error code.
    EM_INSTRUCTION eInstruction = (eWriteMode == WRITE_NOW)? EM_INST_WRITE_DATA : EM_INST_REG_WRITE;
    return eEMSendReceivePacket(ucId, eInstruction, sizeof(pucTxParam), pucTxParam, NULL);
}



/*******************************************************************************
 * FUNCTION: eG15SetTorque
 *
 * PARAMETERS:
 * ~ ucId           - G15 ID.
 * ~ eWriteMode     - Write immediately or wait for action command.
 * ~ ucTorque       - Torque (0 = Off, 1 = On).
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Set the state of torque.
 *
 *******************************************************************************/
EM_ERROR eG15SetTorque(unsigned char ucId, EM_WRITE_MODE eWriteMode, unsigned char ucTorque)
{
    unsigned char pucTxParam[2];

    // Control table address.
    pucTxParam[0] = (unsigned char)G15_ADD_TORQUE_ENABLE;

    // Torque.
    pucTxParam[1] = ucTorque;

    // Send the packet and return the error code.
    EM_INSTRUCTION eInstruction = (eWriteMode == WRITE_NOW)? EM_INST_WRITE_DATA : EM_INST_REG_WRITE;
    return eEMSendReceivePacket(ucId, eInstruction, sizeof(pucTxParam), pucTxParam, NULL);
}



/*******************************************************************************
 * FUNCTION: eG15SetLed
 *
 * PARAMETERS:
 * ~ ucId           - G15 ID.
 * ~ eWriteMode     - Write immediately or wait for action command.
 * ~ ucLed          - LED (0 = Off, 1 = On).
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Set the state of LED.
 *
 *******************************************************************************/
EM_ERROR eG15SetLed(unsigned char ucId, EM_WRITE_MODE eWriteMode, unsigned char ucLed)
{
    unsigned char pucTxParam[2];

    // Control table address.
    pucTxParam[0] = (unsigned char)G15_ADD_LED;

    // LED.
    pucTxParam[1] = ucLed;

    // Send the packet and return the error code.
    EM_INSTRUCTION eInstruction = (eWriteMode == WRITE_NOW)? EM_INST_WRITE_DATA : EM_INST_REG_WRITE;
    return eEMSendReceivePacket(ucId, eInstruction, sizeof(pucTxParam), pucTxParam, NULL);
}



/*******************************************************************************
 * FUNCTION: eG15SetAngleLimit
 *
 * PARAMETERS:
 * ~ ucId           - G15 ID.
 * ~ usLimitCw      - CW Angle Limit (0 - 1087).
 * ~ usLimitCcw     - CCW Angle Limit (0 - 1087).
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Set the CW and CCW angle limit.
 *
 *******************************************************************************/
EM_ERROR eG15SetAngleLimit(unsigned char ucId, unsigned short usLimitCw, unsigned short usLimitCcw)
{
    unsigned char pucTxParam[5];

    // Control table address.
    pucTxParam[0] = (unsigned char)G15_ADD_CW_ANGLE_LIMIT_L;

    // CW Angle Limit.
    pucTxParam[1] = (unsigned char)(usLimitCw & 0x00ff);
    pucTxParam[2] = (unsigned char)(usLimitCw >> 8);

    // CCW Angle Limit.
    pucTxParam[3] = (unsigned char)(usLimitCcw & 0x00ff);
    pucTxParam[4] = (unsigned char)(usLimitCcw >> 8);

    // Send the packet and return the error code.
    EM_ERROR eErrorCode = eEMSendReceivePacket(ucId, EM_INST_WRITE_DATA, sizeof(pucTxParam), pucTxParam, NULL);

    // Delay a while before returning the error code to let the G15 write to EEPROM.
    DelayMs(10);
    return eErrorCode;
}



/*******************************************************************************
 * FUNCTION: eG15GetAngleLimit
 *
 * PARAMETERS:
 * ~ ucId               - G15 ID.
 * ~ pusLimitCw         - Buffer to return the CW angle limit.
 * ~ pusLimitCCw        - Buffer to return the CCW angle limit.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Get the CW and CCW angle limit (0 - 1087).
 *
 *******************************************************************************/
EM_ERROR eG15GetAngleLimit(unsigned char ucId, unsigned short *pusLimitCw, unsigned short *pusLimitCcw)
{
    unsigned char pucTxParam[2];
    unsigned char pucRxBuffer[4];

    // Control table address.
    pucTxParam[0] = (unsigned char)G15_ADD_CW_ANGLE_LIMIT_L;

    // Number of bytes to read.
    pucTxParam[1] = 4;

    // Send the packet.
    EM_ERROR eErrorCode =  eEMSendReceivePacket(ucId, EM_INST_READ_DATA, sizeof(pucTxParam), pucTxParam, pucRxBuffer);

    // Get the angle limit.
    *pusLimitCw = (unsigned short)pucRxBuffer[0] + (unsigned short)(pucRxBuffer[1] << 8);
    *pusLimitCcw = (unsigned short)pucRxBuffer[2] + (unsigned short)(pucRxBuffer[3] << 8);

    // Return the error code.
    return eErrorCode;
}



/*******************************************************************************
 * FUNCTION: eG15SetComplianceSlope
 *
 * PARAMETERS:
 * ~ ucId                   - G15 ID.
 * ~ ucComplianceSlopeCw    - CW Compliance Slope (1 - 254).
 * ~ ucComplianceSlopeCcw   - CCW Compliance Slope (1 - 254).
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Set the CW and CCW Compliance Slope.
 *
 *******************************************************************************/
EM_ERROR eG15SetComplianceSlope(unsigned char ucId, unsigned char ucComplianceSlopeCw, unsigned char ucComplianceSlopeCcw)
{
    unsigned char pucTxParam[3];

    // Control table address.
    pucTxParam[0] = (unsigned char)G15_ADD_CW_COMPLIANCE_SLOPE;

    // CW Compliance Slope.
    pucTxParam[1] = ucComplianceSlopeCw;

    // CCW Compliance Slope.
    pucTxParam[2] = ucComplianceSlopeCcw;

    // Send the packet and return the error code.
    return eEMSendReceivePacket(ucId, EM_INST_WRITE_DATA, sizeof(pucTxParam), pucTxParam, NULL);
}



/*******************************************************************************
 * FUNCTION: eG15SetTorqueLimit
 *
 * PARAMETERS:
 * ~ ucId               - G15 ID.
 * ~ usTorqueLimit      - Torque Limit (0 - 1023).
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Set the torque limit.
 *
 *******************************************************************************/
EM_ERROR eG15SetTorqueLimit(unsigned char ucId, unsigned short usTorqueLimit)
{
    unsigned char pucTxParam[3];

    // Control table address.
    pucTxParam[0] = (unsigned char)G15_ADD_TORQUE_LIMIT_L;

    // Torque Limit
    pucTxParam[1] = (unsigned char)(usTorqueLimit & 0x00ff);
    pucTxParam[2] = (unsigned char)(usTorqueLimit >> 8);

    // Send the packet and return the error code.
    EM_ERROR eErrorCode = eEMSendReceivePacket(ucId, EM_INST_WRITE_DATA, sizeof(pucTxParam), pucTxParam, NULL);

    // Delay a while before returning the error code to let the G15 write to EEPROM.
    DelayMs(10);
    return eErrorCode;
}



/*******************************************************************************
 * FUNCTION: eG15ResetRegister
 *
 * PARAMETERS:
 * ~ ucId   - G15 ID.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Reset the G15 control table to default value except the ID.
 *
 *******************************************************************************/
EM_ERROR eG15ResetRegister(unsigned char ucId)
{
    EM_ERROR eErrorCode = EM_NO_ERROR;
    unsigned char pucTxParam[9];

    // Control table address.
    pucTxParam[0] = (unsigned char)EM_ADD_BAUD_RATE;

    // Register value.
    pucTxParam[1] = 0x67;
    pucTxParam[2] = 0xfa;
    pucTxParam[3] = 0x00;
    pucTxParam[4] = 0x00;
    pucTxParam[5] = 0x3f;
    pucTxParam[6] = 0x04;

    // Send the packet and return the error code.
    eErrorCode |= eEMSendReceivePacket(ucId, EM_INST_WRITE_DATA, 7, pucTxParam, NULL);

    // Delay a while to let the G15 write to its EEPROM.
    DelayMs(200);



    // Control table address.
    pucTxParam[0] = (unsigned char)G15_ADD_LIMIT_TEMPERATURE;

    // Register value.
    pucTxParam[1] = 0x46;
    pucTxParam[2] = 0x41;
    pucTxParam[3] = 0x96;
    pucTxParam[4] = 0xff;
    pucTxParam[5] = 0x03;
    pucTxParam[6] = 0x02;
    pucTxParam[7] = 0x24;
    pucTxParam[8] = 0x24;

    // Send the packet and return the error code.
    eErrorCode |= eEMSendReceivePacket(ucId, EM_INST_WRITE_DATA, 9, pucTxParam, NULL);

    // Delay a while to let the G15 write to its EEPROM.
    DelayMs(200);



    // Control table address.
    pucTxParam[0] = (unsigned char)G15_ADD_TORQUE_ENABLE;

    // Register value.
    pucTxParam[1] = 0x00;
    pucTxParam[2] = 0x00;
    pucTxParam[3] = 0x01;
    pucTxParam[4] = 0x01;
    pucTxParam[5] = 0x20;
    pucTxParam[6] = 0x20;

    // Send the packet and return the error code.
    eErrorCode |= eEMSendReceivePacket(ucId, EM_INST_WRITE_DATA, 7, pucTxParam, NULL);

    // Delay a while to let the G15 process the data.
    DelayMs(100);



    // Control table address.
    pucTxParam[0] = (unsigned char)G15_ADD_MOVING_SPEED_L;

    // Register value.
    pucTxParam[1] = 0x00;
    pucTxParam[2] = 0x00;
    pucTxParam[3] = 0xff;
    pucTxParam[4] = 0x03;

    // Send the packet and return the error code.
    eErrorCode |= eEMSendReceivePacket(ucId, EM_INST_WRITE_DATA, 5, pucTxParam, NULL);

    // Delay a while to let the G15 process the data.
    DelayMs(100);



    // Control table address.
    pucTxParam[0] = (unsigned char)G15_ADD_LOCK;

    // Register value.
    pucTxParam[1] = 0x00;
    pucTxParam[2] = 0x20;
    pucTxParam[3] = 0x00;

    // Send the packet and return the error code.
    eErrorCode |= eEMSendReceivePacket(ucId, EM_INST_WRITE_DATA, 4, pucTxParam, NULL);

    // Delay a while to let the G15 process the data.
    DelayMs(100);


    
    // Delay a while before returning the error code to let the G15 write to EEPROM.
    DelayMs(1000);
    return eErrorCode;
}
