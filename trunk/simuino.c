/*  Simuino is a Arduino Simulator based on Servuino Engine 
    Copyright (C) 2012,2013  Benny Saxen
    
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

#include "servuino/common.h"


// Simulator status
int x_pinMode[MAX_TOTAL_PINS][SCEN_MAX];
int x_pinScenario[MAX_TOTAL_PINS][SCEN_MAX];
int x_pinDigValue[MAX_TOTAL_PINS][SCEN_MAX];
int x_pinAnaValue[MAX_TOTAL_PINS][SCEN_MAX];
int x_pinRW[MAX_TOTAL_PINS][SCEN_MAX];

// Window  ====================
#define UNO_H  17
#define UNO_W  63
#define MEGA_H  32
#define MEGA_W  110
#define UNO_COLOR 7

#define MSG_H  20
#define MSG_W  61
#define MSG_COLOR 6

#define LOG_H  40
#define LOG_W  40
#define LOG_COLOR 3

#define SER_H  40
#define SER_W  30
#define SER_COLOR 4

#define WIN_MODES 5

// Sketch Status 
#define SO_VOID       1
#define SO_SELECTED   2
#define SO_LOADED     3
#define SO_COMP_ERROR 4
#define SO_RUN_ERROR  5

// Current data
int  currentStep = 0;
int  currentLoop = 0;
char currentConf[SIZE_ROW];
int  currentPin  = 0;
int  currentValueD[MAX_PIN_DIGITAL_MEGA];
int  currentValueA[MAX_PIN_ANALOG_MEGA];
char g_currentSketch[80];
int  g_currentSketchStatus = SO_VOID;
int  g_runDelay = 0; //millisec


// Limits
int  g_loops    = 0;
int  g_steps    = 0;
int  g_comments = 0;

int g_dir;

int g_value      = 0;
int s_mode       = S_ADMIN;
int g_warning    = S_YES;
int g_silent     = 0;
int g_scenSource = 0;
int g_pinType    = 0;
int g_pinNo      = 0;
int g_pinValue   = 0;
int g_pinStep    = 0;
int g_debug      = 0;

int g_existError = S_NO;

int   digPinCol[MAX_PIN_DIGITAL_MEGA];
int   digPinRow[MAX_PIN_DIGITAL_MEGA];
int   digValCol[MAX_PIN_DIGITAL_MEGA];
int   digValRow[MAX_PIN_DIGITAL_MEGA];
int   digIdCol[MAX_PIN_DIGITAL_MEGA];
int   digIdRow[MAX_PIN_DIGITAL_MEGA];
int   digStatCol[MAX_PIN_DIGITAL_MEGA];
int   digStatRow[MAX_PIN_DIGITAL_MEGA];
int   digActCol[MAX_PIN_DIGITAL_MEGA];
int   digActRow[MAX_PIN_DIGITAL_MEGA];

int   anaPinCol[MAX_PIN_ANALOG_MEGA];
int   anaPinRow[MAX_PIN_ANALOG_MEGA];
int   anaValCol[MAX_PIN_ANALOG_MEGA];
int   anaValRow[MAX_PIN_ANALOG_MEGA];
int   anaIdCol[MAX_PIN_ANALOG_MEGA];
int   anaIdRow[MAX_PIN_ANALOG_MEGA];
int   anaStatCol[MAX_PIN_ANALOG_MEGA];
int   anaStatRow[MAX_PIN_ANALOG_MEGA];
int   anaActCol[MAX_PIN_ANALOG_MEGA];
int   anaActRow[MAX_PIN_ANALOG_MEGA];

int   s_row,s_col;

char  status[MAX_STEP][SIZE_ROW];
char  simulation[MAX_STEP][SIZE_ROW];
char  serialM[MAX_STEP][SIZE_ROW];
int   serialL[MAX_STEP];

char  simComment[MAX_STEP][SIZE_ROW];
int   stepComment[MAX_STEP];
int   commentStep[MAX_STEP];
int   loopPos[MAX_LOOP];
int   stepLoop[MAX_STEP];
int   loopStep[MAX_LOOP];

int   stepDelay[MAX_STEP];

char  appName[120];

int   interruptMode[2];
int   digitalMode[MAX_PIN_DIGITAL_MEGA];
int   paceMaker = 0;
int   baud = 0;
int   error = 0;

// Debug
int   g_lineSketch[MAX_STEP];
char  g_sourceSketch[MAX_SOURCE_LINES][SIZE_ROW];

// Log
char  logBlankRow[MAX_SERIAL_BUFFER];

// Serial Interface
int   serialMode = S_OFF;
char  prevSerial[MAX_SERIAL_BUFFER];
char  serBlankRow[MAX_SERIAL_BUFFER];

int   s_digitalPin[MAX_STEP][MAX_PIN_DIGITAL_MEGA];
int   s_analogPin[MAX_STEP][MAX_PIN_ANALOG_MEGA];

char  textPinModeIn[MAX_PIN_DIGITAL_MEGA][SIZE_ROW];
char  textPinModeOut[MAX_PIN_DIGITAL_MEGA][SIZE_ROW];
char  textDigitalWriteLow[MAX_PIN_DIGITAL_MEGA][SIZE_ROW];
char  textDigitalWriteHigh[MAX_PIN_DIGITAL_MEGA][SIZE_ROW];
char  textAnalogWrite[MAX_PIN_DIGITAL_MEGA][SIZE_ROW];
char  textAnalogRead[MAX_PIN_ANALOG_MEGA][SIZE_ROW];
char  textDigitalRead[MAX_PIN_DIGITAL_MEGA][SIZE_ROW];
int   scenAnalog    = 0;
int   scenDigital   = 0;
int   scenInterrupt = 0;

// Configuration default values
int   confSteps   = 1000;
int   confWinMode =    2;
int   confLogLev  =    1;
int   confLogFile =    0;
char  confSketchFile[200];
int   confBoardType = UNO;

char  fileTemp[80]         = "temp.txt";
char  fileInfoRun[80]      = "help.txt";
char  fileCopyError[80]    = "copy.error";
char  fileHints[80]        = "hints.txt";
char  fileInfoAdmin[80]    = "help_command.txt";
char  fileInfoGpl[80]      = "gpl.txt";
char  fileProjList[80]     = "conf_list.txt";
char  fileLog[80]          = "log.txt";
char  fileDefault[80]      = "default.conf";
char  fileError[80]        = "error.txt";
char  fileServComp[80]     = "servuino/g++.result";
char  fileServSketch[80]   = "servuino/sketch.ino";
char  fileServArduino[80]  = "servuino/serv.event";
char  fileServError[80]    = "servuino/serv.error";
char  fileServScen[80]     = "servuino/data.scen";
char  fileServScenario[80] = "servuino/data.scenario";
char  fileServCode[80]     = "servuino/data.code";
char  fileServCustom[80]   = "servuino/serv.cust";

char  fileServInoDebug[80] = "servuino/ino.debug";

char  fileServPinmod[80]   = "servuino/serv.pinmod";
char  fileServDigval[80]   = "servuino/serv.digval";
char  fileServAnaval[80]   = "servuino/serv.anaval";
char  fileServPinrw[80]    = "servuino/serv.pinrw";
char  fileServSerial[80]   = "servuino/serv.serial";
char  fileServTime[80]     = "servuino/serv.time";

int  g_nScenDigital = 0;
int  g_nScenAnalog  = 0;

int  g_row_setup = 0;
int  g_row_loop = 0;

int uno_h=0, uno_w=0, uno_x=0, uno_y=0;
int msg_h=0, msg_w=0, msg_x=0, msg_y=0;
int log_h=0, log_w=0, log_x=0, log_y=0;
int ser_h=0, ser_w=0, ser_x=0, ser_y=0;
int board_w=0,board_h=0,board_x=0,board_y=0;
int ap,dp;

//#define DP 5   // Digital Pin Row No
//#define AP 11  // Analog Pin Row No
#define RF 1 
#define ER 1
#define SR 20


WINDOW *uno,*ser,*slog,*msg;
static struct termios orig, nnew;
 
//char  stemp[80];
char  gplFile[80];

FILE  *err;

#include "servuino/common_lib.c"
#include "servuino/arduino.h"
#include "simuino.h"
#include "simuino_lib.c"

//====================================
int runStep(int dir)
//====================================
{
  //char stemp[SIZE_ROW];
  int temp;

  g_dir = dir;

  if(dir == S_FORWARD)currentStep++;
  if(dir == S_BACKWARD)currentStep--;
  if(currentStep > g_steps || currentStep < 1)
    {
      currentStep = checkRange(HEAL,"step",currentStep);
      return(S_STOP); 
    }
  currentLoop = stepLoop[currentStep];
  winLog();
  winSer();
  //strcpy(stemp,status[currentStep]);
  displayStatus();
  unoInfo();

  // Realtime animation
  temp = stepDelay[currentStep];
  if(temp > 0)microDelay(temp);
  iDelay(g_runDelay);
  return(0);
}    

//====================================
int goStep(int step)
//====================================
{
  char stemp[SIZE_ROW];


  if(step > g_steps || step < 1)
    {
      step = checkRange(HEAL,"step",step);
      //return(STOP); 
    }
  currentStep = step;
  currentLoop = stepLoop[currentStep];
  winLog();
  winSer();
  //strcpy(stemp,status[currentStep]);
  displayStatus();
  unoInfo();
  iDelay(g_runDelay);
  return(0);
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
void loadCurrentSketch()
//====================================
{
  int x,res;
  char syscom[120],temp[200];

  g_warning = S_NO;
  putMsg(1,"Loading sketch ...");
  res = loadSketch(g_currentSketch);
  if(res == 0)
  {
    if(confSteps < 0) confSteps = 100;
    if(confSteps > MAX_STEP) confSteps = MAX_STEP-1;
    sprintf(syscom,"cd servuino;./servuino %d %d;",confSteps,g_scenSource);
    x=system(syscom);
    initSim();
    resetSim();
    readSimulation();
    readSketchInfo();
    setRange(confBoardType);
    init(confWinMode);
    saveSetting();
    unoInfo();
    sprintf(temp,"Sketch load ready: %s",confSketchFile);
    putMsg(msg_h-2,temp);
    g_currentSketchStatus = SO_LOADED;
  }
  if(res != 0)g_currentSketchStatus = SO_COMP_ERROR;
}

//====================================
void openCommand()
//====================================
{
  struct stat st;
  int ch,nsteps=1000,x,i,n,stop=0,loop,projNo = 0,ok=0,tmp;
  char *p,str[120],sstr[20],fileName[120],temp[120],syscom[120];
  char command[40][40];

  s_mode = S_ADMIN;
  g_silent = 0;

  readMsg(gplFile);

  while(strstr(str,"ex") == NULL)
    {
      anyErrors();
      unoInfo();

      wmove(uno,board_h-2,1);
      wprintw(uno,"                                                  ");
      if(g_silent==S_NO )mvwprintw(uno,board_h-2,1,"A%1d>",confWinMode);
      if(g_silent==S_YES)mvwprintw(uno,board_h-2,1,"A%1d<",confWinMode);

      strcpy(command[0],"");

      wgetstr(uno,str);

      n = tokCommand(command,str);

      strcpy(sstr,command[0]);

      p = str;

      projNo = atoi(sstr);

      if(strstr(sstr,"gpl"))
        {
          readMsg(gplFile);
        }
      else if(strstr(sstr,"err"))
        {
          readMsg(fileTemp);
        }
      else if(strstr(sstr,"run"))
	{
	  stop = 1;
          if(n == 2)stop = atoi(command[1]);
	  stop = checkRange(HEAL,"step",stop);

	  runMode(stop);
          if(stop==0)putMsg(2,"Back in Admin Mode!");
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
	  strcpy(fileName,fileInfoAdmin);
	  readMsg(fileName);
	}
      else if(strstr(sstr,"rem")) //
	{
	  if(n == 4)
	    {
	      if(strstr(command[1],"a"))g_pinType = ANA;
	      if(strstr(command[1],"d"))g_pinType = DIG;
	      g_pinNo   = atoi(command[2]);
	      g_pinStep = atoi(command[3]);
	      if(g_pinType == ANA)
		ok = checkRange(S_OK,"anapin",g_pinNo);
	      if(g_pinType == DIG)
		ok = checkRange(S_OK,"digpin",g_pinNo);
	      if(ok == S_OK)
		{
		  g_scenSource = 1;
		  sprintf(syscom,"cd servuino;./servuino %d %d %d %d %d %d %d;",confSteps,g_scenSource,g_pinType,g_pinNo,0,g_pinStep,S_DELETE);
		  //putMsg(2,syscom);
		  tmp=system(syscom);
		  initSim();
		  readSketchInfo();
		  readSimulation();
		  runStep(S_FORWARD);
		  readMsg(fileServScen);
		}
	      else
		putMsg(2,"Wrong pin number or pin type!");
	    }
	  else
	    putMsg(2,"Syntax: rem <a or d> <pin> <step>");
	}
      else if(strstr(sstr,"add")) //
	{
	  if(n == 5)
	    {
	      if(strstr(command[1],"a"))g_pinType = ANA;
	      if(strstr(command[1],"d"))g_pinType = DIG;

	      g_pinNo    = atoi(command[2]);
	      g_pinStep  = atoi(command[3]);
	      g_pinValue = atoi(command[4]);

	      ok = S_OK;
	      if(g_pinType == ANA)
		ok = checkRange(S_OK,"anapin",g_pinNo);
	      if(g_pinType == DIG)
		ok = checkRange(S_OK,"digpin",g_pinNo);

	      ok = ok + checkRange(S_OK,"step",g_pinStep);

	      if(ok == S_OK)
		{
		  g_scenSource = 1;
		  sprintf(syscom,"cd servuino;./servuino %d %d %d %d %d %d %d;",confSteps,g_scenSource,g_pinType,g_pinNo,g_pinValue,g_pinStep,S_ADD);
		  tmp=system(syscom);
		  initSim();
		  readSketchInfo();
		  readSimulation();
		  runStep(S_FORWARD);
		  readMsg(fileServScen);
		}
	    }
	  else
	    putMsg(2,"Syntax: add <a or d> <pin> <step> <value>");
	}
      else if(strstr(sstr,"info"))
	  {
	    if(n == 2)
	    {
	     if(strstr(command[1],"err"))
		 {
		  readMsg(fileServError);
		 }
	     else if(strstr(command[1],"g++"))
		 {
		  readMsg(fileServComp);
		 }
	     else if(strstr(command[1],"loop")) 
		 {
		  showLoops();
		 }
	     else if(strstr(command[1],"scen")) // scenario
		 {
		  readMsg(fileServScen);
		 }
	  }
	  else
	  {
	     readMsg(fileInfoAdmin);
	  }
	}

    else if(strstr(sstr,"list"))
	{
	  sprintf(syscom,"ls sketchbook/*.ino > %s;",fileProjList);
	  x=system(syscom);
	  readMsg(fileProjList);	
	}
    else if(strstr(sstr,"sketch"))
	{
	  if(n == 2)
	    {
	      if(strstr(command[1],"ino"))
		{
		  readMsg(confSketchFile);
		}
	      else if(strstr(command[1],"work"))
		{
		  readMsg(fileServSketch);
		}
	    }
	  else
	    readMsg(fileServSketch);	
	}
    else if(strstr(sstr,"win"))
	{
	  confWinMode = atoi(command[1]);
	  if(confWinMode > WIN_MODES)confWinMode = 0;
	  init(confWinMode);
	  unoInfo();
	}
    else if(strstr(sstr,"conf"))
	{
	  if(n == 3)
	    {
	      if(strstr(command[1],"sim"))
		{
		  confSteps = atoi(command[2]);	
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
		      sprintf(temp,"Sketch not found: %s",command[2]);// Issue 16
		      putMsg(msg_h-2,temp);
		    }
		}
	      saveSetting();
	    }
	  //readMsg(currentConf); 
	}
      
      
      else if(strstr(sstr,"sav")) //save config
	{

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
    else if(strstr(sstr,"loop"))
	{
      if(n == 2)loop = atoi(command[1]);
	  loop = checkRange(HEAL,"loop",loop);
	  runLoops(loop);
	}
      else if(strstr(sstr,"clear"))
	{
	  sprintf(syscom,"rm servuino/sketch.ino;rm servuino/data.su;rm servuino/data.scen;");
	  x=system(syscom);
	}
      else if(strstr(sstr,"load"))
	{
        if(n == 2)
	    {
	      confSteps = atoi(command[1]);
	    }
        g_scenSource = 0;
	    loadCurrentSketch();
	}
      else if(projNo > 0 && projNo < 21)
        {
	      selectProj(projNo,g_currentSketch);
	      readConfig(g_currentSketch);
	      g_warning = S_YES;
	      g_currentSketchStatus = SO_SELECTED;
	      setRange(confBoardType);
	      init(confWinMode);
          unoInfo();
          show(slog);
	      //readMsg(g_currentSketch);
	      readMsg(fileProjList);	   
        }
      else if(strstr(sstr,"data"))
	{
	  if(n == 2)
	    {
	      if(strstr(command[1],"ard"))
		{
		  readMsg(fileServArduino);
		}
	      else if(strstr(command[1],"cus"))
		{
		  readMsg(fileServCustom);
		}
	      else if(strstr(command[1],"cod"))
		{
		  readMsg(fileServCode);
		}
	      else if(strstr(command[1],"err"))
		{
		  readMsg(fileServError);
		}
	      else if(strstr(command[1],"sce"))
		{
		  readMsg(fileServScen);
		}
	    }
	  else
	    readMsg(fileServArduino);
	}
      else 
	{
	  putMsg(msg_h-2,"Unknown Admin command");
	}
    }
}

//====================================
void runMode(int stop)
//====================================
{
  int ch,x,step,tmp,res=0,a=0,b=0,ir,ok=0,n=0;
  char tempName[80],syscom[120],temp[80];
  char command[40][40];

  strcpy(tempName,"help.txt");

  s_mode = S_RUN;


  if(stop > 1)
    {
      if(stop > g_steps)stop = g_steps;
      //if(stop > currentStep)
	  runAll(stop);
      return;
    }

  putMsg(3,"Run Mode. Press h for help.");

  while(1)  
    {
      if(g_debug == 1) 
	   readFile(g_currentSketch,g_lineSketch[currentStep]);
	   
      anyErrors();
      if(g_silent==S_NO )mvwprintw(uno,board_h-2,1,"R%1d>",confWinMode);
      if(g_silent==S_YES)mvwprintw(uno,board_h-2,1,"R%1d<",confWinMode);
      unoInfo();
      
      ch = getchar();

    if (ch=='q')
	{
	  return;
	}

    if (ch=='h')
    {
          readMsg(fileInfoRun);
    }
    else if (ch=='c')
    {
		sprintf(temp,"Simulation Length: %d\n Delay............: %d ms",confSteps,g_runDelay);
		putMsg(2,temp);
    }
    else if (ch=='e')
    {
         readMsg(fileServError);
    }
    else if (ch=='d')
    {
          readMsg(fileServTime);
    }
    else if(ch=='y' ) // scenario
	{
	  readMsg(fileServScen);
	}
    else if (ch=='x')
	{
	  readMsg(fileServScenario);
	}
    else if (ch=='G')
	{
	  runAll(g_steps);
	}
    else if (ch=='l')
	{
	  showLoops();
	}
    else if (ch=='s')
	{
	  g_debug++;
	  if(g_debug > 1)
	  {
		  g_debug = 0;
		  putMsg(2,"Run Mode. Press h for help.");
	  }
		  
	}
    else if (ch=='w')
	{
	  confWinMode++;
	  if(confWinMode > WIN_MODES)confWinMode = 0;
          init(confWinMode);
	  mvwprintw(uno,board_h-2,1,"R%1d>",confWinMode);
	  unoInfo();
	}
    else if (ch=='a')
	{
          goStep(1);
	}
    else if (ch=='r')
	{
          goStep(loopStep[currentLoop+1]);
	}
    else if (ch=='o')
	{
          goStep(loopStep[currentLoop]);
	}
    else if (ch=='p')
	{
          goStep(loopStep[currentLoop-1]);
	}
    else if (ch=='z')
	{
          goStep(g_steps);
	}
    else if (ch=='k')
	{
	  resetSim();
	  init(confWinMode);
	  unoInfo();
	  mvwprintw(uno,board_h-2,1,"R%1d>",confWinMode);
	  show(uno);
	}
    else if (ch=='f')// Up Arrow 
	{
	  goStep(currentStep+1);
	}
    else if (ch=='b')// Down Arrow
	{
	  goStep(currentStep-1);
	}
    else if (ch=='R') // Right Arrow
	{
	  runLoop(S_FORWARD);
	}
    else if (ch=='P') // Left Arrow
	{
	  runLoop(S_BACKWARD);
	}
	else if (ch=='A')// Up Arrow 
	{
	  goStep(currentStep+1);
	}
    else if (ch=='B')// Down Arrow
	{
	  goStep(currentStep-1);
	}
    else if (ch=='C') // Right Arrow
	{
	  runLoop(S_FORWARD);
	}
    else if (ch=='D') // Left Arrow
	{
	  runLoop(S_BACKWARD);
	}
    else if (ch=='t')
	{
	  runNextRead();
	}
    else if (ch=='j')
	{
	  runPrevRead();
	}
   /* else if (ch=='i') 
	{
		ok = 0;
          step = currentStep;
	  sprintf(temp,"(Step:%d) Enter: d/a pin value (q - cancel)",step);
          putMsg(2,temp);
	  wgetstr(uno,temp);
	  n = tokCommand(command,temp);
      
	  if(strstr(command[0],"q") == NULL && n == 3)
	    {
	      g_pinNo = atoi(command[1]);
	      x = atoi(command[2]);
	      printf("%s %d %d",command[0],g_pinNo,x);
	      if(strstr(command[0],"a"))
		{
		  ok = ok + checkRange(S_OK,"anapin",g_pinNo);
		  ok = ok + checkRange(S_OK,"anaval",x);
		  g_pinType = ANA;
		}
	      if(strstr(command[0],"d"))
		{
		  ok = ok + checkRange(S_OK,"digpin",g_pinNo);
		  ok = ok + checkRange(S_OK,"digval",x);
		  g_pinType = DIG;
		}
	      if(ok == S_OK)
		{ 
		  g_scenSource = 1;
		  // steps, source, pintype, pinno, pinvalue, pinstep
		  sprintf(syscom,"cd servuino;./servuino %d %d %d %d %d %d %d;",confSteps,g_scenSource,g_pinType,g_pinNo,x,currentStep,S_ADD);
putMsg(2,syscom);
		  tmp=system(syscom);
		  initSim();
		  readSketchInfo();
		  readSimulation();
		  goStep(currentStep);
		  readMsg(fileServScen);
		}
	    }
	  else
	    putMsg(2,"Cancelled!");
	} */
    else if (ch=='v') 
	{
          step = currentStep ;
          res = analyzeEvent(simulation[step]);
	      sprintf(temp," Pin:%d Enter value to be read at step %d (q - cancel)",g_pinNo,step);
          putMsg(2,temp);

          if(res > 0)
	    {
	      wgetstr(uno,temp);

	      if(strstr(temp,"q") == NULL)
		{
		  x = atoi(temp); 
		  ok = S_OK;
		  if(res == ANA)ok = ok + checkRange(S_OK,"anaval",x);
		  if(res == DIG)ok = ok + checkRange(S_OK,"digval",x);
		  if(ok == S_OK)
		    {         
		      g_scenSource = 1;
		      // steps, source, pintype, pinno, pinvalue, pinstep
		      sprintf(syscom,"cd servuino;./servuino %d %d %d %d %d %d %d;",confSteps,g_scenSource,g_pinType,g_pinNo,x,currentStep,S_ADD);
		      tmp=system(syscom);
		      initSim();
		      readSketchInfo();
		      readSimulation();
		      goStep(currentStep);
		      readMsg(fileServScen);
		    }
		}
	      else
		putMsg(2,"Cancelled!");
	    }
	  else
	    putMsg(2,"Next step is not a Read event");
	}
      else
	{
	  sprintf(temp,"Unknown command: %c",ch);
	  putMsg(msg_h-2,temp);
	}
    }
  return;
}
//====================================
int main(int argc, char *argv[])
//====================================
{
  char syscom[120];
  int ch,i,x;

  currentStep = 1;
  currentLoop = 0;



  strcpy(gplFile,"gpl.txt");

  inrpt[0] = IR0;
  inrpt[1] = IR1;
  inrpt[2] = IR2;
  inrpt[3] = IR3;
  inrpt[4] = IR4;
  inrpt[5] = IR5;


  sprintf(syscom,"ls sketchbook/*.ino > %s;",fileProjList);
  x=system(syscom);
  sprintf(syscom,"rm %s;touch %s;",fileTemp,fileTemp);
  x=system(syscom);
  sprintf(syscom,"rm %s;touch %s;",fileError,fileError);
  x=system(syscom);
  sprintf(syscom,"rm %s;touch %s;",fileServError,fileServError);
  x=system(syscom);
  sprintf(syscom,"rm %s;touch %s;",fileCopyError,fileCopyError);
  x=system(syscom);

  err = fopen(fileError,"w"); // Issue 15

  readSetting();
  readConfig(g_currentSketch);


  initSim();

  resetSim();

  readSimulation();

  readSketchInfo();

  setRange(confBoardType);

  init(confWinMode);

  unoInfo();
  show(slog);

  if(confLogFile == S_YES)resetFile("log.txt");

  readMsg(gplFile);

  openCommand();
  
  delwin(uno);
  delwin(ser);
  delwin(slog);
  delwin(msg);
  endwin();

  fclose(err);

}
//====================================
// End of file
//====================================

