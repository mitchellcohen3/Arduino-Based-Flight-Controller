  void initialize_all_sensors(){
    initialize_imu();
  }
  
  void initialize_imu(){
    
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  }
