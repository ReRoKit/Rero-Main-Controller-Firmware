/*******************************************************************************
 * This file provides the functions for the I2C module.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include <peripheral/i2c.h>
#include "Compiler.h"
#include "HardwareProfile.h"
#include "IMU/i2c1.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
 * FUNCTION: vI2C1Init
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Initialize the I2C module.
 * I2C is used to communicate with the IMU.
 *
 *******************************************************************************/
void vI2C1Init(void)
{
    I2CConfigure(I2C1, I2C_ENABLE_HIGH_SPEED);
    I2CSetFrequency(I2C1, GetPeripheralClock(), I2C1_CLOCK_FREQ);
    I2CEnable(I2C1, TRUE);
}



/*******************************************************************************
 * FUNCTION: ucI2C1Write
 *
 * PARAMETERS:
 * ~ ucI2cAddress	- The I2C slave address.
 * ~ ucRegAddress	- The register address to write.
 * ~ ucBytesCount	- Number of bytes to write.
 * ~ pucBuffer		- Buffer for the data.
 *
 * RETURN:
 * ~ Return 1 if error occured.
 * ~ Return 0 if no error.
 *
 * DESCRIPTIONS:
 * Write data to I2C slave.
 *
 *******************************************************************************/
unsigned char ucI2C1Write(unsigned char ucI2cAddress, unsigned char ucRegAddress, unsigned char ucBytesCount, unsigned char* pucBuffer)
{
    I2C_RESULT eI2CResult;


    // Suspend all other task.
    vTaskSuspendAll();
    

    // Make sure the I2C bus is idle.
    if (I2CBusIsIdle(I2C1) == FALSE) {
        // Send stop bit and return as error.
        INTClearFlag(INT_I2C1M);
        I2CStop(I2C1);
        while (INTGetFlag(INT_I2C1M) == 0);     // Wait for completion.

        // Resume all task.
        xTaskResumeAll();
        return 1;
    }


    // Send start bit.
    INTClearFlag(INT_I2C1M);
    eI2CResult = I2CStart(I2C1);
    while (INTGetFlag(INT_I2C1M) == 0);     // Wait for completion.

    // Make sure no error.
    if (eI2CResult != I2C_SUCCESS) {
        // Send stop bit and return as error.
        INTClearFlag(INT_I2C1M);
        I2CStop(I2C1);
        while (INTGetFlag(INT_I2C1M) == 0);     // Wait for completion.

        // Resume all task.
        xTaskResumeAll();
        return 1;
    }


    // Wait until transmitter is ready.
    while (I2CTransmitterIsReady(I2C1) == FALSE);
    
    // Send slave address and indicate to write.
    eI2CResult = I2CSendByte(I2C1, (ucI2cAddress << 1) | I2C_WRITE);

    // Wait until transmission has completed.
    while (I2CTransmissionHasCompleted(I2C1) == FALSE);

    // Make sure no error and slave acknowledged.
    if ((eI2CResult != I2C_SUCCESS) || (I2CByteWasAcknowledged(I2C1) == FALSE)) {
        // Send stop bit and return as error.
        INTClearFlag(INT_I2C1M);
        I2CStop(I2C1);
        while (INTGetFlag(INT_I2C1M) == 0);     // Wait for completion.

        // Resume all task.
        xTaskResumeAll();
        return 1;
    }


    // Send register address.
    eI2CResult = I2CSendByte(I2C1, ucRegAddress);

    // Wait until transmission has completed.
    while (I2CTransmissionHasCompleted(I2C1) == FALSE);

    // Make sure no error and slave acknowledged.
    if ((eI2CResult != I2C_SUCCESS) || (I2CByteWasAcknowledged(I2C1) == FALSE)) {
        // Send stop bit and return as error.
        INTClearFlag(INT_I2C1M);
        I2CStop(I2C1);
        while (INTGetFlag(INT_I2C1M) == 0);     // Wait for completion.

        // Resume all task.
        xTaskResumeAll();
        return 1;
    }


    // Send data.
    while (ucBytesCount-- > 0) {
        // Send out one byte.
        eI2CResult = I2CSendByte(I2C1, *pucBuffer++);

        // Wait until transmission has completed.
        while (I2CTransmissionHasCompleted(I2C1) == FALSE);

        // Make sure no error and slave acknowledged.
        if ((eI2CResult != I2C_SUCCESS) || (I2CByteWasAcknowledged(I2C1) == FALSE)) {
            // Send stop bit and return as error.
            INTClearFlag(INT_I2C1M);
            I2CStop(I2C1);
            while (INTGetFlag(INT_I2C1M) == 0);     // Wait for completion.

            // Resume all task.
            xTaskResumeAll();
            return 1;
        }
    }


    // Send stop bit and return 0.
    INTClearFlag(INT_I2C1M);
    I2CStop(I2C1);
    while (INTGetFlag(INT_I2C1M) == 0);     // Wait for completion.

    // Resume all task.
    xTaskResumeAll();
    return 0;
}



/*******************************************************************************
 * FUNCTION: ucI2C1Read
 *
 * PARAMETERS:
 * ~ ucI2cAddress	- The I2C slave address.
 * ~ ucRegAddress	- The register address to read.
 * ~ ucBytesCount	- Number of bytes to read.
 * ~ pucBuffer		- Buffer for the data.
 *
 * RETURN:
 * ~ Return 1 if error occured.
 * ~ Return 0 if no error.
 *
 * DESCRIPTIONS:
 * Read data from I2C slave.
 *
 *******************************************************************************/
unsigned char ucI2C1Read(unsigned char ucI2cAddress, unsigned char ucRegAddress, unsigned char ucBytesCount, unsigned char* pucBuffer)
{
    I2C_RESULT eI2CResult;


    // Suspend all other task.
    vTaskSuspendAll();


    // Make sure the I2C bus is idle.
    if (I2CBusIsIdle(I2C1) == FALSE) {
        // Send stop bit and return as error.
        INTClearFlag(INT_I2C1M);
        I2CStop(I2C1);
        while (INTGetFlag(INT_I2C1M) == 0);     // Wait for completion.

        // Resume all task.
        xTaskResumeAll();
        return 1;
    }


    // Send start bit.
    INTClearFlag(INT_I2C1M);
    eI2CResult = I2CStart(I2C1);
    while (INTGetFlag(INT_I2C1M) == 0);     // Wait for completion.

    // Make sure no error.
    if (eI2CResult != I2C_SUCCESS) {
        // Send stop bit and return as error.
        INTClearFlag(INT_I2C1M);
        I2CStop(I2C1);
        while (INTGetFlag(INT_I2C1M) == 0);     // Wait for completion.

        // Resume all task.
        xTaskResumeAll();
        return 1;
    }


    // Wait until transmitter is ready.
    while (I2CTransmitterIsReady(I2C1) == FALSE);

    // Send slave address and indicate to write.
    eI2CResult = I2CSendByte(I2C1, (ucI2cAddress << 1) | I2C_WRITE);

    // Wait until transmission has completed.
    while (I2CTransmissionHasCompleted(I2C1) == FALSE);

    // Make sure no error and slave acknowledged.
    if ((eI2CResult != I2C_SUCCESS) || (I2CByteWasAcknowledged(I2C1) == FALSE)) {
        // Send stop bit and return as error.
        INTClearFlag(INT_I2C1M);
        I2CStop(I2C1);
        while (INTGetFlag(INT_I2C1M) == 0);     // Wait for completion.

        // Resume all task.
        xTaskResumeAll();
        return 1;
    }


    // Send register address
    eI2CResult = I2CSendByte(I2C1, ucRegAddress);

    // Wait until transmission has completed.
    while (I2CTransmissionHasCompleted(I2C1) == FALSE);

    // Make sure no error and slave acknowledged.
    if ((eI2CResult != I2C_SUCCESS) || (I2CByteWasAcknowledged(I2C1) == FALSE)) {
        // Send stop bit and return as error.
        INTClearFlag(INT_I2C1M);
        I2CStop(I2C1);
        while (INTGetFlag(INT_I2C1M) == 0);     // Wait for completion.

        // Resume all task.
        xTaskResumeAll();
        return 1;
    }



    // Send repeated start.
    INTClearFlag(INT_I2C1M);
    eI2CResult = I2CRepeatStart(I2C1);
    while (INTGetFlag(INT_I2C1M) == 0);     // Wait for completion.

    // Make sure no error.
    if (eI2CResult != I2C_SUCCESS) {
        // Send stop bit and return as error.
        INTClearFlag(INT_I2C1M);
        I2CStop(I2C1);
        while (INTGetFlag(INT_I2C1M) == 0);     // Wait for completion.

        // Resume all task.
        xTaskResumeAll();
        return 1;
    }


    // Send slave address and indicate to read.
    eI2CResult = I2CSendByte(I2C1, (ucI2cAddress << 1) | I2C_READ);

    // Wait until transmission has completed.
    while (I2CTransmissionHasCompleted(I2C1) == FALSE);

    // Make sure no error and slave acknowledged.
    if ((eI2CResult != I2C_SUCCESS) || (I2CByteWasAcknowledged(I2C1) == FALSE)) {
        // Send stop bit and return as error.
        INTClearFlag(INT_I2C1M);
        I2CStop(I2C1);
        while (INTGetFlag(INT_I2C1M) == 0);     // Wait for completion.

        // Resume all task.
        xTaskResumeAll();
        return 1;
    }


    // Read data.
    while (ucBytesCount-- > 0) {
        // Enable reception.
        eI2CResult = I2CReceiverEnable(I2C1, TRUE);

        // Make sure no error.
        if (eI2CResult != I2C_SUCCESS) {
            // Send stop bit and return as error.
            INTClearFlag(INT_I2C1M);
            I2CStop(I2C1);
            while (INTGetFlag(INT_I2C1M) == 0);     // Wait for completion.

            // Resume all task.
            xTaskResumeAll();
            return 1;
        }


        // Wait until data is received.
        while (I2CReceivedDataIsAvailable(I2C1) == FALSE);

        // Send ACK/NACK.
        if (ucBytesCount > 0) {
            I2CAcknowledgeByte(I2C1, TRUE);
        }
        else {
            I2CAcknowledgeByte(I2C1, FALSE);
        }

        // Read the data.
        *pucBuffer++ = I2CGetByte(I2C1);

        // Wait until the ACK/NACK is sent.
        while (I2CAcknowledgeHasCompleted(I2C1) == FALSE);
    }

    
    // Send stop bit and return 0.
    INTClearFlag(INT_I2C1M);
    I2CStop(I2C1);
    while (INTGetFlag(INT_I2C1M) == 0);     // Wait for completion.

    // Resume all task.
    xTaskResumeAll();
    return 0;
}
