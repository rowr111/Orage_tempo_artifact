void setup() {
  // put your setup code here, to run once:
}

void loop() {
        usbMIDI.sendNoteOn(60, 50, 1);
        delay(1000);

        usbMIDI.sendNoteOff(60,0,1);
        delay(1000);


}
