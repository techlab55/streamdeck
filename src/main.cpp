#include <Arduino.h>
//#include "secrets.h"
#include "display_manager.h"
#include "settings.h"
#include "web_config.h"
#include "obs_client.h"
#include "matrix.h"
#include "version.h"
#include <WiFi.h>
#include <led_status.h>

unsigned long showIpUntil = 0;
void setup()
{
   Serial.begin(115200);


loadSettings();


 displayBegin();
matrixBegin();

delay(1000);

    obsBegin();
    webConfigBegin();
ledBegin();
}
void loop()
{
    if(millis() < showIpUntil)
{
   // displayShowIP(WiFi.localIP().toString());
}
else
{
    displayUpdate(
        getCurrentScene(),
        isRecording(),
        isRecordPaused(),
        isObsConnected(),
        isSourceMode());
}
   webConfigLoop(); 
    matrixUpdate();
obsLoop();

ledUpdate(
    isRecording(),
    isRecordPaused());

    static unsigned long t = 0;

    if(millis() - t > 500)
    {
        t = millis();

        displayUpdate(
    getCurrentScene(),
    isRecording(),
    isRecordPaused(),
    isObsConnected(),
    isSourceMode());
    }
}
