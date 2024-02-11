#define BLYNK_TEMPLATE_ID "TMPL6-NHX7KjS"
#define BLYNK_TEMPLATE_NAME "gpsv2"
#define BLYNK_AUTH_TOKEN "7LZILl2BeVEoDNqmvK5qdpI6C15yMFIg"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <BlynkSimpleEsp8266.h>

int RXPin = D2;
int TXPin = D1;
int BuzzerPin = D0;
int GPSBaud = 9600;
unsigned long Millisz = 5000;
unsigned long currentMillisz = millis();
TinyGPSPlus gps;
SoftwareSerial gpsSerial(TXPin, RXPin);


char auth[] = "7LZILl2BeVEoDNqmvK5qdpI6C15yMFIg";

void setup()
{
  Serial.begin(9600);
  pinMode(BuzzerPin, OUTPUT);
  gpsSerial.begin(GPSBaud);
  Blynk.begin(auth,"OPPO Reno6","hartono13");
}


void loop()
{
  Serial.println("masuk");
  Blynk.virtualWrite(V0, "masuk");
  digitalWrite(BuzzerPin, HIGH);
  delay(200);
  digitalWrite(BuzzerPin, LOW);
  delay(200);  
  while (gpsSerial.available() > 0)
  {
    if (gps.encode(gpsSerial.read()))
    {
      Serial.println("tes");
      displayInfo();
      sendToBlynk(); 
    }
    else
    {
      Serial.println("No GPS detected");
    }
  }

  // if (currentMillisz > Millisz && gps.charsProcessed() < 10)
  // {
  //   Serial.println("No GPS detected");
  //   while (true);
  // }

  Blynk.run();
}

//display serial monitor
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
  delay(1000);
}

// kirim data esp ke blynk
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
