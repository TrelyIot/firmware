#include <Arduino.h>
#include "store.h"

String logMessage;

String getLogs() {
    return logMessage;
}

void logStatus(String message, String typeOfInf) {
    logMessage = typeOfInf + " " + message;
    Serial.println(logMessage);
}

