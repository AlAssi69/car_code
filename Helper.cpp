#include "Arduino.h"

#include <Helper.h>

void Helper::digitalWrites(uint8_t pins[], uint8_t val)
{
    uint8_t arrSize = sizeof(pins) / sizeof(pins[0]);
    for (int i = 0; i < arrSize; i++)
        digitalWrite(pins[i], val);
}

void Helper::pinModes(uint8_t pins[], uint8_t mode)
{
    uint8_t arrSize = sizeof(pins) / sizeof(pins[0]);
    for (int i = 0; i < arrSize; i++)
        pinMode(pins[i], mode);
}

void Helper::plot(String label, float value, bool isLastGraph = false)
{
    Serial.print(label); // May be empty string

    if (label != "")
        Serial.print(": ");

    Serial.print(value);

    if (isLastGraph == false)
        Serial.print(", ");
    else
        Serial.println();
}
