SRC = ..
CFLAGS = -std=c99 -g -ggdb3 -W -Wall -Wno-sign-compare

all: printf_test strtod_test

runall: all
	./printf_test
	./strtod_test

printf_test: printf_test.c $(SRC)/printf.c $(SRC)/printf.h
	$(CC) $(CFLAGS) -I$(SRC) printf_test.c $(SRC)/printf.c -o printf_test

strtod_test: strtod_test.c $(SRC)/strtod.c $(SRC)/strtod.h $(SRC)/ctype.h
	$(CC) $(CFLAGS) -I$(SRC) strtod_test.c $(SRC)/strtod.c $(SRC)/printf.c -lm -o strtod_test

clean:
	rm -f printf_test strtod_test
