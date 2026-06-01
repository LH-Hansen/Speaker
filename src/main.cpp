#include <Arduino.h>

#include "driver/WifiApServer.h"
#include "controller/AudioController.h"
#include "controller/WebController.h"

WifiApServer wifiServer(
    "UNO-R4-AP",
    "12345678"
);

AudioController audioController;
WebController webController(audioController);

void setup()
{
    Serial.begin(115200);

    audioController.begin();
    wifiServer.begin();
}

void loop()
{
    audioController.update();

    WiFiClient client = wifiServer.available();

    if (client)
    {
        webController.handleClient(client);
    }
}