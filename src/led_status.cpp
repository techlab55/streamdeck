#include <Adafruit_NeoPixel.h>

#define LED_PIN 10
#define LED_COUNT 1

Adafruit_NeoPixel pixel(
    LED_COUNT,
    LED_PIN,
    NEO_GRB + NEO_KHZ800);

void ledBegin()
{
    pixel.begin();
    pixel.clear();
    pixel.show();
    pixel.setBrightness(10);
}

void ledUpdate(bool rec, bool pause)
{
    static unsigned long lastBlink = 0;
    static bool blinkState = false;

    if(pause)
    {
        if(millis() - lastBlink > 500)
        {
            lastBlink = millis();
            blinkState = !blinkState;

            if(blinkState)
                pixel.setPixelColor(
                    0,
                    pixel.Color(255,255,0)); // giallo
            else
                pixel.setPixelColor(
                    0,
                    pixel.Color(0,0,0));

            pixel.show();
        }
    }
    else if(rec)
    {
        pixel.setPixelColor(
            0,
            pixel.Color(255,0,0)); // rosso

        pixel.show();
    }
    else
    {
        pixel.setPixelColor(
            0,
            pixel.Color(0,255,0)); // verde

        pixel.show();
    }
}