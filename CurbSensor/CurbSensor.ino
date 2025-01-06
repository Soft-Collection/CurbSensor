#include "Memory.h"
#include "Buzzer.h"
#include "DistanceMeasure.h"
#include "CommonFunctions.h"

//------------------------------------
#define CLOSE_THRESHOLD 10
//------------------------------------
#define CALIBRATE_TIME_IN_MS 10000
#define MUD_TIME_IN_MS 30000
//------------------------------------
#define MUD_SIGNAL_REPEAT_PERIOD_IN_MS 30000
//------------------------------------
#define STATUS_REGULAR 0
#define STATUS_CALIBRATE 1
#define STATUS_MUD 2
//------------------------------------

Memory memory;
Buzzer buzzer;
DistanceMeasure distanceMeasure;

int16_t threshold = 0;

void check();

void setup() 
{
  Serial.begin(115200);
  while (!Serial) delay(10);
  //------------------------------
  //memory.clear();
  threshold = memory.load();
  Serial.print("Threshold is: ");
  Serial.println(threshold);
  //------------------------------
  buzzer.begin(13);
  //------------------------------
  distanceMeasure.begin();
}

void loop() 
{
  check();
  buzzer.check();
}

void check()
{
  int16_t distance = distanceMeasure.getDistance();
  if (distance < 0) return;
  Serial.print(F("Distance: "));
  Serial.print(distance);
  Serial.println(" mm");
  bool isClose = ((distance >= 0) && (distance < CLOSE_THRESHOLD));
  static uint32_t lastIsCloseTick = 0;  
  uint8_t status = STATUS_REGULAR;
  static uint8_t lastStatus = STATUS_REGULAR;
  static uint32_t lastMudSignalTick = 0;
  uint32_t TC = millis();
  //---------------------------------------------------
  if (!isClose)
  {
    lastIsCloseTick = TC;
    status = STATUS_REGULAR;
    if (distance < threshold)
    {
      buzzer.startBeep();      
    }
    else
    {
      buzzer.stopBeep();
    }
  }
  else
  {
    buzzer.stopBeep();
  }
  //---------------------------------------------------
  if (minus_time(TC, lastIsCloseTick) > MUD_TIME_IN_MS)
  {
    status = STATUS_MUD;
  }
  else if (minus_time(TC, lastIsCloseTick) > CALIBRATE_TIME_IN_MS)
  {
    status = STATUS_CALIBRATE;
  }
  //---------------------------------------------------
  if (lastStatus != status)
  {
    //--------------------------------
    if (status == STATUS_REGULAR)
    {
      if (lastStatus == STATUS_CALIBRATE)
      {
        threshold = distance;
        Serial.print("Threshold is: ");
        Serial.println(threshold);
        memory.save(threshold);
        buzzer.startBeep(5, 100, 100);
      }
    }
    else if (status == STATUS_CALIBRATE)
    {
      buzzer.startBeep(3, 500, 500);
    }
    else if (status == STATUS_MUD)
    {
      lastMudSignalTick = TC;
      buzzer.startBeep(10, 50, 50);
    }
    //--------------------------------
    lastStatus = status;
  }
  //---------------------------------------------------
  if (minus_time(TC, lastMudSignalTick) > MUD_SIGNAL_REPEAT_PERIOD_IN_MS)
  {
    if (status == STATUS_MUD)
    {
      buzzer.startBeep(10, 50, 50);
    }
    lastMudSignalTick = TC;
  }
}
