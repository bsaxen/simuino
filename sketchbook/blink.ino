// ===============================================
// SIMUINO_BEGIN
// ===============================================
// BOARD_TYPE:  UNO
// SKETCH_NAME: blink.ino
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
// PINMODE_OUT: 13  "PIN: Led Blink"

// DIGITALWRITE_LOW:  11  "Waiting"
// DIGITALWRITE_HIGH: 11  "Urgent"
// DIGITALWRITE_LOW:  13  "Led is off"
// DIGITALWRITE_HIGH: 13  "Led is on"

// DIGITALREAD:  9  "Read from nine"
// DIGITALREAD: 10  "Read from ten"


// ANALOGREAD: 4  "read analog four"
// ANALOGREAD: 5  "read analog five"

// ===============================================
// SIMUINO_END
// ===============================================
/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
int led = 13;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}
