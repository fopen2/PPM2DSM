//
//
//

#include "DebugTransmitter.h"
#include <stdio.h>

DebugTransmitter::DebugTransmitter(){
}

void DebugTransmitter::begin(unsigned long baud,Changer* changer,Encoder* encoder){
	Serial.begin(baud);
	c = changer;
  e = encoder;
}

void DebugTransmitter::beginFrame(){
	Serial.write("BeginFrame\n");
	uint16_t* channel = c->readChannels();
	for (int i = 0; i < CHANNELNUMBER; i++){
		sprintf(buffer, "%4d ", channel[i]);
		Serial.write(buffer);
	}
  Serial.write("\nForcebind: ");
  sprintf(buffer, "%d", e->forcebind);
  Serial.write(buffer);
	Serial.write(" Bind: ");
  Serial.write(e->isBinding?'1':'0');
  Serial.write(" Model: ");
  sprintf(buffer, "%d \n", e->selectedmodel+1);
  Serial.write(buffer);
}

void DebugTransmitter::endFrame(){
	Serial.write("EndFrame\n");
	delay(1200);//makes stuff easily readable
}

void DebugTransmitter::printByte(byte* b){
	for (char i = 7; i>= 0; i--){//hier nicht sizeof(byte)-1, da es nicht darauf ankommt wie breit die addressen sind, sondern wieviele bits in einem byte sind
		Serial.write((((*b) >> i) % 2 == 0) ? '0' : '1');
	}
}

void DebugTransmitter::write(byte* memory, unsigned int length){
	for (unsigned int i = 0; i < length; i++){
		printByte(memory + i*sizeof(byte));//sizeof(byte) ist 1, da die addressbreite ja 8 ist.
		Serial.write((even) ? '\n' : ' ');
		even ^= 1;
	}
}

void DebugTransmitter::writeError(const char* const error){
	Serial.write("Error: ");
	Serial.write(error);
	Serial.write('\n');
}
