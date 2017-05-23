#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <stdlib.h>
#include <dht.h>

#define DEBUG true
// PushingBox scenario DeviceId code and API
//String deviceId = "v961B38A855B4458";
#define DHT11_PIN 7                   // DHT data pin connected to Arduino 
#define SSID "D***P"                  // "SSID-WiFiname"
#define PASS "DDD******999"         // "password"
#define IP "184.106.153.149"       // thingspeak.com ip

//const char* logServer = "api.pushingbox.com";
String msg = "GET /update?key=66H******PEGGI";     // ThingSpeak API key
String API_KEY = "T91IL********4Q";  //Thingtweet
SoftwareSerial esp8266(8,9);     //Rx,Tx
LiquidCrystal lcd(12,11,5,4,3,2);
dht DHT;// Initialize the DHT sensor

//Variables
//double temp;
//double humid;
int chk;
float temp;
float hum;
int sensorValue;
int pResistor;
int led1 = A5;
int led2 = A4;
int led3 = A3;
int led4 = 13;
int motorPin = 6;
int error;

void setup()
{

//pinMode(buttonPin,INPUT);   
pinMode (led1,OUTPUT); 
pinMode (led2,OUTPUT); 
pinMode (led3,OUTPUT); 
pinMode (led4,OUTPUT);
pinMode (motorPin,OUTPUT);
//pinMode (servoPin,OUTPUT);
//pinMode(backLight, OUTPUT);
  Serial.begin(115200); //or use default 9600.
  Serial.println("SMART GARDEN");
  Serial.println(" ");
  Serial.println("Weather Station");
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Smart Garden");
  delay(100);
  lcd.setCursor(0,1);
  lcd.print("Connecting...");
  esp8266.begin(115200);
  Serial.println("AT");
  esp8266.println("AT");
 

 delay(1000);

 if(esp8266.find("OK"))
  {
    connectWiFi();
    postTweet();
  }
}



void loop()
   {
  //lcd.clear();
  lcd.setCursor(0,0);
  lcd.autoscroll();
  start: //label
  error=0;
  chk = DHT.read11(DHT11_PIN);
  hum = DHT.humidity;
  temp = DHT.temperature;
  sensorValue = analogRead(A0); 
  pResistor = analogRead(A1);
  delay(1000);

   updateTemp();
    //Resend if transmission is not completed
   if (error==1)
   {
      goto start; //go to label "start"
   } 


lcd.print("Rd: ");
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


lcd.print("H= ");
lcd.print((float)DHT.humidity);
lcd.print("%");
lcd.print(" ");


lcd.print("T= ");
lcd.print((float)DHT.temperature);
lcd.print("C");
lcd.print(" ");


lcd.setCursor(0 , 1);

lcd.print("M= ");
lcd.print(sensorValue);
lcd.print(" ");


lcd.print("L= ");
lcd.print(pResistor);
lcd.print(" ");


if (DHT.humidity > 60) {
  digitalWrite (led1, HIGH);
  //digitalWrite(motorPin, HIGH);
  //Servo1.write(180);
} else {
  digitalWrite (led1, LOW);
  // digitalWrite(motorPin, LOW);
  // Servo1.write(0);
}

if (DHT.temperature > 40) {
  digitalWrite (led2, HIGH);
  //digitalWrite(motorPin, HIGH);
  //Servo1.write(180);
} else {
  digitalWrite (led2, LOW);
  //digitalWrite(motorPin, LOW);
  // Servo1.write(0);
}

if (sensorValue < 600) {
  digitalWrite(led3, LOW);
  digitalWrite(motorPin, LOW);
  //status=Plants are watered!
  //Servo1.write(180);
}
else {
  digitalWrite(led3, HIGH);
  digitalWrite(motorPin, HIGH);
  //Servo1.write(0);
}


if (pResistor < 200) {
  //lcd.print("LIGHT ON");
  digitalWrite(led4, HIGH);
  // status=Light is ON!
}
else {
  digitalWrite(led4, LOW);
}
delay(3000);
}

 void updateTemp()
 {
 String cmd = "AT+CIPSTART=\"TCP\",\"";
 cmd += IP;
 cmd += "\",80";
 Serial.println(cmd);
 esp8266.println(cmd);
 delay(2000);
 if(esp8266.find("Error"))
 {
    return;
 }
 cmd = msg ;
 cmd += "&field1=";    //field 1 for temperature
 cmd += String(temp);
 cmd += "&field2=";  //field 2 for humidity
 cmd += String(hum);
 cmd += "&field3=";    //field 3 for moisture 
 cmd += String(sensorValue);
 cmd += "&field4=";  //field 4 for light
 cmd += String(pResistor);
 cmd += "\r\n";
 Serial.print("AT+CIPSEND=");
 esp8266.print("AT+CIPSEND=");
 Serial.println(cmd.length());
 esp8266.println(cmd.length());
 if(esp8266.find(">"))
 {
    Serial.print(cmd);
    esp8266.print(cmd);
 }
 else
 {
    Serial.println("AT+CIPCLOSE");
    esp8266.println("AT+CIPCLOSE");
    //Resend...
    error=1;
 }

}
 boolean connectWiFi()
  {
 Serial.println("AT+CWMODE=3");
 esp8266.println("AT+CWMODE=3");
 delay(2000);
 String cmd="AT+CWJAP=\"";
 cmd+=SSID;
 cmd+="\",\"";
 cmd+=PASS;
 cmd+="\"";
 Serial.println(cmd);
 esp8266.println(cmd);
 delay(2000);
 if(esp8266.find("OK"))
  {
    return true;
  }
  else
  {
    return false;
  }
   
  }
 boolean postTweet( String tweet) {
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  esp8266.print(cmd);
  delay(2000);
  if (esp8266.find("Error")) {
    if (DEBUG) Serial.print("RECEIVED: Error");
    Serial.println("Module error");
    return false;
  }
  String tweetData = "api_key=T91ILB0VJS0P7W4Q";
  tweetData += API_KEY;
  tweetData += "&status=Hey Deep your garden is completely fine!";
  tweetData += tweet;
  cmd = "POST /apps/thingtweet/1/statuses/update HTTP/1.1\n";
  cmd += "Host: api.thingspeak.com\n";
  cmd += "Connection: close\n";
  cmd += "Content-Type: application/x-www-form-urlencoded\n";
  cmd += "Content-Length: ";
  cmd += tweetData.length();
  cmd += "\n\n";
  cmd += tweetData;
  cmd += "\r\n";
  esp8266.print("AT+CIPSEND=");
  esp8266.println(cmd.length());
  if (esp8266.find(">")) {
    if (DEBUG) Serial.print(">");
    if (DEBUG) Serial.print(cmd);
    esp8266.print(cmd);
  } else {
    Serial.println("AT+CIPCLOSE");
    esp8266.print("AT+CIPCLOSE");
  }
  if (esp8266.find("OK")) {
    if (DEBUG) Serial.println("RECEIVED: OK");
    Serial.println("Tweet posted!"); 
    return true;
  } else {
    if (DEBUG) Serial.println("RECEIVED: Error");
    Serial.println("Server error");
    return false;
  }
}
