// MIDI note values that might be useful.
// C1 = 36, D2 = 50, D#2 = 51, E2 = 52, F2 = 53, G2 = 55, G#2 = 56, A2 = 57, A#2 = 58, B2 = 59


const int timeToSend = 1000; //time between sending the tempo
const int functionallyZeroRPMTime = 5000; //if we have gone 5 seconds since the last debounce we are functionally at zero rpm
const long maxRPM = 100.0;

const int reedPin = 3; // reed switch to digital pin 2, change depending on actual pin used
const int velocity = 10; //I don’t think we care about velocity, but need to figure out const value for this
// using the same const for all notes being sent.  make another const if you want a diff length for magicNote.
// keep this const short otherwise if it’s longer than a revolution of the wheel it won’t send another note
const int lengthofNote = 10;

//this is the min allowed time (in milliseconds) between acceptable inputs from the sensor
unsigned int reedBounceTime = 20;
unsigned long lastDebounce = 0;

//need to have longs because of silly arduino math
float longTempo;
float longoldTempo;
int tempo;

unsigned long rpm = 0;
unsigned int interruptCount = 0;
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
    // get the running average.  adjust the 
    longTempo = (((rpm * 127) / maxRPM) * 0.1) + (longoldTempo * 0.9);
    tempo = longTempo;

    sendTimeOld = millis();
    Serial.print("Tempo: ");
    Serial.println(tempo);
  }

  //usbMIDI.sendControlChange(tempo, velocity, 1);

  // discard incoming MIDI messages, this is required.
  //while (usbMIDI.read()) { }
}

void rpm_fun()
{
  //Each rotation, this interrupt function is run twice
  if ( (millis() - lastDebounce) > reedBounceTime)
  {
    //if (interruptCount == 1)
    //{
    rpm = 30000 / (millis() - timeold);
    // Serial.println(rpm);
    timeold = millis();
    lastDebounce = millis();
    interruptCount = 0;
    //}
    //else
    // {
    // interruptCount++;
    // }
  }
}
