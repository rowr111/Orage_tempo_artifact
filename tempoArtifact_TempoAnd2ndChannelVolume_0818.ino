// MIDI note values that might be useful.
// C1 = 36, D2 = 50, D#2 = 51, E2 = 52, F2 = 53, G2 = 55, G#2 = 56, A2 = 57, A#2 = 58, B2 = 59

// CONSTANTS YOU CAN CHANGE TO AJUST THINGS ///////////////////////////////////
const int timeToSend = 1000; //time between sending the tempo/volume commands to Ableton
const int functionallyZeroRPMTime = 5000; //if we have gone 5 seconds since the last detection we are functionally at zero rpm
const int maxRPM = 100; //decrease this to make tempo change more quickly, since that will decrease the range that maps to tempo values of 0-127.
const float runningAverageCount = 5; //smoothing constant. increase to make the tempo less reactive to rpm changes, decrease to make more reactive.
const float start2ndChannelVolume = 64; //0-127, at what tempo to start the second channel volume. 64 is halfway mark.
/////////////////////////////////////////////////////////////////////////////

const int reedPin = 3; // reed switch to digital pin 3
const int velocity = 10; //this is arbitrarily chosen by me and can be changed if needed

//this is the min allowed time (in milliseconds) between acceptable inputs from the sensor
unsigned int reedBounceTime = 20;
unsigned long lastDebounce = 0;

//need to have longs because of silly arduino math
float longTempo;
float longoldTempo;
float longVolume;
//also made some ints to map values back into, bc not sure if usbMIDI will freak out with floats
int tempo;
int Volume;

unsigned long rpm = 0;
unsigned long timeold;
unsigned long sendTimeOld;

void setup()
{
  //setup the reed switch input mapping to the pins on teensy here
  pinMode(reedPin, INPUT);
  attachInterrupt(reedPin, rpm_fun, RISING);
  rpm = 0;
  tempo = 0;
  timeold = 0;
  sendTimeOld = 0;
  Serial.begin(9600);
}

void loop()
{

  if ((millis() - sendTimeOld) > timeToSend)
  {

    longoldTempo = longTempo;
    // how do we detect if it has stoppped spinning?
    if ((millis() - lastDebounce) > functionallyZeroRPMTime)
    { rpm = 0;
    }
    // also if someone goes faster than the max, set rpm to max
    // otherwise it will send a greater tempo than allowed
    if (rpm > maxRPM)
    { rpm = maxRPM;
    }
    // get the running average.
    longTempo = (((rpm * 127) / maxRPM) * (1 / runningAverageCount)) + (longoldTempo * (1 - (1 / runningAverageCount)));
    //since int truncates and doesn't round, add 0.5 here so it will be correctly 'rounded' when truncated.
    Serial.print("LongTempo: ");
    Serial.println(longTempo);

    Serial.print("LongTempoRounded: ");
    Serial.println(longTempo);

    //temporarly add 0.5 for rounding purposes
    longTempo = longTempo + 0.5;
    //and assign it to the int
    tempo = longTempo;
    //now remove the 0.5 otherwise it will mess up averages and go over 127
    longTempo = longTempo - 0.5;
    
    Serial.print("Tempo: ");
    Serial.println(tempo);

    //usbMIDI.sendControlChange(tempo, velocity, 1);

    //start the volume effect at the specified volume.
    if (tempo >= start2ndChannelVolume)
    {
      //the volume is from 0-127 and is spread evenly over the range of start2ndChannelVolume to 127
      longVolume = ((tempo - start2ndChannelVolume) * (128 / (128 - start2ndChannelVolume))) + 1;
      Volume = longVolume;
      Serial.print("Volume2: ");
      Serial.println(Volume);
      //usbMIDI.sendControlChange(Volume, velocity, 2);
    }

    sendTimeOld = millis();
  }

  // discard incoming MIDI messages, this is required.
  //while (usbMIDI.read()) { }
}

void rpm_fun()
{
  if ( (millis() - lastDebounce) > reedBounceTime)
  {
    rpm = 30000 / (millis() - timeold);
    // Serial.println(rpm);
    timeold = millis();
    lastDebounce = millis();
  }
}
