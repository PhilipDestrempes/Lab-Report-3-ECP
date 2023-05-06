/* In this program we will be displaying on serial and lcd display the temperature (C and F) and humidity using a 
DHT22 sensor and will display distance using an ultrasonic sensor. The temperature and humidity readings will be controlled 
using a pushbutton counting from 1-3, 1 displaying the temperature in C, 2 displaying the temperature in F and 3 displaying
the humidity. On the lcd, this will appear at the top in row 0 and in row 1 will demonstarte the distance measured by the 
ultrasonic sensor. 
*/

int button= 13; //set integer for button at value 13
int PBV= 0; //set integer for PBV at value 0 

#include <LiquidCrystal.h> //include liquid crystal library 

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //include constant vaiables for LCD


#define trigPin 10 //defien pin 10 as the sensor's trigger pin 
#define echoPin 8 //define pin 8 as the sensor's echo pin 

float duration, distance; //set the float variables duration and distance 

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

  Serial.begin(9600); //begin serial print at 9600 bps 
  lcd.createChar(0, customChar); //set the byte chart 
  lcd.begin(16, 2); //begin LCD display as 16 colums and 2 rows 
  pinMode(trigPin, OUTPUT); //set trigpin as output
  pinMode(echoPin, INPUT); //set echopin as input 
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

//write a 10 microsecond pulse 
digitalWrite(trigPin, LOW); //set the trigPin to LOW
delayMicroseconds(2); //set a 2 microsecond delay
digitalWrite(trigPin, HIGH); //set the trigPin to HIGH
delayMicroseconds(10); //set a 10 microsecond delay 
digitalWrite(trigPin, LOW); //set the trigPin to LOW

duration = pulseIn(echoPin, HIGH); //set duration variable as the pulseIn (pulse input)

distance = ((duration/2)*0.0343); //derive distance from duration using speed of sound (343m/s)

Serial.print("Distance= ");//serial print distance
lcd.setCursor(0,1); //set LCD cursor to begin at colum 0 row 0 

if (distance >= 330 || distance <= 2){ //if distance is greater than 330 or less than 2 inches 
Serial.println("OoR"); //then serial print OoR (Out of Range)
lcd.print("OUT OF RANGE"); //then LCD print OUT OF RANGE 
}
      
else{ //else 
Serial.print(distance); //serial print the distance 
Serial.println(" cm");  //serial print the units (cm)
lcd.print("D= "); //lcd print D= for distance=
lcd.print(distance); //LCD print the distance 
lcd.print(" cm"); //LCD print the units (cm)
} 

delay(delayMS); // Delay between measurements.
  sensors_event_t event;
  dht.temperature().getEvent(&event); 
  
  if (isnan(event.temperature)) { //if temperature cannot be read 
    Serial.println(F("Error reading temperature!")); //display error message on serial monitor 
    lcd.setCursor(0,0); //set lcd cursors to row 0 colum 0 
    lcd.println("Error"); //display error 
  }

int PB= digitalRead(button); //digital read the pushbutton state 

//****note that pushbutton must be pressed for around 3-5 seconds to allow proper reading 

if (PB == LOW) {    //if pushbutton state is low 
PBV= PBV+1; //at 1 to counter PBV

if (PBV > 3) { //if PBV is greater than 3 
PBV = 1; //PBV is equal to 0 
}
  
switch (PBV){

 case 1: //in the case that PBV is equal to 1 
      Serial.print(F("Temperature Celsius: ")); //serial print "Temperature Celsius:"
      Serial.print(event.temperature); //serial print the temperature reading 
      Serial.println(F("°C")); //serial print degrees C
      lcd.setCursor(0,0); //set lcd cursor to row 0 colum 0 
      lcd.print(F("Temp C: ")); //lcd print "Temp C: "
      lcd.print(event.temperature); //lcd print the temperature reading 
      lcd.write(customChar[8]); //lcd print the degree symbol 
      lcd.print("C"); //lcd print C

      Serial.print("Distance= ");//serial print distance
      lcd.setCursor(0,1); //set LCD cursor to begin at colum 0 row 0 
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

      Serial.print("Distance= ");//serial print distance
      lcd.setCursor(0,1); //set LCD cursor to begin at colum 0 row 0 
      break; //break case 2

case 3: //in the case that PBV is equal to 3
      Serial.print(F("Humidity: ")); //serial print "Humidity"
      Serial.print(event.relative_humidity); //serial print the humidity reading 
      Serial.println(F("%")); //serial print "%""
      lcd.setCursor(0,0); //set lcd cursor to row 0 colum 0 
      lcd.print(F("Humidity: ")); //lcd print "Hum: "
      lcd.print(event.relative_humidity); //lcd print the humidity reading 
      lcd.print("%");
      break; //break case 3
}
}
}




