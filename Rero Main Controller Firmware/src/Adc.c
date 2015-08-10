/*******************************************************************************
 * This file contains the driver for the ADC module.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include "HardwareProfile.h"



/*******************************************************************************
 * FUNCTION: vInitAdc
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Initialize the ADC module.
 *
 *******************************************************************************/
void vInitAdc(void)
{
    // Initialize all IO pins as digital pin.
    AD1PCFG = 0xFFFF;

    // Ensure the ADC is off before setting the configuration
    CloseADC10();
    

    // Define setup parameters for OpenADC10.
    // Turn module on | ouput in integer | trigger mode auto | enable autosample
    #define PARAM1  ADC_MODULE_ON | ADC_FORMAT_INTG | ADC_CLK_AUTO | ADC_AUTO_SAMPLING_ON

    // ADC ref external    | disable offset test    | disable scan mode | perform 2 samples | use dual buffers | use alternate mode
    #define PARAM2  ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE | ADC_SCAN_OFF | ADC_SAMPLES_PER_INT_2 | ADC_ALT_BUF_ON | ADC_ALT_INPUT_ON

    // Use ADC internal clock | set sample time
    #define PARAM3  ADC_CONV_CLK_INTERNAL_RC | ADC_SAMPLE_TIME_15

    // Do not assign channels to scan
    #define PARAM4  SKIP_SCAN_ALL

    // Set AN2 and AN3 as analog inputs
    #define PARAM5  ENABLE_AN2_ANA | ENABLE_AN3_ANA


    // Use ground as neg ref for A | use AN2 for input A |  use ground as neg ref for A | use AN3 for input B
    SetChanADC10( ADC_CH0_NEG_SAMPLEA_NVREF | ADC_CH0_POS_SAMPLEA_AN2 |  ADC_CH0_NEG_SAMPLEB_NVREF | ADC_CH0_POS_SAMPLEB_AN3);

    // Configure ADC using parameter defined above.
    OpenADC10( PARAM1, PARAM2, PARAM3, PARAM4, PARAM5 );



    // Wait for the first conversion to complete so there will be vaild data in ADC result registers.
    while (mAD1GetIntFlag() == 0);
}
