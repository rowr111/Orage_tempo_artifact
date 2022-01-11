const int reedPin = 2; // reed switch to digital pin 2, change depending on actual pin used

const int velocity = 10; //I don’t think we care about velocity, but need to figure out const value for this
const int magicrpm1 = 100; //the bpm that will trigger an effect

//using the same const for all notes being sent.  make another const if you want a diff length for magicNote.
// keep this const short otherwise if it’s longer than a revolution of the wheel it won’t send another note
const int lengthofNote = 10;

unsigned int reedBounceTime = 100;
unsigned long lastDebounce = 0;

unsigned magicNote1ON = 0;
int magicNote1;
int tempo = 0;

 volatile byte half_revolutions;
 unsigned int rpmold;
 unsigned int rpm;
 unsigned long timeold;
 unsigned long magicNote1StartTime;

void setup()
{
  //setup the reed switch input mapping to the pins on teensy here
  pinMode(reedPin, INPUT);
  //start = millis(); // get the initial start time
  attachInterrupt(0, rpm_fun, RISING);
   half_revolutions = 0;
   rpm = 0;
   timeold = 0;
}

void loop()
{
   if (half_revolutions >= 20) 
   {
     //Update RPM every 20 counts, increase this for better RPM resolution,
     //decrease for faster update
     rpmold = rpm;
     rpm = 30*1000/(millis() - timeold)*half_revolutions;
     timeold = millis();
     half_revolutions = 0;
   }

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

  if (rpm > magicrpm1)
  {
    usbMIDI.sendNoteOn(magicNote1, velocity, 1);
    magicNote1ON = 1;
  }

  //if the magic note is on longer than lengthofNote, turn it off and record that they’re off
  if ((magicNote1ON == 1) && ((millis() - magicNote1StartTime) > lengthofNote))
  {
    usbMIDI.sendNoteOff(magicNote1, velocity, 1);
    magicNote1ON = 0;
  }

  // discard incoming MIDI messages, this is required.
  while (usbMIDI.read()) { }
}

 void rpm_fun()
 {
     //Each rotation, this interrupt function is run twice
    if( (millis() - lastDebounce) > reedBounceTime)
    {
      half_revolutions++;
      lastDebounce = millis();
    }
  }


 
