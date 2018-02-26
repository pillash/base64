/*
 * Base64 encodes the input byte array. Uses the standard character set, including '/' and '+' and includes padding at the end if needed.
 * input: array of bytes
 * size: length of input
 * eSize: pointer to a place to store the final encoded string's length
 * returns: pointer to string. Free this pointer when done using it. Returns NULL if there was an error.
 */
char *base64encode(uint8_t *input, unsigned long size, unsigned long *eSize);

/*
 * Base64 decodes the input string. Expects the standard char set including '/' and '+'. Padding characters at the end are not required.
 * input: base64 encoded string
 * size: length of input
 * dSize: pointer to a place to store the decoded array's length
 * returns: array of bytes. Free this pointer when done using it. Returns NULL if input is malformed or there was some error.
 */
uint8_t *base64decode(char *input, unsigned long size, unsigned long *dSize);
