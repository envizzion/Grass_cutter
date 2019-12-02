unsigned long times;
//-----pid--------------------
int errArr[]={0,0,0,0,0};
int error=0,lastErr=0,errCount=0;
float kp=0.1 ,kd=0,ki=0;

//------------------------------
//---------------gyro-----------
boolean is_calliberated=false;


//------------------------------

//-------motorControl------//
int rbSpeed=400,rMax=500;
int lbSpeed=400,lMax=500;
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




void pidFollow(){
 
  
  mSpeed= kp*error + kd*(error-lastErr);
  lastErr=error;
  
  lpwm=rbSpeed -mSpeed; 
  rpwm=lbSpeed+mSpeed;


 
  if (rpwm > rMax) rpwm = rMax; // prevent the motor from going beyond max speed
  if (lpwm > lMax ) lpwm = lMax; // prevent the motor from going beyond max speed
  if (rpwm < 0) rpwm = 0; // keep the motor speed positive
  if (lpwm < 0) lpwm = 0; // keep the motor speed positive

 
  setPWM(lpwm,rpwm);
  setMotorDir('F');


}

void gyroTurn(float angle){
        
      setPWM(0,0); 
      setMotorDir('S');
      delay(1000);
      calliberate_mpu();
      
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

        Serial.println(getAngle('z'));
        loop_timer = micros();  
        calculate_mpu();
        while(loop_timer+20000 > micros());
      }
       setMotorDir('S');
      
        encL.write(0);
        encR.write(0);
        lastErr=0 ;
        resetGyro();
  }
