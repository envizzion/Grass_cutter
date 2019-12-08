

void turnAngle(float angle ,char directn){
  
  
  }



void loop() {
  loop_timer = micros();  

//----------MPU----------
 //calculate_mpu();
// print_mpu_data()    ;
//-----------------------


//moveCM(-100);
//printEncoders();
//pidFollow(B);
//Serial.println(random(-180,180));
//gyroTurn(random(180,-180));
//delay(3000);
//setPID(r1,r2);
//Serial.println(kp);
//LEFT;
//
//  long newPositionL = EncL.read();
//  long newPositionR = EncR.read();
//
 // printEncoders();
//Serial.println(analogRead(A1));

//if(randomPattern){
//  
//  while(FoundWire)
//} 
  searchWire();
  
 
  




while(loop_timer+20000 > micros());

}
