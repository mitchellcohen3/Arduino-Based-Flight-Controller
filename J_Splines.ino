
// Use spline function after the first setpoint has been passed only


    double cP0_x = 0.0; //INPUTS: big picture setpoints to reach
    double cP0_y = 0.0;
    double cP1_x = 0.0;
    double cP1_y = 0.0;
    double cP2_x = 0.0;
    double cP2_y = 0.0;
    double cP3_x = 0.0;
    double cP3_y = 0.0;

    double px_t = 0.0; //OUTPUTS: point on the spline the glider should be headed to
    double py_t = 0.0;

    double t; // Positional weight from 0.0 to 1.0
    double tt;
    double ttt;
    double t_fromx; 
    double t_fromy;

    double gps_x;// temporary until getting the actual GPS reading
    double gps_y;
    
void CatmullRomSpline(){

    // Get control points from big picture setpoints --> will pass through all points (feature of Catmull-Rom spline)
    
    cP0_x = setpoints[0].x;
    cP0_y = setpoints[0].y;
    cP1_x = setpoints[1].x;
    cP1_y = setpoints[1].y;
    cP2_x = setpoints[2].x;
    cP2_y = setpoints[2].y;
    cP3_x = setpoints[3].x;
    cP3_y = setpoints[3].y;

while (1)
    // Calculate t based on the glider's current location

    // Get reading from gps (using "gps_x & gps_y" for now) **REPLACE SOON
    gps_x = 2; //sample
    gps_y = 3;
    
    // Using largest of x and y progress to determine t (best method to get t?)

    t_fromx = abs((gps_x - cP1_x)/(cP2_x - cP1_x));
    t_fromy = abs((gps_y - cP1_y)/(cP2_y - cP1_y));

    // Check if t values are normalized correctly

    if (!((t_fromx >= 0 && t_fromx <= 1) && (t_fromy >= 0 && t_fromy <= 1))){
      Serial.println("t isn't between 0-1, check it.");
    }

 
    if (t_fromx >= t_fromy){
      t = t_fromx;
    }
    else{
      t = t_fromy;
    }

    // Going to the future point (arbitrarily chosen)

    if (t >= 0.0 && t <= 0.9){
      t = t + 0.1; // 0.1 is arbitrarily chosen as the next point -- needs tuning?
    }

     tt = t*t;
     ttt = t*t*t;
  
     // Catmull-Rom equations (assumes uniform spacing of control points)
     // This equation defines the cubic curve that represents the portion of the total curve between two successive control points. 
     // px_t & py_t are the x & y coordinates of the point the glider should attempt to reach next
     // Equations from https://www.mvps.org/directx/articles/catmull/
     
     px_t = 0.5*( (2*cP1_x) + (-cP0_x + cP2_x)*t + (2*cP0_x - 5*cP1_x + 4*cP2_x - cP3_x)*tt + (-cP0_x + 3*cP1_x - 3*cP2_x + cP3_x)*ttt);
     py_t = 0.5*( (2*cP1_y) + (-cP0_y + cP2_y)*t + (2*cP0_y - 5*cP1_y + 4*cP2_y - cP3_y)*tt + (-cP0_y + 3*cP1_y - 3*cP2_y + cP3_y)*ttt);
     // OUTPUT px_t & py_t to other function --> match variable with rest of code


    // Only one coordinate reached is enough? --> CONFIRM WITH FORBES
    if (gps_x >= cP2_x || gps_y >= cP2_y){  // Find a way to consider directionality
        
        cP0_x = cP1_x;
        cP0_y = cP1_y;
        cP1_x = cP2_x;
        cP1_y = cP2_y;
        cP2_x = cP3_x;
        cP2_y = cP3_y;
        cP3_x = cP0_x;
        cP3_y = cP0_y;
    }

 



}

    


