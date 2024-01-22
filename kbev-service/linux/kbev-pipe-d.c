#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include <linux/input.h>

#define INPUT_EVENT_SIZE sizeof(struct input_event)

// Function to set a file descriptor to non-blocking mode
void set_non_blocking(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flags);
}

// Signal handler to gracefully exit the program
void signal_handler(int signo)
{
    if (signo == SIGINT || signo == SIGTERM) {
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input files...> <output file>\n", argv[0]);
        exit(1);
    }

    // Install signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    while (1)
    {

        // Loop through input files
        for (int i = 1; i < argc - 1; i++) {
            int input_fd = open(argv[i], O_RDONLY);
            if (input_fd == -1) {
                perror("Unable to open input file");
                continue; // Move to the next input file
            }

            set_non_blocking(input_fd);

            // Open output file for writing
            int output_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (output_fd == -1) {
                perror("Unable to open output file");
                exit(1);
            }
            char buffer[INPUT_EVENT_SIZE];
            ssize_t bytes_read = read(input_fd, buffer, INPUT_EVENT_SIZE);

            if (bytes_read == INPUT_EVENT_SIZE) {

                struct input_event *ev = (struct input_event *)(&buffer);
                printf("from file %s read key %d output %s\n", argv[i], ev->code, argv[argc - 1]);
                write(output_fd, buffer, INPUT_EVENT_SIZE);
                // Close the output file
                close(output_fd);
                // End of file reached, close the input file and move to the next
                close(input_fd);
            } else if (bytes_read == -1) {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    // Close the output file
                    close(output_fd);
                    // End of file reached, close the input file and move to the next
                    close(input_fd);
                    // No data available right now, continue
                    usleep(1000); // Sleep for a short time to avoid busy-waiting
                    continue;
                } else {
                    // End of file reached, close the input file and move to the next
                    close(input_fd);
                    // Close the output file
                    close(output_fd);
                    // Handle other errors
                    perror("Read error");
                    break;
                }
            } else if (bytes_read == 0) {
                // Close the output file
                close(output_fd);
                // End of file reached, close the input file and move to the next
                close(input_fd);
                break;
            }

            // Sleep for a short time to avoid busy-waiting
            usleep(1000);
        }

    }

    return 0;
}
