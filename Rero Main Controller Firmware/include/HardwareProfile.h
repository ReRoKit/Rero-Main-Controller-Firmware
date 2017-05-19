/*******************************************************************************
 * This file describes the hardware configuration and system properties.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef HARDWARE_PROFILE_H
#define	HARDWARE_PROFILE_H

#include <Compiler.h>
#include "Microchip/Graphics/HWP_GFXv3_EX16_16PMP_QVGAv1.h"



/*******************************************************************************
 * GetSystemClock() returns system clock frequency.
 *
 * GetPeripheralClock() returns peripheral clock frequency.
 *
 * GetInstructionClock() returns instruction clock frequency.
 *
 *******************************************************************************/
#define GetSystemClock()        (80000000ul)
#define GetPeripheralClock()    (GetSystemClock() / (1 << OSCCONbits.PBDIV))
#define GetInstructionClock()   (GetSystemClock())



/*******************************************************************************
 * Program button
 *******************************************************************************/
#define PROGRAM_BUTTON          PORTGbits.RG15
#define PROGRAM_BUTTON_TRIS     TRISGbits.TRISG15



/*******************************************************************************
 * RGB LED
 *******************************************************************************/
#define LEDR                    LATDbits.LATD1
#define LEDR_TRIS               TRISDbits.TRISD1

#define LEDG                    LATDbits.LATD2
#define LEDG_TRIS               TRISDbits.TRISD2

#define LEDB                    LATCbits.LATC2
#define LEDB_TRIS               TRISCbits.TRISC2

#define L_ON                    0
#define L_OFF                   1


/*******************************************************************************
 * LCD backlight
 *******************************************************************************/
#define LCD_BACKLIGHT           LATDbits.LATD0
#define LCD_BACKLIGHT_TRIS      TRISDbits.TRISD0



/*******************************************************************************
 * Touchscreen
 *******************************************************************************/
#define TC_PEN_PIN		PORTCbits.RC3
#define TC_PEN_TRIS		TRISCbits.TRISC3
#define TC_CS			LATGbits.LATG9
#define TC_CS_TRIS		TRISGbits.TRISG9



/*******************************************************************************
 * Power control
 *******************************************************************************/
#define POWER_MAIN          LATBbits.LATB8
#define POWER_MAIN_TRIS     TRISBbits.TRISB8

#define POWER_SUB           LATBbits.LATB9
#define POWER_SUB_TRIS      TRISBbits.TRISB9

#define POWER_BUTTON_TRIS   TRISEbits.TRISE8
#define POWER_BUTTON        PORTEbits.RE8



/*******************************************************************************
 * Audio
 *******************************************************************************/
#define AUDIOAMP_SD         LATEbits.LATE9
#define AUDIOAMP_SD_TRIS    TRISEbits.TRISE9

#define DIGIPOT_CS          LATGbits.LATG12
#define DIGIPOT_CS_TRIS     TRISGbits.TRISG12

#define DIGIPOT_SCK         LATGbits.LATG13
#define DIGIPOT_SCK_TRIS    TRISGbits.TRISG13

#define DIGIPOT_SDO         LATGbits.LATG14
#define DIGIPOT_SDI         PORTGbits.RG14
#define DIGIPOT_DATA_TRIS   TRISGbits.TRISG14



/*******************************************************************************
 * External module (G15 & sensor)
 *******************************************************************************/
#define EXT_UART_BOOSTED_BAUDRATE   115200
#define EXT_UART_ORI_BAUDRATE       19200
#define EXT_UART_RX                 PORTFbits.RF12      // UART Rx pin.

#define EXT_UART_DIR                LATAbits.LATA1      // Control UART direction (0 = Tx, 1 = Rx).
#define EXT_UART_DIR_TRIS           TRISAbits.TRISA1



/*******************************************************************************
 * IMU
 *******************************************************************************/
//#define ENABLE_IMU
#define I2C1_CLOCK_FREQ     100000
#define IMU_INT             PORTDbits.RD8       // MPU6050 Interrupt pin (Active low).



/*******************************************************************************
 * Bluetooth module
 *******************************************************************************/
#define BT4_DUAL_BAUDRATE   115200              // For Bluetooth 4.0 Dual Mode.
#define BT4_BLE_BAUDRATE    9600                // For Bluetooth 4.0 BLE.
#define BT2_BAUDRATE        38400               // For Bluetooth 2.1

#define BT_CONNECTED        PORTAbits.RA0       // Indicate the bluetooth status.
#define BT_CONNECTED_TRIS   TRISAbits.TRISA0

#define BT_RESET            LATAbits.LATA2      // Used to reset the bluetooth module (0 = Reset, 1 = Run).
#define BT_RESET_TRIS       TRISAbits.TRISA2

#define BT_AT_MODE          LATAbits.LATA3      // Select the bluetooth operating mode (0 = Transparent, 1 = AT).
#define BT_AT_MODE_TRIS     TRISAbits.TRISA3



/*******************************************************************************
 * Testpoint
 *******************************************************************************/
//#define PGD                 LATBbits.LATB0
//#define PGD_TRIS            TRISBbits.TRISB0
//
//#define PGC                 LATBbits.LATB1
//#define PGC_TRIS            TRISBbits.TRISB1



/*******************************************************************************
 * USB stack hardware selection options
 *******************************************************************************/
#define self_power          1

#define USE_USB_BUS_SENSE_IO
#if defined(USE_USB_BUS_SENSE_IO)
    #define tris_usb_bus_sense  TRISAbits.TRISA4    // Input
    #define USB_BUS_SENSE       PORTAbits.RA4       //U1OTGSTATbits.SESVD?
#else
    #define USB_BUS_SENSE   1
#endif



/*******************************************************************************
 * MDD File System selection options
 *******************************************************************************/
#define USE_PIC32
#define USE_32BIT

#define USB_USE_MSD
#define USE_SD_INTERFACE_WITH_SPI

#define MILLISECONDS_PER_TICK       10                  // Definition for use with a tick timer
#define TIMER_PRESCALER             TIMER_PRESCALER_8   // Definition for use with a tick timer
#define TIMER_PERIOD                37500               // Definition for use with a tick timer

//#define MDD_USE_SPI_1
//#define MDD_USE_SPI_2
#define MDD_USE_SPI_1A

//SPI Configuration
#define SPI_START_CFG_1     (PRI_PRESCAL_64_1 | SEC_PRESCAL_8_1 | MASTER_ENABLE_ON | SPI_CKE_ON | SPI_SMP_ON)
#define SPI_START_CFG_2     (SPI_ENABLE)

// Define the SPI frequency
#define SPI_FREQUENCY			(20000000)
    
#if defined MDD_USE_SPI_1
       // Description: SD-SPI Chip Select Output bit
   	#define SD_CS               LATBbits.LATB1
   	// Description: SD-SPI Chip Select TRIS bit
    #define SD_CS_TRIS          TRISBbits.TRISB1
       
 	// Description: SD-SPI Card Detect Input bit
 	#define SD_CD               PORTFbits.RF0
    // Description: SD-SPI Card Detect TRIS bit
    #define SD_CD_TRIS          TRISFbits.TRISF0

    // Description: SD-SPI Write Protect Check Input bit
    #define SD_WE               PORTFbits.RF1
    // Description: SD-SPI Write Protect Check TRIS bit
    #define SD_WE_TRIS          TRISFbits.TRISF1
           
    // Description: The main SPI control register
    #define SPICON1             SPI1CON
    // Description: The SPI status register
    #define SPISTAT             SPI1STAT
    // Description: The SPI Buffer
    #define SPIBUF              SPI1BUF
    // Description: The receive buffer full bit in the SPI status register
    #define SPISTAT_RBF         SPI1STATbits.SPIRBF
    // Description: The bitwise define for the SPI control register (i.e. _____bits)
    #define SPICON1bits         SPI1CONbits
    // Description: The bitwise define for the SPI status register (i.e. _____bits)
    #define SPISTATbits         SPI1STATbits
    // Description: The enable bit for the SPI module
    #define SPIENABLE           SPICON1bits.ON
    // Description: The definition for the SPI baud rate generator register (PIC32)
    #define SPIBRG			    SPI1BRG

    // Tris pins for SCK/SDI/SDO lines

    // Description: The TRIS bit for the SCK pin
    #define SPICLOCK            TRISFbits.TRISF6
    // Description: The TRIS bit for the SDI pin
    #define SPIIN               TRISFbits.TRISF7
    // Description: The TRIS bit for the SDO pin
    #define SPIOUT              TRISFbits.TRISF8
    //SPI library functions
    #define putcSPI             putcSPI1
    #define getcSPI             getcSPI1
    #define OpenSPI(config1, config2)   OpenSPI1(config1, config2)
       
#elif defined MDD_USE_SPI_2
    // Description: SD-SPI Chip Select Output bit
	#define SD_CS               LATGbits.LATG9

    // Description: SD-SPI Chip Select TRIS bit
	#define SD_CS_TRIS          TRISGbits.TRISG9
       
	// Description: SD-SPI Card Detect Input bit
	#define SD_CD               0 //PORTEbits.RE5
    // Description: SD-SPI Card Detect TRIS bit
	#define SD_CD_TRIS         		TRISCbits.TRISC14 //TRISEbits.TRISE5

   	// Description: SD-SPI Write Protect Check Input bit
	#define SD_WE               0 //PORTEbits.RE6
   	// Description: SD-SPI Write Protect Check TRIS bit
	#define SD_WE_TRIS          TRISCbits.TRISC13//TRISEbits.TRISE6
       
    // Description: The main SPI control register
    #define SPICON1             SPI2CON
    // Description: The SPI status register
    #define SPISTAT             SPI2STAT
    // Description: The SPI Buffer
    #define SPIBUF              SPI2BUF
    // Description: The receive buffer full bit in the SPI status register
    #define SPISTAT_RBF         SPI2STATbits.SPIRBF
    // Description: The bitwise define for the SPI control register (i.e. _____bits)
    #define SPICON1bits         SPI2CONbits
    // Description: The bitwise define for the SPI status register (i.e. _____bits)
    #define SPISTATbits         SPI2STATbits
    // Description: The enable bit for the SPI module
    #define SPIENABLE           SPI2CONbits.ON
    // Description: The definition for the SPI baud rate generator register (PIC32)
    #define SPIBRG			    SPI2BRG

    // Tris pins for SCK/SDI/SDO lines

    // Description: The TRIS bit for the SCK pin
    #define SPICLOCK            TRISGbits.TRISG6
    // Description: The TRIS bit for the SDI pin
    #define SPIIN               TRISGbits.TRISG7
    // Description: The TRIS bit for the SDO pin
    #define SPIOUT              TRISGbits.TRISG8
    //SPI library functions
    #define putcSPI             putcSPI2
    #define getcSPI             getcSPI2
    #define OpenSPI(config1, config2)   OpenSPI2(config1, config2)
         

#elif defined MDD_USE_SPI_1A
    // Description: SD-SPI Chip Select Output bit
    #define SD_CS               LATDbits.LATD14

    // Description: SD-SPI Chip Select TRIS bit
    #define SD_CS_TRIS          TRISDbits.TRISD14
       
    // Description: SD-SPI Card Detect Input bit
    #define SD_CD               0 //PORTEbits.RE5
   	// Description: SD-SPI Card Detect TRIS bit
    #define SD_CD_TRIS          TRISCbits.TRISC14 //TRISEbits.TRISE5

    // Description: SD-SPI Write Protect Check Input bit
    #define SD_WE               0 //PORTEbits.RE6
    // Description: SD-SPI Write Protect Check TRIS bit
    #define SD_WE_TRIS          TRISCbits.TRISC13//TRISEbits.TRISE6
       
    // Description: The main SPI control register
    #define SPICON1             SPI1ACON
    // Description: The SPI status register
    #define SPISTAT             SPI1ASTAT
    // Description: The SPI Buffer
    #define SPIBUF              SPI1ABUF
    // Description: The receive buffer full bit in the SPI status register
    #define SPISTAT_RBF         SPI1ASTATbits.SPIRBF
    // Description: The bitwise define for the SPI control register (i.e. _____bits)
    #define SPICON1bits         SPI1ACONbits
    // Description: The bitwise define for the SPI status register (i.e. _____bits)
    #define SPISTATbits         SPI1ASTATbits
    // Description: The enable bit for the SPI module
    #define SPIENABLE           SPI1ACONbits.ON
    // Description: The definition for the SPI baud rate generator register (PIC32)
    #define SPIBRG              SPI1ABRG

    // Tris pins for SCK/SDI/SDO lines

    // Description: The TRIS bit for the SCK pin
    #define SPICLOCK            TRISDbits.TRISD15
    // Description: The TRIS bit for the SDI pin
    #define SPIIN               TRISFbits.TRISF2
    // Description: The TRIS bit for the SDO pin
    #define SPIOUT              TRISFbits.TRISF8
    //SPI library functions
    #define putcSPI             putcSPI_1A
    #define getcSPI             getcSPI_1A
    #define OpenSPI(config1, config2)   OpenSPI_1A(config1, config2)

#endif



#endif	/* HARDWARE_PROFILE_H */
