#include "avdweb_AnalogReadFast.h"
boolean foundWire ;
boolean foundGWire;
int wireCutOff = 640;
//int wireArray[10]; //not yet used
int wireMaxCount = 5;//10  ;
int wireAverage,wireCount=0;
int wireGAverage,wireGCount=0;

int maxAmplCounts= 0 ; //counter to track the no of maximum readings(greater than wireCutOff) per wireMaxCount cycle
int maxAmplGCounts= 0 ;
void searchWire(){
  /** average test **/    
//   Serial.println(analogReadFast(A0));
  if(wireCount<wireMaxCount){
   //wireAverage+=analogReadFast(A1);

   
    /** maxAmplitudeCount test **/
   if(analogReadFast(A1)>wireCutOff ||analogReadFast(A0)>wireCutOff - 100 ){
    
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

   if(maxAmplCounts >= 2){  //4 was the best value observed for 10 counts
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

 void searchGWire(){
  
//     Serial.println(analogReadFast(A0));
  if(wireGCount<wireMaxCount){
   //wireAverage+=analogReadFast(A1);

   
   if(analogReadFast(A1)>wireCutOff){
    
      maxAmplGCounts++;
    }
   
   wireGCount++;
  }      
  else{



   if(maxAmplGCounts >= 4){  //4 was the best value observed for 10 counts
    foundGWire = true;
    }
   else {
    foundGWire = false;
    }
   
  // Serial.print(foundWire);
// Serial.println(maxAmplCounts );

  maxAmplGCounts = 0;
  wireGCount = 0 ;
        
    }
  
  
  }

