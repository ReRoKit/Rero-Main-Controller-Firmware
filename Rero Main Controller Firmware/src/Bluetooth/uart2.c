/*******************************************************************************
 * This file provides the functions for the UART module.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "Compiler.h"
#include "HardwareProfile.h"
#include "Bluetooth/uart2.h"
#include "variables.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"



/*******************************************************************************
 * PRIVATE CONSTANT DEFINITION
 *******************************************************************************/

#define TX2_BUFF_SIZE        80
#define RX2_BUFF_SIZE        80



/*******************************************************************************
 * PRIVATE GLOBAL VARIABLES
 *******************************************************************************/

// Buffer and pointer for UART.
static volatile unsigned char prv_pucTxBuffer[TX2_BUFF_SIZE] = {0};
static volatile unsigned char prv_pucRxBuffer[RX2_BUFF_SIZE] = {0};

static volatile UART_BUFFER prv_xTx = {prv_pucTxBuffer, sizeof(prv_pucTxBuffer), 0};
static volatile UART_BUFFER prv_xRx = {prv_pucRxBuffer, sizeof(prv_pucRxBuffer), 0};


/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 *******************************************************************************/

static void prv_vIncPointer(volatile unsigned int *puiPointer, unsigned int uiSize);



/*******************************************************************************
 * FUNCTION: prv_vIncPointer
 *
 * PARAMETERS:
 * ~ puiPointer	- The pointer that we want to increase.
 * ~ uiSize	- Size of the buffer pointed by the pointer.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Increase the pointer. Wrap around if overflow.
 *
 *******************************************************************************/
static void prv_vIncPointer(volatile unsigned int *puiPointer, unsigned int uiSize)
{
    if (++*puiPointer >= uiSize) {
        *puiPointer = 0;
    }
}



/*******************************************************************************
 * FUNCTION: vUART2Init
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Initialize the UART2 module.
 * UART 2 is used by bluetooth module.
 *
 *******************************************************************************/
void vUART2Init(void)
{
    // Configure UART 2.
    UARTConfigure(UART2, UART_ENABLE_HIGH_SPEED | UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(UART2, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART2, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART2, GetPeripheralClock(), BT2_HC05_BAUDRATE);
    UARTEnable(UART2, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

    // Configure the interrupt.
    INTSetVectorPriority(INT_UART_2_VECTOR, INT_PRIORITY_LEVEL_7);

    INTClearFlag(INT_U2TX);
    INTClearFlag(INT_U2RX);
    INTClearFlag(INT_U2E);

    INTEnable(INT_U2RX, INT_ENABLED);
    INTEnable(INT_U2E, INT_ENABLED);
}



/*******************************************************************************
 * FUNCTION: vUART2Init
 *
 * PARAMETERS:
 * ~ ulBaudRate - Baud Rate (bps).
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Set the baud rate for UART2.
 *
 *******************************************************************************/
void vUART2SetBaudRate(unsigned long ulBaudRate)
{
    UARTSetDataRate(UART2, GetPeripheralClock(), ulBaudRate);
}



/*******************************************************************************
 * FUNCTION: vUART2FlushRxBuffer
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Flush all the data in the Rx buffer.
 *
 *******************************************************************************/
void vUART2FlushRxBuffer(void)
{
    unsigned char ucReceivedData;

    unsigned int iStatus = INTDisableInterrupts();
    
    prv_xRx.uiDataCount = 0;
    prv_xRx.uiReadPt = 0;
    prv_xRx.uiWritePt = 0;
    
    // Discard all data available in the UART receive buffer.
    while (UARTReceivedDataIsAvailable(UART2)) {
        // Read the received data.
        ucReceivedData = UARTGetDataByte(UART2);
    }

    // Clear the overrun flag.
    if (UARTGetLineStatus(UART2) & UART_OVERRUN_ERROR) {
        U2STAbits.OERR = 0;
    }

    // Clear the semaphore.
    xSemaphoreTake(xBluetoothRxSemaphore, 0);

    INTRestoreInterrupts(iStatus);
}



/*******************************************************************************
 * FUNCTION: uiUART2GetTxSpace
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ Remaining space in TX buffer.
 *
 * DESCRIPTIONS:
 * Read the remaining empty space in the TX buffer.
 *
 *******************************************************************************/
volatile unsigned int uiUART2GetTxSpace(void)
{
    unsigned int iStatus = INTDisableInterrupts();
    unsigned int uiTxSpace = prv_xTx.uiBufferSize - prv_xTx.uiDataCount;
    INTRestoreInterrupts(iStatus);

    return uiTxSpace;
}



/*******************************************************************************
 * FUNCTION: uiUART2GetRxDataCount
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ Number of bytes available.
 *
 * DESCRIPTIONS:
 * Read the number of bytes available in the Rx buffer.
 *
 *******************************************************************************/
volatile unsigned int uiUART2GetRxDataCount(void)
{
    unsigned int iStatus = INTDisableInterrupts();
    unsigned int uiDataCount = prv_xRx.uiDataCount;
    INTRestoreInterrupts(iStatus);

    return uiDataCount;
}



/*******************************************************************************
 * FUNCTION: uiUART2Read
 *
 * PARAMETERS:
 * ~ *pucBuffer	- Buffer to store the received data.
 * ~ uiLength	- Number of bytes to read.
 *
 * RETURN:
 * ~ Number of bytes read.
 *
 * DESCRIPTIONS:
 * Read the data from UART.
 *
 *******************************************************************************/
unsigned int uiUART2Read(unsigned char *pucBuffer, unsigned char uiLength)
{
    unsigned int uiActualLength = 0;

    while (uiLength-- > 0) {
        // Make sure there is data available in the buffer.
        if (uiUART2GetRxDataCount() > 0) {
            // Copy the data to the buffer.
            *pucBuffer++ = prv_xRx.pucBuffer[prv_xRx.uiReadPt];

            // Increase the number of data read.
            uiActualLength++;

            // Increase the read pointer and decrease the data count.
            prv_vIncPointer(&prv_xRx.uiReadPt, prv_xRx.uiBufferSize);

            unsigned int iStatus = INTDisableInterrupts();
            prv_xRx.uiDataCount--;
            INTRestoreInterrupts(iStatus);
        }	
        else {
            break;
        }
    }

    return uiActualLength;
}



/*******************************************************************************
 * FUNCTION: uiUART2Write
 *
 * PARAMETERS:
 * ~ *pucBuffer	- Buffer for the data to send.
 * ~ uiLength	- Number of bytes to send.
 *
 * RETURN:
 * ~ Number of bytes sent.
 *
 * DESCRIPTIONS:
 * Transmit data via UART.
 *
 *******************************************************************************/
unsigned int uiUART2Write(const unsigned char *pucBuffer, unsigned char uiLength)
{
    unsigned char ucActualLength = 0;

    while (uiLength-- > 0) {
        // Make sure there is empty space in the buffer.
        if (uiUART2GetTxSpace() > 0) {

            // Copy the data to the buffer.
            prv_xTx.pucBuffer[prv_xTx.uiWritePt] = *pucBuffer++;

            // Increase the number of transmitted data.
            ucActualLength++;

            // Increase the write pointer and data count.
            prv_vIncPointer(&prv_xTx.uiWritePt, prv_xTx.uiBufferSize);

            unsigned int iStatus = INTDisableInterrupts();
            prv_xTx.uiDataCount++;
            INTRestoreInterrupts(iStatus);
        }

        else {
            break;
        }
    }

    // Enable the Tx interrupt if there is new data.
    if (ucActualLength > 0) {
        INTEnable(INT_U2TX, INT_ENABLED);
    }

    return ucActualLength;
}



/*******************************************************************************
 * ISR: UART 2 Interrupt.
 *
 * DESCRIPTIONS:
 * Interrupt vector for UART 2.
 *
 *******************************************************************************/
void __ISR(_UART_2_VECTOR, IPL7AUTO) UART2Interrupt(void)
{
    unsigned char ucReceivedData;

    xSystemState.bBluetoothBusy = 1;


    // Rx interrupt.
    if (INTGetEnable(INT_U2RX) && INTGetFlag(INT_U2RX)) {
        INTClearFlag(INT_U2RX);

        // Read out all data available.
        while (UARTReceivedDataIsAvailable(UART2)) {
            // Read the received data.
            ucReceivedData = UARTGetDataByte(UART2);

            // Make sure there is empty space in the buffer.
            if ((prv_xRx.uiBufferSize - prv_xRx.uiDataCount) > 0) {

                // Copy the data to the buffer.
                prv_xRx.pucBuffer[prv_xRx.uiWritePt] = ucReceivedData;

                // Increase the write pointer and data count.
                prv_vIncPointer(&prv_xRx.uiWritePt, prv_xRx.uiBufferSize);
                prv_xRx.uiDataCount++;

                portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
                xSemaphoreGiveFromISR(xBluetoothRxSemaphore, &xHigherPriorityTaskWoken);
            }
            else {
                xSystemError.bBluetoothError = 1;
            }
        }
    }



    // Tx interrupt.
    if (INTGetEnable(INT_U2TX) && INTGetFlag(INT_U2TX)) {
        // Loop until the Tx buffer is fully filled.
        while (UARTTransmitterIsReady(UART2)) {
            // If there is data to transmit...
            if (prv_xTx.uiDataCount > 0) {
                // Shift in the data to the transmit buffer.
                UARTSendDataByte(UART2, prv_xTx.pucBuffer[prv_xTx.uiReadPt]);

                // Increase the read pointer and decrease the data count.
                prv_vIncPointer(&prv_xTx.uiReadPt, prv_xTx.uiBufferSize);
                prv_xTx.uiDataCount--;
            }

            // Else, disable the transmit interrupt.
            else {
                INTEnable(INT_U2TX, INT_DISABLED);
                break;
            }
        }
    }



    // Error Interrupt.
    if (INTGetEnable(INT_U2E) && INTGetFlag(INT_U2E)) {
        INTClearFlag(INT_U2E);

        // Discard all data available.
        while (UARTReceivedDataIsAvailable(UART2)) {
            // Read the received data.
            ucReceivedData = UARTGetDataByte(UART2);
        }

        // Clear the overrun flag.
        if (UARTGetLineStatus(UART2) & UART_OVERRUN_ERROR) {
            U2STAbits.OERR = 0;
        }

        xSystemError.bBluetoothError = 1;
    }

}
