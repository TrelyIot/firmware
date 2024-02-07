#include <Arduino.h>
#include "ESPAsyncWebServer.h"
#include "sensorhandler.h"
#include "struct_sensors.h"
#include "store.h"
#include "statushandler.h"

const char* ssid_default = "Trely";
char soft_ap_ssid[30];

AsyncWebServer server(80);
AsyncEventSource events("/events");

SensorData data_sensors;

const char* PARAM_INPUT_1 = "gprsmac"; // ESP_MASTER MAC
const char* PARAM_INPUT_2 = "idstation"; // ID device
const char* PARAM_INPUT_3 = "sampling_time"; // For sensor time > 1000
const char* PARAM_INPUT_4 = "send_time"; // ID device
const char* PARAM_INPUT_5 = "pulseselector"; // ID device

void update_data() {
  events.send("ping",NULL,millis());
  events.send(String(data_sensors.temperature).c_str(),"temperature",millis());
  events.send(String(data_sensors.humidity).c_str(),"humidity",millis());
  events.send(String(data_sensors.pulses).c_str(),"pulses",millis());
  events.send(String(data_sensors.current).c_str(),"current",millis());
  events.send(String(data_sensors.thermocouple).c_str(),"thermocouple",millis());
}

void update_sensors_values(SensorData data_sensor_updater) {
    data_sensors.temperature = data_sensor_updater.temperature;
    data_sensors.humidity = data_sensor_updater.humidity;
    data_sensors.pulses = data_sensor_updater.pulses;
    data_sensors.current = data_sensor_updater.current;
    data_sensors.thermocouple = data_sensor_updater.thermocouple;
    data_sensors.airflow = data_sensor_updater.airflow;
    update_data();
}

String processor_status(const String& var) {
    if(var == "STATUS_TEXT") {
        return String(getLogs());
    }
    return String();
}

String processor_setup(const String& var) {
    if(var == "MAC_DESTINE") {
        return String(get_gprs_mac());
    }else if(var == "ID_STATION") {
        return String(get_id_station());
    }else if(var == "SAMPLING") {
        return String(get_sampling());
    }else if(var == "SEND_TIME") {
        return String(get_send_time());
    }else if(var == "PULSE_MODE") {
        return String(get_pulse_mode());
    }
    return String();
}

String processor(const String& var){
//   getSensorReadings();

  if(var == "TEMPERATURE"){
    return String(data_sensors.temperature);
  }
  else if(var == "HUMIDITY"){
    return String(data_sensors.humidity);
  }
  else if(var == "PULSES"){
    return String(data_sensors.pulses);
  }
  else if(var == "CURRENT"){
    return String(data_sensors.current);
  }
  else if(var == "THERMOCOUPLE"){
    return String(data_sensors.thermocouple);
  }
  else if(var == "AIRFLOW"){
    return String(data_sensors.airflow);
  }
  return String();
}

const char status_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML><html>
    <head>
    <title>Trely Status</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        html {font-family: Arial; display: inline-block; text-align: center;}
        p { font-size: 1.2rem;}
        body {  margin: 0;}
        .topnav { overflow: hidden; background-color: #242424; color: white; font-size: 1rem; }
    </style>
    </head>
        <body>
            <div class="topnav">
                <h1>Trely Device</h1>
                <p><a class="back-link" href="/">Back</a></p>
            </div>

    

            <div>
                <p>Status</p>
                <label>GPRS Sent Status: <span id="status_text">%STATUS_TEXT%</span></label>
            </div>

        </body>
        <script>
            if (!!window.EventSource) {
            var source = new EventSource('/events');
            
                source.addEventListener('open', function(e) {
                    console.log("Events Connected");
                }, false);

                source.addEventListener('error', function(e) {
                    if (e.target.readyState != EventSource.OPEN) {
                        console.log("Events Disconnected");
                    }
                }, false);

                source.addEventListener('status_text', function(e) {
                    console.log("status_text", e.data);
                    document.getElementById("status_text").innerHTML = e.data;
                }, false);
            }
        </script>
    </html>
)rawliteral";

const char setup_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML><html>
    <head>
    <title>Trely Setup</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        html {font-family: Arial; display: inline-block; text-align: center;}
        p { font-size: 1.2rem;}
        body {  margin: 0;}
        .topnav { overflow: hidden; background-color: #242424; color: white; font-size: 1rem; }
    </style>
    </head>
        <body>
            <div class="topnav">
                <h1>Trely Device</h1>
                <p><a class="back-link" href="/">Back</a></p>
                <a href="/status">Status</a>
            </div>
            <div>
              <form action="/esp_now_mac" method="POST">
                <p>
                  <label for="gprsmac">GPRS Master MAC: <span id="mac_gprs">%MAC_DESTINE%</span></label>

                  <input type="text" id ="gprsmac" name="gprsmac"><br>
                  <input type ="submit" value ="Submit">
                </p>
              </form>
            </div>

            <div>
              <form action="/esp_now_id_station" method="POST">
                <p>
                  <label for="idstation">ID Station: <span id="station_id">%ID_STATION%</span></label>
                  <input type="text" id ="idstation" name="idstation"><br>
                  <input type ="submit" value ="Submit">
                </p>
              </form>
            </div>

            <div>
              <form action="/esp_sampling" method="POST">
                <p>
                  <label for="sampling_time">Sampling Time: <span id="sampling">%SAMPLING%</span> mills</label>
                  <input type="text" id ="sampling_time" name="sampling_time"><br>
                  <input type ="submit" value ="Submit">
                </p>
              </form>
            </div>

            <div>
              <form action="/esp_send_time" method="POST">
                <p>
                  <label for="send_time">Sending time: <span id="sendtime">%SEND_TIME%</span> mills</label>
                  <input type="text" id ="send_time" name="send_time"><br>
                  <input type ="submit" value ="Submit">
                </p>
              </form>
            </div>

            <div>
                <form action="/pulse_selector" method="POST">
                    <p>
                        <label for="pulseselector">Choose Pulse Selector: <span id="pulsemode">%PULSE_MODE%</span></label>
                        <select id="pulseselector" name="pulseselector">
                            <option value="0">Flow</option>
                            <option value="1">Pulse</option>
                        </select><br>
                        <input type="submit" value="Submit">
                        </p>
                </form>
            </div>

        </body>
        <script>
            if (!!window.EventSource) {
            var source = new EventSource('/events');
            
                source.addEventListener('open', function(e) {
                    console.log("Events Connected");
                }, false);

                source.addEventListener('error', function(e) {
                    if (e.target.readyState != EventSource.OPEN) {
                        console.log("Events Disconnected");
                    }
                }, false);
            
                source.addEventListener('mac_gprs', function(e) {
                    console.log("mac_gprs", e.data);
                    document.getElementById("_gprs_mac").innerHTML = e.data;
                }, false);

                source.addEventListener('station_id', function(e) {
                    console.log("station_id", e.data);
                    document.getElementById("stationid").innerHTML = e.data;
                }, false);

                source.addEventListener('sampling', function(e) {
                    console.log("sampling", e.data);
                    document.getElementById("sampling").innerHTML = e.data;
                }, false);

                source.addEventListener('sendtime', function(e) {
                    console.log("sendtime", e.data);
                    document.getElementById("sendtime").innerHTML = e.data;
                }, false);

                source.addEventListener('pulsemode', function(e) {
                    console.log("pulsemode", e.data);
                    if(e.data == "0") document.getElementById("pulsemode").innerHTML = "Flow";
                    else document.getElementById("pulsemode").innerHTML = "Pulse";
                }, false);
            }
        </script>
    </html>
)rawliteral";

const char index_html[] PROGMEM = R"rawliteral(
    <!DOCTYPE HTML><html>
    <head>
    <title>Trely</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        html {font-family: Arial; display: inline-block; text-align: center;}
        p { font-size: 1.2rem;}
        body {  margin: 0;}
        .topnav { overflow: hidden; background-color: #242424; color: white; font-size: 1rem; }
        .content { padding: 20px; }
        .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
        .cards { max-width: 800px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); }
        .reading { font-size: 1.4rem; }
    </style>
    </head>
        <body>
            <div class="topnav">
                <h1>Trely Device</h1>
                <a href="/setup">Setup</a>
                <a href="/status">Status</a>
            </div>
            <div class="content">
                <div class="cards">
                  <div class="card">
                      <p>TEMPERATURE</p><p><span class="reading"><span id="temp">%TEMPERATURE%</span> &deg;C</span></p>
                  </div>
                  <div class="card">
                      <p> HUMIDITY</p><p><span class="reading"><span id="hum">%HUMIDITY%</span> &percnt;</span></p>
                  </div>
                  <div class="card">
                      <p> PULSES</p><p><span class="reading"><span id="pulses">%PULSES%</span> pulses</span></p>
                  </div>
                  <div class="card">
                      <p> CURRENT</p><p><span class="reading"><span id="current">%CURRENT%</span> points</span></p>
                  </div>
                  <div class="card">
                      <p> THERMO</p><p><span class="reading"><span id="thermocouple">%THERMOCOUPLE%</span> &deg;C</span></p>
                  </div>
                  <div class="card">
                      <p> AIRFLOW</p><p><span class="reading"><span id="airflow">%AIRFLOW%</span> &deg;kPa</span></p>
                  </div>
                </div>
            </div>
        </body>
        <script>
            if (!!window.EventSource) {
            var source = new EventSource('/events');
            
                source.addEventListener('open', function(e) {
                    console.log("Events Connected");
                }, false);

                source.addEventListener('error', function(e) {
                    if (e.target.readyState != EventSource.OPEN) {
                        console.log("Events Disconnected");
                    }
                }, false);
            
                source.addEventListener('message', function(e) {
                    console.log("message", e.data);
                }, false);
                
                source.addEventListener('temperature', function(e) {
                    console.log("temperature", e.data);
                    document.getElementById("temp").innerHTML = e.data;
                }, false);
            
                source.addEventListener('humidity', function(e) {
                    console.log("humidity", e.data);
                    document.getElementById("hum").innerHTML = e.data;
                }, false);
            
                source.addEventListener('pulses', function(e) {
                    console.log("pulses", e.data);
                    document.getElementById("pulses").innerHTML = e.data;
                }, false);

                source.addEventListener('current', function(e) {
                    console.log("current", e.data);
                    document.getElementById("current").innerHTML = e.data;
                }, false);

                source.addEventListener('thermocouple', function(e) {
                    console.log("thermocouple", e.data);
                    document.getElementById("thermocouple").innerHTML = e.data;
                }, false);

                source.addEventListener('airflow', function(e) {
                    console.log("airflow", e.data);
                    document.getElementById("airflow").innerHTML = e.data;
                }, false);

                source.addEventListener('mac_gprs', function(e) {
                    console.log("mac_gprs", e.data);
                    document.getElementById("_gprs_mac").innerHTML = e.data;
                }, false);

                source.addEventListener('station_id', function(e) {
                    console.log("station_id", e.data);
                    document.getElementById("stationid").innerHTML = e.data;
                }, false);
            }
        </script>
    </html>
)rawliteral";

void setLocalNetwork() {
    String macAddress = WiFi.macAddress();
    const char* ssid_mac = macAddress.c_str();

    strcpy(soft_ap_ssid, ssid_default);
    strcat(soft_ap_ssid, ssid_mac);

    WiFi.softAP(soft_ap_ssid, nullptr);  // nullptr sets an open Access Point
    Serial.print("Network: ");
    Serial.print(soft_ap_ssid);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html, processor);
    });

    server.on("/setup", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", setup_html, processor_setup);
    });

    server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", status_html, processor_status);
    });

    server.on("/esp_now_mac", HTTP_POST, [](AsyncWebServerRequest *request) {
        int params = request->params();
        for(int i=0; i < params; i++){
            AsyncWebParameter* p = request->getParam(i);
            if(p->isPost()){
                // HTTP POST ssid value
                if (p->name() == PARAM_INPUT_1) {
                    String gprsmac = p->value().c_str();
                    Serial.print("Mac GPRS: ");
                    Serial.println(gprsmac);
                    set_gprs_mac(gprsmac.c_str());
                }

                Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
            }
        }
        request->send_P(200, "text/html", "Restarting");
        ESP.restart();
    });

    server.on("/esp_now_id_station", HTTP_POST, [](AsyncWebServerRequest *request) {
        int params = request->params();
        for(int i=0; i < params; i++){
            AsyncWebParameter* p = request->getParam(i);
            if(p->isPost()){
                if (p->name() == PARAM_INPUT_2) {
                    String idstation = p->value().c_str();
                    Serial.print("ID Station: ");
                    Serial.println(idstation);
                    int intValue = idstation.toInt();
                    set_id_station(intValue);
                }

                Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
            }
        }
        request->send_P(200, "text/html", "Restart");
        ESP.restart();
    });

    server.on("/esp_sampling", HTTP_POST, [](AsyncWebServerRequest *request) {
        int params = request->params();
        for(int i=0; i < params; i++){
            AsyncWebParameter* p = request->getParam(i);
            if(p->isPost()){
                if (p->name() == PARAM_INPUT_3) {
                    String samp = p->value().c_str();
                    Serial.print("Sampling: ");
                    Serial.println(samp);
                    int intValue = samp.toInt();
                    set_sampling(intValue);
                    // preferences.putUInt("sampling", intValue);
                }

                Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
            }
        }
        request->send_P(200, "text/html", "Reseting");
        ESP.restart();
    });

    server.on("/esp_send_time", HTTP_POST, [](AsyncWebServerRequest *request) {
        int params = request->params();
        for(int i=0; i < params; i++){
            AsyncWebParameter* p = request->getParam(i);
            if(p->isPost()){
                if (p->name() == PARAM_INPUT_4) {
                    String sent = p->value().c_str();
                    Serial.print("Time sender: ");
                    Serial.println(sent);
                    int intValue = sent.toInt();
                    set_send_time(intValue);
                    // set_sampling(intValue);
                    // preferences.putUInt("sampling", intValue);
                }

                Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
            }
        }
        request->send_P(200, "text/html", "Reseting");
        ESP.restart();
    });

    // pulse_selector
    server.on("/pulse_selector", HTTP_POST, [](AsyncWebServerRequest *request) {
        int params = request->params();
        for(int i=0; i < params; i++){
            AsyncWebParameter* p = request->getParam(i);
            if(p->isPost()){
                if (p->name() == PARAM_INPUT_5) {
                    String sent = p->value().c_str();
                    Serial.print("Pulse Selector: ");
                    Serial.println(sent);
                    int intValue = sent.toInt();
                    set_gprs_mac(intValue);
                }

                Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
            }
        }
        request->send_P(200, "text/html", "Reseting");
        ESP.restart();
    });

    server.on("/firmware_version", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", "Version 1.0");
    });

    // Handle Web Server Events
    events.onConnect([](AsyncEventSourceClient *client){
        if(client->lastId()){
            Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
        }
        client->send("hello!", NULL, millis(), 10000);
    });

    server.addHandler(&events);
    server.begin();
}