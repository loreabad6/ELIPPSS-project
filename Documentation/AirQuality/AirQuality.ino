/* 
 * PM2.5 and PM10, Temp., RH data logger saves to an SD card with GPS time and location
 * 
 * senseBox Group
 * Unmanned Aerial Systems
 * Geospatial Technologies MSc.
 * 
 * Institute for Geoinformatics, University of Muenster
 * June. 2018
*/

#include <SDS011-select-serial.h>
#include <SPI.h> // library to save to SD card
#include <SD.h>
#include <HDC100X.h>
#include <TinyGPS++.h>
#include <AltSoftSerial.h>

float p10,p25;
int error;
unsigned long logInt;

TinyGPSPlus gps;

AltSoftSerial ss;
#define aq Serial

SDS011 SDS(aq);
HDC100X HDC(0x43);

const int chipSelect = 4; 

void setup() {

  aq.begin(9600);
  ss.begin(9600);

  // Check if an SD card is inserted.
  if (!SD.begin(chipSelect)) {
    return;
  }

  HDC.begin(HDC100X_TEMP_HUMI, HDC100X_14BIT, HDC100X_14BIT, DISABLE);

  File dataFile = SD.open("datalog.csv", FILE_WRITE);
  dataFile.println(F(" PM2.5(mg/m3); PM10(mg/m3); Temperature(C); Relative Humidity(%); Longitude; Latitude; Altitude(m); Time "));
  dataFile.close();

  delay(60000);
}

void loop() {
  if ((millis()-logInt)>10000){
    // Read out the sensors and write the data on SD card
    File dataFile = SD.open("datalog.csv", FILE_WRITE);
    
      //SDS011
      delay(2);
      error = SDS.read(&p25,&p10);
      if (! error) {
        dataFile.print(p25);
        dataFile.print(F(";"));
        dataFile.print(p10);
        dataFile.print(F(";"));
      }
      else
      {
        dataFile.print(F("NA"));
        dataFile.print(F(";"));
        dataFile.print(F("NA"));
        dataFile.print(F(";"));
      }
      delay(1);
      
      //HDC1080
      float Temp = HDC.getTemp(); // Read temperature
      dataFile.print(Temp); // Save temperature
      dataFile.print(F(";"));
      float Humi = HDC.getHumi(); // Read RH
      dataFile.print(Humi); // Save RH
      dataFile.print(F(";"));
     
     //TinyGPS
      delay(2);
      while (ss.available() > 0){
        if (gps.encode(ss.read()))
          {
            if (gps.location.isValid())
            {
              dataFile.print(gps.location.lng(), 5);
              dataFile.print(F(";"));
              dataFile.print(gps.location.lat(), 5);
              dataFile.print(F(";"));
            }
            else
            {
              dataFile.print(F("NA"));
              dataFile.print(F(";"));
              dataFile.print(F("NA"));
              dataFile.print(F(";"));
            }
            if (gps.altitude.isValid())
            {
              dataFile.print(gps.altitude.meters(), 3);
              dataFile.print(F(";"));  
            }
            else
            {
              dataFile.print(F("NA"));
              dataFile.print(F(";"));  
            }  
            if (gps.time.isValid())
            {
              dataFile.print(gps.time.hour());
              dataFile.print(F(":"));
              dataFile.print(gps.time.minute());
              dataFile.print(F(":"));
              dataFile.print(gps.time.second());
              dataFile.print(F(";"));
            }
            else
            {
              dataFile.print(F("NA"));
              dataFile.print(F(";"));
            }  
          }
      }
      if (millis() > 5000 && gps.charsProcessed() < 10)
      {
        dataFile.print(F("NaN"));
        dataFile.print(F(";"));        
        dataFile.print(F("NaN"));
        dataFile.print(F(";")); 
        dataFile.print(F("NaN"));
        dataFile.print(F(";"));
      }
      delay(1);     
    dataFile.println();
    // When the file is open
    if (dataFile) {
      dataFile.close();
    }
    logInt = millis();
  }
}
