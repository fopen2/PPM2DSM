//
//
//

#include "LEDController.h"

LEDController::LEDController(){
}

LEDController* cont;

void LEDController::begin(uint8_t pin){
	this->pin = pin;
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
	bindon=0;
  error=0;
	cont=this;
  /*
  TCCR0A = (1<<WGM00);
  TCCR0B = (1<<CS00)|(1<<CS02);//1024 prescaler
  //TIMSK0=0;*/
}

void LEDController::refresh(){
  if(error){
    //TCCR0A = (1<<WGM00);
    digitalWrite(pin,HIGH);
  }else if(bindon){
    //TCCR0A = (1<<COM0A1)|(1<<WGM00);//phase correct pwm non-inverting mode. OCR0B disconnected.
    //OCR0A = 127;//led off
    digitalWrite(pin,millis()%2==0?HIGH:LOW);
  }else{
    //TCCR0A = (1<<WGM00);
    digitalWrite(pin,(ledswitches%2==0)?LOW:HIGH);
  }
  if(ledswitches>0 && millis()>nextswitch){
    ledswitches--;
    nextswitch=millis()+BLINKPERIOD;
  }
}

void LEDController::setError(){
	error=1;
  refresh();
}

void LEDController::clearErrors(){
	error=0;
  refresh();
}

void LEDController::bind(bool on){
  bindon=on;
  refresh();
}

void LEDController::modelChanged(byte newmodel){
	ledswitches=(newmodel+1)*2;
}
