#include <Arduino.h>
#include "ESP_NOW_HALF.h"
#include "struct_message.h"
#include "struct_sensors.h"
#include "statushandler.h"

int id_device;
// uint8_t mac_destine[] = {0xC4, 0xDE, 0xE2, 0xBA, 0x25, 0xEC};

void stringToMac(const String& macString, uint8_t macDestine[]) {
    // Split the MAC address string into individual hex values
    for (int i = 0; i < 6; i++) {
        String hexByte = macString.substring(i * 2, i * 2 + 2);
        macDestine[i] = strtoul(hexByte.c_str(), nullptr, 16);
    }
}

void sender_begin(String macString, int id) {
    id_device = id;
    uint8_t macDestine[6];
    stringToMac(macString, macDestine);
    espnowsndr_setup(id, macDestine);
}

void sender_delivery(SensorData data_deliver) {
    struct_message myDta;
    myDta.id = id_device;
    myDta._0x60 = data_deliver.temperature + 0.000001;
    myDta._0x61 = data_deliver.humidity + 0.000001;
    myDta._0x62 = data_deliver.pulses + 0.000001;
    myDta._0x63 = data_deliver.current + 0.000001;
    myDta._0x64 = data_deliver.thermocouple + 0.000001;
    myDta._0x65 = data_deliver.airflow + 0.000001;

    espnowsndr_loop(&myDta);
    // logStatus(espnowsndr_loop(&myDta)); // Flow and CO2 stack boiler side
    logStatus("========");
    logStatus(String(data_deliver.temperature));
    // (status_OnDataSent() == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
    logStatus(status_OnDataSent() == 0 ? "Delivery Success" : "Delivery Fail"); // 0 - success | 1 - fail
    // logStatus(String(status_OnDataSent())); // 0 - success | 1 - fail
}