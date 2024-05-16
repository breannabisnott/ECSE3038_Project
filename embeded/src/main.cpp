#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "env.h"

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


void get_sensor_data(float temp){
  HTTPClient http;
  String requestBody;

  http.begin(endpoint);
  http.addHeader("Content-Type", "application/json");
  http.addHeader ("Content-Length","62");
  http.addHeader("api-key", api_key);

  JsonDocument doc;

  doc["temp"] = temp;

  doc.shrinkToFit();

  serializeJson(doc, requestBody);

  int httpResponseCode = http.PUT(requestBody);

  Serial.print("HERE IS THE RESPONSE: ");
  Serial.println(requestBody);
  Serial.println(http.getString());
  Serial.println();

  http.end();
}

void getLight(){
  HTTPClient http;

  String newEndpoint;
  String path = "/api/light";
  newEndpoint = endpoint + path;    //change path

  http.begin(newEndpoint);
  http.addHeader("api-key", api_key);

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

    bool light = doc["light"];
    digitalWrite(LED, light);
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
  pinMode(LED, OUTPUT);
}

void loop() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    sensors.requestTemperatures();
    float x = sensors.getTempCByIndex(0);
    get_sensor_data(x);
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}