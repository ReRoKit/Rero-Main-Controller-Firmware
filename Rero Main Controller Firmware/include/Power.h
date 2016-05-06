/*******************************************************************************
 * Functions to handle the power.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef POWER_H
#define	POWER_H



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                   *
 *******************************************************************************/

void taskBattMonitor( void *pvParameters );
void vPreventLowBattStartup(void);
void vPowerOn(void);
void vPowerOff(void);
void taskPowerOff (void *pvParameters);



#endif	/* POWER_H */
