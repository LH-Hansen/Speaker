#include <Arduino.h>

#include "driver/WifiApServer.h"
#include "controller/WebController.h"

WifiApServer wifiServer(
    "UNO-R4-AP",
    "12345678"
);

WebController webController;

void setup()
{
    Serial.begin(115200);
    wifiServer.begin();
}

void loop()
{
    WiFiClient client = wifiServer.available();

    if (client)
    {
        webController.handleClient(client);
    }
}