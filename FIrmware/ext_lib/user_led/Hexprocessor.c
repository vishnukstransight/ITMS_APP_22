    #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Hexprocessor.h"

int text_name_count = 0;
unsigned int sumTargetArray(unsigned int* targetArray,unsigned  int length) {
   unsigned  int sum = 0;
    for (int i = 0; i < length; i++) {
        sum += targetArray[i];
    }
    return sum;
}

void fnHexmerger(unsigned int* HexData, int length,const char* filename) {

//text_name_count = dat_count;
   unsigned int Array1[length]; // Declare Array1 with a size of 10
//   unsigned int Array2[7] = {2 ,50 ,37 ,1 ,1 ,1 ,131};
///////////////////////////////////////////////////////////////////////////////////////////////////////
#if 1
unsigned int Array2[7];
if(length==387)
{
//Array2[7] = {2 ,50 ,37 ,1 ,1 ,1 ,131};
Array2[0] = 2;
Array2[1] = 50;
Array2[2] = 37;
Array2[3] = 1;
Array2[4] = 1;
Array2[5] = 1;
Array2[6] = 131;
}
else if(length==545)
{
Array2[0] = 2;
Array2[1] = 50;
Array2[2] = 37;
Array2[3] = 1;
Array2[4] = 1;
Array2[5] = 2;
Array2[6] = 33;
}
else if(length==703)
{
Array2[0] = 2;
Array2[1] = 50;
Array2[2] = 37;
Array2[3] = 1;
Array2[4] = 1;
Array2[5] = 2;
Array2[6] = 191;
}
else if(length==861)
{
Array2[0] = 2;
Array2[1] = 50;
Array2[2] = 37;
Array2[3] = 1;
Array2[4] = 1;
Array2[5] = 3;
Array2[6] = 93;
}
else if(length==1350)
{
Array2[0] = 2;
Array2[1] = 50;
Array2[2] = 37;
Array2[3] = 1;
Array2[4] = 1;
Array2[5] = 2;
Array2[6] = 191;
}

#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////
   unsigned int ProcessedArray[length+8];
   unsigned int targetArray[length+7];
unsigned int ChecksumArray[1];
    int i, j;

    // Copy elements of Hexpreprocessor to Array1
    for (i = 0; i < length; i++) {
        Array1[i] = HexData[i];
    }

    // Copy elements of Array2 to targetArray
    for (i = 0; i < 7; i++) {
        targetArray[i] = Array2[i];
    }

    // Copy elements of Array1 to targetArray
    for (j = 0; j <length ; j++) {
        targetArray[i + j] = Array1[j];
    }

    // Print targetArray
    //for (i = 0; i < length+7; i++) {
      //  printf("%d ", targetArray[i]);
   // }
    // Calculate the sum of targetArray elements and store it in a variable
   unsigned  int sum = sumTargetArray(targetArray, length + 7);
   // printf("\nSum of targetArray elements: %d\n", sum);
   char hex_string[9]; // 8 characters for the hexadecimal representation and 1 for the null terminator
   sprintf(hex_string, "%08X", sum);
   //printf("Hexadecimal representation: 0x%s\n", hex_string);
   unsigned int decimal_value;
      // Extract the last two characters (A9)
   char hex_value[3];
   hex_value[0] = hex_string[6];
   hex_value[1] = hex_string[7];
   hex_value[2] = '\0';

   // Use strtol to convert the extracted hexadecimal string to a decimal value
   decimal_value = strtol(hex_value, NULL, 16);
   printf("Decimal value: %u\n", decimal_value);
   ChecksumArray[0] = decimal_value;
    printf("\nCheck sum array%d\n ",ChecksumArray[0]);
    // Copy elements from targetArray to ProcessedArray
    for (int i = 0; i < length+7 ; i++) {
        ProcessedArray[i] = targetArray[i];
    }
   ProcessedArray[length+7] = ChecksumArray[0];
  
 // Print targetArray
    for (i = 0; i < length+8; i++) {
      ProcessedArray[i] == 0;
    }

    for (i = 0; i < length+8; i++) {
      printf("%02X",ProcessedArray[i]);
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int length_2 = sizeof(ProcessedArray) / sizeof(ProcessedArray[0]);
//   FILE *file = fopen("test.txt", "w"); // Open the file for writing
#if 1

//char filename[100];  // Create a buffer to store the filename
       // sprintf(filename, "%d.txt", text_name_count);  // Format the filename
       // FILE *file_txt = fopen(filename, "w");  // Open the file for writing
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char folderName[] = "./Dest";

// Construct the full path by combining folderName and fileName
char fullPath[200]; // Assuming a reasonable maximum path length
snprintf(fullPath, sizeof(fullPath), "%s/%s", folderName, filename);
FILE *file_txt = fopen(fullPath, "w");  // Open the file for writing
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        if (file == NULL) {
  //          printf("Error opening file %s\n", filename);
    //        return 1;  // Exit the program with an error code
      //  }

        // Write data to the file, if needed
        // For example: fprintf(file, "This is file %d\n", text_name_count);

//        fclose(file_txt);  // Close the file
#endif

#if 0
    for (int i = 0; i < length_2; i++) {
        fprintf(file_txt, "%d", ProcessedArray[i]); // Write the current number to the file
        if (i < length_2 - 1) {
            fprintf(file_txt, ","); // Add a comma after each value except the last one
        }
    }
#endif
#if 1
    for (int i = 0; i < length_2; i++) {
        fprintf(file_txt, "%02X", ProcessedArray[i]); // Write the current number to the file
    }
#endif
   // fclose(file); // Close the file
fclose(file_txt);  // Close the file

   printf("Data has been written to test.txt\n");
   // return 0;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

}



