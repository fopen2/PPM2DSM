// Reader.h

#ifndef _READER_h
#define _READER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Changer.h"

class Reader
{
private:
	static const char* timer_ovf;
	static const char* invalid_pulse;
	static const char* too_few_channels;
	Changer* c;

	byte current = 0;
public:
	Reader();
	void begin(Changer* changer,uint8_t inputpin);
	void overflowInterrupt();
	void captureInterrupt();

};

#endif

