void initialize_all_sensors(){
    initialize_imu();
    initialize_servos();
    initialize_GPS();
    initialize_pres();
    check_cutdown_ready();
  }
  
void initialize_imu(){
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  bno.setExtCrystalUse(true);
}

void initialize_servos(){

    /*Attach all servo objects to correct pins*/ 
    
    elevator_servo.attach(SERVO_PIN_ELEVATOR);
    aileron_servo_left.attach(SERVO_PIN_AILERON_LEFT);
    aileron_servo_right.attach(SERVO_PIN_AILERON_RIGHT);
    rudder_servo.attach(SERVO_PIN_RUDDER);
    cutdown_servo.attach(SERVO_PIN_CUTDOWN);  // attaches the servo on pin 9 to the servo object

    /*Write initial angles to servos*/
    
    aileron_servo_left.write(75);
    aileron_servo_right.write(100);
    elevator_servo.write(55);
    rudder_servo.write(110);


    Serial.println("Servos Initialized");
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
    usingInterrupt = false;  //NOTE - we don't want to use interrupts on the Due
  #else
    useInterrupt(true);
  #endif
}

void initialize_pres(){
    Serial.begin(9600);
    Serial.println(F("BMP280 test"));
    
    float seaLevelhPA = get_sealevel_pressure();
    Serial.println(F("Baseline sea level pressure = "));
    Serial.println(seaLevelhPA);
    Serial.println(F(" hPA"));    
  
    if (!bmp.begin()) {  
    Serial.println(F("Could not find a valid BMP280 sensor, Find it!"));
    while (1);
  }
}   
   
void hold_for_gps_fix(){
  float nlo2 = 0;
  float nla2 = 0;
  num_sats = 0;
  
  //Stuck in following loop until a gps fix is obtained.
  while (nla2 == 0 || nlo2 == 0 || num_sats <= 2){
    
   while (Serial1.available())  gps.encode(Serial1.read());
   nla2 = (gps.location.lat());
   nlo2 = (gps.location.lng());
   num_sats = gps.satellites.value();
  }
}


#ifdef __AVR__
// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
#ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;  
    // writing direct to UDR0 is much much faster than Serial.print 
    // but only one character can be written at a time. 
#endif
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}
#endif //#ifdef__AVR__
uint32_t timer = millis();


int create_file(){
  
    if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return 0;
  }
  
    int i = 0;
    String file_name = file_prefix + i + file_type;
    String event_file_name = event_file_prefix + i + file_type;
    char data_file_char_array[file_name.length() + 1];
    char event_file_char_array[event_file_name.length() + 1];
        
  while(i < 1000){
    String file_name = file_prefix + i + file_type;
    String event_file_name = event_file_prefix + i + file_type;

    char data_file_char_array[file_name.length() + 1];
    file_name.toCharArray(data_file_char_array, file_name.length() + 1);
    char event_file_char_array[event_file_name.length() + 1];
    event_file_name.toCharArray(event_file_char_array, event_file_name.length() + 1);
    
    Serial.println(file_name);
    
    if (!SD.exists(data_file_char_array)){
      File dataFile = SD.open(data_file_char_array, FILE_WRITE);
      dataFile.println(header);
      dataFile.close();
  
      File eventFile = SD.open(event_file_char_array, FILE_WRITE);
      eventFile.close(); 
      break;

    }
    else {
      i++;
    }
  }

  return i;
}

int cd_ready = 0; //move this later?

void check_cutdown_ready() {
  if ( temp_ready == 1 ){ //CHECK A SERVO POSITION?  --> depends on the mechanism
    cd_ready = 1;
    }
  
  }
 
    




