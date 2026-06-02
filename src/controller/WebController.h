#pragma once

#include <Arduino.h>
#include <WiFiS3.h>

#include "AudioController.h"

class WebController
{
public:
    WebController(AudioController &audioController);

    void handleClient(WiFiClient client);

private:
    AudioController &_audioController;

    void handleSliderRequest(WiFiClient &client, const String &requestLine);
    void handleAudioRequest(WiFiClient &client);

    int getQueryInt(const String &requestLine, const String &key);

    void sendResponse(WiFiClient &client,
                      const char *contentType,
                      const String &content);
};