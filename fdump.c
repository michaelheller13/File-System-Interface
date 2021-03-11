//Michael Heller
//fdump.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "hexdump.h"


int main (int argc, char *argv[]) {
    //ensure correct parameters
    char error_msg[100] = "Error: Bad parameters. Please give char array, unsigned integer, unsigned integer in that order.";
    if (argc != 4) {
        printf("%s\n", error_msg);
        exit(-1);
    }
    if (strlen(argv[1]) < 2) {
        printf("%s\n", error_msg);
        exit(-1);
    }
    if (sizeof(atoi(argv[2])) != sizeof(unsigned int)) {
        printf("%s\n", error_msg);
        exit(-1);
    }
    if (sizeof(atoi(argv[3])) != sizeof(unsigned int)) {
        printf("%s\n", error_msg);
        exit(-1);
    }

    unsigned int buffer_size = 512;
    unsigned int offset = atoi(argv[2]);
    unsigned int size = atoi(argv[3]);
    unsigned char buffer[buffer_size];

    //open file
    FILE *file;
    file = fopen(argv[1], "r");
    //shift forward the file position indicator by offset
    fseek(file, offset, SEEK_CUR);
    //read size bytes from filename into a buffer
    fread(buffer, size, 1, file);
    //call hexdump
    hexdump(buffer, size);

    fclose(file);
    return 0;
}
