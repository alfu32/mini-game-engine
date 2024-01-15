#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>

bool service_running = true;

void interrupt_handler(int sig)
{
    printf("Received Ctrl+C. Cleaning up and exiting...\n");
    // Add your cleanup code here if needed
    service_running = false;
}

int get_keyboard_input_devices_list(char input_devices_list[256][256], int *count)
{
    FILE *fp;
    char path[1035];

    /* Open the command for reading. */
    fp = popen("for device in /dev/input/event*; do if udevadm info -a -n $device | grep -q \"eyboard\"; then echo $device; fi; done", "r");
    if (fp == NULL)
    {
        printf("Failed to run enumerate keyboard devices\n");
        service_running = false;
        return 1;
    }

    /* Read the output a line at a time - output it. */
    while (fgets(path, sizeof(path), fp) != NULL)
    {
        strcpy(input_devices_list[*count], path);
        (*count)++;
    }

    /* Close the file pointer */
    pclose(fp);
    return 0;
}

void main()
{
    int errnum=0;
    signal(SIGINT, interrupt_handler);

    char current[256];
    getcwd(current, sizeof(current));
    system("mkdir -p /tmp/kbev");
    chdir("/tmp/kbev");
    printf("Configured events service in: /tmp/kbev\n");

    char input_devices_list[256][256];
    int count = 0;
    get_keyboard_input_devices_list(input_devices_list, &count);

    FILE *file = fopen("input_devices.lst", "w");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        service_running = false;
        errnum=1;
    }

    for (int i = 0; i < count; i++)
    {
        fputs(input_devices_list[i], file);
    }
    fclose(file);

    printf("Detected input devices:\n");
    for (int i = 0; i < count; i++)
    {
        printf("%s", input_devices_list[i]);
    }
    printf("Configuring service:\n");

    system("chmod 777 input_devices.lst");
    system("touch events");
    system("chmod 777 events");
    system("rm -rf read.lock");
    system("ls -la");

    // Disable echoing
    
    struct termios oldt;
    struct termios newt;
    tcgetattr(STDIN_FILENO, &(oldt));
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &(newt));

    while (service_running)
    {
        if (access("read.lock", F_OK) != -1)
        {
            // nothing
            system("touch service.is.blocked");
            usleep(10000); // sleep for 0.01 second
        }
        else
        {
            system("rm -rf service.is.blocked");
            system("touch read.lock");

            FILE *fptr = fopen("input_devices.lst", "r");
            if (fptr == NULL)
            {
                printf("Error opening file!\n");
                service_running = false;
                errnum=2;
            }

            char device_path[256];
            if (fgets(device_path, sizeof(device_path), fptr))
            {
                char command[512];
                snprintf(command, sizeof(command), "cat %s > events", device_path);
                system(command);
            }
            fclose(fptr);

            system("rm -rf read.lock");
            /// usleep(40000); // sleep for 0.04 second
            // system("cat input_devices.lst");
        }
    }
    // Enable echoing
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("Keyboard event service exited\n");
    chdir(current);
}