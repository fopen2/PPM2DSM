// Transmitter.h

#ifndef _TRANSMITTER_h
#define _TRANSMITTER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class Transmitter
{
 public:
	 virtual void writeError(const char* const error)=0;
	 virtual void beginFrame();
	 virtual void write(byte* memory, unsigned int length) = 0;//length is the number of BYTES that are to be written. BYTES! not BITS!s
	 virtual void endFrame();

	 //soo either you call writeError, if you dont have a valid frame, or you call beginFrame, then write as often as you need and then call endFrame.
};

#endif

