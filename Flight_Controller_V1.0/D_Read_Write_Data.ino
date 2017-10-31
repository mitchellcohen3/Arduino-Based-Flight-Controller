imu::Quaternion q_actual;
float desired_yaw;

void read_write_data(){
    q_actual = bno.getQuat(); //Get actual orientation (in quaternions)
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    desired_yaw = (euler.x()) * 3.141592/180;
}


