SIMUINO is a Arduino Simulator. Simple and basic. 
Code: c/c++
Platform: Linux (Ubuntu)

Quick starter guide:

1. Download simuino zip-file from www.vdaon.com

2. Unzip the file. A directory named simuino will be created.

3. Go to directory simuino. Compile: g++ -O2 -o simuino simuino.c -lncurses

4. Run: ./simuino 

5. Press "r" to run one loop sequence. Can be repeated.

5. Quit:  press "q"


Test your own sketch in simuino:

1. Copy your sketch (source c-file) to directory/file "simuino/sketch/sketch.pde"

2. Compile "g++ -O2 -o simuino simuino.c -lncurses"

3. Run: ./simuino    
   If your sketch code contains a row:  // simuino: <your_application_name>
   the name of your application is displayed in the middle of the board.

Available commands:

q	  quit
r	  run one loop() sequence
s	  select pin
+	  increase pin value
-	  decrease pin value





