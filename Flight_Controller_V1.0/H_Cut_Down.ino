/***************************************************************************
  BMP280 humidity, temperature & pressure sensor

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required 
  to interface.

  Arduino Library Reference: https://learn.adafruit.com/adafruit-bmp280-barometric-pressure-plus-temperature-sensor-breakout/arduino-test

 ***************************************************************************/

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11 
#define BMP_CS 10

<<<<<<< HEAD
// TO DELETE WHEN WORKING (from original example)
//Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);
=======
float current_alt = 0.0; // initializing
float seaLevelhPA = 1013.25; // will be replaced by first pressure reading from pressure sensor

float get_sealevel_pressure(){
  
  seaLevelhPA = bmp.readPressure();
  return seaLevelhPA;
  }
>>>>>>> 069728703630d61ecf46326ca6022fefb0f86bf5

void get_altitude() {

    Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
    
    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    Serial.print(F("Approx altitude = "));
    Serial.print(bmp.readAltitude(seaLevelhPA)); // adjusted sea level pressure
    Serial.println(" m");
    
    current_alt = bmp.readAltitude(seaLevelhPA);
    Serial.print(F("Stored altitude test = "));
    Serial.print(current_alt);
    Serial.println(" m");
    
    Serial.println();
    delay(2000);

    Serial.print(F("cd_ready = ")); // ready for cutdown ( 1 = yes, 0 = no)
    Serial.println(cd_ready); 
    Serial.println();

    if ((current_alt >= cut_alt) && (cd_ready == 1)){ // Needs to reach the cut-down altitude and the cut-down mechanism needs to be ready to release, so it won't re-enter this loop
      
    //  move_cutdown_test();

      cd_ready = 0;

      }
    
}





