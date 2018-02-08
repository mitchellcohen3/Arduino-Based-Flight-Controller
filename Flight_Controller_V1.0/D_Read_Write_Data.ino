imu::Quaternion q_actual;
imu::Quaternion q_actual_filtered;

float desired_yaw_euler;
float desired_yaw;
float actual_heading;
unsigned long previous_file_size = 0;
float pressure;
int switch_state = 1;
float desired_yaw_previous = 0;
float current_alt;


//Initialize filter cutoff frequencies 

float filterFrequency_imu = 3;  
float filterFrequency_tx = 5;

// Create lowpass filter objects for lowpass filtering
FilterOnePole lowpassFilter_imu_1( LOWPASS, filterFrequency_imu ); 
FilterOnePole lowpassFilter_imu_2( LOWPASS, filterFrequency_imu );
FilterOnePole lowpassFilter_imu_3( LOWPASS, filterFrequency_imu );
FilterOnePole lowpassFilter_imu_4( LOWPASS, filterFrequency_imu ); 
FilterOnePole lowpassFilter_tx_1( LOWPASS, filterFrequency_tx ); 
FilterOnePole lowpassFilter_tx_2( LOWPASS, filterFrequency_tx ); 

void read_data(){
  
//Read transceiver values for attitude setpoints

    desired_pitch = map(pulse_time_pitch, 1000, 2000, -MAX_PITCH_LIMIT, MAX_PITCH_LIMIT);
    desired_pitch = -toRad(desired_pitch);
    desired_pitch = lowpassFilter_tx_1.input(desired_pitch);

    desired_roll = map(pulse_time_roll, 1000, 2000, -MAX_ROLL_LIMIT, MAX_ROLL_LIMIT);
    desired_roll = -toRad(desired_roll);
    desired_roll = lowpassFilter_tx_2.input(desired_roll);
    
    q_actual = bno.getQuat(); //Get actual orientation (in quaternions)

//Filter quaternion data
    
    q_actual_filtered.w() = lowpassFilter_imu_1.input( q_actual.w() );
    q_actual_filtered.x() = lowpassFilter_imu_2.input( q_actual.x() );
    q_actual_filtered.y() = lowpassFilter_imu_3.input( q_actual.y() );
    q_actual_filtered.z() = lowpassFilter_imu_4.input( q_actual.z() );

//Get actual orientation in Euler angles
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    desired_yaw = toRad(euler.x());

//Read pressure
    pressure = bmp.readPressure();
    current_alt = bmp.readAltitude(seaLevelhPA);

//This part is needed to detect a discontinuity in the yaw and flip a switch*/
    if(detect_discontinuity(desired_yaw_previous, desired_yaw)){
          if(switch_state == 1){
              switch_state = -1;              
          }     
          else{
              switch_state = 1;
          }
    }

 //Read GPS Data
    while (Serial1.available())  gps.encode(Serial1.read());
    nlo = (gps.location.lng()); //Using TinyGPS++ library
    nla = (gps.location.lat()); //Using TinyGPS++ library
    actual_heading = gps.course.deg();

//Write all data to SD
    datastring = "";
    datastring += millis();
    datastring += ",";
    datastring += pressure;
    datastring += ",";
    datastring += current_alt;
    datastring += ",";
    datastring += euler.x();
    datastring += ",";
    datastring += euler.y();
    datastring += ",";
    datastring += euler.z();
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
    
    desired_yaw_previous = desired_yaw;
}

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

