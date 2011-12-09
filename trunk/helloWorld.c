//================================================
//  Example HelloWorld
//================================================
//  digital_pins
//           step 0  1  2  3  4  5  6  7  8  9 10 11 12 13
//          --- -------------------------------------------
// SCENDIGPIN 0     0  0  0  0  0  0  0  0  0  0  0  0  0  0
// SCENxDIGPIN 50    0  0  1  0  0  0  0  0  0  0  0  0  0  0
// SCENxDIGPIN 100   0  0  0  1  0  0  0  0  0  0  0  0  0  0
// SCENxDIGPIN 150   0  0  1  0  0  0  0  0  0  0  0  0  0  0
// SCENxDIGPIN 200   0  0  0  1  0  0  0  0  0  0  0  0  0  0
// SCENxDIGPIN 250   0  0  1  0  0  0  0  0  0  0  0  0  0  0
// SCENxDIGPIN 300   0  0  0  1  0  0  0  0  0  0  0  0  0  0
//
//  analog_pins
//            step    0   1   2   3   4   5
//            ------------------------------
// SCENANAPIN   1     0    0    0    0   0  0 
// SCENxANAPIN  50    0    0    0    0   131  2 
// SCENxANAPIN 100    0    0    0    0   116  8 
//
//================================================
// Simuino log text customization
//================================================
// SKETCH_NAME: HelloWorld

// PINMODE_OUT: 11  "PIN: Led Urgent"
// PINMODE_OUT: 12  "PIN: Led Blink"

// DIGITALWRITE_LOW:  11  "Waiting"
// DIGITALWRITE_HIGH: 11  "Urgent"
// DIGITALWRITE_LOW:  12  "Led is on"
// DIGITALWRITE_HIGH: 12  "Led is off"

// DIGITALREAD: 10  "Read from somewhere"


// ANALOGREAD: 5  "read analog value"


//-------- DIGITAL PIN settings ------------------

// Leds
int URGENTLED    = 11;
int BLINKLED     = 12;
int IN_PIN       = 10;
int CONTROL      =  9;
 
//-------- ANALOGUE PIN settings
int SENSOR1  = 4;
int SENSOR2  = 5;

//================================================
//  Function Declarations
//================================================

void blinkLed(int n);

//================================================
void urgent()
//================================================
{
      digitalWrite(URGENTLED, HIGH); 
      delay(401);
      digitalWrite(URGENTLED, LOW); 
}
//================================================
void very_urgent()
//================================================
{
      digitalWrite(URGENTLED, HIGH); 
      delay(402);
      digitalWrite(URGENTLED, LOW); 
}
//================================================
void setup()
//================================================
{
  Serial.begin(9600); 
  attachInterrupt(0,urgent, CHANGE);
  attachInterrupt(1,very_urgent, RISING);
  pinMode(BLINKLED,OUTPUT);   
  pinMode(URGENTLED,OUTPUT);   
  pinMode(IN_PIN,INPUT);
  pinMode(CONTROL,INPUT);
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
  
  delay(1000); 
}

//================================================
void blinkLed(int n)
//================================================
{
  int i;
  for(i=1;i<=n;i++)
    {
      digitalWrite(BLINKLED, HIGH); 
      delay(500);
      digitalWrite(BLINKLED, LOW); 
    }
}
//================================================
// End of Sketch
//================================================
