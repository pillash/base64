#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "base64.h"

uint8_t *readFileBinary(char *filename, unsigned long *size);

int main(int argc, char ** argv) {

    if (argc != 2) {
        printf("Provide 1 filename\n");
        return 0;
    }

    char *filename = argv[1];
    unsigned long size;
    uint8_t *input = readFileBinary(filename, &size);

    unsigned long eSize;
    char *output = base64encode(input, size, &eSize);
    printf("%s\n", output);
    free(input);

    unsigned long ds;
    input = base64decode(output, eSize, &ds);
    if (input == NULL) {
        printf("Error decoding\n");
    }

    for (unsigned int i = 0; i < ds; i++) {
        printf("%c", (char)input[i]);
    }

    printf("\n");

    free(input);
    free(output);

	return 0;
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
