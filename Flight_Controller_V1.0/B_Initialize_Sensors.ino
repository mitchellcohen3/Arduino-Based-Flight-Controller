void initialize_all_sensors(){
    initialize_imu();
<<<<<<< HEAD
    initialize_servos();
=======
    initialize_GPS();
>>>>>>> 9d07381362fdb72110c6a0ca92e399c6ed23bcbc
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
    elevator_servo.attach(SERVO_PIN_ELEVATOR);
    aileron_servo.attach(SERVO_PIN_AILERON_LEFT);
    aileron_servo.write(90);
    elevator_servo.write(90);
    Serial.println("Servo Initialized");
    
    //rudder_servo.attach(SERVO_PIN_RUDDER);
    //aileron_right_servo.attach(SERVO_PIN_AILERON_LEFT);
    //aileron_left_servo.attach(SERVO_PIN_AILERON_RIGHT);
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

    

    
    



