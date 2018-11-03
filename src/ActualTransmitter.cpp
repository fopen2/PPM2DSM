// 
// 
// 

#include "ActualTransmitter.h"

ActualTransmitter::ActualTransmitter(){
}

void ActualTransmitter::begin(unsigned long baud){
	Serial.begin(baud);
}

void ActualTransmitter::write(byte* memory, unsigned int length){
	Serial.write(memory, length);
}

void ActualTransmitter::writeError(const char* const error){
}

void ActualTransmitter::beginFrame(){

}

void ActualTransmitter::endFrame(){

}