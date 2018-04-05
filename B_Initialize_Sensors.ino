//Boolean variables that indicate whether the IMU and pressure sensor are connected
boolean using_imu = true;
boolean using_pressure = true;

//Define variables needed to store data to the SD Card
String header = "millis, Pressure, Altitude, Yaw Angle, Pitch Angle, Roll Angle, Longatude, Latitude, Elevator Servo, L Aielron Servo, R Aileron Servo, Rudder Servo, Desired Pitch, Desired Roll, Roll Rate, Pitch Rate, Yaw Rate, Flight Mode Status";
String datastring;
String file_prefix = "data"; // File name prefix for datalogging
String event_file_prefix = "log"; // Event log file name
String file_type = ".txt"; // File type
const int chipSelect = BUILTIN_SDCARD;

//Function to initialize all sensors
void initialize_all_sensors(){
    
    initialize_imu();
    initialize_servos();
    initialize_pres();
    initialize_GPS();
    
   // check_cutdown_ready();
}

//Function to initialize the IMU
void initialize_imu(){
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 */
    Serial.println("No IMU");
    using_imu = false;
  }

  bno.setExtCrystalUse(true);
}

//Function to attach all servos, and write the initial positions of all servos
void initialize_servos(){

//Attach all servo objects to correct pins
    
    elevator_servo.attach(SERVO_PIN_ELEVATOR);
    aileron_servo_left.attach(SERVO_PIN_AILERON_LEFT);
    aileron_servo_right.attach(SERVO_PIN_AILERON_RIGHT);
    rudder_servo.attach(SERVO_PIN_RUDDER);
    cutdown_servo.attach(SERVO_PIN_CUTDOWN);  

//Write initial angles to servos
    
    aileron_servo_left.write(AL_MIDDLE_POS);
    aileron_servo_right.write(AR_MIDDLE_POS);
    elevator_servo.write(E_MIDDLE_POS);
    rudder_servo.write(R_MIDDLE_POS);
    cutdown_servo.write(180);
}

//Function to initialize the GPS
void initialize_GPS(){
    // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
    GPS.begin(9600);
    mySerial.begin(9600);
    //turn on RMC (recommended minimum) and GGA (fix data) including altitude
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); 
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);   // 1 Hz update rate
    GPS.sendCommand(PGCMD_ANTENNA);      
    
  #ifdef __arm__
    usingInterrupt = false;  //NOTE - we don't want to use interrupts on the Due
  #else
    useInterrupt(true);
  #endif
}

//Function to initialize the pressure sensor. Baseline altitude is also taken based on first pressure readings.
void initialize_pres(){   
  
    if (!bmp.begin()) {  
    //Could not find a valid BMP280 sensor
    using_pressure = false;
  }

  //Neglect the first 100 pressure readings as the first readings are sometimes anomalies 
  for(int i = 0; i < 100; i++){
        seaLevelhPA = get_sealevel_pressure();
        base_altitude = bmp.readAltitude(seaLevelhPA);
      }
}   

//Function that will stay in a loop until a GPS fix is obtained
void hold_for_gps_fix(){
  float nlo2 = 0;
  float nla2 = 0;
  num_sats = 0;
  
  //Stuck in following loop until a gps fix is obtained.
  while (nla2 == 0 || nlo2 == 0 || num_sats <= 2){
    
   while (Serial1.available())  gps.encode(Serial1.read());
   nla2 = (gps.location.lat());
   nlo2 = (gps.location.lng());
   num_sats = gps.satellites.value();
  }
}

//GPS initialization definitions

#ifdef __AVR__
// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
#ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;  
    // writing direct to UDR0 is much much faster than Serial.print 
    // but only one character can be written at a time. 
#endif
}

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
#endif //#ifdef__AVR__
uint32_t timer = millis();

//Function to create a two files; one data file and one log file with all gains printed for testing purposes.
int create_file(){
  
    if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return 0;
  }
  
    int i = 0;
    String file_name = file_prefix + i + file_type;
    String event_file_name = event_file_prefix + i + file_type;
    char data_file_char_array[file_name.length() + 1];
    char event_file_char_array[event_file_name.length() + 1];
        
  while(i < 1000){
    String file_name = file_prefix + i + file_type;
    String event_file_name = event_file_prefix + i + file_type;

    char data_file_char_array[file_name.length() + 1];
    file_name.toCharArray(data_file_char_array, file_name.length() + 1);
    char event_file_char_array[event_file_name.length() + 1];
    event_file_name.toCharArray(event_file_char_array, event_file_name.length() + 1);
    
    Serial.println(file_name);
    
    if (!SD.exists(data_file_char_array)){
      File dataFile = SD.open(data_file_char_array, FILE_WRITE);
      dataFile.println(header);
      dataFile.close();
  
      File eventFile = SD.open(event_file_char_array, FILE_WRITE);
      
      gains_X = "KP X = ";
      gains_X += KP_X;
      gains_X += " KD X = ";
      gains_X += KD_X;
      gains_X += " KI X = ";
      gains_X += KI_X;

      gains_Y = "KP Y = ";
      gains_Y += KP_Y;
      gains_Y += " KD Y = ";
      gains_Y += KD_Y;
      gains_Y += " KI Y = ";
      gains_Y += KI_Y;

      gains_Z = "KP Z = ";
      gains_Z += KP_Z;
      gains_Z += " KD Z = ";
      gains_Z += KD_Z;
      gains_Z += " KI Z = ";
      gains_Z += KI_Z;

      eventFile.println(gains_X);
      eventFile.println(gains_Y);
      eventFile.println(gains_Z);
      
      eventFile.close(); 
      break;

    }
    else {
      i++;
    }
  }

  return i;
}

//Function to initialize interrupts, used to read PWM signals from the receiver 
void initialize_interrupts(){
    timer_start_pitch = 0; 
    timer_start_roll = 0;
    timer_start_FM = 0; 
    timer_start_rudder = 0 ;
    
    attachInterrupt(CHANNEL_1_PIN, calcSignal_roll, CHANGE);
    attachInterrupt(CHANNEL_2_PIN, calcSignal_pitch, CHANGE);
    attachInterrupt(CHANNEL_6_PIN, calcSignal_FM, CHANGE);
    attachInterrupt(CHANNEL_4_PIN, calcSignal_rudder, CHANGE);
}

void check_cutdown_ready() {
  if ( temp_ready == 1 ){ //CHECK A SERVO POSITION?  --> depends on the mechanism
    cd_ready = 1;
    }
  
  }

//Function to set the flight mode
void set_flight_mode(String _flight_mode){
  
    if(_flight_mode == "FBW"){
        flight_mode = 1;
    }
    else if(_flight_mode == "Autopilot"){
        flight_mode = 2;
    }

    //Set flight mode to stabilize if user enters something stupid
    else {
        flight_mode = 3;
    }
}
 
    






