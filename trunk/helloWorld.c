//================================================
//  Example HelloWorld
//================================================
//  digital_pins
//           step 0  1  2  3  4  5  6  7  8  9 10 11 12 13
//          --- -------------------------------------------
// SCENDIGPIN 0   0  0  0  0  0  0  0  0  0  0  0  0  0  0
//
//  analog_pins
//            step    0   1   2   3   4   5
//            ------------------------------
// SCENANAPIN  1    0    0    0    0    0    3 
// SCENANAPIN 20    0    0    0    0    0    1 
// SCENANAPIN 30    0    0    0    0    0    2 
//  interrupts
//          step 0 1
//         ----------
// SCENINRPT   0 0 0
// SCENINRPT  25 1 0
// SCENINRPT  35 0 0
// SCENINRPT  45 1 0
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


// ANALOGREAD: 5  "read analog value"


//-------- DIGITAL PIN settings ------------------

// Leds
int URGENTLED    = 11;
int BLINKLED     = 12;
 
//-------- ANALOGUE PIN settings
int SENSOR  = 5;

//================================================
//  Function Declarations
//================================================

void blinkLed(int n);

//================================================
void urgent()
//================================================
{
      digitalWrite(URGENTLED, HIGH); 
      delay(400);
      digitalWrite(URGENTLED, LOW); 
}
//================================================
void setup()
//================================================
{
  Serial.begin(9600); 
  attachInterrupt(0,urgent, CHANGE);
  pinMode(BLINKLED,OUTPUT);   
  pinMode(URGENTLED,OUTPUT);   
}
	 
//================================================ 
void loop()
//================================================
{
int value;

  Serial.println("Hello Simuino!");
  value = analogRead(SENSOR);
  Serial.print("Analog value read: ");
  Serial.println(value);
  blinkLed(value);
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
