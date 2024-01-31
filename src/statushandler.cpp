#include <Arduino.h>
#include "store.h"

void logStatus(String message, String typeOfInf) {
    String logMessage = typeOfInf + message;
    Serial.println(logMessage);
}

