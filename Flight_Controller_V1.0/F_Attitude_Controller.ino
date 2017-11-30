/*Initialize PID loops for roll, pitch and yaw*/

PIDLoop pitch_PID = PIDLoop(dt, 85,-30, KP_Y, KD_Y, KI_Y);
PIDLoop roll_PID = PIDLoop(dt, 60, -25, KP_X, KD_X, KI_X);
PIDLoop yaw_PID = PIDLoop(dt, 40, -40, KP_Z, KD_Z, KI_Z);

PIDLoop position_PID = PIDLoop(dt, MAX_ROLL_LIMIT, -MAX_ROLL_LIMIT, KP_POSITION, KD_POSITION, KI_POSITION);

void control_pitch(double y_error) { //Function to control the elevator of the UAV

float pitch_servo_output  = pitch_PID.calculate_output(y_error);

    if(pitch_servo_output >= TOL){
        elevator_servo.write(55+pitch_servo_output);
    }
}
    
void control_roll_and_yaw(double x_error){//Function to control the ailerons and rudder of the UAV
    
    /*Calculate roll output and write to ailerons. Left and right ailerons move in opposite directions*/
float roll_servo_output = roll_PID.calculate_output(x_error);


    aileron_servo_left.write(55+roll_servo_output);
    aileron_servo_right.write(90-roll_servo_output);

    /*Calculate yaw output and write to rudder servo*/
float yaw_servo_output = yaw_PID.calculate_output(x_error);

    if(yaw_servo_output >= TOL){
        rudder_servo.write(110+yaw_servo_output);
    }
    
}

void attitude_control(){
  
double x_error; //Error in rotation about x
double y_error; //Error in rotation about y
double z_error; //Error in rotation about z
    
    imu::Quaternion q_ref = euler_to_quat(DESIRED_PITCH, desired_roll, desired_yaw);    //Calculate desired rotation in quaternions
    q_ref.normalize(); //Normalize the reference quaternion
    q_actual.normalize(); //Normalize the measured quaternion
    imu::Quaternion q_error = calculate_error_quat(q_ref, q_actual); //Calculate error quaternion from desired rotation compared to actual rotation

   /*Determine true error quaternion based on the sign of the scalar component*/
   
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
    
    control_pitch(y_error);  
    control_roll_and_yaw(x_error);

}





