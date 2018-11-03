// Encoder.h

#ifndef _ENCODER_h
#define _ENCODER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Transmitter.h"
#include "LEDController.h"
#include "Changer.h"

#define MODELCHANGEFRAMENUM 20

class Encoder
{
private:
	Transmitter* t;
	Changer* c;
	LEDController* l;

	void encodeFrame();
	void refreshSelectedModel(int16_t channel);
  void changeModel(int8_t model);
  void handleFirstFrame();

  //model change only happens when the same model was sent MODELCHANGENUM frames in a row.
  int8_t modelchangecounter = -1;//counter from MODELCHANGENUM to 0. init value ensures select on first frame
  int8_t modelchangetarget = -1;

public:
  //public for debug only
   bool isBinding=0;
   int8_t selectedmodel=-2;
   uint8_t forcebind = 0;

   //public
	 Encoder();
	 void begin(Transmitter* transmitter, Changer* changer, LEDController* l);
	 void handleSignals();
};

#endif
