#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>


#define SILENCE 824
const char* ssid = "FOSSI";
const char* password = "fossi7310";

int soundPin = A0;
int ledPin = LED_BUILTIN;
int sensorValue = 0;

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
  sensorValue = analogRead (soundPin);
  Serial.println (sensorValue, DEC);

    if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin("http://0.0.0.0:54322/sound");      //Specify request destination
    http.addHeader("Content-Type", "text/plain");  //Specify content-type header
 
    int httpCode = http.POST(sensorValue);   //Send the request
    String payload = http.getString();                  //Get the response payload
 
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload
 
    http.end();  //Close connection
 
  } else {
 
    Serial.println("Error in WiFi connection");
 
  }
 
  delay(30000);  //Send a request every 30 seconds
 
}
