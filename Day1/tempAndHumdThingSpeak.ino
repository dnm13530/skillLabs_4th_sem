#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

// Replace with your network credentials

const char* ssid = "Jagadeeshwari";     // Type your WiFi name
const char* pass = "my life 1234";

// ThingSpeak settings
const char* server = "http://api.thingspeak.com";
String apiKey = "1DKRHWJGIMRSC3AH"; // Replace with your ThingSpeak API key

// DHT Sensor settings
#define DHTPIN 4       // GPIO pin where the DHT11/DHT22 is connected
#define DHTTYPE DHT11  // DHT11 or DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(10);
  
  // Connect to Wi-Fi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  dht.begin();
}

void loop() {
  // Reading temperature and humidity
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print to Serial
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %");

  // Check WiFi connection
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    // Prepare URL for ThingSpeak update
    String url = String(server) + "/update?api_key=" + apiKey + "&field1=" + String(t) + "&field2=" + String(h);
    
    http.begin(url);
    int httpCode = http.GET();
    
    // Check for the returning code
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
    } else {
      Serial.println("Error on HTTP request");
    }
    
    http.end(); // Free the resources
  } else {
    Serial.println("WiFi Disconnected");
  }

  // Wait 20 seconds before the next loop
  delay(20000);
}