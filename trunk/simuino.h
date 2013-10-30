/*  Simuino is a Arduino Simulator based on Servuino Engine
    Copyright (C) 2011  Benny Saxen

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
void anyErrors();
int  countRowsInFile(char *fileName);
int  analyzeEvent(char *Event);
int  readEvent(char *ev, int step);
int  readComment(int step);
void mLog0(const char *p);
void wLog0(const char *p);
void mLog1(const char *p, int value1);
void wLog1(const char *p, int value1);
void wLog2(const char *p, int value1, int value2);
void debug(char *msg);
void runMode(int stop);
int  runStep(int dir);
void openCommand();
void iDelay(int ms);
void show(WINDOW *win);
void putMsg(int line,const char *message);
void showError(const char *m, int value);
void saveSetting();
void resetFile(const char *filename);
void logFile(char *m);
void wLog(const char *p, int value1, int value2);
void unoInfo();
void wLogChar(const char *p, const char *value1, int value2);
void showSerial(const char *m, int newLine);
void getString(char *in, char *out);
void readSketchInfo();
void initSim();
void resetSim();
void unimplemented(const char *f);
void readConfig();
void runLoop();
void runAll();
void readSimulation();
void showScenario(char *fileName);
void readMsg(char *fileName);
void init();
int  loadSketch(char sketch[]);
void pinMode(int pin,int mode);
void digitalWrite(int pin,int value);
void analogReference(char type[]);
void analogWrite(int pin,int value); 
void tone(int pin, unsigned int freq);
void tone(int pin, unsigned int freq, unsigned long duration);
void noTone(int pin);
void shiftOut(int dataPin, int clockPin, int bitOrder, int value);
void delay(int ms);
void delayMicroseconds(int us);
void test_math();
void test_trigonometry();
void randomSeed(int seed);
void attachInterrupt(int interrupt,void(*func)(),int mode);
void detachInterrupt(int interrupt);
void interrupts();
void noInterrupts();
int  readStatus();
void readSerial();
void readTime();
int  goStep(int step);
char *replace_str(char *str, char orig[], char rep[]);
