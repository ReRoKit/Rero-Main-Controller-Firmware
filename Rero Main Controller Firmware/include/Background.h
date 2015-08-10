/*******************************************************************************
 * This file is for the background running tasks.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef BACKGROUND_H
#define	BACKGROUND_H



/*******************************************************************************
 * PUBLIC DEFINITION
 *******************************************************************************/

// LED color on rero controller.
typedef enum __attribute__((packed)) {
    LED_OFF,
    LED_RED,
    LED_GREEN,
    LED_BLUE,
    LED_YELLOW,
    LED_CYAN,
    LED_MAGENTA,
    LED_WHITE
} LED_COLOR;



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

void vSetLed(LED_COLOR eColor);
void vBlinkLed(LED_COLOR eColor, unsigned char ucBlinkCount);
void taskShutdownWatchdog (void *pvParameters);
void taskStandbyWatchdog (void *pvParameters);
void taskBlinkLed (void *pvParameters);
void taskMonitorUsb (void *pvParameters);



#endif	/* BACKGROUND_H */
