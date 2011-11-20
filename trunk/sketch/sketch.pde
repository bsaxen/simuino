//================================================
//  Developed by Benny Saxén
//================================================


// Simuino is reading the lines below
//================================================

// simuino: Simuino_test_sketch


// PINMODE_IN:   2   "On/Off button"
// PINMODE_OUT:  8   "Red LED"
// PINMODE_OUT:  9   "Green LED"
// PINMODE_OUT: 10   "Yellow LED"
// PINMODE_OUT: 11   "Stop LED"

// DIGITALWRITE_LOW:   8  "Stepper running CW"
// DIGITALWRITE_HIGH:  8  "Stepper running CCW"

// ANALOGREAD: 0  "Outdoor temp"
// ANALOGREAD: 1  "Indoor temp"
// ANALOGREAD: 2  "Potentiometer"
// ANALOGREAD: 3  "Photoresistor"
// ANALOGREAD: 4  "Outdoor temp"

// ========== End of Simuino Lines ===============

volatile int state = LOW;


void test()
{
  digitalWrite(7,LOW);
  digitalWrite(7,HIGH);
}


void setup()
{
  int i;

  Serial.begin(9600);

  pinMode(0,INPUT);
  pinMode(1,INPUT);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  pinMode(7,OUTPUT);
 
  for(i=8;i<=13;i++) pinMode(i,OUTPUT); 

  attachInterrupt(0, test, CHANGE);
}
	 

void loop()
{
  int i,x;


  x = digitalRead(0);
  Serial.print(x);
  Serial.print(",");
 
  x = digitalRead(1);
  Serial.print(x);
  Serial.print(",");
 
  Serial.println("-");

  long mil = millis();
  Serial.println(mil);

  long mic = micros();
  Serial.println(mic);

  for(i=8;i<=13;i++)digitalWrite(i,LOW);

  analogWrite(9,134);

  for(i=0;i<=5;i++)
    {
      x = analogRead(i);
      Serial.print(x);
      Serial.print(",");
    }
  Serial.println("-");
  delay(300);
  for(i=8;i<=13;i++)digitalWrite(i,HIGH);
}
