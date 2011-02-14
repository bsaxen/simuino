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
#define PWM    5

#define ON     1
#define OFF    0

#define YES    10
#define NO     20

int row,col;
int graph_x= 10,graph_y=10;
int digPinPos[14];
int anaPinPos[6];
char appName[80];
int   analogPin[100];
int   digitalPin[100];
int   digitalMode[100];
int   timeFromStart = 0;
int   paceMaker = 0;
int   baud = 0;
int   logging = YES;
char  logBuffer[100][100];
int   logSize = 1;
int   g[14][900];
int   serialMode = OFF;
char  serialBuffer[240],stemp[80];

WINDOW *uno,*ser,*slog,*com;
static struct termios orig, nnew;
static int peek = -1;

int kbhit()
{

  char ch;
  int nread;

  if(peek != -1) return 1;
  nnew.c_cc[VMIN]=0;
  tcsetattr(0, TCSANOW, &nnew);
  nread = read(0,&ch,1);
  nnew.c_cc[VMIN]=1;
  tcsetattr(0, TCSANOW, &nnew);

  if(nread == 1) {
   peek = ch;
   return 1;
  }

  return 0;
}

int readch()
{

  char ch;

  if(peek != -1) {
    ch = peek;
    peek = -1;
    return ch;
  }

  read(0,&ch,1);
  return ch;
}


void passTime()
{
  int i;
  timeFromStart++;
}

void wLog(const char *p, int value1, int value2)
{
  int i;
  char temp[100],temp2[100];

  sprintf(temp," %d ",timeFromStart);
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
  char msg[200];
  msleep(ms);
}

void show(WINDOW *win)
{
  wmove(win,0,0);
  wrefresh(win);
  iDelay(100);
}

void showSerial()
{
  wmove(ser,0,0);
  wprintw(ser,"%s",serialBuffer);
  wrefresh(ser);
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

void boardInit()
{
  int i;

  for(i=0;i<14;i++)
    {
      digitalPin[i]      = 0;
      digitalMode[i]     = FREE;
    }
  for(i=0;i<6;i++)
    {
      analogPin[i]     = 0;
    }
}

void unimplemented(const char *f)
{
  wmove(ser,0,0);
  wprintw(ser,"unimplemented: %s\n",f);
  wrefresh(ser); 
}
