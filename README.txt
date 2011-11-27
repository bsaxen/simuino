==================================================
SIMUINO is a Arduino Simulator. 
Code: c++
Platform: Linux (Ubuntu)
Version: 0.0.1

Developed by Benny Saxen, ADCAJO
==================================================


--------------------------------------------------
1. Get started
--------------------------------------------------
Quick starter guide:

1. Download simuino zip-file from Google Code, project simuino

2. Unzip the file. A directory named simuino will be created.

3. Go to directory simuino. Compile: g++ -O2 -o simuino simuino.c -lncurses
   You need to have installed packages for ncurses-dev.

4. Run script:   sh runSimuino  <your sketch.pde>  <simulation length in steps>
   Note - expand the size of your terminal !

   Example:   sh runSimuino  stepper_motor.pde  700

   Command h will show available commands

--------------------------------------------------
2. Commands
--------------------------------------------------
Available commands:
 s  step
 r  run a loop
 g  run complete simulation
 a  re-run simulation
 q  quit anytime, except during execution of a command

 l  toggles log level (3: shows delay and serial)
 +  increase delay in stepping by +10
 -  decrease delay in stepping by -10
 f  records logging to file when ON. Toggles ON/OFF (1/0)

 h  show this information
 i  show sketch information

--------------------------------------------------
4. Display
--------------------------------------------------
The SIMUNIO displayes 4 windows:

 - The Arduino board with the pin-layout
 - Serial Interface
 - Logs
 - Messages

4.1  Arduino board Window
     
     For each pin the following data will be displayed:
	 Input or Output pin (in/out)
	 Reading or writing  (r/w)
	 Digital HIGH or LOW value (5 or 0) and values of analog pin (0 - 1023)

4.2  Serial Interface

     Anything printed on this interface will be shown here.

4.3  Logs 

     Most functions in the Arduino reference library will be displayed.
     Every line has two index: loop number, pseudo-time-elapsed, i.e steps/instructions.
     The log-text can be customized to be more readable for the specific appliaction, see chapter 6 below.

4.4  Message

     Shows any error messages and length (no of loops) of scenario.


--------------------------------------------------
6. Sketch information
--------------------------------------------------

Customized logging:

Add the following rows in your sketch with suitable log-text according to your specific sketch.

Example:
// SKETCH_NAME           My_sketch_name
// PINMODE_OUT        2  "your log text for pin 2"
// PINMODE_IN         4  "your log text for pin 4"
// DIGITALWRITE_LOW  11  "your log text for pin 11 when HIGH"
// DIGITALWRITE_HIGH 11  "your log text for pin 11 when LOW"
// ANALOGREAD         1  "your log text for pin 1"
// DIGITALREAD       10  "your log text for pin 10"
// ANALOGWRITE       14  "your log text for pin 14"

--------------------------------------------------
7. Configuration
--------------------------------------------------

The following parameters are configured in the file, config.txt :

LOG_LEVEL  2		 // Level of information to be displayed during simulation (0,1,2,3)
DELAY    100		 // Delay in ms between each step/instruction  (0 - 1000)
LOG_FILE   0		 // Save the log information to the file: log.txt (YES=1,NO=0)
--------------------------------------------------
End of README
--------------------------------------------------
