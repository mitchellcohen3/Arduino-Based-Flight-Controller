/***************************************************************************
  BMP280 humidity, temperature & pressure sensor

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required 
  to interface.

  Arduino Library Reference: https://learn.adafruit.com/adafruit-bmp280-barometric-pressure-plus-temperature-sensor-breakout/arduino-test

 ***************************************************************************/

float seaLevelhPA = 1013.25; // will be replaced by first pressure reading from pressure sensor

float get_sealevel_pressure(){
  
  seaLevelhPA = bmp.readPressure();
  return seaLevelhPA;
  }

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





