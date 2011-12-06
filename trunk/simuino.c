//================================================
//  Developed by Benny Saxen, ADCAJO
//================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <math.h> 
#include <ctype.h>
#include <termios.h>
#include <unistd.h>
#include <ncurses.h>
#include <sys/stat.h>
#include <form.h>

#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define A4 4
#define A5 5


#define LOW    0
#define HIGH   1
#define INPUT  1
#define OUTPUT 2

#define FORWARD  1
#define BACKWARD 2

#define BYTE   1
#define BIN    2
#define OCT    3
#define DEC    4
#define HEX    5

#define CHANGE  1
#define RISING  2
#define FALLING 3
#define MAX_SERIAL 100
#define MAX_LOG  200
#define MAX_STEP 2000
#define MAX_LOOP 2000
#define MAX_PIN_ANALOG 6
#define MAX_PIN_DIGITAL 14

#define SIZE_ROW 180

#define UNO_H  16
#define UNO_W  61
#define UNO_COLOR 6

#define MSG_H  20
#define MSG_W  61
#define MSG_COLOR 6

#define LOG_H  40
#define LOG_W  40
#define LOG_COLOR 3

#define SER_H  40
#define SER_W  30
#define SER_COLOR 4

#define WIN_MODES 4

#define DP 5
#define AP 11
#define RF 1
#define ER 1
#define SR 20

#define ON     1
#define OFF    0

#define YES    1
#define NO     2

#define RUN    1
#define ADMIN  2

#define FREE   0
#define RX     3
#define TX     4


// Current data
int  currentStep = 0;
int  currentLoop = 0;
char currentConf[SIZE_ROW];

// Limits
int  g_loops = 0;
int  g_steps = 0;
int  g_comments = 0;

int g_value = 0;
int s_mode = ADMIN;
int g_warning = YES;


void (*interrupt0)();
void (*interrupt1)();

char  simulation[MAX_STEP][SIZE_ROW];
char  simComment[MAX_STEP][SIZE_ROW];
int   stepComment[MAX_STEP];
int   loopPos[MAX_LOOP];


int   s_row,s_col;
int   digPinPos[MAX_PIN_DIGITAL];
int   anaPinPos[MAX_PIN_ANALOG];
char  appName[120];

int   interruptMode[2];
int   digitalMode[MAX_PIN_DIGITAL];
int   paceMaker = 0;
int   baud = 0;
int   error = 0;

// Log
//int   logging = YES;
char  logBlankRow[500];

// Serial Interface
//int   serialSize = 1;
int   serialMode = OFF;
char  prevSerial[SIZE_ROW];
char  serBlankRow[500];

char  textPinModeIn[MAX_PIN_DIGITAL][SIZE_ROW];
char  textPinModeOut[MAX_PIN_DIGITAL][SIZE_ROW];
char  textDigitalWriteLow[MAX_PIN_DIGITAL][SIZE_ROW];
char  textDigitalWriteHigh[MAX_PIN_DIGITAL][SIZE_ROW];
char  textAnalogWrite[MAX_PIN_DIGITAL][SIZE_ROW];
char  textAnalogRead[MAX_PIN_ANALOG][SIZE_ROW];
char  textDigitalRead[MAX_PIN_DIGITAL][SIZE_ROW];
int   scenAnalog    = 0;
int   scenDigital   = 0;
int   scenInterrupt = 0;

// Configuration default values
char  listConf[80];
int   confSteps   = 1000;
int   confWinMode =    2;
int   confDelay   =  100;
int   confLogLev  =    1;
int   confLogFile =    0;
char  confSketchFile[200];
char  confServuinoFile[200];

char  workSketchFile[80];

int uno_h=0, uno_w=0, uno_x=0, uno_y=0;
int msg_h=0, msg_w=0, msg_x=0, msg_y=0;
int log_h=0, log_w=0, log_x=0, log_y=0;
int ser_h=0, ser_w=0, ser_x=0, ser_y=0;

WINDOW *uno,*ser,*slog,*msg;
static struct termios orig, nnew;
 
char  stemp[80];

#include "simuino.h"
#include "simuino_lib.c"
#include "decode_lib.c"

//====================================
void runStep(int dir)
//====================================
{
  char row[SIZE_ROW],event[SIZE_ROW],temp[SIZE_ROW],mode[5];
  char *p,temp2[SIZE_ROW],temp3[SIZE_ROW],comment[SIZE_ROW];
  int res1 = 0,res2 = 0,value = 0;
  int step = 0,pin,x,y;

  if(dir == FORWARD)currentStep++;
  if(dir == BACKWARD)currentStep--;

  if(currentStep > g_steps)return; 

  if(currentStep > loopPos[currentLoop+1] && loopPos[currentLoop+1] !=0)
    {
      currentLoop++;
    }

  res1 = readEvent(event,currentStep);

  if(res1 > 0)
    {
      if(p=strstr(event,"pinMode"))
	{
	  sscanf(event,"%d %s %s %d",&step,temp,mode,&pin);
	  if(strstr(mode,"IN"))pinMode(pin,INPUT);
	  if(strstr(mode,"OUT"))pinMode(pin,OUTPUT);
	}
      else if (p=strstr(event,"digitalRead"))
	{
	  sscanf(event,"%d %s %d %d",&step,temp,&pin,&value);
	  g_value = value;
	  digitalRead(pin);
	}
      else if (p=strstr(event,"digitalWrite"))
	{
	  sscanf(event,"%d %s %s %d",&step,temp,mode,&pin);
	  if(strstr(mode,"LOW"))digitalWrite(pin,LOW);
	  if(strstr(mode,"HIGH"))digitalWrite(pin,HIGH);
	}
      else if (p=strstr(event,"analogRead"))
	{
	  sscanf(event,"%d %s %d %d",&step,temp,&pin,&value);
	  g_value = value;
	  analogRead(pin);
	}
      else if (p=strstr(event,"analogWrite"))
	{
	  sscanf(event,"%d %s %d %d",&step,temp,&pin,&value);
	  analogWrite(pin,value);
	}
      else if (p=strstr(event,"Serial:begin"))
	{
	  sscanf(event,"%d %s %d",&step,temp,&x);
	  Serial.begin(x);
	}
      else if (p=strstr(event,"Serial:print(int)"))
	{
	  sscanf(event,"%d %s %d",&step,temp,&x);
	  Serial.print(x);
	}
      else if (p=strstr(event,"Serial:print(int,int)"))
	{
	  sscanf(event,"%d %s %d",&step,temp,&x,&y);
	  Serial.print(x,y);
	}
      else if (p=strstr(event,"Serial:print(char)"))
	{
	  getString(event,temp3);
	  Serial.print(temp3);
	}
      else if (p=strstr(event,"Serial:println(int)"))
	{
	  sscanf(event,"%d %s %d",&step,temp,&x);
	  Serial.println(x);
	}
      else if (p=strstr(event,"Serial:println(char)"))
	{
	  getString(event,temp3);
	  Serial.println(temp3);
	}
      else if (p=strstr(event,"delay()"))
	{
	  sscanf(event,"%d %s %d",&step,temp,&x);
	  delay(x);
	}
      else if (p=strstr(event,"delayMicroseconds"))
	{
	  sscanf(event,"%d %s %d",&step,temp,&x);
	  delayMicroseconds(x);
	}
      else if (p=strstr(event,"attachInterrupt"))
	{
	  sscanf(event,"%d %s %d",&step,temp,&x);
	  attachInterrupt(x,0,0);
	}
      else if (p=strstr(event,"detachInterrupt"))
	{
	  sscanf(event,"%d %s %d",&step,temp,&x);
	  detachInterrupt(x);
	}
      else if (p=strstr(event,"interruptRISING"))
	{
	  sscanf(event,"%d %s %d",&step,temp,&x);
	  interrupt(temp,x);
	}
      else if (p=strstr(event,"interruptFALLING"))
	{
	  sscanf(event,"%d %s %d",&step,temp,&x);
	  interrupt(temp,x);
	}
      else if (p=strstr(event,"interruptCHANGE"))
	{
	  sscanf(event,"%d %s %d",&step,temp,&x);
	  interrupt(temp,x);
	}
      else if (p=strstr(event,"interruptLOW"))
	{
	  sscanf(event,"%d %s %d",&step,temp,&x);
	  interrupt(temp,x);
	}
      else
	unimplemented(temp);

      readComment(currentStep);
      
      show(slog);
    }
  else
    showError("Unable to step ",currentStep);

  unoInfo();

  return;
}    
//====================================
int tokCommand(char res[40][40],char *inp)
//====================================
{
  char *pch;
  int count = 0;

  pch = strtok(inp," ");
  while (pch != NULL)
    {
      strcpy(res[count],pch);
      count++;
      pch = strtok(NULL, " ");
    }
  return(count);
}

//====================================
void loadCurrentProj()
//====================================
{
  int x;
  char syscom[120];

  g_warning = NO;
  putMsg(1,"Loading Proj...");
  loadSketch(confSketchFile);
  if(confSteps < 0) confSteps = 100;
  if(confSteps > MAX_STEP) confSteps = MAX_STEP-1;
  sprintf(syscom,"cd servuino;./servuino %d;",confSteps);
  x=system(syscom);
  iDelay(500);
  init(confWinMode);
  initSim();
  resetSim();
  readSimulation(confServuinoFile);
  readSketchInfo();
  unoInfo();
  putMsg(msg_h-2,"Loading ready!");
}

//====================================
void openCommand()
//====================================
{
  struct stat st;
  int ch,nsteps=1000,x,i,n,stop=0,loop,projNo = 0;
  char *p,str[120],sstr[20],fileName[120],temp[120],syscom[120];
  char command[40][40];

  s_mode = ADMIN;

  readMsg(listConf);

  while(strstr(str,"ex") == NULL)
    {
      wmove(uno,UNO_H-2,1);
      wprintw(uno,"                                                  ");
      mvwprintw(uno,UNO_H-2,1,"A%1d>",confWinMode);
      show(uno);
      wmove(uno,UNO_H-2,4);
      strcpy(command[0],"");
      wgetstr(uno,str);

      n = tokCommand(command,str);

      strcpy(sstr,command[0]);

      p = str;

      projNo = atoi(sstr);

      if(strstr(sstr,"run"))
	{
	  stop = 0;
          if(n == 2)stop = atoi(command[1]);
	  runMode(stop);
	}
      else if(strstr(sstr,"res")) // reset simulation
	{
	  resetSim();
	  init(confWinMode);
	  unoInfo();
	  readMsg(currentConf);
	}
      else if(strstr(sstr,"help")) //
	{
	  strcpy(fileName,"help_command.txt");
	  readMsg(fileName);
	}
      else if(strstr(sstr,"info"))
	{
	  if(n == 2)
	    {
	      if(strstr(command[1],"conf"))
		{
		  strcpy(fileName,"default.conf");
		  readMsg(fileName);
		}
	      else if(strstr(command[1],"err"))
		{
		  strcpy(fileName,"servuino/data.error");
		  readMsg(fileName);
		}
	      else if(strstr(command[1],"g++"))
		{
		  strcpy(fileName,"servuino/g++.result");
		  readMsg(fileName);
		}
	      else if(strstr(command[1],"help"))
		{
		  strcpy(fileName,"help_command.txt");
		  readMsg(fileName);
		}
	      else if(strstr(command[1],"loop")) 
		{
		  showLoops();
		}
	      else if(strstr(command[1],"scen")) // scenario
		{
		  showScenario(confSketchFile);
		}
	    }
	  else
	    {
	      strcpy(fileName,"help_command.txt");
	      readMsg(fileName);
	    }
	}

      else if(strstr(sstr,"proj"))
	{
	  if(n == 2)
	    {
	      strcpy(currentConf,command[1]);
	      strcat(currentConf,".conf");
	      readConfig(currentConf);
              g_warning = YES;
	      unoInfo();
	    }
	    readMsg(currentConf);
	}
      else if(strstr(sstr,"list"))
	{
	  readMsg(listConf);	
	}
      else if(strstr(sstr,"sketch"))
	{
	  if(n == 2)
	    {
	      if(strstr(command[1],"conf"))
		{
		  readMsg(confSketchFile);
		}
	      else if(strstr(command[1],"work"))
		{
		  readMsg(workSketchFile);
		}
	    }
	  else
	    readMsg(workSketchFile);	
	}
      else if(strstr(sstr,"conf"))
	{
	  if(n == 3)
	    {
	      if(strstr(command[1],"sim"))
		{
		  confSteps = atoi(command[2]);	
		}
	      if(strstr(command[1],"delay"))
		{
		  confDelay = atoi(command[2]);	
		}
	      else if(strstr(command[1],"log"))
		{
		  confLogLev = atoi(command[2]);
		}
	      else if(strstr(command[1],"win"))
		{
		  confWinMode = atoi(command[2]);
		  if(confWinMode > WIN_MODES)confWinMode = 0;
		  init(confWinMode);
		  unoInfo();
		}
	      else if(strstr(command[1],"sketch"))
		{
		  strcpy(temp,command[2]);
		  if(stat(temp,&st) == 0)
		    {
		      strcpy(confSketchFile,command[2]);
		    }
		  else
		    {
		      sprintf(temp,"Sketch not found: %s",temp);
		      putMsg(msg_h-2,temp);
		    }
		}
	      else if(strstr(sstr,"serv")) // Servuino data file
		{
		  strcpy(confServuinoFile,command[2]);
		}
	      saveConfig(currentConf);
	    }
	  readMsg(currentConf); 
	}
      
      
      else if(strstr(sstr,"sav")) //save config
	{
	  if(n == 2)
	    {
	      strcpy(currentConf,command[1]);
	      strcat(currentConf,".conf");
              
	    }
	  saveConfig(currentConf);
	  readMsg(currentConf);
	  sprintf(syscom,"ls *.conf > conf_list.txt;");
	  x=system(syscom);
	}
      else if(strstr(sstr,"del")) //delete config
	{
	  if(n == 2)
	    {
	      strcpy(currentConf,command[1]);
	      strcat(currentConf,".conf");
              
	    }
          if(strstr(currentConf,"default") == NULL)
          {
	    sprintf(syscom,"rm %s;",currentConf);
	    x=system(syscom);
	    sprintf(syscom,"ls *.conf > conf_list.txt;");
	    x=system(syscom);
	    readMsg(listConf);
            strcpy(currentConf,"default.conf");
          }	
	}
      else if(strstr(sstr,"record"))
	{
          if(n == 2)confLogFile = atoi(command[1]);
	  //sscanf(p,"%s %d",temp,&confLogFile);
	  if(confLogFile >=0 && confLogFile < 2)
	    {
	      saveConfig(currentConf);
	      readMsg(currentConf);
	    }
	}
      else if(strstr(sstr,"win")) //windows layout
        {
          if(n == 2)
          {
              confWinMode = atoi(command[1]);
              if(confWinMode > WIN_MODES)confWinMode = 0;
              init(confWinMode);
              unoInfo();
          }
        }
      else if(strstr(sstr,"loop")) //status
	{
          if(n == 2)loop = atoi(command[1]);
	  runLoops(loop);
	}
      else if(strstr(sstr,"sim")) //status
	{
	  readSimulation(confServuinoFile);
	}
      else if(strstr(sstr,"sys"))
	{
	  p = p+4;
	  wLog(p,-1,-1);
	  x=system(p);
	}
      else if(strstr(sstr,"clear"))
	{
	  sprintf(syscom,"rm servuino/sketch.pde;rm servuino/data.su;");
	  x=system(syscom);
	}
      else if(strstr(sstr,"load"))
	{
          if(n == 2)
	    {
//	      strcpy(confSketchFile,command[1]);
	      confSteps = atoi(command[1]);
	    }

	  loadCurrentProj();
	}
      else if(projNo > 0 && projNo < 10)
        {
           selectProj(projNo,currentConf);
           readConfig(currentConf);
           g_warning = YES;
           unoInfo();
           readMsg(currentConf);   
        }
      else 
	{
	  putMsg(msg_h-2,"Unknown command");
	}
    }
}

//====================================
void runMode(int stop)
//====================================
{
  int ch;
  char tempName[80];
  strcpy(tempName,"help.txt");

  s_mode = RUN;


  if(stop > 0)
    {
      if(stop > g_steps)stop = g_steps;
      if(stop > currentStep)
	runAll(stop);
      return;
    }

  readMsg(tempName);

  wmove(uno,UNO_H-2,1);
  wprintw(uno,"                                                  ");
  mvwprintw(uno,UNO_H-2,1,"R%1d>",confWinMode);
  show(uno);
  wmove(uno,UNO_H-2,4);

  while((ch!='q')&&(ch!='x'))  
    {
      ch = getchar();

      if (ch=='h')
	{
	  readMsg(tempName);
	}
      else if (ch=='g')
	{
	  runAll(g_steps);
	}
      else if (ch=='l')
	{
	  showLoops();
	}
      else if (ch=='w')
	{
	  confWinMode++;
	  if(confWinMode > WIN_MODES)confWinMode = 0;
          init(confWinMode);
	  mvwprintw(uno,UNO_H-2,1,"R%1d>",confWinMode);
	  unoInfo();
	}
      else if (ch=='a')
	{
	  resetSim();
	  init(confWinMode);
	  unoInfo();
	  mvwprintw(uno,UNO_H-2,1,"R%1d>",confWinMode);
	  show(uno);
	}
      else if (ch=='r')
	{
	  runLoop();
	}
      else if (ch=='s') 
	{
	  runStep(FORWARD);
	}
      else if (ch=='l') 
	{
	  confLogLev++;
	  if(confLogLev > 3)confLogLev = 0;
	  // Todo save to conf
	}
      else if (ch=='+') 
	{
	  confDelay = confDelay + 10;
	  // Todo save to conf
	}
      else if (ch=='-') 
	{
	  confDelay = confDelay - 10;
	  if(confDelay < 0)confDelay = 0;
          if(confDelay > 1000)confDelay = 1000;
	  // Todo save to conf
	}
      else if (ch=='f') 
	{
	  confLogFile++;
	  if(confLogFile > 1)confLogFile = 0;
	  // Todo save to conf
	}
      else
        putMsg(msg_h-2,"Unknown command");
    }
}
//====================================
int main(int argc, char *argv[])
//====================================
{
  char syscom[120];
  int ch,i,x;

  if(argc == 2)
    {
      strcpy(currentConf,argv[1]);
      strcat(currentConf,".conf");
    }
  else
    strcpy(currentConf,"default.conf");

  strcpy(listConf,"conf_list.txt");
  strcpy(workSketchFile,"servuino/sketch.pde");

  sprintf(syscom,"ls *.conf > conf_list.txt;");
  x=system(syscom);

  readConfig(currentConf);
  init(confWinMode);
  initSim();
  resetSim();

  readSimulation(confServuinoFile);
  readSketchInfo();
  unoInfo();
  show(slog);

  if(confLogFile == YES)resetFile("log.txt");

  openCommand();
  
  //tcsetattr(0,TCSANOW, &orig);
  delwin(uno);
  delwin(ser);
  delwin(slog);
  delwin(msg);
  endwin();

}
//====================================
// End of file
//====================================

