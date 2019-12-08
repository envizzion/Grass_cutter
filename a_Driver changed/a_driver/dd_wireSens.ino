#include "avdweb_AnalogReadFast.h"
boolean foundWire ;
int wireCutOff = 500;
int wireArray[10]; //not yet used
int wireMaxCount = 10;
int wireAverage,wireCount;


void searchWire(){

  if(wireCount<wireMaxCount){
   wireAverage+=analogReadFast(A1);
  
  }      
  else{
      wireAverage/=wireMaxCount;

        if(wireAverage>wireCutOff){    
           foundWire = true ;
        }
        else {
          foundWire = false;
          }
            Serial.println(wireAverage); 
        wireAverage=0;
        wireCount=0;
        
    }
    
  }

