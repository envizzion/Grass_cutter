/* Encoder Library - Basic Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */

#include <Encoder.h>

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder EncL(3, 5);
Encoder EncR(2, 6);

//   avoid using pins with LEDs attached

void setup() {
  Serial.begin(9600);
  Serial.println("Basic Encoder Test:");
}

long oldPositionL  = -999;
long oldPositionR  = -999;

void loop() {
  long newPositionL = EncL.read();
  long newPositionR = EncR.read();

  if (newPositionL != oldPositionL ||newPositionR != oldPositionR  ) {
    oldPositionL = newPositionL;
    oldPositionR = newPositionR;
    Serial.print(newPositionL);
    Serial.print(" ");
    Serial.println(newPositionR);
  }
 
  
}
