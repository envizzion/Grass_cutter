#include<Wire.h>
#include <util/twi.h>

//THE COMPASS IS QMC ,NOT HMC
 
byte lowByte,highByte,address,gyro_address;
boolean set_gyro_angles= 0 ;
int compass_address = 0x0D ;
unsigned long timer=0;
unsigned long loop_timer;
double gyro_pitch, gyro_roll, gyro_yaw;
double gyro_axis_cal[4];
float gyro_roll_cal, gyro_pitch_cal, gyro_yaw_cal;
int cal_int=0;
float roll_level_adjust, pitch_level_adjust,yaw_level_adjust;
int acc_axis[4], gyro_axis[4];
float angle_roll_acc, angle_pitch_acc,angle_yaw_acc, angle_pitch, angle_roll,angle_yaw;
long acc_x, acc_y, acc_z, acc_total_vector;
boolean gyro_angles_set;
int temperature;
float pid_i_mem_roll, pid_roll_setpoint, gyro_roll_input, pid_output_roll, pid_last_roll_d_error;
float pid_i_mem_pitch, pid_pitch_setpoint, gyro_pitch_input, pid_output_pitch, pid_last_pitch_d_error;
float pid_i_mem_yaw, pid_yaw_setpoint, gyro_yaw_input, pid_output_yaw, pid_last_yaw_d_error;
float angle_pitch_output, angle_roll_output,angle_yaw_output;
void setupMpu(){

   Wire.beginTransmission(address);                             //Start communication with the gyro
    Wire.write(0x6B);                                            //PWR_MGMT_1 register
    Wire.write(0x00);                                            //Set to zero to turn on the gyro
    Wire.endTransmission();                                      //End the transmission
    
    Wire.beginTransmission(address);                             //Start communication with the gyro
    Wire.write(0x6B);                                            //Start reading @ register 28h and auto increment with every read
    Wire.endTransmission();                                      //End the transmission
    Wire.requestFrom(address, 1);                                //Request 1 bytes from the gyro
    while(Wire.available() < 1);                                 //Wait until the 1 byte is received
    Serial.print(F("Register 0x6B is set to:"));
    Serial.println(Wire.read(),BIN);
    
//    Wire.beginTransmission(address);                             //Start communication with the gyro
//    Wire.write(0x1B);                                            //GYRO_CONFIG register
//    Wire.write(0x08);                                            //Set the register bits as 00001000 (500dps full scale)
//    Wire.endTransmission();                                      //End the transmission
//    
 //Configure the accelerometer (+/-8g)
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x1C);                                                    //Send the requested starting register
  Wire.write(0x10);                                                    //Set the requested starting register
  Wire.endTransmission();                                              //End the transmission
  //Configure the gyro (500dps full scale)
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x1B);                                                    //Send the requested starting register
  Wire.write(0x08);                                                    //Set the requested starting register
  Wire.endTransmission();                                              //End the transmission


    Wire.beginTransmission(address);                             //Start communication with the gyro (adress 1101001)
    Wire.write(0x1B);                                            //Start reading @ register 28h and auto increment with every read
    Wire.endTransmission();                                      //End the transmission
    Wire.requestFrom(address, 1);                                //Request 1 bytes from the gyro
    while(Wire.available() < 1);                                 //Wait until the 1 byte is received
    Serial.print(F("Register 0x1B is set to:"));
    Serial.println(Wire.read(),BIN);


      
  }












byte search_gyro(int gyro_address, int who_am_i){
  Wire.beginTransmission(gyro_address);
  Wire.write(who_am_i);
  Wire.endTransmission();
  Wire.requestFrom(gyro_address, 1);
  timer = millis() + 100;
  while(Wire.available() < 1 && timer > millis());
  lowByte = Wire.read();
  address = gyro_address;
  return lowByte;
}



void findMpu(){
  Serial.println(F(""));
    Serial.println(F("==================================================="));
    Serial.println(F("Gyro search"));
    Serial.println(F("==================================================="));
    delay(2000);
    
    Serial.println(F("Searching for MPU-6050 on address 0x68/104"));
    delay(1000);
    if(search_gyro(0x68, 0x75) == 0x68){
      Serial.println(F("MPU-6050 found on address 0x68"));
      
      gyro_address = 0x68;
      address = 0x68;
      return;
    }
    

      Serial.println(F("Searching for MPU-6050 on address 0x69/105"));
      delay(1000);
      if(search_gyro(0x69, 0x75) == 0x68){
        Serial.println(F("MPU-6050 found on address 0x69"));
        gyro_address = 0x69;
             address = 0x69;
      }
    
}  

void calliberateGyro(){
  Serial.println("setup ok");
 //Let's take multiple gyro data samples so we can determine the average gyro offset (calibration).
  for (cal_int = 0; cal_int < 2000 ; cal_int ++){                           //Take 2000 readings for calibration.
 //   if(cal_int % 15 == 0)digitalWrite(12, !digitalRead(12));                //Change the led status to indicate calibration.
    readGyro();                                                      //Read the gyro output.
    gyro_axis_cal[1] += gyro_axis[1];                                       //Ad roll value to gyro_roll_cal.
    gyro_axis_cal[2] += gyro_axis[2];                                       //Ad pitch value to gyro_pitch_cal.
    gyro_axis_cal[3] += gyro_axis[3];                                       //Ad yaw value to gyro_yaw_cal.
                                        
  }
  //Now that we have 2000 measures, we need to devide by 2000 to get the average gyro offset.
  gyro_axis_cal[1] /= 2000;                                                 //Divide the roll total by 2000.
  gyro_axis_cal[2] /= 2000;                                                 //Divide the pitch total by 2000.
  gyro_axis_cal[3] /= 2000;                                  //Divide the yaw total by 2000.
    

 }

 void showMPUCallibResults(){
   Serial.println(F(""));
    Serial.print(F("Axis 1 offset="));
    Serial.println( gyro_axis_cal[1]);
    Serial.print(F("Axis 2 offset="));
    Serial.println( gyro_axis_cal[2]);
    Serial.print(F("Axis 3 offset="));
    Serial.println(gyro_axis_cal[3]);
    Serial.println(F(""));
  }


void readGyro(){
 Wire.beginTransmission(gyro_address);                                   //Start communication with the gyro.
    Wire.write(0x3B);                                                       //Start reading @ register 43h and auto increment with every read.
    Wire.endTransmission();                                                 //End the transmission.
    Wire.requestFrom(gyro_address,14);     
  while(Wire.available() < 14);                                           //Wait until the 14 bytes are received.
    acc_axis[1] = Wire.read()<<8|Wire.read();                               //Add the low and high byte to the acc_x variable.
    acc_axis[2] = Wire.read()<<8|Wire.read();                               //Add the low and high byte to the acc_y variable.
    acc_axis[3] = Wire.read()<<8|Wire.read();                               //Add the low and high byte to the acc_z variable.
    temperature = Wire.read()<<8|Wire.read();                               //Add the low and high byte to the temperature variable.
    gyro_axis[1] = Wire.read()<<8|Wire.read();                              //Read high and low part of the angular data.
    gyro_axis[2] = Wire.read()<<8|Wire.read();                              //Read high and low part of the angular data.
    gyro_axis[3] = Wire.read()<<8|Wire.read();                              //Read high and low part of the angular data.

  
                      //Set acc_z to the correct axis that was stored in the EEPROM.


 

}




void calAngles(){
  byte trigger_axis = 0;
 

 
     
  

 gyro_axis[1] -= gyro_axis_cal[1];                                       //Only compensate after the calibration.
 gyro_axis[2] -= gyro_axis_cal[2];                                       //Only compensate after the calibration.
 gyro_axis[3] -= gyro_axis_cal[3];                                       //Only compensate after the calibration.
  

gyro_roll = gyro_axis[2];                     
gyro_pitch = gyro_axis[1];                     //Set gyro_pitch to the correct axis that was stored in the EEPROM.
gyro_yaw = gyro_axis[3];                       //Set gyro_yaw to the correct axis that was stored in the EEPROM.

acc_x = acc_axis[2];                           //Set acc_x to the correct axis that was stored in the EEPROM.
acc_y = acc_axis[1];                           //Set acc_y to the correct axis that was stored in the EEPROM.
acc_z = acc_axis[3];      



    //Gyro angle calculations
  //0.0000611 = 1 / (250Hz / 65.5)
  angle_pitch += gyro_pitch * 0.0000611;                                    //Calculate the traveled pitch angle and add this to the angle_pitch variable.
   angle_roll += gyro_roll * 0.0000611;                                      //Calculate the traveled roll angle and add this to the angle_roll variable.
  angle_yaw += gyro_yaw* 0.0000611;
  
  //0.000001066 = 0.0000611 * (3.142(PI) / 180degr) The Arduino sin function is in radians
 // angle_pitch -= angle_roll * sin(gyro_yaw * 0.000001066);                  //If the IMU has yawed transfer the roll angle to the pitch angel.
 // angle_roll += angle_pitch * sin(gyro_yaw * 0.000001066);                  //If the IMU has yawed transfer the pitch angle to the roll angel.

  //Accelerometer angle calculations
  acc_total_vector = sqrt((acc_x*acc_x)+(acc_y*acc_y)+(acc_z*acc_z));       //Calculate the total accelerometer vector.
  
  if(abs(acc_y) < acc_total_vector){                                        //Prevent the asin function to produce a NaN
    angle_pitch_acc = asin((float)acc_y/acc_total_vector)* -57.296;          //Calculate the pitch angle.
  }
  if(abs(acc_x) < acc_total_vector){                                        //Prevent the asin function to produce a NaN
    angle_roll_acc = asin((float)acc_x/acc_total_vector)* -57.296;          //Calculate the roll angle.
  }
   if(abs(acc_z) < acc_total_vector){                                        //Prevent the asin function to produce a NaN
    angle_yaw_acc = asin((float)acc_z/acc_total_vector)* -57.296;          //Calculate the roll angle.
  }


//Serial.print("Pitch cal:");
//Serial.print(angle_pitch_acc);
//Serial.print("  Roll cal:");
//Serial.print(angle_roll_acc);
//Serial.print("  Yaw cal:");
//Serial.println(angle_yaw_acc);
  
  //Place the MPU-6050 spirit level and note the values in the following two lines for calibration.
  angle_pitch_acc -= 5.8;                                                   //Accelerometer calibration value for pitch.
  angle_roll_acc -=4.8;                                                    //Accelerometer calibration value for roll.
  angle_yaw_acc +=82.3;
 
 if(set_gyro_angles){                                                 //If the IMU is already started
    angle_pitch = angle_pitch * 0.9996 + angle_pitch_acc * 0.0004;     //Correct the drift of the gyro pitch angle with the accelerometer pitch angle
    angle_roll = angle_roll * 0.9996 + angle_roll_acc * 0.0004;        //Correct the drift of the gyro roll angle with the accelerometer roll angle
    angle_yaw = angle_yaw * 0.9996 + angle_yaw_acc * 0.0004;

  }
  else{                                                                //At first start
    angle_pitch = angle_pitch_acc;                                     //Set the gyro pitch angle equal to the accelerometer pitch angle 
    angle_roll = angle_roll_acc;                                       //Set the gyro roll angle equal to the accelerometer roll angle 
    angle_yaw = angle_yaw_acc;                                       //Set the gyro roll angle equal to the accelerometer roll angle 

    set_gyro_angles = true;                                            //Set the IMU started flag
  }
   
  //To dampen the pitch and roll angles a complementary filter is used
    angle_pitch_output = angle_pitch_output * 0.9 + angle_pitch * 0.1;   //Take 90% of the output pitch value and add 10% of the raw pitch value
    angle_roll_output = angle_roll_output * 0.9 + angle_roll * 0.1;      //Take 90% of the output roll value and add 10% of the raw roll valu
    angle_yaw_output = angle_yaw_output * 0.9 + angle_yaw * 0.1;

  
   //65.5 = 1 deg/sec (check the datasheet of the MPU-6050 for more information).
      gyro_roll_input = (gyro_roll_input * 0.7) + ((gyro_roll / 65.5) * 0.3);   //Gyro pid input is deg/sec.
      gyro_pitch_input = (gyro_pitch_input * 0.7) + ((gyro_pitch / 65.5) * 0.3);//Gyro pid input is deg/sec.
      gyro_yaw_input = (gyro_yaw_input * 0.7) + ((gyro_yaw / 65.5) * 0.3);      //Gyro pid input is deg/sec.


  }

void printMPUData(){
 Serial.print("Pitch:");
Serial.print(angle_pitch_output);
Serial.print("  Roll:");
Serial.print(angle_roll_output);
Serial.print("  Yaw:");
Serial.println(angle_yaw_output);
  
  }






  

