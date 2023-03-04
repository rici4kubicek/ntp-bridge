#include <Arduino.h>
#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include "LittleFS.h"
#include "ArduinoJson.h"

typedef struct
{
    char ap[60];
    char password[60];
} UserConfig;

UserConfig config;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
WiFiManager wifiManager;
static bool shouldSaveConfig = false;

// callback notifying us of the need to save config
static void saveWiFiConfigCallback()
{
    shouldSaveConfig = true;
}

static void getConfig(UserConfig *config)
{
    File file = LittleFS.open("/ap-config.json", "r");
    if (!file)
        return;
    StaticJsonDocument<512> doc;

    strlcpy(config->ap, doc["ap"] | DEFAULT_SSID, sizeof(config->ap));
    strlcpy(config->password, doc["password"] | DEFAULT_PASS, sizeof(config->password));

    file.close();
}

void setup()
{
    Serial.begin(115200);
    LittleFS.begin();

    getConfig(&config);

    wifiManager.autoConnect();
    wifiManager.setSaveConfigCallback(saveWiFiConfigCallback);
    WiFi.begin(config.ap, config.password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    timeClient.begin();
}

void loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        timeClient.update();

        Serial.println(timeClient.getFormattedTime());
    }

    delay(1000);
}