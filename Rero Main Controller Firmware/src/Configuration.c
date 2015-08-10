/*******************************************************************************
 * This file handles reading/writing the configuration to the SD Card.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "Configuration.h"
#include "Variables.h"
#include "HardwareProfile.h"
#include "FatFs/FatFsWrapper.h"
#include "IMU/imu.h"
#include "IMU/inv_mpu.h"
#include "IMU/inv_mpu_dmp_motion_driver.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
 * FUNCTION: vInitConfig
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Read all the configuration from the files.
 * If the file is empty or not found, write the default value to it.
 *
 *******************************************************************************/
void vInitConfig(void)
{
    unsigned short usActualLength;
    
    
    
    // Read the robot name.
    // Write the default value if the file is not found or empty.
    // Else, terminate the string with null.
    usActualLength = usReadConfig(pucRobotNameFilePath, (void*)&pucRobotName, sizeof(pucRobotName) - 1);
    if (usActualLength == 0) {
        usWriteConfig(pucRobotNameFilePath, (void*)&pucRobotName, strlen(pucRobotName));
    } else {
        pucRobotName[usActualLength] = 0;
    }
    
    

    // Read the backlight.
    // Write the default value if the file is not found or empty.
    usActualLength = usReadConfig(pucBacklightFilePath, (void*)&BLight, sizeof(BLight));
    if (usActualLength == 0) {
        usWriteConfig(pucBacklightFilePath, (void*)&BLight, sizeof(BLight));
    }
    
    // Set the backlight.
    SetDCOC1PWM(BLight);
    
    
    
    // Read the volume.
    // Write the default value if the file is not found or empty.
    usActualLength = usReadConfig(pucVolumeFilePath, (void*)&ucVolume, sizeof(ucVolume));
    if (usActualLength == 0) {
        usWriteConfig(pucVolumeFilePath, (void*)&ucVolume, sizeof(ucVolume));
    }
    
    // Set the volume.
    vSetVolume(ucVolume);
    
    
    
    // Read the Bluetooth mode.
    // Write the default value if the file is not found or empty.
    usActualLength = usReadConfig(pucBt4ModeFilePath, (void*)&eBt4Mode, sizeof(eBt4Mode));
    if (usActualLength == 0) {
        usWriteConfig(pucBt4ModeFilePath, (void*)&eBt4Mode, sizeof(eBt4Mode));
    }
    
    
    
    // Read the auto sleep setting.
    // Write the default value if the file is not found or empty.
    usActualLength = usReadConfig(pucSleepFilePath, (void*)&eAutoSleep, sizeof(eAutoSleep));
    if (usActualLength == 0) {
        usWriteConfig(pucSleepFilePath, (void*)&eAutoSleep, sizeof(eAutoSleep));
    }
    
    
    
    // Read the auto shutdown setting.
    // Write the default value if the file is not found or empty.
    usActualLength = usReadConfig(pucShutdownFilePath, (void*)&eAutoShutdown, sizeof(eAutoShutdown));
    if (usActualLength == 0) {
        usWriteConfig(pucShutdownFilePath, (void*)&eAutoShutdown, sizeof(eAutoShutdown));
    }
#ifdef SHOWCASE_ROBOT
    // Do not auto shutdown for showcase robot.
    eAutoShutdown = AUTO_SHUTDOWN_NEVER;
#endif
    
    
    
#ifdef ENABLE_IMU
    // Read the IMU calibration.
    // Run the IMU calibration if the file is not found or empty.
    
    long lImuBias[6] = {0};
    usActualLength = usReadConfig(pucImuCalFilePath, (void*)lImuBias, sizeof(lImuBias));
    if (usActualLength == 0) {
        // Run the self test and make sure it passes.
        if (mpu_run_self_test(&lImuBias[0], &lImuBias[3]) == 0x7) {
            // Save the bias to the file.
            usWriteConfig(pucImuCalFilePath, (void*)lImuBias, sizeof(lImuBias));
        }
    }
    // Convert gyro bias to +-1000dps.
    lImuBias[0] = (long)((float)lImuBias[0] * 32.8f);
    lImuBias[0] = lImuBias[0] >> 16;

    lImuBias[1] = (long)((float)lImuBias[1] * 32.8f);
    lImuBias[1] = lImuBias[1] >> 16;

    lImuBias[2] = (long)((float)lImuBias[2] * 32.8f);
    lImuBias[2] = lImuBias[2] >> 16;

    // Convert accelerometer bias to +-8G.
    lImuBias[3] *= 2048L;
    lImuBias[3] = lImuBias[3] >> 16;

    lImuBias[4] *= 2048L;
    lImuBias[4] = lImuBias[4] >> 16;

    lImuBias[5] *= 2048L;
    lImuBias[5] = lImuBias[5] >> 16;

    // Calibrate the IMU.
    mpu_set_gyro_bias_reg(&lImuBias[0]);
    mpu_set_accel_bias_6050_reg(&lImuBias[3]);
#endif
}



/*******************************************************************************
 * FUNCTION: usReadConfig
 *
 * PARAMETERS:
 * ~ szConfigFilePath   - Path to the configuration file.
 * ~ pvBuffer           - Buffer to store the configuration data.
 * ~ usLength           - Length of the configuration data.
 *
 * RETURN:
 * ~ Number of bytes read from the configuration file.
 *
 * DESCRIPTIONS:
 * Read the configuration data from the file.
 *
 *******************************************************************************/
unsigned short usReadConfig(const char *szConfigFilePath, void *pvBuffer, unsigned short usLength)
{   
    unsigned short usActualLength = 0;
    
    // Take control of SD card if RTOS is running.
    if (xSystemState.bRtosRunning == 1) {
        xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
    }
    
    // Open the config file for read.
    FSFILE *pxConfigFile = FSfopen(szConfigFilePath, "r");
    
    // Make sure the file is opened successfully.
    if (pxConfigFile != NULL) {
        // Read the configuration and close the file.
        usActualLength = FSfread(pvBuffer, 1, usLength, pxConfigFile);
        FSfclose(pxConfigFile);
    }
    
    // Release control of SD card if RTOS is running.
    if (xSystemState.bRtosRunning == 1) {
        xSemaphoreGive(xSdCardMutex);
    }
    
    return usActualLength;
}



/*******************************************************************************
 * FUNCTION: usWriteConfig
 *
 * PARAMETERS:
 * ~ szConfigFilePath   - Path to the configuration file.
 * ~ pvBuffer           - The configuration data.
 * ~ usLength           - Length of the configuration data.
 *
 * RETURN:
 * ~ Number of bytes written to the configuration file.
 *
 * DESCRIPTIONS:
 * Write the configuration data to the file.
 *
 *******************************************************************************/
unsigned short usWriteConfig(const char *szConfigFilePath, void *pvBuffer, unsigned short usLength)
{
    unsigned short usActualLength = 0;
    
    // Take control of SD card if RTOS is running.
    if (xSystemState.bRtosRunning == 1) {
        xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
    }
    
    // Open the config file.
    FSFILE *pxConfigFile = FSfopen(szConfigFilePath, "w");
    
    // Make sure the file is opened successfully.
    if (pxConfigFile != NULL) {
        // Write the configuration and close the file.
        usActualLength = FSfwrite(pvBuffer, 1, usLength, pxConfigFile);
        FSfclose(pxConfigFile);
    }
    
    // Release control of SD card if RTOS is running.
    if (xSystemState.bRtosRunning == 1) {
        xSemaphoreGive(xSdCardMutex);
    }
    
    return usActualLength;
}
