#include "DistanceMeasure.h"

#define MAX_ADJASCENT_DISTANCES_CHANGE 5

DistanceMeasure::DistanceMeasure() :
  vl53(Adafruit_VL53L1X(XSHUT_PIN, IRQ_PIN))
{
}

DistanceMeasure::~DistanceMeasure() 
{
}

void DistanceMeasure::begin()
{
  Wire.begin();
  if (!vl53.begin(0x29, &Wire)) 
  {
    Serial.print(F("Error on init of VL sensor: "));
    Serial.println(vl53.vl_status);
    while (1) delay(10);
  }
  Serial.println(F("VL53L1X sensor OK!"));
  Serial.print(F("Sensor ID: 0x"));
  Serial.println(vl53.sensorID(), HEX);
  if (!vl53.startRanging()) 
  {
    Serial.print(F("Couldn't start ranging: "));
    Serial.println(vl53.vl_status);
    while (1) delay(10);
  }
  Serial.println(F("Ranging started"));
  // Valid timing budgets: 15, 20, 33, 50, 100, 200 and 500ms!
  vl53.setTimingBudget(20);
  Serial.print(F("Timing budget (ms): "));
  Serial.println(vl53.getTimingBudget());
  /*
  vl.VL53L1X_SetDistanceThreshold(100, 300, 3, 1);
  vl.VL53L1X_SetInterruptPolarity(0);
  */
}

int16_t DistanceMeasure::getDistance()
{
  int16_t distance = 0;
  if (vl53.dataReady()) 
  {
    // new measurement for the taking!
    distance = vl53.distance();
    if (distance == -1) 
    {
      // something went wrong!
      Serial.print(F("Couldn't get distance: "));
      Serial.println(vl53.vl_status);
      return -1;
    }
    // data is read out, time for another reading!
    vl53.clearInterrupt();
    return distance;
  }
  return -2; //Data not ready
}

int16_t DistanceMeasure::getEstablishedDistance()
{
  static int16_t lastDistance = 0;
  int16_t distance = this->getDistance();
  if (distance < 0) return distance;
  if (abs(lastDistance - distance) < MAX_ADJASCENT_DISTANCES_CHANGE)
  {
    lastDistance = distance;
    return distance;
  }
  lastDistance = distance;
  return -3; //lastDistance and distance are not close.
}
