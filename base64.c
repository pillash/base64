#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "base64.h"

int parseArguments(int argc, char **argv, char **mode, char **filename);
uint8_t *readFileBinary(char *filename, unsigned long *size);

int main(int argc, char ** argv) {

    char *mode;
    char *filename;
    if (parseArguments(argc, argv, &mode, &filename) == 0) {
        printf("Format: base64 [-e|-d] filename\n");
        return 0;
    }

    if (strcmp(mode, "-e") == 0) {
        unsigned long size;
        uint8_t *input = readFileBinary(filename, &size);

        unsigned long eSize;
        char *output = base64encode(input, size, &eSize);
        printf("%s", output);
        free(input);
        return 0;
    } else if (strcmp(mode, "-d") == 0) {
        unsigned long size;
        char *input = (char *)readFileBinary(filename, &size);

        unsigned long ds;
        uint8_t *output = base64decode(input, size, &ds);
        if (output == NULL) {
            printf("Error decoding\n");
            return 1;
        }

        for (unsigned int i = 0; i < ds; i++) {
            printf("%c", (char)output[i]);
        }

        free(input);
        return 0;
    } else {
        printf("Format: base64 [-e|-d] filename\n");
        return 1;
    }
}

uint8_t *readFileBinary(char *filename, unsigned long *size) {
    FILE *filePtr;
    long fileSize;

    filePtr = fopen(filename, "rb");
    if (filePtr == NULL) {
        return NULL;
    }

    fseek(filePtr, 0, SEEK_END);
    fileSize = ftell(filePtr);
    if (fileSize == -1) {
        return NULL;
    }

    rewind(filePtr);

    uint8_t *fileBuf = malloc((unsigned long)fileSize);
    if (fileBuf == NULL) {
        return NULL;
    }

    if (fread(fileBuf, 1, (unsigned long)fileSize, filePtr) != (unsigned long)fileSize) {
        printf("Didn't read expected number of bytes\n");
        return NULL;
    }

    fclose(filePtr);
    *size = (unsigned long)fileSize;
    return fileBuf;
}

int parseArguments(int argc, char **argv, char **mode, char **filename) {
    if (argc != 3) {
        return 0;
    }

    char *arg1 = argv[1];
    char *arg2 = argv[2];

    if (strcmp(arg1, "-e") == 0 || strcmp(arg1, "-d") == 0) {
        *mode = arg1;
        *filename = arg2;
        return 1;
    }

    if (strcmp(arg2, "-e") == 0 || strcmp(arg2, "-d") == 0) {
        *mode = arg2;
        *filename = arg1;
        return 1;
    }

    return 0;
}
