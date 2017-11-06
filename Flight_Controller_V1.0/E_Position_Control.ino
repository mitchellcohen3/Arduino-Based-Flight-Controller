//long is x dir, lat is y dir
//Get dist from (lo1, la1) to (lo2, la2)
double distTo(double la1, double lo1, double la2, double lo2){
  double p = 3.141592654;
  double radius = 6371000; //earth's radius in meters 
  double dlat = (p/180)*(la2-la1);
  double dlong = (p/180)*(lo2-lo1);
  // convert the necessary input coords from degrees to radians;
  la1 *= (p/180);
  la2 *= (p/180);
  double a, b, c;
  //using Haversine formula
  a = pow(sin(dlat/2), 2) + pow(sin(dlong/2), 2)*cos(la1)*cos(la2);
  b = sqrt(a)/(sqrt(1-a));
  c = 2*atan(b);
  return (radius * c);
}

//use the following to get the coords of the set points
double mtolat(double dy){
  double p = 3.141592654;
  double r = 6371000; //earth's radius in meters 
  return (p/180)*dy/r;
}

double mtolong(double dx, double lat0){
  double p = 3.141592654;
  double r = 6371000; //earth's radius in meters 
  return (p/180)*(dx/r)/(cos(lat0));
}


double getdx (double lo1, double lo2){
  return (distTo(0, lo1, 0, lo2));
}

double getdy (double la1, double la2){
  return (distTo(la1, 0, la2, 0));
}


//this code takes the current heading and gets the angle between it and the set point's coords
//input the heading as an angle counter-clockwise from directly east
//sign convention will have counter clockwise as positive (roll left)
//plane's location is (x,y), set point at (x2, y2) = (dx+x, dy+y)
double angle(double heading, double x, double y, double x2, double y2){
    double dx=x-x2;
    double dy=y-y2;
    double tht = atan2(dy, dx);
    double r = heading-tht;
    return r;
}


//take in the x and y coords of the current and previous points to approximate the heading angle
double getHeading(double x1, double y1, double x2, double y2){
    double dx=x2-x1;
    double dy=y2-y1;
    double tht = atan2(dy, dx);
    return tht;
}




