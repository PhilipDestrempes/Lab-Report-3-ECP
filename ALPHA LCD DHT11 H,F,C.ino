/* This program will demonstarte the reading of tempurature in celcius, fahrenheit and display humidity 
using a DHT22 temperature and humidity sensor. The readins will then be displayed on the serial 
monitor and an lcd (using the liquid crystal library) */

#include <LiquidCrystal.h> //include the liquid crystal library for lcd 

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //declare the variables from the library 

byte customChar[8] = { //use the byte function to display the degree symbol 
    
  0b00000,
	0b11000,
	0b11000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000
};



#include <Adafruit_Sensor.h> //include Adafruit_Sensor.h
#include <DHT.h> //include DHT.h
#include <DHT_U.h> //include DHT_U.h 

#define DHTPIN 7     // Digital pin connected to the DHT sensor 

// Uncomment the type of sensor in use:
//#define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT_Unified dht(DHTPIN, DHTTYPE); //set the DHT pin and type 

uint32_t delayMS; //set uint32_t delayMS 

void setup() {
  Serial.begin(9600); //serial begin at 9600 bps
  lcd.createChar(0, customChar); //set the custom byte for degrees 
  lcd.begin(16, 2); //lcd display will take 16 coloums and 2 rows 
  // Initialize device.
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
 
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  
  delayMS = sensor.min_delay / 1000; // Set delay between sensor readings based on sensor details.
 
}

void loop() {

  delay(delayMS); // Delay between measurements.
  sensors_event_t event; // Get temperature event and print its value.
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) { //if the temperature cannot be read 
    Serial.println(F("Error reading temperature!")); //serial print "error reading temperature"

    delay(1000); //delay 1 second 
  }
  else { //else 
    Serial.print(F("Temperature Celcius: ")); //serial print "Temperature Celcius: "
    Serial.print(event.temperature); //serial print the temperature reading 
    Serial.println(F("°C")); //serial print "°C"

    Serial.print(F("Temperature Fahrenheit: ")); //serial print
    float Fvalue = ((event.temperature*9/5) + 32); //equation to convert celcius to fahrenheit 
    Serial.print(Fvalue); //serial print temperature in fahrenhit 
    Serial.println(F("°F")); //serial print "°F"

    lcd.setCursor(0,0); //lcd start at column 0 row 0 
    lcd.print("T=" ); //lcd print "T= ""
    lcd.print(event.temperature); //lcd print temperature reading 
    lcd.write((byte)0); //lcd write the custom byte from earlier (degree symbol)
    lcd.print("C"); //lcd print "C"
    lcd.print(Fvalue); //lcd print the temperature in fahrenheit 
    lcd.write((byte)0); //lcd write the custom byte from earlier (degree symbol)
    lcd.print("F"); //lcd print "F"

  
  }
  dht.humidity().getEvent(&event); // get humidity event and print its value.
  if (isnan(event.relative_humidity)) { //if temperature cannot be read 
    Serial.println(F("Error reading humidity!")); //serial print "error reading humidity"

    delay(1000); //delay 1 second 
  }
  else { //else 
    Serial.print(F("Humidity: ")); //serial print "humidity: " 
    Serial.print(event.relative_humidity); //serial print humidity reading 
    Serial.println(F("%")); //serial print "%"
  

    lcd.setCursor(0,1); //set lcd cursor to column 0 row 1 
    lcd.print("H= "); //lcd print "H= "
    lcd.print(event.relative_humidity); //lcd print humidity reading 
    lcd.print("%"); //lcd print "%"

  
  }
   
}
