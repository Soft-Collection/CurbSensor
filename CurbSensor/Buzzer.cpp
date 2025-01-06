#include "Buzzer.h"
#include "CommonFunctions.h"

Buzzer::Buzzer() : 
  mBuzzerPin(0),
  mTimes(0),
  mLengthInMS(0),
  mDelayInMS(0),
  mLastEventTick(0),
  mIsPerformingSequenceOfBeeps(false),
  mCurrentTime(0),
  mIsInBeep(false)
{
}

Buzzer::~Buzzer() 
{
}

void Buzzer::begin(uint8_t buzzerPin)
{
  this->mBuzzerPin = buzzerPin;
  pinMode(this->mBuzzerPin, OUTPUT);
}

void Buzzer::end()
{
}

void Buzzer::check() 
{
  if (!this->mIsPerformingSequenceOfBeeps) return;
  uint32_t TC = millis();
  //-----------------------------------------------
  if (minus_time(TC, mLastEventTick) > ((this->mIsInBeep) ? this->mLengthInMS : this->mDelayInMS))
  {
    //--------------------------------------------
    if (this->mIsInBeep)
    {
      digitalWrite(mBuzzerPin, LOW);
      this->mIsInBeep = false;
      this->mCurrentTime++;
      if (this->mCurrentTime >= this->mTimes)
      {
        digitalWrite(mBuzzerPin, LOW);
        this->clear();
      }
    }
    else
    {
      digitalWrite(mBuzzerPin, HIGH);
      this->mIsInBeep = true;
    }
    //--------------------------------------------
    mLastEventTick = TC;
  }
}

void Buzzer::startBeep(uint8_t times, uint16_t lengthInMS, uint16_t delayInMS)
{
  this->mTimes = times;
  this->mLengthInMS = lengthInMS;
  this->mDelayInMS = delayInMS;
  this->mLastEventTick = millis();
  this->mIsPerformingSequenceOfBeeps = true;
  this->mIsInBeep = true;
  digitalWrite(mBuzzerPin, HIGH);
}

void Buzzer::startBeep()
{
  if (this->mIsPerformingSequenceOfBeeps) return;
  this->clear();
  this->mIsInBeep = true;
  digitalWrite(mBuzzerPin, HIGH);
}

void Buzzer::stopBeep()
{
  if (this->mIsPerformingSequenceOfBeeps) return;
  digitalWrite(mBuzzerPin, LOW);
  this->clear();
}

void Buzzer::clear()
{
  this->mTimes = 0;
  this->mLengthInMS = 0;
  this->mDelayInMS = 0;
  this->mLastEventTick = 0;
  this->mIsPerformingSequenceOfBeeps = false;
  this->mCurrentTime = 0;
  this->mIsInBeep = false;
}
