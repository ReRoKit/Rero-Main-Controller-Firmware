/*******************************************************************************
 * Functions for the G15 set path process.
 *
 * Company: Cytron Technologies Sdn Bhd
 * Website: http://www.cytron.com.my
 * Email:   support@cytron.com.my
 *******************************************************************************/

#ifndef SETPATH_H
#define	SETPATH_H



/*******************************************************************************
* PUBLIC FUNCTION PROTOTYPES                                                   *
*******************************************************************************/

void vSetPathStart(unsigned char ucId);
void vSetPathSave(void);
void vSetPathDiscard(void);
void taskSetPath (void *pvParameters);



#endif
