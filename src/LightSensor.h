/* 
 LightSensor Lib for Arduino ALS-PT19
 By: Victor Torre

 Based on: Weather Shield Example
 By: Nathan Seidle / SparkFun Electronics https://github.com/sparkfun/Weather_Shield/

 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 */

#ifndef __LIGHT_ARDUINO_H__
#define __LIGHT_ARDUINO_H__

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class LightSensor {
	public:
		LightSensor();
		bool begin(int light_pin);
		float get_level(int reference_pin, float value);
		float get_level3v3(int reference_pin3v3);
		float get_level5v(int reference_pin5v);
	private:
		int pin;
};


bool LightSensor::begin(int light_pin) {
	pinMode(light_pin, INPUT_PULLUP);
	pin=light_pin;
	return true;
}
float LightSensor::get_level(int reference_pin, float value) {
  float operatingVoltage = analogRead(reference_pin);
  float lightSensor = analogRead(pin);
  operatingVoltage = value / operatingVoltage;
  lightSensor = operatingVoltage * lightSensor;
  return lightSensor;
}
float LightSensor::get_level3v3(int reference_pin3v3) {
	return LightSensor::get_level(reference_pin, 3.3);
}
float LightSensor::get_level5v(int reference_pin5v) {
	return LightSensor::get_level(reference_pin, 5);
}

#endif