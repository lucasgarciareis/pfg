#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

const char *ssid = "Lucas_2G";
const char *password = "lucas230992";
const int pinoSensor = 16; //PINO DIGITAL UTILIZADO PELO SENSOR
hw_timer_t *timer = NULL;
int ok = 0;

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

    attachInterrupt(pinoSensor, vibrou, CHANGE);
}

void loop()
{
    delay(1000);
    postDataToServer();
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

void vibrou()
{
    ok = 1;
    Serial.println("Vibrou!");
}
