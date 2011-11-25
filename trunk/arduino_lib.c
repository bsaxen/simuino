//================================================
//  Developed by Benny Saxen, ADCAJO
//================================================

#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define A4 4
#define A5 5

#define byte int



// Math function min and max
#ifndef max
	#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif

#ifndef min
	#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif
  
char  stemp[80];

//typedef std::string String;

//=====================================
// Functions
//=====================================


//------ Digital I/O -----------------------

void pinMode(int pin,int mode)
{
  char temp[80];
  
  if(mode == INPUT || mode == OUTPUT)
    {
      digitalMode[pin] = mode;
      wmove(uno,DP-1,digPinPos[pin]);
      waddch(uno,ACS_VLINE);
      wmove(uno,DP-2,digPinPos[pin]-1);

      if(mode==INPUT)
	{
	  strcpy(temp,textPinModeIn[pin]);
	  wprintw(uno,"In");
	  if(confLogLev > 1)
	    {
	      if(strstr(temp,"void"))
		wLog("pinMode IN",pin,-1);
	      else
		wLog(temp,pin,-1);
	    }
	}

      if(mode==OUTPUT)
	{
	  strcpy(temp,textPinModeOut[pin]);
	  wprintw(uno,"Out");
	  if(confLogLev > 1)
	    {
	      if(strstr(temp,"void"))
		wLog("pinMode OUT",pin,-1);
	      else
		wLog(temp,pin,-1);
	    }
	}

      show(uno);
    }
  else
    {
      showError("Unknown Pin Mode",mode);
      wLog("pinMode ",pin,-1);
    }
}

void digitalWrite(int pin,int value)
{
  char temp[80];
  passTime();
  if(digitalMode[pin] == OUTPUT)
    {
      s_digitalPin[nloop][pin] = value;

      wmove(uno,DP,digPinPos[pin]);
      if(value==HIGH)
	{
	  strcpy(temp,textDigitalWriteHigh[pin]);
	  waddch(uno,ACS_DIAMOND);
	  if(confLogLev > 0)
	    { 
	      if(strstr(temp,"void"))
		wLog("digitalWrite HIGH",pin,-1);
	      else
		wLog(temp,pin,-1);
	    }
	}
      if(value==LOW)
	{
	  strcpy(temp,textDigitalWriteLow[pin]);
	  waddch(uno,ACS_BULLET);
	  if(confLogLev > 0)
	    {
	      if(strstr(temp,"void"))
		wLog("digitalWrite LOW",pin,-1);
	      else
		wLog(temp,pin,-1);
	    }
	}
      wmove(uno,DP+2,digPinPos[pin]);
      wprintw(uno,"w");
      show(uno);
      stepCommand();
      wmove(uno,DP,digPinPos[pin]-2);
      wprintw(uno,"%3d",value);
      wmove(uno,DP+2,digPinPos[pin]);
      wprintw(uno," ");
      show(uno);
    }
  else
    {
      showError("Wrong pin mode. Should be OUTPUT",pin);
      wLog("digitalWrite",pin,-1);
      stepCommand();
    }
}

int digitalRead(int pin)
{
  int value=0;
  char temp[80];

  passTime();
  if(digitalMode[pin] == INPUT)
    {
      value = getDigitalPinValue(pin,timeFromStart);
 
      wmove(uno,DP+2,digPinPos[pin]);
      wprintw(uno,"r");
      show(uno);

      strcpy(temp,textDigitalRead[pin]);
      if(confLogLev > 0)
	{
	  if(strstr(temp,"void"))
	    wLog("digitalRead",pin,value);
	  else
	    wLog(temp,pin,value);
	}

      stepCommand();
      wmove(uno,DP,digPinPos[pin]);
      wprintw(uno,"%1d",value);
      wmove(uno,DP+2,digPinPos[pin]);
      wprintw(uno," ");
      show(uno);
    }
  else
    {
      showError("Wrong pin mode. Should be INPUT",pin);
      wLog("digitalRead",pin,value);
      stepCommand();
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
  char temp[80];

  passTime();
  value = getAnalogPinValue(pin,timeFromStart);

  if(value > 1023 || value < 0)
    {
      sprintf(temp,"%d Analog pin=%d value out of range = %d",timeFromStart,pin,value);
      showError(temp,-1);
      value = 0;
    }
  
  wmove(uno,AP,anaPinPos[pin]-3);
  wprintw(uno,"%4d",value);
  wmove(uno,AP-2,anaPinPos[pin]);
  wprintw(uno,"r");
  show(uno);

  strcpy(temp,textAnalogRead[pin]);
  if(confLogLev > 0)
    {
      if(strstr(temp,"void"))
	wLog("analogRead",pin,value);
      else
	wLog(temp,pin,value);
    }

  stepCommand();
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
  char temp[80];

  passTime();

  if(digitalMode[pin] != OUTPUT)
    {
      showError("Pin is not in OUPUT mode: ",pin);
      wLog("analogWrite",pin,value);
      stepCommand();
      return;
    }

  if(pin==3 || pin==5 || pin==6 || pin==9 || pin==10 || pin==11)
    {

      if(value > 256 || value < 0)
	{
	  sprintf(temp,"%d AnalogWrite pin=%d value out of range = %d",timeFromStart,pin,value);
	  showError(temp,-1);
	  value = 0;
	}
      
      s_digitalPin[nloop][pin] = value;

      wmove(uno,DP,digPinPos[pin]-2);
      wprintw(uno,"%3d",value);
      wmove(uno,DP+2,digPinPos[pin]);
      wprintw(uno,"a");
      show(uno);

      strcpy(temp,textAnalogWrite[pin]);
      if(confLogLev > 0)
	{
	  if(strstr(temp,"void"))
	    wLog("analogWrite",pin,value);
	  else
	    wLog(temp,pin,value);
	}
      stepCommand();
      wmove(uno,DP+2,digPinPos[pin]);
      wprintw(uno," ");
      show(uno);
    }
  else
    {
      showError("Pin is not of PWM type",pin);
      wLog("analogWrite",pin,value);
      stepCommand();
    }
  return;
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
  unimplemented("pulseIn()");
}

unsigned long pulseIn(int pin, int value, unsigned long timeout)
{
  unimplemented("pulseIn()");
}

//------ Time ------------------------------

unsigned long millis()
{
  //unimplemented("millis()");
  return(timeFromStart*100);
}

unsigned long micros()
{
  //unimplemented("micros()");
  return(timeFromStart*100000);
}

void delay(int ms)
{
  passTime(); 
  if(confLogLev > 1)wLog("delay",ms,-1);
  msleep(ms);
  stepCommand();
}

void delayMicroseconds(int us)
{
  passTime();
  if(confLogLev > 1)wLog("delayMicroseconds",us,-1);
  msleep(us);
  stepCommand();
}

//------ Math ------------------------------
void test_math()
{
  double r,x,z;
  int y;
  y = min(1,2);
  y = max(1,2);
  y = abs(1);
  r = pow(x,z);
  r = sqrt(y);
}


double sq(double x)
{
  return(sqrt(x));
}

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
void test_trigonometry()
{
  double x;
  x = sin(1);
  x = cos(1);
  x = tan(1);
}
//------ Random Numbers --------------------
void randomSeed(int seed)
{
  srand(seed);
}

long random(long upperLimit)
{
  long x = RAND_MAX/upperLimit;
  x = long(rand()/x);
  return(x);
}

long random(long lowerLimit, long upperLimit)
{
  long interval, temp=0;
  if (lowerLimit<upperLimit) 
    { 
      interval = upperLimit - lowerLimit;
      temp = lowerLimit + random(interval);
    }
  return(temp);
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

  interruptMode[interrupt] = mode;

  if(interrupt == 0)
    {
      interrupt0 = func;
    }
  if(interrupt == 1)
    {
      interrupt1 = func;
    }

  if(interrupt != 0 && interrupt != 1)
    showError("Unsupported interrupt number",interrupt);

  if(confLogLev > 0)wLog("attachInterrupt",interrupt,-1);

}

//---------------------------------------------------
void detachInterrupt(int interrupt)
{

  if(interrupt == 0)
    {
      interrupt0 = NULL;
    }
  if(interrupt == 1)
    {
      interrupt1 = NULL;
    }
  
  if(interrupt != 0 && interrupt != 1)
    showError("Unsupported interrupt number",interrupt);
    
  if(confLogLev > 0)wLog("detachInterrupt",interrupt,-1);
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
  void println();
  void println(int x);
  void println(int x,int base);
  void println(const char *p);
  void write(int p);
  void write(char *p);
  void write(char *p, int len);
};

serial Serial,Serial1,Serial2,Serial3;

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
  unimplemented("Serial.available");
  return(1);
}

int serial::read() // the first byte of incoming serial data available (or -1 if no data is available)
{
  unimplemented("Serial.read");
  return(-1);
}

int serial::peek() 
{
   return(-1);
}

void serial::flush() 
{
  showSerial("flush",1);
}

void serial::print(int x) 
{
  passTime();
  sprintf(stemp,"%d",x);
  showSerial(stemp,0);
  if(confLogLev > 2)
    {
      wLog("serial:print",x,-1);
      stepCommand();
    }
}

void serial::print(int x,int base) 
{
  passTime();
  sprintf(stemp,"%d",x);
  showSerial(stemp,0);
  if(confLogLev > 2)
    {
      wLog("serial:print base",x,-1);
      stepCommand();
    }
}

void serial::print(const char *p) 
{
  passTime();
  sprintf(stemp,"%s",p);
  showSerial(stemp,0);
  if(confLogLev > 2)
    {
      wLogChar("serial:print",p,-1);
      stepCommand();
    }
}

void serial::println() 
{
  passTime();
  //sprintf(stemp,"%d",x);
  showSerial(stemp,1);
  if(confLogLev > 2)
    {
      wLog("serial:println()",-1,-1);
      stepCommand();
    }
}

void serial::println(int x) 
{
  passTime();
  sprintf(stemp,"%d",x);
  showSerial(stemp,1);
  if(confLogLev > 2)
    {
      wLog("serial:println",x,-1);
      stepCommand();
    }
}

void serial::println(int x, int base) 
{
  passTime();
  sprintf(stemp,"%d",x);
  showSerial(stemp,1);
  if(confLogLev > 2)
    {
      wLog("serial:println base",x,-1);
      stepCommand();
    }
}

void serial::println(const char *p) 
{
  passTime();
  sprintf(stemp,"%s",p);
  showSerial(stemp,1);
  if(confLogLev > 2)
    {
      wLogChar("serial:println",p,-1);
      stepCommand();
    }
}

void serial::write(int p) 
{
  passTime();
  sprintf(stemp,"%d",p);
  showSerial(stemp,1);
  if(confLogLev > 2)
    {
      wLog("serial:write(val)",p,-1);
      stepCommand();
    }
}

void serial::write(char *p) 
{
  passTime();
  sprintf(stemp,"%s",p);
  showSerial(stemp,1);
  if(confLogLev > 2)
    {
      wLogChar("serial:write(str)",p,-1);
      stepCommand();
    }
}

void serial::write(char *p, int len) 
{
  passTime();
  sprintf(stemp,"%s",p);
  showSerial(stemp,1);
  if(confLogLev > 2)
    {
      wLogChar("serial:write(len)",p,-1);
      stepCommand();
    }
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
/*   if(confLogLev > 0)wLog("Ethernet.begin",-1,-1); */
/* } */
/* void Ethernet::begin(char *mac,char *ip,char *gateway)  */
/* { */
/*   if(confLogLev > 0)wLog("Ethernet.begin",-1,-1); */
/* } */
/* void Ethernet::begin(char *mac,char *ip,char *gateway, char *subnet)  */
/* { */
/*   if(confLogLev > 0)wLog("Ethernet.begin",-1,-1); */
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
/*   if(confLogLev > 0)wLog("Server.Server",port,-1); */
/* } */
/* void Server::begin()  */
/* { */
/*   if(confLogLev > 0)wLog("Server.begin",-1,-1); */
/* } */
/* Client Server::available()  */
/* { */
/*   Client x = Client(); */
/*   if(confLogLev > 0)wLog("Server.available",-1,-1); */
/*   return(x),  */
/* } */
/* void Server::write(char x)  */
/* { */
/*   if(confLogLev > 0)wLog("Server.write",-1,-1); */
/*   sprintf(stemp,"%c",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::write(int x)  */
/* { */
/*   if(confLogLev > 0)wLog("Server.write",-1,-1); */
/*   sprintf(stemp,"%d",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::print(char x)  */
/* { */
/*   if(confLogLev > 0)wLog("Server.print",-1,-1); */
/*   sprintf(stemp,"%c",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::print(int x)  */
/* { */
/*   if(confLogLev > 0)wLog("Server.print",-1,-1); */
/*   sprintf(stemp,"%d",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::print(char *x)  */
/* { */
/*   if(confLogLev > 0)wLog("Server.print",-1,-1); */
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
/*   if(confLogLev > 0)wLog("Server.print",-1,-1); */
/*   sprintf(stemp,"%c",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::print(int x,char *base)  */
/* { */
/*   if(confLogLev > 0)wLog("Server.print",-1,-1); */
/*   sprintf(stemp,"%d",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::print(char *x,char *base)  */
/* { */
/*   if(confLogLev > 0)wLog("Server.print",-1,-1); */
/*   sprintf(stemp,"%s",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::println()  */
/* { */
/*   if(confLogLev > 0)wLog("Server.print",-1,-1); */
/*   sprintf(stemp,"\n"); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::println(char x)  */
/* { */
/*   if(confLogLev > 0)wLog("Server.print",-1,-1); */
/*   sprintf(stemp,"%c\n",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::println(int x)  */
/* { */
/*   if(confLogLev > 0)wLog("Server.print",-1,-1); */
/*   sprintf(stemp,"%d\n",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::println(char *x)  */
/* { */
/*   if(confLogLev > 0)wLog("Server.print",-1,-1); */
/*   sprintf(stemp,"%s\n",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::println(char x,char *base)  */
/* { */
/*   if(confLogLev > 0)wLog("Server.print",-1,-1); */
/*   sprintf(stemp,"%c\n",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::println(int x,char *base)  */
/* { */
/*   if(confLogLev > 0)wLog("Server.print",-1,-1); */
/*   sprintf(stemp,"%d\n",x); */
/*   strcat(ethernetBuffer,stemp); */
/*   showEthernet(); */
/*   passTime(); */
/* } */
/* void Server::println(char *x,char *base)  */
/* { */
/*   if(confLogLev > 0)wLog("Server.print",-1,-1); */
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
/*   if(confLogLev > 0)wLog("serial print",x,-1); */
/*   sprintf(stemp,"%d",x); */
/*   strcat(serialBuffer,stemp); */
/*   showSerial(); */
/*   passTime(); */
/* } */

//====================================
// End of file
//====================================
