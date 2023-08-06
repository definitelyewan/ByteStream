CC = gcc
CFLAGS = -Wall -g -std=c99 -fpic
LDFLAGS = -L.

INC = include/
SRC_MOD = src/modules/
SRC = src/
BIN = bin/
TEST = test/
TEST_BIN = test/bin/

BYTE_SRC_FILES = $(wildcard src/byte*.c) $(wildcard src/modules/byte*.c)
BYTE_OBJ_FILES = $(patsubst src/byte%.c,bin/byte%.o,$(BYTE_SRC_FILES)) $(patsubst src/modules/byte%.c,bin/byte%.o,$(BYTE_SRC_FILES))

BYTE_TEST_SRC_FILES = $(wildcard test/byte*.c)
BYTE_TEST_EXES = $(patsubst test/byte%.c,test/bin/%,$(BYTE_TEST_SRC_FILES))

all: shared static main test

main: $(BIN)libbstream.a main.c
	$(CC) $(CFLAGS) -I$(INC) -L$(BIN) main.c -o main -lbstream

test: static $(BYTE_TEST_EXES)
	for test in $(BYTE_TEST_EXES) ; do valgrind --leak-check=full --show-leak-kinds=all ./$$test ; done

#static lib
static: $(BIN)libbstream.a

$(BIN)libbstream.a: $(BYTE_OBJ_FILES)
	ar rcs $@ $^

#shared lib
shared: $(BIN)libbstream.so

$(BIN)libbstream.so: $(BYTE_OBJ_FILES)
	$(CC) $(CFLAGS) -shared -o $(BIN)libbstream.so $(BIN)byteUnicode.o $(BIN)byteStream.o


#object files
$(BIN)%.o: $(SRC)%.c $(INC)%.h
	$(CC) $(CFLAGS) -I$(INC) -c $< -o $@

$(BIN)%.o: $(SRC_MOD)%.c $(INC)%.h
	$(CC) $(CFLAGS) -I$(INC) -c $< -o $@

$(TEST_BIN)%: $(TEST)byte%.c $(BIN)libbstream.a
	$(CC) $(CFLAGS) -I$(INC) $< -o $@ -L$(BIN) -lbstream -lcmocka

clean:
	rm -rf $(BIN)*.o $(BIN)*.a $(BIN)*.so $(TEST_BIN)*Functions main *.out
