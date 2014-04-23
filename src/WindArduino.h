/* 
 Wind Lib for Arduino
 By: Victor Torre

 Based on: Weather Shield Example
 By: Nathan Seidle / SparkFun Electronics https://github.com/sparkfun/Weather_Shield/

 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 */

#ifndef __WIND_ARDUINO_H__
#define __WIND_ARDUINO_H__

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class WindArduino {
	public:
		WindArduino();
		//The port are used for manage the interruption made by the sensor use D3 for interrupt
		bool begin(int wild_speed_port, void (*callback)(void) );
		//Read the wind direction sensor, return heading in degrees
		int get_wind_direction(int wild_dir_port);
		//Returns the instataneous wind speed
		float get_wind_speed();
		void wspeedIRQ();

	private:
		long lastWindCheck;
		float windSpeed, deltaTime;
		volatile long lastWindIRQ;
		volatile byte windClicks;
};

#endif

