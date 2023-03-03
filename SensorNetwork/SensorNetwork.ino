/* SENSOR NETWORK
 *  Abraham De Alba Franco
 *  Alejandro Flores Madriz
 *  Emil Dobetsberger
 *  Francisco Ramírez Aldaco
 *  Pablo Ortega-Kral
 */
#include <PicoLCD_I2C.h> //https://github.com/smlaage/PicoLCD_I2C
#include <DS1307Pico.h> //https://github.com/3tawi/DS1307Pi
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_GPS.h>



#define GPSSerial Serial2
#define GPSECHO false

Adafruit_GPS GPS(&GPSSerial);
PicoLCD_I2C lcd(0, 0x27, 4, 5, 20);


#define IR 15
#define FOTO_RESISTOR A0

Adafruit_ADXL345_Unified accelerometer = Adafruit_ADXL345_Unified(6,0,7,1,1234); // Initialize SPI connection with ADXL345, assigning ID 1234
DS1307Pico rtc; //Creates clock object, reading I2C address from I2C0
uint32_t lastTime = millis(); 


/*
 * Read current time from I2C bus and prints it to serial monitor
 */
String read_time(){
  //Serial.println(rtc.DSgetTime("%A, %B %d %Y %H:%M:%S"));
  return rtc.DSgetTime("%A, %B %d %Y %H:%M:%S");
}

/*
 * Read from ADC to get analogue value of photoresistor from 0-4095
 */
int read_from_photores(){
  return analogRead(FOTO_RESISTOR);
  
}

/*
 * Read data from accelerometer 
 */
float accel [3];
void read_accelerometer(){
  sensors_event_t event; 
  accelerometer.getEvent(&event);
  // Return data from the 3 axis
  accel[0] = event.acceleration.x;
  accel[1] = event.acceleration.y;
  accel[2] = event.acceleration.z;
  //Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  //Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  //Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
}

int is_presence(){
  return !digitalRead(IR);
}


void setup() {


  Serial.begin(115200);
  pinMode(IR, INPUT);
  pinMode(FOTO_RESISTOR, INPUT);
  lcd.begin();
  lcd.clear();
  rtc.begin();
  if(!accelerometer.begin()){
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }
  // Chose sensor lib
  accelerometer.setRange(ADXL345_RANGE_8_G);
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);
  GPSSerial.println(PMTK_Q_RELEASE);
  // Optional setup to set an initial time
  //rtc.DSadjust(15, 10, 0, 2023, 3, 3); // 00:15:21 16 Mar 2022
 
}

void displayToLCD(int var){
  switch (var){
    case 0:
    {
      char buf [10];
      String strBuf;
      //GPS display
      if (GPS.fix) {
        lcd.clear();
        lcd.setCursor(6, 1);
        lcd.print("Location");
        strBuf = String(GPS.latitudeDegrees,4);
        strBuf.toCharArray(buf,10);
        lcd.setCursor(1, 2);
        lcd.print(buf);
        lcd.print(", ");
        strBuf = String(GPS.longitudeDegrees,4);
        strBuf.toCharArray(buf,10);
        lcd.print(buf);
      }else{
        lcd.clear();
        lcd.setCursor(2, 1);
        lcd.print("No location yet");
      }
      break;
    }
    case 1:
    {
      lcd.clear();
      lcd.setCursor(2,1);
      lcd.print("IR Sensor Status");
      lcd.setCursor(9,2);
      lcd.print("=");
      lcd.write(is_presence()+48);
      break;
    }
    case 2:
    {
      char buf [10];
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("Photoresistor Status");
      lcd.setCursor(8,2);
      itoa(read_from_photores(),buf,10);
      lcd.print(buf);
      break;
    }
    case 3:
    {
      read_accelerometer();
      char buf [10];
      String strBuf;
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("Accelerometer Status");
      lcd.setCursor(1,2);
      lcd.print("x: ");
      strBuf = (String)accel[0];
      strBuf.toCharArray(buf,10);
      lcd.print(buf);
      lcd.print(", ");
      lcd.print("y: ");
      strBuf = (String)accel[1];
      strBuf.toCharArray(buf,10);
      lcd.print(buf);
      lcd.setCursor(3,3);
      lcd.print("z: ");
      strBuf = (String)accel[2];
      strBuf.toCharArray(buf,10);
      lcd.print(buf);
      lcd.print(" m/s^2");
      break;
    }
    case 4:
    {
      String strBuf;
      char buf [40];
      lcd.clear();
      lcd.setCursor(2,1);
      lcd.print("Real-Time Clock");
      lcd.setCursor(6,2);
      ((String)rtc.hour).toCharArray(buf, 40);
      lcd.print(buf);
      lcd.print(":");
      ((String)rtc.minute).toCharArray(buf, 40);
      lcd.print(buf);
      lcd.print(":");
      ((String)rtc.second).toCharArray(buf, 40);
      lcd.print(buf);
      break;
    }
  }
}

int iter = 0;

void loop() {
  char c = GPS.read();
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA())) return; 
  }
  if(millis() - lastTime >= 2000) {
    displayToLCD(iter);
    if(iter >= 4){
      iter = 0;
    }
    else{
      iter ++;
    }
    
    lastTime = millis();
  }
  rtc.DSread(); 
  
  


}
