/*******************************************************************************
 * This is the main program for the rero controller.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "TimeDelay.h"
#include "HardwareProfile.h"
#include "UserProgram.h"
#include "GUI/GraphicScreens.h"
#include "Background.h"
#include "Variables.h"
#include "Configuration.h"
#include "UsbApp.h"
#include "Power.h"
#include "AudioAmp.h"
#include "i2c1.h"
#include "Adc.h"
#include "Bluetooth/uart2.h"
#include "USB/usb.h"
#include "IMU/imu.h"
#include "IMU/inv_mpu.h"
#include "IMU/inv_mpu_dmp_motion_driver.h"
#include "Sound Stream/Startup.h"
#include "Bluetooth/BluetoothControl.h"
#include "TouchScreen/TouchScreen.h"
#include "TouchScreen/TouchScreenTask.h"
#include "Image Decoders/ImageDecoder.h"
#include "Image Decoders/BmpDecoder.h"

#include "FatFs/FatFsWrapper.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 *******************************************************************************/

static void prv_vInitializeSystem(void);



/*******************************************************************************
 * MAIN FUNCTION
 *******************************************************************************/
void main(void)
{
    // Initialize the FreeRTOS queue and semaphores.
    xTouchMsgQueue = xQueueCreate(TOUCH_MSG_QUEUE_SIZE, sizeof(GOL_MSG));
    vSemaphoreCreateBinary(xTouchSemaphore);
    vSemaphoreCreateBinary(xBluetoothRxSemaphore);
    vSemaphoreCreateBinary(xPlayingMotionSemaphore);
    vSemaphoreCreateBinary(xLoadAudioBufferSemaphore);
    xSdCardMutex = xSemaphoreCreateMutex();
    xExternalUartMutex = xSemaphoreCreateMutex();
    xBluetoothMutex = xSemaphoreCreateMutex();



    // Initialize the hardware.
    prv_vInitializeSystem();

    
    // Power up the board if the switch is pressed for more than 500ms.
    // Straight away power up the board if this is not POR.
    if ((mGetPORFlag() != 0) && (mGetBORFlag() != 0)) {
        // Clear the POR and BOR flag.
        mClearPORFlag();
        mClearBORFlag();

        // Turn off the power for now.
        POWER_MAIN = 0;
        POWER_SUB = 0;

        unsigned short i = 0;
        while (i < 50) {
            if (POWER_BUTTON == 0) {
                i++;
                DelayMs(10);
            } else {
                i = 0;
            }
        }
    }
    
    // Turn on the power.
    vPowerOn();

    // Initialize Graphics Library and LCD.
    // This may take up to 500ms.
    GOLInit();
    vCreateScheme();
    ImageDecoderInit();

    // Show the start screen and set the backlight to default value.
    vShowStartScreen();
    SetDCOC1PWM(BLight);



#ifdef ENABLE_IMU
     // Initialize IMU.
    vDisplayLoadingText("Initializing IMU...", 1);
    if (ssInitIMU() != 0) {
        vDisplayLoadingText("Error: IMU Error", 2);
        DelayMs(500);
    }
#endif

    

    vDisplayLoadingText("Detecting SD Card...", 1);
    while (SD_CD);
    
    vDisplayLoadingText("Mounting File System...",1);
    FRESULT eResult;
    do {
        FATFS xFatFs;
        eResult = f_mount((FATFS *)&xFatFs, "0", 1);
        
        switch (eResult) {
            case FR_INVALID_DRIVE:  vDisplayLoadingText("Error: Invalid Drive", 2);     break;
            case FR_DISK_ERR:       vDisplayLoadingText("Error: Disk Error", 2);        break;
            case FR_NOT_READY:      vDisplayLoadingText("Error: Not Ready", 2);         break;
            case FR_NO_FILESYSTEM:  vDisplayLoadingText("Error: No File System", 2);    break;
        }
    } while (eResult != FR_OK);



    // Read the configuration from the config files.
    vDisplayLoadingText("Reading Configuration...", 1);
    vInitConfig();
    
    // Initialize touch screen.
    vDisplayLoadingText("Initializing Touch Screen...", 1);
    TouchInit(NVMWrite, NVMRead, NVMSectorErase, NULL);
    
    

    // Boost the Baudrate for G15 and sensors.
    // And also cycle the torque enable bit for G15. This is the workaround for a bug in old G15 firmware.
    // We can only do that when the G15 / sensor is ready.
    vDisplayLoadingText("Initializing Servos and Sensors...", 1);
    eEMBoostBaudrate();
    vEMInitWorkaround();
    
    
    
    // Display the custom splash screen if it's available.
    // If the file can be opened, it's image file.
    FSFILE *pxSplashScreenFile = FSfopen(szSplashScreenFilePath, "r");
    if (pxSplashScreenFile != NULL) {
        DelayMs(500);
        
        ImageDecode(pxSplashScreenFile, IMG_BMP, 0, 0, GetMaxX(), GetMaxY(), 0, NULL, NULL);
        FSfclose(pxSplashScreenFile);
        
        DelayMs(1500);
    }


    xTaskCreate( taskTouchScreen, "Touch", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 5, NULL );
    xTaskCreate( taskGraphicDisplay, "Display", 1024, NULL, tskIDLE_PRIORITY + 3, NULL );
    xTaskCreate( taskBattMonitor, "BattMon", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 6, NULL );
#ifdef ENABLE_IMU
    xTaskCreate( taskImu, "IMU", 300, NULL, tskIDLE_PRIORITY + 2, NULL );
#endif



#ifdef RUN_USER_PROGRAM
    vSetGuiPage(PAGE_USER_PROGRAM);
#else
    // USB is connected.
    if(USB_BUS_SENSE == 1) {
        // Indicate that we are in USB mode.
        xSystemState.bUsbMode = 1;
        vSetGuiPage(PAGE_USB);

        SetColor(pxDefaultScheme->CommonBkColor);
        ClearDevice();

        vDisplayLoadingText("Starting USB", 1);
        xTaskCreate(taskUsb, "USB", 512, NULL, tskIDLE_PRIORITY, NULL);
    }
    else {
        // Clear the screen with fading effect.
        vClearScreenWithFadeEffect();
        
#ifdef SHOWCASE_ROBOT
        vSetGuiPage(PAGE_PLAY);
#else
        vSetGuiPage(PAGE_MAIN_MENU);
#endif
        
        xTaskCreate( taskBluetoothProgram, "BTProgram", 300, NULL, tskIDLE_PRIORITY + 4, NULL );
    }
    
    xTaskCreate( taskStandbyWatchdog, "Standby",configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL );
    xTaskCreate( taskBlinkLed, "Blink", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );
    xTaskCreate( taskMonitorUsb, "USBMon", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
    
#endif

    // Play startup sound.
    vPlaySoundStream(&xStartupSound);
    
    // Indicate the RTOS is running.
    xSystemState.bRtosRunning = 1;
    
    // Start the RTOS running, this function should never return
    vTaskStartScheduler();

    // Insufficient memory.
    while (1) {
        vSetLed(LED_WHITE);
        DelayMs(500);
        vSetLed(LED_OFF);
        DelayMs(500);
    }

}



/*******************************************************************************
 * FUNCTION: InitializeBoard
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Initialize the system.
 *
 *******************************************************************************/
static void prv_vInitializeSystem(void)
{
    // Set the board for optimal performance.
    SYSTEMConfigPerformance(GetSystemClock());

    // Match the PBus to the fuse setting.
    mOSCSetPBDIV(OSC_PB_DIV_1);

    // Disable JTAG.
    mJTAGPortEnable(0);

    
    // Configure interrupt for multi-vectored mode.
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);

    // Enable global interrupt handling.
    INTEnableInterrupts();
    portENABLE_INTERRUPTS();



    // Initialize the ADC.
    vInitAdc();

    // Initialize the RGB LED.
    vSetLed(LED_OFF);
    LEDR_TRIS = 0;
    LEDG_TRIS = 0;
    LEDB_TRIS = 0;



    // Initialize the LCD backlight.
    LCD_BACKLIGHT_TRIS = 0;

    // Configure PWM for the LCD backlight.
    // PWM frequency = 100Hz.
    OpenTimer2(T2_ON | T2_PS_1_64, 9375);
    OpenOC1(OC_ON | OC_IDLE_STOP | OC_TIMER_MODE16 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0);

    

    // Initialize the USB bus sense.
    tris_usb_bus_sense = 1;

    // Initialize the program switch.
    PROGRAM_BUTTON_TRIS = 1;



    // Initialize the power control.
    POWER_BUTTON_TRIS = 1;
    POWER_MAIN_TRIS = 0;
    POWER_SUB_TRIS = 0;

    // Power btn external Pin Interrupt on RE8.
    ConfigINT1(EXT_INT_PRI_7 | FALLING_EDGE_INT | EXT_INT_ENABLE); // Highest priority = 7

    // Initialize Timer 1.
    // Timer 1 is used to keep track of how long the power button is pressed.
    // Period = 100ms.
    OpenTimer1( ( T1_ON | T1_PS_1_256 ), 31250 );

    // Do not enable the interrupt for now. It will be enabled when the power button is pressed.
    ConfigIntTimer1( T1_INT_OFF | T1_INT_PRIOR_6 );



    // Initialize SPI for SD Card.
    SD_CD_TRIS = 1;
    SD_CS = 1;
    SD_CS_TRIS = 0;
    SD_WE_TRIS = 1;
    
    // Initialize I2C 1 (IMU).
    vI2C1Init();

    // Initialize audio amplifier.
    vAudioAmpInit();

    // Initialize UART 2 and Bluetooth module pin.
    vUART2Init();
    BT_RESET = 0;           // Disable the bluetooth.
    BT_RESET_TRIS = 0;
    BT_AT_MODE = 0;         // Transparent mode.
    BT_AT_MODE_TRIS = 0;
    BT_CONNECTED_TRIS = 1;

    // Initialize UART 5 for G15 and sensor.
    vUART5Init();
    
    // Initialize USB.
    USBDeviceInit();
    
    

//    // For debugging test point.
//    PGC = 0;
//    PGD = 0;
//
//    PGC_TRIS = 0;
//    PGD_TRIS = 0;
}
