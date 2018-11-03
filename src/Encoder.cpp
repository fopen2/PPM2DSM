#include "Encoder.h"

#define CONTROLCHANNELS 1

	Encoder::Encoder(){

	}

	void Encoder::begin(Transmitter* transmitter, Changer* changer, LEDController* ledcontroller){
		t = transmitter;
		c = changer;
		l = ledcontroller;
	}

	void Encoder::handleSignals(){
		const char* const signalFrameFailed = c->isFrameFailed();
		if (signalFrameFailed != NULL){
			t->writeError(signalFrameFailed);
			l->setError();
		}
		else if (c->isFrameComplete()){
			l->clearErrors();
			encodeFrame();
		}
	}

  void Encoder::handleFirstFrame(){
    //detect if all controls are in corners. if so, we activate force bind mode
    uint16_t* channel = c->readChannels();
    forcebind = 255;
    for(int i=0;i<4;i++){
      if(channel[i]>1200 && channel[i]<1800){
        forcebind=0;
      }
    }
  }

  void Encoder::changeModel(int8_t model){
    selectedmodel=model;
    modelchangetarget=selectedmodel;
  }

	void Encoder::refreshSelectedModel(int16_t channel){
		/*
			Control channel:
			The negative value range (926 - 1513) is divided into 10 models.

			The positive value range is divided the same way. When the signal is in the positive range, bind mode is on.

      no bind                     1550                    bind
      9  8  7  6  5  4  3  2  1  0  1  2  3  4  5  6  7  8  9
      926 --------------------- 1513 -------------------- 2100
		*/
		//int16_t constrainedchannel = constrain(channel, 926, 2100) - 1513;//-587 bis 587

		int8_t model = map(abs(constrain(channel, 926, 2100) - 1513), 0, 587, -1, 10);//why 10?
    if(model==-1){
      model=0;
    }
    if(model==selectedmodel || (model!=modelchangetarget && modelchangetarget!=-1)){
      modelchangetarget=model;
      modelchangecounter=MODELCHANGEFRAMENUM;
    }else if(modelchangecounter>0){
      modelchangecounter--;
    }else if(modelchangecounter==0){
      changeModel(model);
      l->modelChanged(model);
    }else{//modelchangecounter==-1 -> this is the very first frame since power-on.
      changeModel(model);
      handleFirstFrame();
    }
	}

	void Encoder::encodeFrame(){
		uint16_t* channel = c->readChannels();
		refreshSelectedModel(channel[6]);
    if(forcebind>0){
      forcebind--;
      isBinding=1;
      l->bind(isBinding);
    }else if(isBinding != (channel[6]>1550)){
			isBinding^=1;
			l->bind(isBinding);
		}
    //DSM Header Byte 00
    t->beginFrame();
		byte header0 =8| 16 | (isBinding ? 128 : 0);//disbable france mode, activate DSMX (shouldnt do anything with my X1TXN C1) and maybe binding. Binding is on when the values are positive. To safely ignore the 0 1550 is chosen instead of 1513
		t->write(&header0, 1);

		//DSM Header Byte 01; currently just the modelnumber
		t->write((byte*)(&selectedmodel), 1);

		//channel data
		for (int i = 0; i < CHANNELNUMBER-CONTROLCHANNELS; i++){

			/*
			Prepare signal. we only have 10 bits for the signal. 1024 is 2^10 so you need 11 bits for that.
			Therefore, 1023 (0x3FF) is the maximum number we can display.
			Note that we do not have to use constrain here because the numbers are unsigned.

			Apart from that you should note that Futaba Transmitters give values between ~926 and 2100 uS.
			The center is 1513. That means Futaba Transmitters can give us 1174 different values.

			That means the output range is 87,2231686541737649 % of the input range.
			*/
			uint16_t b = map(constrain(channel[i], 926, 2100), 926, 2100, 0, 1023);

			byte high = (byte)(i << 2) | highByte(b);
			t->write(&high,1);
			byte low = lowByte(b);
			t->write(&low,1);
		}
		t->endFrame();
	}

	/*
	From http://makeithappend.org/de/archives/679
	DSM Header Byte 00:

	bit 7 (0�80 / 128) : 1 => Bind
	bit 6 (0�40 / 64) : 1 => Extendend Transmitter Range (US) � Attention! this needs more than 50mA (Arduino Micro), maybe more than 150mA (Arduino Pro Mini). For this you can use a small �Pololu 3.3V 300mA Step-Down� Voltage Regulator: D24V3F3)
	bit 5 (0�20 / 32) : 1 => Range Test
	bit 4 (0�10 / 16) : 1 => EU Freq, 0 => France Freq (France = no DSMX!) � this needs ~30mA (you can use the most Arduino Board for this EU/France Range Modes)
	bit 3 (0�08 / 8) : 1 => DSMX, 0 => DSM2
	bit 2 (0�04 / 4) : -
	bit 1 (0�02 / 2) : -
	bit 0 (0�01 / 1) : Type: 0 => Acro (Plane), 1 => Heli

	DSM Header Byte 01:

	0�00 = Mode 1 (Gas � right side)
	0�01 = Mode 2 (Gas � left side)
	.
	0�07 = Mode 8
	0�08 = Mode 9 / Heli (Header Byte 00: bit 0 => 1)
	0�09 = Mode 10 / Heli (Header Byte 00: bit 0 => 1)

	From http://www.rcgroups.com/forums/showpost.php?p=7925933&postcount=8

	Decided to play with this a little more this evening and everything fell into place.
	I tried a serial port rate of 125K instead of 115200. The data is now super easy to see and follow.

	The radio transmits 14 bytes per frame, 6 channels of information, 2 header bytes. Data rate is 125K 8,N,1

	Each frame begins with 00 00

	Each pair of bytes after that represent the channel and its value. Each channel has a valid range(1024 steps - 10 bit), the channel number is embedded in each pair of bytes.
	The radio transmits a 6th channel which is a copy of channel 1 with its endpoints limited. This appears to simply be a function of the radio when in airplane mode, channel 6 is still a full 10 bit channel.

	Here is a breakdown of the min and max values for each channel and the valid 10 bit range for each channel.

	ch1(left stick, up/down)
	bytes 3&4
	0056-03AA -measured min/max
	10 bit range - 000-3FF

	ch2(right stick, left/right)
	bytes 5&6
	0455-07A9 - measured min/max
	10 bit range - 400-7FF

	ch3(right stick, up/down)
	bytes 7&8
	0855-0BA9 - measured min/max
	10 bit range - 800-BFF

	ch4(left stick, right/left)
	bytes 9&10
	0C56-0FAA - measured min/max
	10 bit range - C00-FFF

	ch5(knob, upper right)
	bytes 11&12
	1056-13AA - measured min/max
	10 bit range - 1000-13FF

	ch6(copy of throttle channel) - travel limited
	bytes 13&14
	152A-162A - measured min/max
	10 bit range - 1400-17FF

	Channel breakdown:

	First two bits are 00
	Next four bits indicate channel #
	remaining 10 bits are serial values for the PPM data

	00 00 00 xx xx xx xx xx - ch1 0000-03FF

	00 00 01 xx xx xx xx xx - ch2 0400-07FF

	00 00 10 xx xx xx xx xx - ch3 0800-0BFF

	00 00 11 xx xx xx xx xx - ch4 0C00-0FFF

	00 01 00 xx xx xx xx xx - ch5 1000-13FF

	00 01 01 xx xx xx xx xx - ch6 1400-17FF

	If supported by DSM2 low power module:

	00 01 10 xx xx xx xx xx - ch7 1800-1BFF

	00 01 11 xx xx xx xx xx - ch8 1C00-1FFF

	So there it is, using the above data its now possible to build a 6 channel PPM to serial encoder to interface to the DSM2 module from the LP5DSM radio.


	Questions or comments let me know!

	From: http://www.rcgroups.com/forums/showthread.php?t=721024&page=32

	I used a free parallel port logic analyzer.

	http://www.codeproject.com/Articles/...Logic-Analyzer

	Only works if you have a parallel port of course

	First two header bytes are not the GUID.
	First one sets the operating mode, bind, range check, DSM2,DSMx etc.
	Details were posted here:
	http://www.rcgroups.com/forums/showp...&postcount=309

	Second byte is "model match" number

	With some LP modules the header bytes are ignored.

	Pat MacKenzie

	*/
