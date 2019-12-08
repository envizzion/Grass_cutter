#include "avdweb_AnalogReadFast.h"
boolean foundWire ;
int wireCutOff = 640;
int wireArray[10]; //not yet used
int wireMaxCount = 10;
int wireAverage,wireCount=0;
int maxAmplCounts= 0 ; //counter to track the no of maximum readings(greater than wireCutOff) per wireMaxCount cycle

void searchWire(){
  /** average test **/    
  if(wireCount<wireMaxCount){
   //wireAverage+=analogReadFast(A1);

   
    /** maxAmplitudeCount test **/
   if(analogReadFast(A1)>wireCutOff){
      maxAmplCounts++;
    }
   
   wireCount++;
  }      
  else{

/** average test **/    
//      wireAverage/=wireMaxCount;
//
//        if(wireAverage>wireCutOff){    
//           foundWire = true ;
//        }
//        else {
//          foundWire = false;
//          }
//        //    Serial.println(wireAverage); 
//      //  Serial.println(foundWire);
//        wireAverage=0;
//        wireCount=0;

   /** maxAmplitudeCount test **/

   if(maxAmplCounts >= 4){  //4 was the best value observed for 10 counts
    foundWire = true;
    }
   else {
    foundWire = false;
    }
   
  // Serial.print(foundWire);
// Serial.println(maxAmplCounts );

  maxAmplCounts = 0;
  wireCount = 0 ;
        
    }
    
  }

