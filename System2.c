#include <Servo.h>
#include <Adafruit_Sensor.h>
int getFingerprintIDez();
#include<LiquidCrystal.h>
LiquidCrystal lcd(12,11,10,9,8,7);
#include <SoftwareSerial.h>
SoftwareSerial fingerPrint(2, 3);
//green, Yellow
#include <Adafruit_Fingerprint.h>
uint8_t id;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerPrint);
#define enroll A3
#define del A2
#define up A1
#define down A0
#define OK A4
#define door 13
#define Buzzer_read 4 //white
#define Buzzer 5 //blue
Servo servo1;
int pos = 0;// variable to store the servo position
int a =0;
int x =0;
void setup()
{
Serial.begin(9600);
delay(10);
67
pinMode(enroll, INPUT_PULLUP);
pinMode(up, INPUT_PULLUP);
pinMode(down, INPUT_PULLUP);
pinMode(del, INPUT_PULLUP);
pinMode(OK, INPUT_PULLUP);
pinMode(door, OUTPUT);
pinMode(Buzzer_read, INPUT);
pinMode(Buzzer, OUTPUT);
servo1.attach (6);
lcd.begin(16,2);
lcd.setCursor(0,0);
lcd.print("The Security System");
lcd.setCursor(0,1);
lcd.print("is running");
finger.begin(57600);
Serial.begin(9600);
lcd.setCursor(0,1);
delay(3000);
if (finger.verifyPassword())
{
Serial.println("Found fingerprint sensor!");
}
else
{
Serial.println("Did not find fingerprint sensor :(");
lcd.clear();
lcd.print("module not Found");
lcd.setCursor(0,1);
lcd.print("Check Connections");
while (1);
68
}
}
void loop()
{
lcd.clear ();
lcd.setCursor(0,0);
lcd.print("Press OK ");
lcd.setCursor(0,1);
lcd.print("to open module");
delay (1000);
if(digitalRead(OK)==0)
{
for(int i=0;i<5;i++)
{
lcd.clear();
lcd.print("Place Finger");
delay(2000);
int result=getFingerprintIDez();
if(result>=0)
{
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Press");
lcd.setCursor(0,1);
lcd.print ("Enroll/Delet");
delay(5000);
if(digitalRead(enroll) == 0 || digitalRead(del) == 0)
{
69
checkKeys();
delay(1000);
}
else {
digitalWrite(door, HIGH);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Gete opened");
x=1;
delay (500);
for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
// in steps of 1 degree
servo1.write(pos); // tell servo to go to position in variable 'pos'
delay(5); // waits 15ms for the servo to reach the position
}
for (a=0; a <=5; a+= 1)
{
lcd.clear ();
lcd.setCursor(0, 0);
lcd.print("Gete opened");
lcd.setCursor(0, 1);
lcd.print(a);
delay (2000);
}
delay (1000);
for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
70
servo1.write(pos); // tell servo to go to position in variable 'pos'
delay(5);
}
lcd.clear();
lcd.print("Gete closed");
digitalWrite(door, LOW);
x=0;
delay(1000);
}
return;
}
}
}
if (digitalRead(Buzzer_read)== HIGH){
beep ();
}
}
void checkKeys()
{
71
lcd.clear ();
lcd.setCursor (0 ,0);
lcd.print ("Press...");
lcd.setCursor (0,1);
lcd.print ("Enroll/Delete");
delay (5000);
if(digitalRead(enroll) == 0)
{
lcd.clear();
lcd.print("Please Wait");
delay(1000);
while(digitalRead(enroll) == 0);
Enroll();
}
else if(digitalRead(del) == 0)
{
lcd.clear();
lcd.print("Please Wait");
delay(1000);
delet();
}
}
void Enroll()
{
int count=0;
lcd.clear();
lcd.print("Enroll Finger ");
lcd.setCursor(0,1);
lcd.print("Location:");
while(1)
{

lcd.setCursor(9,1);
lcd.print(count);
if(digitalRead(up) == 0 )
{
count++;
if(count>25)
count=0;
delay(500);
}
else if(digitalRead(down) == 0)
{
count--;
if(count<0)
count=25;
delay(500);
}
else if(digitalRead(OK) == 0)
{
id=count;
getFingerprintEnroll();
return;
}
else if(digitalRead(enroll) == 0)
{
return;
}
}
}
void delet()
{

int count=0;
lcd.clear();
lcd.print("Delete Finger ");
lcd.setCursor(0,1);
lcd.print("Location:");
while(1)
{
lcd.setCursor(9,1);
lcd.print(count);
if(digitalRead(up) == 0)
{
count++;
if(count>25)
count=0;
delay(500);
}
else if(digitalRead(down) == 0)
{
count--;
if(count<0)
count=25;
delay(500);
}
else if(digitalRead(OK) == 0)
{
id=count;
deleteFingerprint(id);
return;
}
else if(digitalRead(enroll) == 0)
{

return;
}
}
}
uint8_t getFingerprintEnroll()
{
int p = -1;
lcd.clear();
lcd.print("finger ID:");
lcd.print(id);
lcd.setCursor(0,1);
lcd.print("Place Finger");
delay(2000);
while (p != FINGERPRINT_OK)
{
p = finger.getImage();
switch (p)
{
case FINGERPRINT_OK:
Serial.println("Image taken");
lcd.clear();
lcd.print("Image taken");
break;
case FINGERPRINT_NOFINGER:
Serial.println("No Finger");
lcd.clear();
lcd.print("No Finger");
break;
case FINGERPRINT_PACKETRECIEVEERR:
Serial.println("Communication error");
lcd.clear();
75
lcd.print("Comm Error");
break;
case FINGERPRINT_IMAGEFAIL:
Serial.println("Imaging error");
lcd.clear();
lcd.print("Imaging Error");
break;
default:
Serial.println("Unknown error");
lcd.clear();
lcd.print("Unknown Error");
break;
}
}
// OK success!
p = finger.image2Tz(1);
switch (p) {
case FINGERPRINT_OK:
Serial.println("Image converted");
lcd.clear();
lcd.print("Image converted");
break;
case FINGERPRINT_IMAGEMESS:
Serial.println("Image too messy");
lcd.clear();
lcd.print("Image too messy");
return p;
case FINGERPRINT_PACKETRECIEVEERR:
Serial.println("Communication error");
lcd.clear();
lcd.print("Comm Error");
76
return p;
case FINGERPRINT_FEATUREFAIL:
Serial.println("Could not find fingerprint features");
lcd.clear();
lcd.print("Feature Not Found");
return p;
case FINGERPRINT_INVALIDIMAGE:
Serial.println("Could not find fingerprint features");
lcd.clear();
lcd.print("Feature Not Found");
return p;
default:
Serial.println("Unknown error");
lcd.clear();
lcd.print("Unknown Error");
return p;
}
Serial.println("Remove finger");
lcd.clear();
lcd.print("Remove Finger");
delay(2000);
p = 0;
while (p != FINGERPRINT_NOFINGER) {
p = finger.getImage();
}
Serial.print("ID "); Serial.println(id);
lcd.clear();
lcd.print("ID ");
lcd.print(id);
delay (1000);
77
p = -1;
Serial.println("Place same finger again");
lcd.clear();
lcd.print("Place Finger");
lcd.setCursor(0,1);
lcd.print(" Again");
while (p != FINGERPRINT_OK) {
p = finger.getImage();
switch (p) {
case FINGERPRINT_OK:
Serial.println("Image taken");
break;
case FINGERPRINT_NOFINGER:
Serial.print(".");
break;
case FINGERPRINT_PACKETRECIEVEERR:
Serial.println("Communication error");
break;
case FINGERPRINT_IMAGEFAIL:
Serial.println("Imaging error");
break;
default:
Serial.println("Unknown error");
return;
}
}
// OK success!
p = finger.image2Tz(2);
switch (p) {
case FINGERPRINT_OK:
Serial.println("Image converted");
78
break;
case FINGERPRINT_IMAGEMESS:
Serial.println("Image too messy");
return p;
case FINGERPRINT_PACKETRECIEVEERR:
Serial.println("Communication error");
return p;
case FINGERPRINT_FEATUREFAIL:
Serial.println("Could not find fingerprint features");
return p;
case FINGERPRINT_INVALIDIMAGE:
Serial.println("Could not find fingerprint features");
return p;
default:
Serial.println("Unknown error");
return p;
}
// OK converted!
Serial.print("Creating model for #"); Serial.println(id);
p = finger.createModel();
if (p == FINGERPRINT_OK) {
Serial.println("Prints matched!");
} else if (p == FINGERPRINT_PACKETRECIEVEERR) {
Serial.println("Communication error");
return p;
} else if (p == FINGERPRINT_ENROLLMISMATCH) {
Serial.println("Fingerprints did not match");
return p;
} else {
79
Serial.println("Unknown error");
return p;
}
Serial.print("ID "); Serial.println(id);
lcd.clear();
lcd.print("ID ");
lcd.print(id);
delay (1000);
p = finger.storeModel(id);
if (p == FINGERPRINT_OK) {
Serial.println("Stored!");
lcd.clear();
lcd.print("Stored!");
delay(2000);
} else if (p == FINGERPRINT_PACKETRECIEVEERR) {
Serial.println("Communication error");
return p;
} else if (p == FINGERPRINT_BADLOCATION) {
Serial.println("Could not store in that location");
return p;
} else if (p == FINGERPRINT_FLASHERR) {
Serial.println("Error writing to flash");
return p;
}
else {
Serial.println("Unknown error");
return p;
}
}
int getFingerprintIDez()
80
{
uint8_t p = finger.getImage();
if (p != FINGERPRINT_OK)
return -1;
p = finger.image2Tz();
if (p != FINGERPRINT_OK)
return -1;
p = finger.fingerFastSearch();
if (p != FINGERPRINT_OK)
{
lcd.clear();
lcd.print("Finger Not Found");
lcd.setCursor(0,1);
lcd.print("Try Later");
delay(2000);
return -1;
}
// found a match!
Serial.print("Found ID #");
Serial.print(finger.fingerID);
return finger.fingerID;
}
uint8_t deleteFingerprint(uint8_t id)
{
uint8_t p = -1;
lcd.clear();
lcd.print("Please wait");
p = finger.deleteModel(id);
if (p == FINGERPRINT_OK)
{
81
Serial.println("Deleted!");
lcd.clear();
lcd.print("Figer Deleted");
lcd.setCursor(0,1);
lcd.print("Successfully");
delay(1000);
}
else
{
Serial.print("Something Wrong");
lcd.clear();
lcd.print("Something Wrong");
lcd.setCursor(0,1);
lcd.print("Try Again Later");
delay(2000);
return p;
}
}
void beep()
{
digitalWrite(Buzzer, HIGH);
delay(2000);
digitalWrite(Buzzer, LOW);
}