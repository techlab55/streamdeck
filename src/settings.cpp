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
    config.obsIP = OBS_IP;
   
    config.sourceScene = SOURCE_SCENE;
    config.obsPort = 4455;
    
    config.audioInput = "micv8s";
    config.scene1 = SCENE1;
    config.scene2 = SCENE2;
    config.scene3 = SCENE3;
    config.scene4 = SCENE4;
    config.scene5 = SCENE5;
    config.scene6 =  SCENE6;
      config.sourceKey1 = SOURCE_KEY1;      
      config.sourceKey2 = SOURCE_KEY2;
    config.sourceKey3 = SOURCE_KEY3;
    config.sourceKey4 = SOURCE_KEY4;
    config.sourceKey5 = SOURCE_KEY5;
    config.sourceKey6 = SOURCE_KEY6;
}

void saveSettings()
{
    delay(1000);
    prefs.begin("streamdeck", false);

    prefs.putString("ssid", config.wifiSSID);
    prefs.putString("pass", config.wifiPass);

    prefs.putString("obsip", config.obsIP);
    prefs.putUShort("obsport", config.obsPort);

    prefs.putString("audio", config.audioInput);
    prefs.putString("scene1", config.scene1);
    prefs.putString("scene2", config.scene2);
    prefs.putString("scene3", config.scene3);
    prefs.putString("scene4", config.scene4);
    prefs.putString("scene5", config.scene5);
    prefs.putString("scene6", config.scene6);
prefs.putString(
    "sourceKey1",
    config.sourceKey1);

prefs.putString(
    "sourceKey2",
    config.sourceKey2);

prefs.putString(
    "sourceKey3",
    config.sourceKey3);

prefs.putString(
    "sourceKey4",
    config.sourceKey4);

prefs.putString(
    "sourceKey5",
    config.sourceKey5);

prefs.putString(
    "sourceKey6",
    config.sourceKey6);

prefs.putString(
    "sourceScene",
    config.sourceScene);
    prefs.end();
   
}

void loadSettings()
{
     Serial.println(">>> SONO ENTRATO IN loadSettings()");

    prefs.begin("streamdeck", true);
   

    config.wifiSSID =
        prefs.getString("ssid", "");

    config.wifiPass =
        prefs.getString("pass", "");

    config.obsIP =
        prefs.getString("obsip", "");

    config.obsPort =
        prefs.getUShort("obsport", 4455);

    config.audioInput =
        prefs.getString("audio", "lav");

    config.scene1 =
        prefs.getString("scene1", "camera1");

    config.scene2 =
        prefs.getString("scene2", "camera2");

    config.scene3 =
        prefs.getString("scene3", "camera3");

    config.scene4 =
        prefs.getString("scene4", "arduino");

    config.scene5 =
        prefs.getString("scene5", "schermo");

    config.scene6 =
        prefs.getString("scene6", "microscopio");

        config.sourceKey1 =
        prefs.getString("sourceKey1", "");

    config.sourceKey2 =
        prefs.getString("sourceKey2", "");

    config.sourceKey3 =
        prefs.getString("sourceKey3", "");

    config.sourceKey4 =
        prefs.getString("sourceKey4", "");

    config.sourceKey5 =
        prefs.getString("sourceKey5", "");

    config.sourceKey6 =
        prefs.getString("sourceKey6", "");

    // MANCAVA
    config.sourceScene =
        prefs.getString("sourceScene", "");

    prefs.end();
    

}