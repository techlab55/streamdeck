#include <Arduino.h>
#include <WiFi.h>
#include "obs_client.h"
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>
#include "settings.h"
#include "web_config.h"
#include "display_manager.h"
static JsonObject makeRequest(
    JsonDocument& doc,
    const char* requestType,
    const char* requestId)
{
    doc["op"] = 6;

    JsonObject d = doc["d"].to<JsonObject>();

    d["requestType"] = requestType;
    d["requestId"] = requestId;

    return d["requestData"].to<JsonObject>();
}



static String sourceToToggle = "";

using namespace websockets;
static String currentScene = "BOOT";
static bool obsConnected = false;
static bool recording = false;
static bool recordPaused = false;
static WebsocketsClient client;
static bool obsReady = false;
String obsScenes[32];
int obsSceneCount = 0;
String sceneSources[32];
int sceneSourceCount = 0;
static String currentSource = "";
void loadDefaults();
static void sendIdentify()
{
    JsonDocument doc;

    doc["op"] = 1;

    JsonObject d = doc["d"].to<JsonObject>();
    d["rpcVersion"] = 1;

    String payload;
    serializeJson(doc, payload);

    client.send(payload);

    Serial.println("Identify inviato");
}

void toggleSource(String sourceName)
{
    sourceToToggle = sourceName;
currentSource = sourceName;
    JsonDocument doc;

    doc["op"] = 6;

    JsonObject d =
        doc["d"].to<JsonObject>();

    d["requestType"] = "GetSceneItemList";
    d["requestId"] = "items";

    JsonObject req =
        d["requestData"].to<JsonObject>();

    req["sceneName"] = currentScene;

    String payload;
    serializeJson(doc, payload);

    client.send(payload);

   
}
void changeScene(const String& sceneName)
{
     currentScene = sceneName;
   JsonDocument doc;

JsonObject req =
    makeRequest(
        doc,
        "SetCurrentProgramScene",
        "scene");

req["sceneName"] = sceneName;

String payload;
serializeJson(doc, payload);

client.send(payload);
}
void requestSceneList()
{
 JsonDocument doc;

makeRequest(
    doc,
    "GetSceneList",
    "scenelist");

String payload;
serializeJson(doc, payload);

client.send(payload);

Serial.println("Richiesta SceneList");
}
/*
void requestSceneItems(String sceneName)
{
    JsonDocument doc;

    doc["op"] = 6;

    JsonObject d =
        doc["d"].to<JsonObject>();

    d["requestType"] = "GetSceneItemList";
    d["requestId"] = "sourcelist";

    JsonObject req =
        d["requestData"].to<JsonObject>();

    req["sceneName"] = sceneName;

    String payload;
    serializeJson(doc, payload);

    client.send(payload);

    Serial.print("Richiesta fonti di ");
    Serial.println(sceneName);
}*/
void requestSceneItems(const String& scene)
{
    JsonDocument doc;

    JsonObject req =
        makeRequest(
            doc,
            "GetSceneItemList",
            "sourcelist");

    req["sceneName"] = scene;

    String payload;
    serializeJson(doc, payload);

    client.send(payload);
}
void onMessage(WebsocketsMessage message)
{


    JsonDocument doc;

    if (deserializeJson(doc, message.data()))
    {
        Serial.println("JSON ERROR");
        return;
    }

    int op = doc["op"] | -1;

    Serial.print("OP=");
    Serial.println(op);

    if (op == 0)
    {
        Serial.println("HELLO");
        sendIdentify();
    }
    if (op == 2)
{
    Serial.println("IDENTIFIED");
    obsReady = true;
    obsConnected = true;
    
requestSceneList();
requestSceneItems("mix1");
   
    JsonDocument req;
    req["op"] = 6;
    JsonObject d = req["d"].to<JsonObject>();
    d["requestType"] = "GetCurrentProgramScene";
    d["requestId"]   = "getScene";

    String payload;
    serializeJson(req, payload);
    client.send(payload);
}



    if(op == 5)
{
    const char* eventType =
        doc["d"]["eventType"];

    if(strcmp(eventType,
              "RecordStateChanged") == 0)
    {
            const char* state =
        doc["d"]["eventData"]["outputState"];

    Serial.print("STATE=");
    Serial.println(state);

    recording = false;
    recordPaused = false;

    if(strcmp(state,
              "OBS_WEBSOCKET_OUTPUT_STARTED") == 0)
    {
        recording = true;
    }

    if(strcmp(state,
              "OBS_WEBSOCKET_OUTPUT_PAUSED") == 0)
    {
        recording = true;
        recordPaused = true;
    }

    if(strcmp(state,
              "OBS_WEBSOCKET_OUTPUT_RESUMED") == 0)
    {
        recording = true;
        recordPaused = false;
    }

    Serial.printf(
        "REC=%d PAUSE=%d\n",
        recording,
        recordPaused);


        }// Evento: scena cambiata (da OBS o da noi)
//if (op == 5)
      else if(strcmp(eventType,
                   "CurrentProgramSceneChanged") == 0)
    {
          const char* name = doc["d"]["eventData"]["sceneName"];
        if (name) {
            currentScene = name;
            Serial.print("SCENA -> ");
            Serial.println(currentScene);
        }
    }
}

// Risposta a una request (es. GetCurrentProgramScene)
if(op == 7)
{
    String requestId =
        doc["d"]["requestId"] | "";

    // =====================
    // SCENE LIST
    // =====================

    if(requestId == "scenelist")
    {
        obsSceneCount = 0;

        JsonArray scenes =
            doc["d"]["responseData"]["scenes"];

        for(JsonObject scene : scenes)
        {
            String name =
                scene["sceneName"].as<String>();

           if(obsSceneCount < 32)
{
    obsScenes[obsSceneCount++] = name;
}

            Serial.print("SCENA=");
            Serial.println(name);
        }

        Serial.print("TOT=");
        Serial.println(obsSceneCount);
    }

    // =====================
    // CURRENT SCENE
    // =====================

   else if(requestId == "getScene")
    {
        const char* name =
            doc["d"]["responseData"]
               ["currentProgramSceneName"];

        if(name)
        {
            currentScene = name;

            Serial.print("SCENA INIT -> ");
            Serial.println(currentScene);
        }
    }
else if(requestId == "sourcelist")
{
    sceneSourceCount = 0;

    JsonArray items =
        doc["d"]["responseData"]["sceneItems"];

    for(JsonObject item : items)
    {
        String name =
            item["sourceName"].as<String>();

        if(sceneSourceCount < 32)
        {
            sceneSources[sceneSourceCount++] =
                name;
        }

      
    }

    
}
    // =====================
    // TOGGLE SOURCE
    // =====================

    else if(requestId == "items")
    {
        JsonArray sceneItems =
            doc["d"]["responseData"]["sceneItems"];

        for(JsonObject item : sceneItems)
        {
            String name =
                item["sourceName"].as<String>();

            if(name == sourceToToggle)
            {
                int itemId =
                    item["sceneItemId"];

                bool enabled =
                    item["sceneItemEnabled"];

                JsonDocument cmd;

                cmd["op"] = 6;

                JsonObject d2 =
                    cmd["d"].to<JsonObject>();

                d2["requestType"] =
                    "SetSceneItemEnabled";

                d2["requestId"] =
                    "toggle";

                JsonObject req2 =
                    d2["requestData"].to<JsonObject>();

                req2["sceneName"] =
                    currentScene;

                req2["sceneItemId"] =
                    itemId;

                req2["sceneItemEnabled"] =
                    !enabled;

                String payload;
                serializeJson(cmd, payload);

                client.send(payload);

                break;
            }
        }
    }
}
}

void obsBegin()
{
     Serial.print("Connessione a: ");
  
WiFi.begin(
    config.wifiSSID.c_str(),
    config.wifiPass.c_str());
  
   //loadDefaults();
  int tentativi = 0;
  const int max_tentativi = 50; // Numero massimo di tentativi (circa 10 secondi)

   while (WiFi.status() != WL_CONNECTED && tentativi < max_tentativi) {
    delay(500);
    Serial.print(".");
    tentativi++;
  }
   //Verifica l'esito della connessione
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[OK] WiFi connesso con successo!");
    Serial.print("Indirizzo IP: ");
    //Serial.println(WiFi.localIP());
    displayShowIP(
    WiFi.localIP().toString().c_str());

delay(2000);
  } else {
    Serial.println("\n[ERRORE] WiFi non disponibile");

    startConfigAP();

    return;
}
 
   

    client.onMessage(onMessage);
String url =
    String("ws://") +
    config.obsIP +
    ":" +
    String(config.obsPort);
    

    Serial.print("OBS -> ");
    Serial.println(url);

    if(client.connect(url))
    {
        
        Serial.println("OBS Connected");
        
    }
    else
    {
        Serial.println("OBS Failed");
    }
}

void obsLoop()
{
    client.poll();
}


void keyPressed(uint8_t key)
{
    if(sourceMode)
        toggleSource(config.sourceKeys[key]);
    else
        changeScene(config.scenes[key].c_str());
}
void toggleMute()
{
    JsonDocument doc;

    doc["op"] = 6;

    JsonObject d = doc["d"].to<JsonObject>();

    d["requestType"] = "ToggleInputMute";
    d["requestId"] = "mute";

    JsonObject req =
        d["requestData"].to<JsonObject>();
    req["inputName"] =
       config.audioInput;
    
        

    String payload;
    serializeJson(doc, payload);

    client.send(payload);

    Serial.println("Toggle Mute");
}

void toggleRecording()
{
    JsonDocument doc;

    doc["op"] = 6;

    JsonObject d = doc["d"].to<JsonObject>();

    d["requestType"] = "ToggleRecord";
    d["requestId"] = "record";

    String payload;
    serializeJson(doc, payload);

    client.send(payload);

    Serial.println("Toggle Record");
}
void togglePauseRecording()
{
    JsonDocument doc;

    doc["op"] = 6;

    JsonObject d = doc["d"].to<JsonObject>();

    d["requestType"] = "ToggleRecordPause";
    d["requestId"] = "pause";

    String payload;
    serializeJson(doc, payload);

    client.send(payload);

    Serial.println("Toggle Pause");
}
void toggleStreaming()
{
    Serial.println("Streaming");
}
const char* getCurrentScene()

{
    return currentScene.c_str();
}
const char* getCurrentSource()
{
    return currentSource.c_str();
}


bool isObsConnected()
{
    return obsConnected;
}
bool isRecording()
{
    return recording;
}

bool isRecordPaused()
{
    return recordPaused;
}