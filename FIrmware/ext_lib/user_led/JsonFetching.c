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

struct route_options routeOptions[50];
// fetching station data
int fetch_station_details(char *routeid, int x)
{
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
        return 1;
    }
    int y;
    // printf("route id received is %s\n", routeid);
    char json_path[256];
    snprintf(json_path, sizeof(json_path), "%s/ITMS/%s/station.setting.json", program_directory, routeOptions[x].route_id.File_name);
    FILE *file = fopen(json_path, "r");
    if (!file)
    {
        perror("Error opening the settings file");
        return 1;
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *json_string = (char *)malloc(length);
    if (!json_string)
    {
        fclose(file);
        perror("Memory allocation error");
        return 1;
    }
    fread(json_string, 1, length, file);
    fclose(file);
    // Parse the JSON
    cJSON *root1 = cJSON_Parse(json_string);
    if (!root1)
    {
        fprintf(stderr, "JSON parsing error: %s\n", cJSON_GetErrorPtr());
        //free(json_string);
        return 1;
    }
    cJSON *upStationDetail = cJSON_GetObjectItem(root1, "UpStationDetail");
    numStations = cJSON_GetArraySize(upStationDetail);
    //  Extract values and store in the struct
    for (int i = 0; i < numStations; i++)
    {
        cJSON *station = cJSON_GetArrayItem(upStationDetail, i);
        cJSON *id = cJSON_GetObjectItem(station, "ID");
        cJSON *name = cJSON_GetObjectItem(station, "Name");
        cJSON *repoting = cJSON_GetObjectItem(station, "Repoting");
        cJSON *Long = cJSON_GetObjectItem(repoting, "Longitude");
        cJSON *Lati = cJSON_GetObjectItem(repoting, "Latitude");
        printf("Latitude: %f Longitude: %f \n", Lati->valuedouble, Long->valuedouble);
        if (id && name)
        {
            // printf("%d\n",i);
            strncpy(routeOptions[i].route_id.Station_id, id->valuestring, sizeof(routeOptions[i].route_id.Station_id));
            // printf("the passed id value is %s\n",routeOptions[i].route_id.Station_id);
            strncpy(routeOptions[i].route_id.Station_name, name->valuestring, sizeof(routeOptions[i].route_id.Station_name));
            // printf("the passed name is %s\n",routeOptions[i].route_id.Station_name);
 
        routeOptions[i].route_id.Station_Long = Long->valuedouble;
        routeOptions[i].route_id.Station_Latti = Lati->valuedouble;   
        }
    }
    printf("Number of Stations: %d\n", numStations);
    printf("\nStation IDs: \n");
    // Print the extracted values
    for (int i = 0; i < numStations; i++)
    {
        //printf("ID: %s, Name: %s\n", routeOptions[i].route_id.Station_id, routeOptions[i].route_id.Station_name);
        printf("ID: %s, Name: %s\n", routeOptions[i].route_id.Station_id, routeOptions[i].route_id.Station_name);
    }
    printf("\n");

    for (int i = 0; i < numStations; i++)
    {
        char UPdatfilename[10];
        char DNdatfilename[10];
        snprintf(UPdatfilename, sizeof(UPdatfilename), "UP%03d", i + 1);
        snprintf(DNdatfilename, sizeof(DNdatfilename), "DN%03d", numStations - i);
        // printf("%s\n", datfilename);
        strcpy(routeOptions[i].route_id.UPDat_name, UPdatfilename);
        strcpy(routeOptions[i].route_id.DNDat_name, DNdatfilename);
        printf("ID: %s, Name: %s\n", routeOptions[i].route_id.Station_id, routeOptions[i].route_id.Station_name);
        printf("datfile name: %s\n", routeOptions[i].route_id.UPDat_name);
        printf("datfile name: %s\n\n", routeOptions[i].route_id.DNDat_name);
    }

    // Clean up
    free(json_string);
    // cJSON_Delete(root1);
    return 0;
}

/****************************************
 * program access to the folder in current folder and access to .bus file in ITMS folder
 * fetch route data to route structure
 ****************************************/
int fetch_data()
{

    char folder_name[] = "ITMS";
    char *filename = NULL;
    cJSON *root = NULL; // Move this declaration here
    // Check if the folder exists
    if (access(folder_name, F_OK) == 0)
    {
        // Open the current directory
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(folder_name)) != NULL)
        {
            while ((ent = readdir(dir)) != NULL)
            {
                // Check if the file has a ".bus" extension
                if (ent->d_type == DT_REG && strstr(ent->d_name, ".bus") != NULL)
                {
                    char full_path[256]; // Increase buffer size
                    snprintf(full_path, sizeof(full_path), "%s/%s", folder_name, ent->d_name);
                    filename = full_path;
                    break; // Stop after finding the first ".bus" file
                }
            }
            closedir(dir);
        }
        else
        {
            perror("Error opening directory");
            return 1;
        }
    }
    else
    {
        printf("The folder '%s' does not exist.\n", folder_name);
        return 1;
    }
    if (filename == NULL)
    {
        printf("No '.bus' file found in the '%s' folder.\n", folder_name);
        return 1;
    }
    // Read the contents of the .bus file
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *file_contents = (char *)malloc(file_size + 1);
    if (file_contents == NULL)
    {
        perror("Memory allocation error");
        fclose(file);
        return 1;
    }
    fread(file_contents, 1, file_size, file);
    file_contents[file_size] = '\0';
    fclose(file);
    // Parse the JSON data
    root = cJSON_Parse(file_contents);
    if (root == NULL)
    {
        fprintf(stderr, "Error parsing JSON\n");
        //free(file_contents);
        return 1;
    }
    // Access "RouteCount" value
    cJSON *RouteCountItem = cJSON_GetObjectItem(root, "RouteCount");
    if (cJSON_IsNumber(RouteCountItem))
    {
        RouteCount = RouteCountItem->valueint;
    }
    // Access the RouteDetails array
    cJSON *RouteDetails = cJSON_GetObjectItem(root, "RouteDetail");
    if (cJSON_IsArray(RouteDetails))
    {
        for (int i = 0; i < cJSON_GetArraySize(RouteDetails) && i < 50; i++)
        {
            cJSON *RouteItem = cJSON_GetArrayItem(RouteDetails, i);
            cJSON *RouteName = cJSON_GetObjectItem(RouteItem, "RouteName");
            cJSON *RouteID = cJSON_GetObjectItem(RouteItem, "RouteID");
            if (RouteName != NULL && cJSON_IsString(RouteName) && RouteID != NULL && cJSON_IsString(RouteID))
            {
                snprintf(routeOptions[i].route_name, sizeof(routeOptions[i].route_name), "%s", RouteName->valuestring);
                snprintf(routeOptions[i].route_id.File_name, sizeof(routeOptions[i].route_id.File_name), "%s", RouteID->valuestring);
            }
        }
    }
    // Print the extracted values
    printf("RouteCount: %d\n\n", RouteCount);
    printf("Route Options:\n");
    for (int i = 0; i < RouteCount; i++)
    {
        printf("Option:%d Route Name: %s, Route ID: %s\n", i, routeOptions[i].route_name, routeOptions[i].route_id.File_name);
    }
    printf("\n");
    for (int i = 0; i < RouteCount; i++)
    {
        strings[i] = routeOptions[i].route_id.File_name;
    }

    // Access the LEDDetail array
    cJSON *LEDDetail = cJSON_GetObjectItem(root, "LEDDetail");
    if (cJSON_IsArray(LEDDetail))
    {
        for (int i = 0; i < cJSON_GetArraySize(LEDDetail); i++)
        {
            cJSON *LEDItem = cJSON_GetArrayItem(LEDDetail, i);
            cJSON *LEDIDItem = cJSON_GetObjectItem(LEDItem, "ID");
            if (LEDIDItem != NULL && cJSON_IsString(LEDIDItem))
            {
                LEDID[i] = LEDIDItem->valuestring;
            }
            printf("LED ID %d: %s\n", i, LEDID[i]); // print the extracted led id
        }
        printf("\n");
    }
    // Clean up
    // cJSON_Delete(root);
     free(file_contents);

    return 0;
}

/****************************************
 * Read
 * fetch STATION_DATA to STATION_STRUCTURE
 ****************************************/

void processLEDIDFolder(const char *base_directory, const char *LEDID, int user_ip, int x)
{
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
        //return 1;
    }
    int y;
    // printf("route id received is %s\n", routeid);

    // Read the JSON file content
    char json_path[256];
    printf("Station Name: %s \n", routeOptions[x].route_id.Station_name);
    snprintf(json_path, sizeof(json_path), "%s/ITMS/%s/%s/%s.json", program_directory, routeOptions[user_ip].route_id.File_name, LEDID, routeOptions[x].route_id.Station_id);
    FILE *file = fopen(json_path, "r");
    if (!file)
    {
        perror("Error opening the file");
        //return 1;
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *json_string = (char *)malloc(length + 1);
    if (!json_string)
    {
        fclose(file);
        perror("Memory allocation error");
        //return 1;
    }

    fread(json_string, 1, length, file);
    json_string[length] = '\0';
    fclose(file);

    // Parse the JSON
    cJSON *root = cJSON_Parse(json_string);
    if (!root)
    {
        fprintf(stderr, "JSON parsing error: %s\n", cJSON_GetErrorPtr());
        free(json_string);
        //return 1;
    }

    // Extract values from "ContentDetail"
    cJSON *contentDetail = cJSON_GetObjectItem(root, "ContentDetail");
    if (!contentDetail || !cJSON_IsArray(contentDetail) || cJSON_GetArraySize(contentDetail) == 0)
    {
        fprintf(stderr, "Invalid or empty ContentDetail section\n");
        cJSON_Delete(root);
        //free(json_string);
        //return 1;
    }


    // Clean up
     free(json_string);
    // cJSON_Delete(root1);
    //return 0;
}

