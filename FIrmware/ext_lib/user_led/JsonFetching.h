#ifndef JSONFETCHING_H
#define JSONFETCHING_H
#define MAX_STRINGS 10
extern int numStations ;
extern int RouteCount ;
extern char *strings[MAX_STRINGS];
extern char *LEDID[50]; // Led id from .bus file
struct station
{
    //int Frame_number;
    //char Frame_speed[10];
    char File_name[20];
    char Station_id[30];
    char Station_name[30];
    float Station_Long;
    float Station_Latti;
    char UPDat_name[10];
    char DNDat_name[10];
    //int delay_value;
};

struct route_options
{
    struct station route_id;
    char route_name[20];
};
extern struct route_options routeOptions[50];
int fetch_station_details(char *routeid, int x);
int fetch_data();
void processLEDIDFolder(const char *base_directory, const char *LEDID, int user_ip, int x);

#endif
