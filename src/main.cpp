#include <Arduino.h>
//#include "secrets.h"
#include "display_manager.h"
#include "settings.h"
#include "web_config.h"
#include "obs_client.h"
#include "matrix.h"
#include "version.h"
void setup()
{
   Serial.begin(115200);


loadSettings();


 displayBegin();
matrixBegin();

delay(1000);

    obsBegin();
    webConfigBegin();

}
void loop()
{
   webConfigLoop(); 
    matrixUpdate();
obsLoop();



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
