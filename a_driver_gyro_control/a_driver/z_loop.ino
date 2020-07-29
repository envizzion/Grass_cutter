
unsigned long main_loop_timer = 0 ; 
boolean randomPattern = false;

boolean squarePattern = true;
boolean isFacingNorth = true; //to track moving direction
int currWidth = 0;
int maxWidth = 3 ;

boolean rechargePattern = false;
boolean foundDock = false; 
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
//----------RANDOM PATTERN-------------------
while(randomPattern){
  main_loop_timer = micros();  

   if(lowVoltage){
    
       randomPattern = false ;
       squarePattern = false;
       rechargePattern = true;
    }
   
   else if(!foundWire){
      pidFollow(F);
   }
   else{
    moveCM(-10);
   // gyroTurn(getRandomAngle());
   encodedTurn(getRandomAngle());
    delay(1000);
    foundWire = false;
    
    }
    searchWire();
    readBattery();
   //  Serial.println(foundWire);
while(main_loop_timer+20000 > micros());
} 
//-------------------------------------------------

//--------------SQUARE PATTERN---------------------
while(squarePattern){
  main_loop_timer = micros();  

   if(lowVoltage){
    
       randomPattern = false ;
       squarePattern = false;
       rechargePattern = true;
    }
   
   else if(!foundWire){
//      piFollow(F);
      pidFollowGyro(F);
   }
   else{
//    moveCM(-10);
      breakMower();
      moveCMGyro(-5);

   // gyroTurn(getRandomAngle());

    currWidth++;
    if(currWidth > maxWidth){   // when maxWidth is reached do 180 turn without inverting facingNorth to invert the pattern
        currWidth=0;
//        encodedTurn(180);
        gyroTurnNew(180);
//        delay(1000);

        }
   else if(isFacingNorth){   //start with facing north
//      encodedTurn(90);  
        gyroTurnNew(90);
//      delay(600); // we have braking now no need to wait more than 600ms
//      moveCM(10);
        moveCMGyro(10);
//      delay(600);
//      encodedTurn(90);
        gyroTurnNew(90);
      isFacingNorth = false;
    }
    else{
//      encodedTurn(-90);  
        gyroTurnNew(-90);
//      delay(600);
//      moveCM(10);
        moveCMGyro(10);
//      delay(600);
//      encodedTurn(-90);
        gyroTurnNew(-90);
        isFacingNorth = true;
      }  
//      delay(600);
      
 
    
      foundWire = false;
      resetGyroNew();
     }
    searchWire();
//    readBattery();
   //  Serial.println(foundWire);
while(main_loop_timer+4000 > micros());
} 

//------------------------------------------------------


//----------------RECHARGE PATTERN----------------------

if(rechargePattern){

 encodedTurn(180);
 delay(1000);
 moveCM(-10); // to get the rotation axis close to the wire .to byypass the edge case of being perpendicular and infront of the wire
 delay(1000);
 allignWithGuidewire();//allign right inductor with the guide wire
  delay(1000);

while(rechargePattern){
  main_loop_timer = micros();  
  
   if(!foundDock){
      pidFollowWire(F);
   }
   else{
//    moveCM(-10);
   // gyroTurn(getRandomAngle());
//   encodedTurn(getRandomAngle());
    stopMower();
    delay(1000);
//    foundWire = false;
    
    }
//    searchDock();
searchGWire();
   //  Serial.println(foundWire);
while(main_loop_timer+20000 > micros());
}
}
//-------------------------------------------------


// delay(2000);
 //gyroTurn(getRandomAngle());
//  gyroTurn(150);
  //Serial.println(micros()-main_loop_timer);
//delay(3000);
//Serial.println(getRandomAngle());
//delay(2000);

//while(loop_timer+20000 > micros());

}
