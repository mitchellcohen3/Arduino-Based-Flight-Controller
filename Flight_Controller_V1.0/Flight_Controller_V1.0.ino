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

    /*Define Servo pins*/

const int SERVO_PIN_ELEVATOR = 9;
const int SERVO_PIN_AILERONS = 10;
const int SERVO_PIN_RUDDER = 11;

void setup() {
    initialize_all_sensors();
}

void loop() {
    control();
}
