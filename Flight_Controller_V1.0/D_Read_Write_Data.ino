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


// filters out changes faster that 5 Hz.
float filterFrequency = 5.0;  
// create a one pole (RC) lowpass filter
FilterOnePole lowpassFilter( LOWPASS, filterFrequency );  

void read_data(){
  
    desired_pitch = map(pulse_time, 1000, 2000, -50, 50);
    desired_pitch = -toRad(desired_pitch);
    Serial.println(desired_pitch);
    
    q_actual = bno.getQuat(); //Get actual orientation (in quaternions)

    q_actual_filtered.w() = lowpassFilter.input( q_actual.w() );
    q_actual_filtered.x() = lowpassFilter.input( q_actual.x() );
    q_actual_filtered.y() = lowpassFilter.input( q_actual.y() );
    q_actual_filtered.z() = lowpassFilter.input( q_actual.z() );
    
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    desired_yaw = toRad(euler.x());
    
    pressure = bmp.readPressure();

//This part is needed to detect a discontinuity in the yaw and flip a switch
    if(detect_discontinuity(desired_yaw_previous, desired_yaw)){
          if(switch_state == 1){
              switch_state = -1;              
          }     
          else{
              switch_state = 1;
          }
    }
    
    while (Serial1.available())  gps.encode(Serial1.read());
    nlo = (gps.location.lng()); //Using TinyGPS++ library
    nla = (gps.location.lat()); //Using TinyGPS++ library
    actual_heading = gps.course.deg();
    
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
    datastring += desired_pitch;
    datastring += ",";
    datastring += desired_roll;
    datastring += ",";
    
    desired_yaw_previous = desired_yaw;
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


