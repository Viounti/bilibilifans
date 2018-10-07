//SDA - D2
//SCL - D1
//VCC - 3V
//GND - G
//本代码禁止商用，若有商用，法律追究.
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4); 
#include <Wire.h> 
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
const char* ssid = "You Wifi name";
const char* password = "You Wifi password";
DynamicJsonDocument jsonBuffer(200);
WiFiClient client;
void setup() {
  lcd.init();//初始化LCD
  lcd.backlight();//背光灯
  lcd.setCursor(0,0);//坐标
  WiFi.begin(ssid,password);//获取名称密码
  while(WiFi.status()!=WL_CONNECTED){  //连接WIFI
    delay(500);
    lcd.print(".");
  }
  lcd.setCursor(0,1);
  lcd.print("WiFi connected");
  delay(2000);
  lcd.clear();
}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin("https://api.bilibili.com/x/relation/stat?vmid=替换这串汉字为你的UID");
    int httpCode=http.GET(); //获取数据
    if(httpCode == 200){
      String resBuff = http.getString();
      DeserializationError error = deserializeJson(jsonBuffer, resBuff); 
      if(error){
        Serial.println("json error");
        while(1);
      }
      JsonObject root = jsonBuffer.as<JsonObject>();//转化数据
      long code = root["code"];
      Serial.println(code);
      long fans = root["data"]["follower"];//定义fans并赋值
      lcd.setCursor(0,0);//LCD屏幕坐标
      lcd.print("//你的名字");
      lcd.setCursor(0,1);//LCD屏幕坐标
      lcd.print("Fans:");
      lcd.print(fans);//打印变量
      delay(1000);
    }
  }
}
