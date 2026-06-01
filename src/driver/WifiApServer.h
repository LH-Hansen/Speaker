#pragma once

#include <Arduino.h>
#include <WiFiS3.h>

class WifiApServer
{
public:
    WifiApServer(const char* ssid,
                 const char* password,
                 uint16_t port = 80)
        : _ssid(ssid),
          _password(password),
          _server(port)
    {
    }

    void begin()
    {
        if (WiFi.beginAP(_ssid, _password) != WL_AP_LISTENING)
        {
            Serial.println("Failed to create AP");

            while (true)
            {
            }
        }

        delay(1000);

        Serial.print("AP IP: ");
        Serial.println(WiFi.localIP());

        _server.begin();
    }

    WiFiClient available()
    {
        return _server.available();
    }

private:
    const char* _ssid;
    const char* _password;
    WiFiServer _server;
};