Weather_Station [![Build Status](https://travis-ci.org/ehooo/Weather_Station.svg?branch=master)](https://travis-ci.org/ehooo/Weather_Station)
===============
Weather Station hace uso de [Sparkfun Weather Shield](https://www.sparkfun.com/products/12081), [Arduino Ethernet Shield](http://arduino.cc/es/Main/ArduinoEthernetShield) y otros sensores extra.

Hay dos librerias [RainArduino](./src/RainArduino.h) y [WindArduino](./src/WindArduino.h) basadas en [Weather_Shield](https://github.com/sparkfun/Weather_Shield)

TODO
====
 + Configurar Red usando SD
 + Servir web desde la SD
 + Salvar logs en la SD

Dependencias
============
 + [MPL3115A2](https://github.com/sparkfun/MPL3115A2_Breakout/tree/master/library/MPL3115A2_Pressure)
 + [HTU21D](https://github.com/sparkfun/HTU21D_Breakout/tree/master/Library/HTU21D_Humidity)
 + [Webduino](https://github.com/sirleech/Webduino)
 + [DHT](http://www.github.com/markruys/arduino-DHT)
 + [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)
 + [BMP085](https://github.com/adafruit/Adafruit_BMP085_Unified)