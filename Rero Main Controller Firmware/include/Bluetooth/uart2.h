/*******************************************************************************
 * This file provides the functions for the UART module.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef UART2_H
#define	UART2_H



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

void vUART2Init(void);
void vUART2SetBaudRate(unsigned long ulBaudRate);
void vUART2FlushRxBuffer(void);
volatile unsigned int uiUART2GetTxSpace(void);
volatile unsigned int uiUART2GetRxDataCount(void);
unsigned int uiUART2Read(unsigned char *pucBuffer, unsigned char uiLength);
unsigned int uiUART2Write(const unsigned char *pucBuffer, unsigned char uiLength);



#endif	/* UART2_H */
