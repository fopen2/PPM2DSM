PPM2DSM
=======

This is an Arduino project that allows adding Spektrum DSM transmission capability to a PPM-capable R/C radio.

It is a complete rewrite of the simple "PPM to DSM v1.07" sketch that can be found on various forums.

The motivation of this project is to fly bind-and-fly models from Horizon Hobby (Spektrum) without having to settle for the limited featureset and quality of my DX6i. Thanks to this project, i can utilize the high quality gimbals, enhanced mixers, timers and the ability to fly in Modes 3 and 4 of my Futaba T14SG.

Features
--------

- supports six channels
- rearranges channel order (pre-setup for futaba)
- controlled entirely through the R/C remote, no additional physical button needed
- diagnostic LED
- bind mode via stick gesture
- optionally, when using a seventh input channel:
  - ModelMatch
  - bind mode via switch on the R/C remote

Hardware
--------

Spektrum R/C transmitters, such as the DX6i, contain removable RF modules (X1TXN in my case). They receive instructions over a serial connection. For this project, the RF module is removed and connected to an arduino pro mini 3.3v 8mhz is connected to the RF module.

The new R/C transmitter (Futaba T14SG in my case) is setup in trainer mode as a slave, which causes it to output a PPM signal. This signal is connected to the arduino.

Finally, a LED is connected to the arduino. This allows troubleshooting.

Pin assignment
--------------

- TX: serial output
- Pin 8: PPM input
- Pin 6: diagnostic LED

Pin numbers can be changed in [Defines.h](src/Defines.h)


Building
--------

This program was developed using [PlatformIO](https://platformio.org/) IDE. The easiest way to build it is to download and install PlatformIO, load the project and hit compile! It was tested on PlatformIO 3.6.1rc7 in conjunction with Visual Studio Code 1.28.2. The project uses the framework `atmelavr` and it was tested using version 1.10.1.

Building with the Arduino IDE should also be possible, but this was not tested.


Resources
---------

http://www.demoboard.de/projekte/ppm2dsm/

https://www.rc-heli.de/board/showthread.php?t=189853

