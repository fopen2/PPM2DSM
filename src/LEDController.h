// LEDController.h

#ifndef _LEDCONTROLLER_h
#define _LEDCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define BLINKPERIOD 5

class LEDController
{
private:
	 uint8_t pin;
	 bool bindon;
   bool error;
   int8_t ledswitches=0;
   unsigned long nextswitch=0;
 public:
	 LEDController();
   void refresh();
	 void clearErrors();
	 void setError();
	 void begin(uint8_t pin);
	 void modelChanged(byte newmodel);
	 void bind(bool on);
	 void timeroverflow();
};

#endif
