#include "OTABlynkCredentials.h"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include "DHT.h"

credentials Credentials;

char auth_token[33];
bool connected = 0;
const int Erasing_button = 0;

char* esp_ssid = "ESP-8266";
char* esp_pass = "12345678";

DHT dhtA(2, DHT22);
int temperature_read = 0;

BLYNK_WRITE(V1) { //Ler pin V0
  int temperature_read = param.asInt();
  Serial.println(temperature_read);
}

void climateRoutine(){
  byte t = dhtA.readTemperature();
  Serial.println(t);
  Blynk.virtualWrite(V0,t);
}

void setup() {
  Serial.begin(115200);
  pinMode(Erasing_button, INPUT);

  for (uint8_t t = 4; t > 0; t--) {
    Serial.println(t);
    delay(1000);
  }

  if (digitalRead(Erasing_button) == LOW) {
    Credentials.Erase_eeprom();
  }

  String auth_string = Credentials.EEPROM_Config();
  auth_string.toCharArray(auth_token, 33);

  if (Credentials.credentials_get()) {
    Blynk.config(auth_token);
    connected = 1;
  } else {
    Credentials.setupAP(esp_ssid, esp_pass);
    connected = 0;
  }

  if (connected) {
    dhtA.begin();
  }
}

void loop() {
  Credentials.server_loops();

  Blynk.run();
  
  if (connected) {
    climateRoutine();
    delay(4700);

    BLYNK_WRITE(V1);

    if ((dhtA.readTemperature() - temperature_read) >= 5) { 
      Blynk.notify("Dormitório Inteligente - Temperatura acima da desejada!");
    } else if ((dhtA.readTemperature() - temperature_read) <= 5) {
      Blynk.notify("Dormitório Inteligente - Temperatura abaixo da desejada!");
    } 
  }
}
