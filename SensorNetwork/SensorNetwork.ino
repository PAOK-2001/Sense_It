/* SENSOR NETWORK
 *  Abraham De Alba Franco
 *  Alejandro Flores Madriz
 *  Emil Dobetsberger
 *  Francisco Ramírez Aldaco
 *  Pablo Ortega-Kral
 */

#include <DS1307Pico.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_GPS.h>

#define GPSSerial Serial2
#define GPSECHO false

Adafruit_GPS GPS(&GPSSerial);

#define IR 15
#define FOTO_RESISTOR A0

Adafruit_ADXL345_Unified accelerometer = Adafruit_ADXL345_Unified(6,0,7,1,1234); // Initialize SPI connection with ADXL345, assigning ID 1234
DS1307Pico rtc; //Creates clock object, reading I2C address from I2C0
uint32_t lastTime; 


/*
 * Read current time from I2C bus and prints it to serial monitor
 */
void read_time(){
  Serial.println(rtc.DSgetTime("%A, %B %d %Y %H:%M:%S"));
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
void read_accelerometer(){
  sensors_event_t event; 
  accelerometer.getEvent(&event);
  // Return data from the 3 axis
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
}

int is_presence(){
  return !digitalRead(IR);
}


void setup() {
  Serial.begin(115200);
  pinMode(IR, INPUT);
  pinMode(FOTO_RESISTOR, INPUT);
  rtc.begin();
  if(!accelerometer.begin()){
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }
  // Chose sensor lib
  accelerometer.setRange(ADXL345_RANGE_16_G);
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);
  GPSSerial.println(PMTK_Q_RELEASE);
  // Optional setup to set an initial time
  //rtc.DSadjust(22, 40, 0, 2023, 3, 2); // 00:15:21 16 Mar 2022
 
}

void loop() {
  char c = GPS.read();
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA())) return; 
   }
  if(millis() - lastTime >= 2000) {
    rtc.DSread(); 
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitudeDegrees, 5);
      Serial.print(", ");
      Serial.println(GPS.longitudeDegrees, 5);
    }
    read_time();
    read_accelerometer();
    Serial.println("Ṕhotoresistor: " + String(read_from_photores()));
    Serial.println("IR: " + String(is_presence()));
    lastTime = millis();
   }

}
