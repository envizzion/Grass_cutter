unsigned long times;
//-----pid--------------------
int errArr[]={0,0,0,0,0};
int error=0,lastErr=0,errCount=0;
float kp=3.45 ,kd=83,ki=0;

//---------------------------



//-------motorControl------//
int rbSpeed=240,rMax=255;
int lbSpeed=240,lMax=255;
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
  
  lpwm=rbSpeed +mSpeed; 
  rpwm=lbSpeed-mSpeed;


 
  if (rpwm > rMax) rpwm = rMax; // prevent the motor from going beyond max speed
  if (lpwm > lMax ) lpwm = lMax; // prevent the motor from going beyond max speed
  if (rpwm < 0) rpwm = 0; // keep the motor speed positive
  if (lpwm < 0) lpwm = 0; // keep the motor speed positive
  
  //FORWD;
 OCR2A = rpwm;
 OCR1B =lpwm;

  IN1_ON;
IN2_OFF;
IN3_ON;
IN4_OFF;
wait(10000);
}
