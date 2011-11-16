//================================================
//  Developed by Benny Saxen, ADCAJO
//================================================

//------ Constants -------------------------
#define LOW    0
#define HIGH   1
#define INPUT  1
#define OUTPUT 2

#define BYTE   1
#define BIN    2
#define OCT    3
#define DEC    4
#define HEX    5

#define CHANGE  1
#define RISING  2
#define FALLING 3


#define STEP_MAX  1000
#define LOOP_MAX  100

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
int stepStep = 0;


// Init
int nloop         = 0;
int timeFromStart = 0;

void (*interrupt0)();
void (*interrupt1)();


void stepCommand();

#include "simuino_lib.c"
#include "arduino_lib.c"
#include "sketch/sketch.pde"


// ----- Commands ----------

void runSim(int n)
{
  int i;
  int currentLoop = nloop;

  if(n == 1)
    {
      nloop++;
      if(nloop < LOOP_MAX)loop();
    }
  else if(n > 1)
    {
      for(i=0;i<n-currentLoop;i++)
	{
	  nloop++;
	  if(nloop < LOOP_MAX)loop();
	}
    }
  return;
}    

void stepCommand()
{
  int ch;

  if(stepStep == 1)
    {
      ch = getchar();
      if (ch=='g')stepStep = 0;
    }
}


//========================================
int main(int argc, char *argv[])
{
  int i,x;
  int ch;
  int nhist;




  getAppName(appName);

  //setStartTime();

  initscr();
  clear();
  noecho();
  cbreak();

  /* record kb modes */
  tcgetattr(0, &orig);
  nnew = orig;
  nnew.c_lflag &= ~ICANON;
  nnew.c_lflag &= ~ECHO;
  nnew.c_lflag &= ~ISIG;
  nnew.c_cc[VMIN] = 1;
  nnew.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &nnew);

  getmaxyx(stdscr,row,col);
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
  wprintw(uno,"SIMUINO - Arduino UNO Pin Analyzer v0.9");
  wrefresh(uno);

  // Serial Window
  serialSize = row;
  ser=newwin(row,col,0,61+30+2);
  wbkgd(ser,COLOR_PAIR(3));
  wrefresh(ser);

  // Log Window
  logSize = row;
  slog=newwin(row,30,0,62);
  wbkgd(slog,COLOR_PAIR(4));
  wrefresh(slog);

  // Message Window
  com=newwin(5,61,AP+4,0);
  wbkgd(com,COLOR_PAIR(6));
  wrefresh(com);


  boardInit();
  readSketchInfo();
  readConfig();
  showConfig();
  if(confLogFile == YES)resetFile("log.txt");

  setup();

  nhist = readExt();
  wmove(com,0,0);
  wprintw(com,"Scenario Loops=%d Interrupts=%d ",nhist,nInterrupts);
  wrefresh(com); 

  while((ch!='q')&&(ch!='Q'))  
    {
      wLog("--- Loop: ",nloop+1,-2);
      ch = getchar();
      wmove(com,1,1);

      if (ch=='r')
	{
	  wrefresh(com); 
	  runSim(1);
	}
      if (ch=='s') 
	{
	  stepStep = 1;
	  runSim(1); 
	}
      if (ch=='g')
	{
	  runSim(nhist);   
	}
    }
  
  tcsetattr(0,TCSANOW, &orig);
  delwin(uno);
  endwin();
}


