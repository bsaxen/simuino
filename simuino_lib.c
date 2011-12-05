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
    wprintw(win,"SIMUINO - Arduino UNO Pin Analyzer 0.1.0");
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
void putMsg(int line,const char *message)
//====================================
{
  wclear(msg);
  wmove(msg,line,1);
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
  wLog(err_msg,value,-1);
  error = 1;
}
//====================================
void addConfList(char *cf)
//====================================
{
  return;
}
//====================================
void saveConfig(char *cf)
//====================================
{
  FILE *out;
  time_t lt;

  out = fopen(cf,"w");
  if(out == NULL)
    {
      showError("No config file ",-1);
    }
  else
    {
      putMsg(msg_h-2,"Configuration saved");
      lt = time(NULL);
      fprintf(out,"# Simuino Configuration %s",ctime(&lt));
      fprintf(out,"# %s\n",cf);

      if(confSteps > MAX_STEP)confSteps = MAX_STEP; 
      fprintf(out,"SIM_LENGTH %d\n",confSteps);

      if(confWinMode >=0 && confWinMode < 4)
	fprintf(out,"WIN_LAYOUT %d\n",confWinMode);
      else
	confWinMode = 0;
      if(confDelay >=0 && confDelay < 1000)
	fprintf(out,"DELAY      %d\n",confDelay);
      else
	confDelay = 50;
      if(confLogLev >=0 && confLogLev < 4)
	fprintf(out,"LOG_LEVEL  %d\n",confLogLev);
      else
	confLogLev = 3;

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
  char temp[100],temp1[100];

  strcpy(temp,"");
  strcpy(temp1,"");

  if(value2 > -2) // add index
    {
	  sprintf(temp," %d,%d ",currentLoop,currentStep);
    }
  strcat(temp,p);
 

  if(value1 > -1)
    {
      sprintf(temp1," %d",value1);
      strcat(temp,temp1);
    }
  
  if(value2 > -1)
    {
      sprintf(temp1," %d",value2);
      strcat(temp,temp1);
    }

  if(error == 1)
    {
      temp[0]='*';
      error = 0;
    }

  if(confLogFile==YES)logFile(temp);


  wmove(slog,1,1);
  wprintw(slog,"%s",logBlankRow);
  wscrl(slog,-1);
  wmove(slog,1,1);
  wprintw(slog,">%s",simulation[currentStep+1]);
  wmove(slog,2,1);
  wprintw(slog,"%s",temp);
  show(slog);
}

//====================================
void mLog0(const char *p)
//====================================
{
  int i;
  char temp[100];

  //p = strstr(p," ");
  strcpy(temp,p);
  if(confLogFile==YES)logFile(temp);

  wmove(slog,1,1);
  wprintw(slog,"%s",logBlankRow);
  wscrl(slog,-1);
  wmove(slog,1,1);
  wprintw(slog,">%s",simulation[currentStep+1]);
  wmove(slog,2,1);
  wprintw(slog,"%s",temp);
  show(slog);
}

//====================================
void mLog1(const char *p, int value1)
//====================================
{
  int i;
  char temp[100], temp1[100];

  strcpy(temp,"");
  strcpy(temp1,"");
  strcat(temp,p);
  sprintf(temp1," %d",value1);
  strcat(temp,temp1);

  if(error == 1)
    {
      temp[0]='*';
      error = 0;
    }

  if(confLogFile==YES)logFile(temp);

  wmove(slog,1,1);
  wprintw(slog,"%s",logBlankRow);
  wscrl(slog,-1);
  wmove(slog,1,1);
  wprintw(slog,">%s",simulation[currentStep+1]);
  wmove(slog,2,1);
  wprintw(slog,"%s",temp);
  show(slog);
}

//====================================
void wLog0(const char *p)
//====================================
{
  int i;
  char temp[100];

  strcpy(temp,"");
  sprintf(temp," %d,%d ",currentLoop,currentStep);
  strcat(temp,p);

  if(error == 1)
    {
      temp[0]='*';
      error = 0;
    }

  if(confLogFile==YES)logFile(temp);

  wmove(slog,1,1);
  wprintw(slog,"%s",logBlankRow);
  wscrl(slog,-1);
  wmove(slog,1,1);
  wprintw(slog,">%s",simulation[currentStep+1]);
  wmove(slog,2,1);
  wprintw(slog,"%s",temp);
  show(slog);
}

//====================================
void wLog1(const char *p, int value1)
//====================================
{
  int i;
  char temp[100], temp1[100];

  strcpy(temp,"");
  strcpy(temp1,"");
  sprintf(temp," %d,%d ",currentLoop,currentStep);
  strcat(temp,p);
  sprintf(temp1," %d",value1);
  strcat(temp,temp1);

  if(error == 1)
    {
      temp[0]='*';
      error = 0;
    }

  if(confLogFile==YES)logFile(temp);

  wmove(slog,1,1);
  wprintw(slog,"%s",logBlankRow);
  wscrl(slog,-1);
  wmove(slog,1,1);
  wprintw(slog,">%s",simulation[currentStep+1]);
  wmove(slog,2,1);
  wprintw(slog,"%s",temp);
  show(slog);
}

//====================================
void wLog2(const char *p, int value1, int value2)
//====================================
{
  int i;
  char temp[100], temp1[100];

  strcpy(temp,"");
  strcpy(temp1,"");
  sprintf(temp," %d,%d ",currentLoop,currentStep);
  strcat(temp,p);
  sprintf(temp1," %d",value1);
  strcat(temp,temp1);
  sprintf(temp1," %d",value2);
  strcat(temp,temp1);

  if(error == 1)
    {
      temp[0]='*';
      error = 0;
    }

  if(confLogFile==YES)logFile(temp);

  wmove(slog,1,1);
  wprintw(slog,"%s",logBlankRow);
  wscrl(slog,-1);
  wmove(slog,1,1);
  wprintw(slog,">%s",simulation[currentStep+1]);
  wmove(slog,2,1);
  wprintw(slog,"%s",temp);
  show(slog);
}


//====================================
void unoInfo()
//====================================
{
  int next;

  wmove(uno,8,3); 
  wprintw(uno,"Sketch: %s",appName);

  if(g_warning == YES)
    wprintw(uno,"  MISMATCH? - load!");
  else
    wprintw(uno,"                   ");

  wmove(uno,10,3);
  next =  loopPos[currentLoop+1];
  if(currentStep == loopPos[currentLoop+1]) next = loopPos[currentLoop+2];
  wprintw(uno,"Step: %4d->%4d (%d,%4d)",currentStep,next,g_loops,g_steps);
  wmove(uno,11,3);
  wprintw(uno,"Scenario: %2d %2d %2d",scenDigital,scenAnalog,scenInterrupt);
  show(uno);
}

//====================================
void showLoops()
//====================================
{
  int i;
  wclear(msg);
  for(i=0;i<g_loops;i++)
  {
    wmove(msg,1+i,2);
    wprintw(msg," Loop: %4d starts at step:%4d",i,loopPos[i]+1);
  }
  show(msg);
}

//====================================
void wLogChar(const char *p, const char *value1, int value2)
//====================================
{
  int i;
  char temp[100],temp2[100];

  strcpy(temp," ");
  if(value2 > -2)  //add index 
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


  wmove(slog,1,1);
  wprintw(slog,"%s",logBlankRow);
  wscrl(slog,-1);
  wmove(slog,1,1);
  wprintw(slog,">%s",simulation[currentStep+1]);
  wmove(slog,2,1);
  wprintw(slog,"%s",temp);
}


//====================================
void showSerial(const char *m, int newLine)
//====================================
{
  int i,slen=0;

  if(serialMode == ON)
    {
      if(newLine == 1)
	{
	  slen = strlen(prevSerial);

	  wmove(ser,2,1+slen);
	  wprintw(ser,"%s",m);

	  wscrl(ser,-1);

	  wmove(ser,1,1);
	  wprintw(ser,"%s",serBlankRow);

	  strcpy(prevSerial,"");
	}
      else
	{
	  strcat(prevSerial,m);
	  wmove(ser,2,1);
	  wprintw(ser,"%s",prevSerial);
	}
      show(ser);
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
      strcpy(simComment[i],"");
      stepComment[i] = 0;
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
void readConfig(char *cf)
//====================================
{
  FILE *in;
  char row[80],*p,temp[40];
  int x;

  in = fopen(cf,"r");
  if(in == NULL)
    {
      showError("No config file",-1);
    }
  else
    {
      while (fgets(row,80,in)!=NULL)
	{
	  if(row[0] != '#')
	    {
	      if(p=strstr(row,"SIM_LENGTH"))
		{
		  sscanf(p,"%s%d",temp,&confSteps);
		}
	      if(p=strstr(row,"WIN_LAYOUT"))
		{
		  sscanf(p,"%s%d",temp,&confWinMode);
		}
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
int readComment(int step)
//====================================
{
  int i;
  
  if(step > 0 && step < MAX_STEP)
    {
      for(i=1;i<=stepComment[0];i++)
	{
	  if(stepComment[i] == step)
	    mLog0(simComment[i]);
	}
    } 
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
void runLoops(int loop)
//====================================
{
  return;
}    

//====================================
void runAll(int stop)
//====================================
{
  while(currentStep < stop)
    runStep(FORWARD);
  return;
}    

//====================================
void readSimulation(char *fileName)
//====================================
{
  FILE *in;
  char row[SIZE_ROW],*p,temp[SIZE_ROW],junk[5];
  int step=0,loop=0;

  g_steps       = 0;
  g_loops       = 0;
  g_comments    = 0;
  scenAnalog    = 0;
  scenDigital   = 0;
  scenInterrupt = 0;

  in = fopen(fileName,"r");
  if(in == NULL)
    {
      showError("No simulation file",-1);
    }
  else
    {
      while (fgets(row,SIZE_ROW,in)!=NULL)
	{

	  if(row[0] == '+')
	    {
	      p = strstr(row,"+ ");
              p = p+2;
	      g_steps++;
	      sscanf(row,"%s%d",junk,&step);
	      if(step == g_steps)
		strcpy(simulation[step],p);
	      else
		{
		  showError(row,step);
		  showError(row,g_steps);
		  //showError("Simulation step out of order",step);
		}
	    }
	  if(row[0] == '=')
	    {
	      g_comments++;
	      p = strstr(row,"= ");
              p = p+2;
	      sscanf(row,"%s%d",junk,&step);
              stepComment[0] = g_comments;
              if(step == g_steps)
		{
		  stepComment[g_comments] = step;
		  strcat(simComment[g_comments],p);
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
  putMsg(msg_h-2,"ready reading simulation");
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
void init(int mode)
//====================================
{
  int i;

  g_value = 0;

  // Down
  endwin();
  delwin(uno);
  delwin(ser);
  delwin(slog);
  delwin(msg);

  // Up
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
  wmove(uno,DP-1,RF+UNO_W-3);waddch(uno,ACS_URCORNER); 
  wmove(uno,AP+1,RF);waddch(uno,ACS_LLCORNER); 
  wmove(uno,AP+1,RF+UNO_W-3);waddch(uno,ACS_LRCORNER); 
  for(i=1;i<UNO_W-3;i++)
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
      wmove(uno,i,RF+UNO_W-3);
      waddch(uno,ACS_VLINE);
    }

  // Pin positions on the board
  for(i=0;i<14;i++)digPinPos[13-i] = RF+4+4*i;
  for(i=0;i<6;i++) anaPinPos[i] = RF+31+5*i;

  for(i=0;i<14;i++){wmove(uno,DP+1,digPinPos[i]-2); wprintw(uno,"%3d",i);}
  for(i=0;i<14;i++){wmove(uno,DP,digPinPos[i]); waddch(uno,ACS_BULLET);}

  for(i=0;i<6;i++){wmove(uno,AP-1,anaPinPos[i]-1); wprintw(uno,"A%1d",i);}
  for(i=0;i<6;i++){wmove(uno,AP,anaPinPos[i]); waddch(uno,ACS_BULLET);}

  show(uno);

  // Message Window
  msg_h = s_row - uno_h;
  msg_w = MSG_W;
  msg=newwin(msg_h,msg_w,uno_h,0);
  wbkgd(msg,COLOR_PAIR(MSG_COLOR));
  show(msg);

  if(mode == 0) // side by side
    {
      // Log Window
      log_h = s_row;
      log_w = LOG_W;
      slog=newwin(log_h,log_w,0,uno_w);
      scrollok(slog,true);
      wbkgd(slog,COLOR_PAIR(LOG_COLOR));
      show(slog); 
      
      // Serial Window   
      ser_h = s_row;
      ser_w = s_col - uno_w - log_w;
      ser=newwin(ser_h,ser_w,0,uno_w + log_w);
      scrollok(ser,true);
      wbkgd(ser,COLOR_PAIR(SER_COLOR));
      show(ser);
    }

  if(mode == 1) // 50 on 50
    {
      // Log Window
      log_h = s_row/2;
      log_w = s_col-uno_w;
      slog=newwin(log_h,log_w,0,uno_w);
      scrollok(slog,true);
      wbkgd(slog,COLOR_PAIR(LOG_COLOR));
      show(slog); 
      
      // Serial Window      
      ser_h = s_row/2+1;
      ser_w = s_col-uno_w;
      ser=newwin(ser_h,ser_w,s_row/2,uno_w);
      scrollok(ser,true);
      wbkgd(ser,COLOR_PAIR(SER_COLOR));
      show(ser);

    }

  if(mode == 2) // 90 on 10
    {
      // Log Window
      log_h = s_row-10;
      log_w = s_col-uno_w;
      slog=newwin(log_h,log_w,0,uno_w);
      scrollok(slog,true);
      wbkgd(slog,COLOR_PAIR(LOG_COLOR));
      show(slog); 

      // Serial Window      
      ser_h = 10;
      ser_w = s_col-uno_w;
      ser=newwin(ser_h,ser_w,log_h,uno_w);
      scrollok(ser,true);
      wbkgd(ser,COLOR_PAIR(SER_COLOR));
      show(ser);
    }

  if(mode == 3) // 10 on 90
    {      
      // Log Window
      log_h = 10;
      log_w = s_col-uno_w;
      slog=newwin(log_h,log_w,0,uno_w);
      scrollok(slog,true);
      wbkgd(slog,COLOR_PAIR(LOG_COLOR));
      show(slog); 

      // Serial Window      
      ser_h = s_row-10;
      ser_w = s_col-uno_w;
      ser=newwin(ser_h,ser_w,log_h,uno_w);
      scrollok(ser,true);
      wbkgd(ser,COLOR_PAIR(SER_COLOR));
      show(ser);
    }

  for(i=0;i<log_w;i++)logBlankRow[i] = ' ';logBlankRow[i]='\0';
  for(i=0;i<ser_w;i++)serBlankRow[i] = ' ';serBlankRow[i]='\0';
}
//====================================
void loadSketch(char sketch[])
//====================================
{
  int x;
  char syscom[120], fileName[80];

  sprintf(syscom,"cp %s servuino/sketch.pde;",sketch);
  x=system(syscom);
  strcpy(confSketchFile,sketch);
  sprintf(syscom,"cd servuino; g++ -O2 -o servuino servuino.c > g++.result 2>&1");
  x=system(syscom);
  strcpy(fileName,"servuino/g++.result");
  readMsg(fileName);
  iDelay(3000);
  readSketchInfo();
}

//====================================
// End of file
//====================================
