imu::Quaternion euler_to_quat(double pitch, double roll, double yaw){ 
  
  /*Converts a given Euler rotation to a quaternion*/

  /*Abbreviations for the various angular functions*/
  double cy = cos(yaw * 0.5);
  double sy = sin(yaw * 0.5);
  double cr = cos(roll * 0.5);
  double sr = sin(roll * 0.5);
  double cp = cos(pitch * 0.5);
  double sp = sin(pitch * 0.5);

  /*Calculate the quaternion parameters*/
  double w = cy * cr * cp + sy * sr * sp;
  double x = cy * sr * cp - sy * cr * sp;
  double y = cy * cr * sp + sy * sr * cp;
  double z = sy * cr * cp - cy * sr * sp;

  /*Assign parameters to quaternion object*/
  imu::Quaternion q(w,x,y,z);
  return q;
}

imu::Quaternion calculate_error_quat(imu::Quaternion q_ref, imu::Quaternion q_current){
  
   /*Calculates the error quaternion based on two input quaternions*/
   
   imu::Quaternion q_conj = q_current.conjugate();
   imu::Quaternion error_quat = q_ref*(q_conj);
   return error_quat;
}

class PIDLoop{
    public:
        PIDLoop(float dt, int maximum, int minimum, float Kp, float Kd, float Ki);
        float calculate_output(float error);
    private:
        float _dt;
        int _maximum;
        int _minimum;
        float _Kp;
        float _Kd;
        float _Ki;
        float _pre_error;
        float _integral;
};

PIDLoop::PIDLoop(float dt, int maximum, int minimum, float Kp, float Kd, float Ki):
    _dt(dt),
    _maximum(maximum),
    _minimum(minimum),
    _Kp(Kp),
    _Kd(Kd),
    _Ki(Ki),
    _pre_error(0),
    _integral(0){}

float PIDLoop::calculate_output(float error){
  
    // Proportional term
    float Pout = _Kp * error;

    // Integral term
    _integral += error * _dt;
    float Iout = _Ki * _integral;

    // Derivative term
    float derivative = (error - _pre_error) / _dt;
    float Dout = _Kd * derivative;

    // Calculate total output
    float output = Pout + Iout + Dout;

    // Restrict to max/min
    if( output > _maximum )
        output = _maximum;
    else if( output < _minimum )
        output = _minimum;

    // Save error to previous error
    _pre_error = error;

    return output;
}




