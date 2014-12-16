// Test sketch for the DHT22 and photocell sensors

// Include required libraries
#include "DHT.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>


// Variables
int lightLevel;
float humidity;
float temperature;

unsigned long time;

// DHT11 sensor pins
#define DHTPIN 8 
#define DHTTYPE DHT22

// DHT 
DHT dht(DHTPIN, DHTTYPE);

                                         
void setup(void)
{
 
  // Initialize DHT sensor
  dht.begin();
  
  // Init serial
  Serial.begin(115200);
  
}
  
void loop(void)
{
    // Read temperature as Celsius
    float t = dht.readTemperature();  
   // Read temperature as Fahrenheit
    float f = dht.readTemperature(true);
    
  
    // Measure the humidity
    float h = dht.readHumidity();
    
    // Measure light level
    int lightLevel = analogRead(A0);
    
    // Compute heat index
    // Must send in temp in Fahrenheit!
    float hi = dht.computeHeatIndex(f, h);
  
    // Print measurements
    Serial.print("Humidity: ");
    Serial.println(h);
    Serial.print("");
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print(f);
    Serial.print(" *F\t");
    Serial.print("Heat index: ");
    Serial.print(hi);
    Serial.println(" *F");
    Serial.print("Light level: ");
    Serial.println(lightLevel);
    
    // Repeat 300 ms
    delay(300);
  
}
