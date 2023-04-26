#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void main()
{
	int nl = 0;
	printf("Enter the Number of Lines to Copy in Reverse: ");
	scanf("%d",&nl);

	if(nl<0)
	{
		printf("Invalid Input...\n");
		exit(0);
	}

	char source_file[20];
	char des_file[20];
	char data_read[2];
	printf("Enter Name of the Source File: ");
	scanf("%s",source_file);
	
	printf("Enter the Name of Destination File: ");
	scanf("%s",des_file);

	int again = 0;
	int fd2 = 0;

	do
	{
		
		int test = open(des_file,O_EXCL);
		if(test!=-1)
		{
			printf("File already exists, Enter a new Name: ");
			scanf("%s",des_file);
		}
		else
		{
			fd2 = open(des_file,O_WRONLY|O_CREAT|O_TRUNC,0644);
			again = 1;
		}
	}
	while(again!=1);
	
	int fd = open(source_file,O_RDONLY);
	
	if(fd>2)
	{
		lseek(fd2,0,SEEK_SET);
		off_t filelength = lseek(fd,0,SEEK_END);

		int count = 0;
		int nlc =0;
		
		while(count+filelength > 0)
		{
			lseek(fd,count-2,SEEK_END);
			read(fd,data_read,1);
			if(data_read[0]=='\n')
			{
				nlc++;
			}
			if (nlc == nl)
			{
				break;
			}
			else
			{
				
				write(fd2,data_read,1);
			}
			count -=1;
		}
		write(fd2,data_read,1);
		printf("Copied the lines to the File in Reverse Order Successfully...\n");
	}
	else
	{
		printf("File Not Found\n");
	}
}
