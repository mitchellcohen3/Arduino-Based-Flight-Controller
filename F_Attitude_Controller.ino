/*Initialize PID loops for roll, pitch and yaw*/

PIDLoop pitch_PID = PIDLoop(dt_inner, 85,-40, KP_Y, KD_Y, KI_Y, I_MAX_PITCH);
PIDLoop roll_PID_R = PIDLoop(dt_inner, 60, -25, KP_X, KD_X, KI_X, I_MAX_ROLL);
PIDLoop roll_PID_L = PIDLoop(dt_inner, 60, -60, KP_X, KD_X, KI_X, I_MAX_ROLL);
PIDLoop yaw_PID = PIDLoop(dt_inner, 40, -40, KP_Z, KD_Z, KI_Z, I_MAX_ROLL);

//Function to control the elevator of the UAV
void control_pitch(double y_error) { 

double pitch_servo_output  = pitch_PID.calculate_output(y_error, pitch_rate);
        
        if(abs(pitch_servo_output) >= TOL){
            elevator_servo.write(E_MIDDLE_POS+pitch_servo_output);   
        }   
}

//Function to control the ailerons and rudder of the UAV 
void control_roll_and_yaw(double x_error){
    
//Calculate roll output and write to ailerons. Left and right ailerons move in opposite directions
double roll_servo_output_R = roll_PID_R.calculate_output(x_error, roll_rate);
double roll_servo_output_L = roll_PID_L.calculate_output(x_error, roll_rate);

    aileron_servo_right.write(AR_MIDDLE_POS+roll_servo_output_R);
    aileron_servo_left.write(AL_MIDDLE_POS+roll_servo_output_L);
    
//Calculate yaw output and write to rudder servo
double rudder_servo_output = yaw_PID.calculate_output(x_error, roll_rate);

    if(abs(rudder_servo_output) >= TOL){
        rudder_servo.write(R_MIDDLE_POS+rudder_servo_output);
    }
    
}

void attitude_control(){
  
//Error in rotation about x (roll) and y (pitch)
double x_error; 
double y_error;

//Calculate desired rotation in quaterions based on desired attitude setpoint
    imu::Quaternion q_ref = euler_to_quat(desired_pitch, desired_roll, desired_yaw);    

//Normalize the reference and measured quaternion
    q_ref.normalize(); 
    q_actual_filtered.normalize(); 

//Calculate error quaternion from desired rotation compared to actual rotation
    imu::Quaternion q_error = calculate_error_quat(q_ref, q_actual_filtered); 

//Determine true error quaternion based on the current yaw location and switch state

    x_error = switch_state*q_error.x();
    y_error = switch_state*q_error.y();

//Failsafe to ensure that errors are correct direction
    if(angular_roll_error*x_error < 0){
        x_error = -x_error;
    }
    
//Pass these errors to pitch, roll and yaw loops 
//Desired roll is also passed to the pitch loop and this is scaled by the A_E_MIX constant
    control_pitch(-y_error - abs(toDeg(desired_roll))*A_E_MIX);  
    control_roll_and_yaw(x_error);

}








