/* SENSOR NETWORK
 *  Abraham De Alba Franco
 *  Alejandro Flores Madriz
 *  Emil Dobetsberger
 *  Francisco Ramírez Aldaco
 *  Pablo Ortega-Kral
 */

#include <DS1307Pico.h>
#define IR 5
#define FOTO_RESISTOR A0

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

int is_presence(){
  return digitalRead(IR);
}

void setup() {
  Serial.begin(9600);
  pinMode(IR, INPUT);
  pinMode(FOTO_RESISTOR, INPUT);
  rtc.begin();
  // Optional setup to set an initial time
  //rtc.DSadjust(22, 40, 0, 2023, 3, 2); // 00:15:21 16 Mar 2022
 
}

void loop() {
  if(millis() - lastTime >= 1000) {
    rtc.DSread(); 
    read_time();
    Serial.println("Ṕhotoresistor: " + String(read_from_photores()));
    lastTime = millis();
   }

}
