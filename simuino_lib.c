//================================================
//  Developed by Benny Saxen, ADCAJO
//================================================

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
  if(win == uno) 
  {
    wmove(win,0,2);
    wprintw(win,"SIMUINO - Arduino UNO Pin Analyzer 0.0.5");
  }
  if(win == ser)
  {
    wmove(win,0,2);
    wprintw(win,"Serial Interface");
  }
  if(win == slog)
  {
    wmove(win,0,2);
    wprintw(win,"Log");
  }
  if(win == msg)
  {
    wmove(win,0,2);
    wprintw(win,"Messages");
  }

  wrefresh(win);
  iDelay(confDelay);
}

//====================================
void putMsg(const char *message)
//====================================
{
  wclear(msg);
  wmove(msg,1,1);
  wprintw(msg,message);
  show(msg);
  return;
}

//====================================
void showError(const char *m, int value)
//====================================
{
  char err_msg[300];
  strcpy(err_msg,"ERROR ");
  strcat(err_msg,m);
  //putMsg(err_msg);
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
      showError("No config.txt ",-1);
    }
  else
    {
      putMsg("Configuration saved");
      lt = time(NULL);
      fprintf(out,"# Simuino Configuration %s",ctime(&lt));
      fprintf(out,"DELAY      %d\n",confDelay);
      fprintf(out,"LOG_LEVEL  %d\n",confLogLev);
      fprintf(out,"LOG_FILE   %d\n",confLogFile);
      fprintf(out,"SKETCH     %s\n",confSketchFile);
      fprintf(out,"SERVUINO   %s\n",confServuinoFile);
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
      fclose(out);
    }


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
  if(scenDigital>0 || scenAnalog>0 || scenInterrupt>0)
  {
     wmove(uno,9,0);
     wprintw(uno," Scenario: Dig=%d Ana=%d Interrupt=%d",scenDigital,scenAnalog,scenInterrupt);
  }
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
void readSketchInfo()
//====================================
{
  FILE *in;
  char row[80],res[40],*p,*q,value[5];
  int pin;

  //wLog("inside",1,2);

  in = fopen("servuino/sketch.pde","r");
  if(in == NULL)
    {
      showError("No servuino/sketch.pde",-1);
    }
  else
    {
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
    }
}

//====================================
void initSim()
//====================================
{
  int i;

  strcpy(appName,"-");

  for(i=0;i<MAX_STEP;i++)
    {
      strcpy(simulation[i],"");
    }
  for(i=0;i<MAX_LOOP;i++)
    {
      loopPos[i] = 0;
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
void resetSim()
//====================================
{
  int i;

  currentStep = 0;
  currentLoop = 0;

  for(i=0;i<MAX_LOG;i++)
    {
       strcpy(logBuffer[i],"");
    }
  for(i=0;i<MAX_SERIAL;i++)
    {
      strcpy(serialBuffer[i],"");
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
      showError("No config.txt",-1);
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
              if(p=strstr(row,"SKETCH"))
                {
                  sscanf(p,"%s%s",temp,confSketchFile);
                }
              if(p=strstr(row,"SERVUINO"))
                {
                  sscanf(p,"%s%s",temp,confServuinoFile);
                }
	    }
	 
	}
    }
  fclose(in);
}
//====================================
int readEvent(char *ev, int step)
//====================================
{
  if(step > 0 && step < MAX_STEP)
    strcpy(ev,simulation[step]);
  else
    return(0);
  return(step);
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
  char row[SIZE_ROW],*p,temp[40];
  int step=0,loop=0;

  g_steps = 0;
  g_loops = 0;
  in = fopen(fileName,"r");
  if(in == NULL)
    {
      showError("No simulation file",-1);
    }
  else
    {
      while (fgets(row,SIZE_ROW,in)!=NULL)
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
        g_loops--;
        fclose(in);
    }
 putMsg("x2");
  putMsg("ready reading simulation");
  return;
}    

//====================================
void showScenario(char *fileName)
//====================================
{
  FILE *in;
  char row[SIZE_ROW];
  int i=0;

  wclear(msg);
  in = fopen(fileName,"r");
  if(in == NULL)
    {
      showError("Unable to open scenario file",-1);
    }
  else
    {
      while (fgets(row,SIZE_ROW,in)!=NULL && i < s_row-1)
        {
          if(strstr(row,"// SCEN"))
          {
            i++;
            wmove(msg,i,1);
            wprintw(msg,row);
          }
        }
      if(i == 0)
      {
          wmove(msg,1,1); wprintw(msg,"No scenario data in sketch");
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
  char row[SIZE_ROW];
  int i=0;

  wclear(msg);
  in = fopen(fileName,"r");
  if(in == NULL)
    {
      showError("Unable to open msg file",-1);
    }
  else
    {
      while (fgets(row,SIZE_ROW,in)!=NULL && i < s_row-1)
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
void init()
//====================================
{
  int i;
  int uno_h=0, uno_w=0;
  int msg_h=0, msg_w=0;
  int log_h=0, log_w=0;
  int ser_h=0, ser_w=0;

  g_value = 0;

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
  uno_w = UNO_W;
  uno_h = UNO_H;
  uno=newwin(uno_h,uno_w,0,0);
  wbkgd(uno,COLOR_PAIR(UNO_COLOR));
  //box(uno, 0 , 0);

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

  //wmove(uno,1,5); 
  //wprintw(uno,"SIMUINO - Arduino UNO Pin Analyzer 0.0.5");
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
}
//====================================
void loadSketch(char sketch[])
//====================================
{
  int x;
  char syscom[120];

  sprintf(syscom,"cp %s servuino/sketch.pde;",sketch);
  x=system(syscom);
  strcpy(confSketchFile,sketch);
  sprintf(syscom,"cd servuino; g++ -O2 -o servuino servuino.c -lncurses;");
  x=system(syscom);
  readSketchInfo();
}

//====================================
// End of file
//====================================
