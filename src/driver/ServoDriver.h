#pragma once

#include <Arduino.h>
#include <Servo.h>

class ServoDriver
{
public:
    ServoDriver(int pin, int trueMaxDegrees, int servoMaxDegrees)
        : _pin(pin),
          _trueMaxDegrees(trueMaxDegrees),
          _servoMaxDegrees(servoMaxDegrees)
    {
    }

    void begin()
    {
        _servo.attach(_pin);
    }

    void writeTrueValue(int value)
    {
        value = constrain(value, 0, _trueMaxDegrees);

        int servoAngle = map(value, 0, _trueMaxDegrees, 0, _servoMaxDegrees);

        _servo.write(servoAngle);
    }

private:
    Servo _servo;

    int _pin;
    int _trueMaxDegrees;
    int _servoMaxDegrees;
};