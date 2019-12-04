

void turnAngle(float angle ,char directn){
  
  
  }



void loop() {
  loop_timer = micros();  

//----------MPU----------
 //calculate_mpu();
// print_mpu_data()    ;
//-----------------------


error = encL.read() - encR.read();

//pidFollow();

//gyroTurn(80.0);
//delay(3000);
//setPID(r1,r2);
//Serial.println(kp);
//LEFT;
//
//  long newPositionL = EncL.read();
//  long newPositionR = EncR.read();
//
 // printEncoders();
Serial.println(analogRead(A1));

   
while(loop_timer+20000 > micros());

}
