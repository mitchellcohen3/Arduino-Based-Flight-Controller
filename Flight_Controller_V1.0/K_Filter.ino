


// filters out changes faster that 5 Hz.
float filterFrequency = 5.0;  

// create a one pole (RC) lowpass filter
FilterOnePole lowpassFilter( LOWPASS, filterFrequency );   

while( true ) {
  lowpassFilter.input( analogRead( INPUT_PIN ) );
  // do something else
}
