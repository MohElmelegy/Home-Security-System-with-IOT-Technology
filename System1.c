#include <Servo.h>
#define BLYNK_PRINT Serial // Comment this out to disable prints and save space
#include <SPI.h>
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SimpleTimer.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include<EEPROM.h>
#include <elapsedMillis.h>
elapsedMillis timeElapsed;
/* Include the RFID library */
#include <SPI.h>
#include <MFRC522.h>/* Define the DIO used for the SDA (SS) and RST (reset)
pins. */
char auth[] = "cd707b7374514ee3a8c6448f32fd4630";
char ssid[] = "Samy_Elmelegy";
char pass[] = "Ihf24031994";
char IP[] = "192.168.1.5";
#include <SoftwareSerial.h>
#define EspSerial Serial1
#define closeLight 40
#define change 24
#define door 47
#define DHTPIN 53
#define pw1 45
#define pw3 41
#define pw_ok 27
#define pw1_check 44
51
#define pw3_check 42
#define pw_ok_check 26
#define SS_PIN 9
#define RST_PIN 8
#define RFID_Enable 28
#define RFID_Disenable 30
int ID_ture = 0;
int ID_flase = 0;
int ID_ture_led =0;
int ID_flase_led = 0;
int flag_outdoor = 0;
int flag_sys = 0;
int flag_out_sys =0;
/* Create an instance of the RFID library */
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
int PIR_sensor = 46;
int PIR_sensor_value = 0;
int vib_sensor=21;
int vib_sensor_value;
int const trigPin = 3;
int const echoPin = 2;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;
Servo servo2;
Servo servo3;
Servo servo4;
int x=0;
int y=0;
int q=0;
52
int g=0;
const int gas_sensor_Pin = A0;
float gas_sensor_value = 0;
const int flame_sensor_Pin = A1;
float flame_sensor_value = 0;
const int water_sensor_Pin = A2;
float water_sensor_value = 0;
// Attach Water sensor to Arduino Digital Pin 8
#define Buzzer 36 // Attach an Buzzer to Digital Pin 51 (or use onboard LED)
// Your ESP8266 baud rate:
#define ESP8266_BAUD 115200
#define DHTTYPE DHT11
ESP8266 wifi(&EspSerial);
//Temp. & Hum. Sensor(DHT)
DHT dht(DHTPIN, DHTTYPE);
WidgetLED led1 (V1);
BlynkTimer timer;
void sendSensor()
{
float h = dht.readHumidity();
float t = dht.readTemperature();
if (isnan(h) || isnan(t)) //is not number
{
Serial.println("Failed to read from DHT sensor!");
}
// You can send any value at any time.
53
// Please don't send more that 10 values per second.
Blynk.virtualWrite(V5, h);
Blynk.virtualWrite(V6, t);
if(t > 28)
{
Serial.println ("Temperature over 28C!");
Blynk.notify("Temperature over 28C!");
}
water_sensor_value = analogRead(water_sensor_Pin);
// determine alarm status
if (water_sensor_value >= 400)
{
Serial.println ("water Laekage!");
Blynk.notify ("water Laekage!");
digitalWrite(Buzzer, HIGH); // sets the LED on
beep();
}
else
{
digitalWrite(Buzzer, LOW); // sets the LED off
}
// print the results to the serial monitor:
Serial.print("water sensor = " );
Serial.println(water_sensor_value);
if (digitalRead(change)== HIGH)
{
54
}
//Ultrasonic sensor
// Duration will be the input pulse width and distance will be the distance to the
obstacle in centimeters
int duration, distance;
// Output pulse with 1ms width on trigPin
digitalWrite(trigPin, HIGH);
delay(1);
digitalWrite(trigPin, LOW);
// Measure the pulse input in echo pin
duration = pulseIn(echoPin, HIGH);
// Distance is half the duration devided by 29.1 (from datasheet)
distance = (duration/2) / 29.1;
// if distance less than 0.5 meter and more than 0 (0 or less means over range)
if (distance <= 50 && distance >= 0 && flag_out_sys ==1)
{
Serial.println ("Garage open");
Blynk.notify("Garage open");
for (pos2 = 0; pos2 <= 180; pos2 += 1)
{ // goes from 0 degrees to 180 degrees
// in steps of 1 degree
servo2.write(pos2); // tell servo to go to position in variable 'pos'
delay(2); // waits 15ms for the servo to reach the position
}
}
else if (distance <= 50 && distance >= 0 && flag_out_sys ==0)
{
Serial.println ("Person detect.....");
Blynk.notify ("Person detect.....");
digitalWrite(Buzzer, HIGH); // sets the Buzzer on
55
beep();
}
else
{
digitalWrite(Buzzer, LOW); // sets the Buzzer off
}
vib_sensor_value= digitalRead (vib_sensor);
if (q == 1 && x == 0)
{
Serial.println ("Person open door!");
Blynk.notify ("Person open door!");
digitalWrite(Buzzer, HIGH); // sets the Buzzer on
beep();
q=0;
}
else
{
digitalWrite(Buzzer, LOW); // sets the Buzzer off
}
Serial.print("Vibration sensor = " );
Serial.println(vib_sensor_value);
//PIR Sensor
PIR_sensor_value = digitalRead(PIR_sensor); // read PIR_sensor value
if (PIR_sensor_value == HIGH && flag_out_sys == 0) // check if the sensor is
HIGH
56
{
Serial.println ("Person detect!");
Blynk.notify ("Person detect!");
digitalWrite(Buzzer, HIGH); // sets the Buzzer on
beep();
}
else
{
digitalWrite(Buzzer, LOW); // sets the Buzzer off
}
// print the results to the serial monitor:
Serial.print("PIR_sensor = " );
Serial.println(PIR_sensor_value);
//flame_sensor
flame_sensor_value = analogRead(flame_sensor_Pin);
// determine alarm status
if (flame_sensor_value <950)
{
Serial.println ("Fire Detected!");
Blynk.notify ("Fire Detected!");
digitalWrite(Buzzer, HIGH); // sets the Buzzer on
beep();
if (y==0 && g ==0){
for (pos3 = 0; pos3 <= 90; pos3 += 1) // goes from 0 degrees to 180 degrees
{
servo3.write(pos3); // tell servo to go to position in variable 'pos'
delay(2); // waits 15ms for the servo to reach the position
}
57
y=1;
}
}
else
{
digitalWrite(Buzzer, LOW); // sets the Buzzer off
}
// print the results to the serial monitor
Serial.print("flame sensor = " );
Serial.println(flame_sensor_value);
//Gas_sensor
gas_sensor_value = analogRead(gas_sensor_Pin);
// determine alarm status
if (gas_sensor_value >= 1050)
{
Serial.println ("Gas Laekage!");
Blynk.notify ("Gas Laekage!");
digitalWrite(Buzzer, HIGH); // sets the Buzzer on
beep();
if (y==0 && g ==0){
for (pos3 = 0; pos3 <= 90; pos3 += 1)
{ // goes from 0 degrees to 180 degrees
// in steps of 1 degree
servo3.write(pos3); // tell servo to go to position in variable 'pos'
delay(2); // waits 15ms for the servo to reach the position
}
g =1;
}
}
58
else
{
digitalWrite(Buzzer, LOW); // sets the Buzzer off
}
Serial.print("gas sensor = " );
Serial.println(gas_sensor_value);
if (flame_sensor_value >1000 && gas_sensor_value <= 400)
{
servo3.write(0); // tell servo to go to position in variable 'pos'
delay(2);
y=0;
g=0;
}
if (digitalRead (change)== HIGH)
{
led1.on (); //outside system on
flag_out_sys =0;
flag_sys = 0;
if ( flag_outdoor== 1 ) {
servo4.write(0); // tell servo to go to position in variable 'pos'
delay(2); // waits 15ms for the servo to reach the position
flag_outdoor = 0;
}
59
}
else if (flag_sys == 1 && digitalRead (change)== LOW)
{
led1.off (); //outside system off
flag_out_sys =1;
}
else
{
led1.on ();
flag_out_sys =0;
}
}
void setup()
{
// Debug console
Serial.begin(9600);
delay(10);
// Set ESP8266 baud rate
EspSerial.begin(ESP8266_BAUD);
delay(10);
Blynk.begin(auth, wifi, ssid, pass, IP);
dht.begin();
60
// Setup a function to be called every second
timer.setInterval(4000L, sendSensor);
pinMode(water_sensor_Pin, INPUT); // The Water Sensor is an Input
pinMode(gas_sensor_Pin, INPUT);
pinMode(flame_sensor_Pin, INPUT);
pinMode(Buzzer, OUTPUT); // The LED is an Output
pinMode(trigPin, OUTPUT); // trig pin will have pulses output
pinMode(echoPin, INPUT); // echo pin should be input to get pulse width
pinMode (PIR_sensor,INPUT);
pinMode (vib_sensor, INPUT);
pinMode(pw1, OUTPUT);
pinMode(pw3, OUTPUT);
pinMode(pw_ok, OUTPUT);
pinMode(pw1_check, INPUT);
pinMode(pw3_check, INPUT);
pinMode(pw_ok_check, INPUT);
pinMode(change, INPUT);
servo2.attach (5);
servo3.attach (6);
servo4.attach (10);
pinMode(door, INPUT);
pinMode(closeLight, OUTPUT);
digitalWrite(closeLight, HIGH);
attachInterrupt(digitalPinToInterrupt (vib_sensor), Vib, HIGH );
Serial.begin(9600);
SPI.begin(); // Initiate SPI bus
61
mfrc522.PCD_Init(); // Initiate MFRC522
}
void loop()
{
Blynk.run();
timer.run(); // Initiates SimpleTimer
if (digitalRead(door)== HIGH)
{
Blynk.notify ("Gate opened");
digitalWrite(closeLight, LOW);
x=1;
}
if (digitalRead(door)== LOW && x==1)
{
digitalWrite(closeLight, HIGH);
x=0;
}
if (digitalRead(pw1_check)== HIGH && digitalRead(pw3_check)== HIGH &&
digitalRead(pw_ok_check)== HIGH)
{
}

if ( mfrc522.PICC_IsNewCardPresent())
{
if ( mfrc522.PICC_ReadCardSerial())
{
delay (300);
//Show UID on serial monitor
Serial.print("UID tag :");
String content= "";
byte letter;
for (byte i = 0; i < mfrc522.uid.size; i++)
{
Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
Serial.print(mfrc522.uid.uidByte[i], HEX);
content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
content.concat(String(mfrc522.uid.uidByte[i], HEX));
}
Serial.println();
Serial.print("Message : ");
content.toUpperCase();
if (content.substring(1) == "C5 3B DA AB") //change here the UID of the card/cards
that you want to give access
{
Serial.println("Authorized access");
ID_ture = 1;
flag_sys = 1;
}
63
else {
Serial.println(" Access denied");
ID_flase = 1;
}
}
}
if (ID_ture == 1){
digitalWrite(RFID_Enable, HIGH);
if (flag_outdoor==0){
for (pos4 = 0; pos4 <= 180; pos4 += 1)
{ // goes from 0 degrees to 180 degrees
// in steps of 1 degree
servo4.write(pos4); // tell servo to go to position in variable 'pos'
delay(2); // waits 15ms for the servo to reach the position
}
flag_outdoor=1;
}
ID_ture =0;
ID_ture_led = 1;
}
if (ID_flase == 1){
digitalWrite(RFID_Disenable, HIGH);
ID_flase = 0;

ID_flase_led = 1;
beep ();
}
if (timeElapsed>2000 && timeElapsed<3000 && ID_ture_led == 1)
{
digitalWrite(RFID_Enable, HIGH);
timeElapsed=0;
ID_ture_led =0;
}
if (timeElapsed>2000 && timeElapsed<3000 && ID_flase_led ==1)
{
digitalWrite(RFID_Disenable, HIGH);
timeElapsed=0;
ID_flase_led =0;
}
}


void Vib()
{
q=1;
}
void beep()
{
digitalWrite(Buzzer, HIGH);
delay(300);
digitalWrite(Buzzer, LOW);
}





