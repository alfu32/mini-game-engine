#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h> // Include this header for wait function
#include <linux/input.h>

#define BUFFER_SIZE sizeof(struct input_event)

const char* format_json="{ord:%05d,time:{tv_sec:%ld,tv_usec:%ld},type:%s,code:%u,value:%x}\n";
const char* format_csv="%05d,%11ld,%12ld,%12s,%4u,%10x\n";
const char* format_tsv="%05d\t%11ld\t%12ld\t%12s\t%4u\t%10x\n";

const char* decode_type(struct input_event* ev){
    switch(ev->type){
        case EV_KEY:
            if ( ev->value == 1) {
                return "key_down";
            } else {
                return "key_up";
            }
        default: return "other";
    }
}

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
        // printf("time.tv_sec\ttime.tv_usec\ttype\tcode\tvalue\n");
    int count=1;
    while ((bytes_read = read(input_fd, buffer, BUFFER_SIZE)) > 0) {
        struct input_event * ev = (struct input_event*)(&buffer);
        printf(format_json,count,ev->time.tv_sec,ev->time.tv_usec,decode_type(ev),ev->code,ev->value);
        count++;
    }

    close(input_fd);

    return 0;
}
