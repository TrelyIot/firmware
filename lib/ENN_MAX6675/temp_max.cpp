#include <SPI.h>
#include "max6675.h"

// B8:D6:1A:0E:51:14 -> T2
// B8:D6:1A:0E:50:E4 -> T1
// B8:D6:1A:0E:50:DC -> T5
// B8:D6:1A:0E:51:20 -> T6
// Pin definitions for MAX6675ISA+T

// const int MAX6675_CS_PIN = 17; // chip select
// const int MAX6675_SO_PIN = 19; // data out
// const int MAX6675_SCK_PIN = 5; // clock

// const int MAX6675_CS_PIN = 18; // chip select
// const int MAX6675_SO_PIN = 19; // data out
// const int MAX6675_SCK_PIN = 5; // clock

// const int MAX6675_CS_PIN = 34; // chip select
// const int MAX6675_SO_PIN = 37; // data out
// const int MAX6675_SCK_PIN = 36; // clock

// const int MAX6675_CS_PIN = 37; // chip select
// const int MAX6675_SO_PIN = 35; // data out
// const int MAX6675_SCK_PIN = 36; // clock

// Firebeetle
const int MAX6675_CS_PIN = 23; // chip select
const int MAX6675_SO_PIN = 19; // data out
const int MAX6675_SCK_PIN = 18; // clock

MAX6675 thermocouple(MAX6675_SCK_PIN, MAX6675_CS_PIN, MAX6675_SO_PIN);

float get_temp_c() {
  return thermocouple.readCelsius();
}

float get_temp_f() {
  return thermocouple.readFahrenheit();
}