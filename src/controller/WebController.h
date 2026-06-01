#pragma once

#include <Arduino.h>
#include <WiFiS3.h>

#include "../generated/WebAssets.h"

class WebController
{
public:
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

                Serial.println(requestLine);

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
        else
        {
            sendResponse(client, "text/html", INDEX_HTML);
        }

        client.stop();
    }

private:
    void handleSliderRequest(WiFiClient& client, const String& requestLine)
    {
        int id = getQueryInt(requestLine, "id");
        int value = getQueryInt(requestLine, "value");

        Serial.print("Slider ");
        Serial.print(id);
        Serial.print(": ");
        Serial.println(value);

        sendResponse(client, "text/plain", "OK");
    }

    int getQueryInt(const String& requestLine, const String& key)
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

    void sendResponse(WiFiClient& client,
                      const char* contentType,
                      const char* content)
    {
        client.println("HTTP/1.1 200 OK");

        client.print("Content-Type: ");
        client.println(contentType);

        client.println("Connection: close");
        client.println();

        client.print(content);
    }
};