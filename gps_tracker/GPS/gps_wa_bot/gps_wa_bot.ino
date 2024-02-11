#define BLYNK_TEMPLATE_ID "TMPL66HIRlOAI"
#define BLYNK_TEMPLATE_NAME "gps"
#define BLYNK_AUTH_TOKEN "nA31H2OMoECuq_bB8xoEFlwRLL9cOVsV"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <UrlEncode.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <BlynkSimpleEsp8266.h>

const char* ssid = "58C";
const char* password = "12345678";

int RXPin = D1;
int TXPin = D2;
int GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial gpsSerial(TXPin, RXPin);
char auth[] = "nA31H2OMoECuq_bB8xoEFlwRLL9cOVsV";

// +international_country_code + phone number
// Portugal +351, example: +351912345678
String phoneNumber = "+6285232189863";
String apiKey = "7079402";
unsigned long lastMessageTime = 0; 
const unsigned long messageInterval = 60000;
String gmapsURL;
void sendMessage(String message){

  // Data to send with HTTP POST
  String url = "http://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);
  WiFiClient client;    
  HTTPClient http;
  http.begin(client, url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200){
    Serial.print("Message sent successfully");
  }
  else{
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();
}

void loop()
{
  while (gpsSerial.available() > 0)
  {
    if (millis() - lastMessageTime >= messageInterval) {
    sendMessage("Google Maps URL: " + gmapsURL);
    lastMessageTime = millis(); // Update waktu terakhir pesan dikirim
  }
    if (gps.encode(gpsSerial.read()))
    {
      displayInfo(); // Tampilkan informasi GPS di Serial Monitor
      sendToBlynk(); // Kirim data ke Blynk
    }
  }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    while (true)
      ;
  }
  gmapsURL = "https://www.google.com/maps?q=" + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
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
  delay(1000);
}

void sendToBlynk()
{
  if (gps.satellites.isValid())
  {
    // Kirim data ke aplikasi Blynk
    Blynk.virtualWrite(V0, gps.location.lat(), 6);  // Gunakan virtual pin V7 untuk latitude
    Blynk.virtualWrite(V1, gps.location.lng(), 6); // Gunakan virtual pin V6 untuk longitude
    Blynk.virtualWrite(V2, gps.speed.kmph());      // Gunakan virtual pin V8 untuk kecepatan
    Blynk.virtualWrite(V3, gps.satellites.value());
    gmapsURL = "https://www.google.com/maps?q=" + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
  }
  else
  {
    Blynk.virtualWrite(V0, gps.location.lat(), 6);  // Gunakan virtual pin V7 untuk latitude
    Blynk.virtualWrite(V1, gps.location.lng(), 6); // Gunakan virtual pin V6 untuk longitude
    Blynk.virtualWrite(V2, gps.speed.kmph()); 
    Blynk.virtualWrite(V3, gps.satellites.value());   
    gmapsURL = "https://www.google.com/maps?q=" + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
  }
}

void setup() {
  //Serial.begin(115200);
  Serial.begin(9600);
  gpsSerial.begin(GPSBaud);
  Blynk.begin(auth,"58C","12345678");
  
  //String gmapsURL = "https://www.google.com/maps?q=" + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  

} 