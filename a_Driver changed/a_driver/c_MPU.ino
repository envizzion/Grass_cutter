//THE COMPASS IS QMC ,NOT HMC


#include <Wire.h>

//Declaring some global variables
int gyro_x, gyro_y, gyro_z;
long acc_x, acc_y, acc_z, acc_total_vector;
int temperature;
long gyro_x_cal, gyro_y_cal, gyro_z_cal;
long loop_timer;
int lcd_loop_counter;
float angle_pitch, angle_roll,angle_yaw;
int angle_pitch_buffer, angle_roll_buffer,angle_yaw_buffer;
boolean set_gyro_angles;
float angle_roll_acc, angle_pitch_acc,angle_yaw_acc;
float angle_pitch_output, angle_roll_output,angle_yaw_output;
unsigned int timeCounter;




void calliberate_mpu(){

  
 // Serial.print("Calibrating gyro");                                       //Print text to screen
  for (int cal_int = 0; cal_int < 500 ; cal_int ++){                  //Run this code 2000 times
    timeCounter = micros();
   // if(cal_int % 125 == 0);//Serial.print(".");                              //Print a dot on the LCD every 125 readings
    read_mpu_6050_data();                                              //Read the raw acc and gyro data from the MPU-6050
    gyro_x_cal += gyro_x;                                              //Add the gyro x-axis offset to the gyro_x_cal variable
    gyro_y_cal += gyro_y;                                              //Add the gyro y-axis offset to the gyro_y_cal variable
    gyro_z_cal += gyro_z;                                              //Add the gyro z-axis offset to the gyro_z_cal variable
    while(timeCounter+20000 > micros());                                                          //Delay 3us to simulate the 250Hz program loop
  }
  gyro_x_cal /= 500;                                                  //Divide the gyro_x_cal variable by 2000 to get the avarage offset
  gyro_y_cal /= 500;                                                  //Divide the gyro_y_cal variable by 2000 to get the avarage offset
  gyro_z_cal /= 500;                                                  //Divide the gyro_z_cal variable by 2000 to get the avarage offset

  
loop_timer = micros();
  }




void calculate_mpu(){

  read_mpu_6050_data(); 
  
  gyro_x -= gyro_x_cal;                                                //Subtract the offset calibration value from the raw gyro_x value
  gyro_y -= gyro_y_cal;                                                //Subtract the offset calibration value from the raw gyro_y value
  gyro_z -= gyro_z_cal;                                                //Subtract the offset calibration value from the raw gyro_z value
  
  //Gyro angle calculations
  //0.0000611 = 1 / (250Hz / 65.5)
  //0.0003053  = 1 / (50Hz / 65.5)
  angle_pitch += gyro_x *  0.0003053;                                   //Calculate the traveled pitch angle and add this to the angle_pitch variable
  angle_roll += gyro_y *  0.0003053;                                    //Calculate the traveled roll angle and add this to the angle_roll variable
  angle_yaw += gyro_z *  0.0003053;                                    //Calculate the traveled roll angle and add this to the angle_roll variable

  //0.000001066 = 0.0000611 * (3.142(PI) / 180degr) The Arduino sin function is in radians
  //0.000005326 =  0.0003053* (3.142(PI) / 180degr)
 //  angle_pitch += angle_roll * sin(gyro_z * 0.000005326);               //If the IMU has yawed transfer the roll angle to the pitch angel
 //  angle_roll -= angle_pitch * sin(gyro_z *0.000005326);               //If the IMU has yawed transfer the pitch angle to the roll angel
  
  //Accelerometer angle calculations
  acc_total_vector = sqrt((acc_x*acc_x)+(acc_y*acc_y)+(acc_z*acc_z));  //Calculate the total accelerometer vector
  //57.296 = 1 / (3.142 / 180) The Arduino asin function is in radians
   if(abs(acc_y) < acc_total_vector){                                        //Prevent the asin function to produce a NaN
    angle_pitch_acc = asin((float)acc_y/acc_total_vector)* 57.296;          //Calculate the pitch angle.
  }
  if(abs(acc_x) < acc_total_vector){                                        //Prevent the asin function to produce a NaN
    angle_roll_acc = asin((float)acc_x/acc_total_vector)* -57.296;          //Calculate the roll angle.
  } //Place the MPU-6050 spirit level and note the values in the following two lines for calibration

  if(abs(acc_z) < acc_total_vector){                                        //Prevent the asin function to produce a NaN
    angle_yaw_acc = asin((float)acc_z/acc_total_vector)* -57.296;          //Calculate the roll angle.
  } //Place the MPU-6050 spirit level and note the values in the following two lines for calibration



 
   angle_pitch_acc -= 5.8;                                                   //Accelerometer calibration value for pitch.
  angle_roll_acc -=4.8;                                                    //Accelerometer calibration value for roll.
  angle_yaw_acc +=87.85;
 //Serial.println(angle_yaw_acc);
 
  if(set_gyro_angles){                                                 //If the IMU is already started
    angle_pitch = angle_pitch * 0.9996 + angle_pitch_acc * 0.0004;     //Correct the drift of the gyro pitch angle with the accelerometer pitch angle
    angle_roll = angle_roll * 0.9996 + angle_roll_acc * 0.0004;        //Correct the drift of the gyro roll angle with the accelerometer roll angle
    angle_yaw = angle_yaw * 0.9996 + angle_yaw_acc * 0.0004;
  }
  else{                                                                //At first start
    angle_pitch = angle_pitch_acc;                                     //Set the gyro pitch angle equal to the accelerometer pitch angle 
    angle_roll = angle_roll_acc;                                       //Set the gyro roll angle equal to the accelerometer roll angle 
   angle_yaw = angle_yaw_acc;
    set_gyro_angles = true;                                            //Set the IMU started flag
  }
  
  //To dampen the pitch and roll angles a complementary filter is used
  angle_pitch_output = angle_pitch_output * 0.9 + angle_pitch * 0.1;   //Take 90% of the output pitch value and add 10% of the raw pitch value
  angle_roll_output = angle_roll_output * 0.9 + angle_roll * 0.1;      //Take 90% of the output roll value and add 10% of the raw roll value
  angle_yaw_output = angle_yaw_output*0.9 + angle_yaw*0.1;
  
  }


void read_mpu_6050_data(){                                             //Subroutine for reading the raw gyro and accelerometer data
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x3B);                                                    //Send the requested starting register
  Wire.endTransmission();                                              //End the transmission
  Wire.requestFrom(0x68,14);                                           //Request 14 bytes from the MPU-6050
  while(Wire.available() < 14);                                        //Wait until all the bytes are received
  acc_x = Wire.read()<<8|Wire.read();                                  //Add the low and high byte to the acc_x variable
  acc_y = Wire.read()<<8|Wire.read();                                  //Add the low and high byte to the acc_y variable
  acc_z = Wire.read()<<8|Wire.read();                                  //Add the low and high byte to the acc_z variable
  temperature = Wire.read()<<8|Wire.read();                            //Add the low and high byte to the temperature variable
  gyro_x = Wire.read()<<8|Wire.read();                                 //Add the low and high byte to the gyro_x variable
  gyro_y = Wire.read()<<8|Wire.read();                                 //Add the low and high byte to the gyro_y variable
  gyro_z= Wire.read()<<8|Wire.read();                                 //Add the low and high byte to the gyro_z variable

}

void print_mpu_data(){                                                      //Subroutine for writing the LCD
//  //To get a 250Hz program loop (4us) it's only possible to write one character per loop
//  //Writing multiple characters is taking to much time
//  if(lcd_loop_counter == 14)lcd_loop_counter = 0;                      //Reset the counter after 14 characters
//  lcd_loop_counter ++;                                                 //Increase the counter
//  if(lcd_loop_counter == 1){
//    angle_pitch_buffer = angle_pitch_output * 10;                      //Buffer the pitch angle because it will change
//  }
//  if(lcd_loop_counter == 2){
//    if(angle_pitch_buffer < 0)Serial.print("-");                          //Print - if value is negative
//    else Serial.print("+");                                               //Print + if value is negative
//    
//  }
//  if(lcd_loop_counter == 3)Serial.print(abs(angle_pitch_buffer)/1000);    //Print first number
//  if(lcd_loop_counter == 4)Serial.print((abs(angle_pitch_buffer)/100)%10);//Print second number
//  if(lcd_loop_counter == 5)Serial.print((abs(angle_pitch_buffer)/10)%10); //Print third number
//  if(lcd_loop_counter == 6)Serial.print(".");                             //Print decimal point
//  if(lcd_loop_counter == 7){
//    Serial.print(abs(angle_pitch_buffer)%10);      //Print decimal number
//    Serial.print("  ");
//  }
//
//  if(lcd_loop_counter == 8){
//    angle_roll_buffer = angle_roll_output * 10;
//  }
//  if(lcd_loop_counter == 9){
//    if(angle_roll_buffer < 0)Serial.print("-");                           //Print - if value is negative
//    else Serial.print("+");                                               //Print + if value is negative
//  }
//  if(lcd_loop_counter == 10)Serial.print(abs(angle_roll_buffer)/1000);    //Print first number
//  if(lcd_loop_counter == 11)Serial.print((abs(angle_roll_buffer)/100)%10);//Print second number
//  if(lcd_loop_counter == 12)Serial.print((abs(angle_roll_buffer)/10)%10); //Print third number
// if(lcd_loop_counter == 13)Serial.print(".");                            //Print decimal point
//  if(lcd_loop_counter == 14)Serial.println(abs(angle_roll_buffer)%10);      //Print decimal number

// Serial.print("Pitch:");
//Serial.println(angle_pitch_output);
//Serial.print("  Roll:");
//Serial.println(angle_roll_output);
//Serial.print("  Yaw:");
//Serial.println(angle_yaw_output);
}


void setup_mpu_6050_registers(){
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050

Wire.write(0x6B);                                                    //Send the requested starting register
  Wire.write(0x00);                                                    //Set the requested starting register
  Wire.endTransmission();                                              //End the transmission
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
}


float getAngle(char c){
    switch(c){
      case 'x': return  angle_pitch_output;
      break;
      case 'y': return  angle_roll_output;
      break;
      case 'z': return  angle_yaw_output;
      }
  }

void resetGyro(){
  gyro_x_cal=0;
  gyro_y_cal=0;
  gyro_z_cal=0;
  gyro_x = 0 ;
  gyro_y = 0;
  gyro_z = 0;
  angle_pitch = 0 ;
  angle_roll = 0;
  angle_yaw = 0;
  }









