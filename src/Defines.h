// Defines.h

#ifndef _DEFINES_h
#define _DEFINES_h

#undef DEBUG

#define CHANNELNUMBER 7
//LED is controlled with hardware PWM
//do NOT change the LEDPIN without checking LEDController.cpp and setting up the hardware PWM generation for the new pin
#define LEDPIN 6

//spektrum: thro,aile,elev,rudd,gear,aux1
//futaba:   aile,elev,thro,rudd,gear,aux1
//mapping from futaba to spektrum
#define CHANNELMAPPING 1,2,0,3,4,5,6,7
#define CHANNELMAPPINGSIZE 8
//define CHANNELMAPPING 0,1,2,3,4,5,6
#define INPUTPIN 8
//Timer 1 uses Pin 8 so a different pin won't work

#endif
