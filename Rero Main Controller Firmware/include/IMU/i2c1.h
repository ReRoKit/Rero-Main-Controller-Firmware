/*******************************************************************************
 * This file provides the functions for the I2C module.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef I2C_H
#define	I2C_H



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

void vI2C1Init(void);
unsigned char ucI2C1Write(unsigned char ucI2cAddress, unsigned char ucRegAddress, unsigned char ucBytesCount, unsigned char* pucBuffer);
unsigned char ucI2C1Read(unsigned char ucI2cAddress, unsigned char ucRegAddress, unsigned char ucBytesCount, unsigned char* pucBuffer);



#endif	/* I2C_H */
