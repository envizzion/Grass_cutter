int gyro_address = 0x68;                                     //MPU-6050 I2C address (0x68 or 0x69)
int acc_calibration_value = -8080;                            //Enter the accelerometer calibration value

//Various settings
float pid_p_gain = 15;                                       //Gain setting for the P-controller (15)
float pid_i_gain = 1.5;                                      //Gain setting for the I-controller (1.5)
float pid_d_gain = 30;                                       //Gain setting for the D-controller (30)
float turning_speed = 30;                                    //Turning speed (20)
float max_target_speed = 150;                                //Max target speed (100)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Declaring global variables
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
byte start=0, received_byte, low_bat;

int left_motor, throttle_left_motor, throttle_counter_left_motor, throttle_left_motor_memory;
int right_motor, throttle_right_motor, throttle_counter_right_motor, throttle_right_motor_memory;
int battery_voltage;
int receive_counter;
int gyro_pitch_data_raw, gyro_yaw_data_raw, accelerometer_data_raw;

long gyro_yaw_calibration_value, gyro_pitch_calibration_value;

unsigned long loop_timer1;

float angle_gyro, angle_acc, angle2, self_balance_pid_setpoint;
float pid_error_temp, pid_i_mem, pid_setpoint, gyro_input, pid_output, pid_last_d_error;
float pid_output_left, pid_output_right;


void setupMPU(){
  
    //By default the MPU-6050 sleeps. So we have to wake it up.
  Wire.beginTransmission(gyro_address);                                     //Start communication with the address found during search.
  Wire.write(0x6B);                                                         //We want to write to the PWR_MGMT_1 register (6B hex)
  Wire.write(0x00);                                                         //Set the register bits as 00000000 to activate the gyro
  Wire.endTransmission();                                                   //End the transmission with the gyro.
  //Set the full scale of the gyro to +/- 250 degrees per second
  Wire.beginTransmission(gyro_address);                                     //Start communication with the address found during search.
  Wire.write(0x1B);                                                         //We want to write to the GYRO_CONFIG register (1B hex)
  Wire.write(0x00);                                                         //Set the register bits as 00000000 (250dps full scale)
  Wire.endTransmission();                                                   //End the transmission with the gyro
  //Set the full scale of the accelerometer to +/- 4g.
  Wire.beginTransmission(gyro_address);                                     //Start communication with the address found during search.
  Wire.write(0x1C);                                                         //We want to write to the ACCEL_CONFIG register (1A hex)
  Wire.write(0x08);                                                         //Set the register bits as 00001000 (+/- 4g full scale range)
  Wire.endTransmission();                                                   //End the transmission with the gyro
  //Set some filtering to improve the raw data.
  Wire.beginTransmission(gyro_address);                                     //Start communication with the address found during search
  Wire.write(0x1A);                                                         //We want to write to the CONFIG register (1A hex)
  Wire.write(0x03);                                                         //Set the register bits as 00000011 (Set Digital Low Pass Filter to ~43Hz)
  Wire.endTransmission();                                                   //End the transmission with the gyro 
callibMPU();
  }

void callibMPU(){
    for(receive_counter = 0; receive_counter < 100; receive_counter++){       //Create 500 loops
//    if(receive_counter % 15 == 0)digitalWrite(13, !digitalRead(13));        //Change the state of the LED every 15 loops to make the LED blink fast
    Wire.beginTransmission(gyro_address);                                   //Start communication with the gyro
    Wire.write(0x45);                                                       //Start reading the Who_am_I register 75h
    Wire.endTransmission();                                                 //End the transmission
    Wire.requestFrom(gyro_address, 4);                                      //Request 2 bytes from the gyro
   gyro_pitch_calibration_value  += Wire.read()<<8|Wire.read();               //Combine the two bytes to make one integer
   gyro_yaw_calibration_value  += Wire.read()<<8|Wire.read();             //Combine the two bytes to make one integer
    delayMicroseconds(3700);                                                //Wait for 3700 microseconds to simulate the main program loop time
  }
  gyro_pitch_calibration_value /= 100;                                      //Divide the total value by 500 to get the avarage gyro offset
  gyro_yaw_calibration_value /= 100;                                        //Divide the total value by 500 to get the avarage gyro offset

  loop_timer1 = micros() + 4000;                                             //Set the loop_timer variable at the next end loop time

}



float gyroGetAngle(){
//  Wire.beginTransmission(gyro_address);                                     //Start communication with the gyro
//  Wire.write(0x3F);                                                         //Start reading at register 3F
//  Wire.endTransmission();                                                   //End the transmission
//  Wire.requestFrom(gyro_address, 2);                                        //Request 2 bytes from the gyro
//  accelerometer_data_raw = Wire.read()<<8|Wire.read();                      //Combine the two bytes to make one integer
////  Serial.println(accelerometer_data_raw);
//  accelerometer_data_raw += acc_calibration_value;                          //Add the accelerometer calibration value
//  if(accelerometer_data_raw > 8200)accelerometer_data_raw = 8200;           //Prevent division by zero by limiting the acc data to +/-8200;
//  if(accelerometer_data_raw < -8200)accelerometer_data_raw = -8200;         //Prevent division by zero by limiting the acc data to +/-8200;
//
//  angle_acc = asin((float)accelerometer_data_raw/8200.0)* 57.296;           //Calculate the current angle according to the accelerometer
//
//  if(start == 0 && angle_acc > -0.5&& angle_acc < 0.5){                     //If the accelerometer angle is almost 0
//    angle_gyro = angle_acc;                                                 //Load the accelerometer angle in the angle_gyro variable
//    start = 1;                                                              //Set the start variable to start the PID controller
//  }
  
  
   Wire.beginTransmission(gyro_address);                                     //Start communication with the gyro
//  Wire.write(0x43);                                                         //Start reading at register 43
  Wire.write(0x45);                                                         //Start reading at register 43

  Wire.endTransmission();                                                   //End the transmission
  Wire.requestFrom(gyro_address, 4);                                        //Request 4 bytes from the gyro
   gyro_pitch_data_raw= Wire.read()<<8|Wire.read();                           //Combine the two bytes to make one integer
  gyro_yaw_data_raw = Wire.read()<<8|Wire.read();                         //Combine the two bytes to make one integer
  
  gyro_pitch_data_raw -= gyro_pitch_calibration_value;                      //Add the gyro calibration value
  gyro_yaw_data_raw -= gyro_yaw_calibration_value;

//  angle_gyro += gyro_pitch_data_raw * 0.000031;                             //Calculate the traveled during this loop angle and add this to the angle_gyro variable
  angle_gyro += gyro_yaw_data_raw * 0.000031;
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //MPU-6050 offset compensation
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //Not every gyro is mounted 100% level with the axis of the robot. This can be cause by misalignments during manufacturing of the breakout board. 
  //As a result the robot will not rotate at the exact same spot and start to make larger and larger circles.
  //To compensate for this behavior a VERY SMALL angle compensation is needed when the robot is rotating.
  //Try 0.0000003 or -0.0000003 first to see if there is any improvement.

//  gyro_yaw_data_raw -= gyro_yaw_calibration_value;                          //Add the gyro calibration value
  //Uncomment the following line to make the compensation active
  //angle_gyro -= gyro_yaw_data_raw * 0.0000003;                            //Compensate the gyro offset when the robot is rotating

//  angle_gyro = angle_gyro * 0.9996 + angle_acc * 0.0004;                    //Correct the drift of the gyro angle with the accelerometer angle
  return angle_gyro;
  }


void gyroPID(){

  
 

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //PID controller calculations
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //The balancing robot is angle driven. First the difference between the desired angel (setpoint) and actual angle (process value)
  //is calculated. The self_balance_pid_setpoint variable is automatically changed to make sure that the robot stays balanced all the time.
  //The (pid_setpoint - pid_output * 0.015) part functions as a brake function.
  pid_error_temp = angle_gyro  - pid_setpoint;
  if(pid_output > 10 || pid_output < -10)pid_error_temp += pid_output * 0.015 ;

  pid_i_mem += pid_i_gain * pid_error_temp;                                 //Calculate the I-controller value and add it to the pid_i_mem variable
  if(pid_i_mem > 400)pid_i_mem = 400;                                       //Limit the I-controller to the maximum controller output
  else if(pid_i_mem < -400)pid_i_mem = -400;
  //Calculate the PID output value
  pid_output = pid_p_gain * pid_error_temp + pid_i_mem + pid_d_gain * (pid_error_temp - pid_last_d_error);
  if(pid_output > 400)pid_output = 400;                                     //Limit the PI-controller to the maximum controller output
  else if(pid_output < -400)pid_output = -400;

  pid_last_d_error = pid_error_temp;                                        //Store the error for the next loop

  if(pid_output < 5 && pid_output > -5)pid_output = 0;                      //Create a dead-band to stop the motors when the robot is balanced

  if(angle_gyro > 30 || angle_gyro < -30 || start == 0 || low_bat == 1){    //If the robot tips over or the start variable is zero or the battery is empty
    pid_output = 0;                                                         //Set the PID controller output to 0 so the motors stop moving
    pid_i_mem = 0;                                                          //Reset the I-controller memory
    start = 0;                                                              //Set the start variable to 0
    self_balance_pid_setpoint = 0;                                          //Reset the self_balance_pid_setpoint variable
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //Control calculations
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  pid_output_left = pid_output;                                             //Copy the controller output to the pid_output_left variable for the left motor
  pid_output_right = pid_output;            
  
  
  
  
  
  
  }


  void testGyro(){
            Serial.println("1");

    setupMPU();
                Serial.println("2");

    angle_gyro = 0;
    while(1){

    gyroGetAngle();
    Serial.println(angle_gyro);

    
    while(loop_timer1 > micros());
    loop_timer1 =micros()+ 4000;
    }
    
    
    
    
    
    
    }
  
 void resetMPU(){
  angle_gyro = 0 ;
  gyro_yaw_calibration_value = 0 ;
  delay(1000); // wait for settling before calliberation 
  callibMPU();

  
  
  } 
  
 
