int sensor = A0;      // Pin that reads values from soil moisture sensor
int writeTemp = 10;   // Sends HIGH signal to 2nd arduino when temperature is high
int writeHumi = 9;    // Sends HIGH signal to 2nd arduino when humidity is low
int writemois = 13;   // Sends HIGH signal to 2nd arduino when soil moisture is less
const int pingPin = 1; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 6; // Echo Pin of Ultrasonic Sensor

// Variables
int out;
long duration, inches, cm;

// Initializing 2 DHT sensors and LCD monitor
#include "DHT.h"
#include <LiquidCrystal.h> 
#define DHTPIN1 7     // Pin to read values from 1st DHT sensor
#define DHTPIN2 8     // Pin to read values from 2nd DHT sensor
#define DHTTYPE DHT11
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  // Pins that are connected to LCD monitor
DHT dht(DHTPIN1,DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  lcd.begin(20,4);
  pinMode(sensor,INPUT);
  pinMode(13,OUTPUT);
  dht.begin();
  dht2.begin();
  delay(2000);
}

void loop() {
  
  // Reading values from soil moisture sensor
  out = analogRead(sensor);
  out = map(out,550,0,0,100);
  
  // Reading initial values from ultrasonic sensor
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  int initial = cm;
  
  // if soil moisture is less than 30%, 
  // switching on the water pump till the water level is decreased by 1cm
  if(out<30) {
    digitalWrite(13, HIGH);
    while(cm <= initial + 1){
      pinMode(pingPin, OUTPUT);
      digitalWrite(pingPin, LOW);
      delayMicroseconds(2);
      digitalWrite(pingPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(pingPin, LOW);
      pinMode(echoPin, INPUT);
      duration = pulseIn(echoPin, HIGH);
      inches = microsecondsToInches(duration);
      cm = microsecondsToCentimeters(duration);
      delay(100);
    }
    digitalWrite(13, LOW);
  }
  
  // Reading D,H,T values from both DHT sensors available and calculating its average value for further usage
  float h = (dht.readHumidity()+dht2.readHumidity())/2;
  float t = (dht.readTemperature()+dht2.readTemperature())/2;
  float f = (dht.readTemperature(true)+dht2.readTemperature(true))/2;
  float hif = (dht.computeHeatIndex(f,h) + dht2.computeHeatIndex(f,h))/2;
  float hic = (dht.computeHeatIndex(t,h,false) + dht2.computeHeatIndex(t,h,false))/2;

  // Printing Soil Moisture, Humidity, Temperature and water level on LCD
  lcd.setCursor(0,0);
  lcd.print(String("Moisture: "));
  lcd.setCursor(10,0);
  lcd.print(String(out) + String("%"));
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print(String("Humidity: "));
  lcd.print(String(h) + "%");
  delay(1000);
  lcd.setCursor(0,2);
  lcd.print(String("Temperature: "));
  lcd.print(t);
  lcd.print("C ");
  delay(1000);
  lcd.setCursor(0,3);
  lcd.print(String("Water Level: "));
  lcd.print(30 - cm);
  lcd.print(String("cm"));
  delay(1000);
  
  // Sending High signal if temperature is greater than 20°C
  if(t > 20){
    digitalWrite(writeTemp,HIGH);
  }
  else{
    digitalWrite(writeTemp,LOW);
  }
  // Sending high signal if humidity is less than 70%
  if(h <= 70){
    analogWrite(writeHumi,HIGH);
  }
  else{
    analogWrite(writeHumi,LOW);
  }

  // Clearing LCD monitor at the end to avoid printing wrong values
  lcd.clear();
}

// Utility functions to calculate distance value from Ultrasonic sensor
long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
