/*******************************************************************************
 * This file provides the functions for the UART module.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "Compiler.h"
#include "HardwareProfile.h"
#include "G15 and Sensors/uart5.h"
#include "variables.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

#define TX5_BUFF_SIZE        64
#define RX5_BUFF_SIZE        64



/*******************************************************************************
 * PRIVATE GLOBAL VARIABLES
 *******************************************************************************/

// Buffer and pointer for UART.
static volatile unsigned char prv_pucTxBuffer[TX5_BUFF_SIZE] = {0};
static volatile unsigned char prv_pucRxBuffer[RX5_BUFF_SIZE] = {0};

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
 * FUNCTION: vUART5Init
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Initialize the UART5 module.
 * UART 5 is used to communicate with G15 and sensors.
 *
 *******************************************************************************/
void vUART5Init(void)
{
    // Initialize the data direction control pin.
    // Initialize to Rx mode.
    EXT_UART_DIR = 1;
    EXT_UART_DIR_TRIS = 0;

    // Configure UART 5.
    UARTConfigure(UART5, UART_ENABLE_HIGH_SPEED | UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(UART5, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART5, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART5, GetPeripheralClock(), EXT_UART_BOOSTED_BAUDRATE);
    UARTEnable(UART5, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

    // Configure the interrupt.
    INTSetVectorPriority(INT_UART_5_VECTOR, INT_PRIORITY_LEVEL_7);
    
    INTClearFlag(INT_U5TX);
    INTClearFlag(INT_U5RX);
    INTClearFlag(INT_U5E);

    INTEnable(INT_U5RX, INT_ENABLED);
    INTEnable(INT_U5E, INT_ENABLED);
}



/*******************************************************************************
 * FUNCTION: vUART5SetBaudrate
 *
 * PARAMETERS:
 * ~ ulBaudrate     - New Baudrate for UART 5.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Set the Baudrate for UART 5.
 *
 *******************************************************************************/
void vUART5SetBaudrate(unsigned long ulBaudrate)
{
    UARTSetDataRate(UART5, GetPeripheralClock(), ulBaudrate);
}



/*******************************************************************************
 * FUNCTION: vUART5FlushRxBuffer
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
void vUART5FlushRxBuffer(void)
{
    unsigned char ucReceivedData;

    unsigned int iStatus = INTDisableInterrupts();

    prv_xRx.uiDataCount = 0;
    prv_xRx.uiReadPt = 0;
    prv_xRx.uiWritePt = 0;


    // Discard all data available in the UART receive buffer.
    while (UARTReceivedDataIsAvailable(UART5)) {
        // Read the received data.
        ucReceivedData = UARTGetDataByte(UART5);
    }

    // Clear the overrun flag.
    if (UARTGetLineStatus(UART5) & UART_OVERRUN_ERROR) {
        U5STAbits.OERR = 0;
    }

    INTRestoreInterrupts(iStatus);
}



/*******************************************************************************
 * FUNCTION: uiUART5GetTxSpace
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
unsigned int uiUART5GetTxSpace(void)
{
    unsigned int iStatus = INTDisableInterrupts();
    unsigned int uiTxSpace = prv_xTx.uiBufferSize - prv_xTx.uiDataCount;
    INTRestoreInterrupts(iStatus);

    return uiTxSpace;
}



/*******************************************************************************
 * FUNCTION: uiUART5GetRxDataCount
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
unsigned int uiUART5GetRxDataCount(void)
{
    unsigned int iStatus = INTDisableInterrupts();
    unsigned int uiDataCount = prv_xRx.uiDataCount;
    INTRestoreInterrupts(iStatus);

    return uiDataCount;
}



/*******************************************************************************
 * FUNCTION: uiUART5Read
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
unsigned int uiUART5Read(unsigned char *pucBuffer, unsigned char uiLength)
{
    unsigned int uiActualLength = 0;

    while (uiLength-- > 0) {
        // Make sure there is data available in the buffer.
        if (uiUART5GetRxDataCount() > 0) {
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
 * FUNCTION: uiUART5Write
 *
 * PARAMETERS:
 * ~ *pucBuffer	- Buffer for the data to send.
 * ~ ucLength	- Number of bytes to send.
 *
 * RETURN:
 * ~ Number of bytes sent.
 *
 * DESCRIPTIONS:
 * Transmit data via UART.
 *
 *******************************************************************************/
unsigned int uiUART5Write(unsigned char *pucBuffer, unsigned char ucLength)
{
    unsigned char ucActualLength = 0;

    while (ucLength-- > 0) {
        // Make sure there is empty space in the buffer.
        if (uiUART5GetTxSpace() > 0) {

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
        INTEnable(INT_U5TX, INT_ENABLED);
    }

    return ucActualLength;
}



/*******************************************************************************
 * FUNCTION: ucUART5IsTransmitting
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ 0 - UART is not transmitting.
 * ~ 1 - Transmission is in progress.
 *
 * DESCRIPTIONS:
 * Check whether the UART is transmitting.
 *
 *******************************************************************************/
unsigned char ucUART5IsTransmitting(void)
{
    unsigned char ucResult;

    unsigned int iStatus = INTDisableInterrupts();
    if ((UARTTransmissionHasCompleted(UART5) == FALSE) || (prv_xTx.uiDataCount > 0)) {
        ucResult = 1;
    } else {
        ucResult = 0;
    }
    INTRestoreInterrupts(iStatus);

    return ucResult;
}



/*******************************************************************************
 * ISR: UART 5 Interrupt.
 *
 * DESCRIPTIONS:
 * Interrupt vector for UART 5.
 *
 *******************************************************************************/
void __ISR(_UART_5_VECTOR, IPL7AUTO) UART5Interrupt(void)
{
    unsigned char ucReceivedData;


    // Rx interrupt.
    if (INTGetEnable(INT_U5RX) && INTGetFlag(INT_U5RX)) {
        INTClearFlag(INT_U5RX);

        // Read out all data available.
        while (UARTReceivedDataIsAvailable(UART5)) {
            // Read the received data.
            ucReceivedData = UARTGetDataByte(UART5);

            // Make sure there is empty space in the buffer.
            if ((prv_xRx.uiBufferSize - prv_xRx.uiDataCount) > 0) {

                // Copy the data to the buffer.
                prv_xRx.pucBuffer[prv_xRx.uiWritePt] = ucReceivedData;

                // Increase the write pointer and data count.
                prv_vIncPointer(&prv_xRx.uiWritePt, prv_xRx.uiBufferSize);
                prv_xRx.uiDataCount++;
            }
        }
    }



    // Tx interrupt.
    if (INTGetEnable(INT_U5TX) && INTGetFlag(INT_U5TX)) {
        // Loop until the Tx buffer is fully filled.
        while (UARTTransmitterIsReady(UART5)) {
            // If there is data to transmit...
            if (prv_xTx.uiDataCount > 0) {
                // Shift in the data to the transmit buffer.
                UARTSendDataByte(UART5, prv_xTx.pucBuffer[prv_xTx.uiReadPt]);

                // Increase the read pointer and decrease the data count.
                prv_vIncPointer(&prv_xTx.uiReadPt, prv_xTx.uiBufferSize);
                prv_xTx.uiDataCount--;
            }

            // Else, disable the transmit interrupt.
            else {
                INTEnable(INT_U5TX, INT_DISABLED);
                break;
            }
        }
    }



    // Error Interrupt.
    if (INTGetEnable(INT_U5E) && INTGetFlag(INT_U5E)) {
        INTClearFlag(INT_U5E);

        // Discard all data available.
        while (UARTReceivedDataIsAvailable(UART5)) {
            // Read the received data.
            ucReceivedData = UARTGetDataByte(UART5);
        }

        // Clear the overrun flag.
        if (UARTGetLineStatus(UART5) & UART_OVERRUN_ERROR) {
            U5STAbits.OERR = 0;
        }

        xSystemError.bUartError = 1;
    }

}
