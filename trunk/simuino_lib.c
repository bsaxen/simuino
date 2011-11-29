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

#define FREE   0
#define RX     3
#define TX     4

int   s_row,s_col;
int   digPinPos[MAX_PIN_DIGITAL];
int   anaPinPos[MAX_PIN_ANALOG];
char  appName[80];

int   interruptMode[2];
int   digitalMode[MAX_PIN_DIGITAL];
int   paceMaker = 0;
int   baud = 0;
int   error = 0;
int   logging = YES;
char  logBuffer[MAX_LOG][100];
int   logSize = 1;
int   serialSize = 1;
int   serialMode = OFF;
char  serialBuffer[100][100];
int   rememberNewLine;
char  textPinModeIn[MAX_PIN_DIGITAL][80];
char  textPinModeOut[MAX_PIN_DIGITAL][80];
char  textDigitalWriteLow[MAX_PIN_DIGITAL][80];
char  textDigitalWriteHigh[MAX_PIN_DIGITAL][80];
char  textAnalogWrite[MAX_PIN_DIGITAL][80];
char  textAnalogRead[MAX_PIN_ANALOG][80];
char  textDigitalRead[MAX_PIN_DIGITAL][80];
int   scenAnalog    = 0;
int   scenDigital   = 0;
int   scenInterrupt = 0;



int   conn;

// Configuration default values
int   confDelay   = 100;
int   confLogLev  =   1;
int   confLogFile =   0;

WINDOW *uno,*ser,*slog,*msg;
static struct termios orig, nnew;
static int peek = -1;

void wLog(const char *p, int value1, int value2);

//====================================
int __nsleep(const struct timespec *req, struct timespec *rem)  
//====================================
{  
  struct timespec temp_rem;  
  if(nanosleep(req,rem)==-1)  
    __nsleep(rem,&temp_rem);  
  else  
    return 1;  
}  

//====================================   
int msleep(unsigned long milisec)  
//====================================
{  
  struct timespec req={0},rem={0};  
  time_t sec=(int)(milisec/1000);  
  milisec=milisec-(sec*1000);  
  req.tv_sec=sec;  
  req.tv_nsec=milisec*1000000L;  
  __nsleep(&req,&rem);  
  return 1;  
}  

//====================================
void iDelay(int ms)
//====================================
{
  msleep(ms);
}

//====================================
void show(WINDOW *win)
//====================================
{
  box(win,0,0);
  wrefresh(win);
  iDelay(confDelay);
}

//====================================
void showError(const char *m, int value)
//====================================
{
  char err_msg[300];
  strcpy(err_msg,"ERROR ");
  strcat(err_msg,m);
  wLog(err_msg,value,-1);
  error = 1;
}
//====================================
void saveConfig()
//====================================
{
  FILE *out;
  time_t lt;

  out = fopen("config.txt","w");
  if(out == NULL)
    {
      showError("Unable to open config.txt ",-1);
    }
  else
    {
      lt = time(NULL);
      fprintf(out,"# Simuino Configuration %s",ctime(&lt));
      fprintf(out,"DELAY      %d\n",confDelay);
      fprintf(out,"LOG_LEVEL  %d\n",confLogLev);
      fprintf(out,"LOG_FILE   %d\n",confLogFile);
    }

  fclose(out);
}

//====================================
void resetFile(const char *filename)
//====================================
{
  FILE *out;
  time_t lt;

  out = fopen(filename,"w");
  if(out == NULL)
    {
      showError("Unable to reset file ",-1);
    }
  else
    {
      lt = time(NULL);
      fprintf(out,"# Simuino Log File %s",ctime(&lt));
    }

  fclose(out);
}

//====================================
void logFile(char *m)
//====================================
{
  FILE *out;

  out = fopen("log.txt","a");
  if(out == NULL)
    {
      showError("Unable to open log.txt",-1);
    }
  else
    {
      fprintf(out,"%s\n",m);
    }

  fclose(out);
}

//====================================
void wLog(const char *p, int value1, int value2)
//====================================
{
  int i;
  char temp[100],temp2[100];

  sprintf(temp," ");

  if(value2 > -2)
    {
	  sprintf(temp," %d,%d ",currentLoop,currentStep);
    }
  strcat(temp,p);
 

  if(value1 > -1)
    {
      sprintf(temp2," %d",value1);
      strcat(temp,temp2);
    }
  
  if(value2 > -1)
    {
      sprintf(temp2," %d",value2);
      strcat(temp,temp2);
    }

  if(error == 1)
    {
      temp[0]='*';
      error = 0;
    }

  for(i=logSize;i>0;i--)strcpy(logBuffer[i],logBuffer[i-1]);
  strcpy(logBuffer[0],temp);

  if(confLogFile==YES)logFile(temp);

  wclear(slog);
  for(i=0;i<logSize;i++)
    {
      wmove(slog,i+1,1);
      wprintw(slog,"%s",logBuffer[i]);
    } 
  show(slog);
}


//====================================
void unoInfo()
//====================================
{
  wmove(uno,8,0); 
  wprintw(uno," Sketch: %s",appName);
  wmove(uno,9,0);
  wprintw(uno," Scenario: Dig=%d Ana=%d Interrupt=%d",scenDigital,scenAnalog,scenInterrupt);
  wmove(uno,10,0);
  wprintw(uno," Steps = %d(%d) Loops = %d(%d)",currentStep,g_steps,currentLoop,g_loops);

  if(loopPos[currentLoop+1] > 0)
    wprintw(uno," Next loop at step %d",loopPos[currentLoop+1]);
  else
    wprintw(uno,"                        ");

  wmove(uno,11,0);
  wprintw(uno," Next: %s",simulation[currentStep+1]);

  show(uno);
}

//====================================
void wLogChar(const char *p, const char *value1, int value2)
//====================================
{
  int i;
  char temp[100],temp2[100];

  strcpy(temp," ");
  if(value2 > -2)
    {
	sprintf(temp," %d,%d ",currentLoop,currentStep);
    }  
  strcat(temp,p);
  

  if(value1)
    {
      sprintf(temp2," %s",value1);
      strcat(temp,temp2);
    }
  
  if(value2 > -1)
    {
      sprintf(temp2," %d",value2);
      strcat(temp,temp2);
    }

  if(error == 1)
    {
      temp[0]='*';
      error = 0;
    }

  for(i=logSize;i>0;i--)strcpy(logBuffer[i],logBuffer[i-1]);
  strcpy(logBuffer[0],temp);

  wclear(slog);
  for(i=0;i<logSize;i++)
    {
      wmove(slog,i+1,1);
      wprintw(slog,"%s",logBuffer[i]);
    } 
  show(slog);
}


//====================================
void showSerial(const char *m, int newLine)
//====================================
{
  int i;

  if(serialMode == ON)
    {
      if(rememberNewLine == 1)
	{
	  for(i=1;i<serialSize;i++)strcpy(serialBuffer[i],serialBuffer[i+1]);
	  strcpy(serialBuffer[serialSize],m);
	}
      else
	strcat(serialBuffer[serialSize],m);
      
      wclear(ser);
      for(i=1;i<=serialSize;i++)
	{
	  wmove(ser,i-1,1);
	  wprintw(ser,"%s",serialBuffer[i]);
	} 
      show(ser);
      rememberNewLine = newLine;
    }
  else
    {
      showError("Serial output without Serial.begin",currentStep);
    }
}


//====================================
int wCustomLog(char *in, char *out)
//====================================
{
  char *q,*p;
  int pin;

  p = strstr(in,":"); 
  p++; 
  sscanf(p,"%d",&pin); 
  p = strstr(p,"\""); 
  p++; 
  q = strstr(p,"\""); 
  strcpy(q,"\0"); 
  strcpy(out,p);
  return(pin);
}

//====================================
void getString(char *in, char *out)
//====================================
{
  char *q,*p;

  p = strstr(in,"'");
  p++;
  q = strstr(p,"'");
  strcpy(q,"\0");
  strcpy(out,p);
  //wLog(out,-1,-1);
  return;
}

//====================================
void readSketchInfo(char *fileName)
//====================================
{
  FILE *in;
  char row[80],res[40],*p,*q,value[5];
  int pin;

  //wLog("inside",1,2);

  in = fopen(fileName,"r");

      while (fgets(row,80,in)!=NULL)
	{

	  if(p=strstr(row,"SKETCH_NAME:"))
	    {
	      q = strstr(p,":");q++;
	      sscanf(q,"%s",appName);
	    }
	  if(p=strstr(row,"PINMODE_IN:"))
	    {
	      pin = wCustomLog(p,res);
	      strcpy(textPinModeIn[pin],res);
	    }
	  if(p=strstr(row,"PINMODE_OUT:"))
	    {
	      pin = wCustomLog(p,res);
	      strcpy(textPinModeOut[pin],res);
	    }
	  if(p=strstr(row,"DIGITALWRITE_LOW:"))
	    {
	      pin = wCustomLog(p,res);
	      strcpy(textDigitalWriteLow[pin],res);
	    }
	  if(p=strstr(row,"DIGITALWRITE_HIGH:"))
	    {
	      pin = wCustomLog(p,res);
	      strcpy(textDigitalWriteHigh[pin],res);
	    }
	  if(p=strstr(row,"ANALOGREAD:"))
	    {
	      pin = wCustomLog(p,res);
	      strcpy(textAnalogRead[pin],res);
	    }
	  if(p=strstr(row,"DIGITALREAD:"))
	    {
	      pin = wCustomLog(p,res);
	      strcpy(textDigitalRead[pin],res);
	    }
	  if(p=strstr(row,"ANALOGWRITE:"))
	    {
	      pin = wCustomLog(p,res);
	      strcpy(textAnalogWrite[pin],res);
	    }
	}
  fclose(in); 
  //exit(0); 
}

//====================================
void boardInit()
//====================================
{
  int i,j;

  currentLoop = 0;

  for(i=0;i<MAX_LOG;i++)
    {
       strcpy(logBuffer[i]," ");
    }
  for(i=0;i<MAX_STEP;i++)
    {
      strcpy(simulation[i]," ");
    }
  for(i=0;i<MAX_LOOP;i++)
    {
      loopPos[i] = 0;
    }
  for(i=0;i<MAX_SERIAL;i++)
    {
      strcpy(serialBuffer[i],"");
    }
  for(i=0;i<MAX_PIN_DIGITAL;i++)
    {
      digitalMode[i] = FREE;
      strcpy(textPinModeIn[i],"void");
      strcpy(textPinModeOut[i],"void");

      strcpy(textDigitalWriteLow[i],"void");
      strcpy(textDigitalWriteHigh[i],"void");

      strcpy(textAnalogWrite[i],"void");
      strcpy(textAnalogRead[i],"void");

      strcpy(textDigitalRead[i],"void");
    }
  for(i=0;i<MAX_PIN_ANALOG;i++)
    {
      strcpy(textAnalogRead[i],"void");
    }
}

//====================================
void unimplemented(const char *f)
//====================================
{
  char temp[200];
  sprintf(temp,"unimplemented: %s\n",f);
  wLog(temp,-1,-1);
}



//====================================
void readConfig()
//====================================
{
  FILE *in;
  char row[80],*p,temp[40];
  int x;

  in = fopen("config.txt","r");
  if(in == NULL)
    {
      showError("Unable to open config.txt",-1);
    }
  else
    {
      while (fgets(row,80,in)!=NULL)
	{
	  if(row[0] != '#')
	    {
	      if(p=strstr(row,"LOG_LEVEL"))
		{
		  sscanf(p,"%s%d",temp,&confLogLev);
		}
	      if(p=strstr(row,"DELAY"))
		{
		  sscanf(p,"%s%d",temp,&confDelay);
		}
	      if(p=strstr(row,"LOG_FILE"))
		{
		  sscanf(p,"%s%d",temp,&confLogFile);
		}
	    }
	 
	}
    }
  fclose(in);
}
//====================================
// End of file
//====================================
