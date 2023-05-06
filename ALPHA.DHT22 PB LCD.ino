/* The following program will demonstrate a DHT22 temperature and humidity sensor displaying temperature in celcius, fahrenhit and 
humidity on a serial monitor and lcd display using a pushbutton. The pushbutton will act as a means to display each of the three types
of readings. If the pushbutton is pressed once, it will display temperature in celcius, if it is pressed twice it will display temperature 
in fahrenhit and if it is pressed a third time humidity. At the fourth press, the displays will default to celcius. This will be done using 
the lcd liquid crystal library. 
 */

int button= 13; //set integer for button at value 13
int PBV= 0; //set integer for PBV at value 0 

#include <LiquidCrystal.h> //include liquid crystal library for lcd 

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

byte customChar[8] = { //include byte chart for degree symbol on lcd display 
    
  0b00000,
	0b11000,
	0b11000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000
};

#include <Adafruit_Sensor.h> 
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 7     //pin connected to the DHT sensor 

// Uncomment the type of sensor in use:
//#define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT_Unified dht(DHTPIN, DHTTYPE); 

uint32_t delayMS;

void setup() { 

pinMode(button, INPUT_PULLUP); //set button as an INPUT pull up 

  Serial.begin(9600); //begin serial communication at 9600 bps 
  lcd.createChar(0, customChar); //set the byte chart 
  lcd.begin(16, 2); //lcd display will take 16 coloums and 2 rows 
  dht.begin(); //begin dht communication 
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
  
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

void loop() { 

delay(delayMS); // Delay between measurements.
sensors_event_t event;
dht.temperature().getEvent(&event); 
  
if (isnan(event.temperature)) { //if temperature cannot be read 
Serial.println(F("Error reading temperature!")); //display error message on serial monitor 
lcd.setCursor(0,0); //set lcd cursors to row 0 colum 0 
lcd.println("Error"); //display error 
}

else{
  
int PB= digitalRead(button); //digital read the pushbutton state 

//****note that pushbutton must be pressed for around 3-5 seconds to allow proper reading 

if (PB == LOW) {    //if pushbutton state is low 
PBV= PBV+1; //at 1 to counter PBV

  if (PBV > 3) { //if PBV is greater than 3 
  PBV = 1; //PBV is equal to 0 
}
  
switch (PBV) { //when PBV is TRUE 

    case 1: //in the case that PBV is equal to 1 
      Serial.print(F("Temperature Celsius: ")); //serial print "Temperature Celsius:"
      Serial.print(event.temperature); //serial print the temperature reading 
      Serial.println(F("°C")); //serial print degrees C
      lcd.setCursor(0,0); //set lcd cursor to row 0 colum 0 
      lcd.print(F("Temp C: ")); //lcd print "Temp C: "
      lcd.print(event.temperature); //lcd print the temperature reading 
      lcd.write(customChar[8]); //lcd print the degree symbol 
      lcd.print("C"); //lcd print C
      break; //break case 1 

    case 2: //in the case that PBV is equal to 2
      Serial.print(F("Temperature Fahrenheit: ")); //serial print "Temperature Fahrenheit:"
      Serial.print(event.temperature * 1.8 + 32); //serial print the temperature reading and convert to fahrenheit
      Serial.println(F("°F")); //serial print degrees F 
      lcd.setCursor(0,0); //set lcd cursor to row 0 colum 0 
      lcd.print(F("Temp f: ")); //lcd print "Temp f: "
      lcd.print(event.temperature* 1.8 + 32); //lcd print the temperature reading 
      lcd.write(customChar[8]); //lcd print the degree symbol 
      lcd.print("F"); //lcd print F
      break; //break case 2

    case 3: //in the case that PBV is equal to 3
      Serial.print(F("Humidity: ")); //serial print "Humidity"
      Serial.print(event.relative_humidity); //serial print the humidity reading 
      Serial.println(F("%")); //serial print "%""
      lcd.setCursor(0,0); //set lcd cursor to row 0 colum 0 
      lcd.print(F("Hum: ")); //lcd print "Hum: "
      lcd.print(event.relative_humidity); //lcd print the humidity reading 
      lcd.print("%"); 
      break; //break case 3
  }
}
}
}
