#include "web_config.h"
#include "settings.h"
#include "obs_client.h"
#include "display_manager.h"
#include <WiFi.h>
#include <WebServer.h>
#include <version.h>


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
    html += "<meta name='viewport' content='width=device-width'>";
    html += "<title>ESP32 StreamDeck</title>";
    html += "</head><body>";

    html += "<h2>ESP32 OBS StreamDeck</h2>";

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

    html += "OBS IP:<br>";
    html += "<input name='obsip' value='" + config.obsIP + "'>";
html += "       Versione: " + String(FW_VERSION_STRING) + "<br><br>";
    html += "OBS Port:<br>";
    html += "<input name='obsport' value='" + String(config.obsPort) + "'>";
   html += "<input type='button' value='Ripristina Default' onclick=\"window.location='/defaults'\">";
html += "<br><br>";
    html += "Audio Input:<br>";
    html += "<input name='audio' value='" + config.audioInput + "'><br><br>";
    html += "Scena 1:<br>";
html += makeSceneSelect("scene1", config.scene1);
html += "<br><br>";

html += "Scena 2:<br>";
html += makeSceneSelect("scene2", config.scene2);
html += "<br><br>";

html += "Scena 3:<br>";
html += makeSceneSelect("scene3", config.scene3);
html += "<br><br>";

html += "Scena 4:<br>";
html += makeSceneSelect("scene4", config.scene4);
html += "<br><br>";

html += "Scena 5:<br>";
html += makeSceneSelect("scene5", config.scene5);
html += "<br><br>";

html += "Scena 6:<br>";
html += makeSceneSelect("scene6", config.scene6);
html += "<br><br>";
html += "<hr>";

html += "<h3>Modalita Sources</h3>";
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


html += "Tasto 1:<br>";
html += "<select name='sourceKey1' id='sourceKey1'></select>";
html += "<input type='hidden' id='saved1' value='" + config.sourceKey1 + "'>";
html += "<br><br>";
html += "Tasto 2:<br>";
html += "<select name='sourceKey2' id='sourceKey2'></select>";
html += "<input type='hidden' id='saved2' value='" + config.sourceKey2 + "'>";
html += "<br><br>";
html += "Tasto 3:<br>";
html += "<select name='sourceKey3' id='sourceKey3'></select>";
html += "<input type='hidden' id='saved3' value='" + config.sourceKey3 + "'>";
html += "<br><br>";
html += "Tasto 4:<br>";
html += "<select name='sourceKey4' id='sourceKey4'></select>";
html += "<input type='hidden' id='saved4' value='" + config.sourceKey4 + "'>";
html += "<br><br>";
html += "Tasto 5:<br>";
html += "<select name='sourceKey5' id='sourceKey5'></select>";
html += "<input type='hidden' id='saved5' value='" + config.sourceKey5 + "'>";
html += "<br><br>";
html += "Tasto 6:<br>";
html += "<select name='sourceKey6' id='sourceKey6'></select>";
html += "<input type='hidden' id='saved6' value='" + config.sourceKey6 + "'>";
html += "<br><br>";
  html += "<input type='submit' value='SALVA'>";
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

window.onload = loadSources;

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
    config.wifiSSID =
        server.arg("ssid");

    config.wifiPass =
        server.arg("pass");

    config.obsIP =
        server.arg("obsip");

    config.obsPort =
        server.arg("obsport").toInt();

    config.audioInput =
        server.arg("audio");
        config.scene1 = server.arg("scene1");
        config.scene2 = server.arg("scene2");
        config.scene3 = server.arg("scene3");

        config.scene4 = server.arg("scene4");
        config.scene5 = server.arg("scene5");
        config.scene6 = server.arg("scene6");
       config.sourceScene =
    server.arg("sourceScene");
config.sourceKey1 = server.arg("sourceKey1");
config.sourceKey2 = server.arg("sourceKey2");
config.sourceKey3 = server.arg("sourceKey3");
config.sourceKey4 = server.arg("sourceKey4");
config.sourceKey5 = server.arg("sourceKey5");
config.sourceKey6 = server.arg("sourceKey6");

        
    saveSettings();

    server.send(
        200,
        "text/html",
        "<h2>Salvato</h2><p>Riavvia ESP32</p>");
        delay (1000);
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
    server.on("/", handleRoot);

    server.on("/save",
              HTTP_POST,
              handleSave);

    server.on("/sources",
              handleSources);

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

