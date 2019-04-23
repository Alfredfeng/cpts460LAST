/********************* cat.c file ****************/
#include "ucode.c"

int main(int argc, char *argv[ ])
{
    prints("$$$$$$$$$$$$$$$$Yuzhu's cat cannot MEOW$$$$$$$$$$$$$$$\n");
	char mybuf[1024], dummy = 0; // a null char at the end of mybuf[]
	int n;
	//simple cat without handling cat from stdin
	if(argc == 1)
	{
		while(1)
		{
			gets(mybuf);
			printf("%s",mybuf);
		}
	}
	int fd = open(argv[1],O_RDONLY);//open for read
	while( n = read(fd, mybuf, 10))
	{
		 mybuf[n] = 0; // as a null-terminated string
		 printf("%s",mybuf);//not good
		//spit out chars from mybuf[] but handle '\n' properly
	}
	close(fd);//close the file descriptor
}