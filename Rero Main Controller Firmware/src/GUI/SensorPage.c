/*******************************************************************************
 * Functions for sensor page in rero GUI.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Variables.h"
#include "Microchip/Graphics/Graphics.h"
#include "GUI/GraphicScreens.h"
#include "GUI/ScrollbarListbox.h"
#include "G15 and Sensors/Sensors.h"
#include "IMU/imu.h"
#include "GUI/SensorPage.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

// Subpage position and margin.
#define SENSOR_MARGIN_LEFT      10
#define SENSOR_MARGIN_RIGHT     5
#define SENSOR_MARGIN_TOP       5
#define SENSOR_MARGIN_BOTTOM    10

#define SENSOR_LIMIT_LEFT       (EM_SUBPAGE_LEFT + SENSOR_MARGIN_LEFT)
#define SENSOR_LIMIT_RIGHT      (EM_SUBPAGE_RIGHT - SENSOR_MARGIN_RIGHT)
#define SENSOR_LIMIT_TOP        (EM_SUBPAGE_TOP + SENSOR_MARGIN_TOP)
#define SENSOR_LIMIT_BOTTOM     (EM_SUBPAGE_BOTTOM - SENSOR_MARGIN_BOTTOM)


// Position and size for graphic objects.
#define TXT_HEIGHT              20
#define SLD_HEIGHT              20
#define TXT_SLD_GAP             1
#define TXT_SLD_HEIGHT          (TXT_HEIGHT + SLD_HEIGHT + TXT_SLD_GAP)

#define TXT_TYPE_X1             SENSOR_LIMIT_LEFT
#define TXT_TYPE_X2             SENSOR_LIMIT_RIGHT
#define TXT_TYPE_Y1             SENSOR_LIMIT_TOP
#define TXT_TYPE_Y2             (TXT_TYPE_Y1 + TXT_HEIGHT)

#define SENSOR_PAGE_CENTER_Y    ((TXT_TYPE_Y2 + SENSOR_LIMIT_BOTTOM) / 2 + 9)


// Single slider subpage.
#define PG1SLD_SLD_VALUE_WIDTH  170
#define PG1SLD_SLD_VALUE_HEIGHT 30

#define PG1SLD_SLD_VALUE_X1     SENSOR_LIMIT_LEFT
#define PG1SLD_SLD_VALUE_X2     (PG1SLD_SLD_VALUE_X1 + PG1SLD_SLD_VALUE_WIDTH)
#define PG1SLD_SLD_VALUE_Y1     (SENSOR_PAGE_CENTER_Y - (PG1SLD_SLD_VALUE_HEIGHT / 2))
#define PG1SLD_SLD_VALUE_Y2     (PG1SLD_SLD_VALUE_Y1 + PG1SLD_SLD_VALUE_HEIGHT)

#define PG1SLD_TXT_VALUE_X1     PG1SLD_SLD_VALUE_X1
#define PG1SLD_TXT_VALUE_X2     PG1SLD_SLD_VALUE_X2
#define PG1SLD_TXT_VALUE_Y2     (PG1SLD_SLD_VALUE_Y1 - TXT_SLD_GAP)
#define PG1SLD_TXT_VALUE_Y1     (PG1SLD_TXT_VALUE_Y2 - TXT_HEIGHT)


// Head module subpage.
#define PGHM_SLD_VALUE_WIDTH    90
#define PGHM_SLD_C2C            70      // Center to center distance between each slider.

#define PGHM_SLD_VALUE1_X1      SENSOR_LIMIT_LEFT
#define PGHM_SLD_VALUE1_X2      (PGHM_SLD_VALUE1_X1 + PGHM_SLD_VALUE_WIDTH)
#define PGHM_SLD_VALUE1_Y1      ((SENSOR_PAGE_CENTER_Y - (PGHM_SLD_C2C / 2) - (SLD_HEIGHT / 2)))
#define PGHM_SLD_VALUE1_Y2      (PGHM_SLD_VALUE1_Y1 + SLD_HEIGHT)

#define PGHM_TXT_VALUE1_X1      PGHM_SLD_VALUE1_X1
#define PGHM_TXT_VALUE1_X2      PGHM_SLD_VALUE1_X2
#define PGHM_TXT_VALUE1_Y2      (PGHM_SLD_VALUE1_Y1 - TXT_SLD_GAP)
#define PGHM_TXT_VALUE1_Y1      (PGHM_TXT_VALUE1_Y2 - TXT_HEIGHT)


#define PGHM_SLD_VALUE2_X1      PGHM_SLD_VALUE1_X1
#define PGHM_SLD_VALUE2_X2      PGHM_SLD_VALUE1_X2
#define PGHM_SLD_VALUE2_Y1      ((SENSOR_PAGE_CENTER_Y + (PGHM_SLD_C2C / 2) - (SLD_HEIGHT / 2)))
#define PGHM_SLD_VALUE2_Y2      (PGHM_SLD_VALUE2_Y1 + SLD_HEIGHT)

#define PGHM_TXT_VALUE2_X1      PGHM_SLD_VALUE1_X1
#define PGHM_TXT_VALUE2_X2      PGHM_SLD_VALUE1_X2
#define PGHM_TXT_VALUE2_Y2      (PGHM_SLD_VALUE2_Y1 - TXT_SLD_GAP)
#define PGHM_TXT_VALUE2_Y1      (PGHM_TXT_VALUE2_Y2 - TXT_HEIGHT)


#define PGHM_CB_HEIGHT          26
#define PGHM_SLD_CB_GAP         20
#define PGHM_TXT_CB_GAP         5

#define PGHM_CB_RED_X1          (PGHM_SLD_VALUE1_X2 + PGHM_SLD_CB_GAP)
#define PGHM_CB_RED_X2          SENSOR_LIMIT_RIGHT
#define PGHM_CB_RED_Y1          (PGHM_SLD_VALUE1_Y1 + (SLD_HEIGHT / 2) - (PGHM_CB_HEIGHT / 2))
#define PGHM_CB_RED_Y2          (PGHM_CB_RED_Y1 + PGHM_CB_HEIGHT)

#define PGHM_CB_GREEN_X1        PGHM_CB_RED_X1
#define PGHM_CB_GREEN_X2        PGHM_CB_RED_X2
#define PGHM_CB_GREEN_Y1        (SENSOR_PAGE_CENTER_Y - (PGHM_CB_HEIGHT / 2))
#define PGHM_CB_GREEN_Y2        (PGHM_CB_GREEN_Y1 + PGHM_CB_HEIGHT)

#define PGHM_CB_BLUE_X1         PGHM_CB_RED_X1
#define PGHM_CB_BLUE_X2         PGHM_CB_RED_X2
#define PGHM_CB_BLUE_Y1         (PGHM_SLD_VALUE2_Y1 + (SLD_HEIGHT / 2) - (PGHM_CB_HEIGHT / 2))
#define PGHM_CB_BLUE_Y2         (PGHM_CB_BLUE_Y1 + PGHM_CB_HEIGHT)

#define PGHM_TXT_LED_X1         PGHM_CB_RED_X1
#define PGHM_TXT_LED_X2         PGHM_CB_RED_X2
#define PGHM_TXT_LED_Y2         (PGHM_CB_RED_Y1 - PGHM_TXT_CB_GAP)
#define PGHM_TXT_LED_Y1         (PGHM_TXT_LED_Y2 - TXT_HEIGHT)


// Colour sensor subpage.
#define PGCL_SLD_VALUE123_WIDTH 70
#define PGCL_SLD_VALUE45_WIDTH  95
#define PGCL_OBJ_GRP_C2C_Y      50      // Center to center distance between each object group.
#define PGCL_OBJ_GRP_GAP_X      15

#define PGCL_SLD_VALUE1_X1      SENSOR_LIMIT_LEFT
#define PGCL_SLD_VALUE1_X2      (PGCL_SLD_VALUE1_X1 + PGCL_SLD_VALUE123_WIDTH)
#define PGCL_SLD_VALUE1_Y1      (SENSOR_PAGE_CENTER_Y - PGCL_OBJ_GRP_C2C_Y - (SLD_HEIGHT / 2))
#define PGCL_SLD_VALUE1_Y2      (PGCL_SLD_VALUE1_Y1 + SLD_HEIGHT)

#define PGCL_TXT_VALUE1_X1      PGCL_SLD_VALUE1_X1
#define PGCL_TXT_VALUE1_X2      PGCL_SLD_VALUE1_X2
#define PGCL_TXT_VALUE1_Y2      (PGCL_SLD_VALUE1_Y1 - TXT_SLD_GAP)
#define PGCL_TXT_VALUE1_Y1      (PGCL_TXT_VALUE1_Y2 - TXT_HEIGHT)


#define PGCL_SLD_VALUE2_X1      PGCL_SLD_VALUE1_X1
#define PGCL_SLD_VALUE2_X2      PGCL_SLD_VALUE1_X2
#define PGCL_SLD_VALUE2_Y1      (SENSOR_PAGE_CENTER_Y - (SLD_HEIGHT / 2))
#define PGCL_SLD_VALUE2_Y2      (PGCL_SLD_VALUE2_Y1 + SLD_HEIGHT)

#define PGCL_TXT_VALUE2_X1      PGCL_SLD_VALUE1_X1
#define PGCL_TXT_VALUE2_X2      PGCL_SLD_VALUE1_X2
#define PGCL_TXT_VALUE2_Y2      (PGCL_SLD_VALUE2_Y1 - TXT_SLD_GAP)
#define PGCL_TXT_VALUE2_Y1      (PGCL_TXT_VALUE2_Y2 - TXT_HEIGHT)


#define PGCL_SLD_VALUE3_X1      PGCL_SLD_VALUE1_X1
#define PGCL_SLD_VALUE3_X2      PGCL_SLD_VALUE1_X2
#define PGCL_SLD_VALUE3_Y1      (SENSOR_PAGE_CENTER_Y + PGCL_OBJ_GRP_C2C_Y - (SLD_HEIGHT / 2))
#define PGCL_SLD_VALUE3_Y2      (PGCL_SLD_VALUE3_Y1 + SLD_HEIGHT)

#define PGCL_TXT_VALUE3_X1      PGCL_SLD_VALUE1_X1
#define PGCL_TXT_VALUE3_X2      PGCL_SLD_VALUE1_X2
#define PGCL_TXT_VALUE3_Y2      (PGCL_SLD_VALUE3_Y1 - TXT_SLD_GAP)
#define PGCL_TXT_VALUE3_Y1      (PGCL_TXT_VALUE3_Y2 - TXT_HEIGHT)


#define PGCL_SLD_VALUE4_X1      (PGCL_SLD_VALUE1_X2 + PGCL_OBJ_GRP_GAP_X)
#define PGCL_SLD_VALUE4_X2      (PGCL_SLD_VALUE4_X1 + PGCL_SLD_VALUE45_WIDTH)
#define PGCL_SLD_VALUE4_Y1      PGCL_SLD_VALUE1_Y1
#define PGCL_SLD_VALUE4_Y2      PGCL_SLD_VALUE1_Y2

#define PGCL_TXT_VALUE4_X1      PGCL_SLD_VALUE4_X1
#define PGCL_TXT_VALUE4_X2      PGCL_SLD_VALUE4_X2
#define PGCL_TXT_VALUE4_Y2      PGCL_TXT_VALUE1_Y2
#define PGCL_TXT_VALUE4_Y1      PGCL_TXT_VALUE1_Y1


#define PGCL_SLD_VALUE5_X1      PGCL_SLD_VALUE4_X1
#define PGCL_SLD_VALUE5_X2      PGCL_SLD_VALUE4_X2
#define PGCL_SLD_VALUE5_Y1      PGCL_SLD_VALUE2_Y1
#define PGCL_SLD_VALUE5_Y2      PGCL_SLD_VALUE2_Y2

#define PGCL_TXT_VALUE5_X1      PGCL_SLD_VALUE4_X1
#define PGCL_TXT_VALUE5_X2      PGCL_SLD_VALUE4_X2
#define PGCL_TXT_VALUE5_Y2      PGCL_TXT_VALUE2_Y2
#define PGCL_TXT_VALUE5_Y1      PGCL_TXT_VALUE2_Y1


#define PGCL_TXT_COLOUR2_X1     PGCL_SLD_VALUE4_X1
#define PGCL_TXT_COLOUR2_X2     PGCL_SLD_VALUE4_X2
#define PGCL_TXT_COLOUR2_Y1     PGCL_SLD_VALUE3_Y1
#define PGCL_TXT_COLOUR2_Y2     PGCL_SLD_VALUE3_Y2

#define PGCL_TXT_COLOUR1_X1     PGCL_SLD_VALUE4_X1
#define PGCL_TXT_COLOUR1_X2     PGCL_SLD_VALUE4_X2
#define PGCL_TXT_COLOUR1_Y2     PGCL_TXT_VALUE3_Y2
#define PGCL_TXT_COLOUR1_Y1     PGCL_TXT_VALUE3_Y1


// Line sensor subpage.
#define PGLN_SLD_VALUE_WIDTH    20
#define PGLN_SLD_VALUE_HEIGHT   70
#define PGLN_SLD_VALUE_GAP      10


#define PGLN_TXT_VALUE1_X1      SENSOR_LIMIT_LEFT
#define PGLN_TXT_VALUE1_X2      (PGLN_TXT_VALUE1_X1 + PGLN_SLD_VALUE_WIDTH)
#define PGLN_TXT_VALUE1_Y1      (TXT_TYPE_Y2 + 2)
#define PGLN_TXT_VALUE1_Y2      (PGLN_TXT_VALUE1_Y1 + TXT_HEIGHT)

#define PGLN_SLD_VALUE1_X1      PGLN_TXT_VALUE1_X1
#define PGLN_SLD_VALUE1_X2      PGLN_TXT_VALUE1_X2
#define PGLN_SLD_VALUE1_Y1      (PGLN_TXT_VALUE1_Y2 + TXT_SLD_GAP)
#define PGLN_SLD_VALUE1_Y2      (PGLN_SLD_VALUE1_Y1 + PGLN_SLD_VALUE_HEIGHT)

#define PGLN_TXT_TITLE1_X1      PGLN_TXT_VALUE1_X1
#define PGLN_TXT_TITLE1_X2      PGLN_TXT_VALUE1_X2
#define PGLN_TXT_TITLE1_Y1      (PGLN_SLD_VALUE1_Y2 + TXT_SLD_GAP)
#define PGLN_TXT_TITLE1_Y2      (PGLN_TXT_TITLE1_Y1 + TXT_HEIGHT)


#define PGLN_TXT_VALUE2_X1      (PGLN_TXT_VALUE1_X2 + PGLN_SLD_VALUE_GAP)
#define PGLN_TXT_VALUE2_X2      (PGLN_TXT_VALUE2_X1 + PGLN_SLD_VALUE_WIDTH)
#define PGLN_TXT_VALUE2_Y1      PGLN_TXT_VALUE1_Y1
#define PGLN_TXT_VALUE2_Y2      PGLN_TXT_VALUE1_Y2

#define PGLN_SLD_VALUE2_X1      PGLN_TXT_VALUE2_X1
#define PGLN_SLD_VALUE2_X2      PGLN_TXT_VALUE2_X2
#define PGLN_SLD_VALUE2_Y1      PGLN_SLD_VALUE1_Y1
#define PGLN_SLD_VALUE2_Y2      PGLN_SLD_VALUE1_Y2

#define PGLN_TXT_TITLE2_X1      PGLN_TXT_VALUE2_X1
#define PGLN_TXT_TITLE2_X2      PGLN_TXT_VALUE2_X2
#define PGLN_TXT_TITLE2_Y1      PGLN_TXT_TITLE1_Y1
#define PGLN_TXT_TITLE2_Y2      PGLN_TXT_TITLE1_Y2


#define PGLN_TXT_VALUE3_X1      (PGLN_TXT_VALUE2_X2 + PGLN_SLD_VALUE_GAP)
#define PGLN_TXT_VALUE3_X2      (PGLN_TXT_VALUE3_X1 + PGLN_SLD_VALUE_WIDTH)
#define PGLN_TXT_VALUE3_Y1      PGLN_TXT_VALUE1_Y1
#define PGLN_TXT_VALUE3_Y2      PGLN_TXT_VALUE1_Y2

#define PGLN_SLD_VALUE3_X1      PGLN_TXT_VALUE3_X1
#define PGLN_SLD_VALUE3_X2      PGLN_TXT_VALUE3_X2
#define PGLN_SLD_VALUE3_Y1      PGLN_SLD_VALUE1_Y1
#define PGLN_SLD_VALUE3_Y2      PGLN_SLD_VALUE1_Y2

#define PGLN_TXT_TITLE3_X1      PGLN_TXT_VALUE3_X1
#define PGLN_TXT_TITLE3_X2      PGLN_TXT_VALUE3_X2
#define PGLN_TXT_TITLE3_Y1      PGLN_TXT_TITLE1_Y1
#define PGLN_TXT_TITLE3_Y2      PGLN_TXT_TITLE1_Y2


#define PGLN_TXT_VALUE4_X1      (PGLN_TXT_VALUE3_X2 + PGLN_SLD_VALUE_GAP)
#define PGLN_TXT_VALUE4_X2      (PGLN_TXT_VALUE4_X1 + PGLN_SLD_VALUE_WIDTH)
#define PGLN_TXT_VALUE4_Y1      PGLN_TXT_VALUE1_Y1
#define PGLN_TXT_VALUE4_Y2      PGLN_TXT_VALUE1_Y2

#define PGLN_SLD_VALUE4_X1      PGLN_TXT_VALUE4_X1
#define PGLN_SLD_VALUE4_X2      PGLN_TXT_VALUE4_X2
#define PGLN_SLD_VALUE4_Y1      PGLN_SLD_VALUE1_Y1
#define PGLN_SLD_VALUE4_Y2      PGLN_SLD_VALUE1_Y2

#define PGLN_TXT_TITLE4_X1      PGLN_TXT_VALUE4_X1
#define PGLN_TXT_TITLE4_X2      PGLN_TXT_VALUE4_X2
#define PGLN_TXT_TITLE4_Y1      PGLN_TXT_TITLE1_Y1
#define PGLN_TXT_TITLE4_Y2      PGLN_TXT_TITLE1_Y2


#define PGLN_TXT_LINE_GAP       10
#define PGLN_RB_LINE_HEIGHT     26

#define PGLN_TXT_LINE_X1        (PGLN_TXT_VALUE4_X2 + PGLN_SLD_VALUE_GAP)
#define PGLN_TXT_LINE_X2        SENSOR_LIMIT_RIGHT
#define PGLN_TXT_LINE_Y1        PGLN_TXT_VALUE1_Y1
#define PGLN_TXT_LINE_Y2        (PGLN_TXT_LINE_Y1 + TXT_HEIGHT)

#define PGLN_RB_DARK_X1         PGLN_TXT_LINE_X1
#define PGLN_RB_DARK_X2         PGLN_TXT_LINE_X2
#define PGLN_RB_DARK_Y1         (PGLN_TXT_LINE_Y2 + PGLN_TXT_LINE_GAP)
#define PGLN_RB_DARK_Y2         (PGLN_RB_DARK_Y1 + PGLN_RB_LINE_HEIGHT)

#define PGLN_RB_LIGHT_X1        PGLN_TXT_LINE_X1
#define PGLN_RB_LIGHT_X2        PGLN_TXT_LINE_X2
#define PGLN_RB_LIGHT_Y1        (PGLN_RB_DARK_Y2 + PGLN_TXT_LINE_GAP)
#define PGLN_RB_LIGHT_Y2        (PGLN_RB_LIGHT_Y1 + PGLN_RB_LINE_HEIGHT)


#define PGLN_BTN_CAL_HEIGHT     30
#define PGLN_BTN_CAL_X1         SENSOR_LIMIT_LEFT
#define PGLN_BTN_CAL_X2         (EM_SUBPAGE_RIGHT - 20)
#define PGLN_BTN_CAL_Y2         SENSOR_LIMIT_BOTTOM
#define PGLN_BTN_CAL_Y1         (PGLN_BTN_CAL_Y2 - PGLN_BTN_CAL_HEIGHT)


// IMU subpage.
#define PGIMU_SLD_VALUE_WIDTH   80
#define PGIMU_SLD_C2C_Y         50      // Center to center distance between each slider.
#define PGIMU_SLD_GAP_X         20


#define PGIMU_SLD_VALUE1_X1     SENSOR_LIMIT_LEFT
#define PGIMU_SLD_VALUE1_X2     (PGIMU_SLD_VALUE1_X1 + PGIMU_SLD_VALUE_WIDTH)
#define PGIMU_SLD_VALUE1_Y1     (SENSOR_PAGE_CENTER_Y - PGIMU_SLD_C2C_Y - (SLD_HEIGHT / 2))
#define PGIMU_SLD_VALUE1_Y2     (PGIMU_SLD_VALUE1_Y1 + SLD_HEIGHT)

#define PGIMU_TXT_VALUE1_X1     PGIMU_SLD_VALUE1_X1
#define PGIMU_TXT_VALUE1_X2     PGIMU_SLD_VALUE1_X2
#define PGIMU_TXT_VALUE1_Y2     (PGIMU_SLD_VALUE1_Y1 - TXT_SLD_GAP)
#define PGIMU_TXT_VALUE1_Y1     (PGIMU_TXT_VALUE1_Y2 - TXT_HEIGHT)


#define PGIMU_SLD_VALUE2_X1     PGIMU_SLD_VALUE1_X1
#define PGIMU_SLD_VALUE2_X2     PGIMU_SLD_VALUE1_X2
#define PGIMU_SLD_VALUE2_Y1     (SENSOR_PAGE_CENTER_Y - (SLD_HEIGHT / 2))
#define PGIMU_SLD_VALUE2_Y2     (PGIMU_SLD_VALUE2_Y1 + SLD_HEIGHT)

#define PGIMU_TXT_VALUE2_X1     PGIMU_SLD_VALUE1_X1
#define PGIMU_TXT_VALUE2_X2     PGIMU_SLD_VALUE1_X2
#define PGIMU_TXT_VALUE2_Y2     (PGIMU_SLD_VALUE2_Y1 - TXT_SLD_GAP)
#define PGIMU_TXT_VALUE2_Y1     (PGIMU_TXT_VALUE2_Y2 - TXT_HEIGHT)


#define PGIMU_SLD_VALUE3_X1     PGIMU_SLD_VALUE1_X1
#define PGIMU_SLD_VALUE3_X2     PGIMU_SLD_VALUE1_X2
#define PGIMU_SLD_VALUE3_Y1     (SENSOR_PAGE_CENTER_Y + PGIMU_SLD_C2C_Y - (SLD_HEIGHT / 2))
#define PGIMU_SLD_VALUE3_Y2     (PGIMU_SLD_VALUE3_Y1 + SLD_HEIGHT)

#define PGIMU_TXT_VALUE3_X1     PGIMU_SLD_VALUE1_X1
#define PGIMU_TXT_VALUE3_X2     PGIMU_SLD_VALUE1_X2
#define PGIMU_TXT_VALUE3_Y2     (PGIMU_SLD_VALUE3_Y1 - TXT_SLD_GAP)
#define PGIMU_TXT_VALUE3_Y1     (PGIMU_TXT_VALUE3_Y2 - TXT_HEIGHT)


#define PGIMU_SLD_VALUE4_X1     (PGIMU_SLD_VALUE1_X2 + PGIMU_SLD_GAP_X)
#define PGIMU_SLD_VALUE4_X2     (PGIMU_SLD_VALUE4_X1 + PGIMU_SLD_VALUE_WIDTH)
#define PGIMU_SLD_VALUE4_Y1     PGIMU_SLD_VALUE1_Y1
#define PGIMU_SLD_VALUE4_Y2     PGIMU_SLD_VALUE1_Y2

#define PGIMU_TXT_VALUE4_X1     PGIMU_SLD_VALUE4_X1
#define PGIMU_TXT_VALUE4_X2     PGIMU_SLD_VALUE4_X2
#define PGIMU_TXT_VALUE4_Y2     PGIMU_TXT_VALUE1_Y2
#define PGIMU_TXT_VALUE4_Y1     PGIMU_TXT_VALUE1_Y1


#define PGIMU_SLD_VALUE5_X1     PGIMU_SLD_VALUE4_X1
#define PGIMU_SLD_VALUE5_X2     PGIMU_SLD_VALUE4_X2
#define PGIMU_SLD_VALUE5_Y1     PGIMU_SLD_VALUE2_Y1
#define PGIMU_SLD_VALUE5_Y2     PGIMU_SLD_VALUE2_Y2

#define PGIMU_TXT_VALUE5_X1     PGIMU_SLD_VALUE4_X1
#define PGIMU_TXT_VALUE5_X2     PGIMU_SLD_VALUE4_X2
#define PGIMU_TXT_VALUE5_Y2     PGIMU_TXT_VALUE2_Y2
#define PGIMU_TXT_VALUE5_Y1     PGIMU_TXT_VALUE2_Y1


#define PGIMU_SLD_VALUE6_X1     PGIMU_SLD_VALUE4_X1
#define PGIMU_SLD_VALUE6_X2     PGIMU_SLD_VALUE4_X2
#define PGIMU_SLD_VALUE6_Y1     PGIMU_SLD_VALUE3_Y1
#define PGIMU_SLD_VALUE6_Y2     PGIMU_SLD_VALUE3_Y2

#define PGIMU_TXT_VALUE6_X1     PGIMU_SLD_VALUE4_X1
#define PGIMU_TXT_VALUE6_X2     PGIMU_SLD_VALUE4_X2
#define PGIMU_TXT_VALUE6_Y2     PGIMU_TXT_VALUE3_Y2
#define PGIMU_TXT_VALUE6_Y1     PGIMU_TXT_VALUE3_Y1


// RGB light module subpage.
#define PGLM_SLD_VALUE_WIDTH    100
#define PGLM_SLD_VALUE_HEIGHT   25
#define PGLM_OBJ_GRP_C2C_Y      40      // Center to center distance between each object group.
#define PGLM_TXT_SLD_GAP        5

#define PGLM_TXT_TITLE1_X1      SENSOR_LIMIT_LEFT
#define PGLM_TXT_TITLE1_X2      SENSOR_LIMIT_LEFT + 45
#define PGLM_TXT_TITLE1_Y1      PGLM_SLD_VALUE1_Y1
#define PGLM_TXT_TITLE1_Y2      PGLM_SLD_VALUE1_Y2

#define PGLM_SLD_VALUE1_X1      PGLM_TXT_TITLE1_X2
#define PGLM_SLD_VALUE1_X2      (PGLM_SLD_VALUE1_X1 + PGLM_SLD_VALUE_WIDTH) 
#define PGLM_SLD_VALUE1_Y1      (SENSOR_PAGE_CENTER_Y - PGLM_OBJ_GRP_C2C_Y - (PGLM_SLD_VALUE_HEIGHT / 2))
#define PGLM_SLD_VALUE1_Y2      (PGLM_SLD_VALUE1_Y1 + PGLM_SLD_VALUE_HEIGHT)

#define PGLM_TXT_VALUE1_X1      (PGLM_SLD_VALUE1_X2 + PGLM_TXT_SLD_GAP)
#define PGLM_TXT_VALUE1_X2      SENSOR_LIMIT_RIGHT
#define PGLM_TXT_VALUE1_Y1      PGLM_SLD_VALUE1_Y1
#define PGLM_TXT_VALUE1_Y2      PGLM_SLD_VALUE1_Y2


#define PGLM_TXT_TITLE2_X1      PGLM_TXT_TITLE1_X1
#define PGLM_TXT_TITLE2_X2      PGLM_TXT_TITLE1_X2
#define PGLM_TXT_TITLE2_Y1      PGLM_SLD_VALUE2_Y1
#define PGLM_TXT_TITLE2_Y2      PGLM_SLD_VALUE2_Y2

#define PGLM_SLD_VALUE2_X1      PGLM_SLD_VALUE1_X1
#define PGLM_SLD_VALUE2_X2      PGLM_SLD_VALUE1_X2
#define PGLM_SLD_VALUE2_Y1      (SENSOR_PAGE_CENTER_Y - (PGLM_SLD_VALUE_HEIGHT / 2))
#define PGLM_SLD_VALUE2_Y2      (PGLM_SLD_VALUE2_Y1 + PGLM_SLD_VALUE_HEIGHT)

#define PGLM_TXT_VALUE2_X1      PGLM_TXT_VALUE1_X1
#define PGLM_TXT_VALUE2_X2      PGLM_TXT_VALUE1_X2
#define PGLM_TXT_VALUE2_Y1      PGLM_SLD_VALUE2_Y1
#define PGLM_TXT_VALUE2_Y2      PGLM_SLD_VALUE2_Y2


#define PGLM_TXT_TITLE3_X1      PGLM_TXT_TITLE1_X1
#define PGLM_TXT_TITLE3_X2      PGLM_TXT_TITLE1_X2
#define PGLM_TXT_TITLE3_Y1      PGLM_SLD_VALUE3_Y1
#define PGLM_TXT_TITLE3_Y2      PGLM_SLD_VALUE3_Y2

#define PGLM_SLD_VALUE3_X1      PGLM_SLD_VALUE1_X1
#define PGLM_SLD_VALUE3_X2      PGLM_SLD_VALUE1_X2
#define PGLM_SLD_VALUE3_Y1      (SENSOR_PAGE_CENTER_Y + PGLM_OBJ_GRP_C2C_Y - (PGLM_SLD_VALUE_HEIGHT / 2))
#define PGLM_SLD_VALUE3_Y2      (PGLM_SLD_VALUE3_Y1 + PGLM_SLD_VALUE_HEIGHT)

#define PGLM_TXT_VALUE3_X1      PGLM_TXT_VALUE1_X1
#define PGLM_TXT_VALUE3_X2      PGLM_TXT_VALUE1_X2
#define PGLM_TXT_VALUE3_Y1      PGLM_SLD_VALUE3_Y1
#define PGLM_TXT_VALUE3_Y2      PGLM_SLD_VALUE3_Y2


//#define PGLM_TXT_TITLE4_X1      PGLM_TXT_TITLE1_X1
//#define PGLM_TXT_TITLE4_X2      PGLM_TXT_TITLE1_X2
//#define PGLM_TXT_TITLE4_Y1      PGLM_SLD_VALUE4_Y1
//#define PGLM_TXT_TITLE4_Y2      PGLM_SLD_VALUE4_Y2
//
//#define PGLM_SLD_VALUE4_X1      PGLM_SLD_VALUE1_X1
//#define PGLM_SLD_VALUE4_X2      PGLM_SLD_VALUE1_X2
//#define PGLM_SLD_VALUE4_Y1      (SENSOR_PAGE_CENTER_Y + (PGLM_OBJ_GRP_C2C_Y / 2) + PGLM_OBJ_GRP_C2C_Y - (PGLM_SLD_VALUE_HEIGHT / 2))
//#define PGLM_SLD_VALUE4_Y2      (PGLM_SLD_VALUE4_Y1 + PGLM_SLD_VALUE_HEIGHT)
//
//#define PGLM_TXT_VALUE4_X1      PGLM_TXT_VALUE1_X1
//#define PGLM_TXT_VALUE4_X2      PGLM_TXT_VALUE1_X2
//#define PGLM_TXT_VALUE4_Y1      PGLM_SLD_VALUE4_Y1
//#define PGLM_TXT_VALUE4_Y2      PGLM_SLD_VALUE4_Y2



/*******************************************************************************
* PRIVATE GLOBAL VARIABLES
*******************************************************************************/

static unsigned char prv_ucSelectedSensorId = 0xff;
static EM_MODEL prv_eSelectedSensorType = EM_MODEL_NONE;

static char prv_szFirmwareRevision[] = "rev:--";

static char prv_szValue1[] = "Value: 000cm   ";
static char prv_szValue2[] = "Value: 000cm   ";
static char prv_szValue3[] = "Value: 000cm   ";
static char prv_szValue4[] = "Value: 000cm   ";
static char prv_szValue5[] = "Value: 000cm   ";
static char prv_szValue6[] = "Value: 000cm   ";

static unsigned char prv_ucLighModuleFirstSelected = 0;


/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 *******************************************************************************/

static void prv_vCreateSensorSubpage(void);
static void prv_vCreateSingleSliderSubpage(char *szSensorType, unsigned short usSliderRange);
static void prv_vCreateHeadModuleSubpage(void);
static void prv_vCreateImuSubpage(void);
static void prv_vCreateColourSensorSubpage(void);
static void prv_vCreateLineSensorSubpage(void);
static void prv_vCreateRgbLightModuleSubpage(void);



/*******************************************************************************
 * FUNCTION: prv_vCreateSensorSubpage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the sensor sub page.
 *
 *******************************************************************************/
static void prv_vCreateSensorSubpage(void)
{
    // Clear the subpage.
    vClearExtModuleSubpage();

    // Create the subpage for the sensor.
    switch (prv_eSelectedSensorType) {
        case EM_MODEL_IR:       prv_vCreateSingleSliderSubpage("INFRARED SENSOR", 255);     break;
        case EM_MODEL_US:       prv_vCreateSingleSliderSubpage("ULTRASONIC SENSOR", 255);   break;
        case EM_MODEL_TACTILE:  prv_vCreateSingleSliderSubpage("TACTILE SENSOR", 1);        break;
        case EM_MODEL_COLOUR:   prv_vCreateColourSensorSubpage();                           break;
        case EM_MODEL_LINE:     prv_vCreateLineSensorSubpage();                             break;
        case EM_MODEL_HEAD:     prv_vCreateHeadModuleSubpage();                             break;
        case EM_MODEL_IMU:      prv_vCreateImuSubpage();                                    break;
        case EM_MODEL_RGBLIGHT: prv_vCreateRgbLightModuleSubpage();                         break;
    }
}



/*******************************************************************************
 * FUNCTION: prv_vCreateSingleSliderSubpage
 *
 * PARAMETERS:
 * ~ szSensorType   - Text for the sensor type.
 * ~ usSliderRange  - Range for the slider.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the sensor subpage with single slider.
 *
 *******************************************************************************/
static void prv_vCreateSingleSliderSubpage(char *szSensorType, unsigned short usSliderRange)
{
    // Create text for sensor type.
    StCreate( GID_SENSOR_TXT_TYPE,
              TXT_TYPE_X1, TXT_TYPE_Y1,
              TXT_TYPE_X2, TXT_TYPE_Y2,
              ST_DRAW, szSensorType, pxLightGreenTxtScheme );
   
    // Create text for sensor firmware revision.
    StCreate( GID_SENSOR_TXT_FIRMWAREREVISION,
              TXT_TYPE_X2 - 40, TXT_TYPE_Y1,
              TXT_TYPE_X2, TXT_TYPE_Y2,
              ST_DRAW, prv_szFirmwareRevision, pxDefaultScheme );
    
    // Create text for sensor value.
    StCreate( GID_SENSOR_TXT_VALUE1,
              PG1SLD_TXT_VALUE_X1, PG1SLD_TXT_VALUE_Y1,
              PG1SLD_TXT_VALUE_X2, PG1SLD_TXT_VALUE_Y2,
              ST_DRAW, prv_szValue1, pxDefaultScheme );

    // Create slider for sensor value.
    SldCreate( GID_SENSOR_SLD_VALUE1,
               PG1SLD_SLD_VALUE_X1, PG1SLD_SLD_VALUE_Y1,
               PG1SLD_SLD_VALUE_X2, PG1SLD_SLD_VALUE_Y2,
               SLD_DRAW | SLD_DISABLED, usSliderRange, 1, 0, pxSliderScheme );
}



/*******************************************************************************
 * FUNCTION: prv_vCreateHeadModuleSubpage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the sensor subpage for head module.
 *
 *******************************************************************************/
static void prv_vCreateHeadModuleSubpage(void)
{
    // Create text for sensor type.
    StCreate( GID_SENSOR_TXT_TYPE,
              TXT_TYPE_X1, TXT_TYPE_Y1,
              TXT_TYPE_X2, TXT_TYPE_Y2,
              ST_DRAW, "HEAD MODULE", pxLightGreenTxtScheme );

    // Create text for sensor firmware revision.
    StCreate( GID_SENSOR_TXT_FIRMWAREREVISION,
              TXT_TYPE_X2 - 40, TXT_TYPE_Y1,
              TXT_TYPE_X2, TXT_TYPE_Y2,
              ST_DRAW, prv_szFirmwareRevision, pxDefaultScheme );
    
    // Create text for sensor value 1.
    StCreate( GID_SENSOR_TXT_VALUE1,
              PGHM_TXT_VALUE1_X1, PGHM_TXT_VALUE1_Y1,
              PGHM_TXT_VALUE1_X2, PGHM_TXT_VALUE1_Y2,
              ST_DRAW, prv_szValue1, pxDefaultScheme );

    // Create slider for sensor value 1.
    SldCreate( GID_SENSOR_SLD_VALUE1,
               PGHM_SLD_VALUE1_X1, PGHM_SLD_VALUE1_Y1,
               PGHM_SLD_VALUE1_X2, PGHM_SLD_VALUE1_Y2,
               SLD_DRAW | SLD_DISABLED, 255, 1, 0, pxSliderScheme );

    
    // Create text for sensor value 2.
    StCreate( GID_SENSOR_TXT_VALUE2,
              PGHM_TXT_VALUE2_X1, PGHM_TXT_VALUE2_Y1,
              PGHM_TXT_VALUE2_X2, PGHM_TXT_VALUE2_Y2,
              ST_DRAW, prv_szValue2, pxDefaultScheme );

    // Create slider for sensor value 2.
    SldCreate( GID_SENSOR_SLD_VALUE2,
               PGHM_SLD_VALUE2_X1, PGHM_SLD_VALUE2_Y1,
               PGHM_SLD_VALUE2_X2, PGHM_SLD_VALUE2_Y2,
               SLD_DRAW | SLD_DISABLED, 255, 1, 0, pxSliderScheme );

    
    // Create title for RGB LED control.
    StCreate( GID_SENSOR_TXT_LED,
              PGHM_TXT_LED_X1, PGHM_TXT_LED_Y1,
              PGHM_TXT_LED_X2, PGHM_TXT_LED_Y2,
              ST_DRAW, "LED Colour:", pxLightBlueTxtScheme );

    // Create check box for red LED.
    CbCreate( GID_SENSOR_CB_R,
              PGHM_CB_RED_X1, PGHM_CB_RED_Y1,
              PGHM_CB_RED_X2, PGHM_CB_RED_Y2,
              CB_CHECKED | CB_DRAW, "Red", pxCheckboxScheme );

    // Create check box for green LED.
    CbCreate( GID_SENSOR_CB_G,
              PGHM_CB_GREEN_X1, PGHM_CB_GREEN_Y1,
              PGHM_CB_GREEN_X2, PGHM_CB_GREEN_Y2,
              CB_CHECKED | CB_DRAW, "Green", pxCheckboxScheme );


    // Create check box for blue LED.
    CbCreate( GID_SENSOR_CB_B,
              PGHM_CB_BLUE_X1, PGHM_CB_BLUE_Y1,
              PGHM_CB_BLUE_X2, PGHM_CB_BLUE_Y2,
              CB_CHECKED | CB_DRAW, "Blue", pxCheckboxScheme );
}



/*******************************************************************************
 * FUNCTION: prv_vCreateColourSensorSubpage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the sensor subpage for colour sensor.
 *
 *******************************************************************************/
static void prv_vCreateColourSensorSubpage(void)
{
    // Create text for sensor type.
    StCreate( GID_SENSOR_TXT_TYPE,
              TXT_TYPE_X1, TXT_TYPE_Y1,
              TXT_TYPE_X2, TXT_TYPE_Y2,
              ST_DRAW, "COLOUR SENSOR", pxLightGreenTxtScheme );
    
    // Create text for sensor firmware revision.
    StCreate( GID_SENSOR_TXT_FIRMWAREREVISION,
              TXT_TYPE_X2 - 40, TXT_TYPE_Y1,
              TXT_TYPE_X2, TXT_TYPE_Y2,
              ST_DRAW, prv_szFirmwareRevision, pxDefaultScheme );
    
    // Create text and slider for sensor value 1.
    StCreate( GID_SENSOR_TXT_VALUE1,
              PGCL_TXT_VALUE1_X1, PGCL_TXT_VALUE1_Y1,
              PGCL_TXT_VALUE1_X2, PGCL_TXT_VALUE1_Y2,
              ST_DRAW, prv_szValue1, pxDefaultScheme );
    
    SldCreate( GID_SENSOR_SLD_VALUE1,
               PGCL_SLD_VALUE1_X1, PGCL_SLD_VALUE1_Y1,
               PGCL_SLD_VALUE1_X2, PGCL_SLD_VALUE1_Y2,
               SLD_DRAW | SLD_DISABLED, 50, 1, 0, pxSliderScheme );
    
    
    // Create text and slider for sensor value 2.
    StCreate( GID_SENSOR_TXT_VALUE2,
              PGCL_TXT_VALUE2_X1, PGCL_TXT_VALUE2_Y1,
              PGCL_TXT_VALUE2_X2, PGCL_TXT_VALUE2_Y2,
              ST_DRAW, prv_szValue2, pxDefaultScheme );
    
    SldCreate( GID_SENSOR_SLD_VALUE2,
               PGCL_SLD_VALUE2_X1, PGCL_SLD_VALUE2_Y1,
               PGCL_SLD_VALUE2_X2, PGCL_SLD_VALUE2_Y2,
               SLD_DRAW | SLD_DISABLED, 50, 1, 0, pxSliderScheme );
    
    
    // Create text and slider for sensor value 3.
    StCreate( GID_SENSOR_TXT_VALUE3,
              PGCL_TXT_VALUE3_X1, PGCL_TXT_VALUE3_Y1,
              PGCL_TXT_VALUE3_X2, PGCL_TXT_VALUE3_Y2,
              ST_DRAW, prv_szValue3, pxDefaultScheme );
    
    SldCreate( GID_SENSOR_SLD_VALUE3,
               PGCL_SLD_VALUE3_X1, PGCL_SLD_VALUE3_Y1,
               PGCL_SLD_VALUE3_X2, PGCL_SLD_VALUE3_Y2,
               SLD_DRAW | SLD_DISABLED, 50, 1, 0, pxSliderScheme );
    
    
    // Create text and slider for sensor value 4.
    StCreate( GID_SENSOR_TXT_VALUE4,
              PGCL_TXT_VALUE4_X1, PGCL_TXT_VALUE4_Y1,
              PGCL_TXT_VALUE4_X2, PGCL_TXT_VALUE4_Y2,
              ST_DRAW, prv_szValue4, pxDefaultScheme );
    
    SldCreate( GID_SENSOR_SLD_VALUE4,
               PGCL_SLD_VALUE4_X1, PGCL_SLD_VALUE4_Y1,
               PGCL_SLD_VALUE4_X2, PGCL_SLD_VALUE4_Y2,
               SLD_DRAW | SLD_DISABLED, 50, 1, 0, pxSliderScheme );
    
    
    // Create text and slider for sensor value 5.
    StCreate( GID_SENSOR_TXT_VALUE5,
              PGCL_TXT_VALUE5_X1, PGCL_TXT_VALUE5_Y1,
              PGCL_TXT_VALUE5_X2, PGCL_TXT_VALUE5_Y2,
              ST_DRAW, prv_szValue5, pxDefaultScheme );
    
    SldCreate( GID_SENSOR_SLD_VALUE5,
               PGCL_SLD_VALUE5_X1, PGCL_SLD_VALUE5_Y1,
               PGCL_SLD_VALUE5_X2, PGCL_SLD_VALUE5_Y2,
               SLD_DRAW | SLD_DISABLED, 150, 1, 0, pxSliderScheme );
    
    
    // Create text and colour box for detected colour.
    StCreate( GID_SENSOR_TXT_COLOUR1,
              PGCL_TXT_COLOUR1_X1, PGCL_TXT_COLOUR1_Y1,
              PGCL_TXT_COLOUR1_X2, PGCL_TXT_COLOUR1_Y2,
              ST_DRAW, "Colour:", pxDefaultScheme );
    
    StCreate( GID_SENSOR_TXT_COLOUR2,
              PGCL_TXT_COLOUR2_X1, PGCL_TXT_COLOUR2_Y1,
              PGCL_TXT_COLOUR2_X2, PGCL_TXT_COLOUR2_Y2,
              ST_DRAW | ST_CENTER_ALIGN | ST_FRAME, "NOT DETECTED", pxCustomScheme1 );
    
    // Reset the custom scheme.
    memcpy(pxCustomScheme1, pxDefaultScheme, sizeof(GOL_SCHEME));
}



/*******************************************************************************
 * FUNCTION: prv_vCreateLineSensorSubpage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the sensor subpage for line sensor.
 *
 *******************************************************************************/
static void prv_vCreateLineSensorSubpage(void)
{
    // Create text for sensor type.
    StCreate( GID_SENSOR_TXT_TYPE,
              TXT_TYPE_X1, TXT_TYPE_Y1,
              TXT_TYPE_X2, TXT_TYPE_Y2,
              ST_DRAW, "LINE SENSOR", pxLightGreenTxtScheme );
    
    // Create text for sensor firmware revision.
    StCreate( GID_SENSOR_TXT_FIRMWAREREVISION,
              TXT_TYPE_X2 - 40, TXT_TYPE_Y1,
              TXT_TYPE_X2, TXT_TYPE_Y2,
              ST_DRAW, prv_szFirmwareRevision, pxDefaultScheme );
    
    // Create objects for sensor value 1.
    StCreate( GID_SENSOR_TXT_VALUE1,
              PGLN_TXT_VALUE1_X1, PGLN_TXT_VALUE1_Y1,
              PGLN_TXT_VALUE1_X2, PGLN_TXT_VALUE1_Y2,
              ST_DRAW | ST_CENTER_ALIGN, prv_szValue1, pxDefaultScheme );
    
    SldCreate( GID_SENSOR_SLD_VALUE1,
               PGLN_SLD_VALUE1_X1, PGLN_SLD_VALUE1_Y1,
               PGLN_SLD_VALUE1_X2, PGLN_SLD_VALUE1_Y2,
               SLD_DRAW | SLD_VERTICAL | SLD_DISABLED, 60, 1, 0, pxSliderScheme );
    
    StCreate( GID_SENSOR_TXT_TITLE1,
              PGLN_TXT_TITLE1_X1, PGLN_TXT_TITLE1_Y1,
              PGLN_TXT_TITLE1_X2, PGLN_TXT_TITLE1_Y2,
              ST_DRAW | ST_CENTER_ALIGN, "S1", pxCustomScheme1 );
    
    
    
    // Create objects for sensor value 2.
    StCreate( GID_SENSOR_TXT_VALUE2,
              PGLN_TXT_VALUE2_X1, PGLN_TXT_VALUE2_Y1,
              PGLN_TXT_VALUE2_X2, PGLN_TXT_VALUE2_Y2,
              ST_DRAW | ST_CENTER_ALIGN, prv_szValue2, pxDefaultScheme );
    
    SldCreate( GID_SENSOR_SLD_VALUE2,
               PGLN_SLD_VALUE2_X1, PGLN_SLD_VALUE2_Y1,
               PGLN_SLD_VALUE2_X2, PGLN_SLD_VALUE2_Y2,
               SLD_DRAW | SLD_VERTICAL | SLD_DISABLED, 60, 1, 0, pxSliderScheme );
    
    StCreate( GID_SENSOR_TXT_TITLE2,
              PGLN_TXT_TITLE2_X1, PGLN_TXT_TITLE2_Y1,
              PGLN_TXT_TITLE2_X2, PGLN_TXT_TITLE2_Y2,
              ST_DRAW | ST_CENTER_ALIGN, "S2", pxCustomScheme2 );
    
    
    
    // Create objects for sensor value 3.
    StCreate( GID_SENSOR_TXT_VALUE3,
              PGLN_TXT_VALUE3_X1, PGLN_TXT_VALUE3_Y1,
              PGLN_TXT_VALUE3_X2, PGLN_TXT_VALUE3_Y2,
              ST_DRAW | ST_CENTER_ALIGN, prv_szValue3, pxDefaultScheme );
    
    SldCreate( GID_SENSOR_SLD_VALUE3,
               PGLN_SLD_VALUE3_X1, PGLN_SLD_VALUE3_Y1,
               PGLN_SLD_VALUE3_X2, PGLN_SLD_VALUE3_Y2,
               SLD_DRAW | SLD_VERTICAL | SLD_DISABLED, 60, 1, 0, pxSliderScheme );
    
    StCreate( GID_SENSOR_TXT_TITLE3,
              PGLN_TXT_TITLE3_X1, PGLN_TXT_TITLE3_Y1,
              PGLN_TXT_TITLE3_X2, PGLN_TXT_TITLE3_Y2,
              ST_DRAW | ST_CENTER_ALIGN, "S3", pxCustomScheme3 );
    
    
    
    // Create objects for sensor value 4.
    StCreate( GID_SENSOR_TXT_VALUE4,
              PGLN_TXT_VALUE4_X1, PGLN_TXT_VALUE4_Y1,
              PGLN_TXT_VALUE4_X2, PGLN_TXT_VALUE4_Y2,
              ST_DRAW | ST_CENTER_ALIGN, prv_szValue4, pxDefaultScheme );
    
    SldCreate( GID_SENSOR_SLD_VALUE4,
               PGLN_SLD_VALUE4_X1, PGLN_SLD_VALUE4_Y1,
               PGLN_SLD_VALUE4_X2, PGLN_SLD_VALUE4_Y2,
               SLD_DRAW | SLD_VERTICAL | SLD_DISABLED, 60, 1, 0, pxSliderScheme );
    
    StCreate( GID_SENSOR_TXT_TITLE4,
              PGLN_TXT_TITLE4_X1, PGLN_TXT_TITLE4_Y1,
              PGLN_TXT_TITLE4_X2, PGLN_TXT_TITLE4_Y2,
              ST_DRAW | ST_CENTER_ALIGN, "S4", pxCustomScheme4 );
    
    
    
    // Create the line colour title and radio button.
    StCreate( GID_SENSOR_TXT_LINE,
              PGLN_TXT_LINE_X1, PGLN_TXT_LINE_Y1,
              PGLN_TXT_LINE_X2, PGLN_TXT_LINE_Y2,
              ST_DRAW, "Line:", pxLightBlueTxtScheme );
    
    RADIOBUTTON *pxRbDark = RbCreate( GID_SENSOR_RB_DARK,
                                      PGLN_RB_DARK_X1, PGLN_RB_DARK_Y1,
                                      PGLN_RB_DARK_X2, PGLN_RB_DARK_Y2,
                                      RB_DRAW | RB_GROUP, " Dark", pxRadioBtnScheme );
    
    RADIOBUTTON *pxRbLight = RbCreate( GID_SENSOR_RB_LIGHT,
                                       PGLN_RB_LIGHT_X1, PGLN_RB_LIGHT_Y1,
                                       PGLN_RB_LIGHT_X2, PGLN_RB_LIGHT_Y2,
                                       RB_DRAW, " Light", pxRadioBtnScheme );
    
    // Check the radio button according to the sensor setting.
    unsigned char ucDarkLight;
    eLineSensorGetDarkLight(prv_ucSelectedSensorId, &ucDarkLight);
    if (ucDarkLight == 0) {
        SetState(pxRbDark, RB_CHECKED);
    } else {
        SetState(pxRbLight, RB_CHECKED);
    }
    
    
    
    // Create button to calibrate the sensor.
    BtnCreate( GID_SENSOR_BTN_CAL,
               PGLN_BTN_CAL_X1, PGLN_BTN_CAL_Y1,
               PGLN_BTN_CAL_X2, PGLN_BTN_CAL_Y2,
               BTN_RADIUS, BTN_DRAW, NULL, "Calibrate", pxBtnScheme );
    
    
    
    // Reset the custom scheme.
    memcpy(pxCustomScheme1, pxDefaultScheme, sizeof(GOL_SCHEME));
    memcpy(pxCustomScheme2, pxDefaultScheme, sizeof(GOL_SCHEME));
    memcpy(pxCustomScheme3, pxDefaultScheme, sizeof(GOL_SCHEME));
    memcpy(pxCustomScheme4, pxDefaultScheme, sizeof(GOL_SCHEME));
}



/*******************************************************************************
 * FUNCTION: prv_vCreateImuSubpage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the sensor subpage for IMU.
 *
 *******************************************************************************/
static void prv_vCreateImuSubpage(void)
{
    // Create text for sensor type.
    StCreate( GID_SENSOR_TXT_TYPE,
              TXT_TYPE_X1, TXT_TYPE_Y1,
              TXT_TYPE_X2, TXT_TYPE_Y2,
              ST_DRAW, "IMU", pxLightGreenTxtScheme );
    

    // Create text for sensor value 1.
    StCreate( GID_SENSOR_TXT_VALUE1,
              PGIMU_TXT_VALUE1_X1, PGIMU_TXT_VALUE1_Y1,
              PGIMU_TXT_VALUE1_X2, PGIMU_TXT_VALUE1_Y2,
              ST_DRAW, prv_szValue1, pxDefaultScheme );

    // Create slider for sensor value 1.
    SldCreate( GID_SENSOR_SLD_VALUE1,
               PGIMU_SLD_VALUE1_X1, PGIMU_SLD_VALUE1_Y1,
               PGIMU_SLD_VALUE1_X2, PGIMU_SLD_VALUE1_Y2,
               SLD_DRAW | SLD_DISABLED, 180, 1, 90, pxSliderScheme );


    // Create text for sensor value 2.
    StCreate( GID_SENSOR_TXT_VALUE2,
              PGIMU_TXT_VALUE2_X1, PGIMU_TXT_VALUE2_Y1,
              PGIMU_TXT_VALUE2_X2, PGIMU_TXT_VALUE2_Y2,
              ST_DRAW, prv_szValue2, pxDefaultScheme );

    // Create slider for sensor value 2.
    SldCreate( GID_SENSOR_SLD_VALUE2,
               PGIMU_SLD_VALUE2_X1, PGIMU_SLD_VALUE2_Y1,
               PGIMU_SLD_VALUE2_X2, PGIMU_SLD_VALUE2_Y2,
               SLD_DRAW | SLD_DISABLED, 180, 1, 90, pxSliderScheme );

    
    // Create text for sensor value 3.
    StCreate( GID_SENSOR_TXT_VALUE3,
              PGIMU_TXT_VALUE3_X1, PGIMU_TXT_VALUE3_Y1,
              PGIMU_TXT_VALUE3_X2, PGIMU_TXT_VALUE3_Y2,
              ST_DRAW, prv_szValue3, pxDefaultScheme );

    // Create slider for sensor value 3.
    SldCreate( GID_SENSOR_SLD_VALUE3,
               PGIMU_SLD_VALUE3_X1, PGIMU_SLD_VALUE3_Y1,
               PGIMU_SLD_VALUE3_X2, PGIMU_SLD_VALUE3_Y2,
               SLD_DRAW | SLD_DISABLED, 180, 1, 90, pxSliderScheme );


    // Create text for sensor value 4.
    StCreate( GID_SENSOR_TXT_VALUE4,
              PGIMU_TXT_VALUE4_X1, PGIMU_TXT_VALUE4_Y1,
              PGIMU_TXT_VALUE4_X2, PGIMU_TXT_VALUE4_Y2,
              ST_DRAW, prv_szValue4, pxDefaultScheme );

    // Create slider for sensor value 4.
    SldCreate( GID_SENSOR_SLD_VALUE4,
               PGIMU_SLD_VALUE4_X1, PGIMU_SLD_VALUE4_Y1,
               PGIMU_SLD_VALUE4_X2, PGIMU_SLD_VALUE4_Y2,
               SLD_DRAW | SLD_DISABLED, 400, 1, 200, pxSliderScheme );


    // Create text for sensor value 5.
    StCreate( GID_SENSOR_TXT_VALUE5,
              PGIMU_TXT_VALUE5_X1, PGIMU_TXT_VALUE5_Y1,
              PGIMU_TXT_VALUE5_X2, PGIMU_TXT_VALUE5_Y2,
              ST_DRAW, prv_szValue5, pxDefaultScheme );

    // Create slider for sensor value 5.
    SldCreate( GID_SENSOR_SLD_VALUE5,
               PGIMU_SLD_VALUE5_X1, PGIMU_SLD_VALUE5_Y1,
               PGIMU_SLD_VALUE5_X2, PGIMU_SLD_VALUE5_Y2,
               SLD_DRAW | SLD_DISABLED, 400, 1, 200, pxSliderScheme );


    // Create text for sensor value 6.
    StCreate( GID_SENSOR_TXT_VALUE6,
              PGIMU_TXT_VALUE6_X1, PGIMU_TXT_VALUE6_Y1,
              PGIMU_TXT_VALUE6_X2, PGIMU_TXT_VALUE6_Y2,
              ST_DRAW, prv_szValue6, pxDefaultScheme );

    // Create slider for sensor value 6.
    SldCreate( GID_SENSOR_SLD_VALUE6,
               PGIMU_SLD_VALUE6_X1, PGIMU_SLD_VALUE6_Y1,
               PGIMU_SLD_VALUE6_X2, PGIMU_SLD_VALUE6_Y2,
               SLD_DRAW | SLD_DISABLED, 400, 1, 200, pxSliderScheme );
}



/*******************************************************************************
 * FUNCTION: prv_vCreateRgbLightModuleSubpage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the module subpage for RGB Light Module.
 *
 *******************************************************************************/
static void prv_vCreateRgbLightModuleSubpage(void)
{
    // Create text for sensor type.
    StCreate( GID_SENSOR_TXT_TYPE,
              TXT_TYPE_X1, TXT_TYPE_Y1,
              TXT_TYPE_X2, TXT_TYPE_Y2,
              ST_DRAW, "RGB LIGHT MODULE", pxLightGreenTxtScheme );
    
    // Create text for sensor firmware revision.
    StCreate( GID_SENSOR_TXT_FIRMWAREREVISION,
              TXT_TYPE_X2 - 40, TXT_TYPE_Y1,
              TXT_TYPE_X2, TXT_TYPE_Y2,
              ST_DRAW, prv_szFirmwareRevision, pxDefaultScheme );
    
    // Create text and slider for sensor value 1.
    StCreate( GID_SENSOR_TXT_TITLE1,
              PGLM_TXT_TITLE1_X1, PGLM_TXT_TITLE1_Y1,
              PGLM_TXT_TITLE1_X2, PGLM_TXT_TITLE1_Y2,
              ST_DRAW, "Red", pxDefaultScheme );
    
    SldCreate( GID_RGBLIGHTMODULE_SLD_VALUE1,
               PGLM_SLD_VALUE1_X1, PGLM_SLD_VALUE1_Y1,
               PGLM_SLD_VALUE1_X2, PGLM_SLD_VALUE1_Y2,
               SLD_DRAW, 255, 1, 0, pxSliderScheme );
    
    StCreate( GID_RGBLIGHTMODULE_TXT_VALUE1,
              PGLM_TXT_VALUE1_X1, PGLM_TXT_VALUE1_Y1,
              PGLM_TXT_VALUE1_X2, PGLM_TXT_VALUE1_Y2,
              ST_DRAW, prv_szValue1, pxDefaultScheme );
    
    
    // Create text and slider for sensor value 2.
    StCreate( GID_SENSOR_TXT_TITLE2,
              PGLM_TXT_TITLE2_X1, PGLM_TXT_TITLE2_Y1,
              PGLM_TXT_TITLE2_X2, PGLM_TXT_TITLE2_Y2,
              ST_DRAW, "Green", pxDefaultScheme );
    
    SldCreate( GID_RGBLIGHTMODULE_SLD_VALUE2,
               PGLM_SLD_VALUE2_X1, PGLM_SLD_VALUE2_Y1,
               PGLM_SLD_VALUE2_X2, PGLM_SLD_VALUE2_Y2,
               SLD_DRAW, 255, 1, 0, pxSliderScheme );
    
    StCreate( GID_RGBLIGHTMODULE_TXT_VALUE2,
              PGLM_TXT_VALUE2_X1, PGLM_TXT_VALUE2_Y1,
              PGLM_TXT_VALUE2_X2, PGLM_TXT_VALUE2_Y2,
              ST_DRAW, prv_szValue2, pxDefaultScheme );
    
    
    // Create text and slider for sensor value 3.
    StCreate( GID_SENSOR_TXT_TITLE3,
              PGLM_TXT_TITLE3_X1, PGLM_TXT_TITLE3_Y1,
              PGLM_TXT_TITLE3_X2, PGLM_TXT_TITLE3_Y2,
              ST_DRAW, "Blue", pxDefaultScheme );
    
    SldCreate( GID_RGBLIGHTMODULE_SLD_VALUE3,
               PGLM_SLD_VALUE3_X1, PGLM_SLD_VALUE3_Y1,
               PGLM_SLD_VALUE3_X2, PGLM_SLD_VALUE3_Y2,
               SLD_DRAW, 255, 1, 0, pxSliderScheme );
    
    StCreate( GID_RGBLIGHTMODULE_TXT_VALUE3,
              PGLM_TXT_VALUE3_X1, PGLM_TXT_VALUE3_Y1,
              PGLM_TXT_VALUE3_X2, PGLM_TXT_VALUE3_Y2,
              ST_DRAW, prv_szValue3, pxDefaultScheme );
    
    
//    // Create text and slider for sensor value 4.
//    StCreate( GID_SENSOR_TXT_TITLE4,
//              PGLM_TXT_TITLE4_X1, PGLM_TXT_TITLE4_Y1,
//              PGLM_TXT_TITLE4_X2, PGLM_TXT_TITLE4_Y2,
//              ST_DRAW, "BRT.", pxDefaultScheme );
//    
//    SldCreate( GID_SENSOR_SLD_VALUE4,
//               PGLM_SLD_VALUE4_X1, PGLM_SLD_VALUE4_Y1,
//               PGLM_SLD_VALUE4_X2, PGLM_SLD_VALUE4_Y2,
//               SLD_DRAW, 255, 1, 255, pxSliderScheme );
//    
//    StCreate( GID_SENSOR_TXT_VALUE4,
//              PGLM_TXT_VALUE4_X1, PGLM_TXT_VALUE4_Y1,
//              PGLM_TXT_VALUE4_X2, PGLM_TXT_VALUE4_Y2,
//              ST_DRAW, prv_szValue4, pxDefaultScheme );
    
    
    // Reset the custom scheme.
    memcpy(pxCustomScheme1, pxDefaultScheme, sizeof(GOL_SCHEME));
}



/*******************************************************************************
 * FUNCTION: vCreateSensorPage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Create the sensor page.
 *
 *******************************************************************************/
void vCreateSensorPage(void)
{
    GOLFree();
    vCreatePageWindow("SENSOR & HEAD MODULE");
    vCreateListboxExtModule("Sensor:");
    
    // Search for connected sensor.
    static LB_FILTER eListboxFilter = LB_FILTER_SENSOR;
    xTaskCreate(taskSeekExtModule, "SeekModule", configMINIMAL_STACK_SIZE, &eListboxFilter, tskIDLE_PRIORITY, NULL);
}



/*******************************************************************************
 * FUNCTION: usMsgSensorPage
 *
 * PARAMETERS:
 * ~ objMsg     - Translated message for the object.
 * ~ pObj       - Pointer to the object.
 * ~ pMsg       - Pointer to the non-translated, raw GOL message.
 *
 * RETURN:
 * ~ If the function returns non-zero, the message will be processed by default.
 *
 * DESCRIPTIONS:
 * Handle the touchscreen event of the sensor page.
 *
 *******************************************************************************/
WORD usMsgSensorPage(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    LISTBOX *pxListbox = (LISTBOX*)GOLFindObject(GID_EXTMODULE_LB);

    // Process the list box event.
    usMsgListboxExtModule(objMsg, pObj, pMsg);


    switch(GetObjID(pObj)) {

        // Refresh button.
        case GID_EXTMODULE_BTN_REFRESH:
            if (objMsg == BTN_MSG_RELEASED) {
                // Deselect the sensor.
                prv_eSelectedSensorType = EM_MODEL_NONE;
                prv_ucSelectedSensorId = 0xff;

                // Reload the subpage again.
                vClearWindowSubpage();
                vCreateListboxExtModule("Sensor:");
                
                // Search for connected sensor.
                static LB_FILTER eListboxFilter = LB_FILTER_SENSOR;
                xTaskCreate(taskSeekExtModule, "SeekModule", configMINIMAL_STACK_SIZE, &eListboxFilter, tskIDLE_PRIORITY, NULL);
            }
            break;

        // List box.
        case GID_EXTMODULE_LB:
            if (pMsg->uiEvent == EVENT_RELEASE) {

                // Turn off LED for previous selection.
                if (prv_ucSelectedSensorId <= EM_MAX_ID) {
                    switch (prv_eSelectedSensorType) {
                        case EM_MODEL_IR:
                        case EM_MODEL_US:
                            eSensorSetLed(prv_ucSelectedSensorId, prv_eSelectedSensorType, 0);
                            break;
                        case EM_MODEL_HEAD:
                            eHeadSetLed(prv_ucSelectedSensorId, 0, 0b11);
                            break;
                        case EM_MODEL_RGBLIGHT:
                        case EM_MODEL_TACTILE:
                        case EM_MODEL_COLOUR:
                        case EM_MODEL_LINE:
                            eSensorSetLed(prv_ucSelectedSensorId, prv_eSelectedSensorType, 0);
                            vTaskDelay(10 / portTICK_RATE_MS);
                            eSensorSetLed(prv_ucSelectedSensorId, prv_eSelectedSensorType, 2);
                            break;
                    }
                }

                // Get the selected item in the list box.
                LISTITEM* pxLbItem = LbGetSel((LISTBOX *)pObj, NULL);

                // If an item is selected, created the subpage.
                if(pxLbItem != NULL) {
                    // Get the ID.
                    prv_ucSelectedSensorId = pxLbItem->data;

                    // Make sure external module is selected.
                    if (prv_ucSelectedSensorId <= EM_MAX_ID) {
                        // Read the sensor model.
                        EM_MODEL eModel;
                        eEMGetModel(prv_ucSelectedSensorId, &eModel);
                        // Save the selected sensor type.
                        prv_eSelectedSensorType = eModel;
                        
                        // Get Firmware version
                        unsigned char ucFirmwareRevision = 0xff;
                        eEMGetFirmwareRevision(prv_ucSelectedSensorId, &ucFirmwareRevision);
                        // Update the text with selected sensor firmware revision.
                        snprintf(prv_szFirmwareRevision, sizeof(prv_szFirmwareRevision), "rev:%02X", ucFirmwareRevision);
                        
                        // Turn on LED for selected sensor.
                        switch (prv_eSelectedSensorType) {
                            case EM_MODEL_IR:
                            case EM_MODEL_US:
                                eSensorSetLed(prv_ucSelectedSensorId, prv_eSelectedSensorType, 1);
                                break;
                            case EM_MODEL_HEAD:
                                eHeadSetLed(prv_ucSelectedSensorId, 0xff, 0b11);
                                break;
                            case EM_MODEL_RGBLIGHT:
                                // read the current RGB values and update to UI page
                                eRgbLightModuleGetRgb(prv_ucSelectedSensorId, &ucRgbLmRed, &ucRgbLmGreen, &ucRgbLmBlue);
                                eRgbLightModuleSetRgb(prv_ucSelectedSensorId, ucRgbLmRed, ucRgbLmGreen, ucRgbLmBlue);
                                prv_ucLighModuleFirstSelected = 1;
                            case EM_MODEL_TACTILE:
                            case EM_MODEL_COLOUR:
                            case EM_MODEL_LINE:
                                eSensorSetLed(prv_ucSelectedSensorId, prv_eSelectedSensorType, 1);
                                vTaskDelay(10 / portTICK_RATE_MS);
                                eSensorSetLed(prv_ucSelectedSensorId, prv_eSelectedSensorType, 2);
                                break;
                        }
                    }

                    // If the IMU is selected.
                    if (strcmp(pxLbItem->pText, "IMU") == 0) {
                        prv_eSelectedSensorType = EM_MODEL_IMU;
                    }

                    
                    // Create the sub page.
                    prv_vCreateSensorSubpage();
                }
            }
            break;



        // Red check box (Head Module).
        case GID_SENSOR_CB_R:
            if (objMsg == CB_MSG_CHECKED) {
                eHeadSetRed(prv_ucSelectedSensorId, 0b111);
            }
            else if (objMsg == CB_MSG_UNCHECKED) {
                eHeadSetRed(prv_ucSelectedSensorId, 0);
            }
            break;

        // Green check box (Head Module).
        case GID_SENSOR_CB_G:
            if (objMsg == CB_MSG_CHECKED) {
                eHeadSetGreen(prv_ucSelectedSensorId, 0b111);
            }
            else if (objMsg == CB_MSG_UNCHECKED) {
                eHeadSetGreen(prv_ucSelectedSensorId, 0);
            }
            break;

        // Blue check box (Head Module).
        case GID_SENSOR_CB_B:
            if (objMsg == CB_MSG_CHECKED) {
                eHeadSetBlue(prv_ucSelectedSensorId, 0b11);
            }
            else if (objMsg == CB_MSG_UNCHECKED) {
                eHeadSetBlue(prv_ucSelectedSensorId, 0);
            }
            break;
            
            
            
        // Red slider (RGB Light Module).
        case GID_RGBLIGHTMODULE_SLD_VALUE1:
                ucRgbLmRed = SldGetPos(pObj);
                eRgbLightModuleSetRgb(prv_ucSelectedSensorId, ucRgbLmRed, ucRgbLmGreen, ucRgbLmBlue);
            break;

        // Green slider (RGB Light Module).
        case GID_RGBLIGHTMODULE_SLD_VALUE2:
                ucRgbLmGreen = SldGetPos(pObj);
                eRgbLightModuleSetRgb(prv_ucSelectedSensorId, ucRgbLmRed, ucRgbLmGreen, ucRgbLmBlue);
            break;

        // Blue slider (RGB Light Module).
        case GID_RGBLIGHTMODULE_SLD_VALUE3:
                ucRgbLmBlue = SldGetPos(pObj);
                eRgbLightModuleSetRgb(prv_ucSelectedSensorId, ucRgbLmRed, ucRgbLmGreen, ucRgbLmBlue);
            break;
            
//        // Brightness slider (RGB Light Module).
//        case GID_SENSOR_SLD_VALUE4:
//                ucRgbLmBrightness = SldGetPos(pObj);
//                eRgbLightModuleSetBrightness(prv_ucSelectedSensorId, ucRgbLmBrightness);
//            break;
            
    
            
        // Dark radio button.
        case GID_SENSOR_RB_DARK:
            if (objMsg == RB_MSG_CHECKED) {
                eLineSensorSetDarkLight(prv_ucSelectedSensorId, 0);
            }
            break;
            
        // Light radio button.
        case GID_SENSOR_RB_LIGHT:
            if (objMsg == RB_MSG_CHECKED) {
                eLineSensorSetDarkLight(prv_ucSelectedSensorId, 1);
            }
            break;
            
            
            
        // Calibrate button.
        case GID_SENSOR_BTN_CAL:
            if (objMsg == BTN_MSG_RELEASED) {
                // Make sure the selected sensor is line sensor.
                if (prv_eSelectedSensorType == EM_MODEL_LINE) {
                    // Disable the listbox and its objects.
                    vDisableListboxExtModule();
                    
                    // Disable the radio button.
                    vGraphicsObjectDisable((void*)GOLFindObject(GID_SENSOR_RB_DARK));
                    vGraphicsObjectDisable((void*)GOLFindObject(GID_SENSOR_RB_LIGHT));
                    
                    // Remove the calibrate button and create the done button.
                    GOLDeleteObjectByID(GID_SENSOR_BTN_CAL);
                    BtnCreate( GID_SENSOR_BTN_DONE,
                               PGLN_BTN_CAL_X1, PGLN_BTN_CAL_Y1,
                               PGLN_BTN_CAL_X2, PGLN_BTN_CAL_Y2,
                               BTN_RADIUS, BTN_DRAW, NULL, "Done", pxBtnScheme );
                    
                    
                    // Start the auto calibration.
                    eLineSensorSetAutoCal(prv_ucSelectedSensorId, 1);
                }
            }
            break;
            
        // Done button.
        case GID_SENSOR_BTN_DONE:
            if (objMsg == BTN_MSG_RELEASED) {
                // Make sure the selected sensor is line sensor.
                if (prv_eSelectedSensorType == EM_MODEL_LINE) {
                    // Enable the listbox and its objects.
                    vEnableListboxExtModule();
                    
                    // Enable the radio button.
                    vGraphicsObjectEnable((void*)GOLFindObject(GID_SENSOR_RB_DARK));
                    vGraphicsObjectEnable((void*)GOLFindObject(GID_SENSOR_RB_LIGHT));
                    
                    // Remove the done button and create the calibrate button.
                    GOLDeleteObjectByID(GID_SENSOR_BTN_DONE);
                    BtnCreate( GID_SENSOR_BTN_CAL,
                               PGLN_BTN_CAL_X1, PGLN_BTN_CAL_Y1,
                               PGLN_BTN_CAL_X2, PGLN_BTN_CAL_Y2,
                               BTN_RADIUS, BTN_DRAW, NULL, "Calibrate", pxBtnScheme );
                    
                    
                    // Stop the auto calibration.
                    eLineSensorSetAutoCal(prv_ucSelectedSensorId, 0);
                }
            }
            break;



        // Exit button.
        case GID_BTN_EXIT:
            if (objMsg == BTN_MSG_RELEASED) {
                // Turn off LED for previous selection.
                // Do not turn off Head LED and RGB Light Module's LED.
                if (prv_ucSelectedSensorId <= EM_MAX_ID) {
                    switch (prv_eSelectedSensorType) {
                        case EM_MODEL_IR:
                        case EM_MODEL_US:
                            eSensorSetLed(prv_ucSelectedSensorId, prv_eSelectedSensorType, 0);
                            break;
                        case EM_MODEL_TACTILE:
                        case EM_MODEL_COLOUR:
                        case EM_MODEL_LINE:
                        case EM_MODEL_RGBLIGHT:
                            eSensorSetLed(prv_ucSelectedSensorId, prv_eSelectedSensorType, 0);
                            vTaskDelay(10 / portTICK_RATE_MS);
                            eSensorSetLed(prv_ucSelectedSensorId, prv_eSelectedSensorType, 2);
                            break;
                    }
                }
                
                // Wait for the searching to complete.
                xSemaphoreTake(xSeekExtModuleSemaphore, portMAX_DELAY);
                
                // Clear the list box.
                LbDelItemsList(pxListbox);

                // Deselect the sensor.
                prv_eSelectedSensorType = EM_MODEL_NONE;
                prv_ucSelectedSensorId = 0xff;
                prv_ucLighModuleFirstSelected = 0;
                
                // Go back to main menu.
                vSetGuiPage(PAGE_MAIN_MENU);
            }
            break;
    }

    return 1;
}



/*******************************************************************************
 * FUNCTION: vUpdateSensorPage
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * This function should be called periodically to update the sensor reading.
 *
 *******************************************************************************/
void vUpdateSensorPage(void)
{
    switch (prv_eSelectedSensorType) {
        case EM_MODEL_IR: {
            // Read the sensor value and make sure there is no error.
            unsigned char ucIrValue;
            if (eInfraredSensorGetValue(prv_ucSelectedSensorId, &ucIrValue) == EM_NO_ERROR) {
                // Update the slider.
                vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SENSOR_SLD_VALUE1), (short)ucIrValue);

                // Update the text.
                snprintf(prv_szValue1, sizeof(prv_szValue1), "Value: %03u", ucIrValue);
                SetState(GOLFindObject(GID_SENSOR_TXT_VALUE1), ST_UPDATE);
            }
            break;
        }


        case EM_MODEL_US: {
            // Read the sensor value and make sure there is no error.
            unsigned short usUsValue;
            if (eUltrasonicSensorGetValue(prv_ucSelectedSensorId, &usUsValue) == EM_NO_ERROR) {
                // Scale the sensor value and limit it's maximum.
                usUsValue /= 10;
                if (usUsValue > 255) {
                    usUsValue = 255;
                }

                // Update the slider.
                vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SENSOR_SLD_VALUE1), usUsValue);

                // Update the text.
                snprintf(prv_szValue1, sizeof(prv_szValue1), "Value: %03ucm", usUsValue);
                SetState(GOLFindObject(GID_SENSOR_TXT_VALUE1), ST_UPDATE);
            }
            break;
        }


        case EM_MODEL_HEAD: {
            // Read the mic value and make sure there is no error.
            unsigned char ucHeadMicValue;
            if (eHeadGetMicValue(prv_ucSelectedSensorId, &ucHeadMicValue) == EM_NO_ERROR) {
                // Update the slider.
                vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SENSOR_SLD_VALUE1), (short)ucHeadMicValue);

                // Update the text.
                snprintf(prv_szValue1, sizeof(prv_szValue1), "MIC: %03u", ucHeadMicValue);
                SetState(GOLFindObject(GID_SENSOR_TXT_VALUE1), ST_UPDATE);
            }

            // Read the IR value and make sure there is no error.
            unsigned char ucHeadIrValue;
            if (eHeadGetIrValue(prv_ucSelectedSensorId, &ucHeadIrValue) == EM_NO_ERROR) {
                // Update the slider.
                vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SENSOR_SLD_VALUE2), (short)ucHeadIrValue);

                // Update the text.
                snprintf(prv_szValue2, sizeof(prv_szValue2), "IR: %03u", ucHeadIrValue);
                SetState(GOLFindObject(GID_SENSOR_TXT_VALUE2), ST_UPDATE);
            }
            break;
        }
            
            
        case EM_MODEL_TACTILE: {
            // Read the sensor value and make sure there is no error.
            unsigned char ucTactileValue;
            if (eTactileSensorGetValue(prv_ucSelectedSensorId, &ucTactileValue) == EM_NO_ERROR) {
                // Update the slider.
                vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SENSOR_SLD_VALUE1), (short)ucTactileValue);

                // Update the text.
                if (ucTactileValue == 0) {
                    snprintf(prv_szValue1, sizeof(prv_szValue1), "State: Released");
                } else {
                    snprintf(prv_szValue1, sizeof(prv_szValue1), "State: Pressed");
                }
                SetState(GOLFindObject(GID_SENSOR_TXT_VALUE1), ST_UPDATE);
            }
            break;
        }
            
            
        case EM_MODEL_COLOUR: {
            // Read the RGB value and make sure there is no error.
            unsigned char ucRed;
            unsigned char ucGreen;
            unsigned char ucBlue;
            if (eColourSensorGetRgb(prv_ucSelectedSensorId, &ucRed, &ucGreen, &ucBlue) == EM_NO_ERROR) {
                // Update the slider and text for red colour.
                vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SENSOR_SLD_VALUE1), ucRed);
                snprintf(prv_szValue1, sizeof(prv_szValue1), "Red: %02u", ucRed);
                SetState(GOLFindObject(GID_SENSOR_TXT_VALUE1), ST_UPDATE);
                
                // Update the slider and text for green colour.
                vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SENSOR_SLD_VALUE2), ucGreen);
                snprintf(prv_szValue2, sizeof(prv_szValue2), "Green: %02u", ucGreen);
                SetState(GOLFindObject(GID_SENSOR_TXT_VALUE2), ST_UPDATE);
                
                // Update the slider and text for blue colour.
                vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SENSOR_SLD_VALUE3), ucBlue);
                snprintf(prv_szValue3, sizeof(prv_szValue3), "Blue: %02u", ucBlue);
                SetState(GOLFindObject(GID_SENSOR_TXT_VALUE3), ST_UPDATE);
            }
            
            
            // Read the brightness value and make sure there is no error.
            unsigned char ucBrightness;
            if (eColourSensorGetBrightness(prv_ucSelectedSensorId, &ucBrightness) == EM_NO_ERROR) {
                // Update the slider and text for brightness.
                vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SENSOR_SLD_VALUE4), ucBrightness);
                snprintf(prv_szValue4, sizeof(prv_szValue4), "Brightness: %02u", ucBrightness);
                SetState(GOLFindObject(GID_SENSOR_TXT_VALUE4), ST_UPDATE);
            }
            
            
            // Read the ambient light value and make sure there is no error.
            unsigned char ucAmbient;
            if (eColourSensorGetAmbient(prv_ucSelectedSensorId, &ucAmbient) == EM_NO_ERROR) {
                // Update the slider and text for ambient light.
                vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SENSOR_SLD_VALUE5), ucAmbient - 90);
                snprintf(prv_szValue5, sizeof(prv_szValue5), "Ambient: %02u", ucAmbient);
                SetState(GOLFindObject(GID_SENSOR_TXT_VALUE5), ST_UPDATE);
            }
            
            
            // Read the colour and make sure there is no error.
            CL_SENSOR_COLOUR eSensorColour;
            if (eColourSensorGetColour(prv_ucSelectedSensorId, &eSensorColour) == EM_NO_ERROR) {
                // Update the custom scheme and text for colour box.
                GFX_COLOR xFillColour;
                GFX_COLOR xTextColour;
                char* szColour;
                switch (eSensorColour) {
                    case CL_RED:
                        xFillColour = BRIGHTRED;
                        xTextColour = pxDefaultScheme->CommonBkColor;
                        szColour = "RED";
                        break;
                        
                    case CL_GREEN:
                        xFillColour = BRIGHTGREEN;
                        xTextColour = pxDefaultScheme->CommonBkColor;
                        szColour = "GREEN";
                        break;
                        
                    case CL_BLUE:
                        xFillColour = BRIGHTBLUE;
                        xTextColour = pxDefaultScheme->CommonBkColor;
                        szColour = "BLUE";
                        break;
                        
                    case CL_YELLOW:
                        xFillColour = BRIGHTYELLOW;
                        xTextColour = pxDefaultScheme->CommonBkColor;
                        szColour = "YELLOW";
                        break;
                        
                    case CL_MAGENTA:
                        xFillColour = BRIGHTMAGENTA;
                        xTextColour = pxDefaultScheme->CommonBkColor;
                        szColour = "MAGENTA";
                        break;
                        
                    case CL_CYAN:
                        xFillColour = BRIGHTCYAN;
                        xTextColour = pxDefaultScheme->CommonBkColor;
                        szColour = "CYAN";
                        break;
                        
                    case CL_WHITE:
                        xFillColour = WHITE;
                        xTextColour = pxDefaultScheme->CommonBkColor;
                        szColour = "WHITE";
                        break;
                    
                    default:
                        xFillColour = pxDefaultScheme->CommonBkColor;
                        xTextColour = pxDefaultScheme->TextColor0;
                        szColour = "N/A";
                        break;
                }
                pxCustomScheme1->CommonBkColor = xFillColour;
                pxCustomScheme1->TextColor0 = xTextColour;
                SetState(GOLFindObject(GID_SENSOR_TXT_COLOUR2), ST_DRAW);

                vStaticTextUpdateText((STATICTEXT*)GOLFindObject(GID_SENSOR_TXT_COLOUR2), szColour);
            }
            break;
        }
            
            
        case EM_MODEL_LINE: {
            // Read the sensor value and 
            unsigned char ucSensor1;
            unsigned char ucSensor2;
            unsigned char ucSensor3;
            unsigned char ucSensor4;
            unsigned char ucDigital;
            EM_ERROR eErrorCode = eLineSensorGetAnalog(prv_ucSelectedSensorId, &ucSensor1, &ucSensor2, &ucSensor3, &ucSensor4);
            eErrorCode |= eLineSensorGetDigital(prv_ucSelectedSensorId, &ucDigital);
            
            // Make sure there is no error.
            if (eErrorCode == EM_NO_ERROR) {
                // Update the objects for sensor 1.
                vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SENSOR_SLD_VALUE1), ucSensor1);
                snprintf(prv_szValue1, sizeof(prv_szValue1), "%02u", ucSensor1);
                SetState(GOLFindObject(GID_SENSOR_TXT_VALUE1), ST_UPDATE);
                
                if ((ucDigital & 0x01) != 0) {
                    pxCustomScheme1->TextColor0 = ORANGE;
                } else {
                    pxCustomScheme1->TextColor0 = pxDefaultScheme->TextColor0;
                }
                SetState(GOLFindObject(GID_SENSOR_TXT_TITLE1), ST_DRAW);
                ucDigital >>= 1;
                
                
                // Update the objects for sensor 2.
                vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SENSOR_SLD_VALUE2), ucSensor2);
                snprintf(prv_szValue2, sizeof(prv_szValue2), "%02u", ucSensor2);
                SetState(GOLFindObject(GID_SENSOR_TXT_VALUE2), ST_UPDATE);
                
                if ((ucDigital & 0x01) != 0) {
                    pxCustomScheme2->TextColor0 = ORANGE;
                } else {
                    pxCustomScheme2->TextColor0 = pxDefaultScheme->TextColor0;
                }
                SetState(GOLFindObject(GID_SENSOR_TXT_TITLE2), ST_DRAW);
                ucDigital >>= 1;
                
                
                // Update the objects for sensor 3.
                vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SENSOR_SLD_VALUE3), ucSensor3);
                snprintf(prv_szValue3, sizeof(prv_szValue3), "%02u", ucSensor3);
                SetState(GOLFindObject(GID_SENSOR_TXT_VALUE3), ST_UPDATE);
                
                if ((ucDigital & 0x01) != 0) {
                    pxCustomScheme3->TextColor0 = ORANGE;
                } else {
                    pxCustomScheme3->TextColor0 = pxDefaultScheme->TextColor0;
                }
                SetState(GOLFindObject(GID_SENSOR_TXT_TITLE3), ST_DRAW);
                ucDigital >>= 1;
                
                
                // Update the objects for sensor 4.
                vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SENSOR_SLD_VALUE4), ucSensor4);
                snprintf(prv_szValue4, sizeof(prv_szValue4), "%02u", ucSensor4);
                SetState(GOLFindObject(GID_SENSOR_TXT_VALUE4), ST_UPDATE);
                
                if ((ucDigital & 0x01) != 0) {
                    pxCustomScheme4->TextColor0 = ORANGE;
                } else {
                    pxCustomScheme4->TextColor0 = pxDefaultScheme->TextColor0;
                }
                SetState(GOLFindObject(GID_SENSOR_TXT_TITLE4), ST_DRAW);
                ucDigital >>= 1;
            }
            break;
        }


        case EM_MODEL_IMU: {
            // Update tilt angle for X-Axis.
            // Update the text.
            snprintf(prv_szValue1, sizeof(prv_szValue1), "AX: %+04d", (short)fAccTiltAngleX);
            SetState(GOLFindObject(GID_SENSOR_TXT_VALUE1), ST_UPDATE);

            // Update the slider.
            vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SENSOR_SLD_VALUE1), (short)fAccTiltAngleX + 90);



            // Update tilt angle for Y-Axis.
            // Update the text.
            snprintf(prv_szValue2, sizeof(prv_szValue2), "AY: %+04d", (short)fAccTiltAngleY);
            SetState(GOLFindObject(GID_SENSOR_TXT_VALUE2), ST_UPDATE);

            // Update the slider.
            vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SENSOR_SLD_VALUE2), (short)fAccTiltAngleY + 90);



            // Update tilt angle for Z-Axis.
            // Update the text.
            snprintf(prv_szValue3, sizeof(prv_szValue3), "AZ: %+04d", (short)fAccTiltAngleZ);
            SetState(GOLFindObject(GID_SENSOR_TXT_VALUE3), ST_UPDATE);

            // Update the slider.
            vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SENSOR_SLD_VALUE3), (short)fAccTiltAngleZ + 90);





            // Update gyro reading for Y-Axis.
            // Update the text.
            snprintf(prv_szValue4, sizeof(prv_szValue4), "GY: %+04d", (short)fGyroY);
            SetState(GOLFindObject(GID_SENSOR_TXT_VALUE4), ST_UPDATE);

            // Update the slider.
            vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SENSOR_SLD_VALUE4), (short)fGyroY + 200);



            // Update gyro reading for X-Axis.
            // Update the text.
            snprintf(prv_szValue5, sizeof(prv_szValue4), "GX: %+04d", (short)fGyroX);
            SetState(GOLFindObject(GID_SENSOR_TXT_VALUE5), ST_UPDATE);

            // Update the slider.
            vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SENSOR_SLD_VALUE5), (short)fGyroX + 200);



            // Update gyro reading for Z-Axis.
            // Update the text.
            snprintf(prv_szValue6, sizeof(prv_szValue4), "GZ: %+04d", (short)fGyroZ);
            SetState(GOLFindObject(GID_SENSOR_TXT_VALUE6), ST_UPDATE);

            // Update the slider.
            vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SENSOR_SLD_VALUE6), (short)fGyroZ + 200);


            break;
        }
        
        
        case EM_MODEL_RGBLIGHT: {
            // Update the RGB and Brightness values.
            if(prv_ucLighModuleFirstSelected == 1){
                vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_RGBLIGHTMODULE_SLD_VALUE1), (short)ucRgbLmRed);
            }
            snprintf(prv_szValue1, sizeof(prv_szValue1), "%03u", ucRgbLmRed);
            SetState(GOLFindObject(GID_RGBLIGHTMODULE_TXT_VALUE1), ST_UPDATE);
            
            if(prv_ucLighModuleFirstSelected == 1){
                vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_RGBLIGHTMODULE_SLD_VALUE2), (short)ucRgbLmGreen);
            }
            snprintf(prv_szValue2, sizeof(prv_szValue2), "%03u", ucRgbLmGreen);
            SetState(GOLFindObject(GID_RGBLIGHTMODULE_TXT_VALUE2), ST_UPDATE);
            
            if(prv_ucLighModuleFirstSelected == 1){
                vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_RGBLIGHTMODULE_SLD_VALUE3), (short)ucRgbLmBlue);
            }
            snprintf(prv_szValue3, sizeof(prv_szValue3), "%03u", ucRgbLmBlue);
            SetState(GOLFindObject(GID_RGBLIGHTMODULE_TXT_VALUE3), ST_UPDATE);
            
//            vSliderUpdatePosition((SLIDER*)GOLFindObject(GID_SENSOR_SLD_VALUE4), (short)ucRgbLmBrightness);
//            snprintf(prv_szValue4, sizeof(prv_szValue4), "%03u", ucRgbLmBrightness);
//            SetState(GOLFindObject(GID_SENSOR_TXT_VALUE4), ST_UPDATE);

            prv_ucLighModuleFirstSelected = 0;
            
            break;
        }
        

    }
}

