#ifndef SENDER_H
#define SENDER_H
#include "struct_sensors.h"

SensorData data_deliver;

void sender_begin(String macString, int id);
void sender_delivery(SensorData data_deliver);

#endif