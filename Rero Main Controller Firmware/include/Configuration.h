/*******************************************************************************
 * This file handles reading/writing the configuration to the SD Card.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef CONFIGURATION_H
#define	CONFIGURATION_H



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

void vInitConfig(void);
unsigned short usReadConfig(const char *szConfigFilePath, void *pvBuffer, unsigned short usLength);
unsigned short usWriteConfig(const char *szConfigFilePath, void *pvBuffer, unsigned short usLength);



#endif	/* CONFIGURATION_H */
