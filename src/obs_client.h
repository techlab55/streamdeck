#pragma once
#include <Arduino.h>

extern String obsScenes[32];
extern int obsSceneCount;
extern String sceneSources[32];
extern int sceneSourceCount;

void obsBegin();
void obsLoop();
void requestSceneList();
void sceneCamera1();
void sceneCamera2();
void sceneCamera3();
void sceneArduino();
void sceneSchermo();
void sceneMicroscopio();

void toggleMute();
void toggleRecording();
void togglePauseRecording();
void toggleStreaming();
void toggleSource(String sourceName);
const char* getCurrentScene();

bool isObsConnected();
bool isRecording();
bool isRecordPaused();