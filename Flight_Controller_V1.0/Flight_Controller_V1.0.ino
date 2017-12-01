#include <Adafruit_BMP280.h>
#include <SPI.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_GPS.h>
#include <math.h> 
#include <Wire.h>   
#include <Servo.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <SPI.h>

/*Define all necessary sensors*/

#define BNO055_SAMPLERATE_DELAY_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055();

SoftwareSerial mySerial(3, 2);
Adafruit_GPS GPS(&mySerial);
#define GPSECHO true;

boolean usingInterrupt = false;
void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy

TinyGPSPlus gps;  //Defining GPS for TinyGPS++ Library

Adafruit_BMP280 bmp; // Defining pressure sensor for cut-down mechanism

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

float x_init, y_init;  //define these as the origin, now have a frame of reference for position
double nlo, nla; //current position in long, lat
double num_sats;
double nx, ny;  //current position in cartesian coords
double px=0, py=0; //previous position, set to zero because that will be our prev position in the first heading calculation
double desired_roll = 0;
int counter = 0;   //counts the current setpt
struct setpt setpoints[4];  //define an array of setpoints to reach, once we have reached the last point, initiate landing


void initialize_setpoints(){
    setpoints[0].x = -400;   //setpts are stored in meters
    setpoints[0].y = 500;   //placeholder until we come up with an actual coordinate scheme
    setpoints[1].x = 0;   
    setpoints[1].y = 0;
    setpoints[2].x = 20;   
    setpoints[2].y = 20;
    setpoints[3].x = 0;   
    setpoints[3].y = 0;
}    

void setup() {
    Serial.begin(9600);
    initialize_all_sensors();
    Serial.println("Sensors Initialized");
    //this loop checks to make sure there is a fix, new data is received, and that it is parsed, otherwise it tries again
    
    hold_for_gps_fix();
    
    /*while(1){
        if (!GPS.fix) continue;
        char c = GPS.read();
        if (GPS.newNMEAreceived()) {
            if (!GPS.parse(GPS.lastNMEA())){
                continue;
            }
            break;
        }    
    }
    */
    
    x_init = (gps.location.lng()); //using TinyGPS++ Library
    y_init = (gps.location.lat()); //using TinyGPS++ Library
    Serial.println(x_init, 6);
    Serial.println(y_init, 6);

    
    initialize_setpoints();
    Serial.println("Setpoints initialized");
    //Cut_Down();  insert the procedure to cut down the plane
    //stabilize();
    
}


void loop() {
    //read_write_data();
    control();
}

