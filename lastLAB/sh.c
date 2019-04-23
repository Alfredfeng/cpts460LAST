/********** sh.c file *************/
#include "ucode.c"

char cmdline[100];
char commandLine[100];
char *_argv[10];//first argv

int main(int argc, char *argv[ ])
{
  int i;

  int pid = getpid();
  printf("Yuzhu: PROC %d running shell program\n", pid);

  // printf("argc = %d\n", argc);
  // for (i=0; i<argc; i++)
  //   printf("argv[%d] = %s\n", i, argv[i]);

  while(1)
  {
  	printf("Enter command>");
  	gets(cmdline);
  	cmdline[strlen(cmdline)] = 0;//kill the '\n'
  	strcpy(commandLine,cmdline);//copy the command line
  	parseCmdHelper(cmdline);//parse the command line

  	if(strcmp(_argv[0],"cd") != 0 && strcmp(_argv[0],"logout") != 0){
  		pid = fork();
  		if(pid)
  		{
  			//parent
  			wait(&pid);//parent waits
  		}
  		else
  		{
  			//child
  			exec(commandLine);//child execute the command
  			exec("exit");//child dies
  		}
  	}
  	else if(strcmp(_argv[0],"logout") == 0)
  	{
  		prints("shell runs logout\n");
  		break;//get out of the while loop
  	}
  	else
	{
		prints("running cd\n");
		chdir(_argv[1]);
	}
  }

  printf("PROC %d exit\n", pid);
}

int parseCmdHelper(char *line)
{
  prints("parsing command...\n");
  char *cp = line; 
  argc = 0;
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
}
