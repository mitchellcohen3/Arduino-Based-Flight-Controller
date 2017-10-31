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

Servo elevator_servo;
Servo aileron_servo;

void setup() {
    Serial.begin(9600);
    Serial.print("Test");
    initialize_all_sensors();
    delay(10000);
}

void loop() {
    read_write_data();
    control();
}

