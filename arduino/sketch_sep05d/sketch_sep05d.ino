/*//==============================================================================//
 * Vibration Sensor interfacing with Arduino
 * Date: - 15-04-2019
 * Author:- Sourav Gupta
 * For:- circuitdigest.com
 */ 
//=============================================================================//
 
/*
 * Pin Description
 */
int vibration_Sensor = 5;
//int LED = LED_BUILTIN;
 
/*
 * Programme flow Description
 */
int present_condition = 0;
int previous_condition = 0;
 
/*
 * Pin mode setup
 */
void setup() {
pinMode(vibration_Sensor, INPUT);
pinMode(LED_BUILTIN, OUTPUT);
digitalWrite(LED_BUILTIN, LOW);
}
 
/*
 * Led blink
 */
/*void led_blink(void) {
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
}*/
 
/*
 * main_loop
 */
 
void loop() {
//previous_condition = present_condition;
present_condition = digitalRead(vibration_Sensor); 
if (previous_condition != present_condition) {
//led_blink();
 digitalWrite(LED_BUILTIN, LOW);
} else {
digitalWrite(LED_BUILTIN, LOW);
}
}
