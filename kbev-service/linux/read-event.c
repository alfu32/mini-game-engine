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
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file1> <input_file2> ... <output_file>\n", argv[0]);
        return 1;
    }
    int input_fd = open(argv[1], O_RDONLY);
    if (input_fd == -1) {
        perror("open");
        return 1;
    }
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
        printf("time.tv_sec\ttime.tv_usec\ttype\tcode\tvalue\n");
    while ((bytes_read = read(input_fd, buffer, BUFFER_SIZE)) > 0) {
        struct input_event * ev = (struct input_event*)(&buffer);
        printf("%11ld\t%12ld\t%4u\t%4u\t%10x\n",ev->time.tv_sec,ev->time.tv_usec,ev->type,ev->code,ev->value);
    }

    close(input_fd);

    return 0;
}
