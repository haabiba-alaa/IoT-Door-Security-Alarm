#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

//pins 
#define trig D2
#define echo D3
#define buzzer D1
#define led D4

bool flag = false;
int distance ;

//Wifi
const char* ssid=  "ALAA";
const char * pass= "OMHM.ALAA";
String link = "http://maker.ifttt.com/trigger/Door/json/with/key/evJW7fdCp6oexaoVpKBWLjFfwe_gj11fdjD2sx4p9cj";
WiFiClient client;
HTTPClient http;
ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin (ssid , pass);
  WiFi.softAP ( "test" , "12345678");
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Used IP address:___ ");
  Serial.println(IP);
  pinMode (trig ,OUTPUT);
  pinMode (echo , INPUT);
  pinMode (buzzer , OUTPUT);
  pinMode (led , OUTPUT);
  server.on ("/Open", handleOn);
  server.on ("/Close", handleOff);
  server.begin();
}

void loop() {
  server.handleClient();
  if (flag) {
  if(WiFi.status()== WL_CONNECTED)
  { 
    findDistance();
    if (distance <= 50)
    {
      tone(buzzer , distance*3);
      http.begin (client , link.c_str());
      int response= http.GET();
      Serial.println( http.getString());
      http.end();
    }
    else {
    noTone (buzzer) ;
    }
  }
 }
}
void findDistance() {
  int duration;
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (1 / 29.1) * (duration / 2);
  delay(60);
}
void handleOn (){
   flag = true;
   digitalWrite(led, HIGH);
}
void handleOff (){
  flag = false;
  digitalWrite(led, LOW);
}
