//================================================
//  Developed by Benny Saxen, ADCAJO
//================================================

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
//#define SCEN_MAX 100
#define LOG_MAX  200
#define MAX_STEP 2000
#define MAX_LOOP 2000
#define MAX_PIN_ANALOG 6
#define MAX_PIN_DIGITAL 14

#define DP 5
#define AP 15
#define RF 0
#define ER 1
#define SR 20

#define ON     1
#define OFF    0

#define YES    1
#define NO     2

// Configuration
int currentStep = 0;
int currentLoop = 0;
int g_loops=0,g_steps=0;

int g_value = 0;

void (*interrupt0)();
void (*interrupt1)();

char  simulation[MAX_STEP][80];
int   loopPos[MAX_LOOP];

#include "simuino_lib.c"
#include "decode_lib.c"


//====================================
int readEvent(char *ev, int step)
//====================================
{
  if(step > 0 && step < MAX_STEP)
    strcpy(ev,simulation[step]);
  else
    return(0);

  // wLog(ev,step,-1);
  return(step);
}    

//====================================
void runStep(int dir)
//====================================
{
  char row[120],event[80],temp[80],mode[5],*p,temp2[80],temp3[80];
  int res  = 0,value = 0;
  int step = 0,pin,x,y;

  if(dir == FORWARD)currentStep++;
  if(dir == BACKWARD)currentStep--;

  if(currentStep > g_steps)return; // Not possible to step beyond simulation length

  if(currentStep > loopPos[currentLoop+1] && loopPos[currentLoop+1] !=0)
    {
      showConfig();
      currentLoop++;
    }

  res = readEvent(event,currentStep);
  if(res != 0)
    {
      showConfig();
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
	  sscanf(event,"%d %s %s",&step,temp,temp2);
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
	  sscanf(event,"%d %s %s",&step,temp,temp2);
	  getString(event,temp3);
	  Serial.println(temp3);
	}
      else if (p=strstr(event,"delay"))
	{
	  sscanf(event,"%d %s %d",&step,temp,&x);
	  delay(x);
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
      else
	unimplemented(temp);

    }
  else
    showError("Unable to step ",currentStep);

  iDelay(confDelay);
  return;
}    

//====================================
void runLoop()
//====================================
{
  runStep(FORWARD);

  if(currentLoop ==  g_loops)
    {
      while(currentStep < g_steps)
	runStep(FORWARD);
    }

  else if(currentLoop >= 0 && currentLoop < g_loops)
    {
      while(currentStep < loopPos[currentLoop+1])
	runStep(FORWARD);
    }


  return;
}    

//====================================
void runAll()
//====================================
{
  while(currentStep < g_steps)
    runStep(FORWARD);
  return;
}    

//====================================
void readSimulation(char *fileName)
//====================================
{
  FILE *in;
  char row[80],*p,temp[40];
  int step=0,loop=0;


  g_steps = 0;
  g_loops = 0;
  in = fopen(fileName,"r");
  if(in == NULL)
    {
      showError("Unable to open simulation file",-1);
    }
  else
    {
      while (fgets(row,80,in)!=NULL)
	{
	  if(row[0] != '#')
	    {
	      g_steps++;
	      sscanf(row,"%d",&step);
	      if(step == g_steps)
		strcpy(simulation[step],row);
	      else
		{
		  showError("Simulation step out of order",step);
		}
	    }
	  else if(p=strstr(row,"LOOP"))
	    {
	      sscanf(p,"%s%d",temp,&loop);
	      loopPos[loop] = step;
	      g_loops++;
	    }  
	}
    }
  fclose(in);
  g_loops--;
  return;
}    

//====================================
void readHelp(char *fileName)
//====================================
{
  FILE *in;
  char row[80];
  int i=0,leave=0;

  wclear(hlp);
  in = fopen(fileName,"r");
  if(in == NULL)
    {
      showError("Unable to open help file",-1);
    }
  else
    {
      while (fgets(row,80,in)!=NULL && i < s_row && leave == 0)
	{
	  if(strstr(row,"# STATUS 0"))leave = 1; // Sketch info case
	  wmove(hlp,i,0);
	  wprintw(hlp,row);
	  i++;
	}
      show(hlp);
    }
  fclose(in);
  return;
}    


//====================================
void init(char *fileName)
//====================================
{
  int i;

  currentStep = 0;
  currentLoop = 0;
  g_loops=0;
  g_steps=0;
  g_value = 0;

  boardInit();
  readSketchInfo(fileName);

  initscr();
  clear();
  //noecho();
  cbreak();

  //tcgetattr(0, &orig);
  //nnew = orig;
  //nnew.c_lflag &= ~ICANON;
  //nnew.c_lflag &= ~ECHO;
  //nnew.c_lflag &= ~ISIG;
  //nnew.c_cc[VMIN] = 1;
  //nnew.c_cc[VTIME] = 0;
  //tcsetattr(0, TCSANOW, &nnew);

  getmaxyx(stdscr,s_row,s_col);
  start_color();
  init_pair(1,COLOR_BLACK,COLOR_BLUE);
  init_pair(2,COLOR_BLACK,COLOR_GREEN);
  init_pair(3,COLOR_BLUE,COLOR_WHITE); 
  init_pair(4,COLOR_RED,COLOR_WHITE); 
  init_pair(5,COLOR_MAGENTA,COLOR_WHITE); 
  init_pair(6,COLOR_WHITE,COLOR_BLACK); 
  
  /*     COLOR_BLACK   0 */
  /*     COLOR_RED     1 */
  /*     COLOR_GREEN   2 */
  /*     COLOR_YELLOW  3 */
  /*     COLOR_BLUE    4 */
  /*     COLOR_MAGENTA 5 */
  /*     COLOR_CYAN    6 */
  /*     COLOR_WHITE   7 */

  // Board Window    
  uno=newwin(AP+3,61,0,0);
  wbkgd(uno,COLOR_PAIR(6));

  wmove(uno,DP-1,RF);waddch(uno,ACS_ULCORNER); 
  wmove(uno,DP-1,RF+60);waddch(uno,ACS_URCORNER); 
  wmove(uno,AP+1,RF);waddch(uno,ACS_LLCORNER); 
  wmove(uno,AP+1,RF+60);waddch(uno,ACS_LRCORNER); 
  for(i=1;i<60;i++)
    {
      wmove(uno,DP-1,RF+i);
      waddch(uno,ACS_HLINE);
      wmove(uno,AP+1,RF+i);
      waddch(uno,ACS_HLINE);
    }
  for(i=DP;i<AP+1;i++)
    {
      wmove(uno,i,RF);
      waddch(uno,ACS_VLINE);
      wmove(uno,i,RF+60);
      waddch(uno,ACS_VLINE);
    }

  // Pin positions on the board
  for(i=0;i<14;i++)digPinPos[13-i] = RF+4+4*i;
  for(i=0;i<6;i++) anaPinPos[i] = RF+26+5*i;

  for(i=0;i<14;i++){wmove(uno,DP+1,digPinPos[i]-2); wprintw(uno,"%3d",i);}
  for(i=0;i<14;i++){wmove(uno,DP,digPinPos[i]); waddch(uno,ACS_BULLET);}
  wmove(uno,DP+5,RF+6); wprintw(uno,"Sketch: %s",appName);  
  for(i=0;i<6;i++){wmove(uno,AP-1,anaPinPos[i]-1); wprintw(uno,"A%1d",i);}
  for(i=0;i<6;i++){wmove(uno,AP,anaPinPos[i]); waddch(uno,ACS_BULLET);}

  wmove(uno,0,5); 
  wprintw(uno,"SIMUINO - Arduino UNO Pin Analyzer 0.0.2");
  wrefresh(uno);

  // Serial Window
  serialSize = s_row;
  ser=newwin(s_row,s_col,0,61+40+2);
  wbkgd(ser,COLOR_PAIR(3));
  wrefresh(ser);

  // Log Window
  logSize = s_row;
  slog=newwin(s_row,40,0,62);
  wbkgd(slog,COLOR_PAIR(6));
  wrefresh(slog);

  // Message Window
  com=newwin(12,61,AP+4,0);
  wbkgd(com,COLOR_PAIR(6));
  wrefresh(com);

  // Help Window
  hlp=newwin(s_row,61,AP+4+13,0);
  wbkgd(hlp,COLOR_PAIR(6));
  wrefresh(hlp);
 
  readSimulation(fileName);
  showConfig();
}
//====================================
void openCommand()
//====================================
{
  int ch;
  char str[80];
  while(strstr(str,"ex") == NULL)
  {
    wmove(uno,DP+6,RF+5);
    wprintw(uno,"                                    ");
    //wclear(uno);
    //mvwgetstr(com,30,0,str);
    mvwprintw(uno,DP+6,RF+6,">>");
    //mvwprintw(com,3,0,"c1>");
    //mvwprintw(ser,30,0,"c3>");
    wrefresh(uno);
    //wrefresh(com);
    //wrefresh(ser);    
    wgetstr(uno,str);
    //ch = getchar();
    wLog(str,-1,-1);
  }
  wmove(uno,DP+6,RF+5);
  wprintw(uno,"                                    ");
  wrefresh(uno);
}

//====================================
int main(int argc, char *argv[])
//====================================
{
  int ch;
  char fileName[80],tempName[80];

  strcpy(tempName,"help.txt");

  if (argc != 2)
    {
      printf("Usage: termuino <servuino data file> \n");
      exit(0);
    }

  strcpy(fileName,argv[1]);

  init(fileName);
  readConfig();
  showConfig();
  if(confLogFile == YES)resetFile("log.txt");
  readHelp(tempName);

  while((ch!='q')&&(ch!='x'))  
    {
      ch = getchar();

      if (ch=='c')
        {
          openCommand();
        }

      if (ch=='h')
	{
	  readHelp(tempName);
	}
      if (ch=='i')
	{
	  readHelp(fileName);
	}
      if (ch=='g')
	{
	  runAll();
	}
      if (ch=='a')
	{
          init(fileName);
	}
      if (ch=='r')
	{
	  runLoop();
	}
      if (ch=='s') 
	{
	  runStep(FORWARD);
	}
      if (ch=='b') 
	{
	  runStep(BACKWARD); 
	}
      if (ch=='l') 
	{
	  confLogLev++;
	  if(confLogLev > 3)confLogLev = 0;
	  showConfig();
	}
      if (ch=='+') 
	{
	  confDelay = confDelay + 10;
	  showConfig();
	}
      if (ch=='-') 
	{
	  confDelay = confDelay - 10;
	  if(confDelay < 0)confDelay = 0;
          if(confDelay > 1000)confDelay = 1000;
	  showConfig();
	}
      if (ch=='f') 
	{
	  confLogFile++;
	  if(confLogFile > 1)confLogFile = 0;
	  showConfig();
	}
    }
  
  //tcsetattr(0,TCSANOW, &orig);
  delwin(uno);
  delwin(ser);
  delwin(slog);
  delwin(hlp);
  endwin();
}
//====================================
// End of file
//====================================

