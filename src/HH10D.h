/********************************************************
  Lib for Humidity sensor - HH10D
  
********************************************************/
#ifndef __HH10D_LIB_H
#define __HH10D_LIB_H

#include <Wire.h>

#define HH10D_SENSITIVITY_REG  0x0A
#define HH10D_OFFSET_REG       0x0C
#define HH10D_ADDRESS          0x51

class HH10D {
  public:
    void begin(int interrupt);
    void interruptHandler();
    float read();
    float readOffset();
    float readSensitivity();
  private:
    int interrupt;
    float offset, sensitivity;
    unsigned long interruptCount, firstInterruptAt, latestInterruptAt;
};

void HH10D::begin(int interrupt) {
  HH10D::interrupt = interrupt;
  HH10D::sensitivity = HH10D::readSensitivity();
  HH10D::offset = HH10D::readOffset();
  HH10D::interruptCount = 0;
  HH10D::firstInterruptAt = 0;
  HH10D::latestInterruptAt = 0;
  //attachInterrupt(interrupt, HH10D::interruptHandler, RISING);
}
void HH10D::interruptHandler() {
  unsigned long time = micros();
  if(HH10D::firstInterruptAt > time){// Micro overfloat
    HH10D::interruptCount = 0;
  }
  if (HH10D::interruptCount == 0){
    HH10D::firstInterruptAt = time;
  }
  HH10D::latestInterruptAt = time;
  HH10D::interruptCount++;
}
float HH10D::read() {
  unsigned long count;
  unsigned long start;
  unsigned long finish;

  if(count < 100) {//Need more data
    return -1.0;
  }

  noInterrupts();
  count = HH10D::interruptCount;
  start = HH10D::firstInterruptAt;
  finish = HH10D::latestInterruptAt;
  HH10D::interruptCount = 0;
  interrupts();

  float frequency = float(count)*1000000.0/float(finish - start);
  return (HH10D::offset-frequency)*HH10D::sensitivity/4096;//pow(2,12) or 0x1 << 12
}
float HH10D::readSensitivity() {
  Wire.beginTransmission(HH10D_ADDRESS);
  Wire.write(HH10D_SENSITIVITY_REG);
  Wire.endTransmission();
  Wire.requestFrom(HH10D_ADDRESS, 2);
  return float( Wire.read() << 8 | Wire.read() );
}
float HH10D::readOffset() {
  Wire.beginTransmission(HH10D_ADDRESS);
  Wire.write(HH10D_OFFSET_REG);
  Wire.endTransmission();
  Wire.requestFrom(HH10D_ADDRESS, 2);
  return float( Wire.read() << 8 | Wire.read() );
}

#endif /* __HH10D_LIB_H */
