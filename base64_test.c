#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "base64.h"

void testEncode(char *input, char *output);
void testDecode(char *input, uint8_t *output, unsigned long expectedSize);

int main() {
    
    testEncode("M", "TQ==");
    testEncode("Ma", "TWE=");
    testEncode("Man", "TWFu");
    testEncode("Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.", "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlzIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGludWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=");

    testDecode("TQ==", (uint8_t *)"M", 1);
    testDecode("TQ", (uint8_t *)"M", 1); // missing padding
    testDecode("TWE=", (uint8_t *)"Ma", 2);
    testDecode("TWE", (uint8_t *)"Ma", 2); // missing padding
    testDecode("TWFu", (uint8_t *)"Man", 3);
    testDecode("TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlzIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGludWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=", (uint8_t *)"Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.", 269);

    // invalid inputs
    testDecode("T", NULL, 0);
    testDecode("TWFu=", NULL, 0);
    return 0;
}

void testEncode(char *input, char *output) {
    unsigned long eSize;
    char *encodedValue = base64encode((uint8_t *)input, strlen(input), &eSize);
    assert(strcmp(encodedValue, output) == 0);
}

void testDecode(char *input, uint8_t *output, unsigned long expectedSize) {
    unsigned long dSize;
    uint8_t *decodedValue = base64decode(input, strlen(input), &dSize);
    if (output == NULL) {
        assert(decodedValue == NULL);
        return;
    }

    assert(dSize == expectedSize);
    int equal = 1;
    for (unsigned int i = 0; i < dSize; i++) {
        if (decodedValue[i] != output[i]) {
            equal = 0;
            break;
        }
    }

    assert(equal);
}
