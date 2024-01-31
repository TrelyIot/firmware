#ifndef STORE_H
#define STORE_H

void store_begin();
void set_gprs_mac(String mac_gprs);
void set_id_station(int id);
void set_sampling(int samp);
void set_send_time(int send);
void set_gprs_mac(int pulseMode);
String get_gprs_mac();
unsigned int get_id_station();
unsigned int get_sampling();
unsigned int get_send_time();
unsigned int get_pulse_mode();
#endif