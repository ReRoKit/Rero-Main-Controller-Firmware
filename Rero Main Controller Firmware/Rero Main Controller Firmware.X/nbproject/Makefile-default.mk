#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Rero_Main_Controller_Firmware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Rero_Main_Controller_Firmware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../src/Bluetooth/BluetoothControl.c ../src/Bluetooth/uart2.c ../src/FatFs/ccsbcs.c ../src/FatFs/diskio.c ../src/FatFs/ff.c ../src/FatFs/SD-SPI.c ../src/FatFs/FatFsWrapper.c "../src/Flash Image/SplashScreenLogo.c" ../src/Font/Segoe_UI_Semibold_11.c ../src/Font/Segoe_UI_Semibold_14.c ../src/Font/Segoe_UI_Semibold_18.c ../src/FreeRTOS/portable/MemMang/heap_3.c ../src/FreeRTOS/portable/MPLAB/PIC32MX/port.c ../src/FreeRTOS/portable/MPLAB/PIC32MX/port_asm.S ../src/FreeRTOS/list.c ../src/FreeRTOS/queue.c ../src/FreeRTOS/tasks.c "../src/G15 and Sensors/G15.c" "../src/G15 and Sensors/uart5.c" "../src/G15 and Sensors/Sensors.c" "../src/G15 and Sensors/ExtModule.c" "../src/GUI/Custom Graphic Object/Battery.c" ../src/GUI/SensorPage.c ../src/GUI/GraphicScreens.c ../src/GUI/G15Page.c ../src/GUI/SetIdPage.c ../src/GUI/SettingsPage.c ../src/GUI/UsbPage.c ../src/GUI/SetBtPinPage.c ../src/GUI/NumbericKeypad.c ../src/GUI/BtSettingsPage.c ../src/GUI/LockScreen.c ../src/GUI/MainMenu.c ../src/GUI/Scheme.c ../src/GUI/ScrollbarListbox.c ../src/GUI/WallpaperPage.c ../src/GUI/HideUnhideScreen.c ../src/GUI/PopUpMessagePage.c ../src/GUI/BluetoothPage.c ../src/GUI/MotionPage.c ../src/GUI/UserProgramPage.c ../src/GUI/Screenshot.c ../src/IMU/i2c1.c ../src/IMU/inv_mpu.c ../src/IMU/inv_mpu_dmp_motion_driver.c ../src/IMU/imu.c ../src/Microchip/Common/TimeDelay.c ../src/Microchip/Graphics/Drivers/ILI9341.c ../src/Microchip/Graphics/Button.c ../src/Microchip/Graphics/Chart.c ../src/Microchip/Graphics/CheckBox.c ../src/Microchip/Graphics/DigitalMeter.c ../src/Microchip/Graphics/EditBox.c ../src/Microchip/Graphics/GOL.c ../src/Microchip/Graphics/GOLSchemeDefault.c ../src/Microchip/Graphics/Grid.c ../src/Microchip/Graphics/GroupBox.c ../src/Microchip/Graphics/ListBox.c ../src/Microchip/Graphics/Meter.c ../src/Microchip/Graphics/Palette.c ../src/Microchip/Graphics/Picture.c ../src/Microchip/Graphics/Primitive.c ../src/Microchip/Graphics/ProgressBar.c ../src/Microchip/Graphics/RadioButton.c ../src/Microchip/Graphics/RoundDial.c ../src/Microchip/Graphics/Slider.c ../src/Microchip/Graphics/StaticText.c ../src/Microchip/Graphics/Window.c "../src/Microchip/Image Decoders/BmpDecoder.c" "../src/Microchip/Image Decoders/ImageDecoder.c" "../src/Microchip/Image Decoders/JpegDecoder.c" "../src/Microchip/Image Decoders/jidctint.c" "../src/Microchip/USB/HID Device Driver/usb_function_hid.c" "../src/Microchip/USB/MSD Device Driver/usb_function_msd_multi_sector.c" ../src/Microchip/USB/usb_descriptors.c ../src/Microchip/USB/usb_device.c "../src/Sound Stream/TouchTone.c" "../src/Sound Stream/Startup.c" "../src/Sound Stream/Shutdown.c" "../src/Sound Stream/Error.c" ../src/Touchscreen/TouchScreen.c ../src/Touchscreen/TouchScreenResistive.c ../src/TouchScreen/TouchScreenTask.c ../src/Main.c ../src/Variables.c ../src/Background.c ../src/SetPath.c ../src/PlayMotion.c ../src/UsbApp.c ../src/Power.c ../src/Planner.c ../src/Log.c ../src/Protocol.c ../src/Adc.c ../src/ErrorTrap.c ../src/AudioAmp.c ../src/UserProgram.c ../src/Configuration.c ../src/TeachMotion.c "../src/Sound Stream/Beep.c"

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/302797332/BluetoothControl.o ${OBJECTDIR}/_ext/302797332/uart2.o ${OBJECTDIR}/_ext/1563925460/ccsbcs.o ${OBJECTDIR}/_ext/1563925460/diskio.o ${OBJECTDIR}/_ext/1563925460/ff.o ${OBJECTDIR}/_ext/1563925460/SD-SPI.o ${OBJECTDIR}/_ext/1563925460/FatFsWrapper.o ${OBJECTDIR}/_ext/689329905/SplashScreenLogo.o ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_11.o ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_14.o ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_18.o ${OBJECTDIR}/_ext/573367119/heap_3.o ${OBJECTDIR}/_ext/1447740322/port.o ${OBJECTDIR}/_ext/1447740322/port_asm.o ${OBJECTDIR}/_ext/1592166420/list.o ${OBJECTDIR}/_ext/1592166420/queue.o ${OBJECTDIR}/_ext/1592166420/tasks.o ${OBJECTDIR}/_ext/523260321/G15.o ${OBJECTDIR}/_ext/523260321/uart5.o ${OBJECTDIR}/_ext/523260321/Sensors.o ${OBJECTDIR}/_ext/523260321/ExtModule.o ${OBJECTDIR}/_ext/54361528/Battery.o ${OBJECTDIR}/_ext/659824929/SensorPage.o ${OBJECTDIR}/_ext/659824929/GraphicScreens.o ${OBJECTDIR}/_ext/659824929/G15Page.o ${OBJECTDIR}/_ext/659824929/SetIdPage.o ${OBJECTDIR}/_ext/659824929/SettingsPage.o ${OBJECTDIR}/_ext/659824929/UsbPage.o ${OBJECTDIR}/_ext/659824929/SetBtPinPage.o ${OBJECTDIR}/_ext/659824929/NumbericKeypad.o ${OBJECTDIR}/_ext/659824929/BtSettingsPage.o ${OBJECTDIR}/_ext/659824929/LockScreen.o ${OBJECTDIR}/_ext/659824929/MainMenu.o ${OBJECTDIR}/_ext/659824929/Scheme.o ${OBJECTDIR}/_ext/659824929/ScrollbarListbox.o ${OBJECTDIR}/_ext/659824929/WallpaperPage.o ${OBJECTDIR}/_ext/659824929/HideUnhideScreen.o ${OBJECTDIR}/_ext/659824929/PopUpMessagePage.o ${OBJECTDIR}/_ext/659824929/BluetoothPage.o ${OBJECTDIR}/_ext/659824929/MotionPage.o ${OBJECTDIR}/_ext/659824929/UserProgramPage.o ${OBJECTDIR}/_ext/659824929/Screenshot.o ${OBJECTDIR}/_ext/659826615/i2c1.o ${OBJECTDIR}/_ext/659826615/inv_mpu.o ${OBJECTDIR}/_ext/659826615/inv_mpu_dmp_motion_driver.o ${OBJECTDIR}/_ext/659826615/imu.o ${OBJECTDIR}/_ext/620447172/TimeDelay.o ${OBJECTDIR}/_ext/1336470976/ILI9341.o ${OBJECTDIR}/_ext/47893468/Button.o ${OBJECTDIR}/_ext/47893468/Chart.o ${OBJECTDIR}/_ext/47893468/CheckBox.o ${OBJECTDIR}/_ext/47893468/DigitalMeter.o ${OBJECTDIR}/_ext/47893468/EditBox.o ${OBJECTDIR}/_ext/47893468/GOL.o ${OBJECTDIR}/_ext/47893468/GOLSchemeDefault.o ${OBJECTDIR}/_ext/47893468/Grid.o ${OBJECTDIR}/_ext/47893468/GroupBox.o ${OBJECTDIR}/_ext/47893468/ListBox.o ${OBJECTDIR}/_ext/47893468/Meter.o ${OBJECTDIR}/_ext/47893468/Palette.o ${OBJECTDIR}/_ext/47893468/Picture.o ${OBJECTDIR}/_ext/47893468/Primitive.o ${OBJECTDIR}/_ext/47893468/ProgressBar.o ${OBJECTDIR}/_ext/47893468/RadioButton.o ${OBJECTDIR}/_ext/47893468/RoundDial.o ${OBJECTDIR}/_ext/47893468/Slider.o ${OBJECTDIR}/_ext/47893468/StaticText.o ${OBJECTDIR}/_ext/47893468/Window.o ${OBJECTDIR}/_ext/219152179/BmpDecoder.o ${OBJECTDIR}/_ext/219152179/ImageDecoder.o ${OBJECTDIR}/_ext/219152179/JpegDecoder.o ${OBJECTDIR}/_ext/219152179/jidctint.o ${OBJECTDIR}/_ext/659191409/usb_function_hid.o ${OBJECTDIR}/_ext/1974569172/usb_function_msd_multi_sector.o ${OBJECTDIR}/_ext/2140021685/usb_descriptors.o ${OBJECTDIR}/_ext/2140021685/usb_device.o ${OBJECTDIR}/_ext/1094399317/TouchTone.o ${OBJECTDIR}/_ext/1094399317/Startup.o ${OBJECTDIR}/_ext/1094399317/Shutdown.o ${OBJECTDIR}/_ext/1094399317/Error.o ${OBJECTDIR}/_ext/791854511/TouchScreen.o ${OBJECTDIR}/_ext/791854511/TouchScreenResistive.o ${OBJECTDIR}/_ext/1707987343/TouchScreenTask.o ${OBJECTDIR}/_ext/1360937237/Main.o ${OBJECTDIR}/_ext/1360937237/Variables.o ${OBJECTDIR}/_ext/1360937237/Background.o ${OBJECTDIR}/_ext/1360937237/SetPath.o ${OBJECTDIR}/_ext/1360937237/PlayMotion.o ${OBJECTDIR}/_ext/1360937237/UsbApp.o ${OBJECTDIR}/_ext/1360937237/Power.o ${OBJECTDIR}/_ext/1360937237/Planner.o ${OBJECTDIR}/_ext/1360937237/Log.o ${OBJECTDIR}/_ext/1360937237/Protocol.o ${OBJECTDIR}/_ext/1360937237/Adc.o ${OBJECTDIR}/_ext/1360937237/ErrorTrap.o ${OBJECTDIR}/_ext/1360937237/AudioAmp.o ${OBJECTDIR}/_ext/1360937237/UserProgram.o ${OBJECTDIR}/_ext/1360937237/Configuration.o ${OBJECTDIR}/_ext/1360937237/TeachMotion.o ${OBJECTDIR}/_ext/1094399317/Beep.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/302797332/BluetoothControl.o.d ${OBJECTDIR}/_ext/302797332/uart2.o.d ${OBJECTDIR}/_ext/1563925460/ccsbcs.o.d ${OBJECTDIR}/_ext/1563925460/diskio.o.d ${OBJECTDIR}/_ext/1563925460/ff.o.d ${OBJECTDIR}/_ext/1563925460/SD-SPI.o.d ${OBJECTDIR}/_ext/1563925460/FatFsWrapper.o.d ${OBJECTDIR}/_ext/689329905/SplashScreenLogo.o.d ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_11.o.d ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_14.o.d ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_18.o.d ${OBJECTDIR}/_ext/573367119/heap_3.o.d ${OBJECTDIR}/_ext/1447740322/port.o.d ${OBJECTDIR}/_ext/1447740322/port_asm.o.d ${OBJECTDIR}/_ext/1592166420/list.o.d ${OBJECTDIR}/_ext/1592166420/queue.o.d ${OBJECTDIR}/_ext/1592166420/tasks.o.d ${OBJECTDIR}/_ext/523260321/G15.o.d ${OBJECTDIR}/_ext/523260321/uart5.o.d ${OBJECTDIR}/_ext/523260321/Sensors.o.d ${OBJECTDIR}/_ext/523260321/ExtModule.o.d ${OBJECTDIR}/_ext/54361528/Battery.o.d ${OBJECTDIR}/_ext/659824929/SensorPage.o.d ${OBJECTDIR}/_ext/659824929/GraphicScreens.o.d ${OBJECTDIR}/_ext/659824929/G15Page.o.d ${OBJECTDIR}/_ext/659824929/SetIdPage.o.d ${OBJECTDIR}/_ext/659824929/SettingsPage.o.d ${OBJECTDIR}/_ext/659824929/UsbPage.o.d ${OBJECTDIR}/_ext/659824929/SetBtPinPage.o.d ${OBJECTDIR}/_ext/659824929/NumbericKeypad.o.d ${OBJECTDIR}/_ext/659824929/BtSettingsPage.o.d ${OBJECTDIR}/_ext/659824929/LockScreen.o.d ${OBJECTDIR}/_ext/659824929/MainMenu.o.d ${OBJECTDIR}/_ext/659824929/Scheme.o.d ${OBJECTDIR}/_ext/659824929/ScrollbarListbox.o.d ${OBJECTDIR}/_ext/659824929/WallpaperPage.o.d ${OBJECTDIR}/_ext/659824929/HideUnhideScreen.o.d ${OBJECTDIR}/_ext/659824929/PopUpMessagePage.o.d ${OBJECTDIR}/_ext/659824929/BluetoothPage.o.d ${OBJECTDIR}/_ext/659824929/MotionPage.o.d ${OBJECTDIR}/_ext/659824929/UserProgramPage.o.d ${OBJECTDIR}/_ext/659824929/Screenshot.o.d ${OBJECTDIR}/_ext/659826615/i2c1.o.d ${OBJECTDIR}/_ext/659826615/inv_mpu.o.d ${OBJECTDIR}/_ext/659826615/inv_mpu_dmp_motion_driver.o.d ${OBJECTDIR}/_ext/659826615/imu.o.d ${OBJECTDIR}/_ext/620447172/TimeDelay.o.d ${OBJECTDIR}/_ext/1336470976/ILI9341.o.d ${OBJECTDIR}/_ext/47893468/Button.o.d ${OBJECTDIR}/_ext/47893468/Chart.o.d ${OBJECTDIR}/_ext/47893468/CheckBox.o.d ${OBJECTDIR}/_ext/47893468/DigitalMeter.o.d ${OBJECTDIR}/_ext/47893468/EditBox.o.d ${OBJECTDIR}/_ext/47893468/GOL.o.d ${OBJECTDIR}/_ext/47893468/GOLSchemeDefault.o.d ${OBJECTDIR}/_ext/47893468/Grid.o.d ${OBJECTDIR}/_ext/47893468/GroupBox.o.d ${OBJECTDIR}/_ext/47893468/ListBox.o.d ${OBJECTDIR}/_ext/47893468/Meter.o.d ${OBJECTDIR}/_ext/47893468/Palette.o.d ${OBJECTDIR}/_ext/47893468/Picture.o.d ${OBJECTDIR}/_ext/47893468/Primitive.o.d ${OBJECTDIR}/_ext/47893468/ProgressBar.o.d ${OBJECTDIR}/_ext/47893468/RadioButton.o.d ${OBJECTDIR}/_ext/47893468/RoundDial.o.d ${OBJECTDIR}/_ext/47893468/Slider.o.d ${OBJECTDIR}/_ext/47893468/StaticText.o.d ${OBJECTDIR}/_ext/47893468/Window.o.d ${OBJECTDIR}/_ext/219152179/BmpDecoder.o.d ${OBJECTDIR}/_ext/219152179/ImageDecoder.o.d ${OBJECTDIR}/_ext/219152179/JpegDecoder.o.d ${OBJECTDIR}/_ext/219152179/jidctint.o.d ${OBJECTDIR}/_ext/659191409/usb_function_hid.o.d ${OBJECTDIR}/_ext/1974569172/usb_function_msd_multi_sector.o.d ${OBJECTDIR}/_ext/2140021685/usb_descriptors.o.d ${OBJECTDIR}/_ext/2140021685/usb_device.o.d ${OBJECTDIR}/_ext/1094399317/TouchTone.o.d ${OBJECTDIR}/_ext/1094399317/Startup.o.d ${OBJECTDIR}/_ext/1094399317/Shutdown.o.d ${OBJECTDIR}/_ext/1094399317/Error.o.d ${OBJECTDIR}/_ext/791854511/TouchScreen.o.d ${OBJECTDIR}/_ext/791854511/TouchScreenResistive.o.d ${OBJECTDIR}/_ext/1707987343/TouchScreenTask.o.d ${OBJECTDIR}/_ext/1360937237/Main.o.d ${OBJECTDIR}/_ext/1360937237/Variables.o.d ${OBJECTDIR}/_ext/1360937237/Background.o.d ${OBJECTDIR}/_ext/1360937237/SetPath.o.d ${OBJECTDIR}/_ext/1360937237/PlayMotion.o.d ${OBJECTDIR}/_ext/1360937237/UsbApp.o.d ${OBJECTDIR}/_ext/1360937237/Power.o.d ${OBJECTDIR}/_ext/1360937237/Planner.o.d ${OBJECTDIR}/_ext/1360937237/Log.o.d ${OBJECTDIR}/_ext/1360937237/Protocol.o.d ${OBJECTDIR}/_ext/1360937237/Adc.o.d ${OBJECTDIR}/_ext/1360937237/ErrorTrap.o.d ${OBJECTDIR}/_ext/1360937237/AudioAmp.o.d ${OBJECTDIR}/_ext/1360937237/UserProgram.o.d ${OBJECTDIR}/_ext/1360937237/Configuration.o.d ${OBJECTDIR}/_ext/1360937237/TeachMotion.o.d ${OBJECTDIR}/_ext/1094399317/Beep.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/302797332/BluetoothControl.o ${OBJECTDIR}/_ext/302797332/uart2.o ${OBJECTDIR}/_ext/1563925460/ccsbcs.o ${OBJECTDIR}/_ext/1563925460/diskio.o ${OBJECTDIR}/_ext/1563925460/ff.o ${OBJECTDIR}/_ext/1563925460/SD-SPI.o ${OBJECTDIR}/_ext/1563925460/FatFsWrapper.o ${OBJECTDIR}/_ext/689329905/SplashScreenLogo.o ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_11.o ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_14.o ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_18.o ${OBJECTDIR}/_ext/573367119/heap_3.o ${OBJECTDIR}/_ext/1447740322/port.o ${OBJECTDIR}/_ext/1447740322/port_asm.o ${OBJECTDIR}/_ext/1592166420/list.o ${OBJECTDIR}/_ext/1592166420/queue.o ${OBJECTDIR}/_ext/1592166420/tasks.o ${OBJECTDIR}/_ext/523260321/G15.o ${OBJECTDIR}/_ext/523260321/uart5.o ${OBJECTDIR}/_ext/523260321/Sensors.o ${OBJECTDIR}/_ext/523260321/ExtModule.o ${OBJECTDIR}/_ext/54361528/Battery.o ${OBJECTDIR}/_ext/659824929/SensorPage.o ${OBJECTDIR}/_ext/659824929/GraphicScreens.o ${OBJECTDIR}/_ext/659824929/G15Page.o ${OBJECTDIR}/_ext/659824929/SetIdPage.o ${OBJECTDIR}/_ext/659824929/SettingsPage.o ${OBJECTDIR}/_ext/659824929/UsbPage.o ${OBJECTDIR}/_ext/659824929/SetBtPinPage.o ${OBJECTDIR}/_ext/659824929/NumbericKeypad.o ${OBJECTDIR}/_ext/659824929/BtSettingsPage.o ${OBJECTDIR}/_ext/659824929/LockScreen.o ${OBJECTDIR}/_ext/659824929/MainMenu.o ${OBJECTDIR}/_ext/659824929/Scheme.o ${OBJECTDIR}/_ext/659824929/ScrollbarListbox.o ${OBJECTDIR}/_ext/659824929/WallpaperPage.o ${OBJECTDIR}/_ext/659824929/HideUnhideScreen.o ${OBJECTDIR}/_ext/659824929/PopUpMessagePage.o ${OBJECTDIR}/_ext/659824929/BluetoothPage.o ${OBJECTDIR}/_ext/659824929/MotionPage.o ${OBJECTDIR}/_ext/659824929/UserProgramPage.o ${OBJECTDIR}/_ext/659824929/Screenshot.o ${OBJECTDIR}/_ext/659826615/i2c1.o ${OBJECTDIR}/_ext/659826615/inv_mpu.o ${OBJECTDIR}/_ext/659826615/inv_mpu_dmp_motion_driver.o ${OBJECTDIR}/_ext/659826615/imu.o ${OBJECTDIR}/_ext/620447172/TimeDelay.o ${OBJECTDIR}/_ext/1336470976/ILI9341.o ${OBJECTDIR}/_ext/47893468/Button.o ${OBJECTDIR}/_ext/47893468/Chart.o ${OBJECTDIR}/_ext/47893468/CheckBox.o ${OBJECTDIR}/_ext/47893468/DigitalMeter.o ${OBJECTDIR}/_ext/47893468/EditBox.o ${OBJECTDIR}/_ext/47893468/GOL.o ${OBJECTDIR}/_ext/47893468/GOLSchemeDefault.o ${OBJECTDIR}/_ext/47893468/Grid.o ${OBJECTDIR}/_ext/47893468/GroupBox.o ${OBJECTDIR}/_ext/47893468/ListBox.o ${OBJECTDIR}/_ext/47893468/Meter.o ${OBJECTDIR}/_ext/47893468/Palette.o ${OBJECTDIR}/_ext/47893468/Picture.o ${OBJECTDIR}/_ext/47893468/Primitive.o ${OBJECTDIR}/_ext/47893468/ProgressBar.o ${OBJECTDIR}/_ext/47893468/RadioButton.o ${OBJECTDIR}/_ext/47893468/RoundDial.o ${OBJECTDIR}/_ext/47893468/Slider.o ${OBJECTDIR}/_ext/47893468/StaticText.o ${OBJECTDIR}/_ext/47893468/Window.o ${OBJECTDIR}/_ext/219152179/BmpDecoder.o ${OBJECTDIR}/_ext/219152179/ImageDecoder.o ${OBJECTDIR}/_ext/219152179/JpegDecoder.o ${OBJECTDIR}/_ext/219152179/jidctint.o ${OBJECTDIR}/_ext/659191409/usb_function_hid.o ${OBJECTDIR}/_ext/1974569172/usb_function_msd_multi_sector.o ${OBJECTDIR}/_ext/2140021685/usb_descriptors.o ${OBJECTDIR}/_ext/2140021685/usb_device.o ${OBJECTDIR}/_ext/1094399317/TouchTone.o ${OBJECTDIR}/_ext/1094399317/Startup.o ${OBJECTDIR}/_ext/1094399317/Shutdown.o ${OBJECTDIR}/_ext/1094399317/Error.o ${OBJECTDIR}/_ext/791854511/TouchScreen.o ${OBJECTDIR}/_ext/791854511/TouchScreenResistive.o ${OBJECTDIR}/_ext/1707987343/TouchScreenTask.o ${OBJECTDIR}/_ext/1360937237/Main.o ${OBJECTDIR}/_ext/1360937237/Variables.o ${OBJECTDIR}/_ext/1360937237/Background.o ${OBJECTDIR}/_ext/1360937237/SetPath.o ${OBJECTDIR}/_ext/1360937237/PlayMotion.o ${OBJECTDIR}/_ext/1360937237/UsbApp.o ${OBJECTDIR}/_ext/1360937237/Power.o ${OBJECTDIR}/_ext/1360937237/Planner.o ${OBJECTDIR}/_ext/1360937237/Log.o ${OBJECTDIR}/_ext/1360937237/Protocol.o ${OBJECTDIR}/_ext/1360937237/Adc.o ${OBJECTDIR}/_ext/1360937237/ErrorTrap.o ${OBJECTDIR}/_ext/1360937237/AudioAmp.o ${OBJECTDIR}/_ext/1360937237/UserProgram.o ${OBJECTDIR}/_ext/1360937237/Configuration.o ${OBJECTDIR}/_ext/1360937237/TeachMotion.o ${OBJECTDIR}/_ext/1094399317/Beep.o

# Source Files
SOURCEFILES=../src/Bluetooth/BluetoothControl.c ../src/Bluetooth/uart2.c ../src/FatFs/ccsbcs.c ../src/FatFs/diskio.c ../src/FatFs/ff.c ../src/FatFs/SD-SPI.c ../src/FatFs/FatFsWrapper.c ../src/Flash Image/SplashScreenLogo.c ../src/Font/Segoe_UI_Semibold_11.c ../src/Font/Segoe_UI_Semibold_14.c ../src/Font/Segoe_UI_Semibold_18.c ../src/FreeRTOS/portable/MemMang/heap_3.c ../src/FreeRTOS/portable/MPLAB/PIC32MX/port.c ../src/FreeRTOS/portable/MPLAB/PIC32MX/port_asm.S ../src/FreeRTOS/list.c ../src/FreeRTOS/queue.c ../src/FreeRTOS/tasks.c ../src/G15 and Sensors/G15.c ../src/G15 and Sensors/uart5.c ../src/G15 and Sensors/Sensors.c ../src/G15 and Sensors/ExtModule.c ../src/GUI/Custom Graphic Object/Battery.c ../src/GUI/SensorPage.c ../src/GUI/GraphicScreens.c ../src/GUI/G15Page.c ../src/GUI/SetIdPage.c ../src/GUI/SettingsPage.c ../src/GUI/UsbPage.c ../src/GUI/SetBtPinPage.c ../src/GUI/NumbericKeypad.c ../src/GUI/BtSettingsPage.c ../src/GUI/LockScreen.c ../src/GUI/MainMenu.c ../src/GUI/Scheme.c ../src/GUI/ScrollbarListbox.c ../src/GUI/WallpaperPage.c ../src/GUI/HideUnhideScreen.c ../src/GUI/PopUpMessagePage.c ../src/GUI/BluetoothPage.c ../src/GUI/MotionPage.c ../src/GUI/UserProgramPage.c ../src/GUI/Screenshot.c ../src/IMU/i2c1.c ../src/IMU/inv_mpu.c ../src/IMU/inv_mpu_dmp_motion_driver.c ../src/IMU/imu.c ../src/Microchip/Common/TimeDelay.c ../src/Microchip/Graphics/Drivers/ILI9341.c ../src/Microchip/Graphics/Button.c ../src/Microchip/Graphics/Chart.c ../src/Microchip/Graphics/CheckBox.c ../src/Microchip/Graphics/DigitalMeter.c ../src/Microchip/Graphics/EditBox.c ../src/Microchip/Graphics/GOL.c ../src/Microchip/Graphics/GOLSchemeDefault.c ../src/Microchip/Graphics/Grid.c ../src/Microchip/Graphics/GroupBox.c ../src/Microchip/Graphics/ListBox.c ../src/Microchip/Graphics/Meter.c ../src/Microchip/Graphics/Palette.c ../src/Microchip/Graphics/Picture.c ../src/Microchip/Graphics/Primitive.c ../src/Microchip/Graphics/ProgressBar.c ../src/Microchip/Graphics/RadioButton.c ../src/Microchip/Graphics/RoundDial.c ../src/Microchip/Graphics/Slider.c ../src/Microchip/Graphics/StaticText.c ../src/Microchip/Graphics/Window.c ../src/Microchip/Image Decoders/BmpDecoder.c ../src/Microchip/Image Decoders/ImageDecoder.c ../src/Microchip/Image Decoders/JpegDecoder.c ../src/Microchip/Image Decoders/jidctint.c ../src/Microchip/USB/HID Device Driver/usb_function_hid.c ../src/Microchip/USB/MSD Device Driver/usb_function_msd_multi_sector.c ../src/Microchip/USB/usb_descriptors.c ../src/Microchip/USB/usb_device.c ../src/Sound Stream/TouchTone.c ../src/Sound Stream/Startup.c ../src/Sound Stream/Shutdown.c ../src/Sound Stream/Error.c ../src/Touchscreen/TouchScreen.c ../src/Touchscreen/TouchScreenResistive.c ../src/TouchScreen/TouchScreenTask.c ../src/Main.c ../src/Variables.c ../src/Background.c ../src/SetPath.c ../src/PlayMotion.c ../src/UsbApp.c ../src/Power.c ../src/Planner.c ../src/Log.c ../src/Protocol.c ../src/Adc.c ../src/ErrorTrap.c ../src/AudioAmp.c ../src/UserProgram.c ../src/Configuration.c ../src/TeachMotion.c ../src/Sound Stream/Beep.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

# The following macros may be used in the pre and post step lines
Device=PIC32MX575F512L
ProjectDir="F:\GitHub Projects\Rero-Main-Controller-Firmware\Rero Main Controller Firmware\Rero Main Controller Firmware.X"
ConfName=default
ImagePath="dist\default\${IMAGE_TYPE}\Rero_Main_Controller_Firmware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}"
ImageDir="dist\default\${IMAGE_TYPE}"
ImageName="Rero_Main_Controller_Firmware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}"
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IsDebug="true"
else
IsDebug="false"
endif

.build-conf:  .pre ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/Rero_Main_Controller_Firmware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
	@echo "--------------------------------------"
	@echo "User defined post-build step: []"
	@
	@echo "--------------------------------------"

MP_PROCESSOR_OPTION=32MX575F512L
MP_LINKER_FILE_OPTION=,--script="..\app_32MX575F512L.ld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1447740322/port_asm.o: ../src/FreeRTOS/portable/MPLAB/PIC32MX/port_asm.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1447740322" 
	@${RM} ${OBJECTDIR}/_ext/1447740322/port_asm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1447740322/port_asm.o 
	@${RM} ${OBJECTDIR}/_ext/1447740322/port_asm.o.ok ${OBJECTDIR}/_ext/1447740322/port_asm.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1447740322/port_asm.o.d" "${OBJECTDIR}/_ext/1447740322/port_asm.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1447740322/port_asm.o.d"  -o ${OBJECTDIR}/_ext/1447740322/port_asm.o ../src/FreeRTOS/portable/MPLAB/PIC32MX/port_asm.S    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1447740322/port_asm.o.asm.d",--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_SIMULATOR=1
	
else
${OBJECTDIR}/_ext/1447740322/port_asm.o: ../src/FreeRTOS/portable/MPLAB/PIC32MX/port_asm.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1447740322" 
	@${RM} ${OBJECTDIR}/_ext/1447740322/port_asm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1447740322/port_asm.o 
	@${RM} ${OBJECTDIR}/_ext/1447740322/port_asm.o.ok ${OBJECTDIR}/_ext/1447740322/port_asm.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1447740322/port_asm.o.d" "${OBJECTDIR}/_ext/1447740322/port_asm.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1447740322/port_asm.o.d"  -o ${OBJECTDIR}/_ext/1447740322/port_asm.o ../src/FreeRTOS/portable/MPLAB/PIC32MX/port_asm.S    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1447740322/port_asm.o.asm.d",--gdwarf-2
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/302797332/BluetoothControl.o: ../src/Bluetooth/BluetoothControl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/302797332" 
	@${RM} ${OBJECTDIR}/_ext/302797332/BluetoothControl.o.d 
	@${RM} ${OBJECTDIR}/_ext/302797332/BluetoothControl.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/302797332/BluetoothControl.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/302797332/BluetoothControl.o.d" -o ${OBJECTDIR}/_ext/302797332/BluetoothControl.o ../src/Bluetooth/BluetoothControl.c     
	
${OBJECTDIR}/_ext/302797332/uart2.o: ../src/Bluetooth/uart2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/302797332" 
	@${RM} ${OBJECTDIR}/_ext/302797332/uart2.o.d 
	@${RM} ${OBJECTDIR}/_ext/302797332/uart2.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/302797332/uart2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/302797332/uart2.o.d" -o ${OBJECTDIR}/_ext/302797332/uart2.o ../src/Bluetooth/uart2.c     
	
${OBJECTDIR}/_ext/1563925460/ccsbcs.o: ../src/FatFs/ccsbcs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1563925460" 
	@${RM} ${OBJECTDIR}/_ext/1563925460/ccsbcs.o.d 
	@${RM} ${OBJECTDIR}/_ext/1563925460/ccsbcs.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1563925460/ccsbcs.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1563925460/ccsbcs.o.d" -o ${OBJECTDIR}/_ext/1563925460/ccsbcs.o ../src/FatFs/ccsbcs.c     
	
${OBJECTDIR}/_ext/1563925460/diskio.o: ../src/FatFs/diskio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1563925460" 
	@${RM} ${OBJECTDIR}/_ext/1563925460/diskio.o.d 
	@${RM} ${OBJECTDIR}/_ext/1563925460/diskio.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1563925460/diskio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1563925460/diskio.o.d" -o ${OBJECTDIR}/_ext/1563925460/diskio.o ../src/FatFs/diskio.c     
	
${OBJECTDIR}/_ext/1563925460/ff.o: ../src/FatFs/ff.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1563925460" 
	@${RM} ${OBJECTDIR}/_ext/1563925460/ff.o.d 
	@${RM} ${OBJECTDIR}/_ext/1563925460/ff.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1563925460/ff.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1563925460/ff.o.d" -o ${OBJECTDIR}/_ext/1563925460/ff.o ../src/FatFs/ff.c     
	
${OBJECTDIR}/_ext/1563925460/SD-SPI.o: ../src/FatFs/SD-SPI.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1563925460" 
	@${RM} ${OBJECTDIR}/_ext/1563925460/SD-SPI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1563925460/SD-SPI.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1563925460/SD-SPI.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1563925460/SD-SPI.o.d" -o ${OBJECTDIR}/_ext/1563925460/SD-SPI.o ../src/FatFs/SD-SPI.c     
	
${OBJECTDIR}/_ext/1563925460/FatFsWrapper.o: ../src/FatFs/FatFsWrapper.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1563925460" 
	@${RM} ${OBJECTDIR}/_ext/1563925460/FatFsWrapper.o.d 
	@${RM} ${OBJECTDIR}/_ext/1563925460/FatFsWrapper.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1563925460/FatFsWrapper.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1563925460/FatFsWrapper.o.d" -o ${OBJECTDIR}/_ext/1563925460/FatFsWrapper.o ../src/FatFs/FatFsWrapper.c     
	
${OBJECTDIR}/_ext/689329905/SplashScreenLogo.o: ../src/Flash\ Image/SplashScreenLogo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/689329905" 
	@${RM} ${OBJECTDIR}/_ext/689329905/SplashScreenLogo.o.d 
	@${RM} ${OBJECTDIR}/_ext/689329905/SplashScreenLogo.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/689329905/SplashScreenLogo.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/689329905/SplashScreenLogo.o.d" -o ${OBJECTDIR}/_ext/689329905/SplashScreenLogo.o "../src/Flash Image/SplashScreenLogo.c"     
	
${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_11.o: ../src/Font/Segoe_UI_Semibold_11.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1020267223" 
	@${RM} ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_11.o.d 
	@${RM} ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_11.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_11.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_11.o.d" -o ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_11.o ../src/Font/Segoe_UI_Semibold_11.c     
	
${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_14.o: ../src/Font/Segoe_UI_Semibold_14.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1020267223" 
	@${RM} ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_14.o.d 
	@${RM} ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_14.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_14.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_14.o.d" -o ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_14.o ../src/Font/Segoe_UI_Semibold_14.c     
	
${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_18.o: ../src/Font/Segoe_UI_Semibold_18.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1020267223" 
	@${RM} ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_18.o.d 
	@${RM} ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_18.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_18.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_18.o.d" -o ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_18.o ../src/Font/Segoe_UI_Semibold_18.c     
	
${OBJECTDIR}/_ext/573367119/heap_3.o: ../src/FreeRTOS/portable/MemMang/heap_3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/573367119" 
	@${RM} ${OBJECTDIR}/_ext/573367119/heap_3.o.d 
	@${RM} ${OBJECTDIR}/_ext/573367119/heap_3.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/573367119/heap_3.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/573367119/heap_3.o.d" -o ${OBJECTDIR}/_ext/573367119/heap_3.o ../src/FreeRTOS/portable/MemMang/heap_3.c     
	
${OBJECTDIR}/_ext/1447740322/port.o: ../src/FreeRTOS/portable/MPLAB/PIC32MX/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1447740322" 
	@${RM} ${OBJECTDIR}/_ext/1447740322/port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1447740322/port.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1447740322/port.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1447740322/port.o.d" -o ${OBJECTDIR}/_ext/1447740322/port.o ../src/FreeRTOS/portable/MPLAB/PIC32MX/port.c     
	
${OBJECTDIR}/_ext/1592166420/list.o: ../src/FreeRTOS/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1592166420" 
	@${RM} ${OBJECTDIR}/_ext/1592166420/list.o.d 
	@${RM} ${OBJECTDIR}/_ext/1592166420/list.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1592166420/list.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1592166420/list.o.d" -o ${OBJECTDIR}/_ext/1592166420/list.o ../src/FreeRTOS/list.c     
	
${OBJECTDIR}/_ext/1592166420/queue.o: ../src/FreeRTOS/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1592166420" 
	@${RM} ${OBJECTDIR}/_ext/1592166420/queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/1592166420/queue.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1592166420/queue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1592166420/queue.o.d" -o ${OBJECTDIR}/_ext/1592166420/queue.o ../src/FreeRTOS/queue.c     
	
${OBJECTDIR}/_ext/1592166420/tasks.o: ../src/FreeRTOS/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1592166420" 
	@${RM} ${OBJECTDIR}/_ext/1592166420/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1592166420/tasks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1592166420/tasks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1592166420/tasks.o.d" -o ${OBJECTDIR}/_ext/1592166420/tasks.o ../src/FreeRTOS/tasks.c     
	
${OBJECTDIR}/_ext/523260321/G15.o: ../src/G15\ and\ Sensors/G15.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/523260321" 
	@${RM} ${OBJECTDIR}/_ext/523260321/G15.o.d 
	@${RM} ${OBJECTDIR}/_ext/523260321/G15.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/523260321/G15.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/523260321/G15.o.d" -o ${OBJECTDIR}/_ext/523260321/G15.o "../src/G15 and Sensors/G15.c"     
	
${OBJECTDIR}/_ext/523260321/uart5.o: ../src/G15\ and\ Sensors/uart5.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/523260321" 
	@${RM} ${OBJECTDIR}/_ext/523260321/uart5.o.d 
	@${RM} ${OBJECTDIR}/_ext/523260321/uart5.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/523260321/uart5.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/523260321/uart5.o.d" -o ${OBJECTDIR}/_ext/523260321/uart5.o "../src/G15 and Sensors/uart5.c"     
	
${OBJECTDIR}/_ext/523260321/Sensors.o: ../src/G15\ and\ Sensors/Sensors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/523260321" 
	@${RM} ${OBJECTDIR}/_ext/523260321/Sensors.o.d 
	@${RM} ${OBJECTDIR}/_ext/523260321/Sensors.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/523260321/Sensors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/523260321/Sensors.o.d" -o ${OBJECTDIR}/_ext/523260321/Sensors.o "../src/G15 and Sensors/Sensors.c"     
	
${OBJECTDIR}/_ext/523260321/ExtModule.o: ../src/G15\ and\ Sensors/ExtModule.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/523260321" 
	@${RM} ${OBJECTDIR}/_ext/523260321/ExtModule.o.d 
	@${RM} ${OBJECTDIR}/_ext/523260321/ExtModule.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/523260321/ExtModule.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/523260321/ExtModule.o.d" -o ${OBJECTDIR}/_ext/523260321/ExtModule.o "../src/G15 and Sensors/ExtModule.c"     
	
${OBJECTDIR}/_ext/54361528/Battery.o: ../src/GUI/Custom\ Graphic\ Object/Battery.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/54361528" 
	@${RM} ${OBJECTDIR}/_ext/54361528/Battery.o.d 
	@${RM} ${OBJECTDIR}/_ext/54361528/Battery.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/54361528/Battery.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/54361528/Battery.o.d" -o ${OBJECTDIR}/_ext/54361528/Battery.o "../src/GUI/Custom Graphic Object/Battery.c"     
	
${OBJECTDIR}/_ext/659824929/SensorPage.o: ../src/GUI/SensorPage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/SensorPage.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/SensorPage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/SensorPage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/SensorPage.o.d" -o ${OBJECTDIR}/_ext/659824929/SensorPage.o ../src/GUI/SensorPage.c     
	
${OBJECTDIR}/_ext/659824929/GraphicScreens.o: ../src/GUI/GraphicScreens.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/GraphicScreens.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/GraphicScreens.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/GraphicScreens.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/GraphicScreens.o.d" -o ${OBJECTDIR}/_ext/659824929/GraphicScreens.o ../src/GUI/GraphicScreens.c     
	
${OBJECTDIR}/_ext/659824929/G15Page.o: ../src/GUI/G15Page.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/G15Page.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/G15Page.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/G15Page.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/G15Page.o.d" -o ${OBJECTDIR}/_ext/659824929/G15Page.o ../src/GUI/G15Page.c     
	
${OBJECTDIR}/_ext/659824929/SetIdPage.o: ../src/GUI/SetIdPage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/SetIdPage.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/SetIdPage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/SetIdPage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/SetIdPage.o.d" -o ${OBJECTDIR}/_ext/659824929/SetIdPage.o ../src/GUI/SetIdPage.c     
	
${OBJECTDIR}/_ext/659824929/SettingsPage.o: ../src/GUI/SettingsPage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/SettingsPage.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/SettingsPage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/SettingsPage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/SettingsPage.o.d" -o ${OBJECTDIR}/_ext/659824929/SettingsPage.o ../src/GUI/SettingsPage.c     
	
${OBJECTDIR}/_ext/659824929/UsbPage.o: ../src/GUI/UsbPage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/UsbPage.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/UsbPage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/UsbPage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/UsbPage.o.d" -o ${OBJECTDIR}/_ext/659824929/UsbPage.o ../src/GUI/UsbPage.c     
	
${OBJECTDIR}/_ext/659824929/SetBtPinPage.o: ../src/GUI/SetBtPinPage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/SetBtPinPage.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/SetBtPinPage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/SetBtPinPage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/SetBtPinPage.o.d" -o ${OBJECTDIR}/_ext/659824929/SetBtPinPage.o ../src/GUI/SetBtPinPage.c     
	
${OBJECTDIR}/_ext/659824929/NumbericKeypad.o: ../src/GUI/NumbericKeypad.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/NumbericKeypad.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/NumbericKeypad.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/NumbericKeypad.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/NumbericKeypad.o.d" -o ${OBJECTDIR}/_ext/659824929/NumbericKeypad.o ../src/GUI/NumbericKeypad.c     
	
${OBJECTDIR}/_ext/659824929/BtSettingsPage.o: ../src/GUI/BtSettingsPage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/BtSettingsPage.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/BtSettingsPage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/BtSettingsPage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/BtSettingsPage.o.d" -o ${OBJECTDIR}/_ext/659824929/BtSettingsPage.o ../src/GUI/BtSettingsPage.c     
	
${OBJECTDIR}/_ext/659824929/LockScreen.o: ../src/GUI/LockScreen.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/LockScreen.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/LockScreen.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/LockScreen.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/LockScreen.o.d" -o ${OBJECTDIR}/_ext/659824929/LockScreen.o ../src/GUI/LockScreen.c     
	
${OBJECTDIR}/_ext/659824929/MainMenu.o: ../src/GUI/MainMenu.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/MainMenu.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/MainMenu.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/MainMenu.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/MainMenu.o.d" -o ${OBJECTDIR}/_ext/659824929/MainMenu.o ../src/GUI/MainMenu.c     
	
${OBJECTDIR}/_ext/659824929/Scheme.o: ../src/GUI/Scheme.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/Scheme.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/Scheme.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/Scheme.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/Scheme.o.d" -o ${OBJECTDIR}/_ext/659824929/Scheme.o ../src/GUI/Scheme.c     
	
${OBJECTDIR}/_ext/659824929/ScrollbarListbox.o: ../src/GUI/ScrollbarListbox.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/ScrollbarListbox.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/ScrollbarListbox.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/ScrollbarListbox.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/ScrollbarListbox.o.d" -o ${OBJECTDIR}/_ext/659824929/ScrollbarListbox.o ../src/GUI/ScrollbarListbox.c     
	
${OBJECTDIR}/_ext/659824929/WallpaperPage.o: ../src/GUI/WallpaperPage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/WallpaperPage.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/WallpaperPage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/WallpaperPage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/WallpaperPage.o.d" -o ${OBJECTDIR}/_ext/659824929/WallpaperPage.o ../src/GUI/WallpaperPage.c     
	
${OBJECTDIR}/_ext/659824929/HideUnhideScreen.o: ../src/GUI/HideUnhideScreen.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/HideUnhideScreen.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/HideUnhideScreen.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/HideUnhideScreen.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/HideUnhideScreen.o.d" -o ${OBJECTDIR}/_ext/659824929/HideUnhideScreen.o ../src/GUI/HideUnhideScreen.c     
	
${OBJECTDIR}/_ext/659824929/PopUpMessagePage.o: ../src/GUI/PopUpMessagePage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/PopUpMessagePage.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/PopUpMessagePage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/PopUpMessagePage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/PopUpMessagePage.o.d" -o ${OBJECTDIR}/_ext/659824929/PopUpMessagePage.o ../src/GUI/PopUpMessagePage.c     
	
${OBJECTDIR}/_ext/659824929/BluetoothPage.o: ../src/GUI/BluetoothPage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/BluetoothPage.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/BluetoothPage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/BluetoothPage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/BluetoothPage.o.d" -o ${OBJECTDIR}/_ext/659824929/BluetoothPage.o ../src/GUI/BluetoothPage.c     
	
${OBJECTDIR}/_ext/659824929/MotionPage.o: ../src/GUI/MotionPage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/MotionPage.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/MotionPage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/MotionPage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/MotionPage.o.d" -o ${OBJECTDIR}/_ext/659824929/MotionPage.o ../src/GUI/MotionPage.c     
	
${OBJECTDIR}/_ext/659824929/UserProgramPage.o: ../src/GUI/UserProgramPage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/UserProgramPage.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/UserProgramPage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/UserProgramPage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/UserProgramPage.o.d" -o ${OBJECTDIR}/_ext/659824929/UserProgramPage.o ../src/GUI/UserProgramPage.c     
	
${OBJECTDIR}/_ext/659824929/Screenshot.o: ../src/GUI/Screenshot.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/Screenshot.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/Screenshot.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/Screenshot.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/Screenshot.o.d" -o ${OBJECTDIR}/_ext/659824929/Screenshot.o ../src/GUI/Screenshot.c     
	
${OBJECTDIR}/_ext/659826615/i2c1.o: ../src/IMU/i2c1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659826615" 
	@${RM} ${OBJECTDIR}/_ext/659826615/i2c1.o.d 
	@${RM} ${OBJECTDIR}/_ext/659826615/i2c1.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659826615/i2c1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659826615/i2c1.o.d" -o ${OBJECTDIR}/_ext/659826615/i2c1.o ../src/IMU/i2c1.c     
	
${OBJECTDIR}/_ext/659826615/inv_mpu.o: ../src/IMU/inv_mpu.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659826615" 
	@${RM} ${OBJECTDIR}/_ext/659826615/inv_mpu.o.d 
	@${RM} ${OBJECTDIR}/_ext/659826615/inv_mpu.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659826615/inv_mpu.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659826615/inv_mpu.o.d" -o ${OBJECTDIR}/_ext/659826615/inv_mpu.o ../src/IMU/inv_mpu.c     
	
${OBJECTDIR}/_ext/659826615/inv_mpu_dmp_motion_driver.o: ../src/IMU/inv_mpu_dmp_motion_driver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659826615" 
	@${RM} ${OBJECTDIR}/_ext/659826615/inv_mpu_dmp_motion_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/659826615/inv_mpu_dmp_motion_driver.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659826615/inv_mpu_dmp_motion_driver.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659826615/inv_mpu_dmp_motion_driver.o.d" -o ${OBJECTDIR}/_ext/659826615/inv_mpu_dmp_motion_driver.o ../src/IMU/inv_mpu_dmp_motion_driver.c     
	
${OBJECTDIR}/_ext/659826615/imu.o: ../src/IMU/imu.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659826615" 
	@${RM} ${OBJECTDIR}/_ext/659826615/imu.o.d 
	@${RM} ${OBJECTDIR}/_ext/659826615/imu.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659826615/imu.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659826615/imu.o.d" -o ${OBJECTDIR}/_ext/659826615/imu.o ../src/IMU/imu.c     
	
${OBJECTDIR}/_ext/620447172/TimeDelay.o: ../src/Microchip/Common/TimeDelay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/620447172" 
	@${RM} ${OBJECTDIR}/_ext/620447172/TimeDelay.o.d 
	@${RM} ${OBJECTDIR}/_ext/620447172/TimeDelay.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/620447172/TimeDelay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/620447172/TimeDelay.o.d" -o ${OBJECTDIR}/_ext/620447172/TimeDelay.o ../src/Microchip/Common/TimeDelay.c     
	
${OBJECTDIR}/_ext/1336470976/ILI9341.o: ../src/Microchip/Graphics/Drivers/ILI9341.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1336470976" 
	@${RM} ${OBJECTDIR}/_ext/1336470976/ILI9341.o.d 
	@${RM} ${OBJECTDIR}/_ext/1336470976/ILI9341.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1336470976/ILI9341.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1336470976/ILI9341.o.d" -o ${OBJECTDIR}/_ext/1336470976/ILI9341.o ../src/Microchip/Graphics/Drivers/ILI9341.c     
	
${OBJECTDIR}/_ext/47893468/Button.o: ../src/Microchip/Graphics/Button.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/Button.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/Button.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/Button.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/Button.o.d" -o ${OBJECTDIR}/_ext/47893468/Button.o ../src/Microchip/Graphics/Button.c     
	
${OBJECTDIR}/_ext/47893468/Chart.o: ../src/Microchip/Graphics/Chart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/Chart.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/Chart.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/Chart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/Chart.o.d" -o ${OBJECTDIR}/_ext/47893468/Chart.o ../src/Microchip/Graphics/Chart.c     
	
${OBJECTDIR}/_ext/47893468/CheckBox.o: ../src/Microchip/Graphics/CheckBox.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/CheckBox.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/CheckBox.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/CheckBox.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/CheckBox.o.d" -o ${OBJECTDIR}/_ext/47893468/CheckBox.o ../src/Microchip/Graphics/CheckBox.c     
	
${OBJECTDIR}/_ext/47893468/DigitalMeter.o: ../src/Microchip/Graphics/DigitalMeter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/DigitalMeter.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/DigitalMeter.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/DigitalMeter.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/DigitalMeter.o.d" -o ${OBJECTDIR}/_ext/47893468/DigitalMeter.o ../src/Microchip/Graphics/DigitalMeter.c     
	
${OBJECTDIR}/_ext/47893468/EditBox.o: ../src/Microchip/Graphics/EditBox.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/EditBox.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/EditBox.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/EditBox.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/EditBox.o.d" -o ${OBJECTDIR}/_ext/47893468/EditBox.o ../src/Microchip/Graphics/EditBox.c     
	
${OBJECTDIR}/_ext/47893468/GOL.o: ../src/Microchip/Graphics/GOL.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/GOL.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/GOL.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/GOL.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/GOL.o.d" -o ${OBJECTDIR}/_ext/47893468/GOL.o ../src/Microchip/Graphics/GOL.c     
	
${OBJECTDIR}/_ext/47893468/GOLSchemeDefault.o: ../src/Microchip/Graphics/GOLSchemeDefault.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/GOLSchemeDefault.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/GOLSchemeDefault.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/GOLSchemeDefault.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/GOLSchemeDefault.o.d" -o ${OBJECTDIR}/_ext/47893468/GOLSchemeDefault.o ../src/Microchip/Graphics/GOLSchemeDefault.c     
	
${OBJECTDIR}/_ext/47893468/Grid.o: ../src/Microchip/Graphics/Grid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/Grid.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/Grid.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/Grid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/Grid.o.d" -o ${OBJECTDIR}/_ext/47893468/Grid.o ../src/Microchip/Graphics/Grid.c     
	
${OBJECTDIR}/_ext/47893468/GroupBox.o: ../src/Microchip/Graphics/GroupBox.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/GroupBox.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/GroupBox.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/GroupBox.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/GroupBox.o.d" -o ${OBJECTDIR}/_ext/47893468/GroupBox.o ../src/Microchip/Graphics/GroupBox.c     
	
${OBJECTDIR}/_ext/47893468/ListBox.o: ../src/Microchip/Graphics/ListBox.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/ListBox.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/ListBox.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/ListBox.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/ListBox.o.d" -o ${OBJECTDIR}/_ext/47893468/ListBox.o ../src/Microchip/Graphics/ListBox.c     
	
${OBJECTDIR}/_ext/47893468/Meter.o: ../src/Microchip/Graphics/Meter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/Meter.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/Meter.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/Meter.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/Meter.o.d" -o ${OBJECTDIR}/_ext/47893468/Meter.o ../src/Microchip/Graphics/Meter.c     
	
${OBJECTDIR}/_ext/47893468/Palette.o: ../src/Microchip/Graphics/Palette.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/Palette.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/Palette.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/Palette.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/Palette.o.d" -o ${OBJECTDIR}/_ext/47893468/Palette.o ../src/Microchip/Graphics/Palette.c     
	
${OBJECTDIR}/_ext/47893468/Picture.o: ../src/Microchip/Graphics/Picture.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/Picture.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/Picture.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/Picture.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/Picture.o.d" -o ${OBJECTDIR}/_ext/47893468/Picture.o ../src/Microchip/Graphics/Picture.c     
	
${OBJECTDIR}/_ext/47893468/Primitive.o: ../src/Microchip/Graphics/Primitive.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/Primitive.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/Primitive.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/Primitive.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/Primitive.o.d" -o ${OBJECTDIR}/_ext/47893468/Primitive.o ../src/Microchip/Graphics/Primitive.c     
	
${OBJECTDIR}/_ext/47893468/ProgressBar.o: ../src/Microchip/Graphics/ProgressBar.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/ProgressBar.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/ProgressBar.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/ProgressBar.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/ProgressBar.o.d" -o ${OBJECTDIR}/_ext/47893468/ProgressBar.o ../src/Microchip/Graphics/ProgressBar.c     
	
${OBJECTDIR}/_ext/47893468/RadioButton.o: ../src/Microchip/Graphics/RadioButton.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/RadioButton.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/RadioButton.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/RadioButton.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/RadioButton.o.d" -o ${OBJECTDIR}/_ext/47893468/RadioButton.o ../src/Microchip/Graphics/RadioButton.c     
	
${OBJECTDIR}/_ext/47893468/RoundDial.o: ../src/Microchip/Graphics/RoundDial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/RoundDial.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/RoundDial.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/RoundDial.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/RoundDial.o.d" -o ${OBJECTDIR}/_ext/47893468/RoundDial.o ../src/Microchip/Graphics/RoundDial.c     
	
${OBJECTDIR}/_ext/47893468/Slider.o: ../src/Microchip/Graphics/Slider.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/Slider.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/Slider.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/Slider.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/Slider.o.d" -o ${OBJECTDIR}/_ext/47893468/Slider.o ../src/Microchip/Graphics/Slider.c     
	
${OBJECTDIR}/_ext/47893468/StaticText.o: ../src/Microchip/Graphics/StaticText.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/StaticText.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/StaticText.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/StaticText.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/StaticText.o.d" -o ${OBJECTDIR}/_ext/47893468/StaticText.o ../src/Microchip/Graphics/StaticText.c     
	
${OBJECTDIR}/_ext/47893468/Window.o: ../src/Microchip/Graphics/Window.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/Window.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/Window.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/Window.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/Window.o.d" -o ${OBJECTDIR}/_ext/47893468/Window.o ../src/Microchip/Graphics/Window.c     
	
${OBJECTDIR}/_ext/219152179/BmpDecoder.o: ../src/Microchip/Image\ Decoders/BmpDecoder.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/219152179" 
	@${RM} ${OBJECTDIR}/_ext/219152179/BmpDecoder.o.d 
	@${RM} ${OBJECTDIR}/_ext/219152179/BmpDecoder.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/219152179/BmpDecoder.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/219152179/BmpDecoder.o.d" -o ${OBJECTDIR}/_ext/219152179/BmpDecoder.o "../src/Microchip/Image Decoders/BmpDecoder.c"     
	
${OBJECTDIR}/_ext/219152179/ImageDecoder.o: ../src/Microchip/Image\ Decoders/ImageDecoder.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/219152179" 
	@${RM} ${OBJECTDIR}/_ext/219152179/ImageDecoder.o.d 
	@${RM} ${OBJECTDIR}/_ext/219152179/ImageDecoder.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/219152179/ImageDecoder.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/219152179/ImageDecoder.o.d" -o ${OBJECTDIR}/_ext/219152179/ImageDecoder.o "../src/Microchip/Image Decoders/ImageDecoder.c"     
	
${OBJECTDIR}/_ext/219152179/JpegDecoder.o: ../src/Microchip/Image\ Decoders/JpegDecoder.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/219152179" 
	@${RM} ${OBJECTDIR}/_ext/219152179/JpegDecoder.o.d 
	@${RM} ${OBJECTDIR}/_ext/219152179/JpegDecoder.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/219152179/JpegDecoder.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/219152179/JpegDecoder.o.d" -o ${OBJECTDIR}/_ext/219152179/JpegDecoder.o "../src/Microchip/Image Decoders/JpegDecoder.c"     
	
${OBJECTDIR}/_ext/219152179/jidctint.o: ../src/Microchip/Image\ Decoders/jidctint.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/219152179" 
	@${RM} ${OBJECTDIR}/_ext/219152179/jidctint.o.d 
	@${RM} ${OBJECTDIR}/_ext/219152179/jidctint.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/219152179/jidctint.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/219152179/jidctint.o.d" -o ${OBJECTDIR}/_ext/219152179/jidctint.o "../src/Microchip/Image Decoders/jidctint.c"     
	
${OBJECTDIR}/_ext/659191409/usb_function_hid.o: ../src/Microchip/USB/HID\ Device\ Driver/usb_function_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659191409" 
	@${RM} ${OBJECTDIR}/_ext/659191409/usb_function_hid.o.d 
	@${RM} ${OBJECTDIR}/_ext/659191409/usb_function_hid.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659191409/usb_function_hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659191409/usb_function_hid.o.d" -o ${OBJECTDIR}/_ext/659191409/usb_function_hid.o "../src/Microchip/USB/HID Device Driver/usb_function_hid.c"     
	
${OBJECTDIR}/_ext/1974569172/usb_function_msd_multi_sector.o: ../src/Microchip/USB/MSD\ Device\ Driver/usb_function_msd_multi_sector.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1974569172" 
	@${RM} ${OBJECTDIR}/_ext/1974569172/usb_function_msd_multi_sector.o.d 
	@${RM} ${OBJECTDIR}/_ext/1974569172/usb_function_msd_multi_sector.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1974569172/usb_function_msd_multi_sector.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1974569172/usb_function_msd_multi_sector.o.d" -o ${OBJECTDIR}/_ext/1974569172/usb_function_msd_multi_sector.o "../src/Microchip/USB/MSD Device Driver/usb_function_msd_multi_sector.c"     
	
${OBJECTDIR}/_ext/2140021685/usb_descriptors.o: ../src/Microchip/USB/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2140021685" 
	@${RM} ${OBJECTDIR}/_ext/2140021685/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/_ext/2140021685/usb_descriptors.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2140021685/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/2140021685/usb_descriptors.o.d" -o ${OBJECTDIR}/_ext/2140021685/usb_descriptors.o ../src/Microchip/USB/usb_descriptors.c     
	
${OBJECTDIR}/_ext/2140021685/usb_device.o: ../src/Microchip/USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2140021685" 
	@${RM} ${OBJECTDIR}/_ext/2140021685/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/2140021685/usb_device.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2140021685/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/2140021685/usb_device.o.d" -o ${OBJECTDIR}/_ext/2140021685/usb_device.o ../src/Microchip/USB/usb_device.c     
	
${OBJECTDIR}/_ext/1094399317/TouchTone.o: ../src/Sound\ Stream/TouchTone.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1094399317" 
	@${RM} ${OBJECTDIR}/_ext/1094399317/TouchTone.o.d 
	@${RM} ${OBJECTDIR}/_ext/1094399317/TouchTone.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1094399317/TouchTone.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1094399317/TouchTone.o.d" -o ${OBJECTDIR}/_ext/1094399317/TouchTone.o "../src/Sound Stream/TouchTone.c"     
	
${OBJECTDIR}/_ext/1094399317/Startup.o: ../src/Sound\ Stream/Startup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1094399317" 
	@${RM} ${OBJECTDIR}/_ext/1094399317/Startup.o.d 
	@${RM} ${OBJECTDIR}/_ext/1094399317/Startup.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1094399317/Startup.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1094399317/Startup.o.d" -o ${OBJECTDIR}/_ext/1094399317/Startup.o "../src/Sound Stream/Startup.c"     
	
${OBJECTDIR}/_ext/1094399317/Shutdown.o: ../src/Sound\ Stream/Shutdown.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1094399317" 
	@${RM} ${OBJECTDIR}/_ext/1094399317/Shutdown.o.d 
	@${RM} ${OBJECTDIR}/_ext/1094399317/Shutdown.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1094399317/Shutdown.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1094399317/Shutdown.o.d" -o ${OBJECTDIR}/_ext/1094399317/Shutdown.o "../src/Sound Stream/Shutdown.c"     
	
${OBJECTDIR}/_ext/1094399317/Error.o: ../src/Sound\ Stream/Error.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1094399317" 
	@${RM} ${OBJECTDIR}/_ext/1094399317/Error.o.d 
	@${RM} ${OBJECTDIR}/_ext/1094399317/Error.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1094399317/Error.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1094399317/Error.o.d" -o ${OBJECTDIR}/_ext/1094399317/Error.o "../src/Sound Stream/Error.c"     
	
${OBJECTDIR}/_ext/791854511/TouchScreen.o: ../src/Touchscreen/TouchScreen.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/791854511" 
	@${RM} ${OBJECTDIR}/_ext/791854511/TouchScreen.o.d 
	@${RM} ${OBJECTDIR}/_ext/791854511/TouchScreen.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/791854511/TouchScreen.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/791854511/TouchScreen.o.d" -o ${OBJECTDIR}/_ext/791854511/TouchScreen.o ../src/Touchscreen/TouchScreen.c     
	
${OBJECTDIR}/_ext/791854511/TouchScreenResistive.o: ../src/Touchscreen/TouchScreenResistive.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/791854511" 
	@${RM} ${OBJECTDIR}/_ext/791854511/TouchScreenResistive.o.d 
	@${RM} ${OBJECTDIR}/_ext/791854511/TouchScreenResistive.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/791854511/TouchScreenResistive.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/791854511/TouchScreenResistive.o.d" -o ${OBJECTDIR}/_ext/791854511/TouchScreenResistive.o ../src/Touchscreen/TouchScreenResistive.c     
	
${OBJECTDIR}/_ext/1707987343/TouchScreenTask.o: ../src/TouchScreen/TouchScreenTask.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1707987343" 
	@${RM} ${OBJECTDIR}/_ext/1707987343/TouchScreenTask.o.d 
	@${RM} ${OBJECTDIR}/_ext/1707987343/TouchScreenTask.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1707987343/TouchScreenTask.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1707987343/TouchScreenTask.o.d" -o ${OBJECTDIR}/_ext/1707987343/TouchScreenTask.o ../src/TouchScreen/TouchScreenTask.c     
	
${OBJECTDIR}/_ext/1360937237/Main.o: ../src/Main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/Main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/Main.o.d" -o ${OBJECTDIR}/_ext/1360937237/Main.o ../src/Main.c     
	
${OBJECTDIR}/_ext/1360937237/Variables.o: ../src/Variables.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Variables.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Variables.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/Variables.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/Variables.o.d" -o ${OBJECTDIR}/_ext/1360937237/Variables.o ../src/Variables.c     
	
${OBJECTDIR}/_ext/1360937237/Background.o: ../src/Background.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Background.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Background.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/Background.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/Background.o.d" -o ${OBJECTDIR}/_ext/1360937237/Background.o ../src/Background.c     
	
${OBJECTDIR}/_ext/1360937237/SetPath.o: ../src/SetPath.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/SetPath.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/SetPath.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/SetPath.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/SetPath.o.d" -o ${OBJECTDIR}/_ext/1360937237/SetPath.o ../src/SetPath.c     
	
${OBJECTDIR}/_ext/1360937237/PlayMotion.o: ../src/PlayMotion.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/PlayMotion.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/PlayMotion.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/PlayMotion.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/PlayMotion.o.d" -o ${OBJECTDIR}/_ext/1360937237/PlayMotion.o ../src/PlayMotion.c     
	
${OBJECTDIR}/_ext/1360937237/UsbApp.o: ../src/UsbApp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/UsbApp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/UsbApp.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/UsbApp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/UsbApp.o.d" -o ${OBJECTDIR}/_ext/1360937237/UsbApp.o ../src/UsbApp.c     
	
${OBJECTDIR}/_ext/1360937237/Power.o: ../src/Power.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Power.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Power.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/Power.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/Power.o.d" -o ${OBJECTDIR}/_ext/1360937237/Power.o ../src/Power.c     
	
${OBJECTDIR}/_ext/1360937237/Planner.o: ../src/Planner.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Planner.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Planner.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/Planner.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/Planner.o.d" -o ${OBJECTDIR}/_ext/1360937237/Planner.o ../src/Planner.c     
	
${OBJECTDIR}/_ext/1360937237/Log.o: ../src/Log.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Log.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Log.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/Log.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/Log.o.d" -o ${OBJECTDIR}/_ext/1360937237/Log.o ../src/Log.c     
	
${OBJECTDIR}/_ext/1360937237/Protocol.o: ../src/Protocol.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Protocol.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Protocol.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/Protocol.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/Protocol.o.d" -o ${OBJECTDIR}/_ext/1360937237/Protocol.o ../src/Protocol.c     
	
${OBJECTDIR}/_ext/1360937237/Adc.o: ../src/Adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Adc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Adc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/Adc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/Adc.o.d" -o ${OBJECTDIR}/_ext/1360937237/Adc.o ../src/Adc.c     
	
${OBJECTDIR}/_ext/1360937237/ErrorTrap.o: ../src/ErrorTrap.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ErrorTrap.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ErrorTrap.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/ErrorTrap.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/ErrorTrap.o.d" -o ${OBJECTDIR}/_ext/1360937237/ErrorTrap.o ../src/ErrorTrap.c     
	
${OBJECTDIR}/_ext/1360937237/AudioAmp.o: ../src/AudioAmp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/AudioAmp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/AudioAmp.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/AudioAmp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/AudioAmp.o.d" -o ${OBJECTDIR}/_ext/1360937237/AudioAmp.o ../src/AudioAmp.c     
	
${OBJECTDIR}/_ext/1360937237/UserProgram.o: ../src/UserProgram.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/UserProgram.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/UserProgram.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/UserProgram.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/UserProgram.o.d" -o ${OBJECTDIR}/_ext/1360937237/UserProgram.o ../src/UserProgram.c     
	
${OBJECTDIR}/_ext/1360937237/Configuration.o: ../src/Configuration.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Configuration.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Configuration.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/Configuration.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/Configuration.o.d" -o ${OBJECTDIR}/_ext/1360937237/Configuration.o ../src/Configuration.c     
	
${OBJECTDIR}/_ext/1360937237/TeachMotion.o: ../src/TeachMotion.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/TeachMotion.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/TeachMotion.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/TeachMotion.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/TeachMotion.o.d" -o ${OBJECTDIR}/_ext/1360937237/TeachMotion.o ../src/TeachMotion.c     
	
${OBJECTDIR}/_ext/1094399317/Beep.o: ../src/Sound\ Stream/Beep.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1094399317" 
	@${RM} ${OBJECTDIR}/_ext/1094399317/Beep.o.d 
	@${RM} ${OBJECTDIR}/_ext/1094399317/Beep.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1094399317/Beep.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1094399317/Beep.o.d" -o ${OBJECTDIR}/_ext/1094399317/Beep.o "../src/Sound Stream/Beep.c"     
	
else
${OBJECTDIR}/_ext/302797332/BluetoothControl.o: ../src/Bluetooth/BluetoothControl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/302797332" 
	@${RM} ${OBJECTDIR}/_ext/302797332/BluetoothControl.o.d 
	@${RM} ${OBJECTDIR}/_ext/302797332/BluetoothControl.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/302797332/BluetoothControl.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/302797332/BluetoothControl.o.d" -o ${OBJECTDIR}/_ext/302797332/BluetoothControl.o ../src/Bluetooth/BluetoothControl.c     
	
${OBJECTDIR}/_ext/302797332/uart2.o: ../src/Bluetooth/uart2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/302797332" 
	@${RM} ${OBJECTDIR}/_ext/302797332/uart2.o.d 
	@${RM} ${OBJECTDIR}/_ext/302797332/uart2.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/302797332/uart2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/302797332/uart2.o.d" -o ${OBJECTDIR}/_ext/302797332/uart2.o ../src/Bluetooth/uart2.c     
	
${OBJECTDIR}/_ext/1563925460/ccsbcs.o: ../src/FatFs/ccsbcs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1563925460" 
	@${RM} ${OBJECTDIR}/_ext/1563925460/ccsbcs.o.d 
	@${RM} ${OBJECTDIR}/_ext/1563925460/ccsbcs.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1563925460/ccsbcs.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1563925460/ccsbcs.o.d" -o ${OBJECTDIR}/_ext/1563925460/ccsbcs.o ../src/FatFs/ccsbcs.c     
	
${OBJECTDIR}/_ext/1563925460/diskio.o: ../src/FatFs/diskio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1563925460" 
	@${RM} ${OBJECTDIR}/_ext/1563925460/diskio.o.d 
	@${RM} ${OBJECTDIR}/_ext/1563925460/diskio.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1563925460/diskio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1563925460/diskio.o.d" -o ${OBJECTDIR}/_ext/1563925460/diskio.o ../src/FatFs/diskio.c     
	
${OBJECTDIR}/_ext/1563925460/ff.o: ../src/FatFs/ff.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1563925460" 
	@${RM} ${OBJECTDIR}/_ext/1563925460/ff.o.d 
	@${RM} ${OBJECTDIR}/_ext/1563925460/ff.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1563925460/ff.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1563925460/ff.o.d" -o ${OBJECTDIR}/_ext/1563925460/ff.o ../src/FatFs/ff.c     
	
${OBJECTDIR}/_ext/1563925460/SD-SPI.o: ../src/FatFs/SD-SPI.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1563925460" 
	@${RM} ${OBJECTDIR}/_ext/1563925460/SD-SPI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1563925460/SD-SPI.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1563925460/SD-SPI.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1563925460/SD-SPI.o.d" -o ${OBJECTDIR}/_ext/1563925460/SD-SPI.o ../src/FatFs/SD-SPI.c     
	
${OBJECTDIR}/_ext/1563925460/FatFsWrapper.o: ../src/FatFs/FatFsWrapper.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1563925460" 
	@${RM} ${OBJECTDIR}/_ext/1563925460/FatFsWrapper.o.d 
	@${RM} ${OBJECTDIR}/_ext/1563925460/FatFsWrapper.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1563925460/FatFsWrapper.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1563925460/FatFsWrapper.o.d" -o ${OBJECTDIR}/_ext/1563925460/FatFsWrapper.o ../src/FatFs/FatFsWrapper.c     
	
${OBJECTDIR}/_ext/689329905/SplashScreenLogo.o: ../src/Flash\ Image/SplashScreenLogo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/689329905" 
	@${RM} ${OBJECTDIR}/_ext/689329905/SplashScreenLogo.o.d 
	@${RM} ${OBJECTDIR}/_ext/689329905/SplashScreenLogo.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/689329905/SplashScreenLogo.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/689329905/SplashScreenLogo.o.d" -o ${OBJECTDIR}/_ext/689329905/SplashScreenLogo.o "../src/Flash Image/SplashScreenLogo.c"     
	
${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_11.o: ../src/Font/Segoe_UI_Semibold_11.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1020267223" 
	@${RM} ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_11.o.d 
	@${RM} ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_11.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_11.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_11.o.d" -o ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_11.o ../src/Font/Segoe_UI_Semibold_11.c     
	
${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_14.o: ../src/Font/Segoe_UI_Semibold_14.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1020267223" 
	@${RM} ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_14.o.d 
	@${RM} ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_14.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_14.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_14.o.d" -o ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_14.o ../src/Font/Segoe_UI_Semibold_14.c     
	
${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_18.o: ../src/Font/Segoe_UI_Semibold_18.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1020267223" 
	@${RM} ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_18.o.d 
	@${RM} ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_18.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_18.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_18.o.d" -o ${OBJECTDIR}/_ext/1020267223/Segoe_UI_Semibold_18.o ../src/Font/Segoe_UI_Semibold_18.c     
	
${OBJECTDIR}/_ext/573367119/heap_3.o: ../src/FreeRTOS/portable/MemMang/heap_3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/573367119" 
	@${RM} ${OBJECTDIR}/_ext/573367119/heap_3.o.d 
	@${RM} ${OBJECTDIR}/_ext/573367119/heap_3.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/573367119/heap_3.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/573367119/heap_3.o.d" -o ${OBJECTDIR}/_ext/573367119/heap_3.o ../src/FreeRTOS/portable/MemMang/heap_3.c     
	
${OBJECTDIR}/_ext/1447740322/port.o: ../src/FreeRTOS/portable/MPLAB/PIC32MX/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1447740322" 
	@${RM} ${OBJECTDIR}/_ext/1447740322/port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1447740322/port.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1447740322/port.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1447740322/port.o.d" -o ${OBJECTDIR}/_ext/1447740322/port.o ../src/FreeRTOS/portable/MPLAB/PIC32MX/port.c     
	
${OBJECTDIR}/_ext/1592166420/list.o: ../src/FreeRTOS/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1592166420" 
	@${RM} ${OBJECTDIR}/_ext/1592166420/list.o.d 
	@${RM} ${OBJECTDIR}/_ext/1592166420/list.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1592166420/list.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1592166420/list.o.d" -o ${OBJECTDIR}/_ext/1592166420/list.o ../src/FreeRTOS/list.c     
	
${OBJECTDIR}/_ext/1592166420/queue.o: ../src/FreeRTOS/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1592166420" 
	@${RM} ${OBJECTDIR}/_ext/1592166420/queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/1592166420/queue.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1592166420/queue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1592166420/queue.o.d" -o ${OBJECTDIR}/_ext/1592166420/queue.o ../src/FreeRTOS/queue.c     
	
${OBJECTDIR}/_ext/1592166420/tasks.o: ../src/FreeRTOS/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1592166420" 
	@${RM} ${OBJECTDIR}/_ext/1592166420/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1592166420/tasks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1592166420/tasks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1592166420/tasks.o.d" -o ${OBJECTDIR}/_ext/1592166420/tasks.o ../src/FreeRTOS/tasks.c     
	
${OBJECTDIR}/_ext/523260321/G15.o: ../src/G15\ and\ Sensors/G15.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/523260321" 
	@${RM} ${OBJECTDIR}/_ext/523260321/G15.o.d 
	@${RM} ${OBJECTDIR}/_ext/523260321/G15.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/523260321/G15.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/523260321/G15.o.d" -o ${OBJECTDIR}/_ext/523260321/G15.o "../src/G15 and Sensors/G15.c"     
	
${OBJECTDIR}/_ext/523260321/uart5.o: ../src/G15\ and\ Sensors/uart5.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/523260321" 
	@${RM} ${OBJECTDIR}/_ext/523260321/uart5.o.d 
	@${RM} ${OBJECTDIR}/_ext/523260321/uart5.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/523260321/uart5.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/523260321/uart5.o.d" -o ${OBJECTDIR}/_ext/523260321/uart5.o "../src/G15 and Sensors/uart5.c"     
	
${OBJECTDIR}/_ext/523260321/Sensors.o: ../src/G15\ and\ Sensors/Sensors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/523260321" 
	@${RM} ${OBJECTDIR}/_ext/523260321/Sensors.o.d 
	@${RM} ${OBJECTDIR}/_ext/523260321/Sensors.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/523260321/Sensors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/523260321/Sensors.o.d" -o ${OBJECTDIR}/_ext/523260321/Sensors.o "../src/G15 and Sensors/Sensors.c"     
	
${OBJECTDIR}/_ext/523260321/ExtModule.o: ../src/G15\ and\ Sensors/ExtModule.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/523260321" 
	@${RM} ${OBJECTDIR}/_ext/523260321/ExtModule.o.d 
	@${RM} ${OBJECTDIR}/_ext/523260321/ExtModule.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/523260321/ExtModule.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/523260321/ExtModule.o.d" -o ${OBJECTDIR}/_ext/523260321/ExtModule.o "../src/G15 and Sensors/ExtModule.c"     
	
${OBJECTDIR}/_ext/54361528/Battery.o: ../src/GUI/Custom\ Graphic\ Object/Battery.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/54361528" 
	@${RM} ${OBJECTDIR}/_ext/54361528/Battery.o.d 
	@${RM} ${OBJECTDIR}/_ext/54361528/Battery.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/54361528/Battery.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/54361528/Battery.o.d" -o ${OBJECTDIR}/_ext/54361528/Battery.o "../src/GUI/Custom Graphic Object/Battery.c"     
	
${OBJECTDIR}/_ext/659824929/SensorPage.o: ../src/GUI/SensorPage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/SensorPage.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/SensorPage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/SensorPage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/SensorPage.o.d" -o ${OBJECTDIR}/_ext/659824929/SensorPage.o ../src/GUI/SensorPage.c     
	
${OBJECTDIR}/_ext/659824929/GraphicScreens.o: ../src/GUI/GraphicScreens.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/GraphicScreens.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/GraphicScreens.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/GraphicScreens.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/GraphicScreens.o.d" -o ${OBJECTDIR}/_ext/659824929/GraphicScreens.o ../src/GUI/GraphicScreens.c     
	
${OBJECTDIR}/_ext/659824929/G15Page.o: ../src/GUI/G15Page.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/G15Page.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/G15Page.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/G15Page.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/G15Page.o.d" -o ${OBJECTDIR}/_ext/659824929/G15Page.o ../src/GUI/G15Page.c     
	
${OBJECTDIR}/_ext/659824929/SetIdPage.o: ../src/GUI/SetIdPage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/SetIdPage.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/SetIdPage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/SetIdPage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/SetIdPage.o.d" -o ${OBJECTDIR}/_ext/659824929/SetIdPage.o ../src/GUI/SetIdPage.c     
	
${OBJECTDIR}/_ext/659824929/SettingsPage.o: ../src/GUI/SettingsPage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/SettingsPage.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/SettingsPage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/SettingsPage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/SettingsPage.o.d" -o ${OBJECTDIR}/_ext/659824929/SettingsPage.o ../src/GUI/SettingsPage.c     
	
${OBJECTDIR}/_ext/659824929/UsbPage.o: ../src/GUI/UsbPage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/UsbPage.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/UsbPage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/UsbPage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/UsbPage.o.d" -o ${OBJECTDIR}/_ext/659824929/UsbPage.o ../src/GUI/UsbPage.c     
	
${OBJECTDIR}/_ext/659824929/SetBtPinPage.o: ../src/GUI/SetBtPinPage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/SetBtPinPage.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/SetBtPinPage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/SetBtPinPage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/SetBtPinPage.o.d" -o ${OBJECTDIR}/_ext/659824929/SetBtPinPage.o ../src/GUI/SetBtPinPage.c     
	
${OBJECTDIR}/_ext/659824929/NumbericKeypad.o: ../src/GUI/NumbericKeypad.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/NumbericKeypad.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/NumbericKeypad.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/NumbericKeypad.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/NumbericKeypad.o.d" -o ${OBJECTDIR}/_ext/659824929/NumbericKeypad.o ../src/GUI/NumbericKeypad.c     
	
${OBJECTDIR}/_ext/659824929/BtSettingsPage.o: ../src/GUI/BtSettingsPage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/BtSettingsPage.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/BtSettingsPage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/BtSettingsPage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/BtSettingsPage.o.d" -o ${OBJECTDIR}/_ext/659824929/BtSettingsPage.o ../src/GUI/BtSettingsPage.c     
	
${OBJECTDIR}/_ext/659824929/LockScreen.o: ../src/GUI/LockScreen.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/LockScreen.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/LockScreen.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/LockScreen.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/LockScreen.o.d" -o ${OBJECTDIR}/_ext/659824929/LockScreen.o ../src/GUI/LockScreen.c     
	
${OBJECTDIR}/_ext/659824929/MainMenu.o: ../src/GUI/MainMenu.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/MainMenu.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/MainMenu.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/MainMenu.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/MainMenu.o.d" -o ${OBJECTDIR}/_ext/659824929/MainMenu.o ../src/GUI/MainMenu.c     
	
${OBJECTDIR}/_ext/659824929/Scheme.o: ../src/GUI/Scheme.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/Scheme.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/Scheme.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/Scheme.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/Scheme.o.d" -o ${OBJECTDIR}/_ext/659824929/Scheme.o ../src/GUI/Scheme.c     
	
${OBJECTDIR}/_ext/659824929/ScrollbarListbox.o: ../src/GUI/ScrollbarListbox.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/ScrollbarListbox.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/ScrollbarListbox.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/ScrollbarListbox.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/ScrollbarListbox.o.d" -o ${OBJECTDIR}/_ext/659824929/ScrollbarListbox.o ../src/GUI/ScrollbarListbox.c     
	
${OBJECTDIR}/_ext/659824929/WallpaperPage.o: ../src/GUI/WallpaperPage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/WallpaperPage.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/WallpaperPage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/WallpaperPage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/WallpaperPage.o.d" -o ${OBJECTDIR}/_ext/659824929/WallpaperPage.o ../src/GUI/WallpaperPage.c     
	
${OBJECTDIR}/_ext/659824929/HideUnhideScreen.o: ../src/GUI/HideUnhideScreen.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/HideUnhideScreen.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/HideUnhideScreen.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/HideUnhideScreen.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/HideUnhideScreen.o.d" -o ${OBJECTDIR}/_ext/659824929/HideUnhideScreen.o ../src/GUI/HideUnhideScreen.c     
	
${OBJECTDIR}/_ext/659824929/PopUpMessagePage.o: ../src/GUI/PopUpMessagePage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/PopUpMessagePage.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/PopUpMessagePage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/PopUpMessagePage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/PopUpMessagePage.o.d" -o ${OBJECTDIR}/_ext/659824929/PopUpMessagePage.o ../src/GUI/PopUpMessagePage.c     
	
${OBJECTDIR}/_ext/659824929/BluetoothPage.o: ../src/GUI/BluetoothPage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/BluetoothPage.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/BluetoothPage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/BluetoothPage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/BluetoothPage.o.d" -o ${OBJECTDIR}/_ext/659824929/BluetoothPage.o ../src/GUI/BluetoothPage.c     
	
${OBJECTDIR}/_ext/659824929/MotionPage.o: ../src/GUI/MotionPage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/MotionPage.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/MotionPage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/MotionPage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/MotionPage.o.d" -o ${OBJECTDIR}/_ext/659824929/MotionPage.o ../src/GUI/MotionPage.c     
	
${OBJECTDIR}/_ext/659824929/UserProgramPage.o: ../src/GUI/UserProgramPage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/UserProgramPage.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/UserProgramPage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/UserProgramPage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/UserProgramPage.o.d" -o ${OBJECTDIR}/_ext/659824929/UserProgramPage.o ../src/GUI/UserProgramPage.c     
	
${OBJECTDIR}/_ext/659824929/Screenshot.o: ../src/GUI/Screenshot.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659824929" 
	@${RM} ${OBJECTDIR}/_ext/659824929/Screenshot.o.d 
	@${RM} ${OBJECTDIR}/_ext/659824929/Screenshot.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659824929/Screenshot.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659824929/Screenshot.o.d" -o ${OBJECTDIR}/_ext/659824929/Screenshot.o ../src/GUI/Screenshot.c     
	
${OBJECTDIR}/_ext/659826615/i2c1.o: ../src/IMU/i2c1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659826615" 
	@${RM} ${OBJECTDIR}/_ext/659826615/i2c1.o.d 
	@${RM} ${OBJECTDIR}/_ext/659826615/i2c1.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659826615/i2c1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659826615/i2c1.o.d" -o ${OBJECTDIR}/_ext/659826615/i2c1.o ../src/IMU/i2c1.c     
	
${OBJECTDIR}/_ext/659826615/inv_mpu.o: ../src/IMU/inv_mpu.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659826615" 
	@${RM} ${OBJECTDIR}/_ext/659826615/inv_mpu.o.d 
	@${RM} ${OBJECTDIR}/_ext/659826615/inv_mpu.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659826615/inv_mpu.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659826615/inv_mpu.o.d" -o ${OBJECTDIR}/_ext/659826615/inv_mpu.o ../src/IMU/inv_mpu.c     
	
${OBJECTDIR}/_ext/659826615/inv_mpu_dmp_motion_driver.o: ../src/IMU/inv_mpu_dmp_motion_driver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659826615" 
	@${RM} ${OBJECTDIR}/_ext/659826615/inv_mpu_dmp_motion_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/659826615/inv_mpu_dmp_motion_driver.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659826615/inv_mpu_dmp_motion_driver.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659826615/inv_mpu_dmp_motion_driver.o.d" -o ${OBJECTDIR}/_ext/659826615/inv_mpu_dmp_motion_driver.o ../src/IMU/inv_mpu_dmp_motion_driver.c     
	
${OBJECTDIR}/_ext/659826615/imu.o: ../src/IMU/imu.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659826615" 
	@${RM} ${OBJECTDIR}/_ext/659826615/imu.o.d 
	@${RM} ${OBJECTDIR}/_ext/659826615/imu.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659826615/imu.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659826615/imu.o.d" -o ${OBJECTDIR}/_ext/659826615/imu.o ../src/IMU/imu.c     
	
${OBJECTDIR}/_ext/620447172/TimeDelay.o: ../src/Microchip/Common/TimeDelay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/620447172" 
	@${RM} ${OBJECTDIR}/_ext/620447172/TimeDelay.o.d 
	@${RM} ${OBJECTDIR}/_ext/620447172/TimeDelay.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/620447172/TimeDelay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/620447172/TimeDelay.o.d" -o ${OBJECTDIR}/_ext/620447172/TimeDelay.o ../src/Microchip/Common/TimeDelay.c     
	
${OBJECTDIR}/_ext/1336470976/ILI9341.o: ../src/Microchip/Graphics/Drivers/ILI9341.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1336470976" 
	@${RM} ${OBJECTDIR}/_ext/1336470976/ILI9341.o.d 
	@${RM} ${OBJECTDIR}/_ext/1336470976/ILI9341.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1336470976/ILI9341.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1336470976/ILI9341.o.d" -o ${OBJECTDIR}/_ext/1336470976/ILI9341.o ../src/Microchip/Graphics/Drivers/ILI9341.c     
	
${OBJECTDIR}/_ext/47893468/Button.o: ../src/Microchip/Graphics/Button.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/Button.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/Button.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/Button.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/Button.o.d" -o ${OBJECTDIR}/_ext/47893468/Button.o ../src/Microchip/Graphics/Button.c     
	
${OBJECTDIR}/_ext/47893468/Chart.o: ../src/Microchip/Graphics/Chart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/Chart.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/Chart.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/Chart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/Chart.o.d" -o ${OBJECTDIR}/_ext/47893468/Chart.o ../src/Microchip/Graphics/Chart.c     
	
${OBJECTDIR}/_ext/47893468/CheckBox.o: ../src/Microchip/Graphics/CheckBox.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/CheckBox.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/CheckBox.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/CheckBox.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/CheckBox.o.d" -o ${OBJECTDIR}/_ext/47893468/CheckBox.o ../src/Microchip/Graphics/CheckBox.c     
	
${OBJECTDIR}/_ext/47893468/DigitalMeter.o: ../src/Microchip/Graphics/DigitalMeter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/DigitalMeter.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/DigitalMeter.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/DigitalMeter.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/DigitalMeter.o.d" -o ${OBJECTDIR}/_ext/47893468/DigitalMeter.o ../src/Microchip/Graphics/DigitalMeter.c     
	
${OBJECTDIR}/_ext/47893468/EditBox.o: ../src/Microchip/Graphics/EditBox.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/EditBox.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/EditBox.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/EditBox.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/EditBox.o.d" -o ${OBJECTDIR}/_ext/47893468/EditBox.o ../src/Microchip/Graphics/EditBox.c     
	
${OBJECTDIR}/_ext/47893468/GOL.o: ../src/Microchip/Graphics/GOL.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/GOL.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/GOL.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/GOL.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/GOL.o.d" -o ${OBJECTDIR}/_ext/47893468/GOL.o ../src/Microchip/Graphics/GOL.c     
	
${OBJECTDIR}/_ext/47893468/GOLSchemeDefault.o: ../src/Microchip/Graphics/GOLSchemeDefault.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/GOLSchemeDefault.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/GOLSchemeDefault.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/GOLSchemeDefault.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/GOLSchemeDefault.o.d" -o ${OBJECTDIR}/_ext/47893468/GOLSchemeDefault.o ../src/Microchip/Graphics/GOLSchemeDefault.c     
	
${OBJECTDIR}/_ext/47893468/Grid.o: ../src/Microchip/Graphics/Grid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/Grid.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/Grid.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/Grid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/Grid.o.d" -o ${OBJECTDIR}/_ext/47893468/Grid.o ../src/Microchip/Graphics/Grid.c     
	
${OBJECTDIR}/_ext/47893468/GroupBox.o: ../src/Microchip/Graphics/GroupBox.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/GroupBox.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/GroupBox.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/GroupBox.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/GroupBox.o.d" -o ${OBJECTDIR}/_ext/47893468/GroupBox.o ../src/Microchip/Graphics/GroupBox.c     
	
${OBJECTDIR}/_ext/47893468/ListBox.o: ../src/Microchip/Graphics/ListBox.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/ListBox.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/ListBox.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/ListBox.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/ListBox.o.d" -o ${OBJECTDIR}/_ext/47893468/ListBox.o ../src/Microchip/Graphics/ListBox.c     
	
${OBJECTDIR}/_ext/47893468/Meter.o: ../src/Microchip/Graphics/Meter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/Meter.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/Meter.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/Meter.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/Meter.o.d" -o ${OBJECTDIR}/_ext/47893468/Meter.o ../src/Microchip/Graphics/Meter.c     
	
${OBJECTDIR}/_ext/47893468/Palette.o: ../src/Microchip/Graphics/Palette.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/Palette.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/Palette.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/Palette.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/Palette.o.d" -o ${OBJECTDIR}/_ext/47893468/Palette.o ../src/Microchip/Graphics/Palette.c     
	
${OBJECTDIR}/_ext/47893468/Picture.o: ../src/Microchip/Graphics/Picture.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/Picture.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/Picture.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/Picture.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/Picture.o.d" -o ${OBJECTDIR}/_ext/47893468/Picture.o ../src/Microchip/Graphics/Picture.c     
	
${OBJECTDIR}/_ext/47893468/Primitive.o: ../src/Microchip/Graphics/Primitive.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/Primitive.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/Primitive.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/Primitive.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/Primitive.o.d" -o ${OBJECTDIR}/_ext/47893468/Primitive.o ../src/Microchip/Graphics/Primitive.c     
	
${OBJECTDIR}/_ext/47893468/ProgressBar.o: ../src/Microchip/Graphics/ProgressBar.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/ProgressBar.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/ProgressBar.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/ProgressBar.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/ProgressBar.o.d" -o ${OBJECTDIR}/_ext/47893468/ProgressBar.o ../src/Microchip/Graphics/ProgressBar.c     
	
${OBJECTDIR}/_ext/47893468/RadioButton.o: ../src/Microchip/Graphics/RadioButton.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/RadioButton.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/RadioButton.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/RadioButton.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/RadioButton.o.d" -o ${OBJECTDIR}/_ext/47893468/RadioButton.o ../src/Microchip/Graphics/RadioButton.c     
	
${OBJECTDIR}/_ext/47893468/RoundDial.o: ../src/Microchip/Graphics/RoundDial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/RoundDial.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/RoundDial.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/RoundDial.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/RoundDial.o.d" -o ${OBJECTDIR}/_ext/47893468/RoundDial.o ../src/Microchip/Graphics/RoundDial.c     
	
${OBJECTDIR}/_ext/47893468/Slider.o: ../src/Microchip/Graphics/Slider.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/Slider.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/Slider.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/Slider.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/Slider.o.d" -o ${OBJECTDIR}/_ext/47893468/Slider.o ../src/Microchip/Graphics/Slider.c     
	
${OBJECTDIR}/_ext/47893468/StaticText.o: ../src/Microchip/Graphics/StaticText.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/StaticText.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/StaticText.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/StaticText.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/StaticText.o.d" -o ${OBJECTDIR}/_ext/47893468/StaticText.o ../src/Microchip/Graphics/StaticText.c     
	
${OBJECTDIR}/_ext/47893468/Window.o: ../src/Microchip/Graphics/Window.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/47893468" 
	@${RM} ${OBJECTDIR}/_ext/47893468/Window.o.d 
	@${RM} ${OBJECTDIR}/_ext/47893468/Window.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/47893468/Window.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/47893468/Window.o.d" -o ${OBJECTDIR}/_ext/47893468/Window.o ../src/Microchip/Graphics/Window.c     
	
${OBJECTDIR}/_ext/219152179/BmpDecoder.o: ../src/Microchip/Image\ Decoders/BmpDecoder.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/219152179" 
	@${RM} ${OBJECTDIR}/_ext/219152179/BmpDecoder.o.d 
	@${RM} ${OBJECTDIR}/_ext/219152179/BmpDecoder.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/219152179/BmpDecoder.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/219152179/BmpDecoder.o.d" -o ${OBJECTDIR}/_ext/219152179/BmpDecoder.o "../src/Microchip/Image Decoders/BmpDecoder.c"     
	
${OBJECTDIR}/_ext/219152179/ImageDecoder.o: ../src/Microchip/Image\ Decoders/ImageDecoder.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/219152179" 
	@${RM} ${OBJECTDIR}/_ext/219152179/ImageDecoder.o.d 
	@${RM} ${OBJECTDIR}/_ext/219152179/ImageDecoder.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/219152179/ImageDecoder.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/219152179/ImageDecoder.o.d" -o ${OBJECTDIR}/_ext/219152179/ImageDecoder.o "../src/Microchip/Image Decoders/ImageDecoder.c"     
	
${OBJECTDIR}/_ext/219152179/JpegDecoder.o: ../src/Microchip/Image\ Decoders/JpegDecoder.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/219152179" 
	@${RM} ${OBJECTDIR}/_ext/219152179/JpegDecoder.o.d 
	@${RM} ${OBJECTDIR}/_ext/219152179/JpegDecoder.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/219152179/JpegDecoder.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/219152179/JpegDecoder.o.d" -o ${OBJECTDIR}/_ext/219152179/JpegDecoder.o "../src/Microchip/Image Decoders/JpegDecoder.c"     
	
${OBJECTDIR}/_ext/219152179/jidctint.o: ../src/Microchip/Image\ Decoders/jidctint.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/219152179" 
	@${RM} ${OBJECTDIR}/_ext/219152179/jidctint.o.d 
	@${RM} ${OBJECTDIR}/_ext/219152179/jidctint.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/219152179/jidctint.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/219152179/jidctint.o.d" -o ${OBJECTDIR}/_ext/219152179/jidctint.o "../src/Microchip/Image Decoders/jidctint.c"     
	
${OBJECTDIR}/_ext/659191409/usb_function_hid.o: ../src/Microchip/USB/HID\ Device\ Driver/usb_function_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/659191409" 
	@${RM} ${OBJECTDIR}/_ext/659191409/usb_function_hid.o.d 
	@${RM} ${OBJECTDIR}/_ext/659191409/usb_function_hid.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/659191409/usb_function_hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/659191409/usb_function_hid.o.d" -o ${OBJECTDIR}/_ext/659191409/usb_function_hid.o "../src/Microchip/USB/HID Device Driver/usb_function_hid.c"     
	
${OBJECTDIR}/_ext/1974569172/usb_function_msd_multi_sector.o: ../src/Microchip/USB/MSD\ Device\ Driver/usb_function_msd_multi_sector.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1974569172" 
	@${RM} ${OBJECTDIR}/_ext/1974569172/usb_function_msd_multi_sector.o.d 
	@${RM} ${OBJECTDIR}/_ext/1974569172/usb_function_msd_multi_sector.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1974569172/usb_function_msd_multi_sector.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1974569172/usb_function_msd_multi_sector.o.d" -o ${OBJECTDIR}/_ext/1974569172/usb_function_msd_multi_sector.o "../src/Microchip/USB/MSD Device Driver/usb_function_msd_multi_sector.c"     
	
${OBJECTDIR}/_ext/2140021685/usb_descriptors.o: ../src/Microchip/USB/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2140021685" 
	@${RM} ${OBJECTDIR}/_ext/2140021685/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/_ext/2140021685/usb_descriptors.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2140021685/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/2140021685/usb_descriptors.o.d" -o ${OBJECTDIR}/_ext/2140021685/usb_descriptors.o ../src/Microchip/USB/usb_descriptors.c     
	
${OBJECTDIR}/_ext/2140021685/usb_device.o: ../src/Microchip/USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2140021685" 
	@${RM} ${OBJECTDIR}/_ext/2140021685/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/2140021685/usb_device.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2140021685/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/2140021685/usb_device.o.d" -o ${OBJECTDIR}/_ext/2140021685/usb_device.o ../src/Microchip/USB/usb_device.c     
	
${OBJECTDIR}/_ext/1094399317/TouchTone.o: ../src/Sound\ Stream/TouchTone.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1094399317" 
	@${RM} ${OBJECTDIR}/_ext/1094399317/TouchTone.o.d 
	@${RM} ${OBJECTDIR}/_ext/1094399317/TouchTone.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1094399317/TouchTone.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1094399317/TouchTone.o.d" -o ${OBJECTDIR}/_ext/1094399317/TouchTone.o "../src/Sound Stream/TouchTone.c"     
	
${OBJECTDIR}/_ext/1094399317/Startup.o: ../src/Sound\ Stream/Startup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1094399317" 
	@${RM} ${OBJECTDIR}/_ext/1094399317/Startup.o.d 
	@${RM} ${OBJECTDIR}/_ext/1094399317/Startup.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1094399317/Startup.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1094399317/Startup.o.d" -o ${OBJECTDIR}/_ext/1094399317/Startup.o "../src/Sound Stream/Startup.c"     
	
${OBJECTDIR}/_ext/1094399317/Shutdown.o: ../src/Sound\ Stream/Shutdown.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1094399317" 
	@${RM} ${OBJECTDIR}/_ext/1094399317/Shutdown.o.d 
	@${RM} ${OBJECTDIR}/_ext/1094399317/Shutdown.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1094399317/Shutdown.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1094399317/Shutdown.o.d" -o ${OBJECTDIR}/_ext/1094399317/Shutdown.o "../src/Sound Stream/Shutdown.c"     
	
${OBJECTDIR}/_ext/1094399317/Error.o: ../src/Sound\ Stream/Error.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1094399317" 
	@${RM} ${OBJECTDIR}/_ext/1094399317/Error.o.d 
	@${RM} ${OBJECTDIR}/_ext/1094399317/Error.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1094399317/Error.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1094399317/Error.o.d" -o ${OBJECTDIR}/_ext/1094399317/Error.o "../src/Sound Stream/Error.c"     
	
${OBJECTDIR}/_ext/791854511/TouchScreen.o: ../src/Touchscreen/TouchScreen.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/791854511" 
	@${RM} ${OBJECTDIR}/_ext/791854511/TouchScreen.o.d 
	@${RM} ${OBJECTDIR}/_ext/791854511/TouchScreen.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/791854511/TouchScreen.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/791854511/TouchScreen.o.d" -o ${OBJECTDIR}/_ext/791854511/TouchScreen.o ../src/Touchscreen/TouchScreen.c     
	
${OBJECTDIR}/_ext/791854511/TouchScreenResistive.o: ../src/Touchscreen/TouchScreenResistive.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/791854511" 
	@${RM} ${OBJECTDIR}/_ext/791854511/TouchScreenResistive.o.d 
	@${RM} ${OBJECTDIR}/_ext/791854511/TouchScreenResistive.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/791854511/TouchScreenResistive.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/791854511/TouchScreenResistive.o.d" -o ${OBJECTDIR}/_ext/791854511/TouchScreenResistive.o ../src/Touchscreen/TouchScreenResistive.c     
	
${OBJECTDIR}/_ext/1707987343/TouchScreenTask.o: ../src/TouchScreen/TouchScreenTask.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1707987343" 
	@${RM} ${OBJECTDIR}/_ext/1707987343/TouchScreenTask.o.d 
	@${RM} ${OBJECTDIR}/_ext/1707987343/TouchScreenTask.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1707987343/TouchScreenTask.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1707987343/TouchScreenTask.o.d" -o ${OBJECTDIR}/_ext/1707987343/TouchScreenTask.o ../src/TouchScreen/TouchScreenTask.c     
	
${OBJECTDIR}/_ext/1360937237/Main.o: ../src/Main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/Main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/Main.o.d" -o ${OBJECTDIR}/_ext/1360937237/Main.o ../src/Main.c     
	
${OBJECTDIR}/_ext/1360937237/Variables.o: ../src/Variables.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Variables.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Variables.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/Variables.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/Variables.o.d" -o ${OBJECTDIR}/_ext/1360937237/Variables.o ../src/Variables.c     
	
${OBJECTDIR}/_ext/1360937237/Background.o: ../src/Background.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Background.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Background.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/Background.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/Background.o.d" -o ${OBJECTDIR}/_ext/1360937237/Background.o ../src/Background.c     
	
${OBJECTDIR}/_ext/1360937237/SetPath.o: ../src/SetPath.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/SetPath.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/SetPath.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/SetPath.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/SetPath.o.d" -o ${OBJECTDIR}/_ext/1360937237/SetPath.o ../src/SetPath.c     
	
${OBJECTDIR}/_ext/1360937237/PlayMotion.o: ../src/PlayMotion.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/PlayMotion.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/PlayMotion.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/PlayMotion.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/PlayMotion.o.d" -o ${OBJECTDIR}/_ext/1360937237/PlayMotion.o ../src/PlayMotion.c     
	
${OBJECTDIR}/_ext/1360937237/UsbApp.o: ../src/UsbApp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/UsbApp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/UsbApp.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/UsbApp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/UsbApp.o.d" -o ${OBJECTDIR}/_ext/1360937237/UsbApp.o ../src/UsbApp.c     
	
${OBJECTDIR}/_ext/1360937237/Power.o: ../src/Power.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Power.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Power.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/Power.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/Power.o.d" -o ${OBJECTDIR}/_ext/1360937237/Power.o ../src/Power.c     
	
${OBJECTDIR}/_ext/1360937237/Planner.o: ../src/Planner.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Planner.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Planner.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/Planner.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/Planner.o.d" -o ${OBJECTDIR}/_ext/1360937237/Planner.o ../src/Planner.c     
	
${OBJECTDIR}/_ext/1360937237/Log.o: ../src/Log.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Log.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Log.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/Log.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/Log.o.d" -o ${OBJECTDIR}/_ext/1360937237/Log.o ../src/Log.c     
	
${OBJECTDIR}/_ext/1360937237/Protocol.o: ../src/Protocol.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Protocol.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Protocol.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/Protocol.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/Protocol.o.d" -o ${OBJECTDIR}/_ext/1360937237/Protocol.o ../src/Protocol.c     
	
${OBJECTDIR}/_ext/1360937237/Adc.o: ../src/Adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Adc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Adc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/Adc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/Adc.o.d" -o ${OBJECTDIR}/_ext/1360937237/Adc.o ../src/Adc.c     
	
${OBJECTDIR}/_ext/1360937237/ErrorTrap.o: ../src/ErrorTrap.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ErrorTrap.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ErrorTrap.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/ErrorTrap.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/ErrorTrap.o.d" -o ${OBJECTDIR}/_ext/1360937237/ErrorTrap.o ../src/ErrorTrap.c     
	
${OBJECTDIR}/_ext/1360937237/AudioAmp.o: ../src/AudioAmp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/AudioAmp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/AudioAmp.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/AudioAmp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/AudioAmp.o.d" -o ${OBJECTDIR}/_ext/1360937237/AudioAmp.o ../src/AudioAmp.c     
	
${OBJECTDIR}/_ext/1360937237/UserProgram.o: ../src/UserProgram.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/UserProgram.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/UserProgram.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/UserProgram.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/UserProgram.o.d" -o ${OBJECTDIR}/_ext/1360937237/UserProgram.o ../src/UserProgram.c     
	
${OBJECTDIR}/_ext/1360937237/Configuration.o: ../src/Configuration.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Configuration.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Configuration.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/Configuration.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/Configuration.o.d" -o ${OBJECTDIR}/_ext/1360937237/Configuration.o ../src/Configuration.c     
	
${OBJECTDIR}/_ext/1360937237/TeachMotion.o: ../src/TeachMotion.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/TeachMotion.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/TeachMotion.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/TeachMotion.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1360937237/TeachMotion.o.d" -o ${OBJECTDIR}/_ext/1360937237/TeachMotion.o ../src/TeachMotion.c     
	
${OBJECTDIR}/_ext/1094399317/Beep.o: ../src/Sound\ Stream/Beep.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1094399317" 
	@${RM} ${OBJECTDIR}/_ext/1094399317/Beep.o.d 
	@${RM} ${OBJECTDIR}/_ext/1094399317/Beep.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1094399317/Beep.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -DMPU6050 -D_SUPPRESS_PLIB_WARNING -D_DISABLE_OPENADC10_CONFIGPORT_WARNING -I"../include" -I"../include/FreeRTOS" -I"../include/FreeRTOS/portable/MPLAB/PIC32MX" -I"../include/Microchip" -I"../include/IMU" -MMD -MF "${OBJECTDIR}/_ext/1094399317/Beep.o.d" -o ${OBJECTDIR}/_ext/1094399317/Beep.o "../src/Sound Stream/Beep.c"     
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/Rero_Main_Controller_Firmware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../app_32MX575F512L.ld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_SIMULATOR=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Rero_Main_Controller_Firmware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}                -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_SIMULATOR=1,--defsym=_min_heap_size=45000,-L"/C:/Program Files/Microchip/MPLAB C32 Suite/lib",-L"/C:/Program Files/Microchip/MPLAB C32 Suite/pic32mx/lib",-Map="${DISTDIR}/Rero_Main_Controller_Firmware.X.${IMAGE_TYPE}.map"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/Rero_Main_Controller_Firmware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../app_32MX575F512L.ld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Rero_Main_Controller_Firmware.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}            -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=45000,-L"/C:/Program Files/Microchip/MPLAB C32 Suite/lib",-L"/C:/Program Files/Microchip/MPLAB C32 Suite/pic32mx/lib",-Map="${DISTDIR}/Rero_Main_Controller_Firmware.X.${IMAGE_TYPE}.map"
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/Rero_Main_Controller_Firmware.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif

.pre:
	@echo "--------------------------------------"
	@echo "User defined pre-build step: []"
	@
	@echo "--------------------------------------"

# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
