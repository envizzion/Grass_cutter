int compass_address = 0x0D ;



void setupCompass(){
    Wire.beginTransmission(compass_address);                             //Start communication with the compass
    Wire.write(0x0B); // Tell the QMC5883 to Continuously Measure
Wire.write(0x01); // Set the Register
   Wire.endTransmission(); 

    Wire.beginTransmission(compass_address); //start talking
  Wire.write(0x09); // Tell the QMC5883 to Continuously Measure
  Wire.write(0x1D); // Set the Register
Wire.endTransmission();
  }


void readCompass(){
   int x,y,z; //triple axis data


  
  //Tell the HMC what regist to begin writing data into


  Wire.beginTransmission(compass_address);
  Wire.write(0x00); //start with register 3.
Wire.endTransmission();
 //Read the data.. 2 bytes for each axis.. 6 total bytes
  Wire.requestFrom(compass_address, 6);
  if(6<=Wire.available()){
    x = Wire.read()<<8; //MSB  x 
    x |= Wire.read(); //LSB  x
    z = Wire.read()<<8; //MSB  z
    z |= Wire.read(); //LSB z
    y = Wire.read()<<8; //MSB y
    y |= Wire.read(); //LSB y
  }
  

  
  Serial.print("X Value: ");
  Serial.println(x);
  Serial.print("Y Value: ");
  Serial.println(y);
  Serial.print("Z Value: ");
  Serial.println(z);
  Serial.println();
  
  
  }
