/*******************************************************************************
 * Wrapper functions for the FatFs library to replace the Microchip MDD library.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef FATFSWRAPPER_H
#define	FATFSWRAPPER_H

#include "FatFs/ff.h"



/*******************************************************************************
 * PUBLIC DEFINITION
 *******************************************************************************/
#define FSFILE          FIL

#define WRITE           "w"
#define READ            "r"
#define APPEND          "a"
#define WRITEPLUS       "w+"
#define READPLUS        "r+"
#define APPENDPLUS      "a+"

#define	SEEK_SET        0
#define	SEEK_CUR        1
#define	SEEK_END        2



/*******************************************************************************
 * PUBLIC DEFINITION
 *******************************************************************************/

unsigned long get_fattime(void);
void vSetClock(unsigned int uiYear, unsigned char ucMonth, unsigned char ucDay, unsigned char ucHour, unsigned char ucMinute, unsigned char ucSecond);
FSFILE *FSfopen(const char * fileName, const char *mode);
int FSfclose(FSFILE *fo);
size_t FSfread (void *ptr, size_t size, size_t n, FSFILE *stream);
size_t FSfwrite(const void *data_to_write, size_t size, size_t n, FSFILE *stream);
int FSfseek(FSFILE *stream, long offset, int whence);
int FSfeof( FSFILE * stream );
FRESULT xFSGetReadOnlyFlag(const char *szFileName, unsigned char *pucReadOnlyFlag);
FRESULT xFSSetReadOnlyFlag(const char *szFileName, unsigned char ucReadOnlyFlag);



#endif	/* FATFSWRAPPER_H */
