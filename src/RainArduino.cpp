/* 
 Wind Lib for Arduino
 By: Victor Torre

 Based on: Weather Shield Example
 By: Nathan Seidle / SparkFun Electronics https://github.com/sparkfun/Weather_Shield/

 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 */
#include "RainArduino.h"

//Fix for Travis/Inotools
#ifndef INPUT_PULLUP
//Valor en https://github.com/arduino/Arduino/blob/master/hardware/arduino/cores/arduino/Arduino.h
#define INPUT_PULLUP 0x2
#endif

#ifndef RAIN_INTERRUPT
#define RAIN_INTERRUPT 2
#endif

RainArduino::RainArduino(){}
bool RainArduino::begin(void (*callback)(void)) {
	pinMode(RAIN_INTERRUPT, INPUT_PULLUP);
	attachInterrupt(0, callback, FALLING);
	return true;
}
void  RainArduino::rainIRQ() {
  raintime = millis(); // grab current time
  raininterval = raintime - rainlast; // calculate interval between this and last event
  if (raininterval > 10) { // ignore switch-bounce glitches less than 10mS after initial edge
    rainHour[minutes] += 0.011; //Increase this minute's amount of rain
    rainlast = raintime; // set up for next event
  }
}
float RainArduino::get_rain() {
  rainin = 0;
  for(int i = 0 ; i < 60 ; i++)
    rainin += rainHour[i];
  return rainin;
}
float RainArduino::get_rain_last() {
  int i=minutes-1;
  if(i<0) i=59;
  return rainHour[i];
}
void RainArduino::loop(){
  if(millis() - lastSecond >= 1000) {
    if(++seconds > 59) {
      seconds = 0;
      if(++minutes > 59) minutes = 0;
      rainHour[minutes] = 0; //Zero out this minute's rainfall amount
    }
  }
}

