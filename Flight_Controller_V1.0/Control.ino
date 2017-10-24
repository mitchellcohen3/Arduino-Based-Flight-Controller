/* Declare global variables*/

const int DESIRED_PITCH = 0;
double desired_roll = 15;
double desired_yaw = 0;

double x_error;
double y_error;
double z_error;

double u_p_x; //Proportional control effort in x
double u_p_y; //Proportional control effort in y
double u_p_z; //Proportional control effort in z

int servo_output_x; //Servo rotation in x (in deg)
int servo_output_y; //Servo rotation in y (in deg)
int servo_output_z; //Servo rotation in z (in deg)

const double KP_X = 1.0; //Proportional gain in x
const double KP_Y = 1.0; //Proportional gain in y
const double KP_Z = 1.0; //Proportional gain in y

const int MAX_SERVO_LIMIT_X = 50;
const int MAX_SERVO_LIMIT_Y = 50;
const int MAX_SERVO_LIMIT_Z = 50;

/*START OF ATTITUDE CONTROL*/

void control_pitch() { //Function to control the pitch of the UAV
    u_p_x = -x_error*(KP_X);
    servo_output_x = map(u_p_x, -KP_X, KP_X,-MAX_SERVO_LIMIT_X, MAX_SERVO_LIMIT_X);
}
    
void control_roll_and_yaw(){
    delay(100);
}

void control(){
  
    imu::Quaternion q_ref = euler_to_quat(DESIRED_PITCH, desired_roll, desired_yaw);    //Calculate desired rotation in quaternions
    imu::Quaternion q_error = calculate_error_quat(q_ref, q_actual); //Calculate error quaternion from desired rotation compared to actual rotation


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


