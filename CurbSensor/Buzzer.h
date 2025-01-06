#ifndef Buzzer_H
#define Buzzer_H

#include <Arduino.h>

class Buzzer 
{
private:
  uint8_t  mBuzzerPin;
  uint8_t  mTimes;
  uint16_t mLengthInMS;
  uint16_t mDelayInMS;
  uint32_t mLastEventTick;
  bool     mIsPerformingSequenceOfBeeps;
  uint8_t  mCurrentTime;
  bool     mIsInBeep;
private:
  void clear();
public:
  Buzzer();
  ~Buzzer();
  void begin(uint8_t buzzerPin);
  void end();
  void check();
  void startBeep(uint8_t times, uint16_t lengthInMS, uint16_t delayInMS);
  void startBeep();
  void stopBeep();
};

#endif  // Buzzer_H
