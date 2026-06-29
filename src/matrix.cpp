#include <Arduino.h>
#include "obs_client.h"
#include "settings.h"

static bool keyHandled[3][3];
static const uint8_t rows[3] = {0, 1, 2};
static const uint8_t cols[3] = {3, 4, 5};

static bool lastState[3][3];
static uint32_t lastTime[3][3];
bool sourceMode = false;
const uint32_t DEBOUNCE_MS = 50;

void executeKey(uint8_t key)
{
    if(key >= 1 && key <= NUM_KEYS)
    {
        uint8_t idx = key - 1;

        if(sourceMode)
        {
            Serial.printf("KEY %d SOURCE -> %s\n",
                          key,
                          config.sourceKeys[idx].c_str());

            toggleSource(config.sourceKeys[idx]);
        }
        else
        {
            Serial.printf("KEY %d SCENE -> %s\n",
                          key,
                          config.scenes[idx].c_str());

            changeScene(config.scenes[idx]);
        }

        return;
    }

    switch(key)
    {
        case 7:
            sourceMode = !sourceMode;

            Serial.println(sourceMode ? "MODE = SOURCES"
                                      : "MODE = SCENES");
            break;

        case 8:
            togglePauseRecording();
            break;

        case 9:
            toggleRecording();
            break;
    }
}
void matrixBegin()
{
    memset(keyHandled, 0, sizeof(keyHandled));
    for(int r=0; r<3; r++)
    {
        pinMode(rows[r], OUTPUT);
        digitalWrite(rows[r], HIGH);
    }

    for(int c=0; c<3; c++)
    {
        pinMode(cols[c], INPUT_PULLUP);
    }

    memset(lastState, 0, sizeof(lastState));
    memset(lastTime, 0, sizeof(lastTime));
}
bool isSourceMode()
{
    return sourceMode;
}

void matrixUpdate()
{
    for(int r=0; r<3; r++)
    {
        digitalWrite(rows[r], LOW);

        delayMicroseconds(20);

        for(int c=0; c<3; c++)
        {
            bool pressed =
                (digitalRead(cols[c]) == LOW);
                if(pressed)
{
    //Serial.printf("R=%d C=%d\n", r, c);
}

if(pressed != lastState[r][c])
{
    lastTime[r][c] = millis();
    lastState[r][c] = pressed;

    if(!pressed)
    {
        
        // il tasto è stato rilasciato,
        // quindi può essere premuto di nuovo
        keyHandled[r][c] = false;
    }
}


if(pressed &&
   !keyHandled[r][c] &&
   millis() - lastTime[r][c] >= DEBOUNCE_MS)
{
    executeKey(r * 3 + c + 1);
    keyHandled[r][c] = true;
}
}
      digitalWrite(rows[r], HIGH);
}
}