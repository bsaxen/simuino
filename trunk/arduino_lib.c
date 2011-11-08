//================================================
//  Developed by Benny Saxen, ADCAJO
//================================================

//------ Constants -------------------------
#define LOW    0
#define HIGH   5
#define INPUT  1
#define OUTPUT 2

#define BYTE   1
#define BIN    2
#define OCT    3
#define DEC    4
#define HEX    5


#define CHANGE  2
#define RISING  3
#define FALLING 4

//=====================================
// Functions
//=====================================


//------ Digital I/O -----------------------

void pinMode(int pin,int mode)
{

  if(mode == INPUT || mode == OUTPUT || mode == PWM)
    {
      digitalMode[pin] = mode;
      wmove(uno,DP-1,digPinPos[pin]);
      waddch(uno,ACS_VLINE);
      wmove(uno,DP-2,digPinPos[pin]-1);
      if(mode==INPUT)
	{
	  wprintw(uno,"In");
	  if(logging==YES)wLog("pinMode IN",pin,-1);
	}
      if(mode==OUTPUT)
	{
	  wprintw(uno,"Out");
	  if(logging==YES)wLog("pinMode OUT",pin,-1);
	}
      if(mode==PWM)
	{
	  wprintw(uno,"PWM");
	  if(logging==YES)wLog("pinMode PWM",pin,-1);
	}
      show(uno);
    }
}

void digitalWrite(int pin,int value)
{

  passTime();
  if(digitalMode[pin] == OUTPUT)
    {

      digitalPin[nloop][pin] = value;

      wmove(uno,DP,digPinPos[pin]);
      if(value==HIGH)
	{
	  waddch(uno,ACS_DIAMOND);
	  if(logging==YES)wLog("digitalWrite HIGH",pin,-1);
	}
      if(value==LOW)
	{
	  waddch(uno,ACS_BULLET);
	  if(logging==YES)wLog("digitalWrite LOW",pin,-1);
	}
      wmove(uno,DP+2,digPinPos[pin]);
      wprintw(uno,"w");
      show(uno);
      stepCommand(1);
      wmove(uno,DP,digPinPos[pin]);
      wprintw(uno,"%1d",value);
      wmove(uno,DP+2,digPinPos[pin]);
      wprintw(uno," ");
      show(uno);
    }
  else
    {
      wmove(uno,ER,RF);
      wprintw(uno,"Error: Wrong pin=%d mode. Should be OUTPUT\n",pin);
      show(uno);
      stepCommand(2);
    }
}

int digitalRead(int pin)
{
  int value=0;

  passTime();
  if(digitalMode[pin] == INPUT)
    {
      value = digitalPin[nloop][pin];
 
      wmove(uno,DP+2,digPinPos[pin]);
      wprintw(uno,"r");
      show(uno);
      if(logging==YES)wLog("digitalRead",pin,value);
      stepCommand(3);
      wmove(uno,DP,digPinPos[pin]);
      wprintw(uno,"%1d",value);
      wmove(uno,DP+2,digPinPos[pin]);
      wprintw(uno," ");
      show(uno);
    }
  else
    {
      move(ER,RF);
      printw("Error: Wrong pin=%d mode. Should be INPUT\n",pin);
      show(uno);
      stepCommand(4);
    }
  return(value);
}

//------ Analog I/O ------------------------

void analogReference(char type[])
{
  unimplemented("analogReference()");
  //DEFAULT, INTERNAL, INTERNAL1V1, INTERNAL2V56, or EXTERNAL
}

int analogRead(int pin)  // Values 0 to 1023
{

  int value;

  passTime();
  value = analogPin[nloop][pin];

  if(value > 1023 || value < 0)
    {
      move(ER,RF);
      printw("Error: Analog pin=%d value out of range =%d\n",pin,value);
      show(uno);
    }
  
  wmove(uno,AP,anaPinPos[pin]-3);
  wprintw(uno,"%4d",value);
  wmove(uno,AP-2,anaPinPos[pin]);
  wprintw(uno,"r");
  show(uno);
  if(logging==YES)wLog("analogRead",pin,value);
  stepCommand(5);
  wmove(uno,AP-2,anaPinPos[pin]);
  wprintw(uno," ");

  wmove(uno,AP+1,anaPinPos[pin]);
  waddch(uno,ACS_VLINE);
  wmove(uno,AP+2,anaPinPos[pin]-1);
  wprintw(uno,"In");

  show(uno);
  return(value); 
}

void analogWrite(int pin,int value) 
// Values 0 to 255   PWM: only pin 3,5,6,9,10,11
{
  passTime();
  if(pin==3 || pin==5 || pin==6 || pin==9 || pin==10 || pin==11)
    {

      digitalPin[nloop][pin] = value;

      wmove(uno,DP,digPinPos[pin]-2);
      wprintw(uno,"%3d",value);
      wmove(uno,DP+2,digPinPos[pin]);
      wprintw(uno,"a");
      show(uno);
      if(logging==YES)wLog("analogWrite",pin,value);
      stepCommand(6);
      wmove(uno,DP+2,digPinPos[pin]);
      wprintw(uno," ");
      show(uno);
    }
  else
    {
      move(ER,RF);
      printw("Error: Pin=%d not PWM\n",pin);
      show(uno);
      stepCommand(7);
    }
}

//------ Advanced I/O ----------------------
void tone(int pin, unsigned int freq)
{
  unimplemented("tone()");
}

void tone(int pin, unsigned int freq, unsigned long duration)
{
  unimplemented("tone()");
}

void noTone(int pin)
{
  unimplemented("noTone()");
}

void shiftOut(int dataPin, int clockPin, int bitOrder, int value)
{
  //bitOrder: which order to shift out the bits; either MSBFIRST or LSBFIRST.
  unimplemented("shiftOut()");
}

unsigned long pulseIn(int pin, int value)
{
  int res=0;
  res = pin*1000;
  return(res);
}

unsigned long pulseIn(int pin, int value, unsigned long timeout)
{
  int res=0;
  res = pin*1000;
  return(res);
}

//------ Time ------------------------------

unsigned long millis()
{
  unimplemented("millis()");
}

unsigned long micros()
{
  unimplemented("micros()");
}

void delay(int ms)
{
  passTime(); 
  if(logging==YES)wLog("delay",ms,-1);
  msleep(ms);
  stepCommand(8);
}

void delayMicroseconds(int us)
{
  passTime();
  if(logging==YES)wLog("delayMicroseconds",us,-1);
  msleep(us);
  stepCommand(9);
}

//------ Math ------------------------------

int map(int x, int fromLow, int fromHigh, int toLow, int toHigh)
{
  int y;
  y = (float)(x-fromLow)/(fromHigh - fromLow)*(toHigh - toLow) + toLow;
  //printf("%d %d\n",x,y);
  return(y);
}

int constrain(int x, int a, int b)
{
  int res;
  if(x<=b && x>=a)res = x;
  if(x>b)res = b;
  if(x<a)res = a;
  return(res);
}

//------ Trigonometry ----------------------
void test1()
{
  double x;
  x = sin(1);
  x = cos(1);
  x = tan(1);
}
//------ Random Numbers --------------------
void randomSeed(int seed)
{
  unimplemented("randomSeed()");
}

void random(int max)
{
  unimplemented("random(1)");
}

void random(int min, int max)
{
  unimplemented("random(2)");
}


//------ Bits and Bytes --------------------
unsigned char lowByte(unsigned char x)
{
  unimplemented("lowByte()");
}

unsigned char highByte(unsigned char x)
{
  unimplemented("lowByte()");
}

unsigned char bitRead(unsigned char x)
{
  unimplemented("bitRead()");
}

unsigned char bitWrite(unsigned char x)
{
  unimplemented("bitWrite()");
}

unsigned char bitSet(unsigned char x)
{
  unimplemented("bitSet()");
}

unsigned char bitClear(unsigned char x)
{
  unimplemented("bitClear()");
}

unsigned char bit(unsigned char x)
{
  unimplemented("bit()");
}

//------ External Interrupts ---------------


void attachInterrupt(int interrupt,void(*func)(),int mode)
{
  unimplemented("attachInterrupt()");
}

//---------------------------------------------------
void detachInterrupt(int interrupt)
{
  unimplemented("detachInterrupt()");
}

//------ Interrupts ------------------------
void interrupts()
{
  unimplemented("interrupts()");
}
void noInterrupts()
{
  unimplemented("noInterrupts()");
}
//------ Communication ---------------------
class serial {

 public:
  void begin(int baudRate);
  void end();
  int  available();
  int  read();
  int  peek();
  void flush();
  void print(int x);
  void print(int x,int base);
  void print(const char *p);
  void println(int x);
  void println(const char *p);
  void write(char *p);
};

serial Serial;

void serial::begin(int baudRate) 
{
  baud = baudRate;
  digitalMode[0] = RX;
  digitalMode[1] = TX;
  serialMode = ON;
}

void serial::end() 
{
  digitalMode[0] = FREE;
  digitalMode[1] = FREE;
  serialMode = OFF;
}

int serial::available()  // returns the number of bytes available to read
{
  return(1);
}

int serial::read() // the first byte of incoming serial data available (or -1 if no data is available)
{
  return(-1);
}

int serial::peek() 
{
   return(-1);
}

void serial::flush() 
{
  strcpy(serialBuffer,"flush");
  showSerial();
}

void serial::print(int x) 
{
  passTime();
  if(logging==YES)wLog("serial:print",x,-1);
  sprintf(stemp,"%d",x);
  strcat(serialBuffer,stemp);
  showSerial();
}

void serial::print(int x,int base) 
{
  passTime();
  if(logging==YES)wLog("serial:print base",x,-1);
  sprintf(stemp,"%d",x);
  strcat(serialBuffer,stemp);
  showSerial();
}

void serial::print(const char *p) 
{
  passTime();
  if(logging==YES)wLogChar("serial:print",p,-1);
  sprintf(stemp,"%s",p);
  strcat(serialBuffer,stemp);
  showSerial();
}

void serial::println(int x) 
{
  passTime();
  if(logging==YES)wLog("serial:println",x,-1);
  sprintf(stemp,"%d\n",x);
  strcat(serialBuffer,stemp);
  showSerial();
}

void serial::println(const char *p) 
{
  passTime();
  if(logging==YES)wLogChar("serial:println",p,-1);
  sprintf(stemp,"%s\n",p);
  strcat(serialBuffer,stemp);
  showSerial();
}

void serial::write(char *p) 
{
   passTime();
   if(logging==YES)wLogChar("serial:write",p,-1);
   sprintf(stemp,"%s\n",p);
   strcat(serialBuffer,stemp);
   showSerial();
}

//======================================================
// Ethernet Library
//======================================================

/* class Ethernet { */
/*  public: */
/*   void begin(int baudRate); */
/* }; */

/* void Ethernet::begin(char *mac,char *ip)  */
/* { */
/*   if(logging==YES)wLog("Ethernet.begin",-1,-1); */
/* } */
/* void Ethernet::begin(char *mac,char *ip,char *gateway)  */
/* { */
/*   if(logging==YES)wLog("Ethernet.begin",-1,-1); */
/* } */
/* void Ethernet::begin(char *mac,char *ip,char *gateway, char *subnet)  */
/* { */
/*   if(logging==YES)wLog("Ethernet.begin",-1,-1); */
/* } */

/* class Server { */
/*  public: */
/* begin() */
/* available() */
/* write() */
/* print() */
/* println() */
/* }; */

/* void Server::Server(int port)  */
/* { */
/*   if(logging==YES)wLog("Server.Server",port,-1); */
/* } */
/* void Server::begin()  */
/* { */
/*   if(logging==YES)wLog("Server.begin",-1,-1); */
/* } */
/* Client Server::available()  */
/* { */
/*   Client x = Client(); */
/*   if(logging==YES)wLog("Server.available",-1,-1); */
/*   return(x),  */
/* } */
/* void Server::write(char x)  */
/* { */
/*   if(logging==YES)wLog("Server.write",-1,-1); */
/*   sprintf(stemp,"%c",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::write(int x)  */
/* { */
/*   if(logging==YES)wLog("Server.write",-1,-1); */
/*   sprintf(stemp,"%d",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::print(char x)  */
/* { */
/*   if(logging==YES)wLog("Server.print",-1,-1); */
/*   sprintf(stemp,"%c",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::print(int x)  */
/* { */
/*   if(logging==YES)wLog("Server.print",-1,-1); */
/*   sprintf(stemp,"%d",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::print(char *x)  */
/* { */
/*   if(logging==YES)wLog("Server.print",-1,-1); */
/*   sprintf(stemp,"%s",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::print(char x,char *base)  */
/* { */
/*   //BIN for binary (base 2)  */
/*   //DEC for decimal (base 10) */
/*   //OCT for octal (base 8) */
/*   //HEX for hexadecimal (base 16) */
/*   if(logging==YES)wLog("Server.print",-1,-1); */
/*   sprintf(stemp,"%c",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::print(int x,char *base)  */
/* { */
/*   if(logging==YES)wLog("Server.print",-1,-1); */
/*   sprintf(stemp,"%d",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::print(char *x,char *base)  */
/* { */
/*   if(logging==YES)wLog("Server.print",-1,-1); */
/*   sprintf(stemp,"%s",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::println()  */
/* { */
/*   if(logging==YES)wLog("Server.print",-1,-1); */
/*   sprintf(stemp,"\n"); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::println(char x)  */
/* { */
/*   if(logging==YES)wLog("Server.print",-1,-1); */
/*   sprintf(stemp,"%c\n",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::println(int x)  */
/* { */
/*   if(logging==YES)wLog("Server.print",-1,-1); */
/*   sprintf(stemp,"%d\n",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::println(char *x)  */
/* { */
/*   if(logging==YES)wLog("Server.print",-1,-1); */
/*   sprintf(stemp,"%s\n",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::println(char x,char *base)  */
/* { */
/*   if(logging==YES)wLog("Server.print",-1,-1); */
/*   sprintf(stemp,"%c\n",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::println(int x,char *base)  */
/* { */
/*   if(logging==YES)wLog("Server.print",-1,-1); */
/*   sprintf(stemp,"%d\n",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::println(char *x,char *base)  */
/* { */
/*   if(logging==YES)wLog("Server.print",-1,-1); */
/*   sprintf(stemp,"%s\n",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */

/* class Client { */
/*  public: */
/*   void begin(int baudRate); */
/* connected() */
/* connect() */
/* write() */
/* print() */
/* println() */
/* available() */
/* read() */
/* flush() */
/* stop() */
/* }; */

/* void serial::print(int x)  */
/* { */
/*   if(logging==YES)wLog("serial print",x,-1); */
/*   sprintf(stemp,"%d",x); */
/*   strcat(serialBuffer,stemp); */
/*   showSerial(); */
/*   passTime(); */
/* } */
