void setup() {
 Serial.begin(9600);


//------MotorControl-------------
initMotors();

//-------Two Wire----------------
  Wire.setClock(400000L);
  Serial.println(TWBR);
   if(TWBR == 12 ){
    Serial.println(F("I2C clock speed is correctly set to 400kHz."));
  }

//-------------------------------



sei();
}


