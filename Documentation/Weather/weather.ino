/* 
 * Weather variables data logger saves to an SD card with a timestamp for each record
 * 
 * senseBox Group
 * Unmanned Aerial Systems
 * Geospatial Technologies MSc.
 * 
 * Institute for Geoinformatics, University of Muenster
 * May. 2018
*/

#include <Wire.h>
#include <HDC100X.h>
#include <BMP280.h>
#include <Makerblog_TSL45315.h>
#include <VEML6070.h>
#include <SPI.h> // library to save to SD card
#include <SD.h> // library for SD card
#include <RV8523.h> // library for getting timestamp

#define I2C_ADDR 0x38
#define IT_1   0x1 //1T

HDC100X HDC(0x43);
BMP280 BMP;
VEML6070 VEML;
Makerblog_TSL45315 TSL = Makerblog_TSL45315(TSL45315_TIME_M4);
RV8523 rtc;

#define P0 1013.25 // atmospheric pressure 
double T, P, A, L, UV; // indicates variable gotten from HDC1=T, BMP280=P, Altitude(P-Po)=A
uint32_t lux;
byte msb=0, lsb=0;
uint16_t uv;
uint8_t sec, min, hour, day, month;
uint16_t year;

const int chipSelect = 4; 

void setup()
{
  //rtc.set(0, 44, 19, 8, 6, 2018); // 13:32:00 18.05.2018 Only use one time
  rtc.start();
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  Serial.println("Starting test...\n");
  delay(1000);
  scan();
  delay(1000);
  
  Serial.print("Initializing SD-Card");

  // Check if an SD card is inserted.
  if (!SD.begin(chipSelect)) {
    Serial.println("Card not found");
    return;
  }
  Serial.println("Card succesfully initialized");
  
  HDC.begin(HDC100X_TEMP_HUMI, HDC100X_14BIT, HDC100X_14BIT, DISABLE);
  VEML.begin();
  TSL.begin();
  BMP.begin();
  BMP.setOversampling(4);

  File dataFile = SD.open("datalog.csv", FILE_WRITE);
  dataFile.println(" Timestamp; Temperature(C); Relative Humidity(%); Iluminance (lux); UV-radiation (uW/cm2); Air Pressure (mBar); Height (m) ");
  dataFile.close();
}


void loop()
{
  // Read out the sensors and write the data on SD card
  File dataFile = SD.open("datalog.csv", FILE_WRITE);

    //RTC-RV8523
    rtc.get(&sec, &min, &hour, &day, &month, &year); // Read timestamp
    dataFile.print(month,DEC);
    dataFile.print("/");
    dataFile.print(day,DEC);
    dataFile.print("/");
    dataFile.print(year,DEC);
    dataFile.print(" ");
    dataFile.print(hour,DEC);
    dataFile.print(":");
    dataFile.print(min,DEC);
    dataFile.print(":");
    dataFile.print(sec);// Save timestamps
    dataFile.print(";");
    Serial.print("Timestamp:      ");
      Serial.print(hour); Serial.print(":"); Serial.print(min); Serial.print(":"); 
      Serial.print(sec); Serial.print("  "); Serial.print(day); Serial.print(".");
      Serial.print(month); Serial.print("."); Serial.println(year);
        
    //HDC1080
    float Temp = HDC.getTemp(); // Read temperature
    dataFile.print(Temp); // Save temperature
    dataFile.print(";");
    Serial.print("Temperature:      "); // Show temperature on Serial Monitor
      Serial.print(HDC.getTemp(),1); Serial.print(" \xC2\xB0"); Serial.println("C");
    float Humi = HDC.getHumi(); // Read RH
    dataFile.print(Humi); // Save RH
    dataFile.print(";");
    Serial.print("Humidity:         "); 
      Serial.print(HDC.getHumi(),1); Serial.println(" %");

    //TSL45315
    float lux = TSL.readLux(); // Read illuminance
    dataFile.print(lux); // Save illuminance
    dataFile.print(";");
    Serial.print("Illuminance:      ");
    Serial.print(lux, DEC);
    Serial.println(" lx");

    //VEML6070
    float uv = VEML.getUV(); // Read UV
    dataFile.print(uv); // Save UV
    dataFile.print(";");
    Serial.print("UV radiation:     "); 
      Serial.print(uv, DEC); Serial.println(" uW/cm2");
    
    //BMP280
    char result = BMP.startMeasurment(); 
    result = BMP.getTemperatureAndPressure(T,P);
    double A = BMP.altitude(P,P0);
    if(result!=0)
      {
        double A = BMP.altitude(P,P0);
        
        Serial.print("Air pressure:     "); 
          Serial.print(P, 2); Serial.println(" mBar");
        Serial.print("Height:     "); 
          Serial.print(A, 2); Serial.println(" m");
      }
    dataFile.print(P);
    dataFile.print(";");
    dataFile.print(A);
    dataFile.print(";");  
    dataFile.println();  
    
  // When the file is open
  if (dataFile) {
    dataFile.close();
  }
  // If the file can not be opened, an error message should be issued
  else {
    Serial.println("Error opening!");
  }
  delay(10000); //Interval of saving and reading: now set to 10 seconds 
}

void scan(void)
{
  byte error, address;

  Serial.println("Looking for devices...");

  Wire.begin();
  for(address = 1; address < 127; address++ )
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if(error == 0)
    {
      Serial.print("Device found: 0x");
      if(address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }
    else if(error==4)
    {
      Serial.print("Error: 0x");
      if(address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }
  }
  Serial.println("");
}
