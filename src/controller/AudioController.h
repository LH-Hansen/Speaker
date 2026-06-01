#pragma once

#include <Arduino.h>

class AudioController
{
public:
    void begin()
    {
        pinMode(A0, INPUT);
    }

    void update()
    {
        int newPotValue = map(analogRead(A0), 0, 1023, 0, 270);

        if (abs(newPotValue - _potValue) >= _threshold)
        {
            _potValue = newPotValue;
            _targetValue = _potValue;
            _lastSource = "pot";

            printState();
        }
    }

    void setWebValue(int id, int value)
    {
        if (id != 0)
            return;

        value = constrain(value, 0, 270);

        if (abs(value - _webValue) >= _threshold)
        {
            _webValue = value;
            _targetValue = _webValue;
            _lastSource = "web";

            printState();
        }
    }

    int getMic() const
    {
        return _targetValue;
    }

    int getValue(int id) const
    {
        if (id == 0)
            return _targetValue;

        return 135;
    }

private:
    int _potValue = 135;
    int _webValue = 135;
    int _targetValue = 135;

    const int _threshold = 5;

    const char* _lastSource = "none";

    void printState()
    {
        Serial.print("target: ");
        Serial.print(_targetValue);

        Serial.print(" pot: ");
        Serial.print(_potValue);

        Serial.print(" web: ");
        Serial.print(_webValue);

        Serial.print(" source: ");
        Serial.println(_lastSource);
    }
};