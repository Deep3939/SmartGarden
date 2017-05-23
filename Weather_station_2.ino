#include <LiquidCrystal.h>
#include <stdlib.h>
#include <dht.h>


#define DHT11_PIN 7                
LiquidCrystal lcd(12,11,5,4,3,2);
dht DHT;

//Variables
int chk;
int sensorValue;
int pResistor;
int led1 = A5;
int led2 = A4;
int led3 = A3;
int led4 = 13;
int motorPin = 6;

void setup()
{

//pinMode(buttonPin,INPUT);   
pinMode (led1,OUTPUT); 
pinMode (led2,OUTPUT); 
pinMode (led3,OUTPUT); 
pinMode (led4,OUTPUT);
pinMode (motorPin,OUTPUT);
//pinMode(backLight, OUTPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Smart Garden");
  delay(100);
  lcd.setCursor(0,1);
  lcd.print("Connecting...");
  Serial.begin(115200); 
  Serial.println("SMART GARDEN");
  Serial.println(" ");
  Serial.println("Weather Station");
  delay(1000);

}

void loop()
   {
 // lcd.clear();
  lcd.setCursor(0 , 0);
  lcd.autoscroll();
  int chk = DHT.read11(DHT11_PIN);
  int humid = DHT.humidity;
  int temp =  DHT.temperature;
  int sensorValue = analogRead(A0); 
  int pResistor = analogRead(A1);

lcd.print("Rd:");
switch (chk)
{
case DHTLIB_OK:
  lcd.print("OK");
  lcd.print(" ");
  break;
case DHTLIB_ERROR_CHECKSUM:
  lcd.print("Checksum error");
  lcd.print(" ");
  break;
case DHTLIB_ERROR_TIMEOUT:
  lcd.print("Time out error");
  lcd.print(" ");
  break;
default:
  lcd.print("Unknown error");
  lcd.print(" ");
  break;
}

lcd.print("H=");
lcd.print((float)DHT.humidity);
lcd.print("%");
lcd.print(" ");


lcd.print("T=");
lcd.print((float)DHT.temperature);
lcd.print("C");
lcd.print(" ");


lcd.setCursor(0 , 1);

lcd.print("M=");
lcd.print(sensorValue);
lcd.print(" ");


lcd.print("L=");
lcd.print(pResistor);
lcd.print(" ");


if (DHT.humidity > 55) {
  digitalWrite (led1, HIGH);
  //digitalWrite(motorPin, HIGH);
} else {
  digitalWrite (led1, LOW);
  // digitalWrite(motorPin, LOW);
}

if (DHT.temperature > 40) {
  digitalWrite (led2, HIGH);
  //digitalWrite(motorPin, HIGH);
} else {
  digitalWrite (led2, LOW);
  //digitalWrite(motorPin, LOW);
}

if (sensorValue < 600){
  digitalWrite(led3, LOW);
  digitalWrite(motorPin, LOW);
}
else {
  digitalWrite(led3, HIGH);
  digitalWrite(motorPin, HIGH);
}


if (pResistor < 150) {
  //lcd.print("LIGHT ON");
  digitalWrite(led4, HIGH);
}
else {
  digitalWrite(led4, LOW);
}
delay(3000);
}



