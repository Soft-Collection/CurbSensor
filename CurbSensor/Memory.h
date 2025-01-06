#ifndef Memory_H
#define Memory_H

#include <Arduino.h>
#include <EEPROM.h>

class Memory
{
public:
  Memory();
  ~Memory();
  void clear();
  int16_t load();
  void save(int16_t val);
};

#endif  // Memory_H
