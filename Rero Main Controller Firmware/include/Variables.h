/*******************************************************************************
 * This file declares all the global variables.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef VARIABLES_H
#define	VARIABLES_H

// Header files for FreeRTOS.
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
 * PUBLIC DEFINITION
 *******************************************************************************/

// System state.
typedef struct {
    unsigned bUsbMode               : 1;
    unsigned bUsbConnected          : 1;
    unsigned bUsbMassStorage        : 1;
    unsigned bStandby               : 1;

    unsigned bPlayMotion            : 1;
    unsigned bSetPath               : 1;
    unsigned bBluetoothMode         : 1;
    unsigned bBluetoothBusy         : 1;    // Data is transmitted or received via bluetooth.
    
    unsigned bBluetoothReady        : 1;    // Bluetooth is initialized and ready.
    unsigned bSdBusy                : 1;    // Busy accessing SD card.
    unsigned bRtosRunning           : 1;
    unsigned                        : 21;
} SYSTEM_STATE;


// System error.
typedef struct {
    unsigned bUartError             : 1;
    unsigned bBluetoothError        : 1;
    unsigned bMallocError           : 1;
    unsigned bStackLowError         : 1;

    unsigned bObjectsOverflowError  : 1;    // Too many graphic objects in single page.
    unsigned                        : 27;
} SYSTEM_ERROR;



// UART buffer information.
typedef struct {
    volatile unsigned char *pucBuffer;  // Pointer to the buffer. Buffer must be allocated explicitly.
    unsigned int uiBufferSize;          // Size of the buffer.
    unsigned int uiReadPt;              // Read pointer.
    unsigned int uiWritePt;             // Write pointer.
    unsigned int uiDataCount;           // Number of bytes available.
} UART_BUFFER;



// Bluetooth 4.0 mode.
typedef enum __attribute__((packed)) {
    EDR,
    BLE
} BT4_MODE;



// Auto sleep settings.
typedef enum __attribute__((packed)) {
    AUTO_SLEEP_15_SEC = 0,
    AUTO_SLEEP_30_SEC,
    AUTO_SLEEP_1_MIN,
    AUTO_SLEEP_2_MIN,
    AUTO_SLEEP_5_MIN,
    AUTO_SLEEP_10_MIN,
    AUTO_SLEEP_NEVER
} AUTO_SLEEP_SETTING;



// Auto shutdown settings.
typedef enum __attribute__((packed)) {
    AUTO_SHUTDOWN_1_MIN = 0,
    AUTO_SHUTDOWN_5_MIN,
    AUTO_SHUTDOWN_10_MIN,
    AUTO_SHUTDOWN_20_MIN,
    AUTO_SHUTDOWN_30_MIN,
    AUTO_SHUTDOWN_60_MIN,
    AUTO_SHUTDOWN_NEVER
} AUTO_SHUTDOWN_SETTING;


// For production.
// Lower to original baudrate 19200.
//#define PRODUCTION

// For showcase robot.
// Play file 5 automatically after power up.
//#define SHOWCASE_ROBOT

// Queue size for touch screen message.
#define TOUCH_MSG_QUEUE_SIZE    6

// Threshold for stack watermark logging.
#define MIN_STACK_WATERMARK     50

// Maximum file name length including the null terminator.
#define MAX_FILENAME_LENGTH     16



/*******************************************************************************
* PUBLIC GLOBAL VARAIBLES PROTOTYPES                                           *
*******************************************************************************/

extern volatile SYSTEM_STATE xSystemState;
extern volatile SYSTEM_ERROR xSystemError;


// Battery percentage.
extern volatile unsigned int uiBattPercentage;

// LCD backlight value.
extern volatile unsigned short BLight;

// Volume value.
extern volatile unsigned char ucVolume;

// Bluetooth 4.0 mode.
extern volatile BT4_MODE eBt4Mode;

// Auto sleep value.
extern volatile AUTO_SLEEP_SETTING eAutoSleep;

// Auto shutdown value.
extern volatile AUTO_SHUTDOWN_SETTING eAutoShutdown;


// Gyro data (deg/s).
extern volatile float fGyroX;
extern volatile float fGyroY;
extern volatile float fGyroZ;

// Accelerometer data (G).
extern volatile float fAccelerometerX;
extern volatile float fAccelerometerY;
extern volatile float fAccelerometerZ;

// Tilt angle calculated from accelerometer data (degree).
extern volatile float fAccTiltAngleX;
extern volatile float fAccTiltAngleY;
extern volatile float fAccTiltAngleZ;

extern xQueueHandle xTouchMsgQueue;
extern xSemaphoreHandle xTouchSemaphore;
extern xSemaphoreHandle xBluetoothRxSemaphore;
extern xSemaphoreHandle xPlayingMotionSemaphore;
extern xSemaphoreHandle xSeekExtModuleSemaphore;
extern xSemaphoreHandle xLoadAudioBufferSemaphore;
extern xSemaphoreHandle xSdCardMutex;
extern xSemaphoreHandle xExternalUartMutex;
extern xSemaphoreHandle xBluetoothMutex;



// Robot name (limited to 13 characters only).
extern unsigned char pucRobotName[20];

// Bluetooth address.
extern char szBluetoothAddress[20];

// Firmware version.
extern const char szFirmwareVersion[];

// Files path.
extern const unsigned char szSplashScreenFilePath[];
extern const unsigned char pucLogFilePath[];
extern const unsigned char pucRobotNameFilePath[];
extern const unsigned char pucBacklightFilePath[];
extern const unsigned char pucVolumeFilePath[];
extern const unsigned char pucBt4ModeFilePath[];
extern const unsigned char pucSleepFilePath[];
extern const unsigned char pucShutdownFilePath[];
extern const unsigned char pucImuCalFilePath[];

extern const char szProgramFolder[];
extern const char szMotionFileExt[];
extern const char szPlannerFileExt[];



#endif	/* VARIABLES_H */

