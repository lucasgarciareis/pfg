#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

#define LED_BUILTIN 2

const char *ssid = "Lucas_2G";
const char *password = "lucas230992";
const int pinoSensor = 16; //PINO DIGITAL UTILIZADO PELO SENSOR
const int pinoLed = 17;
int ok = 0;
int pirValue;

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

    pinMode(pinoSensor, INPUT);
    pinMode(pinoLed, OUTPUT);
}

void loop()
{
    pirValue = digitalRead(pinoSensor);
    if (pirValue == HIGH)
    {
        ok = 1;
        digitalWrite(pinoLed, HIGH); // turn the LED on (HIGH is the voltage level)
    }
    else
    {
        ok = 0;
        digitalWrite(pinoLed, LOW); // turn the LED off by making the voltage LOW
    }
    postDataToServer();
    delay(1000);
}

void postDataToServer()
{

    HTTPClient http;

    http.begin("http://192.168.1.7:54322/vibration");
    //http.begin("http://httpbin.org/post");
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<200> doc;

    if (ok == 1)
    {
        doc["vibration"] = 1;
    }
    else
    {
        doc["vibration"] = 0;
    }

    String requestBody;

    serializeJson(doc, requestBody);

    int httpResponseCode = http.POST(requestBody);

    if (httpResponseCode > 0)
    {
        String response = http.getString();
        Serial.println(httpResponseCode);
        Serial.println(response);
    }
    ok = 0;
}