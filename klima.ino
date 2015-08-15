// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

/*-----( Import needed libraries )-----*/
#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include "Lti.h"


#define DHTPIN    22     
#define DIODE     24
#define FanLamp   29
#define Fan       31
#define Humifier  35
#define Heater    37

LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
Lti Lti(23,25,27);


float setTemp   = 20;
float tTemp[5];
float ttTemp;
float tHum[5];
float ttHum;
float minTemp = 18;
float maxTemp = 28;
float minHum = 55; 
int light_status;

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);
// NOTE: For working with a faster chip, like an Arduino Due or Teensy, you
// might need to increase the threshold for cycle counts considered a 1 or 0.
// You can do this by passing a 3rd parameter for this threshold.  It's a bit
// of fiddling to find the right value, but in general the faster the CPU the
// higher the value.  The default for a 16mhz AVR is a value of 6.  For an
// Arduino Due that runs at 84mhz a value of 30 works.
// Example to initialize DHT sensor for Arduino Due:
//DHT dht(DHTPIN, DHTTYPE, 30);

void setup() {
  Serial.begin(9600); 
  
  lcd.begin(20,4);
  
  Serial.println("DHTxx test!");
  
  
  //Define Relay
  pinMode(FanLamp, INPUT_PULLUP);
  pinMode(FanLamp, OUTPUT);
  pinMode(Fan, OUTPUT);
  pinMode(Humifier, INPUT_PULLUP);
  pinMode(Humifier, OUTPUT);
  pinMode(Heater, INPUT_PULLUP);
  pinMode(Heater, OUTPUT);

  
  pinMode(DIODE, INPUT);
  
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Relay 00001000");
  
  dht.begin();
}

void loop() {

  // Wait a few seconds between measurements.
  delay(2500);
   
 light_status = digitalRead(DIODE);
  
  if(light_status){
    light_off();
  }
  else{
    light_on();   
  }  

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.setCursor(0,0);
    lcd.print("Failed to read DHT  ");
    Lti.med();
    lcd.setCursor(6,1);
    lcd.print("010");
    lcd.setCursor(0,2);
    lcd.print("LTI on LOW ");
    return;
  }

  // Compute heat index
  // Must send in temp in Fahrenheit!
 // float hi = dht.computeHeatIndex(f, h);
 

  tTemp[5] = t;
  tHum[5]   = h;
  
  //Shift Register to the left
  for(int i=0; i<5; i++) { 
   tTemp[i] = tTemp[i+1];
   tHum[i]  = tHum[i+1]; 
  }
  
  //Durchschnittstemperatur berechnen
  for(int i=0;i<5;i++){
    ttTemp+=tTemp[i];
    ttHum+=tHum[i];
  }
  float Temp;
  float Hum;
  Temp=ttTemp/6;
  Hum  =ttHum/6;
  
  
  
  Serial.print("Humidity: "); 
  Serial.print(Hum);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(Temp);
  Serial.println(" *C ");
  
  lcd.setCursor(0,0);
  lcd.print("Temp:");
  lcd.setCursor(5,0);
  lcd.print(Temp);
  lcd.setCursor(10,0);
  lcd.print(" Hum:");
  lcd.setCursor(15,0);
  lcd.print(Hum);
  
  if (Temp >= setTemp && Temp < maxTemp) {
    Lti.med(); 
    lcd.setCursor(6,1);
    lcd.print("010");
    lcd.setCursor(0,2);
    lcd.print("LTI on MED ");
  }
  else if(Temp >= 26){
    Lti.high();
    lcd.setCursor(6,1);
    lcd.print("001");
    lcd.setCursor(0,2);
    lcd.print("LTI on HIGH");
  }
  else{
    Lti.low();
    lcd.setCursor(6,1);
    lcd.print("100");
    lcd.setCursor(0,2);
    lcd.print("LTI on LOW ");
}

  //Humifier Settings
  if(Hum < minHum){
    digitalWrite(Humifier, 0);
    lcd.setCursor(12,1);
    lcd.print("1");
  }
  else{
   digitalWrite(Humifier, 1); 
   lcd.setCursor(12,1);
    lcd.print("0");
  }
  
  
  //Heater Settings
  if(Temp < minTemp){
   digitalWrite(Heater, 0); 
   lcd.setCursor(13,1);
   lcd.print("1");
  }
  else{
   digitalWrite(Heater, 1);
   lcd.setCursor(13,1);
   lcd.print("0"); 
  }
  
//End loop()  
}


void light_off(){
   digitalWrite(FanLamp, 1);
   lcd.setCursor(0,3);
   lcd.print("Ligh is off");
   lcd.setCursor(9,1);
   lcd.print("0");
}

void light_on(){
   digitalWrite(FanLamp, 0);
   lcd.setCursor(0,3);
   lcd.print("Ligh is on ");  
   lcd.setCursor(9,1);
   lcd.print("1");
}
