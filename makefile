CC = gcc
CFLAGS = -Wall -g -std=c99
LDFLAGS= -L.

INC = include/
SRC_MOD = src/modules/
SRC = src/
BIN = bin/


main: $(BIN)byteStream.o main.c
	$(CC) $(CFLAGS) -I$(INC) $(BIN)byteStream.o main.c -o main

$(BIN)byteStream.o: $(SRC)byteStream.c
	$(CC) $(CFLAGS) -I$(INC) $(SRC)byteStream.c -fpic -c -o $(BIN)byteStream.o


clean:
	rm -rf $(BIN)*.o $(BIN)*.so main *.out main
