/* 
 Weather Shield Example
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 16th, 2013
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 Much of this is based on Mike Grusin's USB Weather Board code: https://www.sparkfun.com/products/10586
 
 This code reads all the various sensors (wind speed, direction, rain gauge, humidty, pressure, light, batt_lvl)
 and reports it over the serial comm port. This can be easily routed to an datalogger (such as OpenLog) or
 a wireless transmitter (such as Electric Imp).
 
 Measurements are reported once a second but windspeed and rain gauge are tied to interrupts that are
 calcualted at each report.
 
 This example code assumes the GPS module is not used.
 */

//Fix for Travis/Inotools
#ifndef INPUT_PULLUP
//Valor en https://github.com/arduino/Arduino/blob/master/hardware/arduino/cores/arduino/Arduino.h
#define INPUT_PULLUP 0x2
#endif
//Endfix for Travis/Inotools */

#include <SPI.h>
#include <Wire.h> //I2C needed for sensors
#include <Ethernet.h>
#include "MPL3115A2.h" //Pressure sensor
#include "HTU21D.h" //Humidity sensor
#include "DHT.h"
#include "WebServer.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BMP085_U.h"
#include "WindArduino.h"
#include "RainArduino.h"

MPL3115A2 mpl; //Create an instance of the pressure sensor
HTU21D htu; //Create an instance of the humidity sensor

static uint8_t mac[] = { 0x02, 0xAA, 0xBB, 0xCC, 0x00, 0x22 };
static uint8_t ip[] = { 192, 168, 0, 51 };

#define DHT_SENSOR_PIN 12
WebServer webserver("",80);
DHT dht; //Create an instance of external the humidity sensor

boolean bmp_on=true;
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085); //Create an instance of the pressure sensor

float dato; //Create auxiliar data
char buff[8]; //Create auxiliar data

#define WDIR_PIN A0
WindArduino wind; //Create an instance of wind sensor
void wspeedIRQ(){ wind.wspeedIRQ(); }
RainArduino rain; //Create an instance of rain sensor
void rainIRQ(){ rain.rainIRQ(); }

void defaultCmd(WebServer &server, WebServer::ConnectionType type, char *, bool){
  if (type == WebServer::POST){
    server.httpNoContent();
    return;
  }

  server.httpSuccess();
  Serial.println("HTML base");
  P(message) = 
  "<!DOCTYPE html><html><head>"
    "<title>Meteo Webduino</title>"
    //"<meta http-equiv='Content-Script-Type' content='text/javascript'>"
    "<script src='//code.jquery.com/jquery-2.0.3.js'></script>"
    "<link href='//netdna.bootstrapcdn.com/bootstrap/3.0.3/css/bootstrap.min.css' rel=stylesheet />"
    "<script src='//netdna.bootstrapcdn.com/bootstrap/3.0.3/js/bootstrap.min.js'></script>"
    "<style>  </style>"
    "<script>"
      "function update() {"
        "$.getJSON('/stats.json',function(data){"
          "$('#sensor_data').text(data);"
        "});"
      "};"
      "$(document).ready(function(){ "
        "update();"
        "$('refresh').click(update);"
      "});"
    "</script>"
  "</head>"
  "<body>"
    "<h2>Sensor data</h2>"
    "<textarea id='sensor_data'>"
    "</textarea>"
    "<button type='button' class='btn btn-primary' id='refresh'>Refrestar</button>"
  "</body>"
  "</html>";
  server.printP(message);
}

void statsCmd(WebServer &server, WebServer::ConnectionType type, char *, bool){
  Serial.println("BMP json");
  server.httpSuccess("application/json");
  server.write("{");
  if (type == WebServer::HEAD) return;
  if (rain) {
    server.write("\"rain\":{ \"total\":  ");
    dato = rain.get_rain();
    dtostrf(dato, 5, 2, buff);
    server.write(buff);
    server.write(", \"last\": ");
    dato = rain.get_rain_last();
    dtostrf(dato, 5, 2, buff);
    server.write(buff);
    server.write(" },");
  }
  if (wind) {
    server.write("\"wind\":{ \"direction\":  ");
    dato = wind.get_wind_direction(WDIR_PIN);
    dtostrf(dato, 5, 2, buff);
    server.write(buff);
    server.write(", \"speed\": ");
    dato = wind.get_wind_speed();
    dtostrf(dato, 5, 2, buff);
    server.write(buff);
    server.write(" },");
  }

  if(bmp_on){
    sensors_event_t event;
    bmp.getEvent(&event);
    if (event.pressure) {
      server.write("\"bmp\":{ \"pressure\":  ");
      dtostrf(event.pressure, 5, 2, buff);
      server.write(buff);
      server.write(", \"temperature\": ");
      bmp.getTemperature(&dato);
      dtostrf(dato, 5, 2, buff);
      server.write(buff);
      server.write(", \"altitude\": ");
      dato = bmp.pressureToAltitude(SENSORS_PRESSURE_SEALEVELHPA, event.pressure, dato);
      bmp.getTemperature(&dato);
      dtostrf(dato, 5, 2, buff);
      server.write(buff);
      server.write(" },");
    }
  }
  if (dht) {
    server.write("\"dht\":{ \"status\": \"");
    server.write(dht.getStatusString());
    server.write("\", \"humidity\": ");
    dato = dht.getHumidity();
    dtostrf(dato, 5, 2, buff);
    server.write(buff);
    server.write(", \"temperature\": ");
    dato = dht.getTemperature();
    dtostrf(dato, 5, 2, buff);
    server.write(buff);
    server.write("},");
  }
  if (mpl) {
      server.write("\"mpl\":{ \"pressure\":  ");
      dato = mpl.readPressure();
      dtostrf(dato, 5, 2, buff);
      server.write(buff);
      server.write(", \"temperature\": ");
      dato = mpl.readTemp();
      dtostrf(dato, 5, 2, buff);
      server.write(buff);
      server.write(", \"altitude\": ");
      dato = mpl.readAltitude();
      dtostrf(dato, 5, 2, buff);
      server.write(buff);
      server.write(" },");
  }
  if (htu) {
    server.write("\"htu\":{ \"humidity\": ");
    dato = htu.readHumidity();
    dtostrf(dato, 5, 2, buff);
    server.write(buff);
    server.write(", \"temperature\": ");
    dato = htu.readTemperature();
    dtostrf(dato, 5, 2, buff);
    server.write(buff);
    server.write("},");
  }
  server.write(" }");
}


void setup() {
  Serial.begin(9600);

  Serial.println("Init DHT!!");
  dht.setup(DHT_SENSOR_PIN);

  Serial.println("Init BMP085!!");
  if(!bmp.begin()){
    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    bmp_on = false;
  }

  Serial.println("Init Network!!");
  if( Ethernet.begin(mac) == 0){
    Serial.println("DHCP Error!!");
    Ethernet.begin(mac, ip);
  }
  Serial.print("Server at ");
  Serial.println(Ethernet.localIP());

  Serial.println("Init Wind Sensor!!");
  wind.begin(wspeedIRQ);
  Serial.println("Init Rain Sensor!!");
  rain.begin(rainIRQ);
  interrupts();

  //Configure the pressure sensor
  Serial.println("Init MPL3115A2!!");
  mpl.begin(); // Get sensor online
  mpl.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa
  mpl.setOversampleRate(7); // Set Oversample to the recommended 128
  mpl.enableEventFlags(); // Enable all three pressure and temp event flags 

  //Configure the humidity sensor
  Serial.println("Init HTU21D!!");
  htu.begin();

  Serial.println("Init Webserver!!");
  webserver.setDefaultCommand(&defaultCmd);
  webserver.addCommand("stats.json", &statsCmd);
  webserver.begin();
}

void loop() {
  rain.loop();
  webserver.processConnection();
  delay(100);
}