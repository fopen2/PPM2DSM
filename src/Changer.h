// Changer.h

#ifndef _CHANGER_h
#define _CHANGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Defines.h"


class Changer
{
private:

	uint16_t channeldata[CHANNELNUMBER];
	static const byte mapping[CHANNELMAPPINGSIZE];

	const char* frameFailedVar = NULL;
	bool frameCompleteVar = false;

 public:
	 Changer();
	 void writeChannel(uint16_t data, byte channel);
	 uint16_t* readChannels();
	 void frameFailed(const char* const error);
	 void frameComplete();
	 const char* const isFrameFailed();
	 bool isFrameComplete();

};

#endif
