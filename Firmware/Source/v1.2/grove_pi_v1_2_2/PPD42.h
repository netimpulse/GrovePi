#ifndef DUST_H
#define DUST_H
#include "Arduino.h"
#include <PinChangeInt.h>
#include <PinChangeIntConfig.h>


/* DUST library 

*/

class DUST {
 private:
  uint8_t data[6];
  uint8_t _pin10, _pin25, _sampling_time;
  boolean read(void);
  unsigned long _lastreadtime;
  long triggerOnP1;
  volatile long durationP1;
  volatile long durationP2;
  float _PM10count;
  float _PM25count;
  void pin10trigger(void);
  void pin25trigger(void);
  

 public:
  //DUST(uint8_t pin10, uint8_t pin25, uint8_t _sampling_time=30000);
  void begin(uint8_t pin10, uint8_t pin25, uint8_t _sampling_time=30000);
  float readPM10();
  float readPM25();
  void readAndCache();
  void stop();

};
#endif
