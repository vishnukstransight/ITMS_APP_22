#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <math.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <libgen.h>
#include "cJSON.h"
#include <sys/types.h>
#include <linux/limits.h>
#include <linux/input.h>
#include <signal.h>
#include <ncurses.h>
#include "Hexprocessor.h"
#include "HexPreprocessor.h"

#include "CopyITMS.h"
#include "GeoFence.h"
#include "JsonFetching.h"
#include "HexPostProcessor.h"
#include "uartFunctions.h"
/////////////////////////////////////////LED
int open_port_LED(void)
{
    int fd;
    fd = open("/dev/ttyUSB1", O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)
    {
        perror("open_port: Unable to open /dev/ttyUSB1 - ");
    }
    else
    {
        struct termios options;
        tcgetattr(fd, &options);
        cfsetispeed(&options, B19200);
        cfsetospeed(&options, B19200);
        options.c_cflag |= (CLOCAL | CREAD);
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        options.c_cflag &= ~CSIZE;
        options.c_cflag |= CS8;
        options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
        options.c_oflag &= ~OPOST;
        tcsetattr(fd, TCSANOW, &options);
    }
    return fd;
}
////////////////////////////////////////////LED
/////////////////////////////////////////////GPS
int open_port_GPS(void) {
    int fd;
    fd = open("/dev/ttyUSB2", O_RDWR | O_NOCTTY);
    if (fd == -1) {
        perror("open_port: Unable to open /dev/ttyUSB2 - ");
    } else {
        struct termios options;
        tcgetattr(fd, &options);
        cfsetispeed(&options, B9600);
        cfsetospeed(&options, B9600);
        options.c_cflag |= (CLOCAL | CREAD);
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        options.c_cflag &= ~CSIZE;
        options.c_cflag |= CS8;
        options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
        options.c_oflag &= ~OPOST;
        tcsetattr(fd, TCSANOW, &options);
    }
    return fd;
}
/////////////////////////////////////////////GPS

void send_file_via_serial(const char *file_path, int serial_fd)
{
    int n;
    char hex_value[3];
    unsigned char a;

    FILE *file = fopen(file_path, "r");

    if (file != NULL)
    {
        while (fread(hex_value, 1, 2, file) == 2)
        {
            a = ((unsigned char)strtol(hex_value, NULL, 16));

            n = write(serial_fd, &a, 1);
            if (n < 0)
            {
                perror("write() failed!\n");
            }

            // printf("Write complete\n");
        }

        fclose(file);
    }
    else
    {
        perror("Failed to open the file\n");
    }
}

