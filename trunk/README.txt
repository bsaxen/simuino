==================================================
SIMUINO is a Arduino Simulator. 
Code: c++
Platform: Linux (Ubuntu)
Version: 0.0.5

Developed by Benny Saxen, ADCAJO
==================================================

Content:

1. Get started
2. Commands
3. Display
4. Sketch information
5. Configuration
6. Scenarios
7. Supported language functions

--------------------------------------------------
1. Get started
--------------------------------------------------
Quick starter guide:

1. Download simuino zip-file from Google Code, project simuino

2. Unzip the file. A directory named simuino will be created.

3. Go to directory simuino. Compile: g++ -O2 -o simuino simuino.c -lncurses
   You need to have installed packages for ncurses-dev.

4. Run Simuino:  ./simuino
   Note - expand the size of your terminal !

5. If this is the first time you start Simuino. Type 'c' to enter admin-mode.
   Enter 'conf'  - the configuration is shown in the message window.
   Enter 'sketch <path + your sketch file name>'. This will be saved in the config. 

6. Enter 'load' . The sketch will be part of the Servuino build.

7. Enter 'run <steps>'  This will create a log-file (servuino/data.su).

8. Enter 'ex' to leave admin-mode.

9. Type 'h' if you need to see avalable commands in run-mode

10. Start evaluate the simulation by stepping

Repeat step 6 - 10 if you make any changes in your sketch( i.e. logic, scenario,customized log-text)

--------------------------------------------------
2. Commands
--------------------------------------------------
Available commands:
 s  step
 r  run a loop
 g  run complete simulation
 a  re-run simulation
 q  quit anytime, except during execution of a command
 c  enter command mode
 h  show this information

--------------------------------------------------
3. Display
--------------------------------------------------
The SIMUNIO displays 4 windows

 - The Arduino board with the pin-layout
 - Serial Interface
 - Logs
 - Messages

3.1  Arduino board Window
     
     For each pin the following data will be displayed:
	 Input or Output pin (in/out)
	 Reading or writing  (r/w)
	 Digital HIGH or LOW value (5 or 0) and values of analog pin (0 - 1023)

3.2  Serial Interface

     Anything printed on this interface will be shown here.

3.3  Logs 

     Most functions in the Arduino reference library will be displayed.
     Every line has two index: loop number, pseudo-time-elapsed, i.e steps/instructions.
     The log-text can be customized to be more readable for the specific appliaction, see chapter 4 below.

3.4  Message

     Shows any error messages and length (no of loops) of scenario.


--------------------------------------------------
4. Sketch information
--------------------------------------------------

Customized logging:

Add the following rows in your sketch with suitable log-text according to your specific application.

Example:
// SKETCH_NAME           My_sketch_name
// PINMODE_OUT        2  "your log text for pin 2"
// PINMODE_IN         4  "your log text for pin 4"
// DIGITALWRITE_LOW  11  "your log text for pin 11 when HIGH"
// DIGITALWRITE_HIGH 11  "your log text for pin 11 when LOW"
// ANALOGREAD         1  "your log text for pin 1"
// DIGITALREAD       10  "your log text for pin 10"
// ANALOGWRITE       14  "your log text for pin 14"

This will make Simuino show your text in the log window.

--------------------------------------------------
5. Configuration
--------------------------------------------------

The following parameters are configured in the file, config.txt :

LOG_LEVEL  2		 // Level of information to be displayed during simulation (0,1,2,3)
DELAY    100		 // Delay in ms between each step/instruction  (0 - 1000)
LOG_FILE   0		 // Save the log information to the file: log.txt (YES=1,NO=0)

--------------------------------------------------
6. Scenarios
-------------------------------------------------- 

In order to be able to run specific scenarios, controlled by values read from pins,
it is possible to specify what value shall be read per step/instruction and pin.
Only changes is needed to be specified, example:

10   123
14   150
20    45

will generate:

10   123
11   123
12   123
13   123
14   150
15   150
16   150
17   150
18   150
19   150
20    45
21    45
 
and so on...

The scenario data is specified in your sketch-file, se example below..

Below is an example of a scenario file.
-----------------------------------------
//  digital_pins
//           step 0  1  2  3  4  5  6  7  8  9 10 11 12 13
//          --- -------------------------------------------
// SCENDIGPIN 0   1  0  1  0  0  0  0  0  0  0  0  0  0  0
// SCENDIGPIN 100 0  1  0  0  0  0  0  0  0  0  0  0  0  0
// SCENDIGPIN 200 1  0  1  0  0  0  0  0  0  0  0  0  0  0
// SCENDIGPIN 300 1  1  0  0  0  0  0  0  0  0  0  0  0  0
//
//  analog_pins
//            step    0   1   2   3   4   5
//            ------------------------------
// SCENANAPIN   0    37   0   0   0   0  11 
// SCENANAPIN 100    56   0   0   0   0  12 
// SCENANAPIN 200     0   0   0   0   0  13 
// SCENANAPIN 300   123   0   0   0   0  14 
//
//  interrupts
//          step 0 1
//         ----------
// SCENINRPT 16  1 0
// SCENINRPT 19  0 0
// SCENINRPT 25  1 0
// SCENINRPT 30  1 1
// SCENINRPT 40  0 1
// SCENINRPT 55  1 1
-------------------
End of example


Note - The data has to be given in ascending step order.

--------------------------------------------------
7. Supported language functions
--------------------------------------------------
Unsupported functions are implemented with a dummy, in order to compile without errors.

Digital I/O
	pinMode()		Yes
	digitalWrite()		Yes
	digitalRead()		Yes
Analog I/O
	analogReference()	No
	analogRead()		Yes
	analogWrite() - PWM	Yes
Advanced I/O
	tone()			No
	noTone()		No
	shiftOut()		No
	shiftIn()		No
	pulseIn()		No
Time
	millis()		Yes
	micros()		Yes
	delay()			Yes
	delayMicroseconds()	Yes
Math
	min()			Yes
	max()			Yes
	abs()			Yes
	constrain()		Yes
	map()			Yes
	pow()			Yes
	sqrt()			Yes
Trigonometry
	sin()			Yes
	cos()			Yes
	tan()			Yes
	Random Numbers		
	       randomSeed()	Yes
	       random()		Yes
Bits and Bytes
	lowByte()		No
	highByte()		No
	bitRead()		No
	bitWrite()		No
	bitSet()		No
	bitClear()		No
	bit()			No
External Interrupts
	attachInterrupt()	Yes
	detachInterrupt()	Yes
Interrupts
	interrupts()		No	
	noInterrupts()		No
Communication
	Serial			
		begin()		Yes
		end()		Yes
		available()	No
		read()		No
		peek()		No
		flush()		Yes
		print()		Yes
		println()	Yes
		write()		Yes

--------------------------------------------------
End of README
--------------------------------------------------
