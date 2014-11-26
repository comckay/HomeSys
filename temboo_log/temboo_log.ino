// Include required libraries
#include <Bridge.h>
#include <Temboo.h>
#include <Process.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>

// Contains Temboo account information
#include "TembooAccount.h" 

// Variables
int lightLevel;
float humidity;
float temperature;
unsigned long time;

float temperature_limit;

// Process to get the measurement time
Process date;

// Your Google Docs data
const String GOOGLE_USERNAME = "xxxxx@gmail.com";
const String GOOGLE_PASSWORD = "xxxx";
const String SPREADSHEET_TITLE = "xxxxx";
const String TO_EMAIL_ADDRESS = "xxxx@xx.com";

// Include required libraries
#include "DHT.h"

// DHT11 sensor pins
#define DHTPIN 8 
#define DHTTYPE DHT22

// DHT11 sensor pins
#define DHTPIN2 9
#define DHTTYPE2 DHT22

// DHT & BMP instances
DHT dht(DHTPIN, DHTTYPE);

// DHT & BMP instances
DHT dhtin(DHTPIN2, DHTTYPE2);

// Debug mode ?
boolean debug_mode = false;

void setup() {
  
  // Start Serial
  if (debug_mode == true){
    Serial.begin(115200);
    delay(4000);
    while(!Serial);
  }
  
  // Initialize DHT sensor
  dht.begin();
  
  // Initialize DHT2 sensor
  dhtin.begin();
  
  // Start bridge
  Bridge.begin();
  
  // Start date process
  time = millis();
  if (!date.running())  {
    date.begin("date");
    date.addParameter("+%D-%T");
    date.run();
  }

  if (debug_mode == true){
    Serial.println("Setup complete. Waiting for sensor input...\n");
  }
}
  


void loop() {
  
  // Measure the humidity & temperature
  humidity = dht.readHumidity();
  
   // Read temperature as Fahrenheit
   float temperature = dht.readTemperature(true);
    
  // Measure light level
  int lightLevel = analogRead(A0);
  
  // Compute heat index
    // Must send in temp in Fahrenheit!
  float hi = dht.computeHeatIndex(temperature, humidity);
	
	 // Measure the humidity & temperature inside
   float humidityinside = dhtin.readHumidity();
  
   // Read temperature as Fahrenheit
   float temperatureinside = dhtin.readTemperature(true);
   
    // Compute heat index
    // Must send in temp in Fahrenheit!
    float hiinside = dhtin.computeHeatIndex(temperatureinside, humidityinside);
  

  if (debug_mode == true){
    Serial.println("\nCalling the /Library/Google/Spreadsheets/AppendRow Choreo...");
  }
  
  // Append data to Google Docs sheet
  runAppendRow(humidity, lightLevel, temperature, hi, humidityinside, temperatureinside, hiinside);
  
        
  // Repeat every x minutes
  delay(267840);
}

// Function to add data to Google Docs
void runAppendRow(float humidity, int lightLevel, float temperature, float hi, float humidityinside, float temperatureinside, float hiinside) {
  TembooChoreo AppendRowChoreo;

  // Invoke the Temboo client
  AppendRowChoreo.begin();

  // Set Temboo account credentials
  AppendRowChoreo.setAccountName(TEMBOO_ACCOUNT);
  AppendRowChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
  AppendRowChoreo.setAppKey(TEMBOO_APP_KEY);
  
  // Identify the Choreo to run
  AppendRowChoreo.setChoreo("/Library/Google/Spreadsheets/AppendRow");

  // your Google username (usually your email address)
  AppendRowChoreo.addInput("Username", GOOGLE_USERNAME);

  // your Google account password
  AppendRowChoreo.addInput("Password", GOOGLE_PASSWORD);
  
  // the title of the spreadsheet you want to append to
  AppendRowChoreo.addInput("SpreadsheetTitle", SPREADSHEET_TITLE);
  
  // Restart the date process:
  if (!date.running())  {
    date.begin("date");
    date.addParameter("+%D-%T");
    date.run();
   }
  
  // Get date
  String timeString = date.readString(); 
  
  // Format data
  String data = "";
  data = data + timeString + "," + String(humidity) + "," + String(lightLevel) + "," + String(temperature) + "," + String(hi) + "," + String(humidityinside) + "," + String(temperatureinside) + "," + String(hiinside);

  // Set Choreo inputs
  AppendRowChoreo.addInput("RowData", data);

  // Run the Choreo
  unsigned int returnCode = AppendRowChoreo.run();

  // A return code of zero means everything worked
  if (returnCode == 0) {
    if (debug_mode == true){
      Serial.println("Completed execution of the /Library/Google/Spreadsheets/AppendRow Choreo.\n");
    }
  } else {
    // A non-zero return code means there was an error
    // Read and print the error message
    while (AppendRowChoreo.available()) {
      char c = AppendRowChoreo.read();
      if (debug_mode == true){ Serial.print(c); }
    }
    if (debug_mode == true){ Serial.println(); }
  }
  AppendRowChoreo.close();
}

