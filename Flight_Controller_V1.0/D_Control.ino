/* Declare global variables*/

const int DESIRED_PITCH = 0;
double desired_roll;
double desired_yaw;

double integral_error_past = 0;

double x_error; //Error in rotation about x
double y_error; //Error in rotation about y
double z_error; //Error in rotation about z

const int MAX_SERVO_LIMIT_X = 50;
const int MAX_SERVO_LIMIT_Y = 50;
const int MAX_SERVO_LIMIT_Z = 50;

const int MAX_ROLL_ROTATION = 20; //Max desired roll in degrees

/*START OF ATTITUDE CONTROL*/

void control_pitch() { //Function to control the pitch of the UAV
  
    /* Create and attach elevator servo to correct pin*/
    
    Servo elevator_servo;
    elevator_servo.attach(SERVO_PIN_ELEVATOR);

    /*Declare local variables*/
    
    double u_p_x, u_d_x, u_i_x, u_total_x; //Proportional, derivative, integral, and total control efforts in x
    
    const double KP_X = 1.0; //Proportional gain in x
    const double KI_X = 0; //Integral gain in x
    
    int servo_output_x; //Servo rotation in x (in deg)
    double dt = 0.10; //Timestep for integration
    
    double integral_error = dt*(x_error) + integral_error_past; //Caluclate integral error

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
    Servo aileron_servo;

    rudder_servo.attach(SERVO_PIN_RUDDER);
    aileron_servo.attach(SERVO_PIN_AILERONS);
    
    double u_p_y; //Proportional control effort in y
    double u_p_z; //Proportional control effort in z

    const double KP_Y = 1.0; //Proportional gain in y
    const double KP_Z = 1.0; //Proportional gain in y
    
    int servo_output_y; //Servo rotation in y (in deg)
    int servo_output_z; //Servo rotation in z (in deg)

}

void control(){
  
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


