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
//#include <json-c/json.h>
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
#include "HexPreprocessor.h"



char **getDatFiles(const char *folder_path, const char *file_extension, int *count)
{
    struct dirent *entry;

    // Open the directory
    DIR *dp = opendir(folder_path);

    if (dp == NULL)
    {
        perror("Error opening the directory");
        return NULL;
    }

    // Initialize an array to store file names
    char **dat_files = NULL;
    *count = 0;

    // Read the directory entries
    while ((entry = readdir(dp)) != NULL)
    {
        // Check if the entry ends with the desired extension
        size_t name_len = strlen(entry->d_name);
        size_t ext_len = strlen(file_extension);

        if (name_len >= ext_len &&
            strcmp(entry->d_name + name_len - ext_len, file_extension) == 0)
        {
            // Dynamically allocate memory for each filename
            char *filename = strdup(entry->d_name);
            if (filename != NULL)
            {
                dat_files = (char **)realloc(dat_files, (*count + 1) * sizeof(char *));
                dat_files[(*count)++] = filename;
            }
        }
    }

    // Close the directory
    closedir(dp);

    return dat_files;
}
void TextFetch(char *RouteString)
{
    const char *base_path = "./ITMS/AARGEE/";
    const char *suffix = "/02";
    char folder_path[100]; // Make sure this array is large enough to hold the resulting path.
    // Concatenate the base path, RouteString, and suffix to form the folder_path.
    strcpy(folder_path, base_path);
    strcat(folder_path, RouteString);
    strcat(folder_path, suffix);
    const char *folder_path_ptr = folder_path;
    const char *file_extension = ".dat";
    int count;
    const char *filename;
    char **dat_files = getDatFiles(folder_path, file_extension, &count);
    const char *fileptr; // Declare the const char* variable
    if (dat_files == NULL)
    {
        // return 1;
        return;
    }
    char **text_files = (char **)malloc(count * sizeof(char *));
    if (text_files == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        // return 1;
        return;
    }
    for (int dat_count = 0; dat_count < count; dat_count++)
    {
        const char *dat_filename = dat_files[dat_count];
        const char *extension = ".txt";
        size_t length = strlen(dat_filename) + strlen(extension);
        text_files[dat_count] = (char *)malloc(length + 1); // +1 for the null-terminator
        if (text_files[dat_count] == NULL)
        {
            fprintf(stderr, "Error: Memory allocation failed.\n");
            // return 1;
            return;
        }
        strcpy(text_files[dat_count], dat_filename);
        strcpy(text_files[dat_count] + strlen(dat_filename) - strlen(file_extension), extension);
    }
    // Print or use the file names
    for (int dat_count = 0; dat_count < count; dat_count++)
    {
        printf("%s\n", dat_files[dat_count]);
        //}
        // Construct the filename using dat_files[i]
        char filename[300]; // Adjust the buffer size as needed
        // snprintf(filename, sizeof(filename), "DAT_FILES/%s", dat_files[dat_count]);
        snprintf(filename, sizeof(filename), "%s/%s", folder_path, dat_files[dat_count]);
        // Assign the value of 'filename' to 'fileptr'
        fileptr = filename;
        // const char* filename = "DAT_FILES/UP002.dat";
        unsigned int *HexData = fnDat2hex_Converter(filename);
        // Open the "UP001.dat" file in binary mode
        FILE *file = fopen(filename, "rb");
        // Determine the size of the file
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        int arr_size = file_size / sizeof(uint8_t);
        fnHexmerger(HexData, arr_size, text_files[dat_count]);
        printf("\nByte size:%d\n", arr_size);
    }
    // Free allocated memory for the file names
    for (int i = 0; i < count; i++)
    {
        free(dat_files[i]);
        free(text_files[i]);
    }
    free(dat_files);
    free(text_files);
}
