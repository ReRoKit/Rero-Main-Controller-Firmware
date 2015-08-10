/*******************************************************************************
 * This file provides the functions for audio amplifier and digital
 * potentiometer.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef AUDIOAMP_H
#define	AUDIOAMP_H



/*******************************************************************************
 * PUBLIC DEFINITION
 *******************************************************************************/

typedef struct {
    const unsigned char *pucSoundStream;
    unsigned long ulStreamSize;
} SOUND;



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

void vAudioAmpInit(void);
void vSetVolume(unsigned char ucVolume);
void vPlaySoundStream(const SOUND *pxSound);
void vPlayWaveFile(const char *szWaveFilePath);
void vStopPlayingAudio(void);
unsigned char ucIsAudioPlaying(void);
void taskWaveDecoder(void *pvParameters );


#endif	/* AUDIOAMP_H */
