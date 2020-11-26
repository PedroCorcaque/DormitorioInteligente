#define BLYNK_SERIAL Serial

#include "DHT.h"                              // DHT Sensor
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "Y3IsNnWLZ1wdOYcuHPAlPj_APmrSTRiz";
char ssid[] = "VETORIAL_1063";
char pass[] = "401046819";

DHT dhtA(2, DHT22);                           // DHT instance named dhtA, D1 and sensor type
int parametro = 0;
int notificou = 0;                            // 0 Sem notificação - 1 Notificação Acima - 2 Notificação Abaixo
int parametro_anterior = 0;

BLYNK_WRITE(V2) {
  parametro = param.asInt();
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);              // Connecting to blynk
  dhtA.begin();                               // Starting the DHT-22
}

void loop() {
  Blynk.run();
  byte t1 = dhtA.readTemperature();
  Blynk.virtualWrite(V0,t1);
  delay(4700);
  BLYNK_WRITE(V2);
  
  parametro_anterior = parametro;
  if (parametro_anterior != parametro) {
    notificou = 0;
  }
  
  if ((t1 - parametro) > 5 and notificou != 1) { 
    Blynk.notify("Dormitório Inteligente - Temperatura acima da desejada!");
    notificou = 1;   
  } else if ((t1 - parametro) < 5 and notificou != 2) {
    Blynk.notify("Dormitório Inteligente - Temperatura abaixo da desejada!");
    notificou = 2;
  }
}
