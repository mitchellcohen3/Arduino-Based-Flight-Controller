/* Declare global variables*/

float desired_roll;
float desired_yaw;

float integral_error_past = 0;

float x_error; //Error in rotation about x
float y_error; //Error in rotation about y
float z_error; //Error in rotation about z

void control_pitch() { //Function to control the pitch of the UAV
  
    /* Create and attach elevator servo to correct pin*/
    
    Servo elevator_servo;
    elevator_servo.attach(SERVO_PIN_ELEVATOR);

    /*Declare local variables*/
    
    float u_p_x, u_d_x, u_i_x, u_total_x; //Proportional, derivative, integral, and total control efforts in x
    
    int servo_output_x; //Servo rotation in x (in deg)

    
    float integral_error = dt*(x_error) + integral_error_past; //Caluclate integral error

    /*Calculate control efforts*/
    
    u_p_x = -x_error*(KP_X);
    u_i_x = integral_error*(KI_X);

    u_total_x = u_p_x + u_i_x;

    /*Calculate servo output*/
    
    servo_output_x = map(u_total_x, -KP_X, KP_X,-MAX_SERVO_LIMIT_X, MAX_SERVO_LIMIT_X);
    elevator_servo.write(servo_output_x);  

    /*Update integral error*/
    
    integral_error_past += integral_error;
}
    
void control_roll_and_yaw(){
  
    /* Create and attach rudder and aileron servos to correct pins*/

    Servo rudder_servo;
    Servo aileron_right_servo;
    Servo aileron_left_servo;

    rudder_servo.attach(SERVO_PIN_RUDDER);
    aileron_right_servo.attach(SERVO_PIN_AILERON_LEFT);
    aileron_left_servo.attach(SERVO_PIN_AILERON_RIGHT);
    
    float u_p_y; //Proportional control effort in y
    float u_p_z; //Proportional control effort in z
    
    int servo_output_y; //Servo rotation in y (in deg)
    int servo_output_z; //Servo rotation in z (in deg)

}

void attitude_control(){
  
    imu::Quaternion q_ref = euler_to_quat(DESIRED_PITCH, desired_roll, desired_yaw);    //Calculate desired rotation in quaternions
    imu::Quaternion q_error = calculate_error_quat(q_ref, q_actual); //Calculate error quaternion from desired rotation compared to actual rotation

    /*Calculate the true error in x, y and z*/
    
    if(q_error.w() > 0){
        x_error = q_error.x();
        y_error = q_error.y();
        z_error = q_error.z();
    }

    else{
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
  
    float gps_previous;
    float imu_previous;
    
    if(millis() - gps_previous > gps_period){
        position_control();
        gps_previous = millis();
    }

    if(millis() - imu_previous > imu_period){
        attitude_control();
        imu_previous = millis();
    }
}


