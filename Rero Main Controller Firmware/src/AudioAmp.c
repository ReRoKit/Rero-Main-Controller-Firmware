/*******************************************************************************
 * This file provides the functions for audio amplifier and digital
 * potentiometer.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#include <xc.h>
#include <string.h>
#include "AudioAmp.h"
#include "Compiler.h"
#include "Variables.h"
#include "HardwareProfile.h"
#include "TimeDelay.h"
#include "FatFs/FatFsWrapper.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



/*******************************************************************************
 * PRIVATE DEFINITION
 *******************************************************************************/

typedef enum __attribute__((packed)) {
    AUDIO_SOURCE_NONE,
    AUDIO_SOURCE_SOUNDSTREAM,
    AUDIO_SOURCE_WAVEFILE
} AUDIO_SOURCE;



#define DEFAULT_SAMPLE_RATE 8000
#define AUDIO_BUFFER_SIZE   512



/*******************************************************************************
 * PRIVATE GLOBAL VARIABLES                                                     *
 *******************************************************************************/

static volatile AUDIO_SOURCE eAudioSource;
static volatile unsigned char prv_ucIsPlaying = 0;
static volatile unsigned long prv_ulBitCount = 0;
static volatile unsigned long prv_ulStreamSize;

static volatile const unsigned char *prv_pucSoundStream;
static volatile unsigned char prv_ucBuffer[2][AUDIO_BUFFER_SIZE];
static volatile unsigned short prv_ucBufferPointer[2] = {0};
static volatile unsigned char prv_ucActiveBuffer = 0;



/*******************************************************************************
 * FUNCTION: vAudioAmpInit
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Initialize the audio amplifier and digital potentiometer.
 *
 *******************************************************************************/
void vAudioAmpInit(void)
{
    // Turn off audio amp.
    AUDIOAMP_SD = 1;

    // Deselect the digital pot.
    DIGIPOT_CS = 1;
    DIGIPOT_SCK = 0;
    DIGIPOT_SDO = 0;

    // Set ports direction.
    AUDIOAMP_SD_TRIS = 0;
    DIGIPOT_CS_TRIS = 0;
    DIGIPOT_SCK_TRIS = 0;
    DIGIPOT_DATA_TRIS = 0;


    // Initialize Timer 4.
    // Timer 4 is used for the audio sampling.
    // Do not enable the interrupt for now.
    // It will be enabled when start playing.
    unsigned short usPeriod = GetPeripheralClock() / DEFAULT_SAMPLE_RATE - 1;
    OpenTimer4(T4_ON | T4_SOURCE_INT | T4_PS_1_1, usPeriod);
    ConfigIntTimer4(T4_INT_OFF | T4_INT_PRIOR_7);


    // Initialize Timer 3.
    // Timer 3 is used for the audio PWM.
    // The frequency should be as high as possible.
    // However, the duty cycle for 100% should be 255.
    OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_1, 254);

    // Initialize the audio PWM.
    OpenOC4(OC_ON | OC_TIMER_MODE16 | OC_TIMER3_SRC | OC_PWM_FAULT_PIN_DISABLE, 128, 128);
}



/*******************************************************************************
 * FUNCTION: vPlaySoundStream
 *
 * PARAMETERS:
 * ~ pxSound    - Pointer to the sound stream that we want to play.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Play the sound stream in flash.
 *
 *******************************************************************************/
void vPlaySoundStream(const SOUND *pxSound)
{
    // Stop playing audio first.
    vStopPlayingAudio();
    
    // Reset the bit counter and indicate it's playing.
    prv_ulBitCount = 0;
    prv_ucIsPlaying = 1;

    // Update the sound stream pointer, stream size and the audio source.
    prv_pucSoundStream = pxSound->pucSoundStream;
    prv_ulStreamSize = pxSound->ulStreamSize;
    eAudioSource = AUDIO_SOURCE_SOUNDSTREAM;

    // Turn on audio amp.
    AUDIOAMP_SD = 0;

    // Enable Timer 4 interrupt.
    EnableIntT4;
}



/*******************************************************************************
 * FUNCTION: vPlayWaveFile
 *
 * PARAMETERS:
 * ~ szWaveFilePath    - Path to the wave file.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Play the wave file in SD card.
 *
 *******************************************************************************/
void vPlayWaveFile(const char *szWaveFilePath)
{
    // Stop playing audio first.
    vStopPlayingAudio();
    
    // Update the audio source.
    eAudioSource = AUDIO_SOURCE_WAVEFILE;
    
    // Create the wave decoder task.
    xTaskCreate(taskWaveDecoder, "Audio", 400, (void*)szWaveFilePath, tskIDLE_PRIORITY, NULL);
}



/*******************************************************************************
 * FUNCTION: vStopPlayingAudio
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Stop playing audio.
 * This function will block until the audio stop playing.
 *
 *******************************************************************************/
void vStopPlayingAudio(void)
{
    // Make sure the audio is playing.
    if (prv_ucIsPlaying == 1) {
        // Clear the stream size.
        prv_ulStreamSize = 0;

        // Give the load buffer semaphore to stop the wave decoder task if it's running.
        xSemaphoreGive(xLoadAudioBufferSemaphore);

        // Wait for playing complete.
        while (prv_ucIsPlaying == 1);
    }
}



/*******************************************************************************
 * FUNCTION: ucIsAudioPlaying
 *
 * PARAMETERS:
 * ~ void
 *
 * RETURN:
 * ~ 0 - Not playing.
 * ~ 1 - Playing.
 *
 * DESCRIPTIONS:
 * Check whether the sound is still playing.
 *
 *******************************************************************************/
unsigned char ucIsAudioPlaying(void)
{
    return prv_ucIsPlaying;
}



/*******************************************************************************
 * FUNCTION: vSetVolume
 *
 * PARAMETERS:
 * ~ ucVolume   - Volume (0 - 128).
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Set the volume via the digital potentiometer.
 * Command is sent by using bit bang method.
 *
 * Maximum frequency = 250kHz (4us).
 * Command to set the volume is as follow:
 *
 * AD3 AD2 AD1 AD0 C1 C0 CMDERR D8 D7 D6 D5 D4 D3 D2 D1 D0
 *  0   0   0   0   0  0  Input          Volume
 *
 *******************************************************************************/
void vSetVolume(unsigned char ucVolume)
{
    unsigned char ucCmdErr;

    // Limit the volume to within the range.
    if (ucVolume > 128) {
        ucVolume = 128;
    }

    // Select the digital pot.
    DIGIPOT_CS = 0;

    // Send the address (AD3:AD0) and command (C1:C0).
    unsigned char i;
    for (i = 0; i < 6; i++) {
        DIGIPOT_SDO = 0;
        Delay10us(1);
        DIGIPOT_SCK = 1;
        Delay10us(1);
        DIGIPOT_SCK = 0;
    }

    // Change to input and read the CMDERR bit.
    DIGIPOT_DATA_TRIS = 1;
    Delay10us(1);
    ucCmdErr = DIGIPOT_SDI;
    DIGIPOT_SCK = 1;
    Delay10us(1);
    DIGIPOT_SCK = 0;

    // If CMDERR bit is cleared, return straight away.
    if (ucCmdErr == 0) {
        // Deselect the digital pot.
        DIGIPOT_CS = 1;
        return;
    }

    // Change to output and send the remaining data.
    DIGIPOT_DATA_TRIS = 0;
    for (i = 0; i < 9; i++) {
        DIGIPOT_SDO = ucVolume >> (8 - i);
        Delay10us(1);
        DIGIPOT_SCK = 1;
        Delay10us(1);
        DIGIPOT_SCK = 0;
    }
    
    // Deselect the digital pot.
    DIGIPOT_CS = 1;
}



/*******************************************************************************
 * RTOS TASK: taskWaveDecoder
 *
 * PARAMETERS:
 * ~ pvParameters   - Pointer to the type of external module we are looking for.
 *                  - The data type is LB_FILTER.
 *
 * RETURN:
 * ~ void
 *
 * DESCRIPTIONS:
 * Task to decode the wave file and fill up the audio buffer.
 *
 *******************************************************************************/
void taskWaveDecoder(void *pvParameters )
{
    unsigned long ulReadLength = 0;
    unsigned char ucWriteBuffer = 0;
    
    
    // Open the wave file.
    xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
    FSFILE *pxWaveFile = FSfopen((char*)pvParameters, "r");
    xSemaphoreGive(xSdCardMutex);
    
    // Make sure the wave file is opened successfully.
    if (pxWaveFile != NULL) {
        xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
        
        // Jump to sub-chunk 1.
        FSfseek(pxWaveFile, 12, SEEK_SET);
        
        // Look for the data sub-chunk.
        unsigned long ulSubChunkSize = 0;
        unsigned char *pucSubChunkId[4];
        do {
            // Jump to the next sub-chunk.
            FSfseek(pxWaveFile, ulSubChunkSize, SEEK_CUR);
            
            // Read the ID and size.
            FSfread ((void*)&pucSubChunkId, 1, 4, pxWaveFile);
            FSfread ((void*)&ulSubChunkSize, 1, 4, pxWaveFile);
            
            // Kill the task if we have reached the end of file.
            if (FSfeof(pxWaveFile) != 0) {
                // Close the opened file.
                FSfclose(pxWaveFile);
                
                // Kill the task.
                vTaskDelete(NULL);
            }
        } while (memcmp("data", pucSubChunkId, 4) != 0);
        
        // Read the stream size.
        prv_ulStreamSize = ulSubChunkSize;
        
        
        
        xSemaphoreGive(xSdCardMutex);
        
        // Reset both buffer pointers.
        prv_ucBufferPointer[0] = AUDIO_BUFFER_SIZE;
        prv_ucBufferPointer[1] = AUDIO_BUFFER_SIZE;
        
        // Set the active buffer and writting buffer to 0;
        prv_ucActiveBuffer = 0;
        ucWriteBuffer = 0;
        
        // Reset the bit counter and indicate it's playing.
        prv_ulBitCount = 0;
        prv_ucIsPlaying = 1;
        
        // Turn on audio amp.
        AUDIOAMP_SD = 0;

        // Enable Timer 4 interrupt.
        EnableIntT4;
        
        // Clear the semaphore.
        xSemaphoreTake(xLoadAudioBufferSemaphore, 0);
        
        
        
        // Loop until all the data has been read.
        while (ulReadLength < prv_ulStreamSize) {
            xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
            ulReadLength += FSfread (&prv_ucBuffer[ucWriteBuffer], 1, AUDIO_BUFFER_SIZE, pxWaveFile);
            xSemaphoreGive(xSdCardMutex);
            
            prv_ucBufferPointer[ucWriteBuffer] = 0;
            
            // Toggle the writting buffer.
            ucWriteBuffer ^= 1;
            
            if (prv_ucBufferPointer[ucWriteBuffer] < AUDIO_BUFFER_SIZE) {
                // Wait until it's ready to reload the buffer.
                xSemaphoreTake(xLoadAudioBufferSemaphore, portMAX_DELAY);
            }
            
            
            
            // Read the stack watermark and record it if it's below threshold.
            unsigned portBASE_TYPE uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
            if (uxHighWaterMark < MIN_STACK_WATERMARK) {
                xSystemError.bStackLowError = 1;

                // Only log it when the watermark value changed.
                static portBASE_TYPE uxPreviousWatermark = 0;
                if (uxHighWaterMark != uxPreviousWatermark) {
                    vLogStackWatermark("Wave Decoder Task", (unsigned short)uxHighWaterMark);
                }
                uxPreviousWatermark = uxHighWaterMark;
            }
        }
        
        
        // Close the opened file.
        xSemaphoreTake(xSdCardMutex, portMAX_DELAY);
        FSfclose(pxWaveFile);
        xSemaphoreGive(xSdCardMutex);
    }
    
    vTaskDelete(NULL);
}



/*******************************************************************************
 * ISR: Timer 4 Interrupt.
 *
 * DESCRIPTIONS:
 * This ISR is run at the audio sampling rate.
 * It's used to play the audio.
 *
 *******************************************************************************/
void __ISR(_TIMER_4_VECTOR, IPL7AUTO) TMR4Interrupt(void)
{
    static unsigned long ulStartupCount = 0;
    static unsigned long ulShutdownCount = 0;

    // Clear the interrupt flag.
    mT4ClearIntFlag();

    
    
    // If the audio amp is off, do nothing for the first 166ms to let the audio amp power up.
    if (ulStartupCount < (DEFAULT_SAMPLE_RATE / 6)) {
        SetDCOC4PWM(128);
        ulStartupCount++;
        
        // Reset the shut down counter.
        ulShutdownCount = 0;
    }
    
    
    
    // Output the audio bit via PWM.
    else if (prv_ulBitCount < prv_ulStreamSize) {
        // Reset the shut down counter.
        ulShutdownCount = 0;
        
        
        // Audio source is sound stream in flash.
        if (eAudioSource == AUDIO_SOURCE_SOUNDSTREAM) {
            SetDCOC4PWM(prv_pucSoundStream[prv_ulBitCount]);
            prv_ulBitCount++;
        }
        
        // Audio source is wave file in SD card.
        else if (eAudioSource == AUDIO_SOURCE_WAVEFILE) {
            // Read from the active buffer if the buffer is ready.
            if (prv_ucBufferPointer[prv_ucActiveBuffer] < AUDIO_BUFFER_SIZE) {
                prv_ulBitCount++;
                SetDCOC4PWM(prv_ucBuffer[prv_ucActiveBuffer][prv_ucBufferPointer[prv_ucActiveBuffer]++]);

                // Switch buffer if we have read all the data from a buffer.
                if (prv_ucBufferPointer[prv_ucActiveBuffer] >= AUDIO_BUFFER_SIZE) {
                    prv_ucActiveBuffer ^= 1;
                    
                    // Trigger the task to reload the inactive buffer.
                    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
                    xSemaphoreGiveFromISR(xLoadAudioBufferSemaphore, &xHigherPriorityTaskWoken);
                }
            }
        }
        
    }   // End of "Output the audio bit via PWM".
    
    

    // On the audio amp for another 1s before turning off.
    else if (ulShutdownCount < DEFAULT_SAMPLE_RATE) {
        SetDCOC4PWM(128);
        ulShutdownCount++;
        
        // Clear the playing flag.
        prv_ucIsPlaying = 0;
    }

    // After finish playing the audio, turn off audio amp and disable the interrupt.
    else {
        // Disable Timer 4 interrupt.
        DisableIntT4;

        // Turn off audio amp and reset the power amp startup and shutdown counter.
        AUDIOAMP_SD = 1;
        ulStartupCount = 0;
        ulShutdownCount = 0;
        
        // Clear the playing flag.
        prv_ucIsPlaying = 0;
    }
}
