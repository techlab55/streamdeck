#include "settings.h"
#include <Preferences.h>
#include "secrets.h"
#include "dati.h"

Preferences prefs;

AppConfig config;

void loadDefaults()
{
    config.wifiSSID = WIFI_SSID;
    config.wifiPass = WIFI_PASSWORD;

    config.obsPort = OBS_PORT;

    config.audioInput = DEFAULT_AUDIO;
    config.sourceScene = DEFAULT_SOURCE_SCENE;

    for(uint8_t i = 0; i < NUM_KEYS; i++)
    {
        config.scenes[i]     = DEFAULT_SCENES[i];
        config.sourceKeys[i] = DEFAULT_SOURCES[i];
    }
}

void saveObsIPOnly(const String& ip)
{
    prefs.begin("streamdeck", false);

    prefs.putString("obsip", ip);

    prefs.end();

    config.obsIP = ip;

    Serial.print("OBS IP salvato: ");
    Serial.println(ip);
     delay(500);
    ESP.restart();
}

void saveSettings()
{
    prefs.begin("streamdeck", false);

    prefs.putString("ssid", config.wifiSSID);
    prefs.putString("pass", config.wifiPass);

    prefs.putString("obsip", config.obsIP);
    prefs.putUShort("obsport", config.obsPort);

    prefs.putString("audio", config.audioInput);
    prefs.putString("sourceScene", config.sourceScene);

    for (uint8_t i = 0; i < NUM_KEYS; i++)
    {
        prefs.putString(PREF_SCENES[i], config.scenes[i]);
        prefs.putString(PREF_SOURCES[i], config.sourceKeys[i]);
        
    }
Serial.println("=== VERIFICA SALVATAGGIO ===");

for(uint8_t i=0;i<NUM_KEYS;i++)
{
    Serial.print(PREF_SOURCES[i]);
    Serial.print(" -> ");
    Serial.println(prefs.getString(PREF_SOURCES[i], "<vuoto>"));
}
    prefs.end();
}

void loadSettings()
{
    prefs.begin("streamdeck", true);

    bool firstBoot = !prefs.isKey("obsip");

    if (firstBoot)
    {
        prefs.end();
        loadDefaults();
        return;
    }

    config.wifiSSID = prefs.getString("ssid", WIFI_SSID);
    config.wifiPass = prefs.getString("pass", WIFI_PASSWORD);

    config.obsIP = prefs.getString("obsip", "");
    config.obsPort = prefs.getUShort("obsport", OBS_PORT);

    config.audioInput = prefs.getString("audio", "micv8s");
   config.sourceScene = prefs.getString("sourceScene", "mix1");

    for (uint8_t i = 0; i < NUM_KEYS; i++)
    {
       config.scenes[i] =
    //prefs.getString(PREF_SCENES[i], "");
    prefs.getString(PREF_SCENES[i], DEFAULT_SCENES[i]);
        config.sourceKeys[i] =
    //prefs.getString(PREF_SOURCES[i], "");
    prefs.getString(PREF_SOURCES[i], DEFAULT_SOURCES[i]);
    }

    prefs.end();
}
void printConfig()
{
    Serial.println("\n===== CONFIG =====");

    Serial.println("WiFi : " + config.wifiSSID);
    Serial.println("OBS  : " + config.obsIP);
    Serial.println("Port : " + String(config.obsPort));

    for (uint8_t i = 0; i < NUM_KEYS; i++)
    {
        Serial.printf("Key %d Scene  : %s\n",
                      i + 1,
                      config.scenes[i].c_str());

        Serial.printf("Key %d Source : %s\n",
                      i + 1,
                      config.sourceKeys[i].c_str());
    }

    Serial.println("==================");
}
