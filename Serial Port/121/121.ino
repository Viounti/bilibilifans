#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "TP-LINK_E0AE";
const char* password = "fam96461105";

DynamicJsonDocument jsonBuffer(200);

WiFiClient client;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin("https://api.bilibili.com/x/relation/stat?vmid=17539027");
    int httpCode=http.GET();
    if(httpCode == 200){
      Serial.println("Get OK");
      String resBuff = http.getString();
      DeserializationError error = deserializeJson(jsonBuffer, resBuff); 
      if(error){
        Serial.println("json error");
        while(1);
      }
      JsonObject root = jsonBuffer.as<JsonObject>();
      long fans = root["data"]["follower"];
      Serial.print("Fans:");
      Serial.println(fans);
      delay(1000);
    }
  }
}
