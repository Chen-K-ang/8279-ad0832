#ifndef HARDWARE_U8279_H
#define HARDWARE_U8279_H

#include <reg52.h>
#include <ABSACC.h>

//CS P26   A0 P27   WE P36   RD P37
#define COM8279 XBYTE [0XBFFF] 
#define DAT8279 XBYTE [0X3FFF]

extern unsigned char keyval;
void U8279_init(void);

#endif
