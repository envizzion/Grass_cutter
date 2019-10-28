


int Htime;              //integer for storing high time
int Ltime;                //integer for storing low time
float Ttime;            // integer for storing total time of a cycle
float frequency;        //storing frequency

void setup()
{
  Serial.begin(9600);
    pinMode(8,INPUT);
    delay(1000);
    Serial.println("jijjkk");

}
void loop()
{


   // Htime=pulseIn(8,HIGH);      //read high time
   Ltime=pulseIn(8,LOW);        //read low time
    
    Ttime = Htime+Ltime;

   // frequency=1000000/Ttime;    //getting frequency with Ttime is in Micro seconds
Serial.println(Ltime);
    delay(500);
}

