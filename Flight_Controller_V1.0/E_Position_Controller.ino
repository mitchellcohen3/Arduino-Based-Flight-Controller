//long is x dir, lat is y dir
//Get dist from (lo1, la1) to (lo2, la2)


double distTo(double la1, double lo1, double la2, double lo2){
  double p = 3.141592654;
  double radius = 6367302 + 40; //earth's radius in meters in montreal, 40 is the height above sea level at mcgill 
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

//from l1 -> l2
double getdx (double lo1, double lo2){
  //distTo returns an abs value, return negative if the l1>l2
  if (lo1>lo2) return (-1*(distTo(0, lo1, 0, lo2)));
  return (distTo(0, lo1, 0, lo2));
}

double getdy (double la1, double la2){
  if (la1>la2) return (-1*(distTo(la1, 0, la2, 0)));
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
    double r = tht-heading; //do  this to ensure counter clockwise is +ve
    return r;
}


//take in the x and y coords of the current and previous points to approximate the heading angle
double getHeading(double x1, double y1, double x2, double y2){
    double dx=x2-x1;
    double dy=y2-y1;
    double tht = atan2(dy, dx);
    return tht;
}
//return 1 if sp is reached, advance to next point
//takes in the current position and setpt
int spReached(struct setpt sp, double lon, double lat){
  //get the difference in lat/long of the setpt from the origin 
  //make sure the dla, dlo values correspond w the negatives if any problems arise in debugging
  double dla = mtolat(sp.y);
  double la = y0 + dla;
  double dlo = mtolong(sp.x, la);
  double lo = x0 + dlo;
  double dist = distTo(la, lo, lat, lon);
  if(dist<5.0) return 1;
  return 0;
}  

void position_control(){
  nlo = (gps.location.lng()); //Using TinyGPS++ library
  nla = (gps.location.lat()); //Using TinyGPS++ library
  //nlo = GPS.longitude;
  //nla = GPS.latitude;
  if (spReached(setpoints[counter], nlo, nla)){
    counter++;
  }
  nx = getdx(x0, nlo);
  ny = getdy(y0, nla);
  double head = getHeading(px, py, nx, ny);
  double t= angle(head, nx, ny, setpoints[counter].x, setpoints[counter].y);
  if(t>20){
    desired_roll = MAX_ROLL_LIMIT;
  }else if(t<-20){
    desired_roll = (-1*MAX_ROLL_LIMIT);
  }else{
    desired_roll = (t/20*MAX_ROLL_LIMIT);
  }  
  //these store the previous x/y values
  px=nx;
  py=ny;
}



