const int reedPin = 2; // reed switch to digital pin 2, change depending on actual pin used

const int velocity = 10; //I don’t think we care about velocity, but need to figure out const value for this
const int magicNumber = 666; //the bpm that will trigger an effect

//using the same const for all notes being sent.  make another const if you want a diff length for magicNote.
// keep this const short otherwise if it’s longer than a revolution of the wheel it won’t send another note
const int lengthofNote = 10;

int reedPrev = 0; // reed switch’s previous state.  default to off (0)
int reedON;   // reed switch’s current state
int reedBounceTime = 100;


unsigned long start, elapsed; //time measurements
unsigned long noteON = 0; //when we turned on the notes
unsigned magicNoteON = 0;
int BPM, prevBPM, note, note2;

void setup()
{
  //setup the reed switch input mapping to the pins on teensy here
  pinMode(reedPin, INPUT);

  start = millis(); // get the initial start time
}

void loop()
{

  //if the notes are on longer than lengthofNote, turn them off and record that they’re off
  if ((noteON != 0) and ((millis() - noteON) > lengthofNote))
  {
    usbMIDI.sendNoteOff(note, velocity, 1);
    noteON = 0;
  }
  if ((magicNoteON != 0) and ((millis() - magicNoteON) > lengthofNote))
  {
    usbMIDI.sendNoteOff(note, velocity, 1);
    magicNoteON = 0;
  }

  //get reed switch status
  //we use ! here bc it will return 0 if the switch is closed.
  reedON = !read(reedPin);

  if (reedON == 1 and reedPrev == 0)
  {
    //avoiding bounce - do nothing unless we are past the bounce time
    if ((millis() - start) > reedBounceTime)
    {
      reedPrev == 1; //remember that the switch is ON
      elapsed = millis() - start; // find out how long since the last time the switch was on
      start = millis(); // restart the timer

      //calculate BPM
      BPM = 60000 / elapsed; // it truncates ints, I don’t care if we’re off by 1/2 a bpm.

      // set note based on these recommendations
      // https://www.ableton.com/en/help/article/tempo-mapping-buttons/
      if (BPM > prevBPM)
      {
        note = 127;
        usbMIDI.sendNoteOn(note, velocity, 1);
        noteON = millis();
      }
      else if (BPM < prevBPM)
      {
        note = 1;
        usbMIDI.sendNoteOn(note, velocity, 1);
        noteON = millis();
      }
      //don’t send any note if bpm did not change


      if ( BPM > magicNumber)
      {
        //do something here, like.. send another note on channel 2?
        usbMIDI.sendNoteOn(note2, velocity, 2);
        magicNoteON = millis();
      }
    }
  }

  if (reedON == 0 and reedPrev == 1)
  {
    // when the reed switch is turned off again, set the prev value to off.
    reedPrev = 0;
  }

  // discard incoming MIDI messages, this is required.
  while (usbMIDI.read()) { }
}
