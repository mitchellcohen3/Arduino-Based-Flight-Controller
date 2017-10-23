imu::Quaternion euler_to_quat(double pitch, double roll, double yaw){ 
  
  /*Converts a given Euler rotation to a quaternion*/

  // Abbreviations for the various angular functions
  double cy = cos(yaw * 0.5);
  double sy = sin(yaw * 0.5);
  double cr = cos(roll * 0.5);
  double sr = sin(roll * 0.5);
  double cp = cos(pitch * 0.5);
  double sp = sin(pitch * 0.5);

  //Calculate the quaternion parameters
  double w = cy * cr * cp + sy * sr * sp;
  double x = cy * sr * cp - sy * cr * sp;
  double y = cy * cr * sp + sy * sr * cp;
  double z = sy * cr * cp - cy * sr * sp;

  //Assign parameters to quaternion object
  imu::Quaternion q(w,x,y,z);
  return q;
}

imu::Quaternion calculate_error_quat(imu::Quaternion q_ref, imu::Quaternion q_current){
  
   /*Calculates the error quaternion based on two input quaternions*/
   
   imu::Quaternion q_conj = q_current.conjugate();
   imu::Quaternion error_quat = q_ref*(q_conj);
   return error_quat;
}
 



