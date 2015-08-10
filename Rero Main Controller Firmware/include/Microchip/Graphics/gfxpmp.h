
#ifndef _GFX_PMP_H_FILE
#define _GFX_PMP_H_FILE

#include "HardwareProfile.h"
#include "Compiler.h"
#include "TimeDelay.h"

#ifdef USE_GFX_PMP

// Note:
/*
    All functions here are defined as inline functions for performance.
    When debugging this portion it is best to look at the assembly output 
    and debug from there.
*/

    // error checking
    #ifndef PMP_DATA_SETUP_TIME
        #error "Define PMP_DATA_SETUP_TIME in HardwareProfile.h for data set up before read/write strobe timing"
    #endif    
    #ifndef PMP_DATA_WAIT_TIME
        #error "Define PMP_DATA_WAIT_TIME in HardwareProfile.h for read/write strobe wait states"
    #endif    
    #ifndef PMP_DATA_HOLD_TIME
        #error "Define PMP_DATA_HOLD_TIME in HardwareProfile.h for data hold after read/write strobe"
    #endif    
    
#ifdef __PIC32MX__
#define PMDIN1              PMDIN
#endif


#define PMPWaitBusy()   while(PMMODEbits.BUSY); 

extern inline void __attribute__ ((always_inline)) DeviceWrite(WORD data) 
{ 
	PMDIN1 = data;
	PMPWaitBusy();
}

extern inline WORD __attribute__ ((always_inline)) DeviceRead()
{
WORD value;
	value = PMDIN1;
	PMPWaitBusy();
	PMCONbits.PMPEN = 0; // disable PMP
	value = PMDIN1;
	PMPWaitBusy();
	PMCONbits.PMPEN = 1; // enable  PMP
	return value;
}

extern inline WORD __attribute__ ((always_inline)) SingleDeviceRead()
{
WORD value;
	value = PMDIN1;
	PMPWaitBusy();
	return value;
}

extern inline WORD __attribute__ ((always_inline)) DeviceReadWord()
{
WORD value;
	value = PMDIN1;
	PMPWaitBusy();
	return value;
}

extern inline void __attribute__ ((always_inline)) DriverInterfaceInit(void)
{ 
    // variable for PMP timing calculation
	// GetPeripheralClock() is in Mhz. pClockPeriod will be in nanoseconds.
    DWORD pClockPeriod = (1000000000ul) / GetPeripheralClock();

	DisplayResetEnable();               // hold in reset by default
    DisplayResetConfig();               // enable RESET line
    DisplayCmdDataConfig();             // enable RS line
    DisplayDisable();                   // not selected by default
    DisplayConfig();                    // enable chip select line
    DisplayBacklightOff();              // initially set the backlight to off
    DisplayBacklightConfig();           // set the backlight control pin

    // PMP setup
    PMMODE = 0;
    PMAEN = 0;
    PMCON = 0;
    PMMODEbits.MODE = 2;                // Intel 80 master interface

    #if (PMP_DATA_SETUP_TIME == 0)
        PMMODEbits.WAITB = 0;
    #else    
        if (PMP_DATA_SETUP_TIME <= pClockPeriod)
            PMMODEbits.WAITB = 0;
        else if (PMP_DATA_SETUP_TIME > pClockPeriod)
            PMMODEbits.WAITB = (PMP_DATA_SETUP_TIME / pClockPeriod) + 1;
    #endif
    
    #if (PMP_DATA_WAIT_TIME == 0)
        PMMODEbits.WAITM = 0;
    #else    
        if (PMP_DATA_WAIT_TIME <= pClockPeriod)
            PMMODEbits.WAITM = 1;
        else if (PMP_DATA_WAIT_TIME > pClockPeriod)
            PMMODEbits.WAITM = (PMP_DATA_WAIT_TIME / pClockPeriod) + 1;
    #endif
    
    #if (PMP_DATA_HOLD_TIME == 0)
        PMMODEbits.WAITE = 0;
    #else
        if (PMP_DATA_HOLD_TIME <= pClockPeriod)
            PMMODEbits.WAITE = 0;
        else if (PMP_DATA_HOLD_TIME > pClockPeriod)
            PMMODEbits.WAITE = (PMP_DATA_HOLD_TIME / pClockPeriod) + 1;
    #endif

    #ifdef USE_16BIT_PMP
    PMMODEbits.MODE16 = 1;              // 16 bit mode
    #else
    PMMODEbits.MODE16 = 0;              // 8 bit mode
    #endif
    
    PMCONbits.PTRDEN = 1;               // enable RD line
    PMCONbits.PTWREN = 1;               // enable WR line
    PMCONbits.PMPEN = 1;                // enable PMP

	PMCONbits.RDSP=0;						//active low polarity
	PMCONbits.WRSP=0;						//active low polarity
	PMAEN=	0x00000000;

    DisplayResetDisable();              // release from reset
	
	// hard delay inserted here for devices that needs delays after reset.
	// Value will vary from device to device, please refer to the specific 
	// device data sheet for details.
    Delay10us(20);
}


#endif //#ifdef USE_GFX_PMP
#endif //#ifndef _GFX_PMP_H_FILE
