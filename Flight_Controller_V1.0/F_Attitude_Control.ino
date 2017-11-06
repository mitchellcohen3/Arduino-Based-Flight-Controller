/* Declare global variables*/

double desired_roll;

double x_error; //Error in rotation about x
double y_error; //Error in rotation about y
double z_error; //Error in rotation about z

/*Initialize PID loops for roll, pitch and yaw*/

PIDLoop pitch_PID = PIDLoop(dt, MAX_SERVO_LIMIT_Y, -MAX_SERVO_LIMIT_Y, KP_Y, KD_Y, KI_Y);
PIDLoop roll_PID = PIDLoop(dt, MAX_SERVO_LIMIT_X, -MAX_SERVO_LIMIT_X, KP_X, KD_X, KI_X);
PIDLoop yaw_PID = PIDLoop(dt, MAX_SERVO_LIMIT_Z, -MAX_SERVO_LIMIT_Z, KP_Z, KD_Z, KI_Z);

PIDLoop position_PID = PIDLoop(dt, MAX_ROLL_LIMIT, -MAX_ROLL_LIMIT, KP_POSITION, KD_POSITION, KI_POSITION);

void control_pitch() { //Function to control the pitch of the UAV

    double servo_output  = pitch_PID.calculate_output(y_error);
    elevator_servo.write(90+servo_output);
}
    
void control_roll_and_yaw(){

    double roll_servo_output = roll_PID.calculate_output(x_error);
    aileron_servo.write(90+roll_servo_output);
    //double yaw_servo_output = yaw_PID.calculate_output(y_error);

}

void attitude_control(){
    imu::Quaternion q_ref = euler_to_quat(DESIRED_PITCH, desired_roll, desired_yaw);    //Calculate desired rotation in quaternions
    q_ref.normalize();
    q_actual.normalize();
    imu::Quaternion q_error = calculate_error_quat(q_ref, q_actual); //Calculate error quaternion from desired rotation compared to actual rotation
    
   if(q_error.w() > 0){
        x_error = q_error.x();
        y_error = q_error.y();
        z_error = q_error.z();
      }

    else if(q_error.w() < 0){
       x_error = -q_error.x();
       y_error = -q_error.y();
       z_error = -q_error.z();
    }
    
    control_pitch();  
    control_roll_and_yaw();

}

void position_control(){

}

void control(){
  
    double gps_previous;
    double imu_previous;
    
    if(millis() - gps_previous > gps_period){
        position_control();
        gps_previous = millis();
    }

    if(millis() - imu_previous > imu_period){
        attitude_control();
        imu_previous = millis();
    }
}



