#ifndef DistanceMeasure_H
#define DistanceMeasure_H

#include "Adafruit_VL53L1X.h"

#define IRQ_PIN 2
#define XSHUT_PIN 3

class DistanceMeasure {
private:
  Adafruit_VL53L1X vl53;
public:
  DistanceMeasure();
  ~DistanceMeasure();
  void begin();
  int16_t getDistance();
  int16_t getEstablishedDistance();
};

#endif  // DistanceMeasure_H
