# base64
Small C library for base64 encoding and decoding

There are 3 parts to this:
* base64.c - runner that takes an input file and base64 encodes the contents, and then outputs result to stdout.
* base64_lib.c - the base64 encoding/decoding library itself
* base64_test.c - unit tests for encoding and decoding

To build the runner and libarary, run `make`

To build the unit test, run `make test`
