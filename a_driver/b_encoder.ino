 #include <Encoder.h>
long oldPositionL  = -999;
long oldPositionR  = -999;


// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder EncL(3, 5);
Encoder EncR(2, 6);


