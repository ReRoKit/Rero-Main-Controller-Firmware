/*******************************************************************************
 * This file provides the functions for the UART module.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef UART5_H
#define	UART5_H



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

void vUART5Init(void);
void vUART5SetBaudrate(unsigned long ulBaudrate);
void vUART5FlushRxBuffer(void);
unsigned int uiUART5GetTxSpace(void);
unsigned int uiUART5GetRxDataCount(void);
unsigned int uiUART5Read(unsigned char *pucBuffer, unsigned char uiLength);
unsigned int uiUART5Write(unsigned char *pucBuffer, unsigned char uiLength);
unsigned char ucUART5IsTransmitting(void);



#endif	/* UART5_H */
