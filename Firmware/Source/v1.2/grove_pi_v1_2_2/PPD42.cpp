/* DUST library */

#include <PinChangeInt.h>
#include <PinChangeIntConfig.h>
#include "PPD42.h"
 /*
DHT::DUST(uint8_t pin10, uint8_t pin25, uint8_t sampling) {
  _pin10 = pin10;
  _pin25 = pin25;
  _sampling = sampling;
  firstreading = true;
  _PM10count = 0;
}
*/
volatile long triggerOnP1;
volatile long durationP1;
volatile long durationP2;
volatile float PM10count; // Volatile or just properties of DUST class ?
volatile float PM25count;

void DUST::begin(uint8_t pin10, uint8_t pin25, uint8_t sampletime_ms) {
  // set up the pins!
  _pin10 = pin10;
  _pin25 = pin25;
  _sampletime_ms = sampletime_ms;
  startTime = millis();
  
  PCintPort::attachInterrupt(pin10, pin10trigger,CHANGE);
  PCintPort::attachInterrupt(pin25, pin125trigger,CHANGE);
}

void pin10trigger()
{
  valP1 = digitalRead(_pin25);
  if(valP1 == LOW){
    triggerOnP1 = micros(); 
  }
  if (valP1 == HIGH){
      triggerOffP1 = micros();
      pulseLengthP1 = triggerOffP1 - triggerOnP1;
      durationP1 = durationP1 + pulseLengthP1;
  }
}

void pin25trigger()
{
  valP2 = digitalRead(_pin25);
  if(valP2 == LOW){
    triggerOnP2 = micros(); 
  }
  if (valP2 == HIGH){
      triggerOffP2 = micros();
      pulseLengthP2 = triggerOffP2 - triggerOnP2;
      durationP2 = durationP2 + pulseLengthP2;
  }
}


void calculation()
{
    cli(); // disable interrupts
    
    // Function creates particle count and mass concentration
    // from PPD-42 low pulse occupancy (LPO).
    if ((millis() - startTime) > _sampletime_ms) {
      
      // Generates PM10 and PM2.5 count from LPO.
      // Derived from code created by Chris Nafis
      // http://www.howmuchsnow.com/arduino/airquality/grovedust/
      
      ratioP1 = durationP1/(_sampletime_ms*10.0);
      ratioP2 = durationP2/(_sampletime_ms*10.0);
      countP1 = 1.1*pow(ratioP1,3)-3.8*pow(ratioP1,2)+520*ratioP1+0.62;
      countP2 = 1.1*pow(ratioP2,3)-3.8*pow(ratioP2,2)+520*ratioP2+0.62;
      PM10count = countP2;
      PM25count = countP1 - countP2;
      
   
      // begins PM10 mass concentration algorithm
      /*
      double r10 = 2.6*pow(10,-6);
      double pi = 3.14159;
      double vol10 = (4/3)*pi*pow(r10,3);
      double density = 1.65*pow(10,12);
      double mass10 = density*vol10;
      double K = 3531.5;
      float concLarge = (PM10count)*K*mass10;
      
      // next, PM2.5 mass concentration algorithm
      double r25 = 0.44*pow(10,-6);
      double vol25 = (4/3)*pi*pow(r25,3);
      double mass25 = density*vol25;
      float concSmall = (PM25count)*K*mass25;
      */
      
      // reset for next measure
      startTime = millis();
      durationP1 = 0;
      durationP2 = 0;
      
      sei(); //enable interrupts
    }
}
void DUST::stop()
{
  PCintPort::detachInterrupt(pin10);
  PCintPort::detachInterrupt(pin25);
}
