/*
  Plant Watering System
*/
#include "DHT.h"
#define DHTPIN 7    
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);
const int SoilMoistureSensorPin = A0;
const int WaterPumpPin = 2;
const int WetLEDPin = 13; // ..... Yellow LED 
const int DryLEDPin = 12; // ..... Red LED 
const int PumpLEDPin = 11; //..... Blue LED 
const int air = 1000;
const int dry = 450;
const int wet = 270;
// 10 seconds
const long waterDuration = 1000L * 10L; 
// 30 seconds
const long sensorReadIntervals = 1000L * 30L;
// 2 minutes
const long waterIntervals = 1000L * 60L * 2L;
long lastWaterTime = -waterIntervals - 1;
boolean isWatering = false;
void setup() {
  Serial.begin(9600);
  pinMode(WaterPumpPin, OUTPUT);
  pinMode(WetLEDPin,OUTPUT);
  pinMode(DryLEDPin,OUTPUT);
  pinMode(PumpLEDPin,OUTPUT);
  waterPumpOff();
  dht.begin(); 
}
void loop() {
  mainLoop();
}
void mainLoop() {
  float temperature = getTemperature();
  float humidity = getHumidity();
  long soilMoisture = analogRead(SoilMoistureSensorPin);
  Serial.println("Soil Moisture: " + readableSoilMoisture(soilMoisture) + ", " + soilMoisture);
   if ( soilMoisture < dry) {
  digitalWrite(WetLEDPin,HIGH);
  digitalWrite(DryLEDPin,LOW);
  } else if (soilMoisture >= dry  ) {
  digitalWrite(WetLEDPin,LOW);
  digitalWrite(DryLEDPin,HIGH);
  }
  Serial.println("Temperature: " + String(temperature) + " °C");
  Serial.println("Humidity: " + String(humidity) + " %");
  if (millis() - lastWaterTime > waterIntervals) {
    waterPlants(soilMoisture);
    lastWaterTime = millis();
  }
  delay(sensorReadIntervals);
}
void waterPlants(int soilMoisture) {
  if (soilMoisture >= dry) {
    delay(1000L*60L);
    isWatering = true;
  } else if (soilMoisture < dry) {
    delay(1000L*60L);
    isWatering = false;
  }
  Serial.println(isWatering ? "Starting to water" : "Skipping water");    
  if (isWatering) {
    waterPumpOn();
    delay(waterDuration);
    waterPumpOff();
    Serial.println("Done watering");
  }
}
String readableSoilMoisture(int soilMoisture){
  if (soilMoisture < wet) {
    return "Water";
  } else if (soilMoisture >= wet  && soilMoisture < dry ) {
     return "Wet";
  } else if (soilMoisture >= dry && soilMoisture < air ) {
    return "Dry";
  } else if (soilMoisture >= air ) {
    return "Air";
}
}
float getTemperature() {
  float temperature = dht.readTemperature(false);
  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  }
  return temperature;
}
float getHumidity() {
  float humidity = dht.readHumidity();
  if (isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
  }
  return humidity;
}
void waterPumpOn() {
  Serial.println("Water pump on");
   delay(1000L*5L);
  digitalWrite(WaterPumpPin, HIGH);
  digitalWrite(PumpLEDPin,HIGH);
}
void waterPumpOff() {
  Serial.println("Water pump off");
   delay(1000L*5L);
  digitalWrite(WaterPumpPin, LOW);
  digitalWrite(PumpLEDPin,LOW);
}
