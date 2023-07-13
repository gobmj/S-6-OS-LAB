#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100

int is_prime(int number) {
    for (int i = 2; i <= number / 2; ++i) {
        if (number % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main() {
    int pipe_fd[2];
    pid_t child_pid, n;
    int numbers[20];
    printf("\nEnter the Number of Numbers: ");
    scanf("%d", &n);
    printf("\nEnter the Numbers: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &numbers[i]);
    }

    if (pipe(pipe_fd) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    child_pid = fork();

    if (child_pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Close the write end
        close(pipe_fd[1]);

        // Read numbers
        int number;
        while (read(pipe_fd[0], &number, sizeof(int)) > 0) {
            if (is_prime(number)) {
                // Write the prime number back to the parent process
                write(pipe_fd[1], &number, sizeof(int));
            }
        }

        // Close the read and write ends
        close(pipe_fd[0]);
        close(pipe_fd[1]);

        exit(EXIT_SUCCESS);
    } else {
        // Close the read end
        close(pipe_fd[0]);

        // Send numbers to the child process through the pipe
        for (int i = 0; i < n; i++) {
            write(pipe_fd[1], &numbers[i], sizeof(int));
        }

        // Close the write end
        close(pipe_fd[1]);

        // Wait for the child process to finish
        wait(NULL);

        // Read and display prime numbers returned by the child process
        int prime_number;
        while (read(pipe_fd[0], &prime_number, sizeof(int)) > 0) {
            printf("Parent process: Prime number = %d\n", prime_number);
        }

        // Close the read end
        close(pipe_fd[0]);
    }

    return 0;
}
