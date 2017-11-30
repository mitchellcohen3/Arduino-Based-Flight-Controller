void initialize_all_sensors(){
    //initialize_imu();
    initialize_servos();
    initialize_GPS();
  }
  
void initialize_imu(){
  Serial.println("Orientation Sensor Raw Data Test"); Serial.println("");
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

    /*Write initial angles to servos*/
    
    aileron_servo_left.write(55);
    aileron_servo_right.write(90);
    elevator_servo.write(55);
    rudder_servo.write(110);
    
    Serial.println("Servos Initialized");
}

void initialize_GPS(){
    // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
    GPS.begin(9600);
    //turn on RMC (recommended minimum) and GGA (fix data) including altitude
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); 
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
    GPS.sendCommand(PGCMD_ANTENNA);      
    //useInterrupt(false);  //option to have a timer interrupt go off and read data every 1 millisecond 
       
}

void hold_for_gps_fix(){
  nlo = 0;
  nla = 0;
  num_sats = 0;
  
  //Stuck in following loop until a gps fix is obtained.
  while (nla == 0 || nlo == 0 || num_sats <= 2){
    
   while (mySerial.available())  gps.encode(mySerial.read());
   nla = (gps.location.lat());
   nlo = (gps.location.lng());
   num_sats = gps.satellites.value();

  }
}

    
    



