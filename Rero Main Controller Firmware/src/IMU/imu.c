/*******************************************************************************
 * This file provides the functions for the IMU module.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include <math.h>
#include "IMU/imu.h"
#include "HardwareProfile.h"
#include "Variables.h"
#include "IMU/inv_mpu.h"
#include "IMU/inv_mpu_dmp_motion_driver.h"
#include "Log.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
 * FUNCTION: ssInitIMU
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ Return 0 if successful.
 *
 * DESCRIPTIONS:
 * Initialized the IMU.
 *
 *******************************************************************************/
signed short ssInitIMU(void)
{
    // Initialize the IMU. Return if failed.
    signed short ssErrorCode = mpu_init(NULL);
    if (ssErrorCode != 0) {
        return ssErrorCode;
    }

    // Wake up all sensors.
    mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);

    // Push both gyro and accel data into the FIFO.
    mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);

    /* To initialize the DMP:
     * 1. Call dmp_load_motion_driver_firmware(). This pushes the DMP image in
     *    inv_mpu_dmp_motion_driver.h into the MPU memory.
     * 2. Push the gyro and accel orientation matrix to the DMP.
     * 3. Register gesture callbacks. Don't worry, these callbacks won't be
     *    executed unless the corresponding feature is enabled.
     * 4. Call dmp_enable_feature(mask) to enable different features.
     * 5. Call dmp_set_fifo_rate(freq) to select a DMP output rate.
     * 6. Call any feature-specific control functions.
     *
     * To enable the DMP, just call mpu_set_dmp_state(1). This function can
     * be called repeatedly to enable and disable the DMP at runtime.
     *
     * The following is a short summary of the features supported in the DMP
     * image provided in inv_mpu_dmp_motion_driver.c:
     * DMP_FEATURE_LP_QUAT: Generate a gyro-only quaternion on the DMP at
     * 200Hz. Integrating the gyro data at higher rates reduces numerical
     * errors (compared to integration on the MCU at a lower sampling rate).
     * DMP_FEATURE_6X_LP_QUAT: Generate a gyro/accel quaternion on the DMP at
     * 200Hz. Cannot be used in combination with DMP_FEATURE_LP_QUAT.
     * DMP_FEATURE_TAP: Detect taps along the X, Y, and Z axes.
     * DMP_FEATURE_ANDROID_ORIENT: Google's screen rotation algorithm. Triggers
     * an event at the four orientations where the screen should rotate.
     * DMP_FEATURE_GYRO_CAL: Calibrates the gyro data after eight seconds of
     * no motion.
     * DMP_FEATURE_SEND_RAW_ACCEL: Add raw accelerometer data to the FIFO.
     * DMP_FEATURE_SEND_RAW_GYRO: Add raw gyro data to the FIFO.
     * DMP_FEATURE_SEND_CAL_GYRO: Add calibrated gyro data to the FIFO. Cannot
     * be used in combination with DMP_FEATURE_SEND_RAW_GYRO.
     */
    dmp_load_motion_driver_firmware();
    /*
     * Known Bug -
     * DMP when enabled will sample sensor data at 200Hz and output to FIFO at the rate
     * specified in the dmp_set_fifo_rate API. The DMP will then sent an interrupt once
     * a sample has been put into the FIFO. Therefore if the dmp_set_fifo_rate is at 25Hz
     * there will be a 25Hz interrupt from the MPU device.
     *
     * There is a known issue in which if you do not enable DMP_FEATURE_TAP
     * then the interrupts will be at 200Hz even if fifo rate
     * is set at a different rate. To avoid this issue include the DMP_FEATURE_TAP
     */
    dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP | DMP_FEATURE_GYRO_CAL | DMP_FEATURE_SEND_CAL_GYRO | DMP_FEATURE_SEND_RAW_ACCEL);
    dmp_set_fifo_rate(10);
    mpu_set_dmp_state(1);
}



/*******************************************************************************
 * FUNCTION: ucCalibrateIMU
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ Return 0 if self test passed and IMU is calibrated.
 * ~ Return 1 if failed.
 *
 * DESCRIPTIONS:
 * Run the self test and calibrate the gyro and accelerometer.
 * This must be called when the controller is level and motionless.
 *
 *******************************************************************************/
unsigned char ucCalibrateIMU(void)
{
    long lGyro[3];
    long lAcc[3];

    // Run the self test and make sure it passes.
    if (mpu_run_self_test(lGyro, lAcc) == 0x7) {
        unsigned char i;
        for(i = 0; i < 3; i++) {
            // Convert gyro bias to +-1000dps.
            lGyro[i] = (long)((float)lGyro[i] * 32.8f);
            lGyro[i] = lGyro[i] >> 16;

            // Convert accelerometer bias to +-8G.
            lAcc[i] *= 2048L;
            lAcc[i] = lAcc[i] >> 16;
        }

        mpu_set_gyro_bias_reg(lGyro);
        mpu_set_accel_bias_6050_reg(lAcc);

        return 0;
    }
    // Self test failed.
    else {
        return 1;
    }
}



/*******************************************************************************
 * TASK: taskImu
 *
 * DESCRIPTIONS:
 * Task for processing the IMU data.
 *
 *******************************************************************************/
void taskImu(void *pvParameters)
{
    short sGyro[3] = {0};
    short sAcc[3] = {0};
    long lQuaternion[4] = {0};
    unsigned long ulTimestamp;
    short sSensors;
    unsigned char ucMore;
    
    
    // Get the sensor sensitivity.
    unsigned short usAccSensitivity = 0;
    float fGyroSensitivity = 0.0f;
    mpu_get_accel_sens(&usAccSensitivity);
    mpu_get_gyro_sens(&fGyroSensitivity);


    // Get the current tick time.
    portTickType prevTick = xTaskGetTickCount();

    while (1) {
//        if (IMU_INT == 0) {
            // Read out all the data available.
            do {
                if (dmp_read_fifo(sGyro, sAcc, lQuaternion, &ulTimestamp, &sSensors, &ucMore) != 0) {
                    // TODO: IMU Error.
                }

                else {
                    // Gyro data is available.
                    if (sSensors & INV_XYZ_GYRO) {
                        // Scale the gyro reading to deg/s.
                        fGyroX = (float)sGyro[0] / fGyroSensitivity;
                        fGyroY = (float)sGyro[1] / fGyroSensitivity;
                        fGyroZ = (float)sGyro[2] / fGyroSensitivity;
                    }

                    // Accelerometer data is available.
                    if (sSensors & INV_XYZ_ACCEL) {
                        // Scale the accelerometers reading to G.
                        fAccelerometerX = (float)sAcc[0] / (float)usAccSensitivity;
                        fAccelerometerY = (float)sAcc[1] / (float)usAccSensitivity;
                        fAccelerometerZ = (float)sAcc[2] / (float)usAccSensitivity;

//                        // Calculate tilt angle for X-Axis.
//                        float fSumYZ = sqrtf((fAccelerometerY * fAccelerometerY) + (fAccelerometerZ * fAccelerometerZ));
//                        float fAngle = atan2f(fAccelerometerX, fSumYZ);
//                        fAccTiltAngleX = fAngle * 180.0f / 3.14159f;
//
//                        // Calculate tilt angle for Y-Axis.
//                        float fSumXZ = sqrtf((fAccelerometerX * fAccelerometerX) + (fAccelerometerZ * fAccelerometerZ));
//                        fAngle = atan2f(fAccelerometerY, fSumXZ);
//                        fAccTiltAngleY = fAngle * 180.0f / 3.14159f;
//
//                        // Calculate tilt angle for Z-Axis.
//                        float fSumXY = sqrtf((fAccelerometerX * fAccelerometerX) + (fAccelerometerY * fAccelerometerY));
//                        fAngle = atan2f(fAccelerometerZ, fSumXY);
//                        fAccTiltAngleZ = fAngle * 180.0f / 3.14159f;
                    }

                    // Quaternion data is available.
                    if (sSensors & INV_WXYZ_QUAT) {
                        // Calculate the magnitude.
                        float fQMag = (float)(1L << 30);
//                        float fQMag = (float)lQuaternion[0] * (float)lQuaternion[0];
//                        fQMag += (float)lQuaternion[1] * (float)lQuaternion[1];
//                        fQMag += (float)lQuaternion[2] * (float)lQuaternion[2];
//                        fQMag += (float)lQuaternion[3] * (float)lQuaternion[3];
//                        fQMag = sqrtf(fQMag);

                        // Scale the Quarternion.
                        float fQ0 = (float)lQuaternion[0] / fQMag;
                        float fQ1 = (float)lQuaternion[1] / fQMag;
                        float fQ2 = (float)lQuaternion[2] / fQMag;
                        float fQ3 = (float)lQuaternion[3] / fQMag;

                        // Calculate the tilt angle.
                        // The formula is derived from the DCM formula.
                        float fTemp = 2.0f * ((fQ1 * fQ3) - (fQ0 * fQ2));
                        fAccTiltAngleX = asinf(fTemp) * 180.0f / 3.14159f;

                        fTemp = 2.0f * ((fQ2 * fQ3) + (fQ0 * fQ1));
                        fAccTiltAngleY = asinf(fTemp) * 180.0f / 3.14159f;

                        fTemp = (fQ0 * fQ0) - (fQ1 * fQ1) - (fQ2 * fQ2) + (fQ3 * fQ3);
                        fAccTiltAngleZ = asinf(fTemp) * 180.0f / 3.14159f;
                    }
                }
            } while (ucMore > 0);


            
//        }


        // Loop every 100ms.
        vTaskDelayUntil(&prevTick, 100 / portTICK_RATE_MS);

        // Read the stack watermark and record it if it's below threshold.
        unsigned portBASE_TYPE uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        if (uxHighWaterMark < MIN_STACK_WATERMARK) {
            xSystemError.bStackLowError = 1;

            // Only log it when the watermark value changed.
            static portBASE_TYPE uxPreviousWatermark = 0;
            if (uxHighWaterMark != uxPreviousWatermark) {
                vLogStackWatermark("IMU Task", (unsigned short)uxHighWaterMark);
            }
            uxPreviousWatermark = uxHighWaterMark;
        }
    }
}
