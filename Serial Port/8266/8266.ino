//本代码禁止商用，若有商用，法律追究.
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
const char* ssid = "You Wifi name";
const char* password = "You Wifi password";
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
    http.begin("https://api.bilibili.com/x/relation/stat?vmid=4893237");
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
      long code = root["code"];
      Serial.println(code);
      long fans = root["data"]["follower"];
      Serial.print("Fans:");
      Serial.println(fans);
      delay(1000);
    }
  }
}
