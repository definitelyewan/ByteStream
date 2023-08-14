CC = gcc
CFLAGS = -Wall -std=c99 -fpic
LDFLAGS = -L.

INC = include/
SRC_MOD = src/modules/
SRC = src/
BIN = bin/
TEST = test/
TEST_BIN = test/bin/
BENCH = benchmark/
BENCH_BIN = benchmark/bin/

BYTE_SRC_FILES = $(wildcard src/byte*.c) $(wildcard src/modules/byte*.c)
BYTE_OBJ_FILES = $(patsubst src/byte%.c,bin/byte%.o,$(BYTE_SRC_FILES)) $(patsubst src/modules/byte%.c,bin/byte%.o,$(BYTE_SRC_FILES))

BYTE_TEST_SRC_FILES = $(wildcard test/byte*.c)
BYTE_TEST_EXES = $(patsubst test/byte%.c,test/bin/%,$(BYTE_TEST_SRC_FILES))

BYTE_BENCH_SRC_FILES = $(wildcard benchmark/byte*.c)
BYTE_BENCH_EXES = $(patsubst benchmark/byte%.c,benchmark/bin/%,$(BYTE_BENCH_SRC_FILES))

all: shared static main test bench

#test and bench code
bt: test bench

main: $(BIN)libbstream.a main.c
	$(CC) $(CFLAGS) -I$(INC) -L$(BIN) main.c -o main -lbstream

bench: static $(BYTE_BENCH_EXES)
	for benchmark in $(BYTE_BENCH_EXES) ; do valgrind ./$$benchmark ; done

test: static $(BYTE_TEST_EXES)
	for test in $(BYTE_TEST_EXES) ; do ./$$test ; done

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

$(BENCH_BIN)%: $(BENCH)byte%.c $(BIN)libbstream.a
	$(CC) $(CFLAGS) -I$(INC) $< -o $@ -L$(BIN) -lbstream

clean:
	rm -rf $(BIN)*.o $(BIN)*.a $(BIN)*.so $(TEST_BIN)*Functions $(BENCH_BIN)*Bench main *.out vgcore.*
