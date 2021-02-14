int soundPin = 0;
int ledPin = 14;
int sensorValue = 0;
 
void setup () 
{
  Serial.begin (115200);
  pinMode (ledPin, OUTPUT);
  pinMode (soundPin, INPUT);
}
 
void loop () 
{
  sensorValue = digitalRead (soundPin);
  Serial.println (sensorValue, DEC);
  //if sensor goes above max light led (could be buzzer)
  if(sensorValue >= 1)
  {
    digitalWrite (ledPin, HIGH);
    delay (100);
  } else {
  //switch off LED
  digitalWrite (ledPin, LOW);
  delay (100);
  }
}
