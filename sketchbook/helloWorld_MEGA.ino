
// ===============================================
// SIMUINO_BEGIN
// ===============================================
// BOARD_TYPE:  MEGA
// SKETCH_NAME: HelloWorld_MEGA.ino
// SIM_LENGTH:  600
// WIN_LAYOUT:    2
// SO_DELAY:     40
//================================================
//  Scenario
//================================================
//
// SCENDIGPIN 10    1    0
// SCENDIGPIN 10   50    1
// SCENDIGPIN 10  100    0
// SCENDIGPIN 10  200    1
// SCENDIGPIN  9    1    0
// SCENDIGPIN  9   40    1
// SCENDIGPIN  9  130    0
//
// SCENANAPIN  4    1    5
// SCENANAPIN  5    1    8 
// SCENANAPIN  4   80   12
// SCENANAPIN  5  120   18 
//
//================================================
// Simuino log text customization
//================================================

// PINMODE_OUT: 11  "PIN: Led Urgent"
// PINMODE_OUT: 12  "PIN: Led Blink"

// DIGITALWRITE_LOW:  11  "Waiting"
// DIGITALWRITE_HIGH: 11  "Urgent"
// DIGITALWRITE_LOW:  12  "Led is off"
// DIGITALWRITE_HIGH: 12  "Led is on"

// DIGITALREAD:  9  "Read from nine"
// DIGITALREAD: 10  "Read from ten"


// ANALOGREAD: 4  "read analog four"
// ANALOGREAD: 5  "read analog five"


//-------- DIGITAL PIN settings ------------------

// Leds
int URGENTLED    = 31;
int BLINKLED     = 32;
int IN_PIN       = 52;
int CONTROL      = 53;
 
//-------- ANALOGUE PIN settings
int SENSOR1  = 4;
int SENSOR2  = 5;

//================================================
//  Function Declarations
//================================================

void blinkLed(int n);

//================================================
void urgent0()
//================================================
{
      digitalWrite(URGENTLED, HIGH); 
      delay(400);
      digitalWrite(URGENTLED, LOW); 
}
//================================================
void urgent1()
//================================================
{
      digitalWrite(URGENTLED, HIGH); 
      delay(401);
      digitalWrite(URGENTLED, LOW); 
}
//================================================
void urgent2()
//================================================
{
      digitalWrite(URGENTLED, HIGH); 
      delay(402);
      digitalWrite(URGENTLED, LOW); 
}
//================================================
void urgent3()
//================================================
{
      digitalWrite(URGENTLED, HIGH); 
      delay(403);
      digitalWrite(URGENTLED, LOW); 
}
//================================================
void urgent4()
//================================================
{
      digitalWrite(URGENTLED, HIGH); 
      delay(404);
      digitalWrite(URGENTLED, LOW); 
}
//================================================
void urgent5()
//================================================
{
      digitalWrite(URGENTLED, HIGH); 
      delay(405);
      digitalWrite(URGENTLED, LOW); 
}
//================================================
void setup()
//================================================
{
  Serial.begin(9600); 
  
  pinMode(BLINKLED,OUTPUT);   
  pinMode(URGENTLED,OUTPUT);   
  pinMode(IN_PIN,INPUT);
  pinMode(CONTROL,INPUT);
  pinMode(15,OUTPUT);
  pinMode(44,INPUT);
  pinMode(45,OUTPUT);
  pinMode(46,OUTPUT);
  pinMode(47,INPUT);
  attachInterrupt(0,urgent0, CHANGE);
  attachInterrupt(1,urgent1, RISING);
  attachInterrupt(2,urgent2, FALLING);
  attachInterrupt(3,urgent3, LOW);
  attachInterrupt(4,urgent4, CHANGE);
  attachInterrupt(5,urgent5, RISING);
}
	 
//================================================ 
void loop()
//================================================
{
  int value1,value2,i;

  Serial.println("Hello Simuino!");
  value1 = analogRead(SENSOR1);
  value2 = analogRead(SENSOR2);
  Serial.print("Analog 1 value read: ");
  Serial.println(value1);
  Serial.print("Analog 2 value read: ");
  Serial.println(value2);
  blinkLed(value1);
  value1 = digitalRead(IN_PIN);
  value2 = digitalRead(CONTROL);
  Serial.print("Digital IN_PIN read: ");
  Serial.println(value1);
  Serial.print("Digital CONTROL read: ");
  Serial.println(value2);

  analogWrite(10,123);
  analogWrite(11,167);

  analogWrite(3,127);
  analogWrite(4,147);
  
  delay(1000); 
}

//================================================
void blinkLed(int n)
//================================================
{
  int i;
  for(i=1;i<=2;i++)
    {
      digitalWrite(BLINKLED, HIGH); 
      delay(500);
      digitalWrite(BLINKLED, LOW); 
    }
}
//================================================
// End of Sketch
//================================================
