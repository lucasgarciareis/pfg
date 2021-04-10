#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

const char *ssid = "Lucas_2G";
const char *password = "lucas230992";
const char *serverName = "http://192.168.1.7:54321/esp32"; // "http://httpbin.org/post"

//  GPIOs
int audio_pin = 35;
int movement_pin = 16;
int pressure_pin = 34;
int led_pin = 17;

// variable for storing the sensors value
int sensor_audio = 0;
int sensor_movement = 0;
int sensor_pressure = 0;

void postDataToServer(String);

void setup()
{
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");

    pinMode(movement_pin, INPUT);
    pinMode(led_pin, OUTPUT);
}

void loop()
{

    //Use 2048 for 100 samples and 1024 for 50 samples
    StaticJsonDocument<3072> jsonDoc;
    JsonArray array_sound = jsonDoc.createNestedArray("sound");
    JsonArray array_movement = jsonDoc.createNestedArray("movement");
    JsonArray array_pressure = jsonDoc.createNestedArray("pressure");

    for (int a = 0; a < 50; a++)
    {
        sensor_audio = analogRead(audio_pin);
        sensor_movement = digitalRead(movement_pin);
        sensor_pressure = analogRead(pressure_pin);
        array_sound.add(sensor_audio);
        array_movement.add(sensor_movement);
        array_pressure.add(sensor_pressure);
        delay(20);
    }

    String json_serial;
    serializeJson(jsonDoc, json_serial);

    postDataToServer(json_serial);
}

void postDataToServer(String sensors)
{

    HTTPClient http;

    http.begin(serverName);

    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(sensors);

    if (httpResponseCode > 0)
    {
        String response = http.getString();
        Serial.println(httpResponseCode);
        Serial.println(response);
    }
}