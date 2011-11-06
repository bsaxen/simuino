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
#define PWM    5

#define TEMPO  100


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
int   logging = YES;
char  logBuffer[100][100];
int   logSize = 1;
int   g[14][900];
int   serialMode = OFF;
char  serialBuffer[240],stemp[80];

int conn;

WINDOW *uno,*ser,*slog,*com;
static struct termios orig, nnew;
static int peek = -1;


void passTime()
{
  int i;
  timeFromStart++;
}

void wLog(const char *p, int value1, int value2)
{
  int i;
  char temp[100],temp2[100];

  sprintf(temp," %d,%d ",nloop,timeFromStart);
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
  iDelay(TEMPO);
}

void showSerial()
{
  wmove(ser,1,0);
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
  int i,j;

  nloop = 0;
  for(i=0;i<14;i++)
    {
//      digitalPin[i]      = 0;
      digitalMode[i]     = FREE;
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
  wmove(ser,0,0);
  wprintw(ser,"unimplemented: %s\n",f);
  wrefresh(ser); 
  iDelay(100);
}

int readExt()
{
  FILE *in;
  char row[80],*p;
  int x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,temp,res1=0,res2=0,res=0;

  in = fopen("scenario/analogPins.txt","r");
  if(in == NULL)
    {
      printf("Unable to open analogue scenario\n");
      //exit(0);
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
      printf("Unable to open digital scenario\n");
      //exit(0);
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
  fclose(in)
;
  if(res1 > res2) res = res1;
  else res = res2;
  return(res);
}
