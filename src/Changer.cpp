//
//
//

#include "Changer.h"

const byte Changer::mapping[CHANNELMAPPINGSIZE] = { CHANNELMAPPING };

Changer::Changer(){

}

void Changer::writeChannel(uint16_t data, byte channel){
	channeldata[mapping[channel]] = data;
}

uint16_t* Changer::readChannels(){
	return channeldata;//this is a bit unsafe. The caller could write to the array. It would be safer to copy it, or to make separate function calls for each channel. But that would be less efficient.
}

void Changer::frameFailed(const char* const error){
	frameFailedVar = error;
}

void Changer::frameStart(){
	frameready = micros()+TX_DELAY_MICROS;
	frameCompleteVar=true;
	frame_early_transmission=true;
}

void Changer::frame_no_longer_early(){
	frame_early_transmission=false;
}

bool Changer::is_frame_tx_early(){
	return frame_early_transmission;
}

void Changer::frameComplete(){
	
}

const char* const Changer::isFrameFailed(){
	if (frameFailedVar == NULL)
		return NULL;
	else{
		const char* const r = frameFailedVar;
		frameFailedVar = NULL;
		return r;
	}
}

bool Changer::isFrameReadyToTX(){
	if (frameCompleteVar && (frameready<micros())){
		frameCompleteVar = false;
		return true;
	}
	else return false;
}
