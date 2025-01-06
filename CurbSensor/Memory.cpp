#include "Memory.h"

Memory::Memory()
{
}

Memory::~Memory() 
{
}

void Memory::clear()
{
  for (int i = 0; i < EEPROM.length(); i++)
  {
    EEPROM.write(i, 0);
  }
}

int16_t Memory::load()
{
  int16_t val = 0;
  EEPROM.get(0, val);
  return val;
}

void Memory::save(int16_t val)
{
  EEPROM.put(0, val);
}
