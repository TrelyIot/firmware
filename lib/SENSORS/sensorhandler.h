#ifndef SENSORHANDLER_H
#define SENSORHANDLER_H

#include "struct_sensors.h"

SensorData sensor_loop_readings();
void sensorhandler_begin(bool pulse_mode);
#endif