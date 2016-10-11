/*******************************************************************************
 * Common functions for the external module such as G15 and sensors.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef EXTMODULE_H
#define	EXTMODULE_H



/*******************************************************************************
 * PUBLIC DEFINITION
 *******************************************************************************/

// Broadcast ID.
#define EM_BROADCAST_ID   254

// Maximum servo ID.
#define EM_MAX_ID         253



// External module model.
typedef enum __attribute__((packed)) {
    EM_MODEL_NONE,                  // None.
    EM_MODEL_IMU,                   // Althought IMU is not an external module, we put it here for easier grouping.
    EM_MODEL_G15        = 0x470f,   // G15.
    EM_MODEL_IR         = 0x4952,   // Infrared sensor.
    EM_MODEL_US         = 0x5553,   // Ultrasonic sensor.
    EM_MODEL_HEAD       = 0x484d,   // Head Module.
    EM_MODEL_HM_MIC,                // Microphone on head module.
    EM_MODEL_HM_IR,                 // Infrared sensor on head module.
    EM_MODEL_TACTILE    = 0x5453,   // Tactile sensor.
    EM_MODEL_COLOUR     = 0x4353,   // Colour sensor.
    EM_MODEL_LINE       = 0x4c53,   // Line sensor.
    EM_MODEL_RGBLIGHT   = 0x4c4d    // RGB Light Module.
} EM_MODEL;


// Instruction.
typedef enum __attribute__((packed)) {
    EM_INST_PING        = 0x01,     // Read the status.
    EM_INST_READ_DATA   = 0x02,     // Read from Control Table.
    EM_INST_WRITE_DATA  = 0x03,     // Write to Control Table.
    EM_INST_REG_WRITE   = 0x04,     // Write and wait for ACTION instruction.
    EM_INST_ACTION      = 0x05,     // Triggers REG_WRITE instruction.
    EM_INST_RESET       = 0x06,     // Reset to factory defaults.
    EM_INST_SYNC_WRITE  = 0x83      // Control multiple external modules simultaneously.
} EM_INSTRUCTION;


// Write mode.
typedef enum __attribute__((packed)) {
    WRITE_NOW   = 0x00,     // Write to control table immediately.
    WAIT_ACTION = 0x01      // Write to temporary register and wait for action command.
} EM_WRITE_MODE;


// Error Code.
typedef enum __attribute__((packed)) {
    EM_NO_ERROR             = 0x0000,

    EM_ERR_INPUT_VOLTAGE    = 0x0001,
    EM_ERR_ANGLE_LIMIT      = 0x0002,
    EM_ERR_OVERHEATING      = 0x0004,
    EM_ERR_RANGE            = 0x0008,

    EM_ERR_TX_CHECKSUM      = 0x0010,
    EM_ERR_OVERLOAD         = 0x0020,
    EM_ERR_INSTRUCTION      = 0x0040,

    EM_ERR_RX_TIMEOUT       = 0x0100,
    EM_ERR_RX_HEADER        = 0x0200,
    EM_ERR_RX_ID            = 0x0400,
    EM_ERR_RX_CHECKSUM      = 0x0800,

    EM_ERR_TX_BUSY          = 0x1000,

    EM_ERR_INVALID_MODULE   = 0x2000
} EM_ERROR;



// Common Control Table Address.
typedef enum __attribute__((packed)) {
    EM_ADD_MODEL_L          = 0x00,
    EM_ADD_MODEL_H          = 0x01,
    EM_ADD_VERSION          = 0x02,
    EM_ADD_ID               = 0x03,
    EM_ADD_BAUD_RATE        = 0x04,
    EM_ADD_RETURN_DELAY     = 0x05
} EM_COMMON_CT_ADD;



// Macro to switch between Tx and Rx.
#define vTxMode()           EXT_UART_DIR = 0
#define vRxMode()           EXT_UART_DIR = 1



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

void vEMInitWorkaround(void);
EM_ERROR eEMBoostBaudrate(void);
EM_ERROR eEMOriBaudrate(void);
void vEMDisableAllOutput(void);
EM_ERROR eEMPing(unsigned char ucId);
EM_ERROR eEMGetModel(unsigned char ucId, EM_MODEL *peModel);
EM_ERROR eEMSetId(unsigned char ucCurrentId, unsigned char ucNewId);
EM_ERROR eEMSetAction(void);
EM_ERROR eEMFactoryReset(unsigned char ucId);
EM_ERROR eEMSendReceivePacket(unsigned char ucId, EM_INSTRUCTION eInstruction, unsigned char ucParamLength,
                                 unsigned char *pucTxParam, unsigned char *pucRxData);



#endif	/* EXTMODULE_H */
