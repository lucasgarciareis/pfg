#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

//ALTERAR SSID, PASSWORD E IP NA FUNÇÃO send_data(String)

#define SILENCE 824
const char* ssid = "FOSSI";
const char* password = "fossi7310";


int soundPin = A0;
int ledPin = LED_BUILTIN;
int sensorValue = 0;
float m_count_init = 60;   //amount of measurements made before the json is sent (every second)
int m_count = m_count_init;

int expected_delay = (1/m_count_init)*1000;

void send_data(String);

void setup ()
{
  Serial.begin (115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion

    delay(500);
    Serial.println("Waiting for connection");

  }
  pinMode (ledPin, OUTPUT);
  pinMode (soundPin, INPUT);
}

void loop ()
{
  StaticJsonDocument<1024> jsonDoc;
  JsonArray array = jsonDoc.createNestedArray("data");

  //loop que coleta m_count_init vezes por segundo e monta o array
  while(m_count > 0)
  {
    sensorValue = analogRead (soundPin);
    array.add(sensorValue);
    m_count--;
    //delay(expected_delay);
    delay(expected_delay);
  }
    serializeJson(jsonDoc, Serial); //para printar na porta serial

    String json_serial;
    serializeJson(jsonDoc, json_serial);
    send_data(json_serial);

  m_count = m_count_init;
  //delay(1000);  //Send a request every second
}

void send_data(String data)
{
    if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

      HTTPClient http;    //Declare object of class HTTPClient

      http.begin("http://192.168.101.8:54322/sound");      //Specify request destination
      http.addHeader("Content-Type", "application/json");  //Specify content-type header

      int httpCode = http.POST(data);   //Send the request
      String payload = http.getString();                  //Get the response payload

      Serial.println(httpCode);   //Print HTTP return code
      Serial.println(payload);    //Print request response payload

      http.end();  //Close connection

  } else {

      Serial.println("Error in WiFi connection");

  }
}
