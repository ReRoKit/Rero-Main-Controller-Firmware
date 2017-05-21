/*******************************************************************************
 * Common functions for the external module such as G15 and sensors.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "G15 and Sensors/ExtModule.h"
#include "G15 and Sensors/uart5.h"
#include "G15 and Sensors/G15.h"
#include "G15 and Sensors/Sensors.h"
#include "HardwareProfile.h"
#include "Variables.h"
#include "TimeDelay.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

//// G15 respond timeout (1000us).
//// Baud Rate = 250K, receive a byte needs 40us.
//// First byte should arrive after ~600us.
//// The value is in multiple of 10us.
//#define G15_RESPOND_TIMEOUT 100

// G15 respond timeout (1100us).
// Baud Rate = 115200, receive a byte needs 87us.
// First byte should arrive after ~600us.
// The value is in multiple of 10us.
#define G15_RESPOND_TIMEOUT 110

//// G15 respond timeout (1500us).
//// Baud Rate = 19200, receive a byte needs 520us.
//// First byte should arrive after ~600us.
//// The value is in multiple of 10us.
//#define G15_RESPOND_TIMEOUT 150

// Minimum packet length (No parameter).
#define MIN_PACKET_LENGHT   6



/*******************************************************************************
 * FUNCTION: vEMInitWorkaround
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Initialization workaround for external module.
 * This is workaround for bugs in the firmware of the external module.
 * 1. Cycle the G15 torque enable flag. This is to fix the bug where the
 *    wheel mode flag is not initialized properly.
 * 2. Trigger the ping command for the ultrasonic sensor. Or else, the reading
 *    for the first measurement will be incorrect.
 * 3. For showcase robot, change the compliance slope to 0x28.
 *
 *******************************************************************************/
void vEMInitWorkaround(void)
{
    unsigned char ucId;
    EM_MODEL eModel;
    unsigned char pucConnectedServo[EM_MAX_ID + 1] = {0};

    // Loop through all valid ID.
    for (ucId = 0; ucId <= EM_MAX_ID; ucId++) {
        EM_ERROR eErrorCode = eEMGetModel(ucId, &eModel);
        if (eErrorCode == EM_NO_ERROR) {
            // Enable the torque if it's G15.
            if (eModel == EM_MODEL_G15) {
                eG15SetTorque(ucId, WRITE_NOW, 1);
                
#ifdef SHOWCASE_ROBOT
//                eG15SetComplianceSlope(ucId, 0x28, 0x28);
#endif

                // Record the connected servo.
                pucConnectedServo[ucId] = 1;
            }

            // Read from the ultrasonic sensor for the first time.
            else if (eModel == EM_MODEL_US) {
                unsigned short usSensorValue;
                eUltrasonicSensorGetValue(ucId, &usSensorValue);
            }
        }
    }

    DelayMs(10);

    // Loop through all connected servo and disable the torque.
    for (ucId = 0; ucId <= EM_MAX_ID; ucId++) {
        if (pucConnectedServo[ucId] == 1) {
            eG15SetTorque(ucId, WRITE_NOW, 0);
        }
    }
}



/*******************************************************************************
 * FUNCTION: eEMBoostBaudrate
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Send a command to change the default baudrate for external module from
 * 19200 to 115200.
 *
 *******************************************************************************/
EM_ERROR eEMBoostBaudrate(void)
{
    unsigned char pucTxParam[2];


    // The command is to be sent in original baudrate.
    vUART5SetBaudrate(EXT_UART_ORI_BAUDRATE);


    // Control table address.
    pucTxParam[0] = (unsigned char)EM_ADD_BAUD_RATE;

    // New baud rate.
    pucTxParam[1] = (unsigned char)(2000000 / EXT_UART_BOOSTED_BAUDRATE) - 1;

    // Send the packet.
    EM_ERROR eErrorCode = eEMSendReceivePacket(EM_BROADCAST_ID, EM_INST_WRITE_DATA, sizeof(pucTxParam), pucTxParam, NULL);


    // Reset to new Baudrate.
    vUART5SetBaudrate(EXT_UART_BOOSTED_BAUDRATE);
    DelayMs(10);

    // Reset the external module by power cycling them.
    POWER_SUB = 0;
    DelayMs(100);
    POWER_SUB = 1;
    DelayMs(700);

    // Clear the UART error if there is any.
    xSystemError.bUartError = 0;

    // Return the error code.
    return eErrorCode;
}



/*******************************************************************************
 * FUNCTION: eEMOriBaudrate
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Send a command to change the default baudrate for external module from
 * 115200 to 19200.
 *
 *******************************************************************************/
EM_ERROR eEMOriBaudrate(void)
{
    unsigned char pucTxParam[2];


    // The command is to be sent in original baudrate.
    vUART5SetBaudrate(EXT_UART_BOOSTED_BAUDRATE);


    // Control table address.
    pucTxParam[0] = (unsigned char)EM_ADD_BAUD_RATE;

    // New baud rate.
    pucTxParam[1] = (unsigned char)(2000000 / EXT_UART_ORI_BAUDRATE) - 1;

    // Send the packet.
    EM_ERROR eErrorCode = eEMSendReceivePacket(EM_BROADCAST_ID, EM_INST_WRITE_DATA, sizeof(pucTxParam), pucTxParam, NULL);


    // Reset to new Baudrate.
    vUART5SetBaudrate(EXT_UART_ORI_BAUDRATE);
    DelayMs(10);

    // Reset the external module by power cycling them.
    POWER_SUB = 0;
    DelayMs(100);
    POWER_SUB = 1;
    DelayMs(700);

    // Clear the UART error if there is any.
    xSystemError.bUartError = 0;

    // Return the error code.
    return eErrorCode;
}



/*******************************************************************************
 * FUNCTION: vEMDisableAllOutput
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Turn off torque and LED for all servos.
 *
 *******************************************************************************/
void vEMDisableAllOutput(void)
{
    unsigned char i;
    for (i = 0; i < EM_MAX_ID; i++) {
        // Get the model of the module.
        // Make sure there is respond from the module.
        EM_MODEL eModel;
        if (eEMGetModel(i, &eModel) == EM_NO_ERROR) {
            if (eModel == EM_MODEL_G15) {
                // Turn off torque and LED, then set the speed to 0.
                eG15SetTorqueLed(i, WRITE_NOW, 0, 0);
                eG15SetSpeed(i, WRITE_NOW, 0, POSITION_SPEED_CONTROL);
            }
			else if (eModel == EM_MODEL_RGBLIGHT) {
                // Turn off RGB LED for Light module.
                eRgbLightModuleSetRgb(i, 0, 0, 0);
            }
        }
    }
}



/*******************************************************************************
 * FUNCTION: eEMPing
 *
 * PARAMETERS:
 * ~ ucId           - External Module ID.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Send the ping command.
 *
 *******************************************************************************/
EM_ERROR eEMPing(unsigned char ucId)
{
    // Send the packet and return the error code.
    return eEMSendReceivePacket(ucId, EM_INST_PING, 0, NULL, NULL);
}



/*******************************************************************************
 * FUNCTION: eEMGetModel
 *
 * PARAMETERS:
 * ~ ucId           - External Module ID.
 * ~ peModel        - Buffer to return the model.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Get the external module model.
 *
 *******************************************************************************/
EM_ERROR eEMGetModel(unsigned char ucId, EM_MODEL *peModel)
{
    unsigned char pucTxParam[2];

    // Control table address.
    pucTxParam[0] = EM_ADD_MODEL_L;

    // Number of bytes to read.
    pucTxParam[1] = 2;

    // Send the packet and return the error code.
    return eEMSendReceivePacket(ucId, EM_INST_READ_DATA, sizeof(pucTxParam), pucTxParam, (unsigned char*)peModel);
}



/*******************************************************************************
 * FUNCTION: eEMGetFirmwareRevision
 *
 * PARAMETERS:
 * ~ ucId                   - External Module ID.
 * ~ pucFirmwareRevision    - Buffer to return the firmware revision (1 byte).
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Get the external module firmware revision.
 *
 *******************************************************************************/
EM_ERROR eEMGetFirmwareRevision(unsigned char ucId, unsigned char *pucFirmwareRevision)
{
    unsigned char pucTxParam[2];

    // Control table address.
    pucTxParam[0] = EM_ADD_VERSION;

    // Number of bytes to read.
    pucTxParam[1] = 1;

    // Send the packet and return the error code.
    return eEMSendReceivePacket(ucId, EM_INST_READ_DATA, sizeof(pucTxParam), pucTxParam, (unsigned char*)pucFirmwareRevision);
}



/*******************************************************************************
 * FUNCTION: eEMSetId
 *
 * PARAMETERS:
 * ~ ucCurrentId    - External module current ID.
 * ~ ucNewId        - External module new ID.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Set the external module ID.
 *
 *******************************************************************************/
EM_ERROR eEMSetId(unsigned char ucCurrentId, unsigned char ucNewId)
{
    unsigned char pucTxParam[2];

    // Control table address.
    pucTxParam[0] = (unsigned char)EM_ADD_ID;

    // New ID.
    pucTxParam[1] = ucNewId;

    // Send the packet and return the error code.
    EM_ERROR eErrorCode = eEMSendReceivePacket(ucCurrentId, EM_INST_WRITE_DATA, sizeof(pucTxParam), pucTxParam, NULL);

    // Delay a while before returning the error code to let the G15 write to EEPROM.
    DelayMs(10);
    return eErrorCode;
}



/*******************************************************************************
 * FUNCTION: eEMSetAction
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Send the set action command.
 *
 *******************************************************************************/
EM_ERROR eEMSetAction(void)
{
    // Send the packet and return the error code.
    return eEMSendReceivePacket(EM_BROADCAST_ID, EM_INST_ACTION, 0, NULL, NULL);
}



/*******************************************************************************
 * FUNCTION: eEMFactoryReset
 *
 * PARAMETERS:
 * ~ ucId   - External module ID.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Reset the external module control table to default value.
 *
 *******************************************************************************/
EM_ERROR eEMFactoryReset(unsigned char ucId)
{
    // Send the packet and return the error code.
    EM_ERROR eErrorCode = eEMSendReceivePacket(ucId, EM_INST_RESET, 0, NULL, NULL);

    // Delay a while before returning the error code to let the G15 write to EEPROM.
    DelayMs(1000);
    return eErrorCode;
}



/*******************************************************************************
 * FUNCTION: eEMSendReceivePacket
 *
 * PARAMETERS:
 * ~ ucId           - G15 ID.
 * ~ eInstruction   - G15 Instruction.
 * ~ ucParamLength  - Parameter length.
 * ~ pucTxData      - Pointer to Tx parameter.
 * ~ pucRxData      - Pointer to Rx data.
 *                    Only used for instruction READ_DATA.
 *                    For other instruction, put NULL.
 *
 * RETURN:
 * ~ Error Code.
 *
 * DESCRIPTIONS:
 * Send a packet to the G15 and receive the packet responded by G15.
 *
 *******************************************************************************/
EM_ERROR eEMSendReceivePacket(unsigned char ucId, EM_INSTRUCTION eInstruction, unsigned char ucParamLength,
                                 unsigned char *pucTxParam, unsigned char *pucRxData)
{
    unsigned char i;

    // Return error code.
    EM_ERROR eErrorCode = EM_NO_ERROR;

    // Buffer for the Tx data excluding the parameters and checksum (0xFF, 0xFF, ID, Length, Instruction).
    // This is also used to store the returned status packet (0xFF, 0xFF, ID, Length, Error, Checksum).
    unsigned char pucBuffer[MIN_PACKET_LENGHT];


    // Take control on external UART.
    if (xSystemState.bRtosRunning == 1) {
        xSemaphoreTake(xExternalUartMutex, portMAX_DELAY);
    }


    // Switch to Tx mode.
    vTxMode();

    // Prepare the data.
    pucBuffer[0] = 0xff;
    pucBuffer[1] = 0xff;
    pucBuffer[2] = ucId;
    pucBuffer[3] = ucParamLength + 2;
    pucBuffer[4] = (unsigned char)eInstruction;

    // Calculate the checksum.
    // Checksum = ~ (ID + Length + Instruction + Parameter1 + ... Parameter N)
    unsigned char ucChecksum = pucBuffer[2] + pucBuffer[3] + pucBuffer[4];
    for (i = 0; i < ucParamLength; i++) {
        ucChecksum += pucTxParam[i];
    }
    ucChecksum = ~ucChecksum;


    // Make sure there is enough space in the UART Tx buffer.
    // If not, return with error code.
    if (uiUART5GetTxSpace() < (ucParamLength + MIN_PACKET_LENGHT)) {
        // Release control on external UART.
        if (xSystemState.bRtosRunning == 1) {
            xSemaphoreGive(xExternalUartMutex);
        }

        return EM_ERR_TX_BUSY;
    }


    
    // Do not switch task when transmitting data.
    if (xSystemState.bRtosRunning == 1) {
        vTaskSuspendAll();
    }



    // Send the packet.
    uiUART5Write(pucBuffer, 5);                 // 0xFF, 0xFF, ID, Length, Instruction.
    uiUART5Write(pucTxParam, ucParamLength);    // Parameters.
    uiUART5Write(&ucChecksum, 1);               // Checksum.

    // Wait until all the data is sent.
    while (ucUART5IsTransmitting() == 1);



    // Flush the UART Rx buffer.
    vUART5FlushRxBuffer();

    // Switch to Rx mode.
    vRxMode();



    // Resume all task.
    if (xSystemState.bRtosRunning == 1) {
        xTaskResumeAll();
    }



    // We'll only get a response if not using broadcast ID or PING command is used.
    if ((ucId != EM_BROADCAST_ID) || (eInstruction == EM_INST_PING)) {

        // Waiting for the first byte to arrive.
        unsigned char ucTimeoutCount = G15_RESPOND_TIMEOUT;
        while (uiUART5GetRxDataCount() == 0) {
            if (ucTimeoutCount-- == 0) {
                // Release control on external UART.
                if (xSystemState.bRtosRunning == 1) {
                    xSemaphoreGive(xExternalUartMutex);
                }

                return EM_ERR_RX_TIMEOUT;
            }
            Delay10us(1);
        }

        // Get the first 5 bytes (0xFF, 0xFF, ID, Length, Error).
        for (i = 0; i < 5; i++) {
            // Wait for Rx data. Timeout after 1000us.
            ucTimeoutCount = 100;
            while (uiUART5GetRxDataCount() == 0) {
                if (ucTimeoutCount-- == 0) {
                    // Release control on external UART.
                    if (xSystemState.bRtosRunning == 1) {
                        xSemaphoreGive(xExternalUartMutex);
                    }

                    return EM_ERR_RX_TIMEOUT;
                }
                Delay10us(1);
            }

            // Read the data.
            uiUART5Read(&pucBuffer[i], 1);
        }

        // Calculate the checksum.
        // Checksum = ~ (ID + Length + Error + Parameter1 + ... Parameter N)
        ucChecksum = pucBuffer[2] + pucBuffer[3] + pucBuffer[4];



        // Read the parameter if there is any.
        ucParamLength = pucBuffer[3] - 2;
        for (i = 0; i < ucParamLength; i++) {
            // Wait for Rx data. Timeout after 1000us.
            ucTimeoutCount = 100;
            while (uiUART5GetRxDataCount() == 0) {
                if (ucTimeoutCount-- == 0) {
                    // Release control on external UART.
                    if (xSystemState.bRtosRunning == 1) {
                        xSemaphoreGive(xExternalUartMutex);
                    }

                    return EM_ERR_RX_TIMEOUT;
                }
                Delay10us(1);
            }

            // Read the parameters.
            unsigned char ucTempBuffer;
            uiUART5Read(&ucTempBuffer, 1);

            // Make sure the buffer is not NULL.
            if (pucRxData != NULL) {
                pucRxData[i] = ucTempBuffer;
            }

            // Calculate the checksum.
            ucChecksum += ucTempBuffer;
        }




        // Calculate the checksum.
        ucChecksum = ~ucChecksum;



        // Read the checksum.
        // Wait for Rx data. Timeout after 1000us.
        ucTimeoutCount = 100;
        while (uiUART5GetRxDataCount() == 0) {
            if (ucTimeoutCount-- == 0) {
                // Release control on external UART.
                if (xSystemState.bRtosRunning == 1) {
                    xSemaphoreGive(xExternalUartMutex);
                }

                return EM_ERR_RX_TIMEOUT;
            }
            Delay10us(1);
        }

        // Read the checksum.
        uiUART5Read(&pucBuffer[5], 1);



        // Copy the G15 status into the error code.
        eErrorCode |= (EM_ERROR)pucBuffer[4];

        // Check the returned header.
        if((pucBuffer[0] != 0xff) || (pucBuffer[1] != 0xff)) {
            eErrorCode |= EM_ERR_RX_HEADER;
        }

        // Check the returned ID.
        if(pucBuffer[2] != ucId) {
            eErrorCode |= EM_ERR_RX_ID;
        }

        // Check the checksum.
        if (pucBuffer[5] != ucChecksum) {
            eErrorCode |= EM_ERR_RX_CHECKSUM;
        }

    }   // End of "if ((ucId != EM_BROADCAST_ID) || (eInstruction == PING))".



    // Release control on external UART.
    if (xSystemState.bRtosRunning == 1) {
        xSemaphoreGive(xExternalUartMutex);
    }


    return eErrorCode;
}
