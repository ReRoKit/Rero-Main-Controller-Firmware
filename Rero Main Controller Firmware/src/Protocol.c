/*******************************************************************************
 * Functions to handle the communication protocol with the rero controller.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include <string.h>
#include "Protocol.h"
#include "Variables.h"
#include "Configuration.h"
#include "PlayMotion.h"
#include "Planner.h"
#include "FatFs/FatFsWrapper.h"

#include "G15 and Sensors/G15.h"
#include "G15 and Sensors/Sensors.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

typedef enum __attribute__((packed)) {
    RECEIVING_COMMAND,
    RECEIVING_DATA,
    TRANSMITTING_DATA,
    FINISH_TRANSMITTING
} PROCESS_STATE;



/*******************************************************************************
 * FUNCTION: ucProcessCommandPacket
 *
 * PARAMETERS:
 * ~ pucReceivedPacket  - Received command or data packet.
 *                      - Must be 64 bytes long.
 * ~ pucTransmitPacket  - Buffer for the packet that will be sent back to host.
 *                      - Must be 64 bytes long.
 *
 * RETURN:
 * ~ Number of bytes to be sent.
 * ~ This function must be called repeatedly until it returns 0.
 *
 * DESCRIPTIONS:
 * Process the command received from the host.
 *
 *******************************************************************************/
unsigned char ucProcessCommandPacket(const unsigned char* pucReceivedPacket, unsigned char* pucTransmitPacket)
{
    static PROCESS_STATE eProcessState = RECEIVING_COMMAND;
    static unsigned char ucCommand = 0;
    static unsigned short usNumberOfTxPacket;
    static unsigned long ulNumberOfRxPacket;    // 32-bit
    unsigned char ucTransmitDataLength;

    unsigned char ucId = 0;
    unsigned short usSpeed = 0;
    unsigned short usPosition = 0;
    unsigned short usLimitCw = 0;
    unsigned short usLimitCcw = 0;

    const char* szFileName;
    const char* szFilenameHead;
    unsigned short usFileSize;      // 16-bit
    unsigned long ulLargeFileSize;  // 32-bit
    static char szFullFilePath[MAX_FILENAME_LENGTH * 3];
    static FSFILE *pxFile = NULL;



    // Reset the Tx data length.
    ucTransmitDataLength = 0;

    // Check the current state.
    switch (eProcessState) {
        // Receiving command packet.
        case RECEIVING_COMMAND: {
            ucCommand = pucReceivedPacket[0];

            // Check the command type.
            switch (ucCommand) {
                // Save data to file (with file number).
                case 0x80: {
                    // Get the file size.
                    usFileSize = (unsigned short)pucReceivedPacket[1] + ((unsigned short)pucReceivedPacket[2] << 8);

                    // Get the date and time.
                    vSetClock((unsigned int)pucReceivedPacket[3] + 2000, pucReceivedPacket[4], pucReceivedPacket[5], pucReceivedPacket[6], pucReceivedPacket[7], pucReceivedPacket[8]);

                    // Determine the file name and head of the file name.
                    switch (pucReceivedPacket[9]) {
                        case 1: szFileName = "File1.rmo"; szFilenameHead = "File1"; break;
                        case 2: szFileName = "File2.rmo"; szFilenameHead = "File2"; break;
                        case 3: szFileName = "File3.rmo"; szFilenameHead = "File3"; break;
                        case 4: szFileName = "File4.rmo"; szFilenameHead = "File4"; break;
                        case 5: szFileName = "File5.rmo"; szFilenameHead = "File5"; break;

                        case 6: szFileName = "XU.rmo"; szFilenameHead = "XU"; break;
                        case 7: szFileName = "XD.rmo"; szFilenameHead = "XD"; break;
                        case 8: szFileName = "XL.rmo"; szFilenameHead = "XL"; break;
                        case 9: szFileName = "XR.rmo"; szFilenameHead = "XR"; break;

                        case 10: szFileName = "CU.rmo"; szFilenameHead = "CU"; break;
                        case 11: szFileName = "CD.rmo"; szFilenameHead = "CD"; break;
                        case 12: szFileName = "CL.rmo"; szFilenameHead = "CL"; break;
                        case 13: szFileName = "CR.rmo"; szFilenameHead = "CR"; break;

                        default: szFileName = "File5.rmo"; szFilenameHead = "File5"; break;
                    }

                    // Delete all the program files in the selected memory.
                    ucDeleteProgramFiles(szFilenameHead);

                    // Get the full file path.
                    strcpy(szFullFilePath, szProgramFolder);
                    strcat(szFullFilePath, "/");
                    strcat(szFullFilePath, szFileName);

                    // Open the file for write.
                    xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
                    pxFile = FSfopen(szFullFilePath, "w");
                    xSemaphoreGive(xSdCardMutex);

                    // Calculate the packet length.
                    ulNumberOfRxPacket = (unsigned long)((usFileSize / 64) + 1);

                    // Set next state as receiving data.
                    eProcessState = RECEIVING_DATA;
                    break;
                }

                // Set servo position.
                // Speed is fixed at 0xa8.
                case 0x81: {
                    ucId = pucReceivedPacket[1];
                    usPosition = (unsigned short)pucReceivedPacket[2] + ((unsigned short)pucReceivedPacket[3] << 8);
                    usSpeed = 0xa8;
                    eG15SetPositionSpeed(ucId, WRITE_NOW, usPosition, NORMAL_POSITIONING, usSpeed, POSITION_SPEED_CONTROL);
                    break;
                }

                // Set LED state.
                case 0x82: {
                    ucId = pucReceivedPacket[1];
                    unsigned char ucLed = pucReceivedPacket[2];
                    unsigned char ucHeadLeftRight = pucReceivedPacket[2];
                    unsigned char ucHeadRgb = pucReceivedPacket[3];

                    // Get the module model.
                    EM_MODEL eModel;
                    eEMGetModel(ucId, &eModel);

                    // Set the LED according to model.
                    switch (eModel) {
                        case EM_MODEL_G15: {
                            eG15SetLed(ucId, WRITE_NOW, ucLed);
                            break;
                        }
                        case EM_MODEL_IR:
                        case EM_MODEL_US:
                        case EM_MODEL_TACTILE:
                        case EM_MODEL_COLOUR:
                        case EM_MODEL_LINE: 
						case EM_MODEL_RGBLIGHT: {
                            eSensorSetLed(ucId, eModel, ucLed);
                            break;
                        }
                        case EM_MODEL_HEAD: {
                            eHeadSetLed(ucId, ucHeadRgb, ucHeadLeftRight);
                            break;
                        }
                    }

                    break;
                }

                // Set servo torque state
                case 0x83: {
                    ucId = pucReceivedPacket[1];
                    unsigned char ucTorque = pucReceivedPacket[2];
                    eG15SetTorque(ucId, WRITE_NOW, ucTorque);
                    break;
                }

                // Set servo position/wheel mode and angle limit.
                case 0x84: {
                    ucId = pucReceivedPacket[1];
                    unsigned char ucWheelMode = pucReceivedPacket[2];

                    // Position mode.
                    if (ucWheelMode == 0) {
                        usLimitCw = (unsigned short)pucReceivedPacket[3] + ((unsigned short)pucReceivedPacket[4] << 8);
                        usLimitCcw = (unsigned short)pucReceivedPacket[5] + ((unsigned short)pucReceivedPacket[6] << 8);
                        eG15SetAngleLimit(ucId, usLimitCw, usLimitCcw);
                    }
                    
                    // Wheel mode.
                    else {
                        eG15SetAngleLimit(ucId, 0, 0);
                    }

                    break;
                }

                // Set servo speed (Wheel mode).
                case 0x85: {
                    ucId = pucReceivedPacket[1];
                    G15_SPEED_CONTROL_MODE eSpeedMode = (pucReceivedPacket[2] == 0)? WHEEL_CCW : WHEEL_CW;
                    usSpeed = (unsigned short)pucReceivedPacket[3] + ((unsigned short)pucReceivedPacket[4] << 8);
                    eG15SetSpeed(ucId, WRITE_NOW, usSpeed, eSpeedMode);
                    break;
                }

                // Stop servo at current position.
                case 0x86: {
                    ucId = pucReceivedPacket[1];
                    eG15GetPosition(ucId, &usPosition);
                    eG15SetPosition(ucId, WRITE_NOW, usPosition, NORMAL_POSITIONING);
                    break;
                }

                // Set servo angle limit.
                case 0x87: {
                    ucId = pucReceivedPacket[1];
                    unsigned short usLimitCw = (unsigned short)pucReceivedPacket[2] + (unsigned short)(pucReceivedPacket[3] << 8);
                    unsigned short usLimitCcw = (unsigned short)pucReceivedPacket[4] + (unsigned short)(pucReceivedPacket[5] << 8);
                    eG15SetAngleLimit(ucId, usLimitCw, usLimitCcw);
                    break;
                }

                // Set RGB Colour (RGB Light Module).
                case 0x88: {
                    ucId = pucReceivedPacket[1];
                    unsigned char ucRed = pucReceivedPacket[2];
                    unsigned char ucGreen = pucReceivedPacket[3];
                    unsigned char ucBlue = pucReceivedPacket[4];
                    eRgbLightModuleSetRgb(ucId, ucRed, ucGreen, ucBlue);
                    break;
                }

                // Set robot name.
                case 0x8e: {
                    unsigned char ucNameLength = pucReceivedPacket[1];

                    // Update the robot name.
                    unsigned char i;
                    for (i = 0; i < ucNameLength; i++) {
                        pucRobotName[i] = pucReceivedPacket[i+2];
                    }

                    // Fill in NULL for the remaining allocated spaces
                    // to clear up possible longer previous name.
                    for(; i < sizeof(pucRobotName); i++) {
                        pucRobotName[i] = 0;
                    }

                    // Save the robot name to file.
                    usWriteConfig(pucRobotNameFilePath, (void*)&pucRobotName, strlen(pucRobotName));

                    break;
                }

                // Get robot name.
                case 0x8f: {
                    // Copy the robot name to the buffer.
                    strcpy(pucTransmitPacket, pucRobotName);

                    // Set the return packet size.
                    ucTransmitDataLength = strlen(pucTransmitPacket) + 1;

                    // Set next state as finish transmitting the data.
                    eProcessState = FINISH_TRANSMITTING;
                    break;
                }

                // Get servo current position.
                case 0x91: {
                     ucId = pucReceivedPacket[1];

                    // Get the position.
                    EM_ERROR eErrorCode = eG15GetPosition(ucId, &usPosition);
                    if (eErrorCode == EM_NO_ERROR) {
                        pucTransmitPacket[0] = (unsigned char)(usPosition & 0xff);
                        pucTransmitPacket[1] = (unsigned char)(usPosition >> 8);
                    }
                    else {
                        pucTransmitPacket[0] = 0;
                        pucTransmitPacket[1] = 0;
                    }

                    // Error code.
                    pucTransmitPacket[2] = (unsigned char)(eErrorCode & 0xff);
                    pucTransmitPacket[3] = (unsigned char)(eErrorCode >> 8);

                    // Set the return packet size.
                    ucTransmitDataLength = 4;

                    // Set next state as finish transmitting the data.
                    eProcessState = FINISH_TRANSMITTING;
                    break;
                }

                // Get the number of connected external module and their ID.
                case 0x92: {
                    unsigned char ucNumberOfModule = 0;
                    unsigned char i;
                    for (i = 0; i <= EM_MAX_ID; i++) {
                        if (eEMPing(i) == EM_NO_ERROR) {
                            pucTransmitPacket[++ucNumberOfModule] = i;
                        }
                    }
                    pucTransmitPacket[0] = ucNumberOfModule;

                    // Set the return packet size.
                    ucTransmitDataLength = ucNumberOfModule + 1;

                    // Set next state as finish transmitting the data.
                    eProcessState = FINISH_TRANSMITTING;
                    break;
                }

                // Get servo angle limit.
                case 0x93: {
                    ucId = pucReceivedPacket[1];
                    if (eG15GetAngleLimit(ucId, &usLimitCw, &usLimitCcw) == EM_NO_ERROR) {
                        pucTransmitPacket[0] = ucId;
                    }
                    else {
                        pucTransmitPacket[0] = 0xff;
                    }

                    pucTransmitPacket[1] = (unsigned char)(usLimitCw & 0xff);
                    pucTransmitPacket[2] = (unsigned char)(usLimitCw >> 8);
                    pucTransmitPacket[3] = (unsigned char)(usLimitCcw & 0xff);
                    pucTransmitPacket[4] = (unsigned char)(usLimitCcw >> 8);

                    // Set the return packet size.
                    ucTransmitDataLength = 5;

                    // Set next state as finish transmitting the data.
                    eProcessState = FINISH_TRANSMITTING;
                    break;
                }

                // Get external module model.
                case 0x94: {
                    ucId = pucReceivedPacket[1];

                    EM_MODEL eModel;

                    // Status.
                    if (eEMGetModel(ucId, &eModel) == EM_NO_ERROR) {
                        pucTransmitPacket[0] = ucId;
                    }
                    else {
                        pucTransmitPacket[0] = 0xff;
                    }

                    // Model.
                    pucTransmitPacket[1] = (unsigned char)(eModel & 0xff);
                    pucTransmitPacket[2] = (unsigned char)(eModel >> 8);

                    // Set the return packet size.
                    ucTransmitDataLength = 3;

                    // Set next state as finish transmitting the data.
                    eProcessState = FINISH_TRANSMITTING;
                    break;
                }

                // Delete all the associated files in the memory.
                case 0xa0: {
                    // Delete all the files start with this name.
                    szFilenameHead = &pucReceivedPacket[1];
                    unsigned char ucReadOnly = ucDeleteProgramFiles(szFilenameHead);
                    
                    // Return the read-only flag.
                    pucTransmitPacket[0] = ucReadOnly;
                    
                    // Set the return packet size.
                    ucTransmitDataLength = 1;
                    
                    // Set next state as finish transmitting the data.
                    eProcessState = FINISH_TRANSMITTING;
                    break;
                }

                // Save data to file (with file name).
                case 0xa1: {
                    // Get the file size.
                    usFileSize = (unsigned short)pucReceivedPacket[1] + ((unsigned short)pucReceivedPacket[2] << 8);

                    // Get the date and time.
                    vSetClock((unsigned int)pucReceivedPacket[3] + 2000, pucReceivedPacket[4], pucReceivedPacket[5], pucReceivedPacket[6], pucReceivedPacket[7], pucReceivedPacket[8]);

                    // File name.
                    szFileName = &pucReceivedPacket[9];

                    // Get the full file path.
                    strcpy(szFullFilePath, szProgramFolder);
                    strcat(szFullFilePath, "/");
                    strcat(szFullFilePath, szFileName);

                    // Open the file for write.
                    xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
                    pxFile = FSfopen(szFullFilePath, "w");
                    xSemaphoreGive(xSdCardMutex);

                    // Calculate the packet length.
                    ulNumberOfRxPacket = (unsigned long)((usFileSize / 64) + 1);

                    // Set next state as receiving data.
                    eProcessState = RECEIVING_DATA;
                    break;
                }

                // Save large data to file (with file name).
                case 0xa6: {
                    // Get the file size.
                    ulLargeFileSize = (unsigned long)pucReceivedPacket[1] + 
                            ((unsigned long)pucReceivedPacket[2] << 8) + 
                            ((unsigned long)pucReceivedPacket[3] << 16) + 
                            ((unsigned long)pucReceivedPacket[4] << 24);

                    // Get the date and time.
                    vSetClock((unsigned int)pucReceivedPacket[5] + 2000, pucReceivedPacket[6], pucReceivedPacket[7], pucReceivedPacket[8], pucReceivedPacket[9], pucReceivedPacket[10]);

                    // File name.
                    szFileName = &pucReceivedPacket[11];

                    // Get the full file path.
                    strcpy(szFullFilePath, szProgramFolder);
                    strcat(szFullFilePath, "/");
                    strcat(szFullFilePath, szFileName);

                    // Open the file for write.
                    xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
                    pxFile = FSfopen(szFullFilePath, "w");
                    xSemaphoreGive(xSdCardMutex);

                    // Calculate the packet length.
                    ulNumberOfRxPacket = (ulLargeFileSize / 64) + 1;

                    // Set next state as receiving data.
                    eProcessState = RECEIVING_DATA;
                    break;
                }

                // Read data from file.
                case 0xa2: {
                    // File name.
                    szFileName = &pucReceivedPacket[1];

                    // Get the full file path.
                    strcpy(szFullFilePath, szProgramFolder);
                    strcat(szFullFilePath, "/");
                    strcat(szFullFilePath, szFileName);

                    // Open the file for read.
                    xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
                    pxFile = FSfopen(szFullFilePath, "r");

                    // Read the file size.
                    if (pxFile != NULL) {
                        usFileSize = (unsigned short)f_size(pxFile);
                    }
                    else {
                        usFileSize = 0;
                    }
                    xSemaphoreGive(xSdCardMutex);

                    pucTransmitPacket[0] = (unsigned char)(usFileSize & 0xff);
                    pucTransmitPacket[1] = (unsigned char)(usFileSize >> 8);

                    // Set the return packet size for first packet.
                    ucTransmitDataLength = 2;

                    // Calculate the number of packet.
                    usNumberOfTxPacket = (usFileSize / 64) + 1;

                    // Set next state as transmitting data.
                    eProcessState = TRANSMITTING_DATA;
                    break;
                }

                // Play motion / planner file.
                case 0xa3: {
                    // File name.
                    szFileName = &pucReceivedPacket[1];

                    // Play the motion/planner file.
                    ePlayMotionStart(szFileName);
                    ePlannerRun(szFileName);
                    break;
                }

                // Stop motion / planner file.
                case 0xa4: {
                    // Determine the stop mode.
                    MOTION_STATE eStopMode = STOP_NOW;
                    switch (pucReceivedPacket[1]) {
                        case 0:eStopMode = STOP_NOW; break;
                        case 1:eStopMode = STOP_FRAME; break;
                        case 2:eStopMode = STOP_FILE; break;
                        case 3:eStopMode = STOP_BT; break;
                    }
                    
                    // File name.
                    szFileName = &pucReceivedPacket[2];

                    // Stop the motion /planner file.
                    vPlayMotionStop(szFileName, eStopMode);
                    vPlannerStop(szFileName, eStopMode);
                    break;
                }

                // Get battery level.
                case 0xa5: {
                    // Copy the battery level to the buffer.
                    pucTransmitPacket[0] = (unsigned char)uiBattPercentage;

                    // Set the return packet size.
                    ucTransmitDataLength = 1;

                    // Set next state as finish transmitting the data.
                    eProcessState = FINISH_TRANSMITTING;
                    break;
                }

                // Get sensor value.
                case 0xb0: {
                    ucId = pucReceivedPacket[1];
                    unsigned char ucSensorType = pucReceivedPacket[2];

                    // Reading sensor status.
                    unsigned short usSensorValue = 0;
                    EM_ERROR eErrorCode;
                    switch (ucSensorType) {
                        case 0: eErrorCode = eInfraredSensorGetValue(ucId, (unsigned char*)&usSensorValue); break;
                        case 1: eErrorCode = eUltrasonicSensorGetValue(ucId, &usSensorValue);               break;
                        case 2: eErrorCode = eHeadGetIrValue(ucId, (unsigned char*)&usSensorValue);         break;
                        case 3: eErrorCode = eHeadGetMicValue(ucId, (unsigned char*)&usSensorValue);        break;
                        case 4: eErrorCode = eTactileSensorGetValue(ucId, (unsigned char*)&usSensorValue);  break;
                        case 5: eErrorCode = eLineSensorGetDigital(ucId, (unsigned char*)&usSensorValue);   break;
                        case 6: eErrorCode = eColourSensorGetColour(ucId, (unsigned char*)&usSensorValue);  break;
                        case 7: eErrorCode = eColourSensorGetAmbient(ucId, (unsigned char*)&usSensorValue);  break;
                    }
                    
                    // Error state.
                    pucTransmitPacket[0] = (eErrorCode == EM_NO_ERROR)? ucId : 0xff;

                    // Sensor value.
                    pucTransmitPacket[1] = (unsigned char)(usSensorValue & 0xff);
                    pucTransmitPacket[2] = (unsigned char)(usSensorValue >> 8);


                    // Set the return packet size.
                    ucTransmitDataLength = 3;

                    // Set next state as finish transmitting the data.
                    eProcessState = FINISH_TRANSMITTING;
                    break;
                }

                // NOP command.
                case 0xdb: {
                    break;
                }

                // Get firmware version.
                case 0xdc: {
                    // Copy the firmware version to the buffer.
                    strcpy(pucTransmitPacket, szFirmwareVersion);

                    // Set the return packet size.
                    ucTransmitDataLength = strlen(pucTransmitPacket) + 1;

                    // Set next state as finish transmitting the data.
                    eProcessState = FINISH_TRANSMITTING;
                    break;
                }

            } // switch (ucCommand)

            break;
        }



        // Receiving data to save to file.
        case RECEIVING_DATA: {
            // Check the command type.
            switch (ucCommand) {
                // Save data to file.
                // The file must be opened.
                case 0x80:
                case 0xa1: 
                case 0xa6: {
                    xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
                    
                    // Write data into the file
                    if (pxFile != NULL) {
                        FSfwrite(pucReceivedPacket, 1, 64, pxFile);
                    }

                    // If there is no more data to receive...
                    if (--ulNumberOfRxPacket == 0) {
                        // Close the file.
                        FSfclose(pxFile);
                        pxFile = NULL;

                        // Reset next state.
                        eProcessState = RECEIVING_COMMAND;
                    }
                    
                    xSemaphoreGive(xSdCardMutex);
                    break;
                }
            } // switch (ucCommand)
            break;
        }



        // Transmitting data to the host.
        case TRANSMITTING_DATA: {
            // Check the command type.
            switch (ucCommand) {
                // Read data from file.
                // The file must be opened.
                case 0xa2: {
                    xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
                    
                    // Read the next packet.
                    ucTransmitDataLength = FSfread(pucTransmitPacket, 1, 64, pxFile);

                    // Transmit extra one packet if there is no more data from file.
                    if (ucTransmitDataLength == 0) {
                        ucTransmitDataLength = 1;
                        pucTransmitPacket[0] = 0xff;
                    }

                    // If there is no more data to send...
                    if (--usNumberOfTxPacket == 0) {
                        // Close the file.
                        FSfclose(pxFile);
                        pxFile = NULL;

                        // Reset next state.
                        eProcessState = FINISH_TRANSMITTING;
                    }
                    
                    xSemaphoreGive(xSdCardMutex);

                    break;
                }
            } // switch (ucCommand)

            break;
        }



        // Finish transmitting data to the host.
        // We need this extra state to indicate that there is no more data to be transmitted.
        case FINISH_TRANSMITTING: {
            // Reset the state to receive next command.
            eProcessState = RECEIVING_COMMAND;
            break;
        }



        default: {
            eProcessState = RECEIVING_COMMAND;
            break;
        }

    } // switch (eProcessState)

    return ucTransmitDataLength;
}
