/* 
 * Water level data logger saves to an SD card with a timestamp for each record
 * 
 * senseBox Group
 * Unmanned Aerial Systems
 * Geospatial Technologies MSc.
 * 
 * Institute for Geoinformatics, University of Muenster
 * May. 2018
*/

//Libraries loaded
#include <SPI.h> // library to save to SD card
#include <SD.h> // library for SD card
#include <RV8523.h> // library for getting timestamp
//#include <LiquidCrystal.h>

#define trigPin1 3
#define echoPin1 2
#define trigPin2 8
#define echoPin2 7

long duration, distance, RightSensor,BackSensor,FrontSensor,LeftSensor;

// Timestamp logging initial setup
RV8523 rtc;

uint8_t sec, min, hour, day, month;
uint16_t year;

// SD card initial setup
const int chipSelect = 4;

void setup()
{
 //rtc.set( 30, 05, 16, 9, 6, 2018); // 13:32:00 18.05.2018 Only use one time
  rtc.start();
    
 Serial.begin(9600);
  while (!Serial) {
    ;
  }
  
  Serial.print("Initializing SD-Card");
   // Check if an SD card is inserted.
  if (!SD.begin(chipSelect)) {
    Serial.println("Card not found");
    return;
  }
  Serial.println("Card succesfully initialized");
    
  File dataFile = SD.open("datalog.csv", FILE_WRITE);
  dataFile.println(" Timestamp; LeftSensor (cm); RightSensor (cm)");
  dataFile.close();
pinMode(trigPin1, OUTPUT);
pinMode(echoPin1, INPUT);
pinMode(trigPin2, OUTPUT);
pinMode(echoPin2, INPUT);
}

void loop() {
// Read out the sensors and write the data on SD card
  File dataFile = SD.open("datalog.csv", FILE_WRITE);

  //RTC-RV8523
  rtc.get(&sec, &min, &hour, &day, &month, &year); // Read timestamp
  
  Serial.print("Timestamp:      ");
      Serial.print(hour); Serial.print(":"); Serial.print(min); Serial.print(":"); 
      Serial.print(sec); Serial.print("  "); Serial.print(day); Serial.print(".");
      Serial.print(month); Serial.print("."); Serial.println(year);

  //call the function and get the distance for each sensor
SonarSensor(trigPin1, echoPin1);
RightSensor = distance;
 if(RightSensor == 0) // If we timed out
  {
    pinMode(echoPin1, OUTPUT); // Then we set echo pin to output mode
    digitalWrite(echoPin1, LOW); // We send a LOW pulse to the echo pin
    delayMicroseconds(200);
    pinMode(echoPin1, INPUT); // And finaly we come back to input mode
  }
SonarSensor(trigPin2, echoPin2);
LeftSensor = distance;
 if(LeftSensor == 0) // If we timed out
  {
    pinMode(echoPin2, OUTPUT); // Then we set echo pin to output mode
    digitalWrite(echoPin2, LOW); // We send a LOW pulse to the echo pin
    delayMicroseconds(200);
    pinMode(echoPin2, INPUT); // And finaly we come back to input mode
  }

Serial.print("LeftSensor: ");
Serial.println(LeftSensor);
Serial.print("RightSensor: ");
Serial.println(RightSensor);
// When the file is open
  if (dataFile) {
    dataFile.print(year,DEC);
    dataFile.print("/");
    dataFile.print(month,DEC);
    dataFile.print("/");
    dataFile.print(day,DEC);
    dataFile.print(" ");
    dataFile.print(hour,DEC);
    dataFile.print(":");
    dataFile.print(min,DEC);
    dataFile.print(":");
    dataFile.print(sec);// Save timestamps
    dataFile.print(";");
    // Save the distance on the file
    dataFile.print(LeftSensor);
    dataFile.print(";");
    dataFile.print(RightSensor);
    dataFile.println(";");
    dataFile.close();
    }
  // If the file can not be opened, an error message should be issued
  else {
    Serial.println("Error opening!");
         }
delay(10000);
}

//function sonsarSensor that evaluates each sensor with its parameters
void SonarSensor(int trigPin,int echoPin)
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance = (duration/2) / 29.1;
}
