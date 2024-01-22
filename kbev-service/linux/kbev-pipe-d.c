#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h> // Include this header for wait function
#include <linux/input.h>

#define BUFFER_SIZE sizeof(struct input_event)

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input_file1> <input_file2> ... <output_file>\n", argv[0]);
        return 1;
    }

    // Create a pipe
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    // Fork a child process
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) { // Child process
        // Close the write end of the pipe
        close(pipefd[1]);

        // Set the read end of the pipe as stdin
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        // Open the output file for writing
        int output_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (output_fd == -1) {
            perror("open");
            return 1;
        }

        // Redirect stdout to the output file
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);

        // Execute the cat command to copy data from stdin to stdout
        execlp("cat", "cat", NULL);
        perror("execlp");
        return 1;
    } else { // Parent process
        // Close the read end of the pipe
        close(pipefd[0]);

        // Iterate through input files and write their content into the pipe
        for (int i = 1; i < argc - 1; i++) {
            int input_fd = open(argv[i], O_RDONLY);
            if (input_fd == -1) {
                perror("open");
                return 1;
            }

            char buffer[BUFFER_SIZE];
            ssize_t bytes_read = read(input_fd, buffer, BUFFER_SIZE);

            if ( bytes_read == BUFFER_SIZE) {

                struct input_event *ev = (struct input_event *)(&buffer);
                printf("from file %s read key %d output %s\n", argv[i], ev->code, argv[argc - 1]);

                ssize_t bytes_written = write(pipefd[1], buffer, bytes_read);
                if (bytes_written == -1) {
                    perror("output write error");
                    return 1;
                }
            } else if (bytes_read == -1) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    // No data available right now, continue
                    usleep(1000); // Sleep for a short time to avoid busy-waiting
                } else {
                    // Handle other errors
                    printf("%s");
                    perror("Read error");
                }
            } else if (bytes_read == 0) {
                // End of file reached, close the input and output files and move to the next
            }

            close(input_fd);
        }

        // Close the write end of the pipe
        close(pipefd[1]);

        // Wait for the child process to finish
        wait(NULL);
    }

    return 0;
}
