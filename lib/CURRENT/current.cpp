#include <Arduino.h>

int CT = GPIO_NUM_34;

int get_CT() {
    return analogRead(CT);
}