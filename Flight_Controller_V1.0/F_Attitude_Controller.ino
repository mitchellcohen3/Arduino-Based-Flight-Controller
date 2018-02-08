/*Initialize PID loops for roll, pitch and yaw*/

PIDLoop pitch_PID = PIDLoop(dt_inner, 85,-40, KP_Y, KD_Y, KI_Y);
PIDLoop roll_PID_R = PIDLoop(dt_inner, 60, -25, KP_X, KD_X, KI_X);
PIDLoop roll_PID_L = PIDLoop(dt_inner, 60, -60, KP_X, KD_X, KI_X);
PIDLoop yaw_PID = PIDLoop(dt_inner, 40, -40, KP_Z, KD_Z, KI_Z);

//Function to control the elevator of the UAV
void control_pitch(double y_error) { 

double pitch_servo_output  = pitch_PID.calculate_output(y_error);
        
        if(abs(pitch_servo_output) >= TOL){
            elevator_servo.write(55+pitch_servo_output);   
        }   
}

//Function to control the ailerons and rudder of the UAV 
void control_roll_and_yaw(double x_error){
    
//Calculate roll output and write to ailerons. Left and right ailerons move in opposite directions
    
double roll_servo_output_R = roll_PID_R.calculate_output(x_error);
double roll_servo_output_L = roll_PID_L.calculate_output(x_error);

    if(abs(roll_servo_output_R) >= TOL){
          aileron_servo_right.write(110+roll_servo_output_R);
    }

    if(abs(roll_servo_output_L) >= TOL){
          aileron_servo_left.write(75+roll_servo_output_L);
    }
    
//Calculate yaw output and write to rudder servo
double rudder_servo_output = yaw_PID.calculate_output(x_error);

    if(abs(rudder_servo_output) >= TOL){
        rudder_servo.write(110+rudder_servo_output);
    }
    
}

void attitude_control(){
  
double x_error; //Error in rotation about x (roll)
double y_error; //Error in rotation about y (pitch)
    
    imu::Quaternion q_ref = euler_to_quat(desired_pitch, desired_roll, desired_yaw);    //Calculate desired rotation in quaternions
    q_ref.normalize(); //Normalize the reference quaternion
    q_actual_filtered.normalize(); //Normalize the measured quaternion
    imu::Quaternion q_error = calculate_error_quat(q_ref, q_actual_filtered); //Calculate error quaternion from desired rotation compared to actual rotation

//Determine true error quaternion based on the current yaw location and switch state

    x_error = switch_state*q_error.x();
    y_error = switch_state*q_error.y();

    control_pitch(-y_error);  
    control_roll_and_yaw(x_error);

}






