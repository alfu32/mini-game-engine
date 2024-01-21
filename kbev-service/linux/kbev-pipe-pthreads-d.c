#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <pthread.h> // Include this header for threading

#define BUFFER_SIZE 1024

// Function to read a file and write to the pipe
void* read_file(void* arg) {
    char* filename = (char*)arg;
    int input_fd = open(filename, O_RDONLY);

    if (input_fd == -1) {
        perror("open");
        pthread_exit(NULL);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    ssize_t bytes_written;

    while ((bytes_read = read(input_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(STDOUT_FILENO, buffer, bytes_read);
        if (bytes_written == -1) {
            perror("write");
            close(input_fd);
            pthread_exit(NULL);
        }
    }

    close(input_fd);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input_file1> <input_file2> ... <output_file>\n", argv[0]);
        return 1;
    }
    int has_error=0;

    // Create a thread for each input file
    pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t)*(argc-2));

    for (int i = 1; i < argc - 1; i++) {
        if (pthread_create(&threads[i - 1], NULL, read_file, argv[i]) != 0) {
            perror("pthread_create");
            has_error = 1;
        }
    }
    // Wait for all threads to finish
    for (int i = 0; i < argc - 2; i++) {
        pthread_join(threads[i], NULL);
    }

    // Open the output file for writing
    int output_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1) {
        perror("open");
        free(threads);
        return 1;
    }

    // Redirect stdout to the output file
    dup2(output_fd, STDOUT_FILENO);
    close(output_fd);

    // Execute the cat command to copy data from stdin to stdout
    execlp("cat", "cat", NULL);
    perror("execlp");
    free(threads);
    return 1;
}
