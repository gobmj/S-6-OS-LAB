#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define BUFFER_SIZE 100

int is_prime(int number) {
    if (number <= 1) {
        return 0;
    }

    for (int i = 2; i * i <= number; i++) {
        if (number % i == 0) {
            return 0;
        }
    }

    return 1;
}

int main() {
    int pipe_fd[2];
    pid_t child_pid;
    int numbers[] = {2, 3, 4, 5, 6, 7, 8, 9, 10};  // Numbers to be sent by the parent process
    int num_count = sizeof(numbers) / sizeof(numbers[0]);  // Number of elements in the array

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

        // Read numbers from the parent process
        int number;
        while (read(pipe_fd[0], &number, sizeof(int)) > 0) {
            if (is_prime(number)) {
                // Write the prime number back to the parent process
                write(pipe_fd[1], &number, sizeof(int));
            }
        }

        // Close the read and write ends of the pipe
        close(pipe_fd[0]);
        close(pipe_fd[1]);

        exit(EXIT_SUCCESS);
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

        // Read and display prime numbers returned by the child process
        int prime_number;
        while (read(pipe_fd[0], &prime_number, sizeof(int)) > 0) {
            printf("Parent process: Prime number = %d\n", prime_number);
        }

        // Close the read end of the pipe
        close(pipe_fd[0]);
    }

    return 0;
}
