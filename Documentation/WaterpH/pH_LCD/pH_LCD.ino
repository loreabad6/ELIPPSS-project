/* 
 * pH data logger saves to an SD card with a timestamp for each record
 * 
 * senseBox Group
 * Unmanned Aerial Systems
 * Geospatial Technologies MSc.
 * 
 * Institute for Geoinformatics, University of Muenster
 * May. 2018
*/
#include <LiquidCrystal.h>
#define SensorPin A0    // pH meter Analog output an Arduino Analog Input A0
#define Offset 0.33     // deviation compensate

unsigned long int avgValue;  // Save the average values ​​of the sensor
float b;
int buf[10], temp;

//Pin assignments for DFRobot LCD Keypad Shield
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 
//---------------------------------------------

void setup() {
  //LCD configuration
  lcd.begin(16, 2); // row and lines
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("senseBox Group 2018");
  lcd.setCursor(1, 1);
  lcd.print("  PH");
// scroll 13 positions (string length) to the left
  // to move it offscreen left:
  for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(300);
  }
  pinMode(13, OUTPUT);  
}

void loop() {

  for(int i = 0; i < 10; i++) {     // Take 10 measurements to average 
    buf[i] = analogRead(SensorPin);
    delay(10);
  }

  for(int i = 0; i < 9; i++) {      // sort the measured values ​​by height
    for(int j = i+1; j < 10; j++) {
      if(buf[i] > buf[j]) {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }

   // Take the mean 6 readings and calculate the mean
  avgValue = 0;
  for(int i = 2; i < 8; i++) {                     
    avgValue += buf[i];
  }

  // Conversion of the input value in millivolts
  float voltage = (float)avgValue * 5.0 / 1024 / 6;
  // Conversion of the value of millivolts into the pH scale
  float phValue = 3.5 * voltage + Offset;

  Serial.print("    pH:");  
  Serial.print(phValue, 2);
  Serial.println(" ");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PH: ");
  lcd.print(phValue, 2);
  digitalWrite(13, HIGH);       
  delay(800);
  digitalWrite(13, LOW);

  delay(800);
}
