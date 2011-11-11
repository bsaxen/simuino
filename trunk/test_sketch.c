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

void setup()
{
  int i;
  Serial.begin(9600);
  for(i=0;i<=6;i++)  pinMode(i,INPUT); 
  for(i=7;i<=13;i++) pinMode(i,OUTPUT); 
}
	 

void loop()
{
  int i,x;

  for(i=0;i<=6;i++)
    {
      x = digitalRead(i);
      Serial.print(x);
      Serial.print(",");
    } 
  Serial.println("-");

  for(i=7;i<=13;i++)digitalWrite(i,LOW);

  analogWrite(3,134);

  for(i=0;i<=5;i++)
    {
      x = analogRead(i);
      Serial.print(x);
      Serial.print(",");
    }
  Serial.println("-");
  delay(300);
  for(i=7;i<=13;i++)digitalWrite(i,HIGH);
}
