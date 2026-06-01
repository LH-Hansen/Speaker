#pragma once

#include <Arduino.h>
#include <WiFiS3.h>

#include "../generated/WebAssets.h"
#include "AudioController.h"

class WebController
{
public:
    WebController(AudioController &audioController)
        : _audioController(audioController)
    {
    }

    void handleClient(WiFiClient client)
    {
        if (!client)
            return;

        String requestLine;
        unsigned long timeout = millis() + 50;

        while (client.connected() && millis() < timeout)
        {
            if (client.available())
            {
                requestLine = client.readStringUntil('\n');
                requestLine.trim();

                while (client.available())
                    client.read();

                break;
            }
        }

        if (requestLine.startsWith("GET /style.css"))
        {
            sendResponse(client, "text/css", STYLE_CSS);
        }
        else if (requestLine.startsWith("GET /script.js"))
        {
            sendResponse(client, "application/javascript", SCRIPT_JS);
        }
        else if (requestLine.startsWith("GET /api/slider"))
        {
            handleSliderRequest(client, requestLine);
        }
        else if (requestLine.startsWith("GET /api/audio"))
        {
            handleAudioRequest(client);
        }
        else
        {
            sendResponse(client, "text/html", INDEX_HTML);
        }

        client.stop();
    }

private:
    AudioController &_audioController;

    void handleSliderRequest(WiFiClient &client, const String &requestLine)
    {
        int id = getQueryInt(requestLine, "id");
        int value = getQueryInt(requestLine, "value");

        _audioController.setWebValue(id, value);

        sendResponse(client, "text/plain", "OK");
    }

    void handleAudioRequest(WiFiClient& client)
{
    String json = "{";
    json += "\"mic\":" + String(_audioController.getValue(0)) + ",";
    json += "\"volume\":" + String(_audioController.getValue(1)) + ",";
    json += "\"treble\":" + String(_audioController.getValue(2)) + ",";
    json += "\"bass\":" + String(_audioController.getValue(3));
    json += "}";

    sendResponse(client, "application/json", json);
}

    int getQueryInt(const String &requestLine, const String &key)
    {
        String search = key + "=";

        int start = requestLine.indexOf(search);

        if (start == -1)
            return 0;

        start += search.length();

        int end = requestLine.indexOf("&", start);

        if (end == -1)
            end = requestLine.indexOf(" ", start);

        return requestLine.substring(start, end).toInt();
    }

    void sendResponse(WiFiClient &client,
                      const char *contentType,
                      const String &content)
    {
        client.println("HTTP/1.1 200 OK");
        client.print("Content-Type: ");
        client.println(contentType);
        client.println("Connection: close");
        client.println();
        client.print(content);
    }
};