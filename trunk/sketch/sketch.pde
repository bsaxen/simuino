//================================================
//  Developed by Benny Saxén
//
//  2011-11-12
//================================================
// simuino: Heat_Control


// PINMODE_IN:  2   "init start"
// PINMODE_OUT: 8   "init CCW"
// PINMODE_OUT: 9   "init motor direction"
// PINMODE_OUT: 10  "init step motor"
// PINMODE_OUT: 11  "init motor sleeping"

// DIGITALWRITE_LOW:   7  "LED CW off"
// DIGITALWRITE_HIGH:  7  "LED CW on"
// DIGITALWRITE_LOW:   8  "LED CCW off"
// DIGITALWRITE_HIGH:  8  "LED CCW on"
// DIGITALWRITE_LOW:   9  "Direction ->"
// DIGITALWRITE_HIGH:  9  "Direction <-"
// DIGITALWRITE_LOW:  10  "Step >"
// DIGITALWRITE_HIGH: 10  "Step <"
// DIGITALWRITE_LOW:  11  "StepperDriver sleep"
// DIGITALWRITE_HIGH: 11  "StepperDriver awake"


// ANALOGREAD: 5  "Outdoor temp"
//--------------------------------------------------------------------------
//Theory
// Utomhus sensor: Temp_ute(Celcius) = -2.13xMotstÃ¥nd(kOhm)  + 8.57  
//
// Sensorn seriekopplad med 10kOhm
//
// 10/14*5 = 3.57 v    max  vid 0 grader Celcius  => 3.57/5*1024 = 731
// 10/26*5 = 1.92 v   min  vid ca -20 grader Celcius  => 1.92/5*1024 = 393
// map(x,393,731,20,0)  from analogRead to minus degrees Celcius
//--------------------------------------------------------------------------
int i;
int lowerTempLimit       = 200;
int targetShuntPosition  = 0;
int currentShuntPosition = 0; 
int emergencyStop        = 0;
int minusCelcius         = 0;
int aTempValue           = 0;
//-------- DIGITAL PIN settings --------------------------
// EasyDriver
int DIR       =  9; 
int STEP      = 10;
int SLEEP     = 11;
// Control
int cwLED    =  7;
int ccwLED    =  8;
//-------- ANALOGUE PIN settings
int tempOutdoor = 5;
//-------------------------------------------------------- 



void setup()
{
  Serial.begin(9600);     // open the serial connection at 9600bps

  pinMode(DIR,    OUTPUT);   
  pinMode(STEP,   OUTPUT);
  pinMode(SLEEP,  OUTPUT); 
  pinMode(cwLED,  OUTPUT);
  pinMode(ccwLED, OUTPUT);
}
	 
void turn_cw(int delta);
void turn_ccw(int delta);
int set_shunt_position(int from, int to);
 
void loop()
{
  if(emergencyStop != 1)
    {
      aTempValue          = analogRead(tempOutdoor); // 4 - 16 kOhm
      Serial.print("Sensor:");
      Serial.println(aTempValue);
      minusCelcius        = map(aTempValue,393,731,20,0);
      Serial.print("Celcius(-):");
      Serial.println(minusCelcius);
      targetShuntPosition = map(minusCelcius,0,10,15,30);
      Serial.print("Angle:");
      Serial.println(targetShuntPosition);

      Serial.print(currentShuntPosition); 
      Serial.print("--->");
      Serial.println(targetShuntPosition);

      currentShuntPosition = set_shunt_position(currentShuntPosition,targetShuntPosition);
      if(currentShuntPosition > 90 || currentShuntPosition < 0) emergencyStop = 1;

      delay(300);
    }
}
//================================================================
int set_shunt_position(int from, int to)
{
  int delta=0;

  digitalWrite(ccwLED, HIGH);
  digitalWrite(cwLED, HIGH);
  delay(700);
  digitalWrite(ccwLED, LOW);
  digitalWrite(cwLED, LOW);

  if(to > 90 || to < 0) return(99);
  if(from > 90 || from < 0) return(99);

  //if(from == to) return(to);

  if(from > to)
  {
     delta = from-to;
     turn_ccw(delta);
  }  
  if(from < to)
  {
     delta = to-from;
     turn_cw(delta);
  }
  return(to);
}
void turn_cw(int delta)
{
   int steps = 0;
      steps = map(delta,0,90,0,100);
      digitalWrite(cwLED, HIGH); 
         
      digitalWrite(SLEEP, HIGH); // Set the Sleep mode to AWAKE.
      delay(500);
      digitalWrite(DIR, LOW);        

      // Step one degree
      for(i=0;i<=steps;i++)
	{
	  digitalWrite(STEP, LOW); 
	  delay(10);
	  digitalWrite(STEP, HIGH);
	  delay(10);       
	}  
      digitalWrite(SLEEP, LOW); // Set the Sleep mode to SLEEP.   
      digitalWrite(cwLED, LOW);    
}

void turn_ccw(int delta)
{
   int steps = 0;
      steps = map(delta,0,90,0,100);

      digitalWrite(ccwLED, HIGH);  

      digitalWrite(SLEEP, HIGH); // Set the Sleep mode to AWAKE.
      delay(500);
      digitalWrite(DIR, HIGH);        


      // Step one degree
      for(i=0;i<=steps;i++)
	{
	  digitalWrite(STEP, LOW);    
	  delay(11);
	  digitalWrite(STEP, HIGH);  
	  delay(11);    
	}  
      digitalWrite(SLEEP, LOW); // Set the Sleep mode to SLEEP.  
      digitalWrite(ccwLED, LOW);       
}
