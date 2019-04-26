/********** l2u.c file *************/
#include "ucode.c"

int main(int argc, char *argv[ ])
{
  //implementation for l2u
	char l2u_buf[10];//l2u buffer
	if(argc == 1)
	{
		while(gets(l2u_buf))
		{
			for(int i = 0 ; i < 10 ; i ++)
			{
				l2u_buf[i] = toupper(l2u_buf[i]);//convert to upper case
			}
		}
	}
	else
	{
			prints("$$$$$$$$$$$$$$$$$$$$$$Yuzhu's l2u$$$$$$$$$$$$$$$$$$$$$$\n");
		int n;
		int fd = open(argv[1],O_RDONLY);//fd for read
		int gd = open(argv[2], O_WRONLY | O_CREAT);//fd for write

		while( n = read(fd,l2u_buf,10))
		{
			for(int i = 0 ; i < strlen(l2u_buf) ; i++)
			{
				l2u_buf[i] = toupper(l2u_buf[i]);//convert to upper case
			}
			write(gd,l2u_buf,n);
		}
		close(fd);
		close(gd);
	}
	return 0;
}

char toupper(char cc)
{
	if(cc >= 'a' && cc <= 'z')
		cc = cc -32;
	return cc;
}