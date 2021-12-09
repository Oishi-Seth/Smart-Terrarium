int readMoisture = A0;  // Pin which reads signal from 1st arduino for activating reflex for low moisture
int motor = 10;         // Pin that activates motor to pump water
int readTemp = A1;      // Pin that reads signal from 1st arduino for activating reflex for high temperature
int readHumi = A2;      // Pin that reads signal from 1st arduino for activating reflex for less humidity
int lightpin = A3;      // Pin that reads reads values from photoresistor (LDR sensor)
int ledPin = 6;         // Reflex for low light and high light

// Variables:
int w=0;
int l=0;
int lightlevel = 0;
int pos = 90;

// Initializing Servo that controls buttons on misting fan
#include<Servo.h>
Servo myservo;

void setup() {
  // put your setup code here, to run once:
  pinMode(readMoisture,INPUT);
  pinMode(readTemp,INPUT);
  pinMode(6,OUTPUT);
  pinMode(readHumi,INPUT);
  pinMode(lightpin,INPUT);
  pinMode(10,OUTPUT);
  myservo.attach(9);
}

// Function that switches on mist in misting fan
void start_mist(){
  for(pos=90; pos<=150; pos+=1){
    myservo.write(pos);
    delay(15);
  }
  for(pos=150; pos>=90; pos-=1){
    myservo.write(pos);
    delay(15);
  }
}
// Function to switch on the fan
void start_fan(){
  for(pos=90;pos>=30;pos-=1){
    myservo.write(pos);
    delay(15);
  }
  for(pos=30; pos<=90; pos+=1){
    myservo.write(pos);
    delay(15);
  }
}
// Function to stop the misting in misting fan
void stop_mist(){
  for(pos=90; pos<=150; pos+=1){
    myservo.write(pos);
    delay(15);
  }
  for(pos=150; pos>=90; pos-=1){
    myservo.write(pos);
    delay(15);
  }
}
// Function to stop the fan
void stop_fan(){
  for(pos=90;pos>=30;pos-=1){
    myservo.write(pos);
    delay(15);
  }
  for(pos=30; pos<=90; pos+=1){
    myservo.write(pos);
    delay(15);
  }
  for(pos=90;pos>=30;pos-=1){
    myservo.write(pos);
    delay(15);
  }
  for(pos=30; pos<=90; pos+=1){
    myservo.write(pos);
    delay(15);
  }
}

void loop() {
  
  // Reading moisture reflex signal
  int val = analogRead(readMoisture);
  if(val > 50){
    digitalWrite(10,HIGH);
  }
  else{
    digitalWrite(10,LOW);
  }
  
  // Reading temperature and humidity reflexes signals
  int tempval = analogRead(readTemp);
  int humival = analogRead(readHumi);
  
  // activating fan and misting according to above signals
  if(tempval > 50){
    start_fan();
    delay(10000);
    stop_fan();
    delay(500);
  }
  if(humival > 50){
    start_mist();
    delay(10000);
    stop_mist();
    delay(500);
  }
  
  // Reading light level of photoresistor and controlling LED brightness according to it
  lightlevel = analogRead(lightpin);
  lightlevel = map(lightlevel,0,380,0,225);
  lightlevel = constrain(lightlevel,0,225);
  analogWrite(ledPin, 225 - lightlevel);
  delay(100);
}
