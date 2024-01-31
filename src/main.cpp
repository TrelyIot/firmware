#include <Arduino.h>
#include <WiFi.h>
#include "sender.h"
#include "localwebserver.h"
#include "store.h"
#include "sensorhandler.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

int PERIOD_CYCLE_SENSORS  = 0.1 * 60 * 1000;
int PERIOD_CYCLE_DELIVER  = 0.1 * 60 * 1000;

SensorData data;

void set_sampling_time(int samp) {
  PERIOD_CYCLE_SENSORS = samp;
}

void set_send_timer(int sendT) {
  PERIOD_CYCLE_DELIVER = sendT;
}

void sensorTask(void *pvParameters) {
  for (;;) {
    update_sensors_values(sensor_loop_readings());
    vTaskDelay(PERIOD_CYCLE_SENSORS / portTICK_PERIOD_MS); // adjust delay as needed
  }
}

void deliveryTask(void *pvParameters) {
  for (;;) {
    sender_delivery(data);
    vTaskDelay(PERIOD_CYCLE_DELIVER / portTICK_PERIOD_MS); // adjust delay as needed
  }
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  
  store_begin();

  int sam = get_sampling();
  if(sam > 0) set_sampling_time(sam); // it has to be in millseconds

  int sent_t = get_send_time();
  if(sent_t > 0) set_send_timer(sent_t); // it has to be in millseconds

  int pulse_mode = get_pulse_mode();
  if(pulse_mode == 0) sensorhandler_begin(false);
  else sensorhandler_begin(true);

  sender_begin(get_gprs_mac(), get_id_station());
  setLocalNetwork();

  // sensorhandler_begin();
  xTaskCreatePinnedToCore(sensorTask, "SensorTask", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(deliveryTask, "DeliveryTask", 4096, NULL, 1, NULL, 1);
}

void loop() {
}