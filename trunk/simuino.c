//================================================
//  Developed by Benny Saxén
//================================================

#define HIST_MAX  100

#define DP 5
#define AP 15
#define RF 0
#define ER 1
#define SR 20

#define ON     1
#define OFF    0

#define YES    10
#define NO     20

// Configuration
int pinServer = NO;
int interface_id = 1;
int nloop = 0;

#include "simuino_lib.c"
#include "arduino_lib.c"
#include "sketch/sketch.pde"


// ----- Commands ----------
void selPin()
{
}
void incValue()
{
}
void decValue()
{
}

void runSim(int n)
{
  int i;
  for(i=0;i<n;i++)
    {
      nloop++;
      loop();
      Serial.flush();
      passTime();  
    }
}    
  
//========================================
int main(int argc, char *argv[])
{
  int i,x;
  int ch;
  int nhist;

  getAppName(appName);

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
  
/*     COLOR_BLACK   0 */
/*     COLOR_RED     1 */
/*     COLOR_GREEN   2 */
/*     COLOR_YELLOW  3 */
/*     COLOR_BLUE    4 */
/*     COLOR_MAGENTA 5 */
/*     COLOR_CYAN    6 */
/*     COLOR_WHITE   7 */

// Board Window    
  uno=newwin(AP+4,70,0,0);
  wbkgd(uno,COLOR_PAIR(3));

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
  wmove(uno,DP+5,RF+6); wprintw(uno,"Application: %s",appName);  
  for(i=0;i<6;i++){wmove(uno,AP-1,anaPinPos[i]-1); wprintw(uno,"A%1d",i);}
  for(i=0;i<6;i++){wmove(uno,AP,anaPinPos[i]); waddch(uno,ACS_BULLET);}

  wmove(uno,0,5); 
  wprintw(uno,"SIMUINO - Arduino UNO Pin Analyzer Version: 2011-11-03");
  wrefresh(uno);

  // Serial Window
  ser=newwin(5,70,AP+5,0);
  wbkgd(ser,COLOR_PAIR(5));
  wmove(ser,0,0); 
  wprintw(ser,"[Serial Interface]");
  wrefresh(ser);

  // Log Window
  logSize = row;
  slog=newwin(row,29,0,col-30);
  wbkgd(slog,COLOR_PAIR(4));
  wrefresh(slog);

  // Command Window
  com=newwin(AP+10,20,0,72);
  wbkgd(com,COLOR_PAIR(4));
  wmove(com,0,0);
  if(pinServer==YES)
    {
      wprintw(com,"Server YES");
      strcpy(url,"localhost");
      conn = c_connect(PIN_PORT,url);
      if(conn == 0)pinServer = NO;
    }
  if(pinServer==NO)wprintw(com,"Server NO ");
  wrefresh(com);


  boardInit();
  setup();
  nhist = readExt();
  while((ch!='q')&&(ch!='Q'))  
    {
      x = kbhit();
      if(x) 
	{
	  ch = readch();
	  wmove(com,1,2);
	  wprintw(com,"%c loop %d (%d)",ch,nloop,nhist);
	  wrefresh(com);
	  //if (ch=='s') selPin();
	  //if (ch=='+') incValue();
	  //if (ch=='-') decValue();
	  if (ch=='r') runSim(1);
	  if (ch=='g') runSim(nhist);
	  // if (ch=='c') connect(interface_id);
	  //if (ch=='d') disconnect(interface_id);
	}     
    }
  c_disconnect(conn);
  tcsetattr(0,TCSANOW, &orig);
  delwin(uno);
  endwin();
}
