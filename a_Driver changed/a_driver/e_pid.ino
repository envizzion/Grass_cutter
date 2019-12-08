unsigned long times;
//-----pid--------------------
int errArr[]={0,0,0,0,0};
float error=0,lastErr=0,errCount=0;
float kp=0.1 ,kd=0,ki=0;

byte F = 200;
byte B = 201 ;
//------------------------------
//---------------gyro-----------
boolean is_calliberated=false;
float angle;

//------------------------------

//-------motorControl------//
int rbSpeed=300,rMax=450;
int lbSpeed=350,lMax=450;
int mSpeed=0;
int rpwm,lpwm;

//-------------------------




void wait(unsigned long i){
 // if (i > 16380) {
 // times=millis()+i/1000;
 // while(millis()<times){
    
 //   }
  //}
 // else{
   times=micros()+i;
      while(micros()<times){
   // Serial.println(micros());
    }
}




void pidFollow(byte dir){
 error = encR.read()- encL.read() ;

  
  mSpeed= kp*error + kd*(error-lastErr);
  lastErr=error;
  
  lpwm=rbSpeed -mSpeed; 
  rpwm=lbSpeed+mSpeed;


 
  if (rpwm > rMax) rpwm = rMax; // prevent the motor from going beyond max speed
  if (lpwm > lMax ) lpwm = lMax; // prevent the motor from going beyond max speed
  if (rpwm < 0) rpwm = 0; // keep the motor speed positive
  if (lpwm < 0) lpwm = 0; // keep the motor speed positive

 if(dir==F){
  setPWM(lpwm,rpwm);
  setMotorDir('F');


}

else if(dir==B){
  setPWM(rpwm,lpwm);
  setMotorDir('B');
}
}
void gyroTurn(float angle){
       resetGyro(SOFT);  
      setPWM(0,0); 
      setMotorDir('S');
      delay(1000);
     // calliberate_mpu();
      
      if(angle < 0){
          setMotorDir('L');
          angle*=-1;
        }
        else {
           setMotorDir('R');
          }
      calculate_mpu(); 
      setPWM(400,400);    
      while(abs(getAngle('z'))< angle ){

        //Serial.println(getAngle('z'));
        Serial.println(angle);
        loop_timer = micros();  
        calculate_mpu();
        while(loop_timer+20000 > micros());
      }
       setMotorDir('S');
      
        encL.write(0);
        encR.write(0);
        resetPID() ;
       
  }

// 1 wheel has a diameter of 6.7cm >> circumf >> 3.14 x 6.7 = 21.038cm
// counts for 1 cm = 1118/21.038 = ~53 counts 

  void moveCM(long distance){
     distance*=-53;//-ve is because encoders reduce value when going forward
      resetEncoders();
      resetPID() ;
       if(distance < 0){
        
        while(encR.read() > distance ){
        loop_timer = micros();  
        pidFollow(F);
        while(loop_timer+20000 > micros());
    //    Serial.println(encL.read());
      }
      }
      else{
       while(encR.read() < distance ){
        loop_timer = micros();  
        pidFollow(B);
        while(loop_timer+20000 > micros());
      //          Serial.println(encL.read());

      }
      

        }
        resetPID() ;
       setMotorDir('S');  
      
       

    
    }

  void resetPID(){
    lastErr = 0 ;
    error = 0 ;
    
    }  

//theres a +30 and -30 degree offset from zero when making the random turn to avoid inefficient cutting movements at the same place.
float getRandomAngle(){
   float ang = 30*random(-6,6); //to get random values in minimum of 30 degree steps
         
//   if(ang<0 && ang>-30){
//    ang-=30;
//   }
//   else if(ang>=0 && ang<30){
//    ang+=30;
//   }
   return ang;
  }    
