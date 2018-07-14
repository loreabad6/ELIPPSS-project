/* 
 * Water temperature data logger displays on LCD
 * 
 * senseBox Group
 * Unmanned Aerial Systems
 * Geospatial Technologies MSc.
 * 
 * Institute for Geoinformatics, University of Muenster
 * May. 2018
 * LCD1602 Key Shield 1.0 added 06 June 2018
*/


// First we include the libraries
#include <OneWire.h>
#include <DallasTemperature.h>
//#include <SPI.h> // library to save to SD card
//#include <SD.h> // library for SD card
#include <RV8523.h> // library for getting timestamp
//libraries for LCD
#include <LiquidCrystal.h>
//#include <DFR_Key.h>

//Pin assignments for DFRobot LCD Keypad Shield
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 

// Timestamp logging initial setup
RV8523 rtc;

uint8_t sec, min, hour, day, month;
uint16_t year;

// SD card initial setup
//const int chipSelect = 4; 

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);


void setup() {
  //LCD configuration
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("senseBox Group 2018");
  lcd.setCursor(0, 1);
  lcd.print(" Water Temperature");
// scroll 13 positions (string length) to the left
  // to move it offscreen left:
  for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(300);
  }

  // scroll 29 positions (string length + display length) to the right
  // to move it offscreen right:
  for (int positionCounter = 0; positionCounter < 29; positionCounter++) {
    // scroll one position right:
    lcd.scrollDisplayRight();
    // wait a bit:
    delay(300);
  }

  // scroll 16 positions (display length + string length) to the left
  // to move it back to center:
  for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(300);
  }

  // delay at the end of the full loop:
  delay(1000);


  
  // rtc.set(0, 32, 13, 18, 5, 2018); // 13:32:00 18.05.2018 Only use one time
  rtc.start();
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
}

void loop() {

    // Read out the sensors and write the data on SD card
 
   // call sensors.requestTemperatures() to issue a global temperature
   // request to all devices on the bus
    sensors.requestTemperatures(); // Send the command to get temperature readings
  
    Serial.print("Temperature 0: ");
    Serial.println(sensors.getTempCByIndex(0));
    Serial.print("Temperature 1: ");
    Serial.println(sensors.getTempCByIndex(1)); 

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp 0: ");
    lcd.print(sensors.getTempCByIndex(0));
    lcd.setCursor(0, 1);
    lcd.print("Temp 1: ");
    lcd.print(sensors.getTempCByIndex(1)); 
        

  }  
  delay(5000);

}
