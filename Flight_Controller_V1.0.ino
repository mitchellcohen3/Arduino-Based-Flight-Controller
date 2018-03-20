#include <Servo.h>
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
Adafruit_BNO055 bno = Adafruit_BNO055();

#define mySerial Serial1
Adafruit_GPS GPS(&mySerial);
#define GPSECHO true

boolean usingInterrupt = false;
void useInterrupt(boolean); 

TinyGPSPlus gps;  //Defining GPS for TinyGPS++ Library

Adafruit_BMP280 bmp; // Defining pressure sensor for cut-down mechanism

//Create Servo Objects
Servo elevator_servo;
Servo aileron_servo_left;
Servo aileron_servo_right;
Servo rudder_servo;
Servo cutdown_servo;  

/*
Define global variables needed to read receiver PWM signals
Variables must be global because hardware interrupts are used
*/

#define CHANNEL_1_PIN 3 //Channel 1 Pin From the Rx
#define CHANNEL_2_PIN 4 //Channel 2 pin from the Rx
#define CHANNEL_4_PIN 5 //Channel 2 pin from the Rx
#define CHANNEL_6_PIN 6 //Channel 6 pin from the Rx

volatile unsigned long timer_start_pitch; 
volatile int last_interrupt_time_pitch; 
unsigned long pulse_time_pitch;

volatile unsigned long timer_start_roll; 
volatile int last_interrupt_time_roll; 
unsigned long pulse_time_roll;

volatile unsigned long timer_start_FM; 
volatile int last_interrupt_time_FM; 
unsigned long pulse_time_FM;

volatile unsigned long timer_start_rudder; 
volatile int last_interrupt_time_rudder; 
unsigned long pulse_time_rudder;

int file_num; //Data file number 
int flight_mode;

int num_sats; //Number of satellites, used to determine GPS fix

const float cut_alt = 15; // 200ft = approx 60m
float current_alt;
int cd_ready = 1;

struct setpt{
    //define a set point as a structure with x and y coordinates (in meters)
    double x;
    double y;
};

struct setpt setpoints[4];  //define an array of setpoints to reach, once we have reached the last point, initiate landing

//Function to initialize the position setpoints
void initialize_setpoints(){
    setpoints[0].x = 0;   //setpoints are stored in meters
    setpoints[0].y = -100;   
    setpoints[1].x = 75;      
    setpoints[1].y = -60;
    setpoints[2].x = 150;   
    setpoints[2].y = -100;
    setpoints[3].x = 0;   
    setpoints[3].y = 0;
} 
   
//Define the origin of the coordinate system
float x_init, y_init;  

//Define the variables for attitude setpoints
float desired_roll = 0, desired_pitch = 0;

void setup() {
    Serial.begin(9600);
        
      while (!Serial) {
        ; // wait for serial port to connect. Needed only for USB connection
    }
  
    initialize_all_sensors();
    hold_for_gps_fix();
    
    x_init = (gps.location.lng()); //Set initial X and Y coordinates
    y_init = (gps.location.lat()); 
    
    initialize_setpoints(); //Initialize the setpoint array
    file_num = create_file(); //Create file to store data on SD card

    set_flight_mode("Autopilot"); //Set the flight mode - options are 'FBW' (Fly-by-wire), 'Autopilot', or 'Stabilize'

      if(flight_mode == 1){
          initialize_interrupts(); //initialize interrupts to use for reciever and transmitter. Only used if flight mode is FBW
      }
}
 
void loop() {
//Read and write data each iteration of the loop
    read_data();
    write_data();
    
// Ensure cut-down altitude has been reached and the cut-down mechanism is ready to release
    if((current_alt >= cut_alt) && (cd_ready == 1)){ 
        open_cutdown();
        cd_ready = 0;
        }
        
//After cutdown, control glider
    if(cd_ready == 0){
      
//If channel 6 on the reciever off, fly in manual mode
        if(pulse_time_FM < 1500){
            manual_flight();
        }
        
//If channel 6 is turned past halfway, execute autonomous commands based on the user-entered flight mode          
        else{   
            switch(flight_mode){
                case 1:
                    //In FBW, get desired roll and desired pitch from tramsitter stick, and control UAV
                    desired_pitch = get_desired_pitch();
                    desired_roll = get_desired_roll();
                    control();
                 case 2:
                  //In full autopilot, ensure a GPS fix is obtained and control UAV
                  if(num_sats >= 2){
                      control();
                  }
                  //If fix is lost, switch FM to 'stabilize'
                  else{
                      set_flight_mode("Stabilize");
                      break;
                 }
                  case 3:
                  //In stabilize mode, set desired pitch and desired roll to 0 and control UAV.
                      desired_pitch = 0;
                      desired_roll = 0;
                      control();
            }
    
     
        }
    }
}
