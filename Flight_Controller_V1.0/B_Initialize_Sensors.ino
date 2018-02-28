void initialize_all_sensors(){
    initialize_GPS();
  }
  

void initialize_GPS(){
    // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
    GPS.begin(9600);
    mySerial.begin(9600);
    //turn on RMC (recommended minimum) and GGA (fix data) including altitude
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); 
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);   // 1 Hz update rate
    GPS.sendCommand(PGCMD_ANTENNA);      
    
  #ifdef __arm__
    useInterrupt(false);  //NOTE - we don't want to use interrupts on the Due
  #else
    useInterrupt(true);
  #endif
} 




