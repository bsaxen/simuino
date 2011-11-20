//================================================
//  Developed by Benny Saxen
//================================================


// Simuino is reading the lines below
//================================================

// simuino: Simuino_empty_loop

// PINMODE_OUT:  7   "Pin 7 set to OUTPUT"

// DIGITALWRITE_LOW:   7  "Pin 7 set to LOW"
// DIGITALWRITE_HIGH:  7  "Pin 7 set to HIGH"

// ========== End of Simuino Lines ===============



void test()
{
  digitalWrite(7,LOW);
  digitalWrite(7,HIGH);
}


void setup()
{
  int i;

  Serial.begin(9600);

  pinMode(7,OUTPUT);

  attachInterrupt(0, test, CHANGE);

  Serial.println("Started");
}
	 

void loop()
{

}
