#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "env.h"

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


void post_sensor_data(float temp, bool presence){
  HTTPClient http;
  String requestBody;

  http.begin(endpoint);
  http.addHeader("Content-Type", "application/json");

  JsonDocument doc;

  doc["temp"] = temp;
  doc["presence"] = presence;

  doc.shrinkToFit();

  serializeJson(doc, requestBody);

  int httpResponseCode = http.PUT(requestBody);

  Serial.print("HERE IS THE RESPONSE: ");
  Serial.println(requestBody);
  Serial.println(http.getString());
  Serial.println();

  http.end();
}

void get_sensor_data(){
  HTTPClient http;

  http.begin(endpoint);

  int httpResponseCode = http.GET();

  if(httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    String responseBody = http.getString();
    Serial.println(responseBody);

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, responseBody);

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    bool fanStat = doc["fan"];
    digitalWrite(fan, fanStat);
    bool lightStat = doc["light"];
    digitalWrite(light, lightStat);
  }
  else{
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}

void setup() {
  Serial.begin(9600);
	
	// WiFi_SSID and WIFI_PASS should be stored in the env.h
  WiFi.begin(ssid, password);

	// Connect to wifi
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  // configure fan and light as output pins
  pinMode(fan, OUTPUT);
  pinMode(light, OUTPUT);
}

void loop() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    // temperature data
    sensors.requestTemperatures();
    float t = sensors.getTempCByIndex(0);

    // motion data
    bool p;
    pirStat = digitalRead(pirPin);
    if(pirStat == HIGH){
      p = true;
    }else{
      p = false;
    }
    
    post_sensor_data(t, p);
    get_sensor_data();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}