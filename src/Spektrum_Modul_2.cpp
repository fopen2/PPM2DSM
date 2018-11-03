#include "Defines.h"
#include "Reader.h"
#include "Changer.h"
#include "DebugTransmitter.h"
#include "ActualTransmitter.h"
#include "Transmitter.h"
#include "LEDController.h"
#include "Transmitter.h"
#include "Encoder.h"
#include "Reader.h"

// See Defines.h for global constants

#ifndef DEBUG
#define BAUD 125000
#else
#define BAUD 115200
#endif

LEDController l;
Changer c;
#ifndef DEBUG
ActualTransmitter t;
#else
DebugTransmitter t;
#endif
Encoder e = Encoder();
Reader r = Reader();


void setup()
{
	l.begin(LEDPIN);
#ifndef DEBUG
	t.begin(BAUD);
#else
	t.begin(BAUD, &c, &e);
#endif
	e.begin(&t, &c, &l);
	r.begin(&c, INPUTPIN);
}

void loop()
{
	e.handleSignals();
  l.refresh();
}
