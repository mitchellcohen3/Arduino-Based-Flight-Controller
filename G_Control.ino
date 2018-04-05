void control(){
  
   //Run the position controller at 10 Hz, only if the flight mode is set to 'Autopilot'
   if(flight_mode == 2){
      if(millis() - gps_previous > gps_period){
          position_control();
          gps_previous = millis();
        }
    }

   //Run the attitude controller at 100Hz, only if the IMU is connected
   if(using_imu){
      if(millis() - imu_previous > imu_period){
          attitude_control();
          imu_previous = millis();
        }      
    }      
}

void manual_flight(){

//Get current stick commands and map them to number of degrees of servo turn
double aileron_servo_output = -map(pulse_time_roll, 1000, 2000, -45, 45);
double elevator_servo_output = map(pulse_time_pitch, 1000, 2000, -45, 45);
double rudder_servo_output = -map(pulse_time_rudder, 1000, 2000, -30, 30);

//Write new servo positions
elevator_servo.write(E_MIDDLE_POS+elevator_servo_output);
aileron_servo_right.write(AR_MIDDLE_POS+aileron_servo_output);
aileron_servo_left.write(AL_MIDDLE_POS+aileron_servo_output);
rudder_servo.write(R_MIDDLE_POS+rudder_servo_output);
    
}

void reset_integrators(){
    pitch_PID.reset_integrator();
    roll_PID_R.reset_integrator();
    roll_PID_L.reset_integrator();
    yaw_PID.reset_integrator();
}


