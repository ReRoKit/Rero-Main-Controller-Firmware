#include "HardwareProfile.h"

#if defined (USE_TOUCHSCREEN_RESISTIVE)

#include "Graphics/Graphics.h"
#include "TimeDelay.h"
#include "TouchScreen/TouchScreen.h"
#include "TouchScreen/TouchScreenResistive.h"
#include "Compiler.h"


#define TC_SPI_WIDTH            16 //use 16 clock per 1 SPI cycle
#define TC_CR_Y			0x90 << (TC_SPI_WIDTH - 8) //DFR = 0, PD = 00
#define TC_CR_YB		0x93 << (TC_SPI_WIDTH - 8) //DFR = 0, PD = 11
#define TC_CR_X			0xD0 << (TC_SPI_WIDTH - 8) //DFR = 0, PD = 00
#define TC_CR_XB		0xD3 << (TC_SPI_WIDTH - 8) //DFR = 0, PD = 00
#define TC_CR_Z1		0xB0 <<	(TC_SPI_WIDTH - 8) //DFR = 0, PD = 00
#define TC_CR_Z1B		0xB3 <<	(TC_SPI_WIDTH - 8) //DFR = 0, PD = 00
#define TC_CR_Z2		0xC0 <<	(TC_SPI_WIDTH - 8) //DFR = 0, PD = 00
#define TC_CR_Z2B		0xC3 <<	(TC_SPI_WIDTH - 8) //DFR = 0, PD = 00

#define TOUCHCOUNT		10



// Default Calibration Inset Value (percentage of vertical or horizontal resolution)
// Calibration Inset = ( CALIBRATIONINSET / 2 ) % , Range of 0–20% with 0.5% resolution
// Example with CALIBRATIONINSET == 20, the calibration points are measured
// 10% from the corners.
#define CALIBRATIONINSET   10       // range 0 <= CALIBRATIONINSET <= 40 

#define CAL_X_INSET    (((GetMaxX()+1)*(CALIBRATIONINSET>>1))/100)			//10 percent from corner  MaxX* calibrationinset/2/100
#define CAL_Y_INSET    (((GetMaxY()+1)*(CALIBRATIONINSET>>1))/100)


//////////////////////// Resistive Touch Driver Version ////////////////////////////
// The first three digit is version number and 0xF is assigned to this
// 4-wire resistive driver.
const WORD mchpTouchScreenVersion = 0xF100;

//////////////////////// LOCAL PROTOTYPES ////////////////////////////
void    TouchGetCalPoints(void);
void 	TouchStoreCalibration(void);
void 	TouchCheckForCalibration(void);
void 	TouchLoadCalibration(void);
void 	TouchScreenResistiveTestXY(void);

extern NVM_READ_FUNC           pCalDataRead;                // function pointer to data read
extern NVM_WRITE_FUNC          pCalDataWrite;               // function pointer to data write
extern NVM_SECTORERASE_FUNC    pCalDataSectorErase;         // function pointer to data sector erase


//////////////////////// GLOBAL VARIABLES ////////////////////////////
//#ifndef TOUCHSCREEN_RESISTIVE_PRESS_THRESHOLD
//    // you may define the threshold with a value, define the new value in the 
//    // HardwareProfile.h
//    #define TOUCHSCREEN_RESISTIVE_PRESS_THRESHOLD     256	// between 0-0x03ff the lesser this value 
//											                // the lighter the screen must be pressed
//#endif
#define CALIBRATION_DELAY   300				                // delay between calibration touch points

// Current ADC values for X and Y channels
volatile SHORT      adcX = -1;
volatile SHORT      adcY = -1;

volatile long 		_trA = TOUCHCAL_CA;
volatile long 		_trB = TOUCHCAL_CB;
volatile long 		_trC = TOUCHCAL_CC;
volatile long 		_trD = TOUCHCAL_CD;

// WARNING: Watch out when selecting the value of SCALE_FACTOR 
// since a big value will overflow the signed int type 
// and the multiplication will yield incorrect values.
#ifndef TOUCHSCREEN_RESISTIVE_CALIBRATION_SCALE_FACTOR
    // default scale factor is 128
    #define TOUCHSCREEN_RESISTIVE_CALIBRATION_SCALE_FACTOR 7
#endif

#define SCALE_FACTOR (1<<TOUCHSCREEN_RESISTIVE_CALIBRATION_SCALE_FACTOR)		//scale factor of 128

#ifdef TOUCHSCREEN_RESISTIVE_FLIP_X	
	#define  CalcTouchX(val) 	((_trD-(_trC*val))>>TOUCHSCREEN_RESISTIVE_CALIBRATION_SCALE_FACTOR)
#else
	#define  CalcTouchX(val) 	(((_trC*val)-_trD)>>TOUCHSCREEN_RESISTIVE_CALIBRATION_SCALE_FACTOR)
#endif

#ifdef TOUCHSCREEN_RESISTIVE_FLIP_Y
	#define  CalcTouchY(val) 	((_trB-(_trA*val))>>TOUCHSCREEN_RESISTIVE_CALIBRATION_SCALE_FACTOR)
#else
	#define  CalcTouchY(val) 	(((_trA*val)-_trB)>>TOUCHSCREEN_RESISTIVE_CALIBRATION_SCALE_FACTOR)
#endif

unsigned short spi_buf[5];



/*********************************************************************
* Function: void TouchDetectPosition(void)
********************************************************************/
void TouchDetectPosition(void)
{

    adcX=TouchCommand(TC_CR_X);
    adcY=TouchCommand(TC_CR_Y);
    if((adcX==-1)||(adcY==-1))
    {
        adcX=-1; adcY=-1;
    }
}

//signed short TouchCommand(unsigned short Command){
//    unsigned short buf,pos;
//    unsigned short debounce;
//
//    if(TC_PEN_PIN==0){
//        if(TC_PEN_PIN==0){   //prevent glitches
//            TC_CS = 0;
//            do{
//                spi_buf[0] = Command; //read Y first
//                spi_buf[1] = Command; //then read Y again
//                spi_buf[2] = 0;       //3th word is not used but to clock out data
//
//                SpiChnPutS(SPI_CHANNEL2, (unsigned int*) spi_buf, 3); //plib function will cast to 16bit pointer for short type point
//                SpiChnGetS(SPI_CHANNEL2, (unsigned int*) spi_buf, 3); //plib function will cast to 16bit pointer for short type point
//
//                buf = ((spi_buf[0]& 0x00FF) << 4) | (spi_buf[1] >> 12); //arrange received data
//                pos = ((spi_buf[1]& 0x00FF) << 4) | (spi_buf[2] >> 12); //arrange received data
//
//            }while((pos!=buf)&&(TC_PEN_PIN==0));
//            TC_CS = 1;
//        }
//
//        debounce=500; 			//delay for median filtering
//        while(debounce--);
//        if(TC_PEN_PIN==0)
//        {
//            return ((signed short)pos);
//        }
//    }

//TODO: Modification by WW
    signed short TouchCommand(unsigned short Command){
    unsigned long buf = 0;
    unsigned short debounce;
    unsigned short sampleCount = 0;

    if(TC_PEN_PIN==0){
        if(TC_PEN_PIN==0){   //prevent glitches
            TC_CS = 0;
            do{
                spi_buf[0] = Command; //read Y first
                spi_buf[1] = 0;       //2nd word is not used but to clock out data

                SpiChnPutS(SPI_CHANNEL2, (unsigned int*) spi_buf, 2); //plib function will cast to 16bit pointer for short type point
                SpiChnGetS(SPI_CHANNEL2, (unsigned int*) spi_buf, 2); //plib function will cast to 16bit pointer for short type point

                buf += (unsigned long)(((spi_buf[0]& 0x00FF) << 4) | (spi_buf[1] >> 12)); //arrange received data

            }while((++sampleCount<20)&&(TC_PEN_PIN==0));
            TC_CS = 1;
        }

        buf /= 20;

        //debounce=500; 			//delay for median filtering
        //while(debounce--);
        if(TC_PEN_PIN==0)
        {
            return ((signed short)buf);
        }
    }
//TODO: Modification by WW


    return (-1);
}

/*********************************************************************/
void TouchHardwareInit(void *initValues)
{

/* open spi channel 2 for touch screen driver and init driver*/

    unsigned int srcClk;
    srcClk = GetPeripheralClock();
    SPI2CONbits.ENHBUF=1;
    SpiChnOpen(SPI_CHANNEL2, SPI_OPEN_MSTEN|SPI_OPEN_SMP_END|SPI_OPEN_MODE16,srcClk/100000);
    SPI2CONbits.CKE=1;

    TC_CS_TRIS=0;	//TRISGCLR=0x00000200 ; // clear TRISG9 for output //TC_CS_TRIS=0;
    TC_CS=1;	//LATGSET=0x00000200	; // set LATG9, CS					 //TC_CS=1;
    TC_PEN_TRIS=1;		//	TRISCSET=		//0x00000002	; //TRISC1	//TC_PEN_TRIS=1;


    TC_CS=0;	//LATGCLR=0x00000200	; // clear LATG9, CS //TC_CS=0;	//Chip Select Active
    spi_buf[0] = TC_CR_Y; //read Y first
    spi_buf[1] = TC_CR_Y; //then read Y again
    spi_buf[2] = 0; //3th word is not used
    SpiChnPutS(SPI_CHANNEL2, (unsigned int*)spi_buf, 3);		//plib function will cast to 16bit pointer for short type point
    SpiChnGetS(SPI_CHANNEL2, (unsigned int*)spi_buf, 3);		//plib function will cast to 16bit pointer for short type point
    TC_CS=1;	//LATGSET=0x00000200; //TC_CS=1;

}

/*********************************************************************/
SHORT TouchGetX(void)
{
    long    result;

    result = TouchGetRawX();

    if (result >= 0) {
        result =CalcTouchX(result);
    }
    return ((SHORT)result);
}
/*********************************************************************/
SHORT TouchGetRawX(void)
{
    #ifdef TOUCHSCREEN_RESISTIVE_SWAP_XY
    return adcY;
    #else
    return adcX;
    #endif
}
/********************************************************************/
SHORT TouchGetY(void)
{
    long    result;

    result = TouchGetRawY();
    
    if (result >= 0) {
        result = CalcTouchY(result);
    }
    return ((SHORT)result);
}
/********************************************************************/
SHORT TouchGetRawY(void)
{
    #ifdef TOUCHSCREEN_RESISTIVE_SWAP_XY
    return adcX;
    #else
    return adcY;
    #endif
}

/*********************************************************************
* Function: void TouchStoreCalibration(void)
*
* PreCondition: Non-volatile memory initialization function must be called before
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: stores calibration parameters into non-volatile memory
*
* Note: none
*
********************************************************************/
void TouchStoreCalibration(void)
{
 
 	if (pCalDataWrite != NULL)
	{
		// the coefficient A address is used since it is the first one
		// and this assumes that all stored values are located in one 
		// sector
		if (pCalDataSectorErase != NULL)
			pCalDataSectorErase(ADDRESS_RESISTIVE_TOUCH_COEFA);

    	pCalDataWrite(_trA, ADDRESS_RESISTIVE_TOUCH_COEFA);
    	pCalDataWrite(_trB, ADDRESS_RESISTIVE_TOUCH_COEFB);
    	pCalDataWrite(_trC, ADDRESS_RESISTIVE_TOUCH_COEFC);
    	pCalDataWrite(_trD, ADDRESS_RESISTIVE_TOUCH_COEFD);

    	pCalDataWrite(mchpTouchScreenVersion, ADDRESS_RESISTIVE_TOUCH_VERSION);
    	
 	}
 
}

/*********************************************************************
* Function: void TouchLoadCalibration(void)
*
* PreCondition: Non-volatile memory initialization function must be called before
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: loads calibration parameters from non-volatile memory
*
* Note: none
*
********************************************************************/
void TouchLoadCalibration(void)
{
    if (pCalDataRead != NULL) {
        _trA = pCalDataRead(ADDRESS_RESISTIVE_TOUCH_COEFA);
        _trB = pCalDataRead(ADDRESS_RESISTIVE_TOUCH_COEFB);
        _trC = pCalDataRead(ADDRESS_RESISTIVE_TOUCH_COEFC);
        _trD = pCalDataRead(ADDRESS_RESISTIVE_TOUCH_COEFD);
    }
}

/*********************************************************************
* Function: void TouchGetCalPoints(void)
*
* PreCondition: InitGraph() must be called before
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: gets values for 3 touches
*
* Note: none
*
********************************************************************/
void TouchCalculateCalPoints(WORD *xTouch, WORD *yTouch, WORD *xPoint, WORD *yPoint)
{
    long    trA,trB,trC,trD;						// variables for the coefficients
    long    test1, test2; 							// temp variables (must be signed type)
    long    temp1, temp2;
	
    //Y direction calculation
        test1 = (long)(yPoint[3] - yPoint[0]);  	//same value as (long)yPoint[2] - (long)yPoint[1];
        temp1=  (long)((yTouch[0]+yTouch[1])>>1);  //average the value of two points
        temp2=  (long)((yTouch[3]+yTouch[2])>>1);
        test2 = temp2-temp1;

	trA = (test1 * SCALE_FACTOR);				//signed operation shifting for divide invalid
	if((trA%test2)>=(test2>>1))
            trA+=test2;
	trA/=test2;										//Scaled Pixel per touch Adc value,
																				
	trB = (trA *temp1)-((long)yPoint[0]*SCALE_FACTOR);		// constant, offset of the TC

   //X direction calculation
        test1 = (long)(xPoint[1] - xPoint[0]);		//same as(long)xPoint[2] - (long)xPoint[3];
        temp1=(long)((xTouch[0]+xTouch[3])>>1); //average two points
        temp2=(long)((xTouch[1]+xTouch[2])>>1);
        test2 = temp1-temp2;
	
	trC = (test1 * SCALE_FACTOR);
	if((trC%test2)>=(test2>>1))					//roundup algorithm
            trC+=test2;
	trC/=test2;		

	trD = (trC*temp1)+((long)xPoint[0]*SCALE_FACTOR);


        //update calibration value
        _trA = trA;
	_trB = trB;
	_trC = trC;
	_trD = trD;   
	
}

/*********************************************************************
* Function: void TouchGetCalPoints(void)
*
* PreCondition: InitGraph() must be called before
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: gets values for 3 touches
*
* Note: none
*
********************************************************************/
void TouchCalHWGetPoints(void)
{
	#define TOUCH_DIAMETER	10
	#define SAMPLE_POINTS   4

    XCHAR   calStr1[] = {'o','n',' ','t','h','e',' ','f','i','l','l','e','d',0};
    XCHAR   calStr2[] = {'c','i','r','c','l','e',0};
    XCHAR  	calTouchPress[] = {'P','r','e','s','s',0};
    //XCHAR   calTouchHold[] = {'H','o','l','d',0};
    XCHAR   calTouchRelease[] = {'R','e','l','e','a','s','e',0};
    SHORT   counter;

    WORD    tx[SAMPLE_POINTS], ty[SAMPLE_POINTS];
    WORD    dx[SAMPLE_POINTS], dy[SAMPLE_POINTS];
    WORD    textHeight, msgX, msgY;

	SetFont((void *) &FONTDEFAULT);
    SetColor(BRIGHTRED);

    textHeight = GetTextHeight((void *) &FONTDEFAULT);

    while
    (
        !OutTextXY
            (
                (GetMaxX() - GetTextWidth((XCHAR *)calStr1, (void *) &FONTDEFAULT)) >> 1,
                (GetMaxY() >> 1),
                (XCHAR *)calStr1
            )
    );

    while
    (
        !OutTextXY
            (
                (GetMaxX() - GetTextWidth((XCHAR *)calStr2, (void *) &FONTDEFAULT)) >> 1,
                ((GetMaxY() >> 1) + textHeight),
                (XCHAR *)calStr2
            )
    );

    // calculate center points (locate them at 15% away from the corners)
    // draw the four touch points

    dy[0] = dy[1] = CAL_Y_INSET; 
    dy[2] = dy[3] = (GetMaxY() - CAL_Y_INSET);
    dx[0] = dx[3] = CAL_X_INSET;
    dx[1] = dx[2] = (GetMaxX() - CAL_X_INSET);


    msgY = ((GetMaxY() >> 1) - textHeight);
	
    for(counter = 0; counter < SAMPLE_POINTS; counter++)
    {
    
		// redraw the filled circle to unfilled (previous calibration point)
        if (counter > 0) {
            SetColor(WHITE);
            while (!(FillCircle(dx[counter - 1], dy[counter - 1], TOUCH_DIAMETER - 7)));
        }

	// draw the new filled circle (new calibration point)
        SetColor(BRIGHTRED);
        while(!(Circle(dx[counter], dy[counter], TOUCH_DIAMETER)));
        while(!(FillCircle(dx[counter], dy[counter], TOUCH_DIAMETER-7)));

	// show points left message
        msgX = (GetMaxX() - GetTextWidth((XCHAR *)calTouchPress, (void *) &FONTDEFAULT)) >> 1;
		TouchShowMessage(calTouchPress, BRIGHTRED, msgX, msgY);

        // Wait for press
        do {TouchDetectPosition();} 
    	    while((TouchGetRawX() == -1) && (TouchGetRawY() == -1));

        // erase points left message
        TouchShowMessage(calTouchPress, WHITE, msgX, msgY);
        
        // show RELEASE message
        msgX = (GetMaxX() - GetTextWidth((XCHAR *)calTouchRelease, (void *) &FONTDEFAULT)) >> 1;
            TouchShowMessage(calTouchRelease, BRIGHTRED, msgX, msgY);

        // Wait for release
        //do {TouchDetectPosition();} 
        while((TouchGetRawX() != -1) && (TouchGetRawY() != -1))
		{
			
			ty[counter] = TouchGetRawY();
			tx[counter] = TouchGetRawX();
			TouchDetectPosition();
		}
		


        DelayMs(CALIBRATION_DELAY);
		// erase RELEASE message
		TouchShowMessage(calTouchRelease, WHITE, msgX, msgY);
       
    }

    // Wait for release
    do {
        TouchDetectPosition();
    }  while ((TouchGetRawX() != -1) && (TouchGetRawY() != -1));

	
    TouchCalculateCalPoints(tx, ty, dx, dy);
    
    #ifdef ENABLE_DEBUG_TOUCHSCREEN

//		#warning " debug touchscreen";

	if(PORTGbits.RG15==0)
        TouchScreenResistiveTestXY();
    #endif
}


/*********************************************************************
* Function: void TouchScreenResistiveTestXY(void)
*
* PreCondition: TouchHardwareInit has been called
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: prints raw x,y calibrated x,y and calibration factors to screen
*
* Note: modify pre-processor macro to include/exclude this test code
*       a common place to call this from is at the end of TouchCalHWGetPoints()
*
********************************************************************/
#ifdef ENABLE_DEBUG_TOUCHSCREEN
#include <stdio.h>
void TouchScreenResistiveTestXY(void)
{
    #define BUFFCHARLEN 60
    char buffChar[BUFFCHARLEN];
    WORD buffCharW[BUFFCHARLEN];
    unsigned char i;
    SHORT tempXX, tempYY,tempXX2,tempYY2, calXX, calYY;
    tempXX = tempYY = tempXX2 = tempYY2 = -1;
    while(1)
    {
        do{TouchDetectPosition(); tempXX = TouchGetRawX(); tempYY = TouchGetRawY();}
    	while((tempXX == -1) && (tempYY == -1));   
        
        // use this to always show the values even if not pressing the screen
        //tempXX = TouchGetRawX(); 
        //tempYY = TouchGetRawY();
        
        calXX = TouchGetX();
        calYY = TouchGetY();
        
        if((tempXX != tempXX2)||(tempYY != tempYY2))
        {
            SetColor(WHITE);
            ClearDevice();
            SetColor(BRIGHTRED);
            sprintf(buffChar,"raw_x=%d, raw_y=%d",(WORD)tempXX, (WORD)tempYY);
			
            #ifdef USE_MULTIBYTECHAR
              for(i = 0; i < BUFFCHARLEN;i++)
              {
                buffCharW[i] = buffChar[i];
              }
              while(!OutTextXY(0,0,(XCHAR*)buffCharW));
            #else
              while(!OutTextXY(0,0,(XCHAR*)buffChar));
            #endif
            
            sprintf(buffChar,"cal_x=%d, cal_y=%d",(WORD)calXX, (WORD)calYY);
            #ifdef USE_MULTIBYTECHAR
              for(i = 0; i < BUFFCHARLEN;i++)
              {
                buffCharW[i] = buffChar[i];
              }
              while(!OutTextXY(0,40,(XCHAR*)buffCharW));
            #else
              while(!OutTextXY(0,40,(XCHAR*)buffChar));
            #endif
            
            sprintf(buffChar,"_tr:A=%d,B=%d,C=%d,D=%d",(WORD)_trA,(WORD)_trB,(WORD)_trC,(WORD)_trD);
            #ifdef USE_MULTIBYTECHAR
              for(i = 0; i < BUFFCHARLEN;i++)
              {
                buffCharW[i] = buffChar[i];
              }
              while(!OutTextXY(0,80,(XCHAR*)buffCharW));
            #else
              while(!OutTextXY(0,80,(XCHAR*)buffChar));
            #endif

        }
        
        tempXX2=tempXX;
        tempYY2=tempYY;
    }
}
#endif //#ifdef ENABLE_DEBUG_TOUCHSCREEN


#endif // #if defined (USE_TOUCHSCREEN_RESISTIVE)

