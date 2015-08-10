/*******************************************************************************
 * Functions to handle the communication protocol with the rero controller.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef PROTOCOL_H
#define	PROTOCOL_H



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *******************************************************************************/

unsigned char ucProcessCommandPacket(const unsigned char* pucReceivedPacket, unsigned char* pucTransmitPacket);



#endif	/* PROTOCOL_H */
