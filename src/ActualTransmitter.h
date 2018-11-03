// ActualTransmitter.h

#ifndef _ACTUAL_TRANSMITTER_h
#define _ACTUAL_TRANSMITTER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif


#include "Transmitter.h"

class ActualTransmitter : public Transmitter
{
 public:
	 ActualTransmitter();
	 void write(byte* memory, unsigned int length);//length is the number of BYTES that are to be written. BYTES! not BITS!s
	 void writeError(const char* const error);
	 void begin(unsigned long baud);
	 void beginFrame();
	 void endFrame();
};

#endif

