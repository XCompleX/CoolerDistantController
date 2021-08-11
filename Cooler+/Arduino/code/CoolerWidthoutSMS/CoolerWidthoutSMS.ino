#include "HX711.h"
#include <WiFi.h>
#include <HTTPClient.h>

#define DOUT  27
#define CLK  14

#define LED 13
#define ButtonCalibration 26
/***************WI_FI*************************/
const char* ssid = "NameYourWi-FI";
const char* password = "PasswordYourWi-Fi";

int coolerID = 1;
int percent = 0;
/***************HX711*************************/
HX711 scale;
float scale_calibration =-0.77;
float weight_grMAX = 0;
float weight_grNOW = 0;
float weight_grMIN = 0;
/***************ButtonCallibation*************/
bool firstCalibr = false;
bool secondCalibr = false;
volatile long paused   = 50;
volatile long lastTurn = 0;
volatile int count = 0;

void setup() {  
  pinMode(ButtonCalibration, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  WiFi.begin(ssid,password);
  Serial.begin(115200);
  scale.begin(DOUT,CLK);
  attachInterrupt(digitalPinToInterrupt(ButtonCalibration), buttonManager, CHANGE);
  
  while(!firstCalibr){
    Serial.println("Нажмите кнопку 1-й раз");
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100); 
  }
  
  scale.set_scale();
  scale.tare();
  scale.set_scale(scale_calibration);
  
  weight_grMIN = scale.get_units(10) * 0.035274;
  Serial.println(weight_grMIN);

  digitalWrite(LED, LOW);
  delay(1000);
  
  while(!secondCalibr){
    Serial.println("Нажмите кнопку 2-й раз");
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100); 
  }
  
  weight_grMAX = scale.get_units(10) * 0.035274;
  Serial.println(weight_grMAX);

  digitalWrite(LED, LOW);
}
void loop() {  
  if ((WiFi.status() == WL_CONNECTED)) {
    Serial.print("WIFI CONNECTED | ");
    HTTPClient http;
    readData();
    sendSqlData(http);
    Serial.print(String(weight_grMIN)+" | " );
    Serial.print(String(weight_grNOW)+" | " );
    Serial.print(String(weight_grMAX)+" | " );
    Serial.println(String(percent));
  }
  else{
    Serial.println("WIFI DISCONNECTED");
  }
}
void readData(){
  
  weight_grNOW = scale.get_units(10)* 0.035274;
  percent = (weight_grNOW-weight_grMIN)*100/(weight_grMAX-weight_grMIN);
  
  if(percent < 0) percent = 0;
  else if (percent > 100) percent = 100;
}
void sendSqlData(HTTPClient &http){
    http.begin("YourSite/set_service.php");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded", false, true);
    int httpCode = http.POST("ID=" + String(coolerID) + "&Value=" + String(percent));

    if(httpCode > 0){
      String payload = http.getString();
      Serial.print(httpCode);
      Serial.print(" | ");
      Serial.print(payload); 
    }
    http.end();
}
void buttonManager(){
    if (millis() - lastTurn < paused) return;
    cli();
    count++;
    if(count == 2){
      if(!firstCalibr) firstCalibr = true;
      else if(!secondCalibr) secondCalibr = true;
      count = 0;
    }
    sei(); 
    lastTurn = millis();
}
