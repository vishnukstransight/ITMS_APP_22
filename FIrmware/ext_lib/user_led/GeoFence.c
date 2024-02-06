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

int polyCorners;
float polyX[MAX_POLY_CORNERS];
float polyY[MAX_POLY_CORNERS];
float x, y;

// Function to check if a point is inside a polygon
bool pointInPolygon() {
    int i, j = polyCorners - 1;
    bool oddNodes = false;

    for (i = 0; i < polyCorners; i++) {
        if ((polyY[i] < y && polyY[j] >= y || polyY[j] < y && polyY[i] >= y) &&
            (polyX[i] <= x || polyX[j] <= x)) {
            oddNodes ^= (polyX[i] + (y - polyY[i]) / (polyY[j] - polyY[i]) * (polyX[j] - polyX[i]) < x);
        }
        j = i;
    }

    return oddNodes;
}

// Function to read polygon coordinates from file
void readPolygonFromFile(const char *filename, int lineNumber) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[BUFFER_SIZE];
    for (int i = 1; i <= lineNumber; ++i) {
        if (fgets(line, sizeof(line), file) == NULL) {
            fprintf(stderr, "Error reading from file or line number too large\n");
            exit(EXIT_FAILURE);
        }
    }

    // Parse the line to extract polygon coordinates
    sscanf(line, "(%f, %f), (%f, %f), (%f, %f), (%f, %f), (%f, %f)",
           &polyX[0], &polyY[0], &polyX[1], &polyY[1], &polyX[2], &polyY[2],
           &polyX[3], &polyY[3], &polyX[4], &polyY[4]);

   // printf("(X1:%f,Y1:%f),(X2:%f,Y2:%f),(X3:%f,Y3:%f),(X4:%f,Y4:%f),(X5:%f,Y5:%f)\n",
          // polyX[0], polyY[0], polyX[1], polyY[1], polyX[2], polyY[2], polyX[3], polyY[3], polyX[4], polyY[4]);
    polyCorners = 5;

    fclose(file);
}

// Function to calculate the coordinates of a vertex
void calculateVertex(double centerX, double centerY, double radius, double angle, double *x, double *y) {
    *x = centerX + radius * cos(angle);
    *y = centerY + radius * sin(angle);
}

// Function to generate and print vertices of a polygon
void generatePolygonVertices(double centerX, double centerY, double radius, int numVertices, FILE *outputFile) {
    fprintf(outputFile, "(");

    for (int i = 0; i < numVertices; ++i) {
        double angle = 2 * PI * i / numVertices; // Angle for each vertex
        double x, y;
        calculateVertex(centerX, centerY, radius, angle, &x, &y);

        // Print coordinates in the format (x, y)
        fprintf(outputFile, "%.6lf, %.6lf", x, y);

        // Add a comma after each coordinate pair (except the last one)
        if (i < numVertices - 1) {
            fprintf(outputFile, "), (");
        }
    }

    fprintf(outputFile, ")");
}
