#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_GPS.h>
#include <math.h> 
#include <Wire.h>   
#include <Servo.h>
#include <SoftwareSerial.h>

/*Define all necessary sensors*/

#define BNO055_SAMPLERATE_DELAY_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055();

SoftwareSerial mySerial(3, 2);
Adafruit_GPS GPS(&mySerial);
#define GPSECHO true;


/*Create Servo Objects*/
Servo elevator_servo;
Servo aileron_servo_left;
Servo aileron_servo_right;
Servo rudder_servo;

struct setpt{
    //define a set point as a structure with x and y coordinates (in meters)
    double x;
    double y;
};

double x0, y0;  //define these as the origin, now have a frame of reference for position
double nlo, nla; //current position in long, lat
double nx, ny;  //current position in cartesian coords
double px=0, py=0; //previous position, set to zero because that will be our prev position in the first heading calculation
int counter = 0;   //counts the current setpt
struct setpt setpoints[3];  //define an array of setpoints to reach, once we have reached the last point, initiate landing


void initialize_setpoints(){
    setpoints[0].x = x0 + mtolong(20, y0);
    setpoints[0].y = y0 + mtolat(20);   //placeholder until we come up with an actual coordinate scheme
    
}    

void setup() {
    Serial.begin(9600);
    initialize_all_sensors();
    delay(5000);
    
    //this loop checks to make sure there is a fix, new data is received, and that it is parsed, otherwise it tries again
    while(1){
        if (!GPS.fix) continue;
        char c = GPS.read();
        if (GPS.newNMEAreceived()) {
            if (!GPS.parse(GPS.lastNMEA())){
                continue;
            }
            break;
        }    
    }        
    x0 = GPS.longitude;
    y0 = GPS.latitude;
    initialize_setpoints();
    //Cut_Down();  insert the procedure to cut down the plane
    //stabilize();*/
    
}


void loop() {
    read_write_data();
    control();
}

