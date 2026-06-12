#pragma once

#include <Arduino.h>

struct AppConfig
{
    String wifiSSID;
    String wifiPass;

    String obsIP;
    uint16_t obsPort;

    String audioInput;

    String scene1;
    String scene2;
    String scene3;
    String scene4;
    String scene5;
    String scene6;

    String sourceScene;
    String sourceKey1;
    String sourceKey2;
    String sourceKey3;
    String sourceKey4;
    String sourceKey5;
    String sourceKey6;
   
};

extern AppConfig config;

void loadSettings();
void saveSettings();
void loadDefaults();
void saveObsIPOnly(const String& ip);