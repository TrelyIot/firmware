#include <Arduino.h>
#include <Preferences.h>
Preferences preferences;

String gprs_mac;
unsigned int id_station;
unsigned int sampling;
unsigned int send_time;
int pulse_mode;

void set_gprs_mac(int pulseMode) {
    preferences.putUInt("pulse_mode", pulseMode);
}

void set_gprs_mac(String mac_gprs) {
    preferences.putString("gprs_mac", mac_gprs);
}

void set_id_station(int id) {
    preferences.putUInt("id_station", id);
}

void set_sampling(int samp) {
    preferences.putUInt("sampling", samp);
}

void set_send_time(int send) {
    preferences.putUInt("send_time", send);
}

unsigned int get_pulse_mode() {
    return preferences.getUInt("pulse_mode", 0);
}

String get_gprs_mac() {
    return preferences.getString("gprs_mac", "");
}

unsigned int get_id_station() {
    return preferences.getUInt("id_station", 0);
}

unsigned int get_sampling() {
    return preferences.getUInt("sampling", 0);
}

unsigned int get_send_time() {
    return preferences.getUInt("send_time", 0);
}

void init_vars() {
    if(preferences.isKey("gprs_mac")) gprs_mac = get_gprs_mac();
    if(preferences.isKey("id_station")) id_station = get_id_station();
    if(preferences.isKey("sampling")) sampling = get_sampling();
    if(preferences.isKey("send_time")) send_time = get_send_time();
    if(preferences.isKey("pulse_mode")) pulse_mode = get_pulse_mode();
}

void store_begin() {
    preferences.begin("device-store", false);
    init_vars(); 
}