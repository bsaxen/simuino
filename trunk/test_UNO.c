// BOARD_TYPE: UNO
// SKETCH_NAME: Test_UNO
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
// SCENDIGPIN  2  100    1
// SCENDIGPIN  3    1    0
// SCENDIGPIN  3  140    1
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
//int SENSOR1  = 4;
//int SENSOR2  = 5;

//================================================
//  Function Declarations
//================================================

void blinkLed(int pin,int n);
void preTestCase();
void postTestCase();
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
 // Do nothing
}
	 
int nloop = 0;
//================================================ 
void loop()
//================================================
{
  int value,i;
  nloop++;

  preTestCase();

  Serial.print("----- TestCase: ");Serial.print(nloop);

//--------------------------------
  if (nloop == 1)
  {
    pinMode(SORRY,OUTPUT);
    Serial.println(" Interrupts according to scenario  ------");
    attachInterrupt(0,sorryToBotherYou_1, CHANGE);
    attachInterrupt(1,sorryToBotherYou_2, RISING);
    blinkLed(12,50);
    detachInterrupt(0);
    detachInterrupt(1);
  }

//--------------------------------
  if (nloop == 2)
  {
    Serial.println(" Serial output ------");

    Serial.println("This is a line with cr");
    Serial.print("Test print:");
    Serial.print("Hello ");
    Serial.print("Simuino ");
    Serial.print(100);
    Serial.println("end of line");
    Serial.print("Now the same without ln\n");

    Serial.print("This is a line with cr\n");
    Serial.print("Test print:");
    Serial.print("Hello ");
    Serial.print("Simuino ");
    Serial.print(100);
    Serial.print("end of line\n");
  }

//--------------------------------
  if (nloop == 3)
  {
    Serial.println(" Digital Write Blink Leds 4 - 13  ------");
    for (i=4;i<=13;i++)
    {
      pinMode(i,OUTPUT);
    }
    for (i=4;i<13;i++)
    {
      blinkLed(i,2);
    }

  }

//--------------------------------
  if (nloop == 4)
  {
    Serial.println(" Analog Read  ------");

    for(i=0;i<=5;i++)
    {
       value = analogRead(i);
       Serial.print("Analog Value Pin "); Serial.print(i);Serial.print(": ");
       Serial.println(value);
    }
 
  }

//--------------------------------
  if (nloop == 5)
  {
    Serial.println(" Digital Read pin 3-13  ------");
    for(i=3;i<=13;i++)
    {
       value = digitalRead(i);
       Serial.print("Digital Value Pin "); Serial.print(i);Serial.print(": ");
       Serial.println(value);
    }

  }
  

//--------------------------------
  if (nloop == 6)
  {
    Serial.println(" Port Manipulation DDR and PORT  ------");
    

    // digital pins 7,6,5,4,3,2,1,0
    DDRD = B11111110;  
    // digital pins -,-,13,12,11,10,9,8
    DDRB = B00111111;  
    delay(500);
   // digital pins 7,6,5,4,3,2,1,0
    DDRD = B00000000;  
    // digital pins -,-,13,12,11,10,9,8
    DDRB = B00000000;  
    delay(500);

    // All odd pins to LOW and even to HIGH
    PORTD = B01010100; 
    PORTB = B00010101;
    delay(500);
    // All odd pins to HIGH and even to LOW
    PORTD = B10101000; 
    PORTB = B00101010;
    delay(500);

    // PIND and PINB shall reflect  HIGH/LOW if pinMode = INPUT

    //PORTD &= B00000011;   // turns off 2..7, but leaves pins 0 and 1 alone

  }

//--------------------------------
  if (nloop == 7)
  {
    Serial.println(" PWM  ------");

    i = 3;value=100+i;
    analogWrite(i,value);
    Serial.print("Analog Write Value Pin "); Serial.print(i);Serial.print(": ");
    Serial.println(value);

    i = 5;value=100+i;
    analogWrite(i,value);
    Serial.print("Analog Write Value Pin "); Serial.print(i);Serial.print(": ");
    Serial.println(value);

    i = 6;value=100+i;
    analogWrite(i,value);
    Serial.print("Analog Write Value Pin "); Serial.print(i);Serial.print(": ");
    Serial.println(value);

    i = 9;value=100+i;
    analogWrite(i,value);
    Serial.print("Analog Write Value Pin "); Serial.print(i);Serial.print(": ");
    Serial.println(value);

    i = 10;value=100+i;
    analogWrite(i,value);
    Serial.print("Analog Write Value Pin "); Serial.print(i);Serial.print(": ");
    Serial.println(value);

    i = 11;value=100+i;
    analogWrite(i,value);
    Serial.print("Analog Write Value Pin "); Serial.print(i);Serial.print(": ");
    Serial.println(value);

  }

//--------------------------------
  if (nloop == 8)
  {
    Serial.println(" Read PINB PIND register according to scenario ------");

    for (i=3;i<8;i++)
    {
      value = bitRead(PIND,i);
      if(value == HIGH)
      {
         Serial.print("Pin is HIGH:");
         Serial.println(i);
      }
      else
      {
         Serial.print("Pin is LOW:");
         Serial.println(i);
      }
    }

    for (i=8;i<14;i++)
    {
      value = bitRead(PINB,i-8);
      if(value == HIGH)
      {
         Serial.print("Pin is HIGH:");
         Serial.println(i);
      }
      else
      {
         Serial.print("Pin is LOW:");
         Serial.println(i);
      }
    }

  }

//--------------------------------
  postTestCase();
  delay(10);
 
}

//================================================
void preTestCase()
//================================================
{
  int i;
  Serial.begin(9600);
  for(i=0;i<=13;i++)pinMode(i,INPUT);
}
//================================================
void postTestCase()
//================================================
{
  Serial.println("----- End of Test -----");
  Serial.end();
}
//================================================
void blinkLed(int pin,int n)
//================================================
{
  int i;

  pinMode(pin,OUTPUT);
  for(i=1;i<=n;i++)
    {
      digitalWrite(pin, HIGH); 
      delay(500);
      digitalWrite(pin, LOW); 
    }
  pinMode(pin,INPUT);
}
//================================================
// End of Sketch
//================================================
