#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define BUFFER_SIZE 100

int main() {
    int pipe_fd[2];
    pid_t child_pid;
    int numbers[] = {1, 2, 3, 4, 5};  // Numbers to be sent by the parent process
    int num_count = sizeof(numbers) / sizeof(numbers[0]);  // Number of elements in the array
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
        for (int i = 0; i < num_count; i++) {
            write(pipe_fd[1], &numbers[i], sizeof(int));
        }

        // Close the write end of the pipe
        close(pipe_fd[1]);

        // Wait for the child process to finish
        wait(NULL);

        printf("Parent process: Sent %d numbers to child process.\n", num_count);
    }

    return 0;
}
