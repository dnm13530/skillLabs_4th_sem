#define DHTPIN 4          // Pin connected to the DHT sensor (G4 on ESP32)
#define DHTTYPE DHT11     // DHT 11 sensor type
#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL35UVMq51g"
#define BLYNK_TEMPLATE_NAME "Temperature and Humidity Sensor"
#define BLYNK_AUTH_TOKEN "oP9L4wfQXxWQxFi_RobTxb4sz5dvR-oL"

#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Jagadeeshwari";     // Type your WiFi name
char pass[] = "my life 1234";   // Type your WiFi password

DHT dht(DHTPIN, DHTTYPE);

// Define LED pin
#define LED_PIN 2         // Pin connected to the LED

void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting the setup"));
  Blynk.begin(auth, ssid, pass);

  dht.begin();
  // Wait a few seconds between measurements.
  delay(2000);

 // pinMode(LED_PIN, OUTPUT); // Initialize the LED pin as an output
  //digitalWrite(LED_PIN, HIGH); // Ensure the LED is off initially
  Serial.println(F("DHTxx test!"));
}

void loop() {
  Blynk.run();
  
  // Reading temperature and humidity data  // 
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from the DHT sensor!"));
    return;
  }

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println("");
  // Serial.print(F("°C  Heat index: "));
  // Serial.print(hic);
  // Serial.println(F("°C"));

  // Send data to Blynk
  Blynk.virtualWrite(V5, h);  // Send humidity to virtual pin V5
  Blynk.virtualWrite(V6, t);  // Send temperature to virtual pin V6

  //************TODO*************************
  // TASK : 1
  // Description : Push Heat index data to iot cloud
  //******************************************

  //*******************************************
  // TASK : 2
  //Description : Trigger an event upon reaching a thershold of temperature/humidity/heat-index
  //Below is an example of simple condition were the temperature is greater than 25 degrees 
  //   raise an alert.
  // if(t > 25.00){
  //   Blynk.logEvent("tempalert","Temperature is high");
  // }
  // Note : Event definition should be created in the blynk IoT cloud before configuring it. 
  //********************************************

}

BLYNK_WRITE(V1) { // Button Widget is writing to pin V1
  int buttonState = param.asInt();
  
  Serial.print(F("Button state: "));
  Serial.println(buttonState);

  if (buttonState == 1) { // If button is pressed
    Serial.println(F("Button pressed"));
    digitalWrite(LED_PIN, HIGH); // Turn on the LED
  } else {
    digitalWrite(LED_PIN, LOW);  // Turn off the LED
    Serial.println(F("Button released"));
  }
}