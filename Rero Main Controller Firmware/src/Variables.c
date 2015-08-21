/*******************************************************************************
 * This file declares all the global variables.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "Variables.h"



/*******************************************************************************
* PUBLIC GLOBAL VARIABLES                                                      *
*******************************************************************************/

volatile SYSTEM_STATE xSystemState = {0};
volatile SYSTEM_ERROR xSystemError = {0};


// Battery percentage.
// Initialize the battery percentage to invalid value during startup.
volatile unsigned int uiBattPercentage = 200;

// LCD backlight value.
volatile unsigned short BLight = 6500;

// Volume value.
volatile unsigned char ucVolume = 30;

// Bluetooth 4.0 mode.
volatile BT4_MODE eBt4Mode = EDR;

// Auto sleep value.
volatile AUTO_SLEEP_SETTING eAutoSleep = AUTO_SLEEP_2_MIN;

// Auto shutdown value.
volatile AUTO_SHUTDOWN_SETTING eAutoShutdown = AUTO_SHUTDOWN_10_MIN;



// Gyro data (deg/s).
volatile float fGyroX = 0;
volatile float fGyroY = 0;
volatile float fGyroZ = 0;

// Accelerometer data (G).
volatile float fAccelerometerX = 0;
volatile float fAccelerometerY = 0;
volatile float fAccelerometerZ = 0;

// Tilt angle calculated from accelerometer data (degree).
volatile float fAccTiltAngleX = 0;
volatile float fAccTiltAngleY = 0;
volatile float fAccTiltAngleZ = 0;


// RTOS queue, semaphore and mutex.
xQueueHandle xTouchMsgQueue;
xSemaphoreHandle xTouchSemaphore;
xSemaphoreHandle xBluetoothRxSemaphore;
xSemaphoreHandle xPlayingMotionSemaphore;
xSemaphoreHandle xSeekExtModuleSemaphore;
xSemaphoreHandle xLoadAudioBufferSemaphore;
xSemaphoreHandle xSdCardMutex;
xSemaphoreHandle xExternalUartMutex;
xSemaphoreHandle xBluetoothMutex;



// Robot name.
unsigned char pucRobotName[20] = "rero";

// Bluetooth address.
char szBluetoothAddress[20] = {0};

// Firmware version.
const char szFirmwareVersion[] = __DATE__;

// Files path.
const unsigned char szSplashScreenFilePath[] = "/theme/SplashScreen.bmp";
const unsigned char pucLogFilePath[] = "Log.txt";
const unsigned char pucRobotNameFilePath[] = "/Config/RobotName.txt";
const unsigned char pucBacklightFilePath[] = "/Config/Backlight.bin";
const unsigned char pucVolumeFilePath[] = "/Config/Volume.bin";
const unsigned char pucBt4ModeFilePath[] = "/Config/Bt4Mode.bin";
const unsigned char pucSleepFilePath[] = "/Config/Sleep.bin";
const unsigned char pucShutdownFilePath[] = "/Config/Shutdown.bin";
const unsigned char pucImuCalFilePath[] = "/Config/IMUCal.bin";

const char szProgramFolder[] = "/Program";      // Path to the program folder.
const char szMotionFileExt[] = ".rmo";          // Motion file extension.
const char szPlannerFileExt[] = ".rpl";         // Planner file extension.
