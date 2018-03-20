//Declare quaternions from the imu namespace

imu::Quaternion q_actual;
imu::Quaternion q_actual_filtered;

//Declare
float desired_yaw_deg;
double desired_yaw;
float actual_heading;
float pressure;
int switch_state = 1;
double desired_yaw_previous = 0;

double actual_roll;
double angular_roll_error;

double pitch_rate;
double roll_rate;
double yaw_rate;

//Declare and initialize filter cutoff frequencies 

float filterFrequency_imu = 7;  
float filterFrequency_tx = 7;
float filterFrequency_imu_rates = 2;
float filterFrequency_pressure = 0.5;
float filterFrequency_gps = 3;

// Create lowpass filter objects for lowpass filtering
FilterOnePole lowpassFilter_imu_1( LOWPASS, filterFrequency_imu ); 
FilterOnePole lowpassFilter_imu_2( LOWPASS, filterFrequency_imu );
FilterOnePole lowpassFilter_imu_3( LOWPASS, filterFrequency_imu );
FilterOnePole lowpassFilter_imu_4( LOWPASS, filterFrequency_imu ); 
FilterOnePole lowpassFilter_imu_5( LOWPASS, filterFrequency_imu);
FilterOnePole lowpassFilter_imu_6( LOWPASS, filterFrequency_imu_rates);
FilterOnePole lowpassFilter_imu_7( LOWPASS, filterFrequency_imu_rates);
FilterOnePole lowpassFilter_imu_8( LOWPASS, filterFrequency_imu_rates);

FilterOnePole lowpassFilter_tx_1( LOWPASS, filterFrequency_tx ); 
FilterOnePole lowpassFilter_tx_2( LOWPASS, filterFrequency_tx ); 

FilterOnePole lowpassFilter_pressure( LOWPASS, filterFrequency_pressure ); 
FilterOnePole lowpassFilter_altitude( LOWPASS, filterFrequency_pressure ); 

FilterOnePole lowpassFilter_gps1( LOWPASS, filterFrequency_gps ); 
FilterOnePole lowpassFilter_gps2( LOWPASS, filterFrequency_gps ); 

//Declare GPS variables
float nlo, nla; //current position in long, lat
double nx, ny;  //current position in cartesian coords
double px=0, py=0; //previous position, set to zero because that will be our prev position in the first heading calculation
int counter = 0;   //counts the current setpt
double heading_error;

//Function to read all IMU data
void read_imu_data(){
//Read transceiver values for attitude setpoints
    
    q_actual = bno.getQuat(); //Get actual orientation (in quaternions)
    
//Get angular rates

    imu::Vector<3> gyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    roll_rate = lowpassFilter_imu_6.input(gyro.z());
    pitch_rate = lowpassFilter_imu_7.input(gyro.y());
    yaw_rate = lowpassFilter_imu_8.input(gyro.x());

//Filter quaternion data
    
    q_actual_filtered.w() = lowpassFilter_imu_1.input( q_actual.w() );
    q_actual_filtered.x() = lowpassFilter_imu_2.input( q_actual.x() );
    q_actual_filtered.y() = lowpassFilter_imu_3.input( q_actual.y() );
    q_actual_filtered.z() = lowpassFilter_imu_4.input( q_actual.z() );

//Get actual orientation in Euler angles
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    
    desired_yaw = toRad(euler.x());
    desired_yaw_deg = euler.x(); 
    actual_roll = -euler.z();
   // desired_yaw = lowpassFilter_imu_5.input(desired_yaw);
}

void read_pressure_data(){
//Read pressure
    pressure = lowpassFilter_pressure.input(bmp.readPressure());
    
//Get current altitude
    current_alt = lowpassFilter_altitude.input(bmp.readAltitude(seaLevelhPA)) - base_altitude;
}

void read_data(){
//Read IMU and pressure data
    if(using_imu){
        read_imu_data();
    }
    
    if(using_pressure){
        read_pressure_data();
    }

//This is needed to detect a discontinuity in the yaw and flip a switch
    if(detect_discontinuity(desired_yaw_previous, desired_yaw_deg)){
          if(switch_state == 1){
              switch_state = -1;             
          }     
          else{
              switch_state = 1;
          }
    }

//Read GPS Data, only if new GPS data is ready to be read
    while (Serial1.available())  gps.encode(Serial1.read());
    nlo = lowpassFilter_gps1.input((gps.location.lng())); //Using TinyGPS++ library
    nla = lowpassFilter_gps2.input((gps.location.lat())); //Using TinyGPS++ library

//Write all data to SD
    datastring = "";
    datastring += millis();
    datastring += ",";
    datastring += pressure;
    datastring += ",";
    datastring += current_alt;
    datastring += ",";
    datastring += 0;
    datastring += ",";
    datastring += 0;
    datastring += ",";
    datastring += actual_roll;
    datastring += ",";
    datastring += nlo;
    datastring += ",";
    datastring += nla;
    datastring += ",";
    datastring += elevator_servo.read();
    datastring += ",";
    datastring += aileron_servo_left.read();
    datastring += ",";
    datastring += aileron_servo_right.read();
    datastring += ",";
    datastring += rudder_servo.read();
    datastring += ",";
    datastring += toDeg(desired_pitch);
    datastring += ",";
    datastring += toDeg(desired_roll);
    datastring += ",";
    datastring += pitch_rate;
    datastring += ",";
    datastring += roll_rate;
    datastring += ",";
    datastring += yaw_rate;
    datastring += ",";
    datastring += nx;
    datastring += ",";
    datastring += ny;
    datastring += ",";
      
    desired_yaw_previous = desired_yaw_deg;
    angular_roll_error = toDeg(desired_roll) - actual_roll;
}

//Calculate the pulse time of the elevator signal from the receiver
void calcSignal_pitch() 
{
    //record the interrupt time so that we can tell if the receiver has a signal from the transmitter 
    last_interrupt_time_pitch = micros(); 
    //if the pin has gone HIGH, record the microseconds since the Teensy started up 
    if(digitalRead(CHANNEL_2_PIN) == HIGH) 
    { 
        timer_start_pitch = micros();
    } 
    //otherwise, the pin has gone LOW 
    else
    { 
        //only worry about this if the timer has actually started
        if(timer_start_pitch != 0)
        { 
            //record the pulse time
            pulse_time_pitch = ((volatile int)micros() - timer_start_pitch);
            //restart the timer
            timer_start_pitch = 0;
        }
    } 
} 

//Calculate the pulse time of the aileron signal from the receiver
void calcSignal_roll() 
{
    //record the interrupt time so that we can tell if the receiver has a signal from the transmitter 
    last_interrupt_time_roll = micros(); 
    //if the pin has gone HIGH, record the microseconds since the Teensy started up 
    if(digitalRead(CHANNEL_1_PIN) == HIGH) 
    { 
        timer_start_roll = micros();
    } 
    //otherwise, the pin has gone LOW 
    else
    { 
        //only worry about this if the timer has actually started
        if(timer_start_roll != 0)
        { 
            //record the pulse time
            pulse_time_roll = ((volatile int)micros() - timer_start_roll);
            //restart the timer
            timer_start_roll = 0;
        }
    } 
}

//Calculate the pulse time of the flight mode signal from the receiver
void calcSignal_FM() 
{
    //record the interrupt time so that we can tell if the receiver has a signal from the transmitter 
    last_interrupt_time_FM = micros(); 
    //if the pin has gone HIGH, record the microseconds since the Teensy started up 
    if(digitalRead(CHANNEL_6_PIN) == HIGH) 
    { 
        timer_start_FM = micros();
    } 
    //otherwise, the pin has gone LOW 
    else
    { 
        //only worry about this if the timer has actually started
        if(timer_start_FM != 0)
        { 
            //record the pulse time
            pulse_time_FM = ((volatile int)micros() - timer_start_FM);
            //restart the timer
            timer_start_FM = 0;
        }
    } 
}

//Calculate the pulse time of the rudder signal from the receiver
void calcSignal_rudder() 
{
    //record the interrupt time so that we can tell if the receiver has a signal from the transmitter 
    last_interrupt_time_rudder = micros(); 
    //if the pin has gone HIGH, record the microseconds since the Teensy started up 
    if(digitalRead(CHANNEL_4_PIN) == HIGH) 
    { 
        timer_start_rudder = micros();
    } 
    //otherwise, the pin has gone LOW 
    else
    { 
        //only worry about this if the timer has actually started
        if(timer_start_rudder != 0)
        { 
            //record the pulse time
            pulse_time_rudder = ((volatile int)micros() - timer_start_rudder);
            //restart the timer
            timer_start_rudder = 0;
        }
    } 
}

unsigned long previous_file_size = 0;

//Function to write all data to the SD card
void write_data(){
  String file_name = file_prefix + file_num + file_type;
  char data_file_char_array[file_name.length() + 1];
  file_name.toCharArray(data_file_char_array, file_name.length() + 1 );
  File dataFile = SD.open(data_file_char_array, FILE_WRITE);

    
   if (dataFile) {
    dataFile.println(datastring);
    if (dataFile.size() > previous_file_size){
      previous_file_size = dataFile.size();
    }
    dataFile.close();
   }
  
}

//Function to get the desired pitch if flight mode is FBW
double get_desired_pitch(){
    desired_pitch = map(pulse_time_pitch, 1000, 2000, -MAX_PITCH_LIMIT, MAX_PITCH_LIMIT);
    desired_pitch = -toRad(desired_pitch);
    desired_pitch = lowpassFilter_tx_1.input(desired_pitch);
    return desired_pitch;
}

//Function to get the desired pitch if flight mode is FBW
double get_desired_roll(){
    desired_roll = map(pulse_time_roll, 1000, 2000, -MAX_ROLL_LIMIT, MAX_ROLL_LIMIT) - 1;
    desired_roll = -toRad(desired_roll);
    desired_roll = lowpassFilter_tx_2.input(desired_roll);   
    return desired_roll;
}

