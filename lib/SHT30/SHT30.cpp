#include <Arduino.h>
#include <Wire.h>

#define DEVICE_ADDRESS 0x44 // SHT30

bool read_sensor_SHT30(float& temperature, float& humidity) {
    unsigned int data[6];
    
    Wire.beginTransmission(DEVICE_ADDRESS);
    Wire.write(0x2C);
    Wire.write(0x06);
    // Wire.endTransmission();
    int transmissionStatus = Wire.endTransmission();
    if (transmissionStatus == 0) {
        // Serial.println("Transmission successful");
         delay(500);
    
        Wire.requestFrom(DEVICE_ADDRESS, 6);
        for (int i = 0; i < 6; i++) {
            data[i] = Wire.read();
        }
        delay(50);
        
        temperature = ((((data[0] * 256.0) + data[1]) * 175) / 65535.0) - 45;
        humidity = ((((data[3] * 256.0) + data[4]) * 100) / 65535.0);
        return true;  
    } 
    return false;
}
