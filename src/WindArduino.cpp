/* 
 Wind Lib for Arduino
 By: Victor Torre

 Based on: Weather Shield Example
 By: Nathan Seidle / SparkFun Electronics https://github.com/sparkfun/Weather_Shield/

 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 */
#include "WindArduino.h"

#ifndef WIND_INTERRUPT
#define WIND_INTERRUPT 3
#endif

//Fix for Travis/Inotools
#ifndef INPUT_PULLUP
//Valor en https://github.com/arduino/Arduino/blob/master/hardware/arduino/cores/arduino/Arduino.h
#define INPUT_PULLUP 0x2
#endif
//Endfix for Travis/Inotools */

WindArduino::WindArduino(){
	lastWindCheck = 0;
	lastWindIRQ = 0;
	windClicks = 0;
}
bool WindArduino::begin(void (*callback)(void)){
	pinMode(WIND_INTERRUPT, INPUT_PULLUP);
	attachInterrupt(1, callback, FALLING);
	return true;
}
// Activated by the magnet in the anemometer (2 ticks per rotation), attached to input D3
void  WindArduino::wspeedIRQ() {
  // Ignore switch-bounce glitches less than 10ms (142MPH max reading) after the reed switch closes
  if (millis() - lastWindIRQ > 10) {
    lastWindIRQ = millis(); //Grab the current time
    windClicks++; //There is 1.492MPH for each click per second.
  }
}

//Returns the instataneous wind speed
float WindArduino::get_wind_speed(){
  deltaTime = millis() - lastWindCheck; //750ms

  deltaTime /= 1000.0; //Covert to seconds

  windSpeed = (float)windClicks / deltaTime; //3 / 0.750s = 4

  windClicks = 0; //Reset and start watching for new wind
  lastWindCheck = millis();

  windSpeed *= 1.492; //4 * 1.492 = 5.968MPH

  return(windSpeed);
}

//Read the wind direction sensor, return heading in degrees
int WindArduino::get_wind_direction(int wild_dir_port) {
  unsigned int adc;

  adc = analogRead(wild_dir_port); // get the current reading from the sensor

  // The following table is ADC readings for the wind direction sensor output, sorted from low to high.
  // Each threshold is the midpoint between adjacent headings. The output is degrees for that ADC reading.
  // Note that these are not in compass degree order! See Weather Meters datasheet for more information.

  if (adc < 380) return (113);
  if (adc < 393) return (68);
  if (adc < 414) return (90);
  if (adc < 456) return (158);
  if (adc < 508) return (135);
  if (adc < 551) return (203);
  if (adc < 615) return (180);
  if (adc < 680) return (23);
  if (adc < 746) return (45);
  if (adc < 801) return (248);
  if (adc < 833) return (225);
  if (adc < 878) return (338);
  if (adc < 913) return (0);
  if (adc < 940) return (293);
  if (adc < 967) return (315);
  if (adc < 990) return (270);
  return (-1); // error, disconnected?
}

