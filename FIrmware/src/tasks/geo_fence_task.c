#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <gps.h>
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

#define MAX_SENTENCE_LENGTH 79
#define MAX_STRING_LENGTH 50
#define PI 3.14159265
#define LATITUDE_LENGTH 12  // Adjust the length based on your actual requirements
#define LONGITUDE_LENGTH 12

/****************************************
 * Global Declarations
 ****************************************/

float Station_Long;
float Station_Latti;


char *LEDID[50]; // Led id from .bus file
int RouteCount = 0;
int numStations = 0;
char *strings[MAX_STRINGS];
char *strings_stopid[MAX_STRINGS];
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 1
// Function to convert NMEA coordinates to normal GPS coordinates
double nmeaToNormalGPS_LAT(const char *nmeaCoordinate) {
    int degrees;
    double minutes;

    // Extracting degrees and minutes
    sscanf(nmeaCoordinate, "%2d%lf", &degrees, &minutes);

    // Performing the conversion
    double normalGPS = degrees + (minutes / 60.0);

    return normalGPS;
}
double nmeaToNormalGPS_LON(const char *nmeaCoordinate) {
    int degrees;
    double minutes;

    // Extracting degrees and minutes
    sscanf(nmeaCoordinate, "%3d%lf", &degrees, &minutes);

    // Performing the conversion
    double normalGPS = degrees + (minutes / 60.0);

    return normalGPS;
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
int main(int argc, char *argv[])
{
    const char *folderPath = "./Dest"; // Change this to the folder you want to empty
    deleteFolderContents(folderPath);
    char RouteString[6];
    int user_input, up_down, current_route;
    clone_the_sourcefolder(); // the function for  copying the file into the current folder
    fetch_data();             // the function for fetching route count and route data
    printf("........................enter the route-number: ....................................");
    scanf("%d", &user_input);
    printf("if route is %s enter 0, Else enter 1: ", routeOptions[user_input].route_id.Station_name);
    scanf("%d", &up_down);
    current_route = user_input + 1;
    snprintf(RouteString, sizeof(RouteString), "0000%d", current_route);
    TextFetch(RouteString);
    if (user_input > RouteCount || user_input < 0)
    {
        printf("User input is out of scope\n");
        return 1;
    }

    fetch_station_details(strings[0], user_input); // function for fetching the station details

    if (LEDID[1] != NULL)
    {
        for (int i = 0; i < numStations; i++)
        {
            processLEDIDFolder("ITMS/", LEDID[1], user_input, i);
        }
    }
    // Process "up" data
    processFile("up_csv_test.csv", "up_output.txt", 100 / 111325.0);
    // Process "down" data
    processFile("down_csv_test.csv", "down_output.txt", 50 / 111325.0);
    int line_number = 1;
    int fd_GPS = open_port_GPS();
    int fd = open_port_LED();////////////////////////////////////////////////////////now
    unsigned char buffer[BUFFER_SIZE];
    ssize_t n;

    char latitude[10];
    char longitude[11];
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    char savedSentence[MAX_SENTENCE_LENGTH + 1]; // +1 for null terminator
    char Latitude[LATITUDE_LENGTH + 1];          // +1 for null terminator
    char Longitude[LONGITUDE_LENGTH + 1];          // +1 for null terminator
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int point_in_polygon_counter = 0;
    int scanning_flag = 0;
    int breakFlag = 0; // Flag to control the outer loop
     int i_past_value = 0;
      char *route_srctodst;
      char filename_currentStop[30]; // Assuming the maximum length of "Dest/UP001.txt" won't exceed 30 characters
   if (fd_GPS != -1 && fd != -1) {////////////////////////////////////////////////////////now
   //if (fd_GPS != -1 ) {////////////////////////////////////////////////////////now
        printf("Serial port GPS opened successfully with file descriptor: %d\n", fd_GPS);
       // printf("Serial port LED opened successfully with file descriptor: %d\n", fd);////////////////////////////////////////////////////////now
        while (1) {
            n = read(fd_GPS, buffer, sizeof(buffer));
            if (n > 0) {
               // for (int i = 0; i < n; ++i) {
                    // Check if the received data starts with the specified tag at position i
                char *sentenceStart = strstr((char *)buffer, "$GNGGA,");
                if (sentenceStart != NULL)
                {
                    // Copy the desired sentence along with the next 41 characters to the variable
                    strncpy(savedSentence, sentenceStart, MAX_SENTENCE_LENGTH);
                    savedSentence[MAX_SENTENCE_LENGTH] = '\0'; // Null-terminate the string

                    // Extract and save the 19th to 29th characters from savedSentence in Latitude
                    if (strlen(savedSentence) >= 44)
                    {
                        snprintf(Latitude, sizeof(Latitude), "%.*s", 11, savedSentence + 18);
                        snprintf(Longitude, sizeof(Longitude), "%.*s", 12, savedSentence + 32);
                           // Example NMEA coordinates for India
                        const char *nmeaLatitude = Latitude;  // Use 'N' for North in India
                        const char *nmeaLongitude = Longitude; // Use 'E' for East in India
                        
                           // Convert NMEA coordinates to normal GPS coordinates
                        double normalLatitude = nmeaToNormalGPS_LAT(nmeaLatitude);
                        double normalLongitude = nmeaToNormalGPS_LON(nmeaLongitude);
                            // Printing the converted coordinates
                        printf("Latitude: %.6f\n", normalLatitude);
                        printf("Longitude: %.6f\n", normalLongitude);
                            x = normalLongitude;////////////////////////////////////////////////////////now
                            y = normalLatitude;////////////////////////////////////////////////////////now

    for (int i = 1; i < 8; i++) {
        readPolygonFromFile("up_output.txt", i);

        // You can now use the latitude and longitude variables as needed
        if (pointInPolygon()) {
            if (i_past_value != i) {
                printf("Inside ploygon when i: %d\n", i);
                route_srctodst = routeOptions[i-1].route_id.UPDat_name;
                sprintf(filename_currentStop, "Dest/%s.txt", route_srctodst);
               send_file_via_serial(filename_currentStop, fd);////////////////////////////////////////////////////////now
                //close(fd);
                 sleep(1);
                i_past_value = i;  // Set the flag to true after printing
            }
        } else {
            printf("\n....OUTSIDE.....\n");
        }
        }                        
                        }
                    }
              //  }
            } else if (n == 0) {
                printf("No data received\n");
            } else {
                perror("read() failed!\n");
                break;
            }

            // Add a delay if needed to control the rate of reading
            //sleep(1); // 100ms delay
             usleep(1000000); // 100ms delay
        }

        close(fd_GPS);
        close(fd);////////////////////////////////////////////////////////now
    }


    
  

    return 0;
}
#endif



void geo_fence_task_func(void *pParameters) {

const char *folderPath = "./ext_lib/user_led//Dest"; // Change this to the folder you want to empty
    deleteFolderContents(folderPath);
    char RouteString[6];
    int user_input, up_down, current_route;
    clone_the_sourcefolder();
    fetch_data();
    printf("........................enter the route-number: ....................................");
    scanf("%d", &user_input);
    printf("if route is %s enter 0, Else enter 1: ", routeOptions[user_input].route_id.Station_name);
    scanf("%d", &up_down);
    current_route = user_input + 1;
    snprintf(RouteString, sizeof(RouteString), "0000%d", current_route);
    TextFetch(RouteString);
    if (user_input > RouteCount || user_input < 0) {
        printf("User input is out of scope\n");
        return 1;
    }

    fetch_station_details(strings[0], user_input);

    if (LEDID[1] != NULL) {
        for (int i = 0; i < numStations; i++) {
            processLEDIDFolder("ITMS/", LEDID[1], user_input, i);
        }
    }

    // Process "up" data
    processFile("/ext_lib/user_led/up_csv_test.csv", "/ext_lib/user_led/up_output.txt", 100 / 111325.0);
    // Process "down" data
    processFile("/ext_lib/user_led/down_csv_test.csv", "/ext_lib/user_led/down_output.txt", 50 / 111325.0);

    int line_number = 1;
    int fd = open_port_LED();
    unsigned char buffer[BUFFER_SIZE];
    ssize_t n;

    char latitude[10];
    char longitude[11];
    char savedSentence[MAX_SENTENCE_LENGTH + 1];
    char Latitude[LATITUDE_LENGTH + 1];
    char Longitude[LONGITUDE_LENGTH + 1];

    int point_in_polygon_counter = 0;
    int scanning_flag = 0;
    int breakFlag = 0;
    int i_past_value = 0;
    char *route_srctodst;
    char filename_currentStop[30];


    if (fd != -1) {
    //     printf("Serial port LED opened successfully with file descriptor: %d\n", fd);

        extern struct gps_data_t OGps_data;
        extern pthread_mutex_t gpsd_mutex;
        int status;
        // // Open a connection to the local gpsd service
        // if ((status = gps_open("localhost", "2947", &gps_data)) == -1) {
        //     fprintf(stderr, "gps_open() failed: %s\n", gps_errstr(status));
        //     exit(EXIT_FAILURE);
        // }

        // // Enable JSON output
        // gps_stream(&gps_data, WATCH_ENABLE | WATCH_JSON, NULL);

        while (1) {
              
            // if (gps_waiting(&OGps_data, 500000)) { // 5 seconds timeout
                pthread_mutex_lock(&gpsd_mutex);
                status = gps_read(&OGps_data, NULL, 0);
                if (status == -1) {
                    fprintf(stderr, "gps_read() failed: %s\n", status);
                    pthread_mutex_unlock(&gpsd_mutex);
                    exit(EXIT_FAILURE);
                } else {
                    
                    x = OGps_data.fix.longitude;
                    y = OGps_data.fix.latitude;
                    
                    pthread_mutex_unlock(&gpsd_mutex);
                        // fprintf(stderr," latitude = %03d.%06d\n\r",(uint32_t)y);
                        // fprintf(stderr," longitude = %03d.%06d\n\r",(uint32_t)x);
                   // printf("Latitude: %lf, Longitude: %lf\n",gps_data.fix.latitude, gps_data.fix.longitude);

                    for (int i = 1; i < 8; i++) {
                        readPolygonFromFile("up_output.txt", i);

                        if (pointInPolygon()) {
                            if (i_past_value != i) {
                                // printf("Inside polygon ");

                                sleep(1);
                                i_past_value = i;
                            }
                        } else {
                            // printf("\n....OUTSIDE.....\n");
                        }
                    }
                }
            // } else {
            //     fprintf(stderr, "Timeout waiting for GPS data\n");
            // }

            usleep(100000); // 100ms delay
        }

        // Close the gpsd connection
        // gps_stream(&gps_data, WATCH_DISABLE, NULL);
        // gps_close(&gps_data);

        close(fd);
    }

}