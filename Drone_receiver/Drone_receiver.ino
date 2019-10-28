#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
Servo esc;
int throttlePin = 7;
const uint64_t pipeIn = 0xE8E8F0F0E1LL; //Remember that this code is the same as in the transmitter
int x;
RF24 radio(9, 10); 
unsigned long times;
unsigned long zero_timer,t1,t2,t3,t4,loop_timer,main_timer;
 int receiver_input[4];
//We could use up to 32 channels
struct MyData {
uint16_t throttle; //We define each byte of data input, in this case just 6 channels
uint16_t yaw;
uint16_t pitch;
uint16_t roll;
uint16_t AUX1;
uint16_t AUX2;

};

MyData data;

void resetData()
{
//We define the inicial value of each data input
//3 potenciometers will be in the middle position so 127 is the middle from 254
data.throttle = 512;
data.yaw = 000;
data.pitch = 512;
data.roll = 512;
data.AUX1 = 000;
data.AUX2 = 000;

}

/**************************************************/

void setup()
{
DDRD |= B11110000;

Serial.begin(9600); //Set the speed to 9600 bauds if you want.
//You should always have the same speed selected in the serial monitor
resetData();


main_timer=micros();
}

/**************************************************/

unsigned long lastRecvTime = 0,recvTime=0;

void recvData()
{
recvTime=millis();  
while( radio.available() && recvTime+100>millis()) {
radio.read(&data, sizeof(MyData));
}



lastRecvTime = millis(); //here we receive the data

}

/**************************************************/
void wait(unsigned long i){
 

 // if (i > 16380) {
 // times=millis()+i/1000;
 // while(millis()<times){
    
 //   }
  //}
 // else{
   times=micros()+i;
      while(micros()<times){
    
    }
    
   // }
}
void loop()
{
recvData();

unsigned long now = millis();
if ( now - lastRecvTime > x ) {



x=now-lastRecvTime;
}



//Here we check if we've lost signal, if we did we reset the values 
if ( now - lastRecvTime > 1000 ) {
// Signal lost?
//resetData();


}
receiver_input[0]=data.throttle+1000;
receiver_input[1]=data.yaw+1000;
receiver_input[2]=data.pitch+1000;
receiver_input[3]=data.roll+1000;

Serial.print("Throttle: "); Serial.print(receiver_input[0]);  Serial.print("    ");
Serial.print("Yaw: ");      Serial.print(receiver_input[1]);       Serial.print("    ");
Serial.print("Pitch: ");    Serial.print(receiver_input[2]);     Serial.print("    ");
Serial.print("Roll: ");     Serial.print(receiver_input[3]);      Serial.print("    ");
Serial.print("Aux1: ");     Serial.print(data.AUX1+1000);      Serial.print("    ");
Serial.print("Aux2: ");     Serial.print(data.AUX2+1000);      Serial.print("    ");       Serial.print(x);   Serial.print(" \n");
//x=0;




//if(main_timer +4000<= micros()){
 main_timer=millis();
 zero_timer = micros();                                     //Reset the zero timer.
  PORTD |= B11110000;                                        //Set port 4, 5, 6 and 7 high at once
  t1 = data.throttle + zero_timer+1000;   //Calculate the time when digital port 4 is set low
  t2 = data.yaw+ zero_timer+1000;   //Calculate the time when digital port 5 is set low
  t3 = data.pitch+ zero_timer+1000;   //Calculate the time when digital port 6 is set low
  t4 = data.roll+ zero_timer+1000;   //Calculate the time when digital port 7 is set low
  
  while(PORTD >= 16){                                        //Execute the loop until digital port 4 to 7 is low
    loop_timer = micros();                               //Check the current time
    if(t1 <= loop_timer)PORTD &= B11101111; //When the delay time is expired, digital port 4 is set low
    if(t2 <= loop_timer)PORTD &= B11011111; //When the delay time is expired, digital port 5 is set low
    if(t3 <= loop_timer)PORTD &= B10111111; //When the delay time is expired, digital port 6 is set low
    if(t4 <= loop_timer)PORTD &= B01111111; //When the delay time is expired, digital port 7 is set low
  }

//}
  
//esc.writeMicroseconds(data.throttle+1000);
}

/**************************************************/
