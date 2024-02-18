#define BLYNK_TEMPLATE_ID "TMPL6-NHX7KjS"
#define BLYNK_TEMPLATE_NAME "gpsv2"
#define BLYNK_AUTH_TOKEN "7LZILl2BeVEoDNqmvK5qdpI6C15yMFIg"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#include <ThingESP.h>

ThingESP8266 thing("adrielfs", "botGpsTracker", "botGpsTracker");

int RXPin = D2;
int TXPin = D1;
int GPSBaud = 9600;
int buttonState = 0;
int buzzPin = D0;
TinyGPSPlus gps;
SoftwareSerial gpsSerial(TXPin, RXPin);
char auth[] = "7LZILl2BeVEoDNqmvK5qdpI6C15yMFIg";
int LED = LED_BUILTIN;


void setup()
{
  Serial.begin(9600);
  gpsSerial.begin(GPSBaud);
  thing.SetWiFi("Andromax-M2S-E2C1", "07155201");
  pinMode(LED, OUTPUT);
  pinMode(buzzPin, OUTPUT);
  digitalWrite(buzzPin, LOW);
  thing.initDevice();

  Blynk.begin(auth,"Andromax-M2S-E2C1", "07155201");
  Blynk.setProperty(V2, "onLabel", "ON");
  Blynk.virtualWrite(V2, 0); 
  Blynk.syncVirtual(V2); 
}

String HandleResponse(String query)
{

  if(query == "on")
  {
  digitalWrite(LED, 0);
  return "gps tracker on!";
  }
  else if (query == "off")
  {
    digitalWrite(LED, 1);
    return "gps tracker off!";
  }
  else if (query == "send coordinates") {
    
    return "https://www.google.com/maps?q=" + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
  }
else
{
  return "invalid command!";
}

}

void loop()
{
  thing.Handle();
   while (gpsSerial.available() > 0)
  {
    if (gps.encode(gpsSerial.read()))
    {
      //displayInfo(); 
      sendToBlynk(); 
    }
  }

  Blynk.run();
  
}

void displayInfo()
{
  if (gps.location.isValid())
  {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Speed: ");
    Serial.println(gps.speed.kmph());
    Serial.print("Satellite: ");
    Serial.println(gps.satellites.value()); 
  }
  else
  {
    Serial.println("Location: Not Available");
  }

  Serial.println();
  Serial.println();
  delay(10000);
}

void sendToBlynk()
{
  if (gps.satellites.isValid())
  {
    Blynk.virtualWrite(V0, gps.location.lat(), 6); 
    Blynk.virtualWrite(V1, gps.location.lng(), 6); 
    Blynk.virtualWrite(V4, gps.speed.kmph());     
    Blynk.virtualWrite(V3, gps.satellites.value());

  }
  else
  {
    Blynk.virtualWrite(V0, gps.location.lat(), 6); 
    Blynk.virtualWrite(V1, gps.location.lng(), 6);
    Blynk.virtualWrite(V4, gps.speed.kmph()); 
    Blynk.virtualWrite(V3, gps.satellites.value());   

  }
}

BLYNK_WRITE(V2) {
  buttonState = param.asInt();
  
  if (buttonState == 1) {
    // Button pressed
    Serial.println("buzzer off");
    digitalWrite(buzzPin, LOW); 
  } else {
    // Button released
    Serial.println("buzzer on");
    digitalWrite(buzzPin, HIGH);
  }
}
