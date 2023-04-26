#include<stdio.h>
#include<stdlib.h>

int main()
{
	FILE *src,*dest;
	char src_name[100],dest_name[100];
	char ch;
	printf("Enter Name of the Source File: ");
	scanf("%s",src_name);
	src=fopen(src_name,"r");
	if(src==NULL)
	{
		printf("Unable to Open the Source File...");
		exit(0);
	}
	printf("Enter the Name of Destination File: ");
	scanf("%s",dest_name);
	dest=fopen(dest_name,"r");
	if(dest!=NULL)
	{
		fclose(dest);
		printf("File %s already exists, Enter a new Name: ",dest_name);
		scanf("%s",dest_name);
	}
	dest=fopen(dest_name,"w");
	if(dest==NULL)
	{
		printf("Unable to Create Destination File...");
		exit(0);
	}
	while((ch=fgetc(src))!=EOF)
	{
		fputc(ch,dest);
	}
	fclose(src);
	fclose(dest);
	printf("File copied Successfully...\n");
	return 0;
}
