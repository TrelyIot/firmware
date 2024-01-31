#include <Arduino.h>
#include <Wire.h>
#include "temp_max.h"
#include "pulse.h"
#include "SHT30.h"
#include "current.h"
#include "struct_sensors.h"
#include "omron.h"

#define TEMPERATURE_ENABLE 10

void thermocouple_begin() {
    pinMode(TEMPERATURE_ENABLE, OUTPUT);
    digitalWrite(TEMPERATURE_ENABLE, HIGH);
}

void sensorhandler_begin(bool pulse_mode) {
    Wire.begin(); // Initialize I2C communication
    thermocouple_begin();
    pulse_begin(pulse_mode); // false for FM | true for GAS
}

SensorData sensor_loop_readings() {
    SensorData mySensorData;
    float SHT30_TEMPERATURE;
    float SHT30_HUMIDITY;

    if(read_sensor_SHT30(SHT30_TEMPERATURE, SHT30_HUMIDITY)) {
        mySensorData.temperature = SHT30_TEMPERATURE;
        mySensorData.humidity = SHT30_HUMIDITY;
    }
    
    mySensorData.current = get_CT();
    mySensorData.thermocouple = get_temp_c();

    mySensorData.pulses = get_PULSE();
    mySensorData.airflow = get_diff_press();
    return mySensorData;
}