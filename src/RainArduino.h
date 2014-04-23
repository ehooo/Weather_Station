/* 
 Wind Lib for Arduino
 By: Victor Torre

 Based on: Weather Shield Example
 By: Nathan Seidle / SparkFun Electronics https://github.com/sparkfun/Weather_Shield/

 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 */

#ifndef __RAIN_ARDUINO_H__
#define __RAIN_ARDUINO_H__

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class RainArduino {
	public:
		RainArduino();
		//The port are used for manage the interruption made by the sensor use D2 for interrupt
		bool begin(void (*callback)(void) );
		// Count rain gauge bucket tips as they occur
		// Activated by the magnet and reed switch in the rain gauge, attached to input D2
		void rainIRQ();
		//Total rainfall for the day is calculated within the interrupt
		//Calculate amount of rainfall for the last 60 minutes
		float get_rain();
		float get_rain_last();
		void loop();

	private:
		long lastSecond;
		float rainin;
		byte seconds; //When it hits 60, increase the current minute
		byte minutes; //Keeps track of where we are in various arrays of data
		volatile float rainHour[60]; //60 floating numbers to keep track of 60 minutes of rain
		// volatiles are subject to modification by IRQs
		volatile unsigned long raintime, rainlast, raininterval, rain;
};

#endif

