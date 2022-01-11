int x = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {

  for (int i = 0; i <= 127; i++)
  {
    //usbMIDI.sendControlChange(i, 10, 1);
    //Serial.print("tempo: ");
    //Serial.println(i);

    if (i > 63)
    {
      Serial.print("tempo2: ");
      Serial.println((i-64)*2);
      //usbMIDI.sendControlChange(x, 10, 2);
     // if (x > 125) {
       // x = x + 1;
      //}
      //else {
       // x = x + 2;
      //}
    }
    delay(100);
  }

    for (int i = 127; i >= 0; i--)
    {
      //usbMIDI.sendControlChange(i, 10, 1);
      // Serial.print("tempo: ");
      // Serial.println(i);
      if (i > 63)
      {
        Serial.print("tempo2: ");
      Serial.println((i-64)*2);
        //usbMIDI.sendControlChange(x, 10, 2);
       // x = x - 2;
      }
      delay(100);
    }

    //discard any incoming midi messages, this is required.
    //while (usbMIDI.read()) { }

  }

