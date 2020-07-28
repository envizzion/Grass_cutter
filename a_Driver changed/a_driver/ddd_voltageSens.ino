#define cutOffVoltage  10 ; //cutoff voltage in volts

byte lowVoltage = 0 ;
int voltageSens = 0 ;
int voltageSensMaxCounts = 5 ;
int voltageSensCounter = 0 ; 
int voltageCutOff = 1024*(10/12);

void readBattery() {


if(voltageSensCounter < voltageSensMaxCounts){
  voltageSens += analogReadFast(A2) ;
  voltageSensCounter++ ;
  }
else {
  voltageSens  = voltageSens/voltageSensMaxCounts;
     if(voltageSens < voltageCutOff){
      lowVoltage = 1 ; 
    }
    else {
      lowVoltage = 0 ;
      }
   voltageSens = 0 ;
   voltageSensCounter = 0;
  }


  
  }
