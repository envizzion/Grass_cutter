void setup() {
 Serial.begin(115200);


//------MotorControl-------------
initMotors();

//-------Two Wire----------------
  Wire.setClock(400000L);
//  Serial.println(TWBR);
   if(TWBR == 12 ){
 Serial.println(F("I2C clock speed is correctly set to 400kHz."));
  }

//-------------MPU------------------
setupMPU();
//findMpu();
 
  //setup_mpu_6050_registers(); //temp commented since mpu got burned
//  calliberate_mpu();  //temp commented since mpu got burned
//----------------------------------



//while(1){searchWire();}


sei();

while(1){
//  encodedTurn(-90);
// moveCM(-10);
moveCMGyro(20);
//gyroTurnNew(-180);
  delay(2000);

}
//testGyro();
//testPIDFollow();
}



