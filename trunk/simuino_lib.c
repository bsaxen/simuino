/*  Simuino is a Arduino Simulator based on Servuino Engine
    Copyright (C) 2011  Benny Saxen

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/


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
int analyzeEvent(char *event)
//====================================
{
  int step,pin,value;
  char str[80];
  {
    if(strstr(event,"analogRead") || strstr(event,"digitalRead"))
      {
	sscanf(event,"%d %s %d %d",&step,str,&pin,&value);
	g_pinNo = pin;
	g_pinValue = value;
	if(strstr(event,"analog"))  g_pinType = ANA;
	if(strstr(event,"digital")) g_pinType = DIG;
	if(currentStep+1 != step) putMsg(5,"Step number mismatch in analyzeEvent");
	return(g_pinType);
      } 
    return(0); 
  }
}
//====================================
void show(WINDOW *win)
//====================================
{
  int next;

  box(win,0,0);
  if(win == uno) 
    {
      wmove(win,0,2);
      if(confBoardType ==UNO)
	wprintw(win,"SIMUINO - Arduino UNO Pin Analyzer 0.1.4");
      if(confBoardType ==MEGA)
	wprintw(win,"SIMUINO - Arduino MEGA Pin Analyzer 0.1.4");
      wmove(win,1,2);
      wprintw(uno,"Sketch: %s",appName);
    }
  if(win == ser)
    {
      wmove(win,0,2);
      wprintw(win,"Serial Interface");
    }
  if(win == slog)
    {
      next =  loopPos[currentLoop+1];
      if(currentStep == loopPos[currentLoop+1]) next = loopPos[currentLoop+2];
      wmove(win,0,2);
      wprintw(win,"Log  ");
      if(currentStep == g_steps)
	wprintw(slog,"%4d ->| (%d,%4d)",currentStep,g_loops,g_steps);
      else
	wprintw(slog,"%4d ->%4d (%d,%4d)",currentStep,next,g_loops,g_steps);
    }
  if(win == msg)
    {
      wmove(win,0,2);
      wprintw(win,"Messages");
    }
  
  wmove(uno,board_h-2,4);
  wprintw(uno,"                  ");
  wmove(uno,board_h-2,4);
  wrefresh(uno);
  wrefresh(win);
  //iDelay(confDelay);
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
void errorLog(const char msg[], int x)
//====================================
{
  showError(msg,x);
}
//====================================
void showError(const char *m, int value)
//====================================
{
  char err_msg[300];
  strcpy(err_msg,"SimuinoERROR: ");
  strcat(err_msg,m);
  fprintf(err,"%s %d\n",err_msg,value,-1);
  error = 1;
}
//====================================
void addConfList(char *cf)
//====================================
{
  return;
}
//====================================
void delConfList(char *cf)
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

      //fprintf(out,"BOARD_TYPE %d\n",confBoardType);

      if(confSteps > MAX_STEP)confSteps = MAX_STEP; 
      fprintf(out,"SIM_LENGTH %d\n",confSteps);

      if(confWinMode >=0 && confWinMode <= WIN_MODES)
	fprintf(out,"WIN_LAYOUT %d\n",confWinMode);
      else
	confWinMode = 0;
      if(confDelay >=0 && confDelay < 1000)
	fprintf(out,"DELAY      %d\n",confDelay);
      else
	confDelay = 50;

      fprintf(out,"SKETCH     %s\n",confSketchFile);
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

  out = fopen(fileLog,"a");
  if(out == NULL)
    {
      showError("Unable to open log file",-1);
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
  if(g_silent == 0)wprintw(slog,"%s",temp);
  show(slog);
}

//====================================
void mLog0(const char *p)
//====================================
{
  int i;
  char temp[100];

  strcpy(temp,p);
  if(confLogFile==YES)logFile(temp);

  wmove(slog,1,1);
  wprintw(slog,"%s",logBlankRow);
  wscrl(slog,-1);
  wmove(slog,1,1);
  wprintw(slog,">%s",simulation[currentStep+1]);
  wmove(slog,2,1);
  if(g_silent == 0)wprintw(slog,"%s",temp);
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
  if(g_silent == 0)wprintw(slog,"%s",temp);
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
  if(g_silent == 0)wprintw(slog,"%s",temp);
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
  if(g_silent == 0)wprintw(slog,"%s",temp);
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
  if(g_silent == 0)wprintw(slog,"%s",temp);

  wmove(uno,board_h-2,4);
  show(slog);
}


//====================================
void unoInfo()
//====================================
{
  //int next;

  wmove(uno,ap+2,3); 

  if(g_existError == YES)
    wprintw(uno,"  [Errors - err]");
  else if(g_warning == YES)
    wprintw(uno,"  [Possible Mismatch - load]");
  else
    wprintw(uno,"                           ");

  show(uno);
}

//====================================
void showLoops()
//====================================
{
  int i;
  wclear(msg);
  wmove(msg,1,2);
  wprintw(msg," Loop information:");
  for(i=0;i<g_loops;i++)
    {
      if(i < msg_h-1)
	{
	  wmove(msg,2+i,2);
          if(currentStep == loopPos[i])
	    wprintw(msg,">%3d: %4d -> %4d",i,loopPos[i]+1,loopPos[i+1]);
          else
            wprintw(msg," %3d: %4d -> %4d",i,loopPos[i]+1,loopPos[i+1]);
	}
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
  if(g_silent == 0)wprintw(slog,"%s",temp);
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

	  wmove(ser,1,1+slen);
	  wprintw(ser,"%s",m);

	  wscrl(ser,-1);

	  wmove(ser,1,1);
	  wprintw(ser,"%s",serBlankRow);

	  strcpy(prevSerial,"");
	}
      else
	{
	  slen = strlen(prevSerial);
	  if(slen < MAX_SERIAL_BUFFER)
	    {
	      strcat(prevSerial,m);
	      wmove(ser,1,1);
	      wprintw(ser,"%s",prevSerial);
	    }
	  else
	    showError("Serial Buffer full",currentStep);
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
  int pin=0;

  p = strstr(in,":");
  if(p==NULL) 
  {
    showError("Custom Log: Cant find any semicolon",0);
    strcpy(out,"Custom : Failed"); 
    return(0);
  }

  p++; 
  sscanf(p,"%d",&pin); 
  p = strstr(p,"\""); 
  if(p==NULL)
  {
    showError("Custom Log: Cant find first \" ",0);
    strcpy(out,"Custom \" Failed");
    return(0);
  }

  p++; 
  q = strstr(p,"\""); 
  if(q==NULL)
  {
    showError("Custom Log: Cant find second \" ",0);
    strcpy(out,"Custom \" Failed");
    return(0);
  }

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
  return;
}

//====================================
void readSketchInfo()
//====================================
{
  FILE *in;
  char row[80],res[40],*p,*q,value[5];
  int pin;

  in = fopen(fileServSketch,"r");
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
	  if(p=strstr(row,"BOARD_TYPE"))
	    {
	      if(strstr(row,"UNO") != NULL) confBoardType = UNO;
	      if(strstr(row,"MEGA")!= NULL) confBoardType = MEGA;
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
  for(i=0;i<MAX_PIN_DIGITAL_MEGA;i++)
    {
      strcpy(textPinModeIn[i],"void");
      strcpy(textPinModeOut[i],"void");

      strcpy(textDigitalWriteLow[i],"void");
      strcpy(textDigitalWriteHigh[i],"void");

      strcpy(textAnalogWrite[i],"void");
      strcpy(textDigitalRead[i],"void");
   
      currentValueD[i] = 0;
    }
  for(i=0;i<MAX_PIN_ANALOG_MEGA;i++)
    {
      strcpy(textAnalogRead[i],"void");
      currentValueA[i] = 0;
    }

}
//====================================
void resetSim()
//====================================
{
  int i;
  currentStep = 0;
  currentLoop = 0;
  for(i=0;i<max_digPin;i++)
    {
      digitalMode[i] = FREE;
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
/* 	      if(p=strstr(row,"LOG_LEVEL")) */
/* 		{ */
/* 		  sscanf(p,"%s%d",temp,&confLogLev); */
/* 		} */
	      if(p=strstr(row,"DELAY"))
		{
		  sscanf(p,"%s%d",temp,&confDelay);
		}
/* 	      if(p=strstr(row,"LOG_FILE")) */
/* 		{ */
/* 		  sscanf(p,"%s%d",temp,&confLogFile); */
/* 		} */
              if(p=strstr(row,"SKETCH"))
                {
                  sscanf(p,"%s%s",temp,confSketchFile);
                }
/*               if(p=strstr(row,"BOARD_TYPE")) */
/*                 { */
/*                   sscanf(p,"%s%d",temp,&confBoardType); */
/*                 } */
	    }
	 
	}
    }
  fclose(in);
}
//====================================
int readEvent(char *ev, int step)
//====================================
{
  if(step > 0 && step <= g_steps)
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
  
  if(step > 0 && step <= g_steps)
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
void runLoops(int targetLoop)
//====================================
{
  int stop;
  targetLoop = checkRange(HEAL,"loop",targetLoop);
  while(currentLoop < targetLoop && stop == 0)
    {
      stop = runStep(FORWARD);
    }
  return;
}    

//====================================
void runAll(int stop)
//====================================
{
  int x;
  stop = checkRange(HEAL,"step",stop);
  while(currentStep < stop)
    x = runStep(FORWARD);
  return;
}    

//====================================
void endOfSimulation()
//====================================
{
  wmove(slog,1,1);
  wprintw(slog,"%s",logBlankRow);
  wscrl(slog,-1);
  wmove(slog,1,1);
  wprintw(slog,">%s",logBlankRow);
  wmove(slog,2,1);
  wprintw(slog,"-=End of Simulation=-");
  show(slog);
  return;
}    

//====================================
void runNextRead()
//====================================
{
  char event[80];
  int x;

  while (!strstr(event,"digitalRead") && !strstr(event,"analogRead") && currentStep < g_steps)
    {
      runStep(FORWARD);
      x = readEvent(event, currentStep+1);
    }
  return;
}    


//====================================
int readSimulation(char *fileName)
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
	     // g_loops++;
	    }
          else if(p=strstr(row,"ENDOFSIM"))
            {
              loopPos[loop] = step;
              strcpy(simulation[step+1],"End of Simulation !");
            }
          else if(p=strstr(row,"SCENARIODATA"))
            {
              sscanf(p,"%s%d%d%d",temp,&scenDigital,&scenAnalog,&scenInterrupt);
            }
	}
      g_loops = loop;
      loopPos[loop] = step;
      fclose(in);
    }
  return(g_loops);
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
void selectProj(int projNo,char *projName)
//====================================
{
  FILE *in;
  char row[SIZE_ROW],temp[SIZE_ROW],*p;
  int i=0;

  strcpy(projName,fileDefault);

  in = fopen(fileProjList,"r");
  if(in == NULL)
    {
      showError("Unable to open list conf file",-1);
    }
  else
    {
      while (fgets(row,SIZE_ROW,in)!=NULL)
        {
	  i++;
	  if(i==projNo) 
	    {
              sscanf(row,"%s",projName);
	    }
        }
    }
  fclose(in);
  return;
}

//====================================
void readMsg(char *fileName)
//====================================
{
  FILE *in;
  char row[SIZE_ROW],temp[SIZE_ROW],*p;
  int i=0,ch;

  wclear(msg);
  in = fopen(fileName,"r");
  if(in == NULL)
    {
      showError("Unable to open msg file",-1);
    }
  else
    {
      while (fgets(row,SIZE_ROW,in)!=NULL && ch != 'x')
	{
          i++;
          // If Conf List File
          if(strstr(fileName,fileProjList) != NULL)
	    {
	      strcpy(temp,row);
	      if(p = strstr(temp,".conf")) strcpy(p,"\0");
	      if(strstr(row,currentConf))
		sprintf(row,"> %d %s",i,temp);
	      else
		sprintf(row,"  %d %s",i,temp);
	    }
          if(i < msg_h-2)
	    {
	      wmove(msg,i,1);
	      wprintw(msg,row);
	    }
          else
	    {
	      wmove(msg,msg_h-2,1);
	      wprintw(msg,"press any key (q to quit) >>");
	      wrefresh(msg);
	      ch = getchar();
	      if(ch == 'q')
		{
		  fclose(in);
		  return;
		}
	      wscrl(msg,msg_h-2);
	      i = 1;
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
void init(int mode)
//====================================
{
  int i,j,k;
  
  if(confBoardType == UNO)
    {
      board_w = UNO_W;
      board_h = UNO_H;
      board_x = 0;
      board_y = 0;
    }

  if(confBoardType == MEGA)
    {
      board_w = MEGA_W;
      board_h = MEGA_H;
      board_x = 0;
      board_y = 0;
    }

  dp = 5;
  ap = board_h - 5;

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

  getmaxyx(stdscr,s_row,s_col);
  start_color();
  init_pair(1,COLOR_BLACK,COLOR_BLUE);
  init_pair(2,COLOR_BLACK,COLOR_GREEN);
  init_pair(3,COLOR_BLUE,COLOR_WHITE); 
  init_pair(4,COLOR_RED,COLOR_WHITE); 
  init_pair(5,COLOR_MAGENTA,COLOR_WHITE); 
  init_pair(6,COLOR_WHITE,COLOR_BLACK); 
  init_pair(7,COLOR_WHITE,COLOR_BLUE);
  
  /*     COLOR_BLACK   0 */
  /*     COLOR_RED     1 */
  /*     COLOR_GREEN   2 */
  /*     COLOR_YELLOW  3 */
  /*     COLOR_BLUE    4 */
  /*     COLOR_MAGENTA 5 */
  /*     COLOR_CYAN    6 */
  /*     COLOR_WHITE   7 */

  // Board Window    
  uno=newwin(board_h,board_w,board_x,board_y);
  wbkgd(uno,COLOR_PAIR(UNO_COLOR));
  //box(uno, 0 , 0);

  wmove(uno,dp-1,RF);waddch(uno,ACS_ULCORNER); 
  wmove(uno,dp-1,RF+board_w-3);waddch(uno,ACS_URCORNER); 
  wmove(uno,ap+1,RF);waddch(uno,ACS_LLCORNER); 
  wmove(uno,ap+1,RF+board_w-3);waddch(uno,ACS_LRCORNER); 
  for(i=1;i<board_w-3;i++)
    {
      wmove(uno,dp-1,RF+i);
      waddch(uno,ACS_HLINE);
      wmove(uno,ap+1,RF+i);
      waddch(uno,ACS_HLINE);
    }
  for(i=dp;i<ap+1;i++)
    {
      wmove(uno,i,RF);
      waddch(uno,ACS_VLINE);
      wmove(uno,i,RF+board_w-3);
      waddch(uno,ACS_VLINE);
    }

  // Pin positions on the board
  for(i=0;i<MAX_PIN_DIGITAL_UNO;i++)
    {
      digPinCol[i]  = RF+3+4*(MAX_PIN_DIGITAL_UNO-i-1);
      digPinRow[i]  = dp;
      digIdCol[i]   = RF+2+4*(MAX_PIN_DIGITAL_UNO-i-1);
      digIdRow[i]   = dp+1;
      digActCol[i]  = RF+3+4*(MAX_PIN_DIGITAL_UNO-i-1);
      digActRow[i]  = dp+2;
      digStatCol[i] = RF+3+4*(MAX_PIN_DIGITAL_UNO-i-1);
      digStatRow[i] = dp-2;
    }
  for(i=MAX_PIN_DIGITAL_UNO;i<22;i++)
    {
      digPinCol[i]  = RF+10+4*i;
      digPinRow[i]  = dp;
      digIdCol[i]   = RF+9+4*i;
      digIdRow[i]   = dp+1;
      digActCol[i]  = RF+10+4*i;
      digActRow[i]  = dp+2;
      digStatCol[i] = RF+10+4*i;
      digStatRow[i] = dp-2;
    }
  if(confBoardType == MEGA)
    {
      j = dp+3;
      for(i=22;i<=max_digPin;i=i+2)
	{
	  j++;
	  digPinCol[i]  = board_w - 35;
	  digPinRow[i]  = j; 
	  digIdCol[i]   = board_w - 39;
	  digIdRow[i]   = j; 
	  digActCol[i]  = board_w - 41;
	  digActRow[i]  = j; 
	  digStatCol[i] = board_w - 32;
	  digStatRow[i] = j; 
	}
      j = dp+3;
      for(i=23;i<=max_digPin;i=i+2)
	{
	  j++;
	  digPinCol[i]  = board_w - 11;
	  digPinRow[i]  = j; 
	  digIdCol[i]   = board_w -  9;
	  digIdRow[i]   = j; 
	  digActCol[i]  = board_w -  6;
	  digActRow[i]  = j; 
	  digStatCol[i] = board_w - 14;
	  digStatRow[i] = j; 
	}
    }
  for(i=0;i<=max_anaPin;i++) anaPinCol[i] = RF+27+5*i;

  for(i=0;i<=max_digPin;i++){wmove(uno,digIdRow[i],digIdCol[i]); wprintw(uno,"%2d",i);}
  for(i=0;i<=max_digPin;i++){wmove(uno,digPinRow[i],digPinCol[i]); waddch(uno,ACS_BULLET);}
  //for(i=0;i<=max_digPin;i++){wmove(uno,digActRow[i],digActCol[i]); wprintw(uno,"a");}
  //for(i=0;i<=max_digPin;i++){wmove(uno,digStatRow[i],digStatCol[i]);wprintw(uno,"s");}

  for(i=0;i<=max_anaPin;i++){wmove(uno,ap-1,anaPinCol[i]-1); wprintw(uno,"A%1d",i);}
  for(i=0;i<=max_anaPin;i++){wmove(uno,ap,anaPinCol[i]); waddch(uno,ACS_BULLET);}

  show(uno);

  if(mode == 0) // side by side
    {
      msg_h = s_row - board_h;
      msg_w = board_w;
      msg_x = board_h;
      msg_y = 0;

      log_h = s_row;
      log_w = LOG_W;
      log_x = 0;
      log_y = board_w;   
      
      ser_h = s_row;
      ser_w = s_col - board_w - log_w;
      ser_x = 0;
      ser_y = board_w+log_w;
    }

  if(mode == 1) // 50 on 50
    {
      msg_h = s_row - board_h;
      msg_w = board_w;
      msg_x = board_h;
      msg_y = 0;

      log_h = s_row/2;
      log_w = s_col-uno_w;
      log_x = 0;
      log_y = board_w;   

      ser_h = s_row/2+1;
      ser_w = s_col-board_w;
      ser_x = s_row/2;
      ser_y = board_w;
    }

  if(mode == 2) // 90 on 10
    {
      msg_h = s_row - board_h;
      msg_w = board_w;
      msg_x = board_h;
      msg_y = 0;

      log_h = s_row-10;
      log_w = s_col-board_w;
      log_x = 0;
      log_y = board_w;   

      ser_h = 10;
      ser_w = s_col-board_w;
      ser_x = log_h;
      ser_y = board_w;
    }

  if(mode == 3) // 10 on 90
    {      
      msg_h = s_row - board_h;
      msg_w = board_w;
      msg_x = board_h;
      msg_y = 0;
      
      log_h = 10;
      log_w = s_col-board_w;
      log_x = 0;
      log_y = board_w;   

      ser_h = s_row-10;
      ser_w = s_col-board_w;
      ser_x = log_h;
      ser_y = board_w;
    }

  if(mode == 4) // big message to the right. Log on Ser
    {      
      msg_h = s_row;
      msg_w = s_col - board_w;
      msg_x = 0;
      msg_y = board_w;
      
      log_h = s_row-board_h-10;
      log_w = board_w;
      log_x = board_h;
      log_y = 0;   

      ser_h = s_row-board_h-log_h;
      ser_w = board_w;
      ser_x = log_h+board_h;
      ser_y = 0;
    }

  if(mode == 5) // big message to the right. Log and Ser side by side
    {      
      msg_h = s_row;
      msg_w = s_col - board_w;
      msg_x = 0;
      msg_y = board_w;
      
      log_h = s_row-board_h;
      log_w = board_w/2;
      log_x = board_h;
      log_y = 0;   

      ser_h = s_row-board_h;
      ser_w = board_w/2;
      ser_x = board_h;
      ser_y = log_w;
    }

  msg=newwin(msg_h,msg_w,msg_x,msg_y);
  scrollok(msg,true);
  wbkgd(msg,COLOR_PAIR(MSG_COLOR));
  show(msg);

  slog=newwin(log_h,log_w,log_x,log_y);
  scrollok(slog,true);
  wbkgd(slog,COLOR_PAIR(LOG_COLOR));
  show(slog); 

  ser=newwin(ser_h,ser_w,ser_x,ser_y);
  scrollok(ser,true);
  wbkgd(ser,COLOR_PAIR(SER_COLOR));
  show(ser);

  for(i=0;i<log_w;i++)logBlankRow[i] = ' ';logBlankRow[i]='\0';
  for(i=0;i<ser_w;i++)serBlankRow[i] = ' ';serBlankRow[i]='\0';
}

//====================================
int  countRowsInFile(char *fileName)
//====================================
{
  FILE *in;
  char row[SIZE_ROW];
  int res=0;
  
  in = fopen(fileName,"r");
  if(in == NULL)
    {
      showError("countRowsInFile: Unable to open file",-1);
    }
  else
    {
      while (fgets(row,SIZE_ROW,in)!=NULL)
        {
	  res++;
	}
      fclose(in);
      return(res);
    }
  return(999);
}
//====================================
void anyErrors()
//====================================
{
  int x;
  char syscom[200];
  
  g_existError = NO;
  x = system("rm temp.txt");
  sprintf(syscom,"cat %s %s %s> %s",fileError,fileServError,fileCopyError,fileTemp);
  x = system(syscom); 
  x = countRowsInFile(fileTemp);
  if(x > 0 && x != 999)g_existError = YES;
  if(x == 999)putMsg(2,"Unable to read error file");
  show(uno);
}


//====================================
int loadSketch(char sketch[])
//====================================
{
  int x,ch,res;
  char syscom[120];

  sprintf(syscom,"cp %s %s > %s 2>&1;",sketch,fileServSketch,fileCopyError);
  x=system(syscom);
  strcpy(confSketchFile,sketch);
  sprintf(syscom,"cd servuino; g++ -O2 -o servuino servuino.c > g++.result 2>&1;");
  x=system(syscom);

  x=countRowsInFile(fileServComp);
  if(x > 0)
    {
      readMsg(fileServComp);
      wmove(msg,msg_h-2,1);
      wprintw(msg,"press any key to continue >>");
      wrefresh(msg);
      ch = getchar();
      putMsg(2,"Check your sketch or report an Issue to Simuino");
      return(1);
    }
  readSketchInfo();
  return(0);
}

//====================================
// End of file
//====================================
