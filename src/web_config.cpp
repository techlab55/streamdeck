#include "web_config.h"
#include "settings.h"
#include "obs_client.h"
#include "display_manager.h"
#include <WiFi.h>
#include <WebServer.h>
#include <version.h>
#include <dati.h>

WebServer server(80);

void webConfigLoop()
{
    server.handleClient();
}
void startConfigAP()
{
    Serial.println("START AP");

   WiFi.mode(WIFI_AP_STA);

    WiFi.softAP(
        "ESP32-STREAMDECK",
        "12345678");

    Serial.println(
        WiFi.softAPIP());
      
displayUpdate(
    "CONFIG WIFI",
    false,
    false,
    false,
    false);
    
}
String makeSceneSelect(
    const char* field,
    const String& current)
{
    String html;

    html += "<select name='";
    html += field;
    html += "'>";

    for(int i=0;i<obsSceneCount;i++)
    {
        html += "<option";

        if(obsScenes[i] == current)
            html += " selected";

        html += ">";

        html += obsScenes[i];

        html += "</option>";
    }
if(obsSceneCount == 0)
{
    html += "<option>NESSUNA SCENA</option>";
}
    html += "</select>";

    return html;
}


String makePage()
{
    String html;

    html += "<html><head>";
    html += "<meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width'>";
    html += "<title>ESP32 StreamDeck</title>";
    html += R"rawliteral(
<style>
body{
    background:#1e1e1e;
    color:#e0e0e0;
    font-family:Arial,sans-serif;
    margin:15px;
}

h2,h3{
    color:#4fc3f7;
}

.card{
    background:#2b2b2b;
    padding:15px;
    border-radius:10px;
    margin-bottom:15px;
    box-shadow:0 0 8px rgba(0,0,0,0.4);
}

input,select{
    width:100%;
    padding:10px;
    margin-top:5px;
    margin-bottom:10px;
    background:#333;
    color:white;
    border:1px solid #555;
    border-radius:6px;
    box-sizing:border-box;
}

input[type=submit],
input[type=button],
button{
    background:#1976d2;
    color:white;
    border:none;
    border-radius:6px;
    padding:12px;
    margin-top:5px;
    cursor:pointer;
}

input[type=submit]:hover,
input[type=button]:hover,
button:hover{
    background:#2196f3;
}

.status{
    background:#1b5e20;
    color:white;
    padding:10px;
    border-radius:8px;
    text-align:center;
    margin-bottom:15px;
}

hr{
    border:0;
    height:1px;
    background:#555;
}
</style>
)rawliteral";
    html += "</head><body>";

    //html += "<h2>ESP32 OBS StreamDeck</h2>";
html += "<div class='status'>";
html += "<b>ESP32 OBS StreamDeck</b><br>";
html += "Firmware ";
html += FW_VERSION_STRING;
html += "<br>";
html += "IP ESP: ";
html += WiFi.localIP().toString();
html += "</div>";
html += "<div class='card'>";
html += "<h3>📶 WiFi</h3>";
    html += "<form action='/save' method='post'>";
  
    html += "SSID:<br>";
    html += "<input name='ssid' value='" + config.wifiSSID + "'><br><br>";
html += "Password:<br>";
html += "<input type='password' id='pass' name='pass' value='" +
        config.wifiPass +
        "'><br><br>";


html += "<button type='button' onclick='togglePass()'>&#128065;</button>";

html += R"rawliteral(
<script>
function togglePass()
{
  var p = document.getElementById('pass');

  if(p.type === 'password')
      p.type = 'text';
  else
      p.type = 'password';
}
</script>
)rawliteral";
html += "</div>";
html += "<div class='card'>";
html += "<h3>🎥 OBS</h3>";
    html += "OBS IP:<br>";
    html += "<input name='obsip' value='" + config.obsIP + "'>";
    html += "<input type='button' value='Salva solo OBS IP'onclick='saveObsIp()'>";

    html += R"rawliteral(
    <script>
async function saveObsIp()
{
    let obsip =
        document.querySelector(
            "input[name='obsip']"
        ).value;

    await fetch('/saveobsip', {
        method: 'POST',
        headers: {
            'Content-Type':
                'application/x-www-form-urlencoded'
        },
        body: 'obsip=' +
              encodeURIComponent(obsip)
    });

    alert('OBS IP salvato');
}
</script>

)rawliteral";
html += "       Versione: " + String(FW_VERSION_STRING) + "<br><br>";
    html += "OBS Port:<br>";
    html += "<input name='obsport' value='" + String(config.obsPort) + "'>";
   html += "<input type='button' value='Ripristina Default' onclick=\"window.location='/defaults'\">";
html += "<br><br>";
    html += "Audio Input:<br>";
    html += "<input name='audio' value='" + config.audioInput + "'><br><br>";
  html += "</div>";
html += "<div class='card'>";
html += "<h3>🎬 Scene</h3>";
 
for(uint8_t i=0;i<NUM_KEYS;i++)
{
    html += "Scena ";
    html += String(i+1);
    html += ":<br>";

    html += makeSceneSelect(
        PREF_SCENES[i],
        config.scenes[i]);

    html += "<br><br>";
}
html += "</div>";
html += "<hr>";
html += "<div class='card'>";
html += "<h3>🎛 Modalità Sources</h3>";
//html += "<h3>Modalita Sources</h3>";
html += "Scena:<br>";


html += "<select id='sourceScene' name='sourceScene'>";
for(int i=0;i<obsSceneCount;i++)
{
    html += "<option";

    if(obsScenes[i] == config.sourceScene)
        html += " selected";

    html += ">";

    html += obsScenes[i];
    html += "</option>";
}
html += "</select><br><br>";

for(uint8_t i = 0; i < NUM_KEYS; i++)
{
    html += "Tasto ";
    html += String(i + 1);
    html += ":<br>";

    html += "<select name='";
    html += PREF_SOURCES[i];
    html += "' id='";
    html += PREF_SOURCES[i];
    html += "'></select>";

    html += "<input type='hidden' id='saved";
    html += String(i + 1);
    html += "' value='";
    html += config.sourceKeys[i];
    html += "'>";

    html += "<br><br>";
}

html += "<br>";
html += "<input type='submit' value='💾 SALVA CONFIGURAZIONE'>";
html += "</div>";
  //html += "<input type='submit' value='SALVA'>";
    html += "</form>";
html += R"rawliteral(
<script>

async function loadSources()
{
    let response =
        await fetch('/sources');

    let sources =
        await response.json();

    let ids =
    [   
        'sourceKey1',
        'sourceKey2',
        'sourceKey3',
        'sourceKey4',
        'sourceKey5',
        'sourceKey6'
    ];

    ids.forEach(id =>
{
    let sel = document.getElementById(id);

    let saved =
        document.getElementById(
            'saved' + id.replace('sourceKey','')
        ).value;

    sel.innerHTML = '';

    sources.forEach(src =>
    {
        let opt =
            document.createElement('option');

        opt.value = src;
        opt.text  = src;

        if(src === saved)
            opt.selected = true;

        sel.appendChild(opt);
    });
});
}

window.onload = async function()
{
    await loadSources();

    document.getElementById("sourceScene")
        .addEventListener(
            "change",
            loadSources);
}

</script>
)rawliteral";
    html += "</body></html>";
   
    return html;
}
void handleSources()
{
    String json = "[";

    for(int i=0;i<sceneSourceCount;i++)
    {
        json += "\"";
        json += sceneSources[i];
        json += "\"";

        if(i < sceneSourceCount - 1)
            json += ",";
    }

    json += "]";
Serial.print("FONTI=");
Serial.println(sceneSourceCount);

for(int i=0;i<sceneSourceCount;i++)
{
    Serial.println(sceneSources[i]);
}
    server.send(
        200,
        "application/json",
        json);
}

void handleRoot()
{
    server.send(200, "text/html", makePage());
}

void handleSave()
{
    // WiFi
    config.wifiSSID = server.arg("ssid");
    config.wifiPass = server.arg("pass");

    // OBS
    config.obsIP = server.arg("obsip");
    config.obsPort = server.arg("obsport").toInt();

    // Audio
    config.audioInput = server.arg("audio");

    // Modalità Sources
    config.sourceScene = server.arg("sourceScene");

    // Configurazione tasti
    for(uint8_t i = 0; i < NUM_KEYS; i++)
    {
        config.scenes[i] = server.arg(PREF_SCENES[i]);
        config.sourceKeys[i] = server.arg(PREF_SOURCES[i]);
    }

    Serial.println("=== SOURCES RICEVUTI DAL WEB ===");

    for(uint8_t i = 0; i < NUM_KEYS; i++)
    {
        Serial.printf("sourceKey%d = %s\n",
                      i + 1,
                      config.sourceKeys[i].c_str());
    }

    saveSettings();

    server.send(
        200,
        "text/html",
        "<h2>Salvato</h2><p>Riavvia ESP32</p>");

    delay(1000);
    ESP.restart();
}
void webConfigBegin()
{
    Serial.println("WEB START");
server.on("/defaults", HTTP_GET, []()
{

    // loadDefaults();
    saveSettings();

    server.send(200, "text/plain", "Default caricati");

    delay(1000);
    ESP.restart();
});
server.on("/saveobsip",
          HTTP_POST,
          []()
{
    saveObsIPOnly(
        server.arg("obsip"));

    server.send(
        200,
        "text/plain",
        "OK");
});
    server.on("/", handleRoot);

    server.on("/save",
              HTTP_POST,
              handleSave);

    server.on("/sources",
              handleSources);
         server.on("/scenes", HTTP_GET, []()
{
    String json = "[";

    for(int i=0; i<obsSceneCount; i++)
    {
        if(i) json += ",";

        json += "\"";
        json += obsScenes[i];
        json += "\"";
    }

    json += "]";

    server.send(200, "application/json", json);
});

server.on("/config", HTTP_GET, []()
{
    String json = "{";
    
    json += "\"obsip\":\"" + config.obsIP + "\",";
    json += "\"scene1\":\"" + config.scenes[0] + "\",";
    json += "\"scene2\":\"" + config.scenes[1] + "\",";
    json += "\"scene3\":\"" + config.scenes[2]+ "\",";
    json += "\"scene4\":\"" + config.scenes[3]+ "\",";
    json += "\"scene5\":\"" + config.scenes[4] + "\",";
    json += "\"scene6\":\"" + config.scenes[5] + "\",";

    json += "\"sourceKey1\":\"" + config.sourceKeys[0] + "\",";
    json += "\"sourceKey2\":\"" + config.sourceKeys[1] + "\",";
    json += "\"sourceKey3\":\"" + config.sourceKeys[2] + "\",";
    json += "\"sourceKey4\":\"" + config.sourceKeys[3] + "\",";
    json += "\"sourceKey5\":\"" + config.sourceKeys[4] + "\",";
    json += "\"sourceKey6\":\"" + config.sourceKeys[5]  + "\"";

    json += "}";

    server.send(200, "application/json", json);
});

    server.onNotFound([]()
    {
        Serial.print("404 -> ");
        Serial.println(server.uri());

        server.send(404,
                    "text/plain",
                    "Not found");
    });

    server.begin();

    Serial.println("WEB READY");
}

