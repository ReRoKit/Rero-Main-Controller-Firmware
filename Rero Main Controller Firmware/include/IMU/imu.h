/*******************************************************************************
 * This file provides the functions for the IMU module.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef IMU_H
#define	IMU_H



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

signed short ssInitIMU(void);
unsigned char ucCalibrateIMU(void);
void taskImu(void *pvParameters);



#endif	/* IMU_H */

