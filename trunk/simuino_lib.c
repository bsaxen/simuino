//================================================
//  Developed by Benny Saxen, ADCAJO
//================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <math.h> 
#include <termios.h>
#include <unistd.h>
#include <ncurses.h>

#define FREE   0
#define RX     3
#define TX     4

int   row,col;
int   graph_x = 10,graph_y = 10;
int   digPinPos[14];
int   anaPinPos[6];
char  appName[80];
int   analogPin[HIST_MAX][100];
int   digitalPin[HIST_MAX][100];
int   digitalMode[100];
int   paceMaker = 0;
int   baud = 0;
int   error = 0;
int   logging = YES;
char  logBuffer[100][100];
int   logSize = 1;
int   serialSize = 1;
int   serialMode = OFF;
char  serialBuffer[100][100];
int   rememberNewLine;
char  textPinModeIn[14][80];
char  textPinModeOut[14][80];
char  textDigitalWriteLow[14][80];
char  textDigitalWriteHigh[14][80];
char  textAnalogWrite[14][80];
char  textAnalogRead[14][80];
char  textDigitalRead[14][80];

int   conn;

// Configuration default values
int   confDelay  = 100;
int   confLogLev =   1;

WINDOW *uno,*ser,*slog,*com;
static struct termios orig, nnew;
static int peek = -1;


//===================================================
void passTime()
{
  timeFromStart++;
}

void wLog(const char *p, int value1, int value2)
{
  int i;
  char temp[100],temp2[100];

  sprintf(temp," ");

  if(value2 > -2)
    {
      sprintf(temp," %d,%d ",nloop,timeFromStart);
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

  wclear(slog);
  for(i=0;i<logSize;i++)
    {
      wmove(slog,i,0);
      wprintw(slog,"%s",logBuffer[i]);
    } 
  wrefresh(slog);
}

void wLogChar(const char *p, const char *value1, int value2)
{
  int i;
  char temp[100],temp2[100];

  //sprintf(temp," ");
  strcpy(temp," ");
  if(value2 > -2)
    {
      sprintf(temp," %d,%d ",nloop,timeFromStart);
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

  for(i=logSize;i>0;i--)strcpy(logBuffer[i],logBuffer[i-1]);
  strcpy(logBuffer[0],temp);

  wclear(slog);
  for(i=0;i<logSize;i++)
    {
      wmove(slog,i,0);
      wprintw(slog,"%s",logBuffer[i]);
    } 
  wrefresh(slog);
}


int __nsleep(const struct timespec *req, struct timespec *rem)  
{  
  struct timespec temp_rem;  
  if(nanosleep(req,rem)==-1)  
    __nsleep(rem,&temp_rem);  
  else  
    return 1;  
}  
   
int msleep(unsigned long milisec)  
{  
  struct timespec req={0},rem={0};  
  time_t sec=(int)(milisec/1000);  
  milisec=milisec-(sec*1000);  
  req.tv_sec=sec;  
  req.tv_nsec=milisec*1000000L;  
  __nsleep(&req,&rem);  
  return 1;  
}  


void iDelay(int ms)
{
  msleep(ms);
}

void show(WINDOW *win)
{
  wrefresh(win);
  iDelay(confDelay);
}

void showError(const char *m, int value)
{
  error = 1;
  wmove(com,2,1);
  wprintw(com,"                                       ");
  wmove(com,2,1);
  if(value == -1)
    wprintw(com,"Error %s",m);
  else
    wprintw(com,"Error %s %d",m,value);

  show(com);
}

void showConfig()
{
  wmove(com,0,20);
  wprintw(com," Delay=%d",confDelay);
  wprintw(com," LogLevel=%d",confLogLev);
  show(com);
}



void showSerial(const char *m, int newLine)
{
  int i;

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
      wmove(ser,i-1,0);
      wprintw(ser,"%s",serialBuffer[i]);
    } 
  wrefresh(ser);
  rememberNewLine = newLine;
}

void getAppName(char *app)
{
  FILE *in;
  char row[80],junk[20],*p;

  strcpy(app,"unknown");
  in = fopen("sketch/sketch.pde","r");
  if(in == NULL)
    {
      printf("Unable to open sketch\n");
      exit(0);
    }
  else
    {
      while (fgets(row,80,in)!=NULL)
	{
	  if(p=strstr(row,"simuino:"))
	    {
	      p=p+9;
	      sscanf(p,"%s",app);
	    }
	}
    }
  fclose(in);  
}

int wCustomLog(char *in, char *out)
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

void readSketchInfo()
{
  FILE *in;
  char row[80],res[40],*p,value[5];
  int pin;

  in = fopen("sketch/sketch.pde","r");
  if(in == NULL)
    {
      showError("Unable to open sketch",-1);
    }
  else
    {
      while (fgets(row,80,in)!=NULL)
	{
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
    }
  fclose(in);  
}


void boardInit()
{
  int i,j;

  nloop = 0;
  for(i=0;i<14;i++)
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
  for(i=0;i<14;i++)
    {
      for(j=0;j<HIST_MAX;j++)
	{
          analogPin[j][i]   = 0;
          digitalPin[j][i]  = 0;
	}
    }
}

void unimplemented(const char *f)
{
  char temp[200];
  sprintf(temp,"unimplemented: %s\n",f);
  wLog(temp,-1,-1);
}

int readExt()
{
  FILE *in;
  char row[80],*p;
  int x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,temp,res1=0,res2=0,res=0;

  in = fopen("scenario/analogPins.txt","r");
  if(in == NULL)
    {
      showError("Unable to open analog scenario",-1);
    }
  else
    {
      while (fgets(row,80,in)!=NULL)
	{
	  sscanf(row,"%d%d%d%d%d%d%d",&temp,&x0,&x1,&x2,&x3,&x4,&x5);
	  if(temp<HIST_MAX)
	    {
	      res1=temp;
	      analogPin[temp][0]= x0;
	      analogPin[temp][1]= x1;
	      analogPin[temp][2]= x2;
	      analogPin[temp][3]= x3;
	      analogPin[temp][4]= x4;
	      analogPin[temp][5]= x5;
	    }
	}
    }
  fclose(in);  

  in = fopen("scenario/digitalPins.txt","r");
  if(in == NULL)
    {
      showError("Unable to open digital scenario",-1);
    }
  else
    {
      while (fgets(row,80,in)!=NULL)
        {
          sscanf(row,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",&temp,&x0,&x1,&x2,&x3,&x4,&x5,&x6,&x7,&x8,&x9,&x9,&x10,&x11,&x12,&x13);
          if(temp<HIST_MAX)
	    {
	      res2 = temp;
              digitalPin[temp][0]= x0;
              digitalPin[temp][1]= x1;
              digitalPin[temp][2]= x2;
              digitalPin[temp][3]= x3;
              digitalPin[temp][4]= x4;
              digitalPin[temp][5]= x5;
              digitalPin[temp][6]= x6;
              digitalPin[temp][7]= x7;
              digitalPin[temp][8]= x8;
              digitalPin[temp][9]= x9;
              digitalPin[temp][10]= x10;
              digitalPin[temp][11]= x11;
              digitalPin[temp][12]= x12;
              digitalPin[temp][13]= x13;
            }
        }
    }
  fclose(in);

  if(res1 > res2) res = res1;
  else res = res2;
  return(res);
}

void readConfig()
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
	    }
	 
	}
    }
  fclose(in);
}
