const int DESIRED_PITCH = 0;

const int MAX_SERVO_LIMIT_Y = 50;
const int MAX_SERVO_LIMIT_Z = 50;
const int MAX_SERVO_LIMIT_X = 50;

const int MAX_ROLL_ROTATION = 20; //Max desired roll in degrees

const double KP_X = 1.0; //Proportional gain in x
const double KP_Y = 1.0; //Proportional gain in y
const double KP_Z = 1.0; //Proportional gain in y

const double KI_X = 0; //Integral gain in x

const double dt = 0.10; //Timestep for integration

const double gps_period = 1000;
const double imu_period = 100;

    /*Define Servo pins*/

const int SERVO_PIN_ELEVATOR = 9;
const int SERVO_PIN_AILERON_LEFT = 10;
const int SERVO_PIN_AILERON_RIGHT = 11;
const int SERVO_PIN_RUDDER = 12;
