#include<stdio.h>
#include<unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <errno.h>

int checkprime(int num)
{
    if(num<=1)
    {
        return -1;
    }
    for (int i = 2; i <= num / 2; ++i)
    {
        if (num % i == 0)
        {
            return -1;
        }
    }
    return 1;
}

int main() 
{
    int returnstatus;
    int pid;
    int size_pipe[2];
    int arr_pipe[2];
    int size_pipe_two[2];
    int arr_pipe_two[2];
    if (pipe(size_pipe) == -1)
    {
        printf("Unable to create pipe 1\n");
        return -1;
    }
    if (pipe(arr_pipe) == -1)
    {
        printf("Unable to create pipe 2\n");
        return -1;
    }
    if (pipe(size_pipe_two) == -1)
    {
        printf("Unable to create pipe 1\n");
        return -1;
    }
    if (pipe(arr_pipe_two) == -1)
    {
        printf("Unable to create pipe 2\n");
        return -1;
    }
    pid = fork();
    if(pid==-1)  
    {
        printf("Unable to create the process\n");
        return -1;
    }
    if (pid == 0) 
    {    
        close(size_pipe[1]);
        close(arr_pipe[1]);
        close(size_pipe_two[0]);
        close(arr_pipe_two[0]);
        int size;
        read(size_pipe[0], &size, sizeof(size));
        int sub_num_arr[size];
        read(arr_pipe[0], &sub_num_arr, sizeof(sub_num_arr));
        int prime_arr[size];
        int k=0;
        for(int i=0;i<size;i++)
        {
            if(checkprime(sub_num_arr[i])==1)
            {
                prime_arr[k]=sub_num_arr[i];
                k++;
            }
        }
        returnstatus=write(size_pipe_two[1], &k, sizeof(k));
        if(returnstatus==-1)
        {
            printf("Error occurred at write of size two");
            return -1;
        }
        returnstatus=write(arr_pipe_two[1],prime_arr, sizeof(prime_arr));
        if(returnstatus==-1)
        {
            printf("Error occurred at write of arr two");
            return -1;
        }
   } 
   else 
   { 
        close(size_pipe[0]);
        close(arr_pipe[0]);
        close(size_pipe_two[1]);
        close(arr_pipe_two[1]);
        int size;
        printf("\nEnter the Number of Numbers: ");
        scanf("%d", &size);
        int num_arr[size];
        returnstatus=write(size_pipe[1], &size, sizeof(size));
        if(returnstatus==-1)
        {
            printf("Error occurred at write of size");
            return -1;
        }
        printf("Enter the numbers: ");
        for(int i=0;i<size;i++)
        {
            scanf("%d",&num_arr[i]);
        }
        returnstatus=write(arr_pipe[1],num_arr, sizeof(num_arr));
        if(returnstatus==-1)
        {
            printf("Error occurred at write of arr");
            return -1;
        }
        read(size_pipe_two[0], &size, sizeof(size));
        int prime_arr[size];
        read(arr_pipe_two[0], &prime_arr, sizeof(prime_arr));
        printf("\nThe Prime Numbers are: ");
        for(int i=0;i<size;i++)
        {
            printf("%d ",prime_arr[i]);
        }
        wait(NULL);
   }
   return 0;
}