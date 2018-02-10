const float MAX_ROLL_LIMIT = 90;
const float MAX_PITCH_LIMIT = 90;

const float dt_inner = 0.01; //Innerloop timestep for integration 
const float dt_outer = 0.1; //Outerloop timestep for integration

const double gps_period = 1000;
const double imu_period = 10;

const float TOL = 0; // tolerance for deadband in PID

    /*Input gains below*/

const float KP_X = 50; //Proportional gain in x (Roll)
const float KD_X = 0; //Derivative gain in x
const float KI_X = 0; //Integral gain in x

const float KP_Y = 50; //Proportional gain in y (Pitch)
const float KD_Y = 0; //Derivative gain in y
const float KI_Y = 0; //Integral gain in y

const float KP_Z = 25;//Proportional gain in z (Rudder)
const float KD_Z = 0; //Derivative gain in z
const float KI_Z = 0; //Integral gain in z

const float KP_POSITION = 1;
const float KD_POSITION = 0;
const float KI_POSITION = 0;

    /*Define Servo pins*/

const int SERVO_PIN_ELEVATOR = 10;
const int SERVO_PIN_AILERON_LEFT = 7;
const int SERVO_PIN_AILERON_RIGHT = 9;
const int SERVO_PIN_RUDDER = 8;
const int SERVO_PIN_CUTDOWN = 13;

const int temp_ready = 1; // Status of cut-down mechanism (Until we set up the input coming from the actual mechanism)
const float cut_alt = 60; // 200ft = approx 60m
float base_altitude;

unsigned long gps_previous = 0;
unsigned long imu_previous = 0;
float seaLevelhPA;

String gains_X;
String gains_Y;
String gains_Z;



