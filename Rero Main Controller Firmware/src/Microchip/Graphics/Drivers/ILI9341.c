/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  ILITEK ILI9341 controller driver
 *****************************************************************************
 * FileName:        ILI9341.c
 * Dependencies:    Graphics.h
 * Processor:       PIC32
 * Compiler:        MPLAB C32
 * Linker:          MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2009 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 05/26/09     ...
 * 04/12/11     Graphics Library Version 3.00 Support
 *****************************************************************************/
#include "HardwareProfile.h"

#if defined (GFX_USE_DISPLAY_CONTROLLER_ILI9341)

#include "Compiler.h"
#include "TimeDelay.h"
#include "Graphics/DisplayDriver.h"
#include "Graphics/ILI9341.h"
#include "Graphics/Primitive.h"
#include "Graphics/gfxpmp.h"
   
// Color
GFX_COLOR   _color;

// Clipping region control
SHORT   _clipRgn;

// Clipping region borders
SHORT   _clipLeft;
SHORT   _clipTop;
SHORT   _clipRight;
SHORT   _clipBottom;

/////////////////////// LOCAL FUNCTIONS PROTOTYPES ////////////////////////////
void PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch);
void PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch);
void PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch);
void PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch);

void PutImage1BPPExt(SHORT left, SHORT top, void *image, BYTE stretch);
void PutImage4BPPExt(SHORT left, SHORT top, void *image, BYTE stretch);
void PutImage8BPPExt(SHORT left, SHORT top, void *image, BYTE stretch);
void PutImage16BPPExt(SHORT left, SHORT top, void *image, BYTE stretch);

void LCD_Write_COM(char data);
void LCD_Write_DATA(unsigned short data);
void WritePixel(unsigned short data);
void SetAddress(SHORT x, SHORT y);

// Write command to LCD
void LCD_Write_COM(char data)
{
    DisplaySetCommand();
    DeviceWrite(data);
}
// write data to LCD function
void LCD_Write_DATA(unsigned short data)
{
    DisplaySetData();
    DeviceWrite(data);
}

void LCD_EnterSleep(void)
{
    LCD_Write_COM(0x28);
    LCD_Write_COM(0x10);
}
void LCD_ExitSleep(void)
{
    LCD_Write_COM(0x11);
    DelayMs(120);
    LCD_Write_COM(0x29);
}

// function write pixel to LCD
void WritePixel(unsigned short data)
{
    LCD_Write_COM(0x2C);
    LCD_Write_DATA(data);
}

// function to set LCD RAM address
void SetAddress(SHORT x, SHORT y)
{
    //LCD pixels from y= 0-319 and x=0-239

    LCD_Write_COM(0x2A);
    LCD_Write_DATA(x>>8);         //Start column
    LCD_Write_DATA(x&0xFF);
    //LCD_Write_DATA(239);        //End column

    LCD_Write_COM(0x2B);
    LCD_Write_DATA(y>>8);
    LCD_Write_DATA(y&0xFF);       //Start row
    //LCD_Write_DATA(319);        //End row
}


//function for Initialize or reset LCD
void ResetDevice()
{
    // Initialize the device
    DriverInterfaceInit();
    DelayMs(1);

    DisplayResetEnable();
    DelayMs(2);
    DisplayResetDisable();
    DelayMs(2);

    DisplayEnable();
    
    //ILI9341
      //ILI9341
    LCD_Write_COM(0xCF);
    LCD_Write_DATA (0x00);
    LCD_Write_DATA (0xAA);
    LCD_Write_DATA(0XE0);

    LCD_Write_COM(0xED);
    LCD_Write_DATA (0x67);
    LCD_Write_DATA (0x03);
    LCD_Write_DATA (0X12);
    LCD_Write_DATA (0X81);

    LCD_Write_COM(0xE8);
    LCD_Write_DATA (0x85);
    LCD_Write_DATA (0x11);
    LCD_Write_DATA (0x78);

    LCD_Write_COM(0xCB);
    LCD_Write_DATA (0X39);
    LCD_Write_DATA (0x2C);
    LCD_Write_DATA (0X00);
    LCD_Write_DATA (0x34);
    LCD_Write_DATA (0X02);

    LCD_Write_COM(0xF7);
    LCD_Write_DATA (0x20);

    LCD_Write_COM(0xEA);
    LCD_Write_DATA (0x00);
    LCD_Write_DATA (0x00);

    LCD_Write_COM(0xC0); //Power control
    LCD_Write_DATA(0x21); //qsh (0x10); //VRH[5:0]

    LCD_Write_COM(0xC1); //Power control
    LCD_Write_DATA (0x11); //SAP[2:0];BT[3:0]

    LCD_Write_COM(0xC5); //VCM control
    LCD_Write_DATA (0x24); //0x1C
    LCD_Write_DATA (0x3C);

    LCD_Write_COM(0xC7); //VCM control2
    LCD_Write_DATA(0Xb7);

    LCD_Write_COM(0xB1);
    LCD_Write_DATA (0x00);
    LCD_Write_DATA (0x1B);

    LCD_Write_COM(0xB6); // Display Function Control
    LCD_Write_DATA (0x0A);
    LCD_Write_DATA (0xa2);// (0x82);

    LCD_Write_COM(0x3A); //
    LCD_Write_DATA (0x55);  //0x55 65K 0x66 262K

    LCD_Write_COM(0xF2); // 3Gamma Function Disable
    LCD_Write_DATA (0x00);

    LCD_Write_COM(0x26); //Gamma curve selected
    LCD_Write_DATA (0x01);

    LCD_Write_COM(0xB7); //VCM control2
    LCD_Write_DATA(0X06);

    LCD_Write_COM(0xE0); //Set Gamma
    LCD_Write_DATA (0x0F);
    LCD_Write_DATA (0x1B);
    LCD_Write_DATA (0x19);
    LCD_Write_DATA (0x0c);
    LCD_Write_DATA (0x0D);
    LCD_Write_DATA (0x07);
    LCD_Write_DATA (0x44);
    LCD_Write_DATA (0XA9);
    LCD_Write_DATA (0x2F);
    LCD_Write_DATA (0x08);
    LCD_Write_DATA (0x0D);
    LCD_Write_DATA (0x03);
    LCD_Write_DATA (0x10);
    LCD_Write_DATA (0x0B);
    LCD_Write_DATA (0x00);

    LCD_Write_COM(0XE1); //Set Gamma
    LCD_Write_DATA (0x00);
    LCD_Write_DATA (0x24);
    LCD_Write_DATA (0x26);
    LCD_Write_DATA (0x03);
    LCD_Write_DATA (0x12);
    LCD_Write_DATA (0x08);
    LCD_Write_DATA (0x3C);
    LCD_Write_DATA (0x56);
    LCD_Write_DATA (0x50);
    LCD_Write_DATA (0x07);
    LCD_Write_DATA (0x12);
    LCD_Write_DATA (0x0C);
    LCD_Write_DATA (0x2F);
    LCD_Write_DATA (0x34);
    LCD_Write_DATA (0x0F);

    LCD_Write_COM(0x3A);         //setting pixset format 16bit/pixel for 65K colors
    LCD_Write_DATA (0x55);	//16bits per pixel

    LCD_Write_COM(0x36);     //Memory Access Control

//TODO: ORIENTATION CTRL =====
    LCD_Write_DATA(0x68);    //MY MX MV		RGB=1--> BRG  MV=0 potrait mode

// Orientation control bits.
//    D7 MY (ROW address order)
//    D6 MX (COLUMN address order)
//    D5 MV (ROW / COLUMN Exchange)
//    D4 ML (Vertical refresh order)
//    D3 BGR/RGB
//    D2 MH (horizontal refresh order)
//    D1 0
//    D0 0

//TODO: ORIENTATION CTRL =====
//0110 1000 = 0x68  //new LCD, IH's prototype
//0110 1000 = 0xA8  //new LCD, SC's prototype
//1010 1000 = 0x28  //old LCD, SC's prototype
//============================
//  Change touchscreen orientation
//  - in file HWP_GFXv3_EX16_16PMP_QVGAv1.h
//  - find #warning " flip and swap setting";
//  - modify the define
//============================
//============================


//    LCD_Write_COM(0x36); // Memory Access Control
//     //if(DISP_ORIENTATION==0)
//            LCD_Write_DATA (0x00,0xC8);  //0xC8 or 0x48 or 0x08 for different orientation.
//     //else
//     //       LCD_Write_DATA (0x28);

    LCD_Write_COM(0x2A);		//page addres set
    LCD_Write_DATA(0x00);
    LCD_Write_DATA(0x00);
    LCD_Write_DATA(0x01);
    LCD_Write_DATA(0x3F);		//320pixels

     LCD_Write_COM(0x2B);
    LCD_Write_DATA(0x00);
    LCD_Write_DATA(0x00);
    LCD_Write_DATA(0x00);
    LCD_Write_DATA(0xEF);		//240pixels


    LCD_Write_COM(0x11); //Exit Sleep
    DelayMs(240);

    LCD_Write_COM(0x29); //Display on
    LCD_Write_COM(0x2c);


    DisplayDisable();

    DelayMs(30);

}

//function to put pixel
void PutPixel(SHORT x, SHORT y)
{
    if(_clipRgn)
    {
        if(x < _clipLeft)
            return;
        if(x > _clipRight)
            return;
        if(y < _clipTop)
            return;
        if(y > _clipBottom)
            return;
    }

    DisplayEnable();
    SetAddress(x, y);
    WritePixel(_color);
    DisplayDisable();
}



/*********************************************************************
 * Function: WORD GetPixel(SHORT x, SHORT y)
 *
 * PreCondition: none
 *
 * Input: x,y - pixel coordinates
 *
 * Output: pixel color
 *
 * Side Effects: none
 *
 * Overview: returns pixel color at x,y position
 *
 * Note: none
 *
 ********************************************************************/
WORD GetPixel(SHORT x, SHORT y)
{
    DisplayEnable();
    
    SetAddress(x, y);
    LCD_Write_COM(0x2E);
    DisplaySetData();
    
    // Read 2 words of dummy data.
    SingleDeviceRead();
    SingleDeviceRead();
    
    WORD data1 = SingleDeviceRead();
    WORD data2 = SingleDeviceRead();
    
    DisplayDisable();
    
    unsigned char red = (unsigned char)(data1 >> 8);
    unsigned char green = (unsigned char)(data1 & 0xff);
    unsigned char blue = (unsigned char)(data2 >> 8);
    return (RGBConvert(red, green, blue));
}



/*********************************************************************
* Function: void vPutPixelRow(SHORT x, SHORT y, WORD *pixel, WORD length)
*
* Overview: Puts pixel in a row with the given x,y coordinate position.
*
* PreCondition: none
*
* Input: x - x start position of the pixel.
*        y - y start position of the pixel.
*        *pixel - pointer to the color data.
*        length - the length of the row.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void vPutPixelRow(SHORT x, SHORT y, WORD *pixel, WORD length)
{
    // Make sure it's within the screen size.
    if ((x <= GetMaxX()) && (y <= GetMaxY())) {

        SetAddress(x, y);

        // Send command to start writing pixel data.
        LCD_Write_COM(0x2C);

        while (length-- > 0) {
            // Write pixel to screen
            LCD_Write_DATA(*pixel++);
        }
    }
}



void SetClipRgn(SHORT left, SHORT top, SHORT right, SHORT bottom)
{
    _clipLeft=left;
    _clipTop=top;
    _clipRight=right;
    _clipBottom=bottom;

}

void SetClip(BYTE control)
{
    _clipRgn=control;
}

WORD IsDeviceBusy(void)
{  
    return (0);
}

WORD Bar(SHORT left, SHORT top, SHORT right, SHORT bottom)
{
    register SHORT  x, y;

    #ifndef USE_NONBLOCKING_CONFIG
    while(IsDeviceBusy() != 0);

    /* Ready */
    #else
    if(IsDeviceBusy() != 0)
        return (0);
    #endif
    if(_clipRgn)
    {
        if(left < _clipLeft)
            left = _clipLeft;
        if(right > _clipRight)
            right = _clipRight;
        if(top < _clipTop)
            top = _clipTop;
        if(bottom > _clipBottom)
            bottom = _clipBottom;
    }

    DisplayEnable();
    for(y = top; y < bottom + 1; y++)
    {
        SetAddress(left, y);
        LCD_Write_COM(0x2C);
        for(x = left; x < right + 1; x++)
        {
          LCD_Write_DATA(_color);
        }
    }

    DisplayDisable();
    return (1);
}

void ClearDevice(void)
{
    DWORD   counter;

    DisplayEnable();
    SetAddress(0, 0);
    LCD_Write_COM(0x2C);
    for(counter = 0; counter < (DWORD) (GetMaxX() + 1) * (GetMaxY() + 1); counter++)
    {
        LCD_Write_DATA(_color);
    }

    DisplayDisable();
}

#ifndef USE_TRANSPARENT_COLOR

    #ifdef USE_BITMAP_FLASH

/*********************************************************************
* Function: void PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        image - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch)
{
    register FLASH_BYTE *flashAddress;
    register FLASH_BYTE *tempFlashAddress;
    BYTE                temp = 0;
    WORD                sizeX, sizeY;
    WORD                x, y;
    BYTE                stretchX, stretchY;
    WORD                pallete[2];
    BYTE                mask;

    // Move pointer to size information
    flashAddress = image + 2;

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    pallete[0] = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    pallete[1] = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;

    DisplayEnable();
    for(y = 0; y < sizeY; y++)
    {
        tempFlashAddress = flashAddress;
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            flashAddress = tempFlashAddress;
            SetAddress((WORD) left, (WORD) top);
            LCD_Write_COM(0x2C);
            mask = 0;
            for(x = 0; x < sizeX; x++)
            {

                // Read 8 pixels from flash
                if(mask == 0)
                {
                    temp = *flashAddress;
                    flashAddress++;
                    mask = 0x80;
                }

                // Set color
                if(mask & temp)
                {
                    SetColor(pallete[1]);
                }
                else
                {
                    SetColor(pallete[0]);
                }

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    LCD_Write_DATA(_color);
                }

                // Shift to the next pixel
                mask >>= 1;
            }

            top++;
        }
    }

    DisplayDisable();
}

/*********************************************************************
* Function: void PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs 16 color image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch)
{
    register FLASH_BYTE *flashAddress;
    register FLASH_BYTE *tempFlashAddress;
    WORD                sizeX, sizeY;
    register WORD       x, y;
    BYTE                temp = 0;
    register BYTE       stretchX, stretchY;
    WORD                pallete[16];
    WORD                counter;

    // Move pointer to size information
    flashAddress = image + 2;

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;

    // Read pallete
    for(counter = 0; counter < 16; counter++)
    {
        pallete[counter] = *((FLASH_WORD *)flashAddress);
        flashAddress += 2;
    }

    DisplayEnable();
    for(y = 0; y < sizeY; y++)
    {
        tempFlashAddress = flashAddress;
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            flashAddress = tempFlashAddress;
            SetAddress(left, top);
            LCD_Write_COM(0x2C);
            for(x = 0; x < sizeX; x++)
            {

                // Read 2 pixels from flash
                if(x & 0x0001)
                {

                    // second pixel in byte
                    SetColor(pallete[temp >> 4]);
                }
                else
                {
                    temp = *flashAddress;
                    flashAddress++;

                    // first pixel in byte
                    SetColor(pallete[temp & 0x0f]);
                }

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    LCD_Write_DATA(_color);
                }
            }

            top++;
        }
    }

    DisplayDisable();
}

/*********************************************************************
* Function: void PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs 256 color image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch)
{
    register FLASH_BYTE *flashAddress;
    register FLASH_BYTE *tempFlashAddress;
    WORD                sizeX, sizeY;
    WORD                x, y;
    BYTE                temp;
    BYTE                stretchX, stretchY;
    WORD                pallete[256];
    WORD                counter;

    // Move pointer to size information
    flashAddress = image + 2;

    // Read image size
    sizeY = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD *)flashAddress);
    flashAddress += 2;

    // Read pallete
    for(counter = 0; counter < 256; counter++)
    {
        pallete[counter] = *((FLASH_WORD *)flashAddress);
        flashAddress += 2;
    }

    DisplayEnable();
    for(y = 0; y < sizeY; y++)
    {
        tempFlashAddress = flashAddress;
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            flashAddress = tempFlashAddress;
            SetAddress(left, top);
            LCD_Write_COM(0x2C);
            for(x = 0; x < sizeX; x++)
            {

                // Read pixels from flash
                temp = *flashAddress;
                flashAddress++;

                // Set color
                SetColor(pallete[temp]);

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    LCD_Write_DATA(_color);
                }
            }

            top++;
        }
    }

    DisplayDisable();
}

/*********************************************************************
* Function: void PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs hicolor image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE *image, BYTE stretch)
{
    register FLASH_WORD *flashAddress;
    register FLASH_WORD *tempFlashAddress;
    WORD                sizeX, sizeY;
    register WORD       x, y;
    WORD                temp;
    register BYTE       stretchX, stretchY;

    // Move pointer to size information
    flashAddress = (FLASH_WORD *)image + 1;

    // Read image size
    sizeY = *flashAddress;
    flashAddress++;
    sizeX = *flashAddress;
    flashAddress++;

    DisplayEnable();
    for(y = 0; y < sizeY; y++)
    {
        tempFlashAddress = flashAddress;
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            flashAddress = tempFlashAddress;
            SetAddress(left, top);
            LCD_Write_COM(0x2C);
            for(x = 0; x < sizeX; x++)
            {

                // Read pixels from flash
                temp = *flashAddress;
                flashAddress++;

                // Set color
                SetColor(temp);

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    LCD_Write_DATA(_color);
                }
            }

            top++;
        }
    }

    DisplayDisable();
}

    #endif // USE_BITMAP_FLASH
    #ifdef USE_BITMAP_EXTERNAL

/*********************************************************************
* Function: void PutImage1BPPExt(SHORT left, SHORT top, void* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
void PutImage1BPPExt(SHORT left, SHORT top, void *image, BYTE stretch)
{
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    WORD            pallete[2];
    BYTE            lineBuffer[((GetMaxX() + 1) / 8) + 1];
    BYTE            *pData;
    SHORT           byteWidth;

    BYTE            temp;
    BYTE            mask;
    WORD            sizeX, sizeY;
    WORD            x, y;
    BYTE            stretchX, stretchY;

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Get pallete (2 entries)
    ExternalMemoryCallback(image, sizeof(BITMAP_HEADER), 2 * sizeof(WORD), pallete);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 2 * sizeof(WORD);

    // Line width in bytes
    byteWidth = bmp.width >> 3;
    if(bmp.width & 0x0007)
        byteWidth++;

    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;

    for(y = 0; y < sizeY; y++)
    {

        // Get line
        ExternalMemoryCallback(image, memOffset, byteWidth, lineBuffer);
        memOffset += byteWidth;
        DisplayEnable();
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            pData = lineBuffer;
            SetAddress(left, top);
            LCD_Write_COM(0x2C);
            mask = 0;
            for(x = 0; x < sizeX; x++)
            {

                // Read 8 pixels from flash
                if(mask == 0)
                {
                    temp = *pData++;
                    mask = 0x80;
                }

                // Set color
                if(mask & temp)
                {
                    SetColor(pallete[1]);
                }
                else
                {
                    SetColor(pallete[0]);
                }

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    LCD_Write_DATA(_color);
                }

                // Shift to the next pixel
                mask >>= 1;
            }

            top++;
        }

        DisplayDisable();
    }
}

/*********************************************************************
* Function: void PutImage4BPPExt(SHORT left, SHORT top, void* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
void PutImage4BPPExt(SHORT left, SHORT top, void *image, BYTE stretch)
{
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    WORD            pallete[16];
    BYTE            lineBuffer[((GetMaxX() + 1) / 2) + 1];
    BYTE            *pData;
    SHORT           byteWidth;

    BYTE            temp;
    WORD            sizeX, sizeY;
    WORD            x, y;
    BYTE            stretchX, stretchY;

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Get pallete (16 entries)
    ExternalMemoryCallback(image, sizeof(BITMAP_HEADER), 16 * sizeof(WORD), pallete);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 16 * sizeof(WORD);

    // Line width in bytes
    byteWidth = bmp.width >> 1;
    if(bmp.width & 0x0001)
        byteWidth++;

    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;

    for(y = 0; y < sizeY; y++)
    {

        // Get line
        ExternalMemoryCallback(image, memOffset, byteWidth, lineBuffer);
        memOffset += byteWidth;
        DisplayEnable();
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            pData = lineBuffer;
            SetAddress(left, top);
            LCD_Write_COM(0x2C);
            for(x = 0; x < sizeX; x++)
            {

                // Read 2 pixels from flash
                if(x & 0x0001)
                {

                    // second pixel in byte
                    SetColor(pallete[temp >> 4]);
                }
                else
                {
                    temp = *pData++;

                    // first pixel in byte
                    SetColor(pallete[temp & 0x0f]);
                }

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                     LCD_Write_DATA(_color);
                }
            }

            top++;
        }

        DisplayDisable();
    }
}

/*********************************************************************
* Function: void PutImage8BPPExt(SHORT left, SHORT top, void* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
void PutImage8BPPExt(SHORT left, SHORT top, void *image, BYTE stretch)
{
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    WORD            pallete[256];
    BYTE            lineBuffer[(GetMaxX() + 1)];
    BYTE            *pData;

    BYTE            temp;
    WORD            sizeX, sizeY;
    WORD            x, y;
    BYTE            stretchX, stretchY;

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Get pallete (256 entries)
    ExternalMemoryCallback(image, sizeof(BITMAP_HEADER), 256 * sizeof(WORD), pallete);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 256 * sizeof(WORD);

    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;

    for(y = 0; y < sizeY; y++)
    {

        // Get line
        ExternalMemoryCallback(image, memOffset, sizeX, lineBuffer);
        memOffset += sizeX;
        DisplayEnable();
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            pData = lineBuffer;
            SetAddress(left, top);
            LCD_Write_COM(0x2C);
            for(x = 0; x < sizeX; x++)
            {
                temp = *pData++;
                SetColor(pallete[temp]);

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    LCD_Write_DATA(_color);
                }
            }

            top++;
        }

        DisplayDisable();
    }
}

/*********************************************************************
* Function: void PutImage16BPPExt(SHORT left, SHORT top, void* image, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, image - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
void PutImage16BPPExt(SHORT left, SHORT top, void *image, BYTE stretch)
{
    register DWORD  memOffset;
    BITMAP_HEADER   bmp;
    WORD            lineBuffer[(GetMaxX() + 1)];
    WORD            *pData;
    WORD            byteWidth;

    WORD            temp;
    WORD            sizeX, sizeY;
    WORD            x, y;
    BYTE            stretchX, stretchY;

    // Get image header
    ExternalMemoryCallback(image, 0, sizeof(BITMAP_HEADER), &bmp);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER);

    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;

    byteWidth = sizeX << 1;

    for(y = 0; y < sizeY; y++)
    {

        // Get line
        ExternalMemoryCallback(image, memOffset, byteWidth, lineBuffer);
        memOffset += byteWidth;
        DisplayEnable();
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            pData = lineBuffer;
            SetAddress(left, top);
            LCD_Write_COM(0x2C);
            for(x = 0; x < sizeX; x++)
            {
                temp = *pData++;
                SetColor(temp);

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    LCD_Write_DATA(_color);
                }
            }

            top++;
        }

        DisplayDisable();
    }
}

    #endif //USE_BITMAP_EXTERNAL
#endif //#ifndef USE_TRANSPARENT_COLOR

#endif // USE_DRV_PUTIMAGE
