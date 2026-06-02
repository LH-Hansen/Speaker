#pragma once

#include <Arduino.h>

class Potentiometer
{
public:
    Potentiometer(uint8_t pin)
        : _pin(pin)
    {
        pinMode(_pin, INPUT);
    }

    int read()
    {
        return analogRead(_pin);
    }
private:
    uint8_t _pin;
};