void initialize_all_sensors(){
    initialize_imu();
    initialize_servos();
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


    

    
    



