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

void setup() {  
  pinMode(ButtonCalibration, INPUT);
  pinMode(LED, OUTPUT);
  WiFi.begin(ssid,password);
  Serial.begin(115200);
  scale.begin(DOUT,CLK);

  while(!firstCalibr){
    firstCalibr = digitalRead(ButtonCalibration);
    Serial.println("Нажмите кнопку 1-й раз");
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500); 
  }
  
  scale.set_scale();
  scale.tare();
  scale.set_scale(scale_calibration);
  
  weight_grMIN = scale.get_units(10) * 0.035274;
  Serial.println(weight_grMIN);
  
  while(firstCalibr){
    firstCalibr = digitalRead(ButtonCalibration);
    Serial.println("Отпустите кнопку");
    digitalWrite(LED, HIGH);
    delay(100);
  }
  
  digitalWrite(LED, LOW);
  delay(15000);
  
  while(!secondCalibr){
    secondCalibr = digitalRead(ButtonCalibration);
    Serial.println("Нажмите кнопку 2-й раз");
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500); 
  }
  
  weight_grMAX = scale.get_units(10) * 0.035274;
  Serial.println(weight_grMAX);
  
  while(secondCalibr){
    secondCalibr = digitalRead(ButtonCalibration);
    Serial.println("Отпустите кнопку");
    digitalWrite(LED, HIGH);
    delay(100);
  }

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
    http.begin("YourSite/set_service.php?ID=" + String(coolerID) + "&Value=" + String(percent));
    int httpCode = http.GET();

    if(httpCode > 0){
      String payload = http.getString();
      Serial.print(httpCode);
      Serial.print(" | ");
      Serial.print(payload); 
    }
    http.end();
}
