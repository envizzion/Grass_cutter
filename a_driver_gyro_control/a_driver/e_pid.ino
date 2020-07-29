unsigned long times;
//-----pid--------------------
int errWArr[] = {0, 0, 0, 0, 0};
byte errWCount = 0;
float error = 0, lastErr = 0, errCount = 0;
float kp = 0.1 , kd = 0.01, ki = 0;

float kpW = 0.1 , kdW = 0, kiW = 0;
float errorW = 0,lastErrW = 0;
byte F = 200;
byte B = 201 ;
//------------------------------
//---------------gyro-----------
boolean is_calliberated = false;
float angle;

//------------------------------

//-------motorControl------//
int rbSpeed = 330,rbASpeed = 300 , rMax = 400,rAMax = 400;
int lbSpeed = 330,lbASpeed = 300 ,lMax = 400,lAMax = 400;
int mSpeed = 0;
int rpwm, lpwm;

//-------------------------




void wait(unsigned long i) {
  // if (i > 16380) {
  // times=millis()+i/1000;
  // while(millis()<times){

  //   }
  //}
  // else{
  times = micros() + i;
  while (micros() < times) {
    // Serial.println(micros());
  }
}




void pidFollow(byte dir) {
  error = encR.read() - encL.read() ;


  mSpeed = kp * error + kd * (error - lastErr);
  lastErr = error;

  lpwm = rbSpeed - mSpeed;
  rpwm = lbSpeed + mSpeed;



  if (rpwm > rMax) rpwm = rMax; // prevent the motor from going beyond max speed
  if (lpwm > lMax ) lpwm = lMax; // prevent the motor from going beyond max speed
  if (rpwm < 0) rpwm = 0; // keep the motor speed positive
  if (lpwm < 0) lpwm = 0; // keep the motor speed positive

  if (dir == F) {
    setPWM(lpwm, rpwm);
    setMotorDir('F');


  }

  else if (dir == B) {
    setPWM(rpwm, lpwm);
    setMotorDir('B');
  }
}

void pidFollowWire(byte dir) {
  error = encR.read() - encL.read() ;
//  errorW = analogReadFast(A1) ;
  errWArr[errWCount] = analogReadFast(A1) ;
  errWCount ++;

  if(errWCount >=5){
    errWCount = 0 ;
    }
  errorW = 0 ;
  for(int i = 0 ; i< 5 ;i++){
      errorW +=errWArr[i];
}

     errorW = errorW/5;
     
//        Serial.println(errorW);


  
  mSpeed = kp * error + kd * (error - lastErr) + kpW*errorW;

Serial.println(mSpeed);
  
  lastErr = error;
  lastErrW = errorW;
  lpwm = rbSpeed - mSpeed;
  rpwm = lbSpeed + mSpeed;



  if (rpwm > rMax) rpwm = rMax; // prevent the motor from going beyond max speed
  if (lpwm > lMax ) lpwm = lMax; // prevent the motor from going beyond max speed
  if (rpwm < 0) rpwm = 0; // keep the motor speed positive
  if (lpwm < 0) lpwm = 0; // keep the motor speed positive

  if (dir == F) {
    setPWM(lpwm, rpwm);
    setMotorDir('F');


  }

  else if (dir == B) {
    setPWM(rpwm, lpwm);
    setMotorDir('B');
  }
}

void gyroTurn(float angle) {
  resetGyro(SOFT);
  setPWM(0, 0);
  setMotorDir('S');
  delay(1000);
  // calliberate_mpu();

  if (angle < 0) {
    setMotorDir('L');
    angle *= -1;
  }
  else {
    setMotorDir('R');
  }
  calculate_mpu();
  setPWM(400, 400);
  while (abs(getAngle('z')) < angle ) {

    //Serial.println(getAngle('z'));
    Serial.println(angle);
    loop_timer = micros();
    calculate_mpu();
    while (loop_timer + 20000 > micros());
  }
  setMotorDir('S');

  encL.write(0);
  encR.write(0);
  resetPID() ;

}

// 1 wheel has a diameter of 6.7cm >> circumf >> 3.14 x 6.7 = 21.038cm
// counts for 1 cm = 1118/21.038 = ~53 counts

void moveCM(long distance) {
  distance *= -53; //-ve is because encoders reduce value when going forward
  resetEncoders();
  resetPID() ;
  if (distance < 0) {

    while (readEncL() >= distance && readEncR() >= distance ) {
      loop_timer = micros();
      pidFollow(F);
      while (loop_timer + 20000 > micros());
      //    Serial.println(encL.read());
    }
  }
  else {
    while (readEncL() <= distance && readEncR() <= distance) {
      loop_timer = micros();
      pidFollow(B);
      while (loop_timer + 20000 > micros());
      //          Serial.println(encL.read());

    }
  }
  resetPID() ;
  setMotorDir('S');
  Serial.println(readEncL());
  Serial.println(readEncR());
  
}


void stopMower(){
    resetPID() ;
  setMotorDir('S');
  }

void resetPID() {
  lastErr = 0 ;
  error = 0 ;

}

//theres a +30 and -30 degree offset from zero when making the random turn to avoid inefficient cutting movements at the same place.
float getRandomAngle() {
  float ang = 30 * random(-6, 6); //to get random values in minimum of 30 degree steps

  //   if(ang<0 && ang>-30){
  //    ang-=30;
  //   }
  //   else if(ang>=0 && ang<30){
  //    ang+=30;
  //   }
  return ang;
}

//distance between 2 wheels is 18 cm , gives circumference = 3.14x18 = 56.5 cm , that gives an encoder count of 56.5 * 53(53 counts for 1 cm) = 2996 for one roatation
// so for 1 degree no of counts is  = 2996/360 = ~8 counts
void encodedTurn(int angle) {
  angle -= angle/8;
  
  resetEncoders();
  setPWM(0, 0);
  setMotorDir('S');
  resetPID() ;
  if (angle < 0) {
    setMotorDir('L');
    angle *= -1;
  }
  else {
    setMotorDir('R');
  }

  angle *= 8 ; //turn angle to encoder counts

  setPWM(350, 350);
  while (abs(encL.read()) < angle ); //only reading one encoder is enough for now.
  setPWM(0, 0);
  setMotorDir('S');
  resetPID();
  resetEncoders();

}

void allignWithGuidewire() {
  resetEncoders();
  setPWM(0, 0);
  setMotorDir('S');
  resetPID() ;


  setMotorDir('R');



  setPWM(350, 350);
  while (!foundGWire )searchGWire(); //only reading one encoder is enough for now.
  setPWM(0, 0);
  setMotorDir('S');
  resetPID();
  resetEncoders();
}

float lastErrA ,errorA,kpA=1,kdA=0.3 , kiA = 0.005 ,  iErrorA = 0;


void gyroTurnNew(float angle) {
  setPWM(0, 0);
  setMotorDir('S');
  delay(1000);
  // calliberate_mpu();

  if (angle < 0) {
    setMotorDir('L');  
  }
  else {
    setMotorDir('R');
  }
  resetMPU();
  lastErrA = 0 ;
  iErrorA = 0;
  setPWM(370, 370);
while (abs(angle_gyro) < abs(angle) - 5 ) {
    gyroGetAngle();
//    pidRotate(angle); 
    //Serial.println(getAngle('z'));
    Serial.print(angle_gyro);Serial.print(" ");Serial.println(mSpeed);
    while(loop_timer1 > micros());
    loop_timer1 =micros()+ 4000;
  }

  //for breaking
  if (angle < 0) {
    setMotorDir('R');
    
  }
  else {
    setMotorDir('L');
  }
  delay(150);
  setMotorDir('S');

  encL.write(0);
  encR.write(0);
  resetPID() ;

}


void pidRotate(float err) {

  errorA = abs(err) - abs(angle_gyro) ; 
  
  iErrorA += kiA*errorA;                                 //Calculate the I-controller value and add it to the pid_i_mem variable
  if(iErrorA > rMax )iErrorA = rMax;                                       //Limit the I-controller to the maximum controller output


  mSpeed = kpA*errorA + kdA*(errorA - lastErrA) + iErrorA;
  lastErrA = mSpeed;

  lpwm = rbASpeed + mSpeed;
  rpwm = rbASpeed + mSpeed;



  

  if (rpwm > rAMax) rpwm = rAMax; // prevent the motor from going beyond max speed
  if (lpwm > lAMax ) lpwm = lAMax; // prevent the motor from going beyond max speed
  if (rpwm < 0) rpwm = 0; // keep the motor speed positive
  if (lpwm < 0) lpwm = 0; // keep the motor speed positive


    setPWM(rpwm, lpwm);

}

float kpAF = 3 , kdAF = 8 ;

void pidFollowGyro(byte dir) {
  errorA =  gyroGetAngle();



  mSpeed = kpAF * errorA + kdAF * (errorA - lastErrA);
  lastErrA = errorA;

  lpwm = rbSpeed + mSpeed;
  rpwm = lbSpeed - mSpeed;



  if (rpwm > rMax) rpwm = rMax; // prevent the motor from going beyond max speed
  if (lpwm > lMax ) lpwm = lMax; // prevent the motor from going beyond max speed
  if (rpwm < 0) rpwm = 0; // keep the motor speed positive
  if (lpwm < 0) lpwm = 0; // keep the motor speed positive

  if (dir == F) {
    setPWM(lpwm, rpwm);
    setMotorDir('F');


  }

  else if (dir == B) {
    setPWM(rpwm, lpwm);
    setMotorDir('B');
  }
}

void testPIDFollow(){
  
  resetMPU();
  lastErrA = 0 ;
  iErrorA = 0;
  while (1) {
    pidFollowGyro(F);
    Serial.print(angle_gyro);Serial.print(" ");Serial.println(mSpeed);
    while(loop_timer1 > micros());
    loop_timer1 =micros()+ 4000;
  }

  
  }

void moveCMGyro(long distance) {
  distance *= -53; //-ve is because encoders reduce value when going forward
  resetEncoders();
  resetPID() ;
  resetMPU();
  lastErrA = 0 ;
  iErrorA = 0;
  if (distance < 0) {

    while ((readEncL() + readEncR())/2 >= distance  ) {
      loop_timer = micros();
      pidFollowGyro(F);
      while(loop_timer1 > micros());
      loop_timer1 =micros()+ 4000;
    }
      setMotorDir('B'); //for breaking

  }
  else {
    while ((readEncL() + readEncR())/2 <= distance) {
      loop_timer = micros();
      pidFollowGyro(B);
      while(loop_timer1 > micros());
      loop_timer1 =micros()+ 4000;

    }
      setMotorDir('F'); //for breaking

  }
  delay(80); //breaking time
  setMotorDir('S');
  Serial.println(readEncL());
  Serial.println(readEncR());
  
}

//u turn for the square pattern not complete yet
void gyroUTurn(char dir){

  resetEncoders();
  resetPID() ;
  resetMPU();
  lastErrA = 0 ;
  iErrorA = 0;

  
  
  
  
  
  }

  void resetGyroNew(){
      resetMPU();
      lastErrA = 0 ;
      iErrorA = 0;
    }

  void  breakMower(){
     setMotorDir('B'); //for breaking
     delay(80);
     setMotorDir('S');
     delay(1000);    
    }
