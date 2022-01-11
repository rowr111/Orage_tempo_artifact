// MIDI note values that might be useful.
// C1 = 36, D2 = 50, D#2 = 51, E2 = 52, F2 = 53, G2 = 55, G#2 = 56, A2 = 57, A#2 = 58, B2 = 59

const int velocity = 10;

void setup()
{
  //nothing to do here..
}

void loop()
{
  // first send the control change that'll be the general tempo effect
  for (int x = 0; x < 20; x++)
  {
    usbMIDI.sendControlChange(0, velocity, 1);
    delay(1000);
  }
  //now send the control change that'l be the volume secondary effect
  for (int y = 0; y < 20; y++)
  {
    usbMIDI.sendControlChange(0, velocity, 2);
    delay(1000);
  }
  //now lets send the note we want to use for the note effect
  for (int z = 0; z < 20; z++)
  {
    usbMIDI.sendNoteOn(36, velocity, 1);
    delay(500);
    usbMIDI.sendNoteOff(36, velocity, 1);
    delay(500);
  }

  // discard incoming MIDI messages, this is required.
  while (usbMIDI.read()) { }
}






