/* This program will show a ultrasonic sensor distance readings and dispaly it
both on the serial monitor and the LCD diaply. This will be done by calculating 
ultrasonic sensors wave and turning it into a readable distance. The program will
also utilize the <LiquidCrystal.h> library for the LCD */

#include <LiquidCrystal.h> //include liquid crystal library 

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //include constant vaiables for LCD


#define trigPin 10 //defien pin 10 as the sensor's trigger pin 
#define echoPin 13 //define pin 13 as the sensor's echo pin 

float duration, distance; //set the float variables duration and distance 

void setup() {

  Serial.begin(9600); //begin serial print at 9600 bps 
  lcd.begin(16, 2); //begin LCD display as 16 colums and 2 rows 
  pinMode(trigPin, OUTPUT); //set trigpin as output
  pinMode(echoPin, INPUT); //set echopin as input 
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
lcd.setCursor(0,0); //set LCD cursor to begin at colum 0 row 0
  
if (distance >= 330 || distance <= 2){ //if distance is greater than 330 or less than 2 inches 
Serial.println("OoR"); //then serial print OoR (Out of Range)
lcd.print("OUT OF RANGE"); //then LCD print OUT OF RANGE 
}

else{ //else 
Serial.print(distance); //serial print the distance 
Serial.print(" cm");  //serial print the units (cm)
  
lcd.print("D= "); //lcd print D= for distance=
lcd.print(distance); //LCD print the distance 
lcd.print(" cm"); //LCD print the units (cm)

} 

delay(500); //delay 500 milliseconds 
lcd.clear(); //clear the LCD
}
