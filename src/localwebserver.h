#ifndef LOCALWEBSERVER_H
#define LOCALWEBSERVER_H

#include "struct_sensors.h"

SensorData data_sensor_updater;

void setLocalNetwork();
void update_sensors_values(SensorData data_sensor_updater);
#endif