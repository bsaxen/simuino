//================================================
//  Example HelloWorld
//================================================
// BOARD_TYPE UNO
// SCENSIMLEN 600
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
// SCENDIGPIN  2    1    0
// SCENDIGPIN  2  200    1
// SCENDIGPIN  3    1    0
// SCENDIGPIN  3  220    1
// SCENDIGPIN  2  240    0
// SCENDIGPIN  3  260    0

//
// SCENANAPIN  4    1    5
// SCENANAPIN  5    1    8 
// SCENANAPIN  4   80   12
// SCENANAPIN  5  120   18 
//
//================================================
// Simuino log text customization
//================================================
// SKETCH_NAME: Test_UNO

// PINMODE_OUT: 13  "This is interrupt LED"
// PINMODE_OUT: 12  "PIN: Led Blink"

// DIGITALWRITE_LOW:  13  "Boom"
// DIGITALWRITE_HIGH: 13  "Crash"
// DIGITALWRITE_LOW:  12  "Led is off"
// DIGITALWRITE_HIGH: 12  "Led is on"

// DIGITALREAD:  9  "Read from nine"
// DIGITALREAD: 10  "Read from ten"


// ANALOGREAD: 4  "read analog four"
// ANALOGREAD: 5  "read analog five"


//-------- DIGITAL PIN settings ------------------

// Leds
int SORRY    = 13;
 
//-------- ANALOGUE PIN settings
int SENSOR1  = 4;
int SENSOR2  = 5;

//================================================
//  Function Declarations
//================================================

void blinkLed(int pin,int n);

//================================================
void sorryToBotherYou_1()
//================================================
{
      digitalWrite(SORRY, HIGH); 
      delay(901);
      digitalWrite(SORRY, LOW); 
}
//================================================
void sorryToBotherYou_2()
//================================================
{
      digitalWrite(SORRY, HIGH);
      delay(902);
      digitalWrite(SORRY, LOW);
}

//================================================
void setup()
//================================================
{
  Serial.begin(9600); 
  attachInterrupt(0,sorryToBotherYou_1, CHANGE);
  attachInterrupt(1,sorryToBotherYou_2, RISING);
  pinMode(SORRY,OUTPUT);
}
	 
int nloop = 0;
//================================================ 
void loop()
//================================================
{
  int value1,value2,i;
  nloop++;
// Test 1--------------------------------
  if (nloop == 1)
  {
    Serial.println("----- Test 1: Serial output ------");
    Serial.println("This is a line with cr");
    Serial.println("Test print:");
    Serial.print("Hello ");
    Serial.print("Simuino ");
    Serial.print(100);
    Serial.println(200);
    Serial.println("----- End of Test 1 -----");
  }

// Test 2--------------------------------
  if (nloop == 2)
  {
    Serial.println("----- Test 2: Blink All Leds  ------");
    for (i=4;i<13;i++)
    {
      pinMode(i,OUTPUT);
    }
    for (i=4;i<13;i++)
    {
      blinkLed(i,2);
    }
    Serial.println("----- End of Test 2 -----");
  }

// Test 3 --------------------------------
  if (nloop == 3)
  {
    Serial.println("----- Test 3: Analog Read  ------");
    value1 = analogRead(SENSOR1);
    value2 = analogRead(SENSOR2);
    Serial.print("Analog Value 1: ");
    Serial.println(value1);
    Serial.print("Analog Value 2: ");
    Serial.println(value2);
    Serial.println("----- End of Test 3 -----");
  }

// Test 4 --------------------------------
  if (nloop == 4)
  {
    pinMode(7,INPUT);
    pinMode(8,INPUT);
    Serial.println("----- Test 4: Digital Read  ------");
    value1 = digitalRead(7);
    value2 = digitalRead(8);
    Serial.print("Digital Value 1: ");
    Serial.println(value1);
    Serial.print("Digital Value 2: ");
    Serial.println(value2);
    pinMode(7,OUTPUT);
    pinMode(8,OUTPUT);
    Serial.println("----- End of Test 4 -----");
  }
 
// Test 5 --------------------------------
  if (nloop == 5)
  {
    Serial.println("----- Test 5: Interrupts  ------");
    blinkLed(12,50);
    Serial.println("----- End of Test 5 -----");
  }

  delay(10);
 
}

//================================================
void blinkLed(int pin,int n)
//================================================
{
  int i;
  for(i=1;i<=n;i++)
    {
      digitalWrite(pin, HIGH); 
      delay(500);
      digitalWrite(pin, LOW); 
    }
}
//================================================
// End of Sketch
//================================================
