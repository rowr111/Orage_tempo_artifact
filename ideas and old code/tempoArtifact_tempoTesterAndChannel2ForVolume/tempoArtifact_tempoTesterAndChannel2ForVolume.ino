void setup() {
}

void loop() {

  for (int i = 0; i <= 127; i++)
  {
    usbMIDI.sendControlChange(i, 10, 1);

    if (i > 63)
    {
      usbMIDI.sendControlChange(((i-64)*2)+1, 10, 2);
    }
    delay(1000);
  }

  for (int i = 127; i >= 0; i--)
  {
    usbMIDI.sendControlChange(i, 10, 1);
    if (i > 63)
    {
      usbMIDI.sendControlChange(((i-64)* 2)+1, 10, 2);
    }
    if (i == 63)
    {
      usbMIDI.sendControlChange(0, 10, 2);
    }
    
    delay(1000);
  }

  //discard any incoming midi messages, this is required.
  while (usbMIDI.read()) { }

}

