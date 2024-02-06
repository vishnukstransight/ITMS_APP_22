#ifndef COPYITMS_H
#define COPYITMS_H


int directoryExists(const char *path);
void copy_directory(const char *src, const char *dest);
void listDirectoryContents(const char *directory);
void processConnectedStorageDevice(const char *device_name, const char *program_directory, const char *username);
void listConnectedStorageDevices(const char *username, const char *program_directory);
void clone_the_sourcefolder();


#endif
