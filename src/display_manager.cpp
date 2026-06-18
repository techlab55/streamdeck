#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "version.h"


static Adafruit_SSD1306 display(
    128,
    64,
    &Wire,
    -1);

void displayBegin()
{
    Wire.begin(6,7);

    if(!display.begin(
        SSD1306_SWITCHCAPVCC,
        0x3C))
    {
        Serial.println("OLED FAIL");

        while(true);
    }

    display.clearDisplay();
    display.setRotation(2);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(20,0);
    display.println("TECHLAB55");

    display.display();

    Serial.println("OLED OK");
}
void displayShowIP(const char* ip)
{
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);

    display.println("WIFI OK");
    display.println();
    display.println("IP:");
    display.println(ip);

    display.display();
}
void displayUpdate(
    const char* scene,
    bool rec,
    bool pause,
    bool obs,
    bool sourceMode)
{
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);

   display.print("OBS:");
display.print(obs ? "OK" : "OFF");

display.print("  ");

display.println(sourceMode ? "SRC" : "SCN");

    display.println();

    display.print("SCENA:");
    display.println(scene);//sceneName

    display.println();

    display.print("REC:");
    display.println(rec ? "ON" : "OFF");

    display.print("PAUSE:");
    display.println(pause ? "ON" : "OFF");
   

    display.setCursor(70,55);

    display.print("V:");
    display.println(FW_VERSION_STRING);
    display.display();
}