void control(){
    
   if(millis() - gps_previous > gps_period){
        //position_control();
        gps_previous = millis();
        
    }
    
    if(millis() - imu_previous > imu_period){
        attitude_control();
        imu_previous = millis();
    }
   
}

void calcSignal() 
{
    //record the interrupt time so that we can tell if the receiver has a signal from the transmitter 
    last_interrupt_time = micros(); 
    //if the pin has gone HIGH, record the microseconds since the Teensy started up 
    if(digitalRead(CHANNEL_1_PIN) == HIGH) 
    { 
        timer_start = micros();
    } 
    //otherwise, the pin has gone LOW 
    else
    { 
        //only worry about this if the timer has actually started
        if(timer_start != 0)
        { 
            //record the pulse time
            pulse_time = ((volatile int)micros() - timer_start);
            //restart the timer
            timer_start = 0;
        }
    } 
} 


