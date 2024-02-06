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




int directoryExists(const char *path)
{
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

void copy_directory(const char *src, const char *dest)
{
    char cmd[200];
    snprintf(cmd, sizeof(cmd), "cp -r '%s' '%s'", src, dest);
    system(cmd);
}

void listDirectoryContents(const char *directory)
{
    // Open the specified directory
    DIR *dir = opendir(directory);
    if (dir == NULL)
    {
        perror("Failed to open directory");
        return;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        // Exclude current and parent directory entries
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            printf("%s/%s\n", directory, entry->d_name);
        }
    }
    closedir(dir);
}

void processConnectedStorageDevice(const char *device_name, const char *program_directory, const char *username)
{
    char media_directory[200];
    snprintf(media_directory, sizeof(media_directory), "/media/%s", username);

    char device_path[256];
    snprintf(device_path, sizeof(device_path), "%s/%s", media_directory, device_name);

    // Check if "ITMS" folder is present in the device
    char itms_path[256];
    snprintf(itms_path, sizeof(itms_path), "%s/ITMS", device_path);

    if (directoryExists(itms_path))
    {
        printf("ITMS folder is present in Device %s\n", device_name);

        // Copy the "ITMS" folder to the directory of the executing program
        char program_destination[256];
        snprintf(program_destination, sizeof(program_destination), "%s/ITMS", program_directory);
        copy_directory(itms_path, program_destination);
    }

    printf("Contents of Device %s:\n", device_name);
    listDirectoryContents(device_path);
    printf("\n");
}

void listConnectedStorageDevices(const char *username, const char *program_directory)
{
    char media_directory[200];
    snprintf(media_directory, sizeof(media_directory), "/media/%s", username);

    // Open the /media directory
    DIR *dir = opendir(media_directory);
    if (dir == NULL)
    {
        perror("Failed to open /media directory");
        return;
    }

    printf("Connected Storage Devices:\n");
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            printf("Device: %s\n", entry->d_name);
            processConnectedStorageDevice(entry->d_name, program_directory, username);
        }
    }

    closedir(dir);
}

void clone_the_sourcefolder()
{
    char username[50]; // Buffer for storing the username
    if (getlogin_r(username, sizeof(username)) != 0)
    {
        perror("Failed to get the username");
        return;
    }

    // Get the directory of the executing program
    char program_directory[200];
    char executable_path[200];
    ssize_t count = readlink("/proc/self/exe", executable_path, sizeof(executable_path) - 1);
    if (count != -1)
    {
        executable_path[count] = '\0';
        snprintf(program_directory, sizeof(program_directory), "%s", dirname(executable_path));
    }
    else
    {
        perror("Failed to get the program's directory");
        return;
    }

    listConnectedStorageDevices(username, program_directory);
}
