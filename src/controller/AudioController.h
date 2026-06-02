#pragma once

#include <Arduino.h>
#include "../driver/ServoDriver.h"

class AudioController
{
public:
    void begin();
    void update();

    void setWebValue(int id, int value);

    int getMic() const;
    int getValue(int id) const;

private:
    static const int CHANNEL_COUNT = 4;

    static const int MIC = 0;
    static const int TREBLE = 1;
    static const int BASS = 2;
    static const int VOLUME = 3;

    static const int TRUE_MAX_DEGREES = 270;
    static const int SERVO_MAX_DEGREES = 180;

    static const int _potPins[CHANNEL_COUNT];

    ServoDriver _servos[CHANNEL_COUNT] = {
        ServoDriver(9,  TRUE_MAX_DEGREES, SERVO_MAX_DEGREES),
        ServoDriver(10, TRUE_MAX_DEGREES, SERVO_MAX_DEGREES),
        ServoDriver(11, TRUE_MAX_DEGREES, SERVO_MAX_DEGREES),
        ServoDriver(12, TRUE_MAX_DEGREES, SERVO_MAX_DEGREES)
    };

    int _potValues[CHANNEL_COUNT] = {0, 0, 0, 0};
    int _webValues[CHANNEL_COUNT] = {0, 0, 0, 0};
    int _targetValues[CHANNEL_COUNT] = {0, 0, 0, 0};

    const int _threshold = 10;
    const char* _lastSource[CHANNEL_COUNT] = {"none", "none", "none", "none"};

    int readPotValue(int id);
    void applyTargetValue(int id, const char* source);
    void printState(int id);
};