#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100

int main() {
    int pipe_fd[2];
    pid_t child_pid;
    int numbers[20],n;
    printf("\nEnter the Number of Numbers (max 20): ");
    scanf("%d",&n);
  	printf("\nEnter the Numbers: ");
    for (int i=0;i<n;i++)
    {
    	scanf("%d",&numbers[i]);
    }
    int sum = 0;

    // Create the pipe
    if (pipe(pipe_fd) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    // Fork the process
    child_pid = fork();

    if (child_pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Child process

        // Close the write end of the pipe
        close(pipe_fd[1]);

        // Read numbers from the parent process and calculate the sum
        int number;
        while (read(pipe_fd[0], &number, sizeof(int)) > 0) {
            sum += number;
        }

        // Close the read end of the pipe
        close(pipe_fd[0]);

        // Display the sum
        printf("Child process: Sum = %d\n", sum);
    } else {
        // Parent process

        // Close the read end of the pipe
        close(pipe_fd[0]);

        // Send numbers to the child process through the pipe
        for (int i = 0; i < n; i++) {
            write(pipe_fd[1], &numbers[i], sizeof(int));
        }

        // Close the write end of the pipe
        close(pipe_fd[1]);

        // Wait for the child process to finish
        wait(NULL);

        printf("Parent process: Sent %d numbers to child process.\n", n);
    }

    return 0;
}
