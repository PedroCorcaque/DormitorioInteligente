#define BLYNK_SERIAL Serial

#include "DHT.h"                              // DHT Sensor
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "Y3IsNnWLZ1wdOYcuHPAlPj_APmrSTRiz";
char ssid[] = "VETORIAL_1063";
char pass[] = "401046819";

DHT dhtA(2, DHT22);                           // DHT instance named dhtA, D1 and sensor type
int temperature = 0;

BLYNK_WRITE(V2) {
  temperature = param.asInt();
  Serial.print("Parametro: ");
  Serial.println(temperature);  
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);              // Connecting to blynk
  dhtA.begin();                               // Starting the DHT-22
}

void loop() {
  Blynk.run();
  
   byte t1 = dhtA.readTemperature();
   Serial.print("Temperatura: ");
   Serial.println(t1);
   Blynk.virtualWrite(V0,t1);
   delay(4700);
   BLYNK_WRITE(V2);
   Serial.print("Parametro loop: ");
   Serial.println(temperature);
   Serial.print("Temperatura: ");
   Serial.println(t1);

   if ((t1 - temperature) >= 5) { 
      Blynk.notify("Dormitório Inteligente - Temperatura acima da desejada!");
    } else if ((t1 - temperature) <= 5) {
      Blynk.notify("Dormitório Inteligente - Temperatura abaixo da desejada!");
    }
}