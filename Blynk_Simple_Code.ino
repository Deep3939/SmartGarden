#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SimpleTimer.h>
#include <dht.h>


char auth[] = "34fe2e17274e4a9abfb2568a35ee3c21";
char ssid[] = "DEEP";
char pass[] = "DDD333ddd999";

#include <SoftwareSerial.h>
SoftwareSerial EspSerial(8,9); // RX, TX

#define ESP8266_BAUD 115200
//#define IPAddress "192.168.1.100" 

ESP8266 wifi(&EspSerial);

#define DHT11_PIN 7          

dht DHT;
SimpleTimer timer;
void sendSensor()
{
  int chk = DHT.read11(DHT11_PIN);
  float h = DHT.humidity;
  float t = DHT.temperature; 
  int s = analogRead(A0); 
  int p = analogRead(A1);

  if (isnan(h) || isnan(t) || isnan(s) || isnan(p)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V0, h);
  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, s);
  Blynk.virtualWrite(V3, p);
}

void setup()
{
  
  Serial.begin(115200);

 
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);
 // Blynk.begin(auth, wifi, ssid, pass, "blynk-cloud.com", 8442);
// Blynk.begin(auth, wifi, ssid, pass, IPAddress(192,168,1,100), 8442);
 Blynk.email( "SMART GARDEN", "Your garden is online.");

  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates SimpleTimer
}
