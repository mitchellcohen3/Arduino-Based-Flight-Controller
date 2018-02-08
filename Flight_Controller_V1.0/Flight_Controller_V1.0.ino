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

boolean usingInterrupt = false;
void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy

TinyGPSPlus gps;  //Defining GPS for TinyGPS++ Library

Adafruit_BMP280 bmp; // hardware SPI // Defining pressure sensor for cut-down mechanism

/*Create Servo Objects*/
Servo elevator_servo;
Servo aileron_servo_left;
Servo aileron_servo_right;
Servo rudder_servo;
Servo cutdown_servo;  

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
float desired_roll = 0, desired_pitch = 0;
int counter = 0;   //counts the current setpt
struct setpt setpoints[4];  //define an array of setpoints to reach, once we have reached the last point, initiate landing

String header = "millis, Pressure, Altitude, X, Y, Z, Longatude, Latitude, Elevator Servo, L Aielron Servo, R Aileron Servo, Rudder Servo, Desired Pitch, Desired Roll";

int file_num; 
String datastring;
String file_prefix = "data"; // File name prefix for datalogging
String event_file_prefix = "log"; // Event log file name
String file_type = ".txt"; // File type
const int chipSelect = BUILTIN_SDCARD;

#define CHANNEL_1_PIN 3 //Channel 1 Pin From the Rx
#define CHANNEL_2_PIN 4 //Channel 2 pin from the Rx

volatile unsigned long timer_start_pitch; 
volatile int last_interrupt_time_pitch; 
unsigned long pulse_time_pitch;

volatile unsigned long timer_start_roll; 
volatile int last_interrupt_time_roll; 
unsigned long pulse_time_roll;

void initialize_setpoints(){
    setpoints[0].x = -150;   //setpts are stored in meters
    setpoints[0].y = 190;   //placeholder until we come up with an actual coordinate scheme
    setpoints[1].x = 0;      
    setpoints[1].y = 0;
    setpoints[2].x = 0;   
    setpoints[2].y = 0;
    setpoints[3].x = 0;   
    setpoints[3].y = 0;
}    

void setup() {
    Serial.begin(9600);
    
       while (!Serial) {
    ; // wait for serial port to connect. Needed only for USB connection
  }
  
    initialize_all_sensors();
    
    //this loop checks to make sure there is a fix, new data is received, and that it is parsed, otherwise it tries again
    
    //hold_for_gps_fix();
    
    x_init = (gps.location.lng()); //using TinyGPS++ Library
    y_init = (gps.location.lat()); //using TinyGPS++ Library

    //Serial.println(x_init);
    //Serial.println(y_init);
    
    initialize_setpoints();
    file_num = create_file();
    initialize_interrupts(); //initialize interrupts to use for reciever and transmitter
    Serial.println("Ready to go");
    
}
 
void loop() {
    read_data();
    write_data();
    //get_altitude(); // Calling cut-down function

    /*
    while (Serial1.available())  gps.encode(Serial1.read());
    nlo = (gps.location.lng()); //Using TinyGPS++ library
    nla = (gps.location.lat()); //Using TinyGPS++ library
    */
    
   control();
    
}


