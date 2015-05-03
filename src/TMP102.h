/********************************************************
  Lib for Digital Temperature Sensor Breakout - TMP102
  
********************************************************/
#ifndef __TMP102_LIB_H
#define __TMP102_LIB_H

#include <Wire.h>

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#define TMP102_ADDRESS_GND   0x48
#define TMP102_ADDRESS_VCC   0x49
#define TMP102_ADDRESS_SDA   0x4A
#define TMP102_ADDRESS_SCL   0x4B
#define TMP102_RESOLUTION    0.0625
#define TMP102_TEMP_REG      0x00
#define TMP102_CONF_REG      0x01
#define TMP102_TEMP_LOW_REG  0x02
#define TMP102_TEMP_HIGH_REG 0x03

/*
Configuration Register
OS  R1  R0  F1  F0  POL  TM  SD
CR1  CR0  AL  EM  0  0  0  0
*/
#define TMP102_ALERT_BIT      0x0020
#define TMP102_EXTENDED_BIT   0x0010
#define TMP102_SHUTDOWN_BIT   0x0100
#define TMP102_THERMOSTAT_BIT 0x0200
#define TMP102_ONESHOT_BIT    0x8000

//Continuos-Conversion Mode (CR1/CR0)
#define TMP102_CONF_8HZ 0x00C0  // 8 Hz conversion rate
#define TMP102_CONF_4HZ 0x0080  // 4 Hz conversion rate *default*
#define TMP102_CONF_1HZ 0x0040  // 1 Hz conversion rate
#define TMP102_CONF_QHZ 0x0000  // 0.25 Hz conversion rate (Q = 1/4)

// Extended Mode
#define TMP102_CONF_EM_ON  TMP102_EXTENDED_BIT
#define TMP102_CONF_EM_OFF 0x0000

// Shutdown Mode
#define TMP102_CONF_SD_ON  TMP102_SHUTDOWN_BIT
#define TMP102_CONF_SD_OFF 0x0000

// One-Shot/Conversion Ready
#define TMP102_CONF_OS_ON  TMP102_ONESHOT_BIT
#define TMP102_CONF_OS_OFF 0x0000

// Thermostat Mode
#define TMP102_CONF_TM_ON   TMP102_THERMOSTAT_BIT
#define TMP102_CONF_TM_OFF  0x0000

#define TMP102_CONF_COMPARATION TMP102_CONF_TM_OFF
#define TMP102_CONF_INTERRUPT   TMP102_CONF_TM_ON

// Polarity
#define TMP102_CONF_POL_UP   0x0000
#define TMP102_CONF_POL_DOWN 0x0400

// Fault Queue (F1/F0)
#define TMP102_CONF_FAULT_1 0x0000
#define TMP102_CONF_FAULT_2 0x0800
#define TMP102_CONF_FAULT_4 0x1000
#define TMP102_CONF_FAULT_6 0x1800

class TMP102 {
  public:
    void begin(int address, int pinAlert=-1);
    float read();
    int readConf();
    bool setConf(int conf);
    int readHigh();
    bool setHigh(int value);
    int readLow();
    bool setLow(int value);
    bool areAlert();
  private:
    int address, pinAlert;
};

void TMP102::begin(int address, int pinAlert) {
  TMP102::address = address;
  TMP102::pinAlert = pinAlert;
}

float TMP102::read() {
  Wire.beginTransmission(TMP102::address);
  Wire.write(TMP102_TEMP_REG);
  Wire.endTransmission();
  Wire.requestFrom(TMP102::address, 2);
  return float( Wire.read() << 8 | Wire.read() >> 4) * TMP102_RESOLUTION;
}
int TMP102::readConf(){
  Wire.beginTransmission(TMP102::address);
  Wire.write(TMP102_CONF_REG);
  Wire.endTransmission();
  Wire.requestFrom(TMP102::address, 2);
  return Wire.read() << 8 | Wire.read();
}
bool TMP102::setConf(int conf){
  Wire.beginTransmission(TMP102::address);
  Wire.write(TMP102_CONF_REG);
  Wire.write(conf);
  return Wire.endTransmission() == 0;
}
bool TMP102::areAlert(){
  if (TMP102::pinAlert < 0){
    int conf = TMP102::readConf();
    if (conf && TMP102_CONF_POL_DOWN){
      return !(conf && TMP102_ALERT_BIT);
    }
    return (conf && TMP102_ALERT_BIT);
  }
  return digitalRead(TMP102::pinAlert);
}
int TMP102::readHigh() {
  Wire.beginTransmission(TMP102::address);
  Wire.write(TMP102_TEMP_HIGH_REG);
  Wire.endTransmission();
  Wire.requestFrom(TMP102::address, 2);
  return Wire.read() << 8 | Wire.read();
}
bool TMP102::setHigh(int value) {
  Wire.beginTransmission(TMP102::address);
  Wire.write(TMP102_TEMP_HIGH_REG);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}
int TMP102::readLow() {
  Wire.beginTransmission(TMP102::address);
  Wire.write(TMP102_TEMP_LOW_REG);
  Wire.endTransmission();
  Wire.requestFrom(TMP102::address, 2);
  return Wire.read() << 8 | Wire.read();
}
bool TMP102::setLow(int value) {
  Wire.beginTransmission(TMP102::address);
  Wire.write(TMP102_TEMP_LOW_REG);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

#endif /* __TMP102_LIB_H */
