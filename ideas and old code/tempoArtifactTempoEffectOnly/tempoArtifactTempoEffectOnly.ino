
// MIDI note values that might be useful.
// C1 = 36, D2 = 50, D#2 = 51, E2 = 52, F2 = 53, G2 = 55, G#2 = 56, A2 = 57, A#2 = 58, B2 = 59

const int reedPin = 3; // reed switch to digital pin 2, change depending on actual pin used

const int velocity = 10; //I don’t think we care about velocity, but need to figure out const value for this

// using the same const for all notes being sent.  make another const if you want a diff length for magicNote.
// keep this const short otherwise if it’s longer than a revolution of the wheel it won’t send another note
const int lengthofNote = 10;

//this is the min allowed time between acceptable inputs from the sensor
unsigned int reedBounceTime = 10;
unsigned long lastDebounce = 0;

int tempo = 0;

volatile byte half_revolutions;
unsigned int rpmold;
unsigned int rpm;
unsigned long timeold;

void setup()
{
  //setup the reed switch input mapping to the pins on teensy here
  pinMode(reedPin, INPUT);
  attachInterrupt(reedPin, rpm_fun, RISING);
  half_revolutions = 0;
  rpm = 0;
  timeold = 0;
}

void loop()
{
  if (half_revolutions >= 10)
  {
    //increase this for better RPM resolution,
    //decrease for faster update
    rpmold = rpm;
    rpm = 30 * 1000 / (millis() - timeold) * half_revolutions;
    timeold = millis();
    half_revolutions = 0;

    if (rpmold < rpm)
    { // send midi command to increase tempo
      if (tempo < 127)
      {
        tempo++;
        usbMIDI.sendControlChange(tempo, velocity, 1);
      }
    }
    else if (rpmold > rpm)
    { // send midi command to decrease tempo
      if (tempo > 0)
      {
        tempo--;
        usbMIDI.sendControlChange(tempo, velocity, 1);
      }
    }
  }
  // discard incoming MIDI messages, this is required.
  while (usbMIDI.read()) { }
}

void rpm_fun()
{
  //Each rotation, this interrupt function is run twice
  if ( (millis() - lastDebounce) > reedBounceTime)
  {
    half_revolutions++;
    lastDebounce = millis();
  }
}





