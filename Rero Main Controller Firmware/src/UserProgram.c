/*******************************************************************************
 * Functions for user defined program.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "UserProgram.h"
#include "HardwareProfile.h"
#include "G15 and Sensors/G15.h"
#include "G15 and Sensors/Sensors.h"

// Header files for FreeRTOS.
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



#ifdef RUN_USER_PROGRAM
/*******************************************************************************
* TASK: taskBluetoothProgram
*
* DESCRIPTIONS:
* Task for the bluetooth programming.
*
*******************************************************************************/
void taskUserProgram(void *pvParameters)
{
    // Set the G15 angle limit (0 - 1087).
    eG15SetAngleLimit(11, 0, 1087);

    // Turn on torque for G15.
    eG15SetTorqueLed(11, WRITE_NOW, 1, 1);

    

    while (1) {
        // Read from IR sensor.
        unsigned char ucSensorValue;
        eInfraredSensorGetValue(101, &ucSensorValue);

        // Set the G15 position according to the sensor value.
        eG15SetPositionSpeed(11, WRITE_NOW, ucSensorValue, NORMAL_POSITIONING, 0, POSITION_SPEED_CONTROL);

//        // Toggle blue LED.
//        LEDB ^= 1;
//        vTaskDelay(50 / portTICK_RATE_MS);
    }
    
}
#endif
