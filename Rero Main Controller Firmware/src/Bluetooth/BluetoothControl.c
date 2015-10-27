/*******************************************************************************
 * This file provides the functions for Bluetooth control.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include <string.h>
#include "Variables.h"
#include "Bluetooth/BluetoothControl.h"
#include "Bluetooth/uart2.h"
#include "PlayMotion.h"
#include "Planner.h"
#include "GUI/GraphicScreens.h"
#include "Protocol.h"
#include "Log.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

// Maximum delay between each received byte in a packet.
// Unit = ms.
#define BT_RX_TIMEOUT           1000

// Acknowledgement.
// Error has higher priority.
#define BT_ACK_CHECKSUM_ERROR   0xaa
#define BT_ACK_NOT_READY        0x50
#define BT_ACK_READY            0x55



/*******************************************************************************
 * PRIVATE GLOBAL VARIABLES
 *******************************************************************************/

static BLUETOOTH_MODE prv_eBtMode = TRANSPARENT;
static unsigned char prv_pucTxBuffer[64];
static unsigned char prv_pucRxBuffer[64];


/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 *******************************************************************************/

static unsigned int prv_uiReadBluetooth(unsigned char *pucBuffer, unsigned char uiLength, unsigned long ulTimeout);
static void prv_vSendAcknowledge(unsigned char ucAck);



/*******************************************************************************
 * FUNCTION: prv_uiReadBluetooth
 *
 * PARAMETERS:
 * ~ *pucBuffer	- Buffer to store the received data.
 * ~ uiLength	- Number of bytes to read.
 * ~ ulTimeout  - Timeout in ms for each byte.
 *
 * RETURN:
 * ~ Number of bytes read.
 *
 * DESCRIPTIONS:
 * Read the data from Bluetooth.
 * This function will return if timeout.
 *
 *******************************************************************************/
static unsigned int prv_uiReadBluetooth(unsigned char *pucBuffer, unsigned char uiLength, unsigned long ulTimeout)
{
    unsigned int uiActualLength = 0;

    while (uiLength-- > 0) {
        // Clear the semaphore if there is any.
        xSemaphoreTake(xBluetoothRxSemaphore, 0);

        // Wait if there is no data in the UART Rx buffer.
        if (uiUART2GetRxDataCount() == 0) {
            // Return if timeout.
            if (xSemaphoreTake(xBluetoothRxSemaphore, ulTimeout / portTICK_RATE_MS) == pdFALSE) {
                return uiActualLength;
            }
        }

        // Read the data.
        uiUART2Read(pucBuffer++, 1);

        // Increase the number of data read.
        uiActualLength++;
    }

    return uiActualLength;
}



/*******************************************************************************
 * FUNCTION: prv_vSendAcknowledge
 *
 * PARAMETERS:
 * ~ ucAck      - Acknowledge byte.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Send an acknowledgement.
 * This function will wait until there is space in the UART Tx buffer.
 *
 *******************************************************************************/
static void prv_vSendAcknowledge(unsigned char ucAck)
{
    // Wait until there is space in the UART Tx buffer.
    while (uiUART2GetTxSpace() < 1) {
        vTaskDelay(10 / portTICK_RATE_MS);
    }

    // Send the acknowledgement.
    uiUART2Write(&ucAck, 1);
}



/*******************************************************************************
 * FUNCTION: eGetBluetoothVersion
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ Bluetooth version.
 *
 * DESCRIPTIONS:
 * Get the version of the bluetooth module.
 *
 *******************************************************************************/
BLUETOOTH_VERSION eGetBluetoothVersion(void)
{
    static BLUETOOTH_VERSION eBtVersion = UNKNOWN;

    // Check the bluetooth version if this is the first time this function is called.
    if (eBtVersion == UNKNOWN) {
        // Assume it's bluetooth v2.0...
        // Enable the bluetooth module in AT mode.
        // The baudrate will always be 38400.
        BT_AT_MODE = AT;
        BT_RESET = 1;
        vUART2SetBaudRate(BT2_BAUDRATE);


        // Flush the Rx FIFO.
        vUART2FlushRxBuffer();

        // Keep pinging the bluetooth module using v2.0 protocol for up to 2 seconds or response is received.
        unsigned char ucLoopCount = 20;
        do {
            // Ping the bluetooth module.
            const unsigned char szPingCommand[] = "AT\r\n";
            while (uiUART2GetTxSpace() < (sizeof(szPingCommand) - 1));
            uiUART2Write(szPingCommand, sizeof(szPingCommand) - 1);

            vTaskDelay(100 / portTICK_RATE_MS);

        // Loop for 2 seconds if "OK\r\n" is not received.
        // We don't check the received string here. We only expect to receive 4 bytes of data.
        } while ((--ucLoopCount > 0) && (uiUART2GetRxDataCount() < 4));



        // Now, check whether we have received any response from the bluetooth module using v2.0 protocol.
        // If timeout occurred, means we didn't receive any response.
        if (ucLoopCount == 0) {
            // So now we will try to use the v4.0 protocol.
            vUART2SetBaudRate(BT4_BAUDRATE);


            // Flush the Rx FIFO.
            vUART2FlushRxBuffer();

            // Keep pinging the bluetooth module using v4.0 protocol for up to 10 seconds or response is received.
            ucLoopCount = 100;
            do {
                // Ping the bluetooth module.
                const unsigned char szPingCommand[] = "AT";
                while (uiUART2GetTxSpace() < (sizeof(szPingCommand) - 1));
                uiUART2Write(szPingCommand, sizeof(szPingCommand) - 1);

                vTaskDelay(100 / portTICK_RATE_MS);

            // Loop for 10 seconds if "OK" is not received.
            // We don't check the received string here. We only expect to receive 2 bytes of data.
            } while ((--ucLoopCount > 0) && (uiUART2GetRxDataCount() < 2));

            // Now, check whether we have received any response from the bluetooth module using v4.0 protocol.
            // If timeout occurred, means we didn't receive any response.
            if (ucLoopCount == 0) {
                // Set the bluetooth error flag as we didn't receive response from the bluetooth module using either v2.0 or v4.0 protocol.
                xSystemError.bBluetoothError = 1;

                // Disable the bluetooth module.
                vDisableBluetooth();
            }

            // Timeout did not occur.
            // This means the bluetooth module is v4.0.
            else {
                eBtVersion = BT_V40;

                // Disable the AT MODE selection pin.
                BT_AT_MODE_TRIS = 1;
            }
        }

        // Timeout did not occur.
        // This means the bluetooth module is v2.0.
        else {
            eBtVersion = BT_V20;
        }
    }

    
    return eBtVersion;
}



/*******************************************************************************
 * FUNCTION: vEnableBluetooth
 *
 * PARAMETERS:
 * ~ eBtMode    - Turn on bluetooth in AT or TRANSPARENT mode.
 *              - Not used for Bluetooth 4.0.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Enable the bluetooth module and wait until it is ready.
 *
 *******************************************************************************/
void vEnableBluetooth(BLUETOOTH_MODE eBtMode)
{
    unsigned char ucLoopCount;

    // If we are entering AT mode, set the global mode here.
    if (eBtMode == AT) {
        prv_eBtMode = AT;
    }


    // Get the bluetooth version.
    BLUETOOTH_VERSION eBtVersion = eGetBluetoothVersion();

    // Bluetooth v2.0.
    if (eBtVersion == BT_V20) {
        // Enable the bluetooth module.
        BT_AT_MODE = eBtMode;
        BT_RESET = 1;
        vUART2SetBaudRate(BT2_BAUDRATE);
        vTaskDelay(100 / portTICK_RATE_MS);

        // Only ping the bluetooth module in AT mode.
        if (eBtMode == AT) {
            // Flush the Rx FIFO.
            vUART2FlushRxBuffer();

            // Keep pinging the bluetooth module for up to 2 seconds or response is received.
            ucLoopCount = 20;
            do {
                // Ping the bluetooth module.
                const unsigned char szPingCommand[] = "AT\r\n";
                while (uiUART2GetTxSpace() < (sizeof(szPingCommand) - 1));
                uiUART2Write(szPingCommand, sizeof(szPingCommand) - 1);

                vTaskDelay(100 / portTICK_RATE_MS);

            // Loop for 2 seconds if "OK\r\n" is not received.
            // We don't check the received string here. We only expect to receive 4 bytes of data.
            } while ((--ucLoopCount > 0) && (uiUART2GetRxDataCount() < 4));

            // If timeout occurred, means we didn't receive any response.
            if (ucLoopCount == 0) {
                // Set the bluetooth error flag.
                xSystemError.bBluetoothError = 1;

                // Disable the bluetooth module.
                vDisableBluetooth();
            }
        }

        // In transparent mode, just put some delay.
        else {
            vTaskDelay(700 / portTICK_RATE_MS);
        }
    }

    // Bluetooth v4.0.
    else if (eBtVersion == BT_V40) {
        // Enable the bluetooth module.
        BT_RESET = 1;
        vUART2SetBaudRate(BT4_BAUDRATE);
        vTaskDelay(100 / portTICK_RATE_MS);

        // Flush the Rx FIFO.
        vUART2FlushRxBuffer();


        // Keep pinging the bluetooth module for up to 10 seconds or response is received.
        ucLoopCount = 100;
        do {
            // Ping the bluetooth module.
            const unsigned char szPingCommand[] = "AT";
            while (uiUART2GetTxSpace() < (sizeof(szPingCommand) - 1));
            uiUART2Write(szPingCommand, sizeof(szPingCommand) - 1);

            vTaskDelay(100 / portTICK_RATE_MS);

        // Loop for 10 seconds if "OK" is not received.
        // We don't check the received string here. We only expect to receive 2 bytes of data.
        } while ((--ucLoopCount > 0) && (uiUART2GetRxDataCount() < 2));

        // If timeout occurred, means we didn't receive any response.
        if (ucLoopCount == 0) {
            // Set the bluetooth error flag.
            xSystemError.bBluetoothError = 1;

            // Disable the bluetooth module.
            vDisableBluetooth();
        }
        
        // We received the response from bluetooth module.
        else {
            // If we are entering transparent mode, start the EDR / BLE module accordingly.
            if (eBtMode == TRANSPARENT) {
                // Determine the start command base on the bluetooth 4.0 mode.
                unsigned char *szStartCommand;
                switch (eBt4Mode) {
                    case EDR:   szStartCommand = "AT+STARE";    break;
                    case BLE:   szStartCommand = "AT+STARB";    break;
                    default:    szStartCommand = "AT+STARE";    break;
                }
                
                
                // Flush the Rx FIFO.
                vUART2FlushRxBuffer();

                // Start the EDR / BLE module.
                while (uiUART2GetTxSpace() < strlen(szStartCommand));
                uiUART2Write(szStartCommand, strlen(szStartCommand));

                // Wait until the respond is received.
                // Response: OK+STARE or OK+STARB
                // We don't check the received string here. We only make sure the number of bytes received is correct.
                if (prv_uiReadBluetooth(prv_pucRxBuffer, 8, BT_RX_TIMEOUT) < 8) {
                    // This command is not compatible with old BT4.0 firmware, thus just ignore it if timeout occured.
                    
//                    // Timeout occurred.
//                    // Set the error flag.
//                    xSystemError.bBluetoothError = 1;
//
//                    // Disable the bluetooth module.
//                    vDisableBluetooth();
//
//                    return;
                }
            }
        }
    }

    // Can't communicate with bluetooth module.
    else {
        // Set the bluetooth error flag and return.
        xSystemError.bBluetoothError = 1;

        // Disable the bluetooth module.
        vDisableBluetooth();
    }


    // If we are entering TRANSPARENT mode, set the global mode after receiving response from the bluetooth module.
    if (eBtMode == TRANSPARENT) {
        prv_eBtMode = TRANSPARENT;
    }
}



/*******************************************************************************
 * FUNCTION: vDisableBluetooth
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Disable the bluetooth module.
 *
 *******************************************************************************/
void vDisableBluetooth(void)
{
    BT_RESET = 0;
}



/*******************************************************************************
 * FUNCTION: vResetBluetooth
 *
 * PARAMETERS:
 * ~ eBtMode    - Reset the bluetooth in AT or TRANSPARENT mode.
 *              - Not used for Bluetooth 4.0.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Reset the bluetooth module and wait until it is ready.
 *
 *******************************************************************************/
void vResetBluetooth(BLUETOOTH_MODE eBtMode)
{
    vDisableBluetooth();
    vTaskDelay(50 / portTICK_RATE_MS);
    vEnableBluetooth(eBtMode);
}



/*******************************************************************************
 * FUNCTION: vConfigureBluetooth
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Configure the bluetooth module.
 *
 *******************************************************************************/
void vConfigureBluetooth(void)
{
    // Get the bluetooth version.
    BLUETOOTH_VERSION eBtVersion = eGetBluetoothVersion();

    // Bluetooth v2.0.
    if (eBtVersion == BT_V20) {
        // Flush the Rx FIFO.
        vUART2FlushRxBuffer();

        // Change the baudrate to 38400.
        const unsigned char pucSetBaudCommand[] = "AT+UART=38400,0,0\r\n";
        while (uiUART2GetTxSpace() < (sizeof(pucSetBaudCommand) - 1));
        uiUART2Write(pucSetBaudCommand, sizeof(pucSetBaudCommand) - 1);

        // Wait until OK\r\n is received.
        // We don't check the received string here. We only expect to receive 4 bytes of data.
        if (prv_uiReadBluetooth(prv_pucRxBuffer, 4, BT_RX_TIMEOUT) < 4) {
            // Timeout occurred.
            // Set the error flag.
            xSystemError.bBluetoothError = 1;

            // Disable the bluetooth module.
            vDisableBluetooth();

            return;
        }


        // Flush the Rx FIFO.
        vUART2FlushRxBuffer();

        // Set as slave.
        const unsigned char pucSetSlaveCommand[] = "AT+ROLE=0\r\n";
        while (uiUART2GetTxSpace() < (sizeof(pucSetSlaveCommand) - 1));
        uiUART2Write(pucSetSlaveCommand, sizeof(pucSetSlaveCommand) - 1);

        // Wait until OK\r\n is received.
        // We don't check the received string here. We only expect to receive 4 bytes of data.
        if (prv_uiReadBluetooth(prv_pucRxBuffer, 4, BT_RX_TIMEOUT) < 4) {
            // Timeout occurred.
            // Set the error flag.
            xSystemError.bBluetoothError = 1;

            // Disable the bluetooth module.
            vDisableBluetooth();

            return;
        }


        // Flush the Rx FIFO.
        vUART2FlushRxBuffer();

        // Change the bluetooth name.
        const unsigned char pucSetNameCommand[] = "AT+NAME=";
        while (uiUART2GetTxSpace() < (sizeof(pucSetNameCommand) - 1));
        uiUART2Write(pucSetNameCommand, sizeof(pucSetNameCommand) - 1);

        while (uiUART2GetTxSpace() < strlen(pucRobotName));
        uiUART2Write(pucRobotName, strlen(pucRobotName));

        while (uiUART2GetTxSpace() < 2);
        uiUART2Write("\r\n", 2);

        // Wait until OK\r\n is received.
        // We don't check the received string here. We only expect to receive 4 bytes of data.
        if (prv_uiReadBluetooth(prv_pucRxBuffer, 4, BT_RX_TIMEOUT) < 4) {
            // Timeout occurred.
            // Set the error flag.
            xSystemError.bBluetoothError = 1;

            // Disable the bluetooth module.
            vDisableBluetooth();

            return;
        }


        // Flush the Rx FIFO.
        vUART2FlushRxBuffer();

        // Read the bluetooth address.
        const unsigned char pucReadAddressCommand[] = "AT+ADDR?\r\n";
        while (uiUART2GetTxSpace() < (sizeof(pucReadAddressCommand) - 1));
        uiUART2Write(pucReadAddressCommand, sizeof(pucReadAddressCommand) - 1);

        // Read the address.
        unsigned char pucAddressBuffer[23];
        if (prv_uiReadBluetooth(pucAddressBuffer, 23, BT_RX_TIMEOUT) < 23) {
            // Timeout occurred.
            // Set the error flag.
            xSystemError.bBluetoothError = 1;

            // Disable the bluetooth module.
            vDisableBluetooth();

            return;
        }
        
        // The response will be in this format: "+ADDR:abcd:ee:fdfd9b".
        // Extract the address from the buffer wihout the header and deliminator.
        strncpy(&szBluetoothAddress[0], &pucAddressBuffer[6], 4);
        strncpy(&szBluetoothAddress[4], &pucAddressBuffer[11], 2);
        strncpy(&szBluetoothAddress[6], &pucAddressBuffer[14], 6);
        szBluetoothAddress[12] = 0;
        
        // Convert the lowercase to upper case character.
        unsigned char i;
        for (i = 0; i < strlen(szBluetoothAddress); i++) {
            // If it's lowercase character...
            if ((szBluetoothAddress[i] >= 'a') && (szBluetoothAddress[i] <= 'z')) {
                szBluetoothAddress[i] -= 32;
            }
        }
    }



    // Bluetooth v4.0.
    else if (eBtVersion == BT_V40) {
        // Flush the Rx FIFO.
        vUART2FlushRxBuffer();

        // Change the PIO1 to indicate the connection status.
        // Command: AT+PIO11
        const unsigned char pucPio1Command[] = "AT+PIO11";
        while (uiUART2GetTxSpace() < (sizeof(pucPio1Command) - 1));
        uiUART2Write(pucPio1Command, sizeof(pucPio1Command) - 1);

        // Wait until the respond is received.
        // Response: OK+Set:1
        // We don't check the received string here. We only make sure the number of bytes received is correct.
        if (prv_uiReadBluetooth(prv_pucRxBuffer, 8, BT_RX_TIMEOUT) < 8) {
            // Timeout occurred.
            // Set the error flag.
            xSystemError.bBluetoothError = 1;

            // Disable the bluetooth module.
            vDisableBluetooth();

            return;
        }


        // Flush the Rx FIFO.
        vUART2FlushRxBuffer();

        // Disable dual mode. Only either EDR or BLE can be connected at one time.
        // Command: AT+DUAL1
        const unsigned char pucDualCommand[] = "AT+DUAL1";
        while (uiUART2GetTxSpace() < (sizeof(pucDualCommand) - 1));
        uiUART2Write(pucDualCommand, sizeof(pucDualCommand) - 1);

        // Wait until the respond is received.
        // Response: OK+Set:1
        // We don't check the received string here. We only make sure the number of bytes received is correct.
        if (prv_uiReadBluetooth(prv_pucRxBuffer, 8, BT_RX_TIMEOUT) < 8) {
            // Timeout occurred.
            // Set the error flag.
            xSystemError.bBluetoothError = 1;

            // Disable the bluetooth module.
            vDisableBluetooth();

            return;
        }
        
        
        // Flush the Rx FIFO.
        vUART2FlushRxBuffer();

        // Do not start the EDR mode automatically after power on.
        // Command: AT+IMME1
        const unsigned char pucImmeCommand[] = "AT+IMME1";
        while (uiUART2GetTxSpace() < (sizeof(pucImmeCommand) - 1));
        uiUART2Write(pucImmeCommand, sizeof(pucImmeCommand) - 1);

        // Wait until the respond is received.
        // Response: OK+Set:1
        // We don't check the received string here. We only make sure the number of bytes received is correct.
        if (prv_uiReadBluetooth(prv_pucRxBuffer, 8, BT_RX_TIMEOUT) < 8) {
            // This command is not compatible with old BT4.0 firmware, thus just ignore it if timeout occured.
            
//            // Timeout occurred.
//            // Set the error flag.
//            xSystemError.bBluetoothError = 1;
//
//            // Disable the bluetooth module.
//            vDisableBluetooth();
//
//            return;
        }
        
        
        // Flush the Rx FIFO.
        vUART2FlushRxBuffer();

        // Do not start the BLE mode automatically after power on.
        // Command: AT+IMMB1
        const unsigned char pucImmbCommand[] = "AT+IMMB1";
        while (uiUART2GetTxSpace() < (sizeof(pucImmbCommand) - 1));
        uiUART2Write(pucImmbCommand, sizeof(pucImmbCommand) - 1);

        // Wait until the respond is received.
        // Response: OK+Set:1
        // We don't check the received string here. We only make sure the number of bytes received is correct.
        if (prv_uiReadBluetooth(prv_pucRxBuffer, 8, BT_RX_TIMEOUT) < 8) {
            // This command is not compatible with old BT4.0 firmware, thus just ignore it if timeout occured.
            
//            // Timeout occurred.
//            // Set the error flag.
//            xSystemError.bBluetoothError = 1;
//
//            // Disable the bluetooth module.
//            vDisableBluetooth();
//
//            return;
        }


        // Flush the Rx FIFO.
        vUART2FlushRxBuffer();

        // Change the bluetooth name for EDR mode.
        // Command: AT+NAME<name>
        const unsigned char pucSetEdrNameCommand[] = "AT+NAME";
        while (uiUART2GetTxSpace() < (sizeof(pucSetEdrNameCommand) - 1));
        uiUART2Write(pucSetEdrNameCommand, sizeof(pucSetEdrNameCommand) - 1);

        while (uiUART2GetTxSpace() < strlen(pucRobotName));
        uiUART2Write(pucRobotName, strlen(pucRobotName));

        // Wait until the respond is received.
        // Response: OK+Set:<name>
        // We don't check the received string here. We only make sure the number of bytes received is correct.
        unsigned char ucResponseLength = 7 + strlen(pucRobotName);
        if (prv_uiReadBluetooth(prv_pucRxBuffer, ucResponseLength, BT_RX_TIMEOUT) < ucResponseLength) {
            // Timeout occurred.
            // Set the error flag.
            xSystemError.bBluetoothError = 1;

            // Disable the bluetooth module.
            vDisableBluetooth();

            return;
        }
        


        // Flush the Rx FIFO.
        vUART2FlushRxBuffer();

        // Change the bluetooth name for BLE mode.
        // Command: AT+NAMB<name>
        const unsigned char pucSetBleNameCommand[] = "AT+NAMB";
        while (uiUART2GetTxSpace() < (sizeof(pucSetBleNameCommand) - 1));
        uiUART2Write(pucSetBleNameCommand, sizeof(pucSetBleNameCommand) - 1);

        while (uiUART2GetTxSpace() < strlen(pucRobotName));
        uiUART2Write(pucRobotName, strlen(pucRobotName));

        // Wait until the respond is received.
        // Response: OK+Set:<name>
        // We don't check the received string here. We only make sure the number of bytes received is correct.
        ucResponseLength = 7 + strlen(pucRobotName);
        if (prv_uiReadBluetooth(prv_pucRxBuffer, ucResponseLength, BT_RX_TIMEOUT) < ucResponseLength) {
            // Timeout occurred.
            // Set the error flag.
            xSystemError.bBluetoothError = 1;

            // Disable the bluetooth module.
            vDisableBluetooth();

            return;
        }


        // Flush the Rx FIFO.
        vUART2FlushRxBuffer();

        // Read the MAC address.
        // Determine the start command base on the bluetooth 4.0 mode.
        unsigned char *pucReadAddressCommand;
        switch (eBt4Mode) {
            case EDR:   pucReadAddressCommand = "AT+ADDE?";    break;
            case BLE:   pucReadAddressCommand = "AT+ADDB?";    break;
            default:    pucReadAddressCommand = "AT+ADDE?";    break;
        }
        while (uiUART2GetTxSpace() < strlen(pucReadAddressCommand));
        uiUART2Write(pucReadAddressCommand, strlen(pucReadAddressCommand));

        // Read the MAC address.
        // Response: OK+Get:<address>
        // Read and discard the "OK+Get:".
        if (prv_uiReadBluetooth(prv_pucRxBuffer, 7, BT_RX_TIMEOUT) < 7) {
            // Timeout occurred.
            // Set the error flag.
            xSystemError.bBluetoothError = 1;

            // Disable the bluetooth module.
            vDisableBluetooth();

            return;
        }

        // Read the address.
        if (prv_uiReadBluetooth((unsigned char*)szBluetoothAddress, 12, BT_RX_TIMEOUT) < 12) {
            // Timeout occurred.
            // Set the error flag.
            xSystemError.bBluetoothError = 1;

            // Disable the bluetooth module.
            vDisableBluetooth();

            return;
        }

        // Terminate with null.
        szBluetoothAddress[12] = 0;
    }



    // Can't communicate with bluetooth module.
    else {
        // Set the bluetooth error flag and return.
        xSystemError.bBluetoothError = 1;

        // Disable the bluetooth module.
        vDisableBluetooth();

        return;
    }
}



/*******************************************************************************
 * FUNCTION: vGetBluetoothPin
 *
 * PARAMETERS:
 * ~ pszPin     - Buffer to store the pin in ASCII. Must be at least 5 bytes.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Read the pin from the bluetooth module.
 *
 *******************************************************************************/
void vGetBluetoothPin(char* pszPin)
{
    // Get the bluetooth version.
    BLUETOOTH_VERSION eBtVersion = eGetBluetoothVersion();

    // Bluetooth v2.0.
    if (eBtVersion == BT_V20) {
        // Flush the Rx FIFO.
        vUART2FlushRxBuffer();

        // Read the bluetooth pin.
        const char szReadPinCommand[] = "AT+PSWD?\r\n";
        while (uiUART2GetTxSpace() < (sizeof(szReadPinCommand) - 1));
        uiUART2Write(szReadPinCommand, sizeof(szReadPinCommand) - 1);

        // Read the pin.
        // We expect to receive 16 bytes of data.
        // Expected response: "+PSWD:1234\r\nOK\r\n"
        if (prv_uiReadBluetooth(prv_pucRxBuffer, 16, BT_RX_TIMEOUT) < 16) {
            // Timeout occurred.
            // Set the error flag.
            xSystemError.bBluetoothError = 1;

            // Disable the bluetooth module.
            vDisableBluetooth();

            // Return with blank pin.
            *pszPin = 0;
            return;
        }

        // The pin is character 6 - 9.
        pszPin[0] = prv_pucRxBuffer[6];
        pszPin[1] = prv_pucRxBuffer[7];
        pszPin[2] = prv_pucRxBuffer[8];
        pszPin[3] = prv_pucRxBuffer[9];
        pszPin[4] = 0;
    }



    // Bluetooth v4.0.
    else if (eBtVersion == BT_V40) {
        // Flush the Rx FIFO.
        vUART2FlushRxBuffer();

        // Read the bluetooth EDR pin.
        const char szReadPinCommand[] = "AT+PINE?";
        while (uiUART2GetTxSpace() < (sizeof(szReadPinCommand) - 1));
        uiUART2Write(szReadPinCommand, sizeof(szReadPinCommand) - 1);

        // Read the pin.
        // We expect to receive 11 bytes of data.
        // Expected response: OK+Get:1234
        if (prv_uiReadBluetooth(prv_pucRxBuffer, 11, BT_RX_TIMEOUT) < 11) {
            // Timeout occurred.
            // Set the error flag.
            xSystemError.bBluetoothError = 1;

            // Disable the bluetooth module.
            vDisableBluetooth();

            // Return with blank pin.
            *pszPin = 0;
            return;
        }

        // The pin is character 7 - 10.
        pszPin[0] = prv_pucRxBuffer[7];
        pszPin[1] = prv_pucRxBuffer[8];
        pszPin[2] = prv_pucRxBuffer[9];
        pszPin[3] = prv_pucRxBuffer[10];
        pszPin[4] = 0;
    }



    // Can't communicate with bluetooth module.
    else {
        // Set the error flag.
        xSystemError.bBluetoothError = 1;

        // Disable the bluetooth module.
        vDisableBluetooth();

        // Return with blank pin.
        *pszPin = 0;
        return;
    }
}



/*******************************************************************************
 * FUNCTION: ucSetBluetoothPin
 *
 * PARAMETERS:
 * ~ szPin     - New pin in ASCII string.
 *
 * RETURN:
 * ~ Return 0 if successful.
 * ~ Return 1 if failed.
 *
 * DESCRIPTIONS:
 * Set the new bluetooth pin.
 *
 *******************************************************************************/
unsigned char ucSetBluetoothPin(const char* szPin)
{
    // Get the bluetooth version.
    BLUETOOTH_VERSION eBtVersion = eGetBluetoothVersion();

    // Bluetooth v2.0.
    if (eBtVersion == BT_V20) {
        // Flush the Rx FIFO.
        vUART2FlushRxBuffer();

        // Set the bluetooth pin.
        const char szSetPinCommand[] = "AT+PSWD=";
        while (uiUART2GetTxSpace() < (sizeof(szSetPinCommand) - 1));
        uiUART2Write(szSetPinCommand, sizeof(szSetPinCommand) - 1);

        while (uiUART2GetTxSpace() < strlen(szPin));
        uiUART2Write(szPin, strlen(szPin));

        while (uiUART2GetTxSpace() < 2);
        uiUART2Write("\r\n", 2);


        // Wait until "OK\r\n" is received.
        // We expect to receive 4 bytes of data.
        // Read the response.
        if (prv_uiReadBluetooth(prv_pucRxBuffer, 4, BT_RX_TIMEOUT) < 4) {
            // Timeout occurred.
            // Set the error flag.
            xSystemError.bBluetoothError = 1;

            // Disable the bluetooth module.
            vDisableBluetooth();

            return 1;
        }

        prv_pucRxBuffer[4] = 0;

        // If the received data is not "OK\r\n"...
        if (strcmp("OK\r\n", prv_pucRxBuffer) != 0) {
            return 1;
        }
        else {
            return 0;
        }
    }



    // Bluetooth v4.0.
    else if (eBtVersion == BT_V40) {
        // Flush the Rx FIFO.
        vUART2FlushRxBuffer();

        // Set the bluetooth pin.
        const char szSetPinCommand[] = "AT+PINE";
        while (uiUART2GetTxSpace() < (sizeof(szSetPinCommand) - 1));
        uiUART2Write(szSetPinCommand, sizeof(szSetPinCommand) - 1);

        while (uiUART2GetTxSpace() < strlen(szPin));
        uiUART2Write(szPin, strlen(szPin));


        // Wait until the response is received.
        // We expect to receive 11 bytes of data.
        // Expected response: OK+Set:1234
        // Read the response.
        if (prv_uiReadBluetooth(prv_pucRxBuffer, 11, BT_RX_TIMEOUT) < 11) {
            // Timeout occurred.
            // Set the error flag.
            xSystemError.bBluetoothError = 1;

            // Disable the bluetooth module.
            vDisableBluetooth();

            return 1;
        }

        prv_pucRxBuffer[11] = 0;

        // If the received data does not start with "OK", return as error.
        if (strncmp("OK", prv_pucRxBuffer, 2) != 0) {
            return 1;
        }
        else {
            return 0;
        }
    }



    // Can't communicate with bluetooth module.
    else {
        // Set the error flag.
        xSystemError.bBluetoothError = 1;

        // Disable the bluetooth module.
        vDisableBluetooth();

        return 1;
    }
}



/*******************************************************************************
* TASK: taskBluetoothProgram
*
* DESCRIPTIONS:
* Task for the bluetooth programming.
*
*******************************************************************************/
void taskBluetoothProgram(void *pvParameters)
{
    unsigned char i;
    unsigned char ucHeader;
    unsigned char ucLength;
    unsigned char ucReceivedChecksum;
    unsigned char ucCalculatedChecksum;
    unsigned char ucReceivedAck;



    // Take the Bluetooth mutex.
    xSemaphoreTake(xBluetoothMutex, portMAX_DELAY);

    // Flush the Rx FIFO.
    vUART2FlushRxBuffer();

    // Configure the bluetooth and wait until it's ready.
    vEnableBluetooth(AT);
    vConfigureBluetooth();
    vResetBluetooth(TRANSPARENT);

    // Flush the Rx FIFO.
    vUART2FlushRxBuffer();

    // Release the Bluetooth mutex.
    xSemaphoreGive(xBluetoothMutex);
    
    
    // Bluetooth is ready.
    xSystemState.bBluetoothReady = 1;



    while (1) {
        // Read the stack watermark and record it if it's below threshold.
        unsigned portBASE_TYPE uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        if (uxHighWaterMark < MIN_STACK_WATERMARK) {
            xSystemError.bStackLowError = 1;

            // Only log it when the watermark value changed.
            static portBASE_TYPE uxPreviousWatermark = 0;
            if (uxHighWaterMark != uxPreviousWatermark) {
                vLogStackWatermark("Bluetooth Program Task", (unsigned short)uxHighWaterMark);
            }
            uxPreviousWatermark = uxHighWaterMark;
        }



        // If Bluetooth is disconnected in Bluetooth page...
        if ( (xSystemState.bBluetoothMode == 1) && (BT_CONNECTED == 0) ) {
            // Stop all planner and motion.
            vPlannerStopCurrent(STOP_NOW);
            vPlayMotionStopAll(STOP_NOW);

            // Flush the Rx FIFO.
            vUART2FlushRxBuffer();
        }



        // Do not process the data if Bluetooth module is in AT mode.
        if (prv_eBtMode == AT) {
            vTaskDelay(200 / portTICK_RATE_MS);
        }
        
        // In transparent mode.
        else {

            // Read the 1st header.
            // Loop back if the header is not 0xff or timeout.
            if (prv_uiReadBluetooth(&ucHeader, 1, BT_RX_TIMEOUT) < 1) continue;
            if (ucHeader != 0xff) continue;

            // Read the 2nd header.
            // Loop back if the header is not 0xff or timeout.
            if (prv_uiReadBluetooth(&ucHeader, 1, BT_RX_TIMEOUT) < 1) continue;
            if (ucHeader != 0xff) continue;

            // Read the Length.
            // Loop back if timeout.
            if (prv_uiReadBluetooth(&ucLength, 1, BT_RX_TIMEOUT) < 1) continue;
            
            
            
            // If the length is > 65, means that it's using legacy rero remote protocol.
            if (ucLength > 65) {
                
                static const char* szFileNameB1 = "File1";
                static const char* szFileNameB2 = "File2";
                static const char* szFileNameB3 = "File3";
                static const char* szFileNameB4 = "File4";
                static const char* szFileNameB5 = "File5";

                static const char* szFileNameXU = "XU";
                static const char* szFileNameXD = "XD";
                static const char* szFileNameXL = "XL";
                static const char* szFileNameXR = "XR";

                static const char* szFileNameCU = "CU";
                static const char* szFileNameCD = "CD";
                static const char* szFileNameCL = "CL";
                static const char* szFileNameCR = "CR";


                const char* szSelectedFileName;
                
                
                // The lenght is part of the data.
                prv_pucRxBuffer[2] = ucLength;

                // Read the remaining 2 bytes.
                // Loop back if timeout occured.
                if (prv_uiReadBluetooth(&prv_pucRxBuffer[3], 2, BT_RX_TIMEOUT) < 2) continue;



                // Select the motion file.
                // Cross buttons.
                if (prv_pucRxBuffer[2] == 'x') {
                    switch(prv_pucRxBuffer[3]) {
                        case 'u': szSelectedFileName = szFileNameXU; break;
                        case 'd': szSelectedFileName = szFileNameXD; break;
                        case 'l': szSelectedFileName = szFileNameXL; break;
                        case 'r': szSelectedFileName = szFileNameXR; break;
                    }
                }
                // Circle buttons.
                else if (prv_pucRxBuffer[2] == 'c') {
                    switch(prv_pucRxBuffer[3]) {
                        case 'u': szSelectedFileName = szFileNameCU; break;
                        case 'd': szSelectedFileName = szFileNameCD; break;
                        case 'l': szSelectedFileName = szFileNameCL; break;
                        case 'r': szSelectedFileName = szFileNameCR; break;
                    }
                }
                // Square buttons.
                else if (prv_pucRxBuffer[2] == 'b') {
                    switch(prv_pucRxBuffer[3]) {
                        case '1': szSelectedFileName = szFileNameB1; break;
                        case '2': szSelectedFileName = szFileNameB2; break;
                        case '3': szSelectedFileName = szFileNameB3; break;
                        case '4': szSelectedFileName = szFileNameB4; break;
                        case '5': szSelectedFileName = szFileNameB5; break;
                    }
                }
                // Request battery level.
                else if ((prv_pucRxBuffer[2] == 'B') && (prv_pucRxBuffer[3] == 'B') && (prv_pucRxBuffer[4] == 'B')) {
                    static unsigned char pucBattTxPacket[] = {0xff, 0xff, 'B', 0};
                    pucBattTxPacket[3] = (unsigned char)(uiBattPercentage * 5 / 100);
                    uiUART2Write(pucBattTxPacket, sizeof(pucBattTxPacket));
                }


                // Determine to play or stop the file.
                // Button is pressed.
                if(prv_pucRxBuffer[4] == 'p') {
                    ePlayMotionStart(szSelectedFileName);
                    ePlannerRun(szSelectedFileName);
                }
                // Button is released.
                else if(prv_pucRxBuffer[4] == 'r') {
                    vPlayMotionStop(szSelectedFileName, STOP_BT);
                    vPlannerStop(szSelectedFileName, STOP_BT);
                }
                
            }   // End of legacy rero remote protocol.
            
            
            
            // Else, it's using latest bluetooth protocol.
            else {
                // Read the data packet.
                // Loop back if timeout.
                if (prv_uiReadBluetooth(prv_pucRxBuffer, ucLength - 1, BT_RX_TIMEOUT) < (ucLength - 1)) continue;

                // Read the Checksum.
                // Loop back if timeout.
                if (prv_uiReadBluetooth(&ucReceivedChecksum, 1, BT_RX_TIMEOUT) < 1) continue;



                // Calculate the checksum.
                ucCalculatedChecksum = ucLength;
                for (i = 0; i < (ucLength - 1); i++) {
                    ucCalculatedChecksum += prv_pucRxBuffer[i];
                }
                ucCalculatedChecksum = ~ucCalculatedChecksum;

                // Send acknowledgement and loop back if the checksum is incorrect.
                if (ucReceivedChecksum != ucCalculatedChecksum) {
                    prv_vSendAcknowledge(BT_ACK_CHECKSUM_ERROR);
                    continue;
                }

                // Send the acknowledgement.
                if (eGetGuiPage() == PAGE_BLUETOOTH) {
                    prv_vSendAcknowledge(BT_ACK_READY);
                }
                else {
                    prv_vSendAcknowledge(BT_ACK_NOT_READY);
                }



                // Fill the remaining bytes with 0xff if it's not full.
                for (i = (ucLength - 1); i < 64; i++) {
                    prv_pucRxBuffer[i] = 0;
                }

                // Process the data only if we are in the correct GUI page.
                unsigned char ucTransmitDataLength;
                if (eGetGuiPage() == PAGE_BLUETOOTH) {
                    ucTransmitDataLength = ucProcessCommandPacket(prv_pucRxBuffer, prv_pucTxBuffer);
                }
                else {
                    ucTransmitDataLength = 0;
                }

                // Send the data if there is any.
                while (ucTransmitDataLength > 0) {

                    // Calculate the checksum.
                    ucCalculatedChecksum = ucTransmitDataLength + 1;
                    for (i = 0; i < ucTransmitDataLength; i++) {
                        ucCalculatedChecksum += prv_pucTxBuffer[i];
                    }
                    ucCalculatedChecksum = ~ucCalculatedChecksum;


                    // Retry until no error.
                    // Exit if timeout.
                    do {
                        // Make sure there is enough space in the UART Tx buffer and transmit the data.
                        while (uiUART2GetTxSpace() < (ucTransmitDataLength + 4)) {
                            vTaskDelay(50 / portTICK_RATE_MS);
                        }

                        // Send the header.
                        ucHeader = 0xff;
                        uiUART2Write(&ucHeader, 1);
                        uiUART2Write(&ucHeader, 1);

                        // Send the data length.
                        ucLength = ucTransmitDataLength + 1;
                        uiUART2Write(&ucLength, 1);

                        // Send the data packet.
                        uiUART2Write(prv_pucTxBuffer, ucTransmitDataLength);

                        // Send the checksum.
                        uiUART2Write(&ucCalculatedChecksum, 1);

                        // Read the acknowledgement.
                        // Break if timeout.
                        if (prv_uiReadBluetooth(&ucReceivedAck, 1, BT_RX_TIMEOUT) < 1) break;
                    } while (ucReceivedAck != BT_ACK_READY);

                    // Process the data again in case there is more data to send.
                    ucTransmitDataLength = ucProcessCommandPacket(prv_pucRxBuffer, prv_pucTxBuffer);
                }
                
            }   // End of latest bluetooth protocol.

        }

    }   // End of "while (1)".

}
