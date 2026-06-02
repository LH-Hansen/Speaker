#include "AudioController.h"

const int AudioController::_potPins[CHANNEL_COUNT] = {
    A0, // Mic
    A1, // Treble
    A2, // Bass
    A3  // Volume
};

void AudioController::begin()
{
    for (int i = 0; i < CHANNEL_COUNT; i++)
    {
        pinMode(_potPins[i], INPUT);
        _servos[i].begin();

        _potValues[i] = readPotValue(i);
        _webValues[i] = _potValues[i];
        _targetValues[i] = _potValues[i];

        applyTargetValue(i, "init");
    }
}

void AudioController::update()
{
    for (int i = 0; i < CHANNEL_COUNT; i++)
    {
        int newPotValue = readPotValue(i);

        if (abs(newPotValue - _potValues[i]) >= _threshold)
        {
            _potValues[i] = newPotValue;
            _targetValues[i] = _potValues[i];

            applyTargetValue(i, "pot");
        }
    }
}

void AudioController::setWebValue(int id, int value)
{
    if (id < 0 || id >= CHANNEL_COUNT)
        return;

    value = constrain(value, 0, TRUE_MAX_DEGREES);

    if (abs(value - _webValues[id]) >= _threshold)
    {
        _webValues[id] = value;
        _targetValues[id] = _webValues[id];

        applyTargetValue(id, "web");
    }
}

int AudioController::getMic() const
{
    return _targetValues[MIC];
}

int AudioController::getValue(int id) const
{
    if (id < 0 || id >= CHANNEL_COUNT)
        return 0;

    return _targetValues[id];
}

int AudioController::readPotValue(int id)
{
    return map(analogRead(_potPins[id]), 0, 1023, 0, TRUE_MAX_DEGREES);
}

void AudioController::applyTargetValue(int id, const char* source)
{
    _lastSource[id] = source;
    _servos[id].writeTrueValue(_targetValues[id]);
    printState(id);
}

void AudioController::printState(int id)
{
    Serial.print("id: ");
    Serial.print(id);

    Serial.print(" target: ");
    Serial.print(_targetValues[id]);

    Serial.print(" pot: ");
    Serial.print(_potValues[id]);

    Serial.print(" web: ");
    Serial.print(_webValues[id]);

    Serial.print(" source: ");
    Serial.println(_lastSource[id]);
}