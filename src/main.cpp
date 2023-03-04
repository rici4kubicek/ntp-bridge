#include <Arduino.h>
#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>

const char *ssid = "<SSID>";
const char *password = "<PASSWORD>";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
WiFiManager wifiManager;
static bool shouldSaveConfig = false;

// callback notifying us of the need to save config
static void saveWiFiConfigCallback()
{
    shouldSaveConfig = true;
}

void setup()
{
    Serial.begin(115200);
    wifiManager.autoConnect();
    wifiManager.setSaveConfigCallback(saveWiFiConfigCallback);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    timeClient.begin();
}

void loop()
{
    timeClient.update();

    Serial.println(timeClient.getFormattedTime());

    delay(1000);
}