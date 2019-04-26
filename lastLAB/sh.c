/********** sh.c file *************/
#include "ucode.c"

//extern int in, out, err;

//char cmdline[100];
//char commandLine[100]; //complete command line
char *_argv[10];//first argv

int do_IOredirection(int i, int type);
int do_pipe(int i);
int hasPipe(char *cmdline);
int hasIO(char *cmdline);
int isCD(char *cmdline);
void clearArgv();


int main(int argc, char *argv[ ])
{
  int i = 0;
  int ioNeeded = 0; // flag for whether io redirection is needed
  int pid = getpid();
  printf("Yuzhu: PROC %d running shell program\n", pid);
  char cmdline[100];

  // printf("argc = %d\n", argc);
  // for (i=0; i<argc; i++)
  //   printf("argv[%d] = %s\n", i, argv[i]);

  while(1)
  {
  	printf("Enter command>");
  	gets(cmdline);
  	cmdline[strlen(cmdline)] = 0;//kill the new line character
  	interpreter(cmdline);//interpret the command
  }

  printf("PROC %d exit\n", pid);
  return 0;
}

//interpreter function that handles commands
//check base case
//check case with IO redirection symbol
//check case with pipe symbol
int interpreter(char *cmdline)
{
	char subcmd[80] = "";//left subcmd
	char rsubcmd[20] = "";//right subcmd, it is a simple command
	char cmd[100];
	int containsIO = 0;//default: no IO
	int containsPipe = 0;//default: no pipe
	int fd;//file descriptor for io redirection
	int status;
	int pd[2];//pipe
	prints("\n");
	prints(cmdline);
	prints("\n");

	//check for illegal input
	if(cmdline == 0)
	{
		printf("ERROR: cmd is null\n");
		return -1;
	}


	strcpy(cmd,cmdline);//get a copy of commandLine

	//again, check illegal input
	if(strcmp(cmd,"")==0)
	{
		return 0;
	}

	containsIO = hasIO(cmd);
	containsPipe = hasPipe(cmd);
	printf("containsIO=%d\n",containsIO);
	printf("containsPipe=%d\n",containsPipe);
	//check base case: whether cmdline has no pipe and no IO redirection symbol
	if(!containsIO && !containsPipe)
	{
		//handling cd...
		if(isCD(cmd))
		{
			chdir(_argv[1]);
		}
		else
		{
			
			//fork a child process to do the program
			
			pid = fork();
			if(pid)
			{
				//wait(&status);//parent waits
				wait(&pid);
			}
			else
			{
				printf("primitive cmd=%s\n",cmd);
				exec(cmd);//child execute the command
				//exec("exit");
			}
			
			//exec(cmd);
		}
	}
	//handling pipe and IO redirection
	int argc = parseCmdHelper(cmdline);
	for(int i = argc ; i >= 0 ; i--)
	{
		if(_argv[i] != 0)
		{
			//check I/O redirection symbol
			if(strcmp(_argv[i],">") == 0 || strcmp(_argv[i],">>") == 0 || strcmp(_argv[i],"<") == 0)
			{
				printf("Handling I/O redirection\n");
				for( int j = 0 ; j < i ; j ++)
				{
					
					strcat(subcmd,_argv[j]);
					strcat(subcmd," ");//add a space character
				}
				subcmd[strlen(subcmd)-1] = 0;//kill the last space
				printf("In io redirection: left cmd=%s\n",subcmd);
				//handling IO redirection:
				char file[25]; strcpy(file,_argv[i+1]);
				printf("file=%s\n",file);
				//handling types of IO redirection:
				if(strcmp(_argv[i],">")==0)
				{
					prints("io for write\n");
					fd = open(file,O_WRONLY | O_CREAT);
					close(1);//close stdout
					dup(fd);
					close(fd);
					open("/dev/tty0", O_WRONLY); // //reopen the stdout file descriptor
					interpreter(subcmd);
					//exec(subcmd);
				}
				else if(strcmp(_argv[i],">>") ==0)
				{
					printf("io for append\n");
					fd = open(file,O_APPEND);
					close(1);
					dup(fd);
					close(fd);
					interpreter(subcmd);
				}
				else if(strcmp(_argv[i],"<")==0)
				{
					printf("io for read\n");
					fd = open(file,O_RDONLY);
					close(0);
					dup(fd);
					close(fd);
					open("/dev/tty0", O_RDONLY);
					interpreter(subcmd);
					//to be continued
				}
				//interpreter(subcmd);
			}


			//check pipe symbol '|'
			if(strcmp(_argv[i],"|") == 0)
			{
				printf("Handling pipe\n");
				for( int j = 0 ; j < i ; j ++)
				{	
					strcat(subcmd,_argv[j]);
					strcat(subcmd," ");
				}
				subcmd[strlen(subcmd)-1] = 0;//kill the last space character
				printf("in pipe left cmd=%s\n",subcmd);
				for(int j = i+1 ;j < argc ; j ++)
				{
					strcat(rsubcmd, _argv[j]);
					strcat(rsubcmd, " ");//append a space character
				}
				rsubcmd[strlen(rsubcmd) - 1] = 0;//kill the last space character
				//exec(subcmd);
				pipe(pd); //create a pipe: pd[0]	
				pid = fork(); //fork a child to share the pipe
				if(pid)
				{
					//parent : as the pipe reader
					close(pd[1]); //close pipe WRITE end
					dup2(pd[0],0); //redirect stdin to pipe READ end;
					exec(rsubcmd);
				}
				else
				{
					close(pd[0]); //close pipe READ end
					dup2(pd[1],1); // redirect stdout to pipe WRITE end
					interpreter(subcmd);
					//exec(subcmd);
				}
			}	
		}//end if
		
	}//end for
	//printf("cmd=%s\n",cmdline);
	//there are no pipe symbols or I/O redirection symbols
	return 0;
}

int parseCmdHelper(char *line)
{
  //prints("parsing command...\n");
  char *cp = line; 
  int argc = 0;
  while (*cp != 0){
    while (*cp == ' ')  *cp++ = 0;
    if (*cp != 0)// skip over blanks // token start
      _argv[argc++] = cp; // pointed by argv[ ]
    while (*cp != ' ' && *cp != 0) // scan token chars
      cp++;
    if (*cp != 0)
      *cp = 0;
    else // end of token
      break; // end of line
    cp++;// continue scan
  } //end outer while
  _argv[argc] = 0; // argv[argc]=0
  return argc;
}

void clearArgv()
{
	for(int i = 0 ; i < 10 ; i ++)
	{
		_argv[i] = 0;//clear argv
	}
}


//naive implementation for I/O redirection
//type 0 means "<"; type 1 means ">"; type 2 means ">>"

//this function checks if the cmdline contains pipe symbol
int hasPipe(char *cmdline)
{
	printf("Inside containsPipe()\n");
	int result = 0;//default means not containing pipe
	char cmd[25];
	if(cmdline == 0)
	{
		printf("ERROR: cmdline is null\n");
		return -1;
	}
	strcpy(cmd,cmdline);

	clearArgv();//clear argv
	int argc = parseCmdHelper(cmd);

	for(int i = 0 ; i < argc ; i++)
	{
		//printf("argv[%d]=%s\n",i,_argv[i]);
		if(strcmp(_argv[i],"|") ==0)
		{
			//printf("Contains pipe: result = 1\n");
			result = 1;
			return result;
		}
	}
	return result;
}

int hasIO(char *cmdline)
{
	printf("Inside containsIO()\n");
	int result = 0;//default value: no IO
	char cmd[25];
	if(cmdline == 0)
	{
		printf("ERROR: cmdline is null\n");
		return -1;
	}
	strcpy(cmd,cmdline);

	clearArgv();
	int argc = parseCmdHelper(cmd);
	for(int i = 0 ; i < argc ; i ++)
	{
		//printf("argv[%d]=%s\n",i,_argv[i]);
		if(strcmp(_argv[i],">")==0 || strcmp(_argv[i],"<") == 0 || strcmp(_argv[i],">>") ==0)
		{
			//printf("Contains IO redirection symbol: result = 1\n");
			result = 1;
			return result;
		}
	}
	return result;
}

int isCD(char *cmdline)
{
	int result = 0;
	char cmd[25];
	if(cmdline == 0)
	{
		printf("ERROR: cmdline is null\n");
		return -1;
	}
	strcpy(cmd,cmdline);
	parseCmdHelper(cmd);
	if(strcmp(_argv[0],"cd") ==0 )
		result = 1;
	return result;
}
