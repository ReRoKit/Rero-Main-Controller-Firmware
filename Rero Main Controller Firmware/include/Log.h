/*******************************************************************************
 * This file provides the logging function to the SD card.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef LOG_H
#define	LOG_H



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                   *
 *******************************************************************************/

void vLogStackWatermark(const char* szTaskName, unsigned short usWatermark);
void vLogStackOverflow(const char* szTaskName);
void vLogMallocError(const char* szSource);
void vLogGeneralException(unsigned char ucExcCode);



#endif	/* LOG_H */
