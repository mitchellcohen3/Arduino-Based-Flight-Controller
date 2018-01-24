

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


    for (int t=0.0; t<=1.0 && t>=0.0; t=t+0.1){
      
         tt = t*t;
         ttt = t*t*t;
    
         // Catmull-Rom equations (assumes uniform spacing of control points)
         // This equation defines the cubic curve that represents the portion of the total curve between two successive control points. 
         // px_t & py_t are the x & y coordinates of the point the glider should attempt to reach next
         // Equations from https://www.mvps.org/directx/articles/catmull/
         
         px_t = 0.5*( (2*cP1_x) + (-cP0_x + cP2_x)*t + (2*cP0_x - 5*cP1_x + 4*cP2_x - cP3_x)*tt + (-cP0_x + 3*cP1_x - 3*cP2_x + cP3_x)*ttt);
         py_t = 0.5*( (2*cP1_y) + (-cP0_y + cP2_y)*t + (2*cP0_y - 5*cP1_y + 4*cP2_y - cP3_y)*tt + (-cP0_y + 3*cP1_y - 3*cP2_y + cP3_y)*ttt);
     }

}

    
