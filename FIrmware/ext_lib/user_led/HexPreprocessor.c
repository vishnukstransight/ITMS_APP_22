

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "HexPreprocessor.h"

//static unsigned int Hexpreprocessor[387];

unsigned int* fnDat2hex_Converter(const char* filename)
{
    // Open the "UP001.dat" file in binary mode
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening the file");
        return NULL;
    }

    // Determine the size of the file
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read the binary data from the file into an array
    uint8_t *binary_data = (uint8_t *)malloc(file_size);
    if (fread(binary_data, sizeof(uint8_t), file_size, file) != file_size) {
        perror("Error reading from the file");
        fclose(file);
        free(binary_data);
        return NULL;
    }

    // Close the file
    fclose(file);

    // Calculate the number of values based on the size of the file
    int num_values = file_size / sizeof(uint8_t);

    // Create an array to store decimal values
    uint64_t *decimal_array = (uint64_t *)malloc(num_values * sizeof(uint64_t));

    // Convert binary data to decimal integers and store in the array
    for (int i = 0; i < num_values; i++) {
        decimal_array[i] = binary_data[i];
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 1
unsigned int* Hexpreprocessor = (unsigned int*)malloc(num_values * sizeof(unsigned int));
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Copy decimal values to Hexpreprocessor
    for (int i = 0; i < num_values; i++) {
        if (i < num_values) {
            Hexpreprocessor[i] = (unsigned int)decimal_array[i];
        } else {
            // Handle the case where num_values is less than 387
            Hexpreprocessor[i] = 0;  // Set a default value or take any other appropriate action
        }
    }

    // Print the decimal values
   // for (int i = 0; i < ; i++) {
       // printf("\n%d\n", Hexpreprocessor[i]);
   // }

    // Free dynamically allocated memory
    free(binary_data);
    free(decimal_array);

    return Hexpreprocessor;
}

