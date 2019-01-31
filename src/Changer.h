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
	bool frame_early_transmission = true;
	unsigned long frameready = 0;

 public:
	 Changer();
	 void writeChannel(uint16_t data, byte channel);
	 uint16_t* readChannels();
	 void frameFailed(const char* const error);
	 void frameComplete();
	 void frameStart();
	 const char* const isFrameFailed();
	 bool isFrameReadyToTX();
	 void frame_no_longer_early();
	 bool is_frame_tx_early();

};

#endif
