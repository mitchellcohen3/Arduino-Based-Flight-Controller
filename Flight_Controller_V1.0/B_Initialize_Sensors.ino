void initialize_all_sensors(){
    initialize_imu();
    initialize_GPS();
  }
  
void initialize_imu(){
    
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
}



void initialize_GPS(){
    //SoftwareSerial mySerial(3,2); //if we want to print data to the serial monitor
    #define GPSECHO  true
    // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
    GPS.begin(9600);
    //turn on RMC (recommended minimum) and GGA (fix data) including altitude
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY); 
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
    GPS.sendCommand(PGCMD_ANTENNA);       
    //useInterrupt(false);  //option to have a timer interrupt go off and read data every 1 millisecond
     
}

    

    
    

