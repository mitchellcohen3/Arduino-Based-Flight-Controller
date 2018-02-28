#include <Adafruit_BMP280.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_GPS.h>
#include <math.h> 
#include <Wire.h>   
#include <Servo.h>
#include <TinyGPS++.h>
#include <SPI.h>
#include <SD.h>
#include <Filters.h>

/*Define all necessary sensors*/

#define BNO055_SAMPLERATE_DELAY_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055();

#define mySerial Serial1
Adafruit_GPS GPS(&mySerial);
#define GPSECHO true

boolean usingInterrupt = true;
void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}

TinyGPSPlus gps;  //Defining GPS for TinyGPS++ Library

Adafruit_BMP280 bmp; // hardware SPI // Defining pressure sensor for cut-down mechanism

struct setpt{
    //define a set point as a structure with x and y coordinates (in meters)
    double x;
    double y;
};

float x_init, y_init;  //define these as the origin, now have a frame of reference for position
float nlo, nla; //current position in long, lat
double num_sats;
double nx, ny;  //current position in cartesian coords
double px=0, py=0; //previous position, set to zero because that will be our prev position in the first heading calculation
float desired_roll, desired_pitch = 0;
int counter = 0;   //counts the current setpt
struct setpt setpoints[4];  //define an array of setpoints to reach, once we have reached the last point, initiate landing

#define CHANNEL_1_PIN 3 //Channel 1 Pin From the Rx
#define CHANNEL_2_PIN 4 //Channel 2 pin from the Rx

volatile unsigned long timer_start; 
volatile int last_interrupt_time; //calcSignal is the interrupt handler 
unsigned long pulse_time;

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
    
    
       while (!Serial) {
     // wait for serial port to connect. Needed only for USB connection
    }
  
  
  
    initialize_all_sensors();
    
    Serial.println("Sensors Initialized");
    //this loop checks to make sure there is a fix, new data is received, and that it is parsed, otherwise it tries again
    
    hold_for_gps_fix();
    
    x_init = (gps.location.lng()); //using TinyGPS++ Library
    y_init = (gps.location.lat()); //using TinyGPS++ Library
    
    Serial.println(x_init, 6);
    Serial.println(y_init, 6);

    
    initialize_setpoints();
    Serial.println("Setpoints initialized");
    attachInterrupt(CHANNEL_1_PIN, calcSignal, CHANGE);
        
    
}
 
void loop() {
    control();
}


