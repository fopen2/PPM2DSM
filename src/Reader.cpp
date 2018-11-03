//
//
//

#include "Reader.h"

#if F_CPU == 16000000
#define TICKS_PER_uS      2               // number of timer ticks per 1 microsecond with prescaler = 8 and CPU 8MHz
#elif F_CPU == 8000000
#define TICKS_PER_uS      1               // number of timer ticks per 1 microsecond with prescaler = 8 and CPU 8MHz
#else
#error This version supports only 16 and 8MHz processors
#endif

#define MIN_IN_PULSE  ( 750 * TICKS_PER_uS) // valid pulse must be at least   750us
#define MAX_IN_PULSE  (2250 * TICKS_PER_uS) // valid pulse must be less than 2250us
#define SYNC_GAP_LEN  (5000 * TICKS_PER_uS) // we assume a space at least 5000us is sync

const char* Reader::timer_ovf = "Timer overflow";
const char* Reader::invalid_pulse = "Invalid pulse length";
const char* Reader::too_few_channels = "Frame carried too few channels";

Reader* interruptreader = NULL;

Reader::Reader(){
}

void Reader::begin(Changer* changer,uint8_t inputpin){
	c = changer;

	pinMode(inputpin, INPUT);
	interruptreader = this;

	TCCR1A = 0x00;                         // COM1A1=0, COM1A0=0 => Disconnect Pin OC1 from Timer/Counter 1
	//  PWM11=0,  PWM10=0 => PWM Operation disabled
	//  TCCR1B = (1<<ICES1) | (1<<CS11);        // capture using rising edge,  prescaler = 8
	TCCR1B = (1 << CS11);                     // capture using falling edge, prescaler = 8
	// 8MHz clock with prescaler 8 means TCNT1 increments every 1 uS
	TIMSK1 = _BV(ICIE1) | _BV(TOIE1);        // enable input capture and overflow interrupts for timer 1
}

void Reader::overflowInterrupt(){
	c->frameFailed(timer_ovf);
	current = CHANNELNUMBER+2;
}

void Reader::captureInterrupt(){
	// we want to measure the time to the end of the pulse

	/*Serial.write("CAPT: ");
	Serial.print(icr1 / TICKS_PER_uS);//debug
	Serial.write(" current=");
	Serial.print(current);
	*///Serial.write('\n');

	if (ICR1 >= SYNC_GAP_LEN){                  // is the space between pulses big enough to be the SYNC
		if (current < CHANNELNUMBER)
			c->frameFailed(too_few_channels);
		current = 0;
	}
	else{
		if (current < CHANNELNUMBER) {         // check if its a valid channel pulse and save it
			if ((ICR1 >= MIN_IN_PULSE) && (ICR1 <= MAX_IN_PULSE))  // check for valid channel data
				c->writeChannel((ICR1 / TICKS_PER_uS), current++);         // store pulse length as microseconds
			else{
				c->frameFailed(invalid_pulse);
				//wait for sync pulse
				current = CHANNELNUMBER + 2;
			}
      if (current==CHANNELNUMBER){
        c->frameComplete();
      }
		}
	}
}

//note that we don't need to check if interruptreader is null because these interrupts can only happen after begin() has been called.
ISR(TIMER1_OVF_vect) {
	interruptreader->overflowInterrupt();
}

ISR(TIMER1_CAPT_vect) {
	TCNT1 = 0;                  // reset the counter right away
	/*Serial.write("rawCAPT: ");
	Serial.print(ICR1 / TICKS_PER_uS);
	*///Serial.write('n');
	interruptreader->captureInterrupt();
}
