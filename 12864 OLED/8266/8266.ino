//SDA - D2
//SCL - D1
//VCC - 3V
//GND - G
//本代码禁止商用，若有商用，法律追究.
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Adafruit_ssd1306syp.h>
unsigned int localPort = 2390;  
const char* ssid = "WIFI NAME";  //bazhegaile
const char* password = "Password";
DynamicJsonDocument jsonBuffer(200);
WiFiClient client;
IPAddress timeServerIP;
const char* ntpServerName = "time.windows.com";
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[ NTP_PACKET_SIZE];
WiFiUDP udp;
void setup() {
  display.initialize();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);//获取名称密码
  display.println("Connecting To:");
  display.println("SSID:"+String(ssid));
  display.println("Password:"+String(password));
    display.update();
  display.print("[<=");
  while(WiFi.status()!=WL_CONNECTED){  //连接WIFI
    delay(500);
    display.print("=");
      display.update();
  }
  display.print("=>]");
  display.println();
  display.update();
  delay(1000);
    display.clear();
 display.println("WiFi connected");
 display.println("Local IP:"+WiFi.localIP());
   display.update();
 udp.begin(localPort);
 delay(2000);
}

void loop() {
  display.clear();
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin("https://api.bilibili.com/x/relation/stat?vmid=你的UID");  //zhe de UID gai cheng ni de
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
      display.setCursor(0,0);//display屏幕坐标
      display.println("UP:你的名字"); 
      display.println("Fans:");
      display.println(fans);//打印变量
      showTime();
      display.update();
      delay(10000);
    }

  
  }
}
void showTime(){
  WiFi.hostByName(ntpServerName, timeServerIP); 
  sendNTPpacket(timeServerIP);
  delay(1000);
  int cb = udp.parsePacket();
  if (!cb) {
    display.println("no packet yet");
  }
  else {
    udp.read(packetBuffer, NTP_PACKET_SIZE);
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;
    unsigned long epoch = secsSince1900 - seventyYears;
    display.print(((epoch  % 86400L) / 3600)+8);
    display.print(':');
    if ( ((epoch % 3600) / 60) < 10 ) {
      display.print('0');
    }
    display.print((epoch  % 3600) / 60);
  }
}
void sendNTPpacket(IPAddress& address){
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  packetBuffer[0] = 0b11100011;
  packetBuffer[1] = 0;
  packetBuffer[2] = 6;
  packetBuffer[3] = 0xEC; 
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  udp.beginPacket(address, 123);
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}
