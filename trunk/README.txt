==================================================
SIMUINO is a Arduino Simulator. Simple and basic. 
Code: c/c++
Platform: Linux (Ubuntu)0
Version: 0.5

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

4. Run: ./simuino 

5. Press "r" to run one loop() sequence. Can be repeated.

5. Quit:  press "q"


Test your own sketch in simuino:

1. Copy your sketch (source c-file) to directory/file "simuino/sketch/sketch.pde"

2. Compile "g++ -O2 -o simuino simuino.c -lncurses"

3. Run: ./simuino    
   If your sketch code contains a row:  // simuino: <your_application_name>
   the name of your application is displayed in the middle of the board.


--------------------------------------------------
2. Commands
--------------------------------------------------
Available commands:

q    quit
r    run one loop() sequence
s    step one instruction
g    run complete scenario, according to the content in scenario/digitalPins.txt and analogPins.txt


--------------------------------------------------
3. Input
-------------------------------------------------- 

In order to be able to run specific scenarios, controlled by values read from pins,
it is possible to specify what value shall be read per loop and pin.

The files, analogPins.txt and digitalPins.txt are located in the subdirectory "scenario".

The content of analogPins.txt:
----------------------------------
1 100 101 102 103 104 100
2 200 101 102 103 104  70
3 300 101 102 103 104  80
4 400 101 102 103 104  75
5 500 101 102 103 104 120
6 600 101 102 103 104  40
7 700 101 102 103 104 100
8 800 101 102 103 104  50
9 900 101 102 103 104 106
10 1000 101 102 103 104 105
----------------------------------
The first column specifies the loop number (in this example the scenario runs the loop from 1 to 10). 
The other columns is the value to be read for pins A0 - A5.

The content of digitalPins.txt:
----------------------------------
1  1 0 1 0 1 0 1 0 1 0 1 0 1 0
2  0 1 0 1 0 1 0 1 0 1 0 1 0 1
3  1 0 0 0 1 0 1 0 1 0 1 0 1 0
4  0 1 0 1 0 1 0 1 0 1 0 1 0 1
5  1 0 0 0 1 0 1 0 1 0 1 0 1 0
6  0 1 0 1 0 1 0 1 0 1 0 1 0 1
7  1 0 0 0 1 0 1 0 1 0 1 0 1 0
8  0 1 0 1 0 1 0 1 0 1 0 1 0 1
9  1 0 0 0 1 0 1 0 1 0 1 0 1 0
10 0 1 0 1 0 1 0 1 0 1 0 1 0 1
11 1 0 0 0 1 0 1 0 1 0 1 0 1 0
12 0 1 0 1 0 1 0 1 0 1 0 1 0 1
13 1 0 0 0 1 0 1 0 1 0 1 0 1 0
14 0 1 0 1 0 1 0 1 0 1 0 1 0 1
15 1 0 0 0 1 0 1 0 1 0 1 0 1 0
16 0 1 0 1 0 1 0 1 0 1 0 1 0 1
----------------------------------
The first column specifies the loop number. 
The other columns is the value to be read for pins D0 - A13.


Using the examples om the pindata files above, the command g will run the loop 16 times. The values of the analog pins will be zero for loop number 11- 16.

--------------------------------------------------
4. Display
--------------------------------------------------
The SIMUNIO displayes 4 windows:

 - The Arduino board with the pin-layout
 - Serial Interface
 - Logs
 - Commands

4.1  Arduino board Window
     
     For each pin the following data will be displayed:
	 Input or Output pin (in/out)
	 Reading or writing  (r/w)
	 Digital HIGH or LOW value (5 or 0) and values of analog pin (0 - 1023)

4.2  Serial Interface

     Anything printed on this interface will be shown here.

4.3  Logs 

     Most functions in the Arduino reference library will be displayed.
     Every line has two index: loop number, pseudo-time-elapsed.

4.4  Command

     Display the command entered.

--------------------------------------------------
End of README
--------------------------------------------------
