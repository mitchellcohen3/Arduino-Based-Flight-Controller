const double DESIRED_PITCH = 0;

const int MAX_SERVO_LIMIT_X = 80;
const int MAX_SERVO_LIMIT_Y = 80;
const int MAX_SERVO_LIMIT_Z = 50;

const float MAX_ROLL_LIMIT = 40;

const int MAX_ROLL_ROTATION = 20; //Max desired roll in degrees

const float dt = 0.10; //Timestep for integration 

const float gps_period = 1000;
const float imu_period = 10000;

const float TOL = 4.00; // tolerance for deadband in PID

    /*Input gains below*/

const float KP_X = 200; //Proportional gain in x
const float KD_X = 0; //Derivative gain in x
const float KI_X = 0; //Integral gain in x

const float KP_Y = 100; //Proportional gain in y
const float KD_Y = 0; //Derivative gain in y
const float KI_Y = 0; //Integral gain in y

const float KP_Z = 100; //Proportional gain in z
const float KD_Z = 0; //Derivative gain in z
const float KI_Z = 0; //Integral gain in z


const float KP_POSITION = 50;
const float KD_POSITION = 0;
const float KI_POSITION = 0;

    /*Define Servo pins*/

const int SERVO_PIN_ELEVATOR = 9;
const int SERVO_PIN_AILERON_LEFT = 10;
const int SERVO_PIN_AILERON_RIGHT = 11;
const int SERVO_PIN_RUDDER = 12;


