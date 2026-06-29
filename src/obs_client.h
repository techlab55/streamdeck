#pragma once
#include <Arduino.h>

extern String obsScenes[32];
extern int obsSceneCount;
extern String sceneSources[32];
extern int sceneSourceCount;

void obsBegin();
void obsLoop();
void requestSceneList();

void toggleMute();
void toggleRecording();
void togglePauseRecording();
void toggleStreaming();
void toggleSource(String sourceName);
const char* getCurrentScene();
const char* getCurrentSource();
void changeScene(const String& sceneName);
bool isObsConnected();
bool isRecording();
bool isRecordPaused();