void loop() {
  // put your main code here, to run repeatedly:
//pidFollow();
//setPID(r1,r2);
//Serial.println(kp);
LEFT;
setPWM(300,300);

  long newPositionL = EncL.read();
  long newPositionR = EncR.read();

  if (newPositionL != oldPositionL ||newPositionR != oldPositionR  ) {
    oldPositionL = newPositionL;
    oldPositionR = newPositionR;
    Serial.print(newPositionL);
    Serial.print(" ");
    Serial.println(newPositionR);
  }

}
