/*Initialize PID loops for roll, pitch and yaw*/

PIDLoop pitch_PID = PIDLoop(dt, 85,-40, KP_Y, KD_Y, KI_Y);
PIDLoop roll_PID_R = PIDLoop(dt, 60, -25, KP_X, KD_X, KI_X);
PIDLoop roll_PID_L = PIDLoop(dt, 60, -60, KP_X, KD_X, KI_X);
PIDLoop yaw_PID = PIDLoop(dt, 40, -40, KP_Z, KD_Z, KI_Z);

PIDLoop position_PID = PIDLoop(dt, MAX_ROLL_LIMIT, -MAX_ROLL_LIMIT, KP_POSITION, KD_POSITION, KI_POSITION);

void control_pitch(double y_error) { //Function to control the elevator of the UAV

float pitch_servo_output  = pitch_PID.calculate_output(y_error);

        elevator_servo.write(55+pitch_servo_output);
        
}
    
void control_roll_and_yaw(double x_error){//Function to control the ailerons and rudder of the UAV
    
    /*Calculate roll output and write to ailerons. Left and right ailerons move in opposite directions*/
    
float roll_servo_output_R = roll_PID_R.calculate_output(x_error);
float roll_servo_output_L = roll_PID_L.calculate_output(x_error);


    aileron_servo_left.write(75+roll_servo_output_L);
    aileron_servo_right.write(100+roll_servo_output_R);

    /*Calculate yaw output and write to rudder servo*/
float yaw_servo_output = yaw_PID.calculate_output(x_error);

    if(abs(yaw_servo_output) >= TOL){
        rudder_servo.write(110+yaw_servo_output);
    }
    
}

void attitude_control(){
  
double x_error; //Error in rotation about x
double y_error; //Error in rotation about y
double z_error; //Error in rotation about z

    
    imu::Quaternion q_ref = euler_to_quat(desired_pitch, desired_roll, desired_yaw);    //Calculate desired rotation in quaternions
    q_ref.normalize(); //Normalize the reference quaternion
    q_actual.normalize(); //Normalize the measured quaternion
    imu::Quaternion q_error = calculate_error_quat(q_ref, q_actual); //Calculate error quaternion from desired rotation compared to actual rotation

   /*Determine true error quaternion based on the current yaw location*/

    x_error = switch_state*q_error.x();
    y_error = switch_state*q_error.y();

    control_pitch(-y_error);  
    control_roll_and_yaw(x_error);

}






