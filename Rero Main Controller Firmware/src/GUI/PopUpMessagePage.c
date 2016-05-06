/*******************************************************************************
 * Functions for pop up message in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "Variables.h"
#include "GUI/GraphicScreens.h"
#include "GUI/PopUpMessagePage.h"



/*******************************************************************************
 * FUNCTION: vCreateGenericMessagePage
 *
 * PARAMETERS:
 * ~ szText     - The text to display.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create a blank page with text message at the middle.
 * Font size = Large.
 *
 *******************************************************************************/
void vCreateGenericMessagePage(char *szText)
{
    void *pFont = (void *)ptrLargeFont;
    
    
    // Calculate the center position.
    SHORT sLeft = (GetMaxX() - GetTextWidth(szText, pFont)) / 2;
    SHORT sTop = (GetMaxY() - GetTextHeight(pFont)) / 2;;

    // Clear the screen.
    SetColor(pxDefaultScheme->CommonBkColor);
    ClearDevice();

    // Show the text.
    SetFont(pFont);
    SetColor(pxDefaultScheme->TextColor0);
    OutTextXY(sLeft, sTop, szText);
}



/*******************************************************************************
 * FUNCTION: vCreateLowBattShutdownMessage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the low batt shutting down message.
 *
 *******************************************************************************/
void vCreateLowBattShutdownMessage(void)
{
    const unsigned short BATT_POS_X = 115;
    const unsigned short BATT_POS_Y = 60;

    const unsigned short TXT1_POS_X = 100;
    const unsigned short TXT1_POS_Y = 120;

    const unsigned short TXT2_POS_X = 70;
    const unsigned short TXT2_POS_Y = 180;
    
    
    
    // Turn on the backlight.
    SetDCOC1PWM(BLight);

    // Clear all GOL objects.
    GOLFree();

    // Set background to red.
    SetColor(RED);
    ClearDevice();

    // Draw the battery icon.
    SetColor(WHITE);
    Bar( BATT_POS_X,      BATT_POS_Y + 15,
         BATT_POS_X + 10, BATT_POS_Y + 35 );

    Bar( BATT_POS_X + 10, BATT_POS_Y,
         BATT_POS_X + 90, BATT_POS_Y + 50);

    SetColor(RED);
    Bar( BATT_POS_X + 18, BATT_POS_Y + 8,
         BATT_POS_X + 82, BATT_POS_Y + 42);

    SetColor(WHITE);
    Bar( BATT_POS_X + 72, BATT_POS_Y + 13,
         BATT_POS_X + 77, BATT_POS_Y + 37);



    // Show the text.
    SetFont((void *)ptrLargeFont);
    OutTextXY(TXT1_POS_X, TXT1_POS_Y, "LOW BATTERY");

    SetFont((void *)ptrMediumFont);
    OutTextXY(TXT2_POS_X, TXT2_POS_Y, "Shutting down in 5 seconds...");
}



/*******************************************************************************
 * FUNCTION: vCreateChargeBatteryMessage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the charge battery message.
 *
 *******************************************************************************/
void vCreateChargeBatteryMessage(void)
{
    const unsigned short BATT_POS_X = 115;
    const unsigned short BATT_POS_Y = 60;

    const unsigned short TXT1_POS_X = 100;
    const unsigned short TXT1_POS_Y = 120;

    const unsigned short TXT2_POS_X = 80;
    const unsigned short TXT2_POS_Y = 180;
    
    
    
    // Turn on the backlight.
    SetDCOC1PWM(BLight);

    // Clear all GOL objects.
    GOLFree();

    // Set background to red.
    SetColor(RED);
    ClearDevice();

    // Draw the battery icon.
    SetColor(WHITE);
    Bar( BATT_POS_X,      BATT_POS_Y + 15,
         BATT_POS_X + 10, BATT_POS_Y + 35 );

    Bar( BATT_POS_X + 10, BATT_POS_Y,
         BATT_POS_X + 90, BATT_POS_Y + 50);

    SetColor(RED);
    Bar( BATT_POS_X + 18, BATT_POS_Y + 8,
         BATT_POS_X + 82, BATT_POS_Y + 42);

    SetColor(WHITE);
    Bar( BATT_POS_X + 72, BATT_POS_Y + 13,
         BATT_POS_X + 77, BATT_POS_Y + 37);



    // Show the text.
    SetFont((void *)ptrLargeFont);
    OutTextXY(TXT1_POS_X, TXT1_POS_Y, "LOW BATTERY");

    SetFont((void *)ptrMediumFont);
    OutTextXY(TXT2_POS_X, TXT2_POS_Y, "Please charge the battery!");
}



/*******************************************************************************
 * FUNCTION: vCreateShutdownErrorMessage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the error to shut down message.
 *
 *******************************************************************************/
void vCreateShutdownErrorMessage(void)
{
    // Clear all GOL objects.
    GOLFree();

    // Set background to red.
    SetColor(RED);
    ClearDevice();


    // The error title.
    char *szTitle = "UNABLE TO SHUT DOWN";

    // Calculate the position.
    SHORT sTitleWidth = GetTextWidth(szTitle, (void *)ptrLargeFont);
    SHORT sTitleHeight = GetTextHeight((void *)ptrLargeFont);
    SHORT sTitleLeft = (GetMaxX() + 1 - sTitleWidth) / 2;
    SHORT sTitleTop = 70;

    // Show the text.
    SetFont((void *)ptrLargeFont);
    SetColor(pxDefaultScheme->TextColor0);
    OutTextXY(sTitleLeft, sTitleTop, szTitle);


    // Draw box for the title.
    SHORT sLineWidth = 5;
    SHORT sLineSpacing = 10;

    // Top.
    Bar( sTitleLeft - sLineSpacing - sLineWidth,               sTitleTop - sLineSpacing - sLineWidth,
         sTitleLeft + sTitleWidth + sLineSpacing + sLineWidth, sTitleTop - sLineSpacing );

    // Bottom.
    Bar( sTitleLeft - sLineSpacing - sLineWidth,               sTitleTop + sTitleHeight + sLineSpacing,
         sTitleLeft + sTitleWidth + sLineSpacing + sLineWidth, sTitleTop + sTitleHeight + sLineSpacing + sLineWidth );

    // Left.
    Bar( sTitleLeft - sLineSpacing - sLineWidth, sTitleTop - sLineSpacing - sLineWidth,
         sTitleLeft - sLineSpacing,              sTitleTop + sTitleHeight + sLineSpacing + sLineWidth );

    // Rigth.
    Bar( sTitleLeft + sTitleWidth + sLineSpacing,              sTitleTop - sLineSpacing - sLineWidth,
         sTitleLeft + sTitleWidth + sLineSpacing + sLineWidth, sTitleTop + sTitleHeight + sLineSpacing + sLineWidth );



    // The error message.
    char *szMessage = "Please unplug the battery manually.";

    // Calculate the center position.
    SHORT sMessageLeft = (GetMaxX() + 1 - GetTextWidth(szMessage, (void *)ptrLargeFont)) / 2;
    SHORT sMessageTop = 150;

    // Show the text.
    SetFont((void *)ptrLargeFont);
    SetColor(pxDefaultScheme->TextColor0);
    OutTextXY(sMessageLeft, sMessageTop, szMessage);



    // The contact support message.
    char *szContact = "Support Email: support@cytron.com.my";

    // Calculate the center position.
    SHORT sContactLeft = (GetMaxX() + 1 - GetTextWidth(szContact, (void *)ptrMediumFont)) / 2;
    SHORT sContactTop = 180;

    // Show the text.
    SetFont((void *)ptrMediumFont);
    SetColor(pxLightBlueTxtScheme->TextColor0);
    OutTextXY(sContactLeft, sContactTop, szContact);
}
