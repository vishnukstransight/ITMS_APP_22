#ifndef __GEOFENCE_H_
#define __GEOFENCE_H_

#define PI 3.14159265
#define BUFFER_SIZE 1000
#define MAX_POLY_CORNERS 100

extern int polyCorners;
extern float polyX[MAX_POLY_CORNERS];
extern float polyY[MAX_POLY_CORNERS];
extern float x, y;

bool pointInPolygon();
void readPolygonFromFile(const char *filename, int lineNumber);
void calculateVertex(double centerX, double centerY, double radius, double angle, double *x, double *y);
void generatePolygonVertices(double centerX, double centerY, double radius, int numVertices, FILE *outputFile);
  
#endif  
