#ifndef _STRUCT_SENSORS_H
#define _STRUCT_SENSORS_H

#include <Arduino.h>

struct SensorData {
  float temperature;
  float humidity;
  unsigned long long pulses;
  int current;
  float thermocouple;
  float airflow;
};

#endif