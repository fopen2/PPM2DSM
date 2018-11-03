// DebugTransmitter.h

#ifndef _DEBUGTRANSMITTER_h
#define _DEBUGTRANSMITTER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Transmitter.h"
#include "Changer.h"
#include "Encoder.h"

class DebugTransmitter : public Transmitter
{
public:
	DebugTransmitter();
	void begin(unsigned long baud, Changer* changer,Encoder* encoder);
	void write(byte* memory, unsigned int length);//length is the number of BYTES that are to be written. BYTES! not BITS!s
	void writeError(const char* const error);
	void beginFrame();
	void endFrame();
private:
	Changer* c=NULL;
  Encoder* e=NULL;
	char buffer[6];
	bool even = false;
	void printByte(byte* b);
};


#endif
