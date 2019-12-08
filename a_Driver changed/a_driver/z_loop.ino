
unsigned long main_loop_timer = 0 ; 
boolean randomPattern = true;

void loop() {
 // main_loop_timer = micros();  

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
//Serial.println(analogReadFast(A1));

while(randomPattern){
  main_loop_timer = micros();  

   if(!foundWire){
      pidFollow(F);
   }
   else{
    moveCM(-10);
    gyroTurn(getRandomAngle());
    delay(4000);
    foundWire = false;
    
    }
    searchWire();
   //  Serial.println(foundWire);
while(main_loop_timer+20000 > micros());
} 
  
 
 //gyroTurn(getRandomAngle());
//  gyroTurn(150);
  //Serial.println(micros()-main_loop_timer);
//delay(3000);
//Serial.println(getRandomAngle());
//delay(2000);

//while(loop_timer+20000 > micros());

}
