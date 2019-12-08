#include<Wire.h>
#include <MPU6050.h>

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
byte SOFT = 110;
byte HARD = 111;


uint16_t sampleRate = 1000;
byte filterLevel=3;
byte gyroRange=1;
byte accelRange=2;
MPU6050 mpu(sampleRate,filterLevel,gyroRange,accelRange);

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
   Wire.setClock(400000L);
mpu.initialize();
mpu.accelZero();
mpu.gyroZero();
}
unsigned long timer ;
void loop() {
timer = micros();
calculate_mpu();
Serial.println(angle_yaw_output);
while(timer+20000 > micros());
}




void calculate_mpu(){

  mpu.retrieve(); 
  
  gyro_x =  mpu.gX;                                                //Subtract the offset calibration value from the raw gyro_x value
  gyro_y = mpu.gY;                                                //Subtract the offset calibration value from the raw gyro_y value
  gyro_z = mpu.gZ;                                                //Subtract the offset calibration value from the raw gyro_z value

  acc_x = mpu.aX;
   acc_y = mpu.aY;
   acc_z = mpu.aZ;
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
