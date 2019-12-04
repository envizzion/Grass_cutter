 #include <Encoder.h>
long oldPositionL  = 0;
long oldPositionR  =0;
long newPositionL  = 0;
long newPositionR  = 0;

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability

// Right M- 1 round takes 1110 encoder counts
// Left M - 1 round takes 1039 encoder counts


Encoder encL(3, 5);
Encoder encR(2, 6);

void printEncoders(){
  newPositionL = encL.read();
  newPositionR = encR.read();
  if (newPositionL != oldPositionL ||newPositionR != oldPositionR  ) {
    oldPositionL = newPositionL;
    oldPositionR = newPositionR;
    Serial.print(newPositionL);
    Serial.print(" ");
    Serial.println(newPositionR);
 }
  }
long readEncL(){
    return encL.read()*1110/1039 ;
  }
long readEncR(){
    return encR.read() ;
  }  
  
