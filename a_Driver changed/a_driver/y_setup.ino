void setup() {
 Serial.begin(115200);


//------MotorControl-------------
initMotors();

//-------Two Wire----------------
  Wire.setClock(400000L);
//  Serial.println(TWBR);
   if(TWBR == 12 ){
//    Serial.println(F("I2C clock speed is correctly set to 400kHz."));
  }

//-------------MPU------------------

//findMpu();
  setup_mpu_6050_registers();
//  calliberate_mpu();
//----------------------------------


sei();
}


