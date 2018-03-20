const float MAX_ROLL_LIMIT = 45;
const float MAX_PITCH_LIMIT = 35;

const float I_MAX_PITCH = 3;
const float I_MAX_ROLL = 3;

const float dt_inner = 0.01; //Innerloop timestep for integration (s) 
const float dt_outer = 0.1; //Outerloop timestep for integration (s)

const double gps_period = 100;
const double imu_period = 10;
const double print_period = 1000;

const float TOL = 0; // tolerance for deadband in PID

//Input gains below

const float KP_X = 25; //Proportional gain in x (Roll)
const float KD_X = 0; //Derivative gain in x
const float KI_X = 0; //Integral gain in x

const float KP_Y = 25; //Proportional gain in y (Pitch)
const float KD_Y = 0; //Derivative gain in y
const float KI_Y = 0; //Integral gain in y

const float KP_Z = 10; //Proportional gain in z (Rudder)
const float KD_Z = 0; //Derivative gain in z
const float KI_Z = 0; //Integral gain in z

const float KP_POSITION = 1;
const float KD_POSITION = 0;
const float KI_POSITION = 0;

//Define Servo pins

const int SERVO_PIN_ELEVATOR = 10;
const int SERVO_PIN_AILERON_LEFT = 7;
const int SERVO_PIN_AILERON_RIGHT = 9;
const int SERVO_PIN_RUDDER = 8;
const int SERVO_PIN_CUTDOWN = 38;

const int temp_ready = 1; // Status of cut-down mechanism (Until we set up the input coming from the actual mechanism)
float base_altitude;


unsigned long gps_previous = 0;
unsigned long imu_previous = 0;
float seaLevelhPA;

String gains_X;
String gains_Y;
String gains_Z;

//Input middle servo positions below (adjust based on the trim of the aircraft)
const float AR_MIDDLE_POS = 115;
const float AL_MIDDLE_POS = 78;
const float R_MIDDLE_POS = 95;
const float E_MIDDLE_POS = 95;



