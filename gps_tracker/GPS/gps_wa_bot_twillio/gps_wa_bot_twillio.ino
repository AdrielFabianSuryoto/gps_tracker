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
TinyGPSPlus gps;
SoftwareSerial gpsSerial(TXPin, RXPin);
char auth[] = "7LZILl2BeVEoDNqmvK5qdpI6C15yMFIg";
int LED = LED_BUILTIN;
// unsigned long Millisz = 5000;
// unsigned long currentMillisz = millis();
void setup()
{
  Serial.begin(9600);
  gpsSerial.begin(GPSBaud);

  thing.SetWiFi("58C", "12345678");
  pinMode(LED, OUTPUT);
  thing.initDevice();

  Blynk.begin(auth,"58C","12345678");
}

String HandleResponse(String query)
{
//if (gps.speed.isValid() == false)
//{
  //return "kendaraan anda menjauhi anda";
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

  else if (query == "led off") {
    
    return "Done: LED Turned OFF";
  }

  else if (query == "led status")
  {
    return digitalRead(LED) ? "LED is OFF" : "LED is ON";
  }
 
//}
else
{
  return "kendaraan anda masih di tempat!";
}

}

void loop()
{
  thing.Handle();
   while (gpsSerial.available() > 0)
  {
    if (gps.encode(gpsSerial.read()))
    {
      //displayInfo(); // Tampilkan informasi GPS di Serial Monitor
      sendToBlynk(); // Kirim data ke Blynk
    }
  }

  // if (millis() > Millisz && gps.charsProcessed() < 10)
  // {
  //   Serial.println("No GPS detected");
  //   while (true)
  //   {yield();}
  // }
  //yield();
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
    //Serial.print();
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
    // Kirim data ke aplikasi Blynk
    Blynk.virtualWrite(V0, gps.location.lat(), 6); 
    Blynk.virtualWrite(V1, gps.location.lng(), 6); 
    Blynk.virtualWrite(V2, gps.speed.kmph());     
    Blynk.virtualWrite(V3, gps.satellites.value());

  }
  else
  {
    Blynk.virtualWrite(V0, gps.location.lat(), 6); 
    Blynk.virtualWrite(V1, gps.location.lng(), 6);
    Blynk.virtualWrite(V2, gps.speed.kmph()); 
    Blynk.virtualWrite(V3, gps.satellites.value());   

  }
}
