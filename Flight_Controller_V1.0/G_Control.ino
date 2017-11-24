void control(){
  
    double gps_previous;
    double imu_previous;
    
   /* 
   if(millis() - gps_previous > gps_period){
        position_control();
        gps_previous = millis();
    }
    */

    if(millis() - imu_previous > imu_period){
        attitude_control();
        imu_previous = millis();
    }
}
