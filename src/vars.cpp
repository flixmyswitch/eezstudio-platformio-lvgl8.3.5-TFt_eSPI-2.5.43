#include <stdint.h>
#include "vars.h"
#include <Arduino.h>

int32_t speed = 100;

extern "C" int32_t get_var_speed()
{
  return speed;
}

extern "C" void set_var_speed(int32_t value)
{
  speed = value;

  Serial.print("Speed is =  ");
  Serial.println(speed);
  delay(100);
}

int32_t counter = 0;
int32_t get_var_counter()
{
  return counter;
}
void set_var_counter(int32_t value)
{
  counter = value;
}
