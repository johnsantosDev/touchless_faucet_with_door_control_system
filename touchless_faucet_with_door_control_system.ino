// TOUCHLESS FAUCET WITH DOOR CONTROL SYSTEM
// ARDUINO PROJECTS BY R
// AUTHOR: RUCKSIKAA RAAJKUMAR
#include <Wire.h>
#include "LiquidCrystal_I2C.h"
#include <Servo.h>
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
#define trig 5
#define echo 4
Servo servo;
const int sensor = 3; 
const int Relay = 6;
int state;
int value; 
long duration;
int distance;
void setup() {
  Serial.begin(9600); // Set baud rate as 9600
  lcd.init();
  lcd.backlight();
  lcd.print("Loading...");
  delay(2000);
  lcd.seCursor(0,0);
  lcd.print("Wash your hands");  
  lcd.setCursor(0,1);
  lcd.print("before you enter");
  delay(4000);
  servo.attach(9); // The servo motor is connected to D9
  pinMode(trig, OUTPUT); // Set the trigger pin as OUTPUT
  pinMode(echo, INPUT); // Set the echo pin as INPUT
  pinMode(sensor, INPUT); // Configure the pin of the IR sensor as INPUT
  pinMode(Relay, OUTPUT); // Configure the pin of the relay module as OUTPUT
}
void loop() {
  digitalWrite(trig, LOW); 
  delayMicroseconds(5);
  digitalWrite(trig, HIGH); // Set the trigger pin HIGH to send the ultrasonic wave (pulse)
  delayMicroseconds(10);
  digitalWrite(trig, LOW); 
  value=digitalRead(sensor); // Read the digital signal sent by the IR sensor and store it in the 'value' variable
  duration = pulseIn(echo, HIGH); // Calculate time taken (in microseconds) for the pulse emitted by the trigger pin to reach the echo pin.
  distance = (duration/2) * (331.3/10000); // Calculate the distance from the sensor to the obstacle in cm, using the speed of sound in air(m/s) and the time taken (stored in duration variable)
  Serial.println(distance);
  if(distance>1 && distance<15){ // If you place your hands within 15 cm
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Hands detected");
    lcd.setCursor(0,1);
    lcd.print("Wash hands - 20s");
    digitalWrite(Relay, HIGH); //Turns on the submersible water pump or solenoid water valve
    state=1; // Assign state variable to 1
    delay(1000); // Delay period of 1 second must be used to prevent clicking of the relay module
    lcd.clear();
    lcd.print("You are safe now");
  }else{
    digitalWrite(Relay, LOW); //Turns off the submersible water pump or solenoid water valve
   }
   if((state==1)&&(value==LOW)){ // If you place your hands in front of the IR sensor after washing your hands
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("You may go in");
    lcd.setCursor(1,1);
    lcd.print("10 seconds left");
    servo.write(90);
    delay(10000); // The door will be opened for 10 seconds
    servo.write(0);
    lcd.clear();
    lcd.print("Wash your hands");
    lcd.setCursor(0,1);
    lcd.print("before you enter");
    state=0;
   } else if((state==0)&&(value==LOW)){ // If you do not wash your hands before you enter
    lcd.clear();
    lcd.print("Wash your hands");
    lcd.setCursor(0,1);
    lcd.print("to grant access");
   }  
}
