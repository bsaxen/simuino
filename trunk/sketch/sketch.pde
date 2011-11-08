//================================================
//  Developed by Benny Saxén
//================================================
// simuino: Panna reglering


int i;
int started;
int INCR = 20;
int upperTempLimit = 300;
int lowerTempLimit = 200;
int targetShuntPosition = 0;
int currentShuntPosition = 0; 
int emergencyStop = 0;
//-------- DIGITAL PIN settings --------------------------
// EasyDriver
int DIR       =  9; 
int STEP      = 10;
int SLEEP     = 11;
// Control
int redLED    =  8;
int yellowLED =  7;
int startSignal = 2;
//-------- ANALOGUE PIN settings
int tempOutdoor = 5;
//-------------------------------------------------------- 

int aTempValue = 0;

void setup()
{
  Serial.begin(9600);     // open the serial connection at 9600bps

  pinMode(DIR, OUTPUT);   
  pinMode(STEP, OUTPUT);
  pinMode(SLEEP, OUTPUT); 
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(startSignal,INPUT);

  digitalWrite(DIR, LOW);
  digitalWrite(SLEEP, LOW);
  digitalWrite(STEP, LOW);

  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW); 
}
	 
void turn_cw(int delta);
void turn_ccw(int delta);
int set_shunt_position(int from, int to);
 
void loop()
{

started = digitalRead(startSignal);
// Check if applicationis to be started
if(started == LOW && emergencyStop == 0)
{
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);

  aTempValue = analogRead(tempOutdoor);
  targetShuntPosition = map(aTempValue,0,255,0,90);
  delay(300);
  Serial.print(currentShuntPosition); 
  Serial.print("   ");
  Serial.println(targetShuntPosition);
  currentShuntPosition = set_shunt_position(currentShuntPosition,targetShuntPosition);
  if(currentShuntPosition > 90 || currentShuntPosition < 0) emergencyStop = 1;
}
else
{
  Serial.println("Application stopped");
}

}
//================================================================
int set_shunt_position(int from, int to)
{
  int delta=0;
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
      steps = map(delta,0,90,0,10);
      digitalWrite(redLED, LOW); 
      digitalWrite(DIR, LOW);                 
      //digitalWrite(SLEEP, HIGH); // Set the Sleep mode to AWAKE.
      for(i=0;i<=steps;i++)
	{
	  digitalWrite(STEP, LOW); 
	  delay(10);
	  digitalWrite(STEP, HIGH);
	  delay(10);       
	}  
      digitalWrite(SLEEP, LOW); // Set the Sleep mode to SLEEP.       
}

void turn_ccw(int delta)
{
   int steps = 0;
      steps = map(delta,0,90,0,10);

      digitalWrite(redLED, HIGH);             
      digitalWrite(DIR, HIGH);                 
      //digitalWrite(SLEEP, HIGH); // Set the Sleep mode to AWAKE.
      for(i=0;i<=steps;i++)
	{
	  digitalWrite(STEP, LOW);    
	  delay(11);
	  digitalWrite(STEP, HIGH);  
	  delay(11);    
	}  
      digitalWrite(DIR, LOW);
      digitalWrite(SLEEP, LOW); // Set the Sleep mode to SLEEP.       
}
