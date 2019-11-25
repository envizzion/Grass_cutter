#include <avr/io.h>
#include <avr/interrupt.h>

//========VARIABLES===============================

unsigned long times;
//-----pid------
int errArr[]={0,0,0,0,0};
int error=0,lastErr=0,errCount=0;
float kp=3.45 ,kd=83,ki=0;

//--------------



//-------motorControl------//
int rbSpeed=240,rMax=255;
int lbSpeed=240,lMax=255;
int mSpeed=0;
int rpwm,lpwm;
//--motor control--//

//RIGHT MOTOR
#define EN1  DDRB |= (1<<2);
#define EN1_ON TCCR1A |= (1<<COM1B1);


#define IN2  DDRB |= (1<<5);
#define IN2_ON  PORTB |= (1<<5);
#define IN2_OFF  DDRB &=~(1<<5);

#define IN1  DDRB |= (1<<4);
#define IN1_ON  PORTB |= (1<<4);
#define IN1_OFF  PORTB &=~(1<<4);

//LEFT MOTOR
#define IN3  DDRD |= (1<<7);
#define IN3_ON  PORTD |= (1<<7);
#define IN3_OFF  PORTD &=~(1<<7);

#define IN4 DDRB |= (1<<0);
#define IN4_ON  PORTB |= (1<<0);
#define IN4_OFF  PORTB &=~(1<<0);


#define EN2  DDRB |= (1<<1);
#define EN2_ON TCCR1A |= (1<<COM1A1);

#define All_OFF ;IN1_OFF;IN2_OFF;IN3_OFF;IN4_OFF;

#define FORWD ; IN1_ON; IN2_OFF; IN3_ON; IN4_OFF;

#define REVS ;IN1_OFF;IN2_ON;IN3_OFF;IN4_ON;

#define LEFT;IN2_OFF;IN1_ON; IN4_ON; IN3_OFF;

#define RIGHT ; IN2_ON; IN1_OFF;IN4_OFF;IN3_ON;

//-------------------------//

//==============================================================


//----------------===============METHODS============-------------------


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

//----------MotorControl-------------------------
void initMotors(){
  IN1;IN2;IN3;IN4;EN1;EN2;

TCCR1A=0x00;
TCCR1B=0x00;





TCCR1A |=(1<<COM1B1|1<<COM1A1|WGM11); //phase correct 9 bit pwm ~15 khz
//TCCR1A |=(1<<COM1B1|1<<COM1A1|WGM10); //phase correct 9 bit pwm ~30 khz
TCCR1B |= (1<<CS10);  


//OCR2A =rpwm;
//OCR1B =lpwm;

 }

 void setPWM(int lSpeed , int rSpeed){
    OCR1B =rSpeed;
    OCR1A =lSpeed;
  }

//-----------------------------------------------

//-------------------PID-------------------------

void pidFollow(){

//Serial.println(error);


  

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

//-----------------------------------------------



//----------------==================================-------------------


void setup() {
 Serial.begin(9600);


//------MotorControl-------
initMotors();

//-----------------------



sei();
}

void loop() {
  // put your main code here, to run repeatedly:
//pidFollow();
//setPID(r1,r2);
//Serial.println(kp);
LEFT;
setPWM(300,300);
}
