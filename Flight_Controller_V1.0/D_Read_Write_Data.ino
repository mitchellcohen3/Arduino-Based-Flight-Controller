imu::Quaternion q_actual;
float desired_yaw;
float actual_heading;
unsigned long previous_file_size = 0;

void read_data(){
    q_actual = bno.getQuat(); //Get actual orientation (in quaternions)
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    desired_yaw = (euler.x())*PI/180;

    while (Serial1.available())  gps.encode(Serial1.read());
    nlo = (gps.location.lng()); //Using TinyGPS++ library
    nla = (gps.location.lat()); //Using TinyGPS++ library
    actual_heading = gps.course.deg();
    
    datastring = "";
    datastring += millis();
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

