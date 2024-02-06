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
#include "FileManagement.h"


void deleteFolderContents(const char *folderPath)
{
    struct dirent *entry;
    DIR *dir = opendir(folderPath);

    if (!dir)
    {
        perror("Error opening directory");
        return;
    }

    while ((entry = readdir(dir)))
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        char entryPath[PATH_MAX];
        snprintf(entryPath, sizeof(entryPath), "%s/%s", folderPath, entry->d_name);

        if (remove(entryPath) != 0)
        {
            perror("Error deleting file");
        }
    }

    closedir(dir);
}




void processFile(const char *inputFileName, const char *outputFileName, double radius) {
    FILE *inputFile = fopen(inputFileName, "r");
    FILE *outputFile = fopen(outputFileName, "w");

    if (inputFile == NULL || outputFile == NULL) {
        fprintf(stderr, "Error opening files for '%s' data.\n", inputFileName);
        return;
    }

    char line[100]; // Assuming a line length of 100 characters is sufficient
    double centerX, centerY;

    while (fgets(line, sizeof(line), inputFile) != NULL) {
        if (sscanf(line, "%lf %lf", &centerX, &centerY) == 2) {
            generatePolygonVertices(centerX, centerY, radius, 5, outputFile);
            fprintf(outputFile, "\n");
        }
    }

    fclose(inputFile);
    fclose(outputFile);
}
