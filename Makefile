CC=gcc
CFLAGS=-Werror -Wall -Wextra -O2

OBJS=base64_lib.o

base64: $(OBJS) base64.o
	$(CC) $(CFLAGS) -o base64 $(OBJS) base64.o

test: $(OBJS) base64_test.o
	$(CC) $(CFLAGS) -o base64_test $(OBJS) base64_test.o

base64.o: base64.c base64.h
	$(CC) $(CFLAGS) -c base64.c
	
base64_lib.o: base64_lib.c base64.h
	$(CC) $(CFLAGS) -c base64_lib.c

base64_test.o: base64_test.c base64.h
	$(CC) $(CFLAGS) -c base64_test.c

clean:
	rm -f *~ *.o base64 base64_test
