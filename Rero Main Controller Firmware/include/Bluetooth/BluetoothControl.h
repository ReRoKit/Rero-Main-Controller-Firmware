/*******************************************************************************
 * This file provides the functions for Bluetooth control.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef BLUETOOTHCONTROL_H
#define	BLUETOOTHCONTROL_H



/*******************************************************************************
 * PUBLIC DEFINITION
 *******************************************************************************/

// Bluetooth module operation mode.
typedef enum __attribute__((packed)) {
    TRANSPARENT = 0,
    AT = 1
} BLUETOOTH_MODE;

// Bluetooth version.
typedef enum __attribute__((packed)) {
    UNKNOWN,
    BT_V20_HC05,    //Classic EDR
    BT_V40_HM12,    //Dual Mode EDR+BLE
    BT_V40_HM10,    //BLE
    BT_V40_BT05,    //BLE
    BT_V40_USRBLE100    //BLE
} BLUETOOTH_VERSION;



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

BLUETOOTH_VERSION eGetBluetoothVersion(void);
void vEnableBluetooth(BLUETOOTH_MODE eBtMode);
void vDisableBluetooth(void);
void vResetBluetooth(BLUETOOTH_MODE eBtMode);
void vConfigureBluetooth(void);
void vGetBluetoothPin(char* pszPin);
unsigned char ucSetBluetoothPin(const char* szPin);
void taskBluetoothProgram(void *pvParameters);



#endif	/* BLUETOOTHCONTROL_H */
