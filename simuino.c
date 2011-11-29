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
#define MAX_SERIAL 100
#define MAX_LOG  200
#define MAX_STEP 2000
#define MAX_LOOP 2000
#define MAX_PIN_ANALOG 6
#define MAX_PIN_DIGITAL 14

#define UNO_H  19
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
char  fileServuino[80];

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
//      unoInfo();
      currentLoop++;
    }

  res = readEvent(event,currentStep);
  if(res != 0)
    {
//wLog(event,step,8);
 //     unoInfo();
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

      //wLog("-----",-1,-1);
      show(slog);
    }
  else
    showError("Unable to step ",currentStep);

  unoInfo();
  //iDelay(confDelay);
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
          else if(p=strstr(row,"SCENARIODATA"))
            {
              sscanf(p,"%s%d%d%d",temp,&scenDigital,&scenAnalog,&scenInterrupt);
            }  
	}
    }
  fclose(in);
  g_loops--;
  return;
}    

//====================================
void showScenario(char *fileName)
//====================================
{
  FILE *in;
  char row[80];
  int i=0;

  wclear(msg);
  in = fopen(fileName,"r");
  if(in == NULL)
    {
      showError("Unable to open scenario file",-1);
    }
  else
    {
      while (fgets(row,120,in)!=NULL && i < s_row-1)
        {
          if(strstr(row,"// SCEN"))
          {
            i++;
            wmove(msg,i,1);
            wprintw(msg,row);
          }
        }
      show(msg);
    }
  fclose(in);
  return;
}

//====================================
void readMsg(char *fileName)
//====================================
{
  FILE *in;
  char row[80];
  int i=0;

  wclear(msg);
  in = fopen(fileName,"r");
  if(in == NULL)
    {
      showError("Unable to open msg file",-1);
    }
  else
    {
      while (fgets(row,120,in)!=NULL && i < s_row-1)
	{
          i++;
	  wmove(msg,i,1);
	  wprintw(msg,row);
	}
      show(msg);
    }
  fclose(in);
  return;
}    


//====================================
void init(char *fileName)
//====================================
{
  int i;
  int uno_h=0, uno_w=0;
  int msg_h=0, msg_w=0;
  int log_h=0, log_w=0;
  int ser_h=0, ser_w=0;

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
  //uno=newwin(AP+3,61,0,0);
  uno_w = UNO_W;
  uno_h = UNO_H;
  uno=newwin(uno_h,uno_w,0,0);
  wbkgd(uno,COLOR_PAIR(UNO_COLOR));
  box(uno, 0 , 0);

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
  unoInfo();

  for(i=0;i<6;i++){wmove(uno,AP-1,anaPinPos[i]-1); wprintw(uno,"A%1d",i);}
  for(i=0;i<6;i++){wmove(uno,AP,anaPinPos[i]); waddch(uno,ACS_BULLET);}

  wmove(uno,1,5); 
  wprintw(uno,"SIMUINO - Arduino UNO Pin Analyzer 0.0.4");
  show(uno);

  // Message Window
  msg_h = s_row - uno_h;
  msg_w = MSG_W;
  msg=newwin(msg_h,msg_w,uno_h,0);
  wbkgd(msg,COLOR_PAIR(MSG_COLOR));
  show(msg);

  // Help Window
  //hlp=newwin(s_row-(AP+3)-6,61,AP+4+6,0);
  //wbkgd(hlp,COLOR_PAIR(MSG_COLOR));
  //wrefresh(hlp);

  // Log Window
  logSize = s_row-1;
  log_h = s_row;
  log_w = LOG_W;
  slog=newwin(log_h,log_w,0,uno_w);
  wbkgd(slog,COLOR_PAIR(LOG_COLOR));
  show(slog); 

  // Serial Window
  serialSize = s_row-1;
  ser_h = s_row;
  ser_w = s_col - uno_w - log_w;
  ser=newwin(ser_h,ser_w,0,uno_w + log_w);
  wbkgd(ser,COLOR_PAIR(SER_COLOR));
  show(ser);

  readSimulation(fileName);
  unoInfo();
}
//====================================
void openCommand()
//====================================
{
  int ch;
  char *p,str[80],fileName[80],temp[80];

  strcpy(fileName,"help_command.txt");
  readMsg(fileName);

  while(strstr(str,"ex") == NULL)
  {
    wmove(uno,UNO_H-2,1);
    wprintw(uno,"              ");
    mvwprintw(uno,UNO_H-2,1,">>");
    show(uno);
    wgetstr(uno,str);
    if(p=strstr(str,"conf"))
      {
	strcpy(fileName,"config.txt");
        readMsg(fileName);
      }
    if(p=strstr(str,"help"))
      {
        strcpy(fileName,"help_command.txt");
        readMsg(fileName);
      }
    if(p=strstr(str,"delay"))
      {
       sscanf(p,"%s %d",temp,&confDelay);
       if(confDelay >=0 && confDelay < 1000)
          saveConfig();
      }
    if(p=strstr(str,"log"))
      {
       sscanf(p,"%s %d",temp,&confLogLev);
       if(confLogLev >=0 && confLogLev < 4)
          saveConfig();
      }
    if(p=strstr(str,"file"))
      {
       sscanf(p,"%s %d",temp,&confLogFile);
       if(confLogFile >=0 && confLogFile < 2)
          saveConfig();
      }
    if(p=strstr(str,"scen"))
      {
          showScenario(fileServuino);
      }

  }
  wmove(uno,UNO_H-2,1);
  wprintw(uno,"                ");	
  show(uno);
  strcpy(fileName,"help.txt");
  readMsg(fileName);
}

//====================================
int main(int argc, char *argv[])
//====================================
{
  int ch;
  char tempName[80];

  strcpy(tempName,"help.txt");

  if (argc != 2)
    {
      printf("Usage: simuino <servuino data file> \n");
      exit(0);
    }

  strcpy(fileServuino,argv[1]);

  init(fileServuino);
  readConfig();
  //showConfig();
  if(confLogFile == YES)resetFile("log.txt");
  readMsg(tempName);

  while((ch!='q')&&(ch!='x'))  
    {
      ch = getchar();

      if (ch=='c')
        {
          openCommand();
        }
      if (ch=='h')
	{
	  readMsg(tempName);
	}
      if (ch=='g')
	{
	  runAll();
	}
      if (ch=='a')
	{
          init(fileServuino);
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
	  // Todo save to config.txt
	}
      if (ch=='+') 
	{
	  confDelay = confDelay + 10;
	  // Todo save to config.txt
	}
      if (ch=='-') 
	{
	  confDelay = confDelay - 10;
	  if(confDelay < 0)confDelay = 0;
          if(confDelay > 1000)confDelay = 1000;
	  // Todo save to config.txt
	}
      if (ch=='f') 
	{
	  confLogFile++;
	  if(confLogFile > 1)confLogFile = 0;
	  // Todo save to config.txt
	}
    }
  
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

