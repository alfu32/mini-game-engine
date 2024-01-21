#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // Include this for read and write functions
#include <fcntl.h>
#include <ev.h>

#define BUFFER_SIZE 1024


void read_file(ev_io *watcher, int revents,struct ev_loop *loop) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(watcher->fd, buffer, BUFFER_SIZE)) > 0) {
        ssize_t bytes_written = write(STDOUT_FILENO, buffer, bytes_read);
        if (bytes_written == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    if (bytes_read == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    close(watcher->fd);
    ev_io_stop(EV_A_ watcher);
    free(watcher);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input_file1> <input_file2> ... <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Initialize libev
    struct ev_loop* loop = EV_DEFAULT;

    // Iterate through input files and create watchers for them
    for (int i = 1; i < argc - 1; i++) {
        int input_fd = open(argv[i], O_RDONLY | O_NONBLOCK);
        if (input_fd == -1) {
            perror("open");
            return EXIT_FAILURE;
        }

        ev_io *watcher = (ev_io *)malloc(sizeof(ev_io));
        ev_io_init(watcher, read_file, input_fd, EV_READ);
        ev_io_start(loop, watcher);
    }

    // Open the output file for writing
    int output_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    // Redirect stdout to the output file
    dup2(output_fd, STDOUT_FILENO);
    close(output_fd);

    // Start the event loop
    ev_run(loop, 0);

    return EXIT_SUCCESS;
}
