#pragma once

#include <Arduino.h>

// Numero di tasti fisici dello Stream Deck
constexpr uint8_t NUM_KEYS = 6;


struct AppConfig
{
    // WiFi
    String wifiSSID;
    String wifiPass;

    // OBS
    String obsIP;
    uint16_t obsPort;

    // Audio
    String audioInput;

    // Modalità Sources
    String sourceScene;

   // Azione associata ai 6 pulsanti
    String scenes[NUM_KEYS];
    String sourceKeys[NUM_KEYS];
};

extern AppConfig config;
extern bool sourceMode;

void printConfig();
void loadSettings();
void saveSettings();
void loadDefaults();
void saveObsIPOnly(const String& ip);