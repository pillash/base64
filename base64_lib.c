#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "base64.h"

unsigned long decodedSize(char *input, unsigned long size);
int8_t charToInt(char c);
char *lookupTable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *base64encode(uint8_t *input, unsigned long size, unsigned long *eSize)
{
    unsigned long encodedSize;
    if (size % 3 == 0) {
        encodedSize = size * 4 / 3;
    } else {
        encodedSize = (size + 3 - (size % 3)) * 4 / 3;
    }

    char *encoded = malloc((encodedSize+1) * sizeof(char));

    unsigned int encodedIndex = 0;
    for (unsigned int i = 0; i < size; i+=3) {
        uint8_t b1 = input[i];
        uint8_t e1 = b1 >> 2; // First 6 bits
        encoded[encodedIndex++] = lookupTable[(int)e1];

        uint8_t b1_2 = (uint8_t)((b1 & (uint8_t)3) << 4);
        if (i+1 < size) {
            uint8_t b2 = input[i+1];
            uint8_t e2 = (b2 >> 4) | b1_2; // Last 2 bits of b1 combined with first 4 bits of b2
            encoded[encodedIndex++] = lookupTable[(int)e2];
            uint8_t b2_2 = (uint8_t)((b2 & (uint8_t)15) << 2); // Last 4 bits of b2, shifted 2 to the left
            if (i+2 < size) {
                uint8_t b3 = input[i+2];
                uint8_t e3 = b2_2 | (b3 >> 6); // Last 4 bits of b2 combined with first 2 bits of b3, if b3 exists
                encoded[encodedIndex++] = lookupTable[(int)e3];
                encoded[encodedIndex++] = lookupTable[(int)(b3 & 63)]; // bast 6 bits of b3
            } else {
                // If b3 doesn't exist, then just use last 4 bits of b2 combined with 2 '0' bits, which is just 
                // last 4 bits of b2 shifted to left 2 times
                encoded[encodedIndex++] = lookupTable[(int)b2_2];
                encoded[encodedIndex++] = '=';
            }
        } else {
            // just use last 2 bits of b1 shifted 4 times
            encoded[encodedIndex++] = lookupTable[(int)b1_2];
            encoded[encodedIndex++] = '=';
            encoded[encodedIndex++] = '=';
        }
    }

    encoded[encodedSize] = '\0';
    *eSize = encodedSize;
    return encoded;
}

uint8_t *base64decode(char *input, unsigned long size, unsigned long *dSize) {
    if (size < 2 || input == NULL || input[size] != '\0') {
        return NULL;
    }

    unsigned long ds = decodedSize(input, size);
    if (ds == 0) {
        return NULL;
    }

    uint8_t *decoded = malloc(ds * sizeof(uint8_t));
    unsigned int decodedIndex = 0;
    for (unsigned int i = 0; i < size; i += 4) {
        int8_t c1 = charToInt(input[i]);
        int8_t c2 = charToInt(input[i+1]);
        if (c1 == -1 || c2 == -1) {
            free(decoded);
            return NULL;
        }

        uint8_t c1_8 = (uint8_t)c1;
        uint8_t c2_8 = (uint8_t)c2;
        // Combine the 6 bits from c1 with first 2 bits from c2 to get first decoded byte
        decoded[decodedIndex++] = (uint8_t)((c1_8 << 2) | (3 & (c2_8 >> 4)));

        // break if ending in '==' or '==' is missing
        if (i+2 >= size || input[i+2] == '=') {
            break;
        }

        int8_t c3 = charToInt(input[i+2]);
        if (c3 == -1) {
            free(decoded);
            return NULL;
        }

        uint8_t c3_8 = (uint8_t)c3;
        // Combine last 4 bits of c2 with first 4 bits of c3 to get second decoded byte
        decoded[decodedIndex++] = (uint8_t)(((c2_8 & 15) << 4) | (c3_8 >> 2));

        // break if ending in '=' or '=' is missing
        if (i+3 >= size || input[i+3] == '=') {
            break;
        }

        int8_t c4 = charToInt(input[i+3]);
        if (c4 == -1) {
            free(decoded);
            return NULL;
        }

        uint8_t c4_8 = (uint8_t)c4;
        // Combine last 2 bits of c3 with all 6 bits of c4 to get third decoded byte
        decoded[decodedIndex++] = (uint8_t)(((c3_8 & 3) << 6) | c4_8);
    }    

    *dSize = ds;
    return decoded;
}

/*
 * Converts the input base64 character to it's byte output value.
 * Returns -1 if it's an invalid char
 */
int8_t charToInt(char c) {
    if (c >= 65 && c <= 90) {
        return (int8_t)(c - 65);
    }

    if (c >= 97 && c <= 122) {
        return (int8_t)(c - 71);
    }

    if (c >= 48 && c <= 57) {
        return (int8_t)(c + 4);
    }

    if (c == '+') {
        return 62;
    }

    if (c == '/') {
        return 63;
    }

    if (c == '=') {
        return 0;
    }

    return -1;
}

/*
 * Returns length of decoded byte array given encoded input string. This accounts for missing padding characters in the input.
 * Returns 0 if input is an invalid size
 * */
unsigned long decodedSize(char *input, unsigned long size) {
    unsigned long decodedSize = size / 4 * 3;

    char l1 = input[size-1];
    char l2 = input[size-2];

    if (l1 == '=' && l2 == '=') {
        return decodedSize - 2;
    } else if (l1 == '=') {
        return decodedSize - 1;
    }

    if (size % 4 == 0) {
        return decodedSize;
    }

    if (size % 4 == 2) {
        return decodedSize + 1;
    }

    if (size % 4 == 3) {
        return decodedSize + 2;
    }

    return 0;
}
