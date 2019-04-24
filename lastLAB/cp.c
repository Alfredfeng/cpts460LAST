/********** test.c file *************/
#include "ucode.c"
int main(int argc, char *argv[ ])
{
	prints("$$$$$$$$$$$$$$$$$$$$$$Yuzhu's copy$$$$$$$$$$$$$$$$$$$$$$\n");
	char cp_buf[10];
	int n;
	int fd = open(argv[1],O_RDONLY);//fd for read
	int gd = open(argv[2], O_WRONLY | O_CREAT);//fd for write

	while( n = read(fd,cp_buf,10))
	{
		write(gd,cp_buf,n);
	}
	close(fd);
	close(gd);
	return 0;
}